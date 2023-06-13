#include "stm32f0xx.h"
#include "initIO.h"
#include "i2c.h"
#include "display.h"
#include "functions.h"
#include "adc.h"

#define viborka  1024 //Число замеров АЦП
uint16_t adcData[2];//Буфер значений AЦП
uint32_t Usumma[2];  //Сумма за х преобразований
uint16_t adci=0;  //Сколько выборок было сделано
uint16_t U[2];  //значение напряжений на основе усреднения многих выборок
uint16_t adcOk=0; //флаг того что данные АЦП готовы для использования
uint16_t TFT[2];//значения напряжения для вывода на экран
uint16_t tmp;
uint8_t i;

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
	//инициализирую ADC и DMA
	init_ADC(adcData);

	Usumma[0]=0;Usumma[1]=0;
while(1)
{
 
//Если АЦП выполнил все преобразования,DMA говорит о том что данные приняты
//DMA1_FLAG_TC1: DMA1 Channel1 transfer complete flag
if (DMA_GetFlagStatus(DMA1_FLAG_TC1)==1)
  {		
	  //очистка флага
	 // DMA1->ISR&= ~DMA_ISR_TCIF1 ; //Сбрасываем флаг 		    
	  if (adci==viborka)
		 {
			 U[0]=Usumma[0]/viborka;  //Усредняю
			 U[1]=Usumma[1]/viborka;
		   Usumma[0]=0;Usumma[1]=0; //Обнуляю
       adci=0; 				
		   adcOk=1;  //говорю что данные готовы для использования
     }  
    else
     { //Суммируем значения
	     Usumma[0]=Usumma[0]+adcData[0];
		   Usumma[1]=Usumma[1]+adcData[1];  
			 adci++;	
      }   				 											
				// очистка флага
     DMA1->ISR&= ~DMA_ISR_TCIF1 ; // Сбрасываем флаг
	} 	
   //вывожу на экран значения
	 //U=ADCcode*3.3/4095
	  if(adcOk==1)
		{			
  	  for(i=0;i<2;i++)
			{ 
				//вычисляю напряжение
			  tmp=U[i]*330/0x0fff;
				//вывод на экран
			  showhex(64,i,tmp/100);
				Litera(69,i,'.',1);
				showhex(74,i,tmp%100);
				Litera(90,i,'V',1);
			}
			adcOk=0;//сброс флага
		}


 }

}
