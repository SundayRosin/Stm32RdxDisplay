#include "stm32f0xx.h"


#define     OWI_GPIO_CLK   RCC_AHBPeriph_GPIOF //определение для вкл тактовой частоты порта
#define     OWI_GPIO       GPIOF   //!< 1-Wire PORT Data register.
#define     OWI_PIN        GPIO_Pin_4   //!< 1-Wire Input pin register.
#define     OWI_Low_Bit    GPIOF->BRR= GPIO_BRR_BR_8   //установка бита в 0
#define     OWI_Hi_Bit     GPIOF->BSRR =GPIO_BSRR_BS_8  //установка бита в 1
#define     OWI_GPIO_Read  GPIOF->IDR                  //чтение порта
#define     OWI_Set_Input  GPIOF->MODER&=~GPIO_Pin_4
#define     OWI_Set_Out    GPIOF->MODER|=GPIO_Pin_4

//инициализирует порт
void ow_init(void);


//задержка мкс
void delay(uint32_t cycles);

//генерирует сигнал начала посылки
void set_presence(void);


//отправляет бит
void ow_write_bit(uint8_t bit);

//читает бит
uint8_t ow_read_bit();

//отправляет байт
void ow_write_byte(uint8_t data);
