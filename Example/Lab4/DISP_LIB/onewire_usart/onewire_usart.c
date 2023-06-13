#include "onewire_usart.h"

//настройка USART2 для работы через DMA
void ow_init(void)
{
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  //подаем тактовую частоту на периферию
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  //включаем альтернативные функции порта А
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);
  //Настраиваю выводы USART2 Rx(PA3) Tx(PA2)
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  //настраиваю USART
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode =USART_Mode_Tx| USART_Mode_Rx;
  USART_Init(USART2, &USART_InitStructure);
  USART_Cmd(USART2,ENABLE);   	 
}
	
	
//формирует сигнал "сброс"
uint8_t ow_reset(void)
{
 uint8_t ow_presence;//есть ли датчик на шине?
 USART_InitTypeDef USART_InitStructure;
     
 USART_DMACmd(USART2, USART_DMAReq_Tx | USART_DMAReq_Rx, DISABLE);
 //Настраиваю USART на скорость 9600	
 USART_DeInit(USART2);		
 USART_InitStructure.USART_BaudRate = 9600;
 USART_InitStructure.USART_WordLength = USART_WordLength_8b;
 USART_InitStructure.USART_StopBits = USART_StopBits_1;
 USART_InitStructure.USART_Parity = USART_Parity_No;
 USART_InitStructure.USART_HardwareFlowControl =
   USART_HardwareFlowControl_None;
 USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
 USART_Init(USART2, &USART_InitStructure);
 USART_Cmd(USART2,ENABLE); 
 USART_ClearFlag(USART2, USART_FLAG_TC);		
 USART_SendData(USART2, 0xf0);
 while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
 ow_presence = USART_ReceiveData(USART2);
		
 USART_DeInit(USART2);
 USART_InitStructure.USART_BaudRate = 115200;
 USART_InitStructure.USART_WordLength = USART_WordLength_8b;
 USART_InitStructure.USART_StopBits = USART_StopBits_1;
 USART_InitStructure.USART_Parity = USART_Parity_No;
 USART_InitStructure.USART_HardwareFlowControl =
        USART_HardwareFlowControl_None;
 USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
 USART_Init(USART2, &USART_InitStructure);
 USART_Cmd(USART2,ENABLE);
 if (ow_presence != 0xf0){return 1;}
 else {return 0;}
    
}

//отправка данных из массива 
void ow_write(const uint8_t *command,uint16_t len) 
{
 DMA_InitTypeDef DMA_InitStructure;
 DMA_DeInit (DMA1_Channel4);    
 //в качестве адреса периферии устанавливаем адрес регистра данных USART2
 DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &(USART2->TDR);
 //В качестве адреса памяти берем переданный в параметрах массив   
 DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&command[0];
 //Направление передачи — из памяти в периферию
 DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
 //Длина передаваемого блока данных      
 DMA_InitStructure.DMA_BufferSize = len;
 //не увеличивать адрес переферии после каждой передачи
 DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
 //увеличивать адрес памяти после передачи
 DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
 //размер данных 1байт
 DMA_InitStructure.DMA_PeripheralDataSize =
                   DMA_PeripheralDataSize_Byte;
 DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
 //обычный режим работы
 DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
 //низкий приоритет
 DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
 //режим память-память отключен
 DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
 //канал DMA для USART2 TX
 DMA_Init(DMA1_Channel4, &DMA_InitStructure);
 //по окончании передачи обратиться к DMA  
 USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
 DMA_Cmd(DMA1_Channel4, ENABLE);
} 

//читает данные
void ow_read(const uint8_t *command, uint8_t *buf, uint16_t len) 
{
 DMA_InitTypeDef DMA_InitStructure;
 //5 канал DMA USART2 RX
 DMA_DeInit(DMA1_Channel5);
 DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->RDR);
 DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&buf[0];
 //из перефирии в память
 DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
 DMA_InitStructure.DMA_BufferSize = len;
 DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
 DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
 DMA_InitStructure.DMA_PeripheralDataSize =
                        DMA_PeripheralDataSize_Byte;
 DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
 DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
 DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
 DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
 DMA_Init(DMA1_Channel5, &DMA_InitStructure);
  //tx
 DMA_DeInit(DMA1_Channel4); 
	
 DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &(USART2->TDR);
 DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&command[0];
 DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
 DMA_InitStructure.DMA_BufferSize = len;
 DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
 DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
 DMA_InitStructure.DMA_PeripheralDataSize =
              DMA_PeripheralDataSize_Byte;
 DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
 DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
 DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
 DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
 DMA_Init(DMA1_Channel4, &DMA_InitStructure);

 USART_DMACmd(USART2,USART_DMAReq_Rx|USART_DMAReq_Tx, ENABLE);
 DMA_Cmd(DMA1_Channel5, ENABLE);      
 DMA_Cmd(DMA1_Channel4, ENABLE);
          
   while (DMA_GetFlagStatus(DMA1_FLAG_TC5) == RESET){;}
}
