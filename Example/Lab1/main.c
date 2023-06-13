#include "stm32f0xx.h"


//функция задержки
//предположительно дожна быть около 1сек
void delay ()
{
  unsigned long Cl=4800000; 
  while(Cl>0) Cl--;
	
}


int main(void)
{
  //подаем тактовую частоту на  GPIOC
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;  
	//Делаем выходами нужные выводы порта С
	GPIOC->MODER|=(GPIO_MODER_MODER8_0|GPIO_MODER_MODER9_0); 	

	while(1)
	{
 	 delay();
   GPIOC->ODR |= GPIO_ODR_8; //устанавливаем выводы в 1
	 GPIOC->ODR |= GPIO_ODR_9;
   delay();
   GPIOC->ODR &= ~GPIO_ODR_8; //устанавливаем выводы в 0
   GPIOC->ODR &= ~GPIO_ODR_9;
	}
 
}

