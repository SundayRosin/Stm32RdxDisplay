#include "initIO.h"
#include "i2c.h"
#include "display.h"

uint8_t i,j;
uint8_t str[][10]=
{
  {242,242,242,242,242,242},
  {242,242,242,242,242,242,242,242},
  {242,242,242,242,242,242,242,242,242,242}	

};

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
	
  ClearLCD(0);//очищаею дисплей
  Litera(96,7,'5',1);//вывожу символ	 
	//вывожу ФИО
	for(i=1;i<4;i++)
	{ pstring(64,i,str[j++]);	}	   
	while(1){;};		
}