#include "stm32f0xx.h"
#include "initIO.h"
#include "i2c.h"
#include "display.h"
#include "functions.h"
#include "onewire_usart.h"


#define OW_0    0x00 //установка шины в 0
#define OW_1    0xff //установка шины в 1
#define OW_R    0xff //сброс 

//команда запуска конвертирования температуры 
//0xcc =11001100;0x44=1000100; биты передаются LB..HB 
const uint8_t convert_t[] = {
                OW_0, OW_0, OW_1, OW_1, OW_0, OW_0, OW_1, OW_1, // 0xcc SKIP ROM
                OW_0, OW_0, OW_1, OW_0, OW_0, OW_0, OW_1, OW_0  // 0x44 запуск п-я
};

//инициализация, чтение устройства
const uint8_t read_device[] = {
                OW_0, OW_0, OW_1, OW_1, OW_0, OW_0, OW_1, OW_1, // 0xcc SKIP ROM
                OW_0, OW_1, OW_1, OW_1, OW_1, OW_1, OW_0, OW_1, // 0xbe READ SCRATCH
                OW_R, OW_R, OW_R, OW_R, OW_R, OW_R, OW_R, OW_R,
                OW_R, OW_R, OW_R, OW_R, OW_R, OW_R, OW_R, OW_R
};


 
//буфер куда будут считываться данные с датчика
uint8_t dev_buff[32];
uint16_t t=0;//температура
uint32_t i;

int main(void)
{
  initIO();//инициализация портов
  i2c_init();//инициализируем i2C	
	//Выводим дисплей  из состояния reset
  GPIOB->BSRR = GPIO_BSRR_BS_8; //PB8=1     
	delay_ms(300);
  init_display();//настраиваю дисплей	
	delay_ms(100);	
	//зажигаю синий светодиод на плате
   GPIOC->BSRR = GPIO_BSRR_BS_8; 
  	delay_ms(1000);
	GPIOC->BRR =GPIO_BRR_BR_8;	
  delay_ms(1000); 
	
	ClearLCD(0);//очищаею дисплей
	ow_init();//инициализация шины

while(1)
{  
	ow_reset();
	ow_write(convert_t,16);
  delay_ms(5000);//~1c  
  ow_reset(); 
 	ow_read(read_device,dev_buff,32); 

   //собираю в uint16_t 
   t=0;  
	for (i=16;i<32; i++)
	 {
    if (dev_buff[i] == 0xff)
    {t =(t>>1)|0x8000;}
		else {t=t>>1;}
   }
    
    //вывожу код с датчика
     showhex(60,0,t);
	  //знак, бит стращего разр
	   if((t&0x8000)==0)
		 {Litera(60,1,'+',1); }
     else
	  { 			 
      Litera(60,1,'-',1);
      t = ~t+1;//преобр в полож 
    };  		 
		//выводим целую часть температуры
		  showhex(67,1,t/16);
		 Litera(79,1,'.',1);//точка
		//получаю дробную часть
		t&=0x0F;
		 //преобразуем в целое число
    t=(t<<1)+(t<<3);// Умножаем на 10
    t=(t>>4);//делим на 16 или умножаем на 0.0625 
		showhex(84,1,t); 
		Litera(95,1,191,1);//грудус
}	 
		
}