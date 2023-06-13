#include "stm32f0xx.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"
#include "1w.h"



//инициализирует порт
void ow_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd(OWI_GPIO_CLK, ENABLE);
	
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(OWI_GPIO, &GPIO_InitStructure);  
}

//задержка мкс
void delay(uint32_t cycles)
{
	int cn,z; 
	for(cn=0;cn<cycles;cn++);  	 
	
}

//генерирует сигнал начала посылки
void set_presence(void)
{
	delay(100);
	OWI_Low_Bit; //ставим нуль	
  delay(3500); //420us	
	OWI_Hi_Bit; 	
	
}

//отправляет бит
void ow_write_bit(uint8_t bit)
{
		OWI_Low_Bit; //нуль на шине
	 if(bit==1){delay(150);}
		else {delay(500);} 
	  OWI_Hi_Bit; //возвращаем шину в исходное состояние	
	 if(bit==1){delay(650);}
		else {delay(200);} 
	  
}


uint8_t ow_read_bit()
{
	uint8_t bit = 0;
	OWI_Low_Bit; //нуль на шине
	delay(80);
	OWI_Hi_Bit; //возвращаем шину в исходное состояние	
	delay(50);
	OWI_Set_Input; //вывод шины вход
	
	if((GPIOA->IDR&OWI_PIN)>0){bit=1;}
  else {bit=0;} 	

	OWI_Set_Out;
	delay(600);
	return bit;
}

//отправляет байт
void ow_write_byte(uint8_t data)
{
	uint8_t i; 
	for(i = 0; i<8; i++)
  ow_write_bit(data>>i & 1);
}

