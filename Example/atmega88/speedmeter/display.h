#define F_CPU 8000000UL	// определение тактовой частоты для макросов __delay_ms() и __delay_us()
#include <util/twi.h>
#include <avr/pgmspace.h> // для того что бы масив был в flash памяти  //http://www.nongnu.org/avr-libc/user-manual/pgmspace.html
#include <Util/delay.h>

//Нужно на случай если кто то забыл повесить на I2C резисторы в 10к,подтягивает внутренние резисторы.

#define i2c_PORT	PORTC				// Порт где сидит нога TWI
#define i2c_DDR		DDRC
#define i2c_SCL		5				// Номера соответствующих выводов
#define i2c_SDA		4

//Инициализация дисплея
void init_display(void);

//Основная функция которая выводит все на экран.

// 0(first_column)-номер начального столбца с которого выводится изображение
// 1(x_size)-ширина разряда в пикселях, или размер поля для символа по x координате
// 2 (nomber_razrad) -cколько разрядов будем выводить на экран
// 3 (first_page) -начальная страница памяти для вывода изображения
//4 (end_page)- конечная страница памяти для вывода изображения
//5 (data_array[]) -массив данных, содержащих номер в массиве знакогенератора, для вывода на экран
// работает только для маленьких символов 10х16, и содержащих не более 2х запятых в строке.
//6 (where_point) после какого разряда поставим запятую? если стоит цифра больше самих разрадов, > nomber_razrad то запятых не будет
//7 (where_point_one) вторая запятая.
//8 (point_big_on) если point_big_on=1 значит в больших разрядах будет светится точка,если point_big_on=0 точки нет.


void send_pixel(unsigned char first_column,  unsigned char x_size,unsigned char nomber_razrad,  unsigned char first_page,unsigned char end_page,unsigned char data_array[],
unsigned char where_point,unsigned char where_point_one,unsigned char point_big_on);