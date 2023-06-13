#include "stm32f0xx.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"
#include "i2c.h"
#include "display.h"
#include <stdio.h>

uint8_t i;

int main(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
 
//подаем тактовый сигнал на  GPIOВ,GPIOC  
RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);  
//Настраиваем PB8(RST) как выход
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
//Настраиваем PC8(RST) как выход
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);   
  //включаем альтернативные функции выводов SDA,SCL 
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_1);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_1);
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  //подтягиваем резисторы к +,открытый сток,задем скорость
  //выводов	
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  i2c_init();//инициализируем i2C	
	//Выводим дисплей  из состояния reset
  GPIOB->BSRR = GPIO_BSRR_BS_8; //PB8=1     
	delay_ms(300);
  init_display();//настраиваю дисплей	
	delay_ms(100);	
	//зажигаю синий светодиод на плате
   GPIOC->BSRR = GPIO_BSRR_BS_8; 
	  
	ClearLCD(0);//очищаею дисплей
 	ShowPixel(10,10,1);//отображаю пиксель
	Hline(0,132,32); // рисую линию
 	//рисую круги разного диаметра
	 for(i=0;i<4;i++)
 	 {
	  Circle(65,32,i*10,1);
   }
   while(1){;};		
}