/*
 * test_i2c.c
 *
 * Created: 24.06.2013 14:24:23
 */
#define F_CPU 8000000UL	// определение тактовой частоты для макросов __delay_ms() и __delay_us()

#include <avr/io.h>  //подключаем библиотеку ввода вывода
#include <avr/interrupt.h>  //подключаем библеотеку прерываний
#include <Util/delay.h>
#include "display.h"

//подключение заголовочных файлов 1Wire библиотеки
#include "OWIPolled.h"
#include "OWIHighLevelFunctions.h"
#include "OWIBitFunctions.h"
#include "common_files\OWIcrc.h"

//Библеотека для работы  с еепром
#include "eeprom.h"

#define BUS OWI_PIN_1
//команды датчика DS18B20
#define DS18B20_SKIP_ROM            0xcc
#define DS18B20_CONVERT_T           0x44
#define DS18B20_READ_SCRATCHPAD     0xbe

unsigned char counter_t0 = 0;
unsigned char LCD_buffer[4]; //Буфер для вывода 3х 10тичных знаков на экран 
unsigned long ICR_buffer = 0, speed;
const unsigned long speed_const = 28712160;
unsigned short t0_overflow = 0;
unsigned char speed_measure_flag = 0;
unsigned char adc_buffer = 0, taxo_buffer = 0;
unsigned short t2_overflow = 0, t2_overflow_buffer = 0;
unsigned short taxo = 0;

unsigned char counter_termo = 0; //Счетчик времени с которым опрашивать датчик температуры
unsigned char temperature[3] = { 0,0,0 }; //Значение температуры целое и после точки,отрицательная/положительная
unsigned char termo_data[2] = { 0,0 }; //данные с датчика	 
unsigned int tmp = 0; //Переменная для работ с отрицательной температурой

unsigned char probeg[9]; //массив для хранения значения пробега 8-3 км,2-сотни метров,1-десятки,0-единицы,значек км
unsigned char dlina = 0;
unsigned short probeg_hex = 0;
unsigned char odometr[3] = { 0,0,0 };;

//Перевод двоичных в десятичное
void byte_to_ten(unsigned short x)
{
	LCD_buffer[2] = 0;
	LCD_buffer[1] = 0;
	LCD_buffer[0] = 0;
	LCD_buffer[3] = 0;

	while (x >= 1000)
	{
		x -= 1000;
		LCD_buffer[3]++;
	}

	while (x >= 100)
	{
		x -= 100;
		LCD_buffer[0]++;
	}

	while (x >= 10)
	{
		x -= 10;
		LCD_buffer[1]++;
	}

	while (x >= 1)
	{
		x -= 1;
		LCD_buffer[2]++;
	}
};

//Прерывание по переполнению T0
ISR(TIMER0_OVF_vect)
{
	unsigned char point_termo;
	asm("cli");
	//30.63 "гц" приходит прирывание ,каждые 0.0328с

    //"Делитель" на     //Каждые 0.49сек будем выводить на дисплей
	if (counter_t0 == 15)
	{
		counter_t0 = 0;
		if ((ICR_buffer != 0) & (speed_measure_flag == 1))
		{
			speed_measure_flag = 0;
			speed = speed_const / ICR_buffer;
		}
		else
		{
			speed = 0;
		}
		byte_to_ten(speed);
		//unsigned char jon[4]={4,4,5,10};

		LCD_buffer[3] = 10;
		send_pixel(0, 32, 4, 0, 3, LCD_buffer, 5, 5, 1);

		//Выводим значение напряжения
		/*
		  Vin=(ADC*Vref)/256;Vref=3.3v делитель 12к 3.3к
		  Vin =(ADC*15.312)/256 ~ = (ADC*153)/256
		*/
		unsigned short volt;

		volt = adc_buffer * 153;
		if (adc_buffer != 0)
		{
			volt = volt / 256;
		}
		else
		{
			volt = 0;
		};

		byte_to_ten(volt);

		unsigned char kilo[4] = { LCD_buffer[0],LCD_buffer[1],LCD_buffer[2],11 };
		send_pixel(50, 10, 4, 6, 7, kilo, 15, 1, 0);

		//выводим значение тахометра	  
		if ((t2_overflow_buffer == 7812) | ((taxo_buffer == 0) & (t2_overflow_buffer == 0)))
		{
			taxo = 0;
		}
		else
		{
			if (taxo_buffer > 128) { t2_overflow_buffer++; };

			taxo = 39063 / t2_overflow_buffer;
			taxo = taxo * 6;
			t2_overflow_buffer = 0;
			taxo_buffer = 0;
		}

		byte_to_ten(taxo);
		unsigned char tax[5] = { LCD_buffer[3],LCD_buffer[0],LCD_buffer[1],LCD_buffer[2],10 };

		send_pixel(0, 10, 5, 6, 7, tax, 15, 15, 0);

		//Выводим значение температуры на дисплей
		unsigned char termo_disp[3];

		byte_to_ten(temperature[0]);

		//Если температура положительна
		if (temperature[2] == 0)
		{
			//Если целая часть температуры < 100градусов
			if (temperature[0] < 100)
			{
				//Заносим целую часть в буфер
				termo_disp[0] = LCD_buffer[1];
				termo_disp[1] = LCD_buffer[2];

				//Выводим дробную часть
				byte_to_ten(temperature[1]);

				termo_disp[2] = LCD_buffer[2];
				point_termo = 1;//отображаем десятичную точку

			}
			//если >100град отображаем только целую часть
			else
			{
				termo_disp[0] = LCD_buffer[0];
				termo_disp[1] = LCD_buffer[1];
				termo_disp[2] = LCD_buffer[2];
				point_termo = 20; //отключаем точку в разряде

			};
		}
		else
		{//Температура отрицательна

			termo_disp[0] = 14;//Выводим знак минус
			termo_disp[1] = LCD_buffer[1];
			termo_disp[2] = LCD_buffer[2];
			point_termo = 20; //отключаем точку в разряде
		};

		send_pixel(93, 10, 3, 6, 7, termo_disp, 15, point_termo, 0);

		//Выводим значение пробега

        // unsigned char giga[12]={0,3,2,7,8,4,13,20,0,0,0};
		probeg[6] = 13;//значек км
		send_pixel(5, 10, 7, 4, 5, probeg, 4, 9, 0);

		//Выводим показания одометра
		//точка 3 пикселя
		send_pixel(78, 10, 3, 4, 5, odometr, 1, 9, 0);
	}
	else
	{
		counter_t0++;
	};

	asm("sei");
}


//Обработка прирывания по ICP
//Полезно почитать как работает захват http://we.easyelectronics.ru/AVR/timers-capture-mode-ili-pravilnaya-rabota-s-taymerom-v-rezhime-zahvata.html 
ISR(TIMER1_CAPT_vect)
{
	asm("cli");

	TCNT1 = 0; 	//обнуление счетчика TCNT1
	speed_measure_flag = 1;

	if (t0_overflow == 0)
	{
		ICR_buffer = ICR1;
	}
	else
	{
		ICR_buffer = 65535 * t0_overflow + ICR1;
		t0_overflow = 0;
	}

	// инкремент счетчика метров
	dlina++;
	asm("sei");
}

ISR(TIMER1_OVF_vect)
{
	asm("cli");
	t0_overflow++;
	asm("sei");
};

//Прерывание от АЦП
ISR(ADC_vect)
{
	asm("cli");
	adc_buffer = ADCH; //сохранили значение
	asm("sei");
}

//Внешнее прерывание от тахометра
ISR(INT0_vect)
{
	asm("cli");
	//Если прерывание T2 разрешено
	if (TIMSK2 == 0x01)
	{
		//Останавливаем счетчик
		TIMSK2 = 0;
		taxo_buffer = TCNT2; //cохраняем значения
		TCNT2 = 0;
		t2_overflow_buffer = t2_overflow;
		t2_overflow = 0;

		TIMSK2 = 0x01; //запускаем счетчик
	}
	else
	{ //если первый раз тут

		TIMSK2 = 0x01; //запускаем счетчик
	}

	asm("sei");
}

//Переполнение T2
ISR(TIMER2_OVF_vect)
{
	asm("cli");
	t2_overflow++; //сохранили значение
	asm("sei");
}

//Анализ еепром,и выводы о дальнейших действиях.
void init_eeprom(void)
{
	unsigned char i;

re_read:
	//Читаем значение "пробега"
	for (i = 0;i < 8;i++)
	{
		probeg[i] = ee_read(i);

	};

	//Если только что зашили прошивку
	if (probeg[3] == 0xff)
	{
		//Записываем нули во все ячейки
		for (i = 0;i < 8;i++)
		{
			ee_write(i, 0);

		}

		goto re_read;
	};
}

//Вычисление пробега
void measure_probeg(void)
{
	//x=число пробега 
	unsigned char temp = 0, i;

	/*
	 while (probeg_hex>=1000)
	 {
		 probeg_hex=probeg_hex-1000;
		 temp++; //Высчитываем сколько тысячь у нас набежало, с момента последнего входа сюда.
	 };
	 */

	while (probeg_hex >= 100)
	{
		probeg_hex = probeg_hex - 100;
		temp++;
	}

	probeg[5] = probeg[5] + temp; // "Перекручиваем" счетчик cотен на temp единиц
	odometr[2] = odometr[2] + temp;

	temp = 0;

	if (odometr[2] == 10) { odometr[2] = 0;odometr[1]++; }
	if (odometr[1] == 10) { odometr[1] = 0;odometr[0]++; }
	if (odometr[0] == 10) { odometr[0] = 0; };

	//Если сотни метров =10
	if (probeg[5] == 10)
	{
		probeg[5] = 0;
		probeg[4]++;
		//Если единицы километров =10

		if (probeg[4] == 10)
		{
			i = 4;
			while (i != 0)
			{
				if (probeg[i] == 10)
				{
					probeg[i] = 0;

					if (i != 0)
					{
						probeg[i - 1]++;
					}
					else
					{
						probeg[0] = 0;
					};//Превышение разрядной сетки
				};
				i--;
			}
		}
	}
}

int main(void)
{
	//Анализ еепром,и выводы о дальнейших действиях.
	init_eeprom();

	//Инициализация T0
	TCCR0B = 0b00000101; //Предделитель на 1024
	TIMSK0 = 0b00000001;  //Разрешили прерывания

	//Установка Т1
	// режим ICP
	// тактовый сигнал Fclk=Fo/8

	TCCR1A = 0b00000000;
	// TCCR1B
		 // ICNC1-ICES1-X-WGM13-WGM12-CS12-CS11-CS10
	TCCR1B = 0b01000010;//Input capture noise canceler отключен
	TIMSK1 = 0b00100001;// Разрешить прерывания input capture interrupt, и переполнения таймера

	//Инициализация TC2
	TCCR2A = 0;

	// FOC2A-FOC2B-  -       -WGM22-CS22-CS21-CS20
	//   0    0     0     0     0     1    0    0
	TCCR2B = 0b00000010;
	//Fclk=Fo/8

//Настройка внешнего прерывания INT0

/*
Внешнее прерывание может происходить по одному из условий:
– по низкому уровню на выводах INT0, INT1,
– по любому изменению логического уровня на выводах INT0, INT1
– по спадающему фронту сигнала на выводах INT0, INT1,
– по нарастающему фронту на выводах INT0, INT1, INT2.
*/

	EICRA = 0b00000010; //по спадающему фронту из 1 в 0
	EIMSK = 0b00000001; //разрешено прерывание INT0

	//Настройка АЦП

		   //ADC7-порт для измерения напряжения
		   //ADLAR=1;
		   //AVCC with external capacitor at AREF pin;3.3v
		   // REFS1-REFS0-ADLAR-MUX4-MUX3-MUX2-MUX1-MUX0
	ADMUX = 0b01100111;

	//ADSC=1; In Free Running mode, write this bit to one to start the first conversion
	//ADATE=0;ADC auto trigger enable
	//ADIE=1: ADC interrupt enable
	//Делитель на 128, тактовая 62500
   //ADEN-ADSC-ADATE-ADIF-ADIE-ADPS2:ADPS0
	ADCSRA = 0b11001111;

	//Отключаем аналоговый компаратор
	//Bit 7 – ACD: Analog comparator disable
	ACSR = 0b10000000;

	//Инициализация 1 wire
	OWI_Init(BUS);

	asm("sei");
	init_display();

	unsigned char jon[4] = { 4,4,5,10 };

	jon[0] = 0; jon[1] = 1;
	send_pixel(121, 6, 1, 4, 5, jon, 10, 10, 0);

	jon[0] = 1;
	send_pixel(126, 6, 1, 6, 7, jon, 10, 10, 0);

loop:
	//Работа с термодатчиком

	OWI_DetectPresence(BUS); //подаем сигнал сброса
	OWI_SkipRom(BUS); //подаем    команду для адресации всех устройств на шине
	OWI_SendByte(DS18B20_CONVERT_T, BUS);//запук преобразования

	//ждем, когда датчик завершит преобразование
	while (!OWI_ReadBit(BUS));

	OWI_DetectPresence(BUS);  //подаем сигнал сброса
	OWI_SkipRom(BUS); //подаем    команду для адресации всех устройств на шине
	OWI_SendByte(DS18B20_READ_SCRATCHPAD, BUS);  //команду - чтение внутренней памяти

	termo_data[0] = OWI_ReceiveByte(BUS); // считываем внутреннюю память датчика в массив
	termo_data[1] = OWI_ReceiveByte(BUS);


	//выводим знак и преобразуем число, если оно отрицательное
	//Если значение температуры отрицательное число
	if ((termo_data[1] & 128) != 0) {

		// «склеиваем» нулевой и первый байты ОЗУ датчика
		tmp = ((unsigned int)termo_data[1] << 8) | termo_data[0];
		//выполняем операцию логического отрицания
		tmp = ~tmp + 1;
		//помещаем результат в соответствующие переменные
		termo_data[0] = tmp;
		termo_data[1] = tmp >> 8;
		temperature[2] = 1; //Говорим о том что нужно выводить знак минус 
	}
	else
	{ //температура положительная

		temperature[2] = 0;// Температура положительна
	};

	//Значение целого числа температуры
	temperature[0] = (termo_data[0] >> 4) | ((termo_data[1] & 7) << 4);

	//Значение дробной части температуры
	//выделяем с помощью битовой маски дробную часть
	temperature[1] = (termo_data[0] & 15);

	//преобразуем в целое число
	temperature[1] = (temperature[1] << 1) + (temperature[1] << 3);// Умножаем на 10
	temperature[1] = (temperature[1] >> 4);//делим на 16 или умножаем на 0.0625

	//Cчитаем пробег	 
	if (dlina >= 100)
	{
		probeg_hex = probeg_hex + 80;
		dlina = dlina - 100;
	};

	measure_probeg();
	goto loop;
}