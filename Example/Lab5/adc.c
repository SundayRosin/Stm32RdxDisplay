#include "stm32f0xx.h"
#include "stm32f0xx_conf.h"


//Функция инициализации ADC
void init_ADC(uint16_t *buf)
{
  ADC_InitTypeDef          ADC_InitStructure;
	DMA_InitTypeDef          DMA_InitStructure;
	RCC->AHBENR |= (RCC_AHBENR_GPIOAEN);  ////подаем тактовую частоту на  GPIOA 
	 //Для АЦП
	GPIOA->MODER |= GPIO_MODER_MODER5;  ;//PA5 ф-я аналоговый вход.
	GPIOB->MODER |= GPIO_MODER_MODER1;  ;//PB1 ф-я аналоговый вход.  
	
		/* Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);

	ADC1->CFGR2|=ADC_CFGR2_JITOFFDIV4 ;//12mhz clock
	/* Initialize ADC structure */
	ADC_StructInit(&ADC_InitStructure);
	/* Configure the ADC1 in continous mode with a resolutuion equal to 12 bits */
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;
	ADC_Init(ADC1,&ADC_InitStructure);
	// channels selection and it's sampling time config
	
   //Detection on both rising and falling edges11, TIM1_CC4
	//ADC1->CFGR1|=ADC_CFGR1_EXTEN_1|ADC_CFGR1_EXTEN_0|ADC_CFGR1_EXTSEL_0 ;  
	
	
	ADC_ChannelConfig(ADC1,ADC_Channel_5,ADC_SampleTime_239_5Cycles);
	ADC_ChannelConfig(ADC1,ADC_Channel_9,ADC_SampleTime_239_5Cycles);
	//ADC_ChannelConfig(ADC1,ADC_Channel_TempSensor,ADC_SampleTime_55_5Cycles);
	//ADC_TempSensorCmd(ENABLE);
	//ADC_ChannelConfig(ADC1,ADC_Channel_Vrefint,ADC_SampleTime_55_5Cycles);
	//ADC_VrefintCmd(ENABLE);
	//ADC_ChannelConfig(ADC1,ADC_Channel_Vbat,ADC_SampleTime_55_5Cycles);
	//ADC_VbatCmd(ENABLE);// Vbat not present in UFQFPN32 pakage
	/* ADC Calibration */
	ADC_GetCalibrationFactor(ADC1);
	/* Enable ADC1 */
	ADC_Cmd(ADC1,ENABLE);
	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_ADEN)){;}/* Wait the ADCEN falg */

	/* DMA1 Channel1 Config */
		DMA_DeInit(DMA1_Channel1);
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
		DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&buf[0];
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
		DMA_InitStructure.DMA_BufferSize = 2;
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
		DMA_InitStructure.DMA_Priority = DMA_Priority_High;
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
		DMA_Init(DMA1_Channel1,&DMA_InitStructure);
		DMA_Cmd(DMA1_Channel1,ENABLE);
		/* ADC DMA request in circular mode */
		ADC_DMARequestModeConfig(ADC1,ADC_DMAMode_Circular);
		ADC_DMACmd(ADC1,ENABLE);
	/* ADC1 regular Software Start Conv */
	ADC_StartOfConversion(ADC1);
} 