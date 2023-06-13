#include "i2c.h"

//записывает данные data по адресу adr, в регистр reg,
void RegWrite(uint8_t adr, uint8_t reg, uint8_t data)
{
	
//I2C1->CR2 |= (uint32_t)(LIS302DL_ADDR) | (uint32_t)(1 << 16) | I2C_CR2_RELOAD | I2C_CR2_START ;
  I2C_TransferHandling(I2C1,adr, 1, I2C_Reload_Mode, I2C_Generate_Start_Write);
  while(!(I2C1->ISR & I2C_ISR_TXIS)){;}
  
//I2C1->TXDR = (uint8_t)(addr);
  I2C_SendData(I2C1, reg);
  while(!(I2C1->ISR & I2C_ISR_TCR)){;}
    
//I2C1->CR2 |= I2C_CR2_AUTOEND;
  I2C_AutoEndCmd(I2C1, ENABLE);
  while(!(I2C1->ISR & I2C_ISR_TXIS)){;}
    
//I2C1->TXDR = data;
  I2C_SendData(I2C1, data);
  
//I2C1->CR2 &= ~I2C_CR2_RELOAD;
  I2C_ReloadCmd(I2C1, DISABLE);
    
  while(!(I2C1->ISR & I2C_ISR_STOPF)){;}
  I2C1->ICR = I2C_ICR_STOPCF;
}

//отправляет байт устройству
void i2c_write(uint8_t adr,uint8_t data)
{
  //RegWrite(adr,data,data);
     I2C_TransferHandling(I2C1,adr, 1, I2C_Reload_Mode, I2C_Generate_Start_Write);
     while(!(I2C1->ISR & I2C_ISR_TXIS)){;}
      

    I2C_AutoEndCmd(I2C1, ENABLE);
    while(!(I2C1->ISR & I2C_ISR_TXIS)){;}
     
	 			
		I2C_SendData(I2C1,data); 
    			 

     I2C_ReloadCmd(I2C1, DISABLE);
    
    while(!(I2C1->ISR & I2C_ISR_STOPF)){;}
    I2C1->ICR = I2C_ICR_STOPCF;
}


//инициализирует интерфейс
void i2c_init()
{	
 I2C_InitTypeDef  I2C_InitStructure;	
 //задаем тактовый сигнал для модуля I2C
 //тактируемся от HSI	
 RCC_I2CCLKConfig(RCC_I2C1CLK_HSI);	
//подает тактовую частоту на I2C1 
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE); 
	
	 //задаю режим работы I2C
  I2C_StructInit(&I2C_InitStructure);
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
  I2C_InitStructure.I2C_DigitalFilter = 0;
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_OwnAddress1 = 0x5;
   //I2C_InitStructure.I2C_Timing = 0x40B22536;
  I2C_Init(I2C1, &I2C_InitStructure);	
	
  I2C_Cmd(I2C1, ENABLE);
	
}



/*
 Отправляет данный из буфера buf по адресу adr
  cnt-количество байт для отправки
 */
void i2c_WrBuf(uint8_t adr, uint8_t *buf, uint8_t cnt) 
{
     uint8_t i=0; 
	
	for(i=0;i<cnt;i++)
   {	
     I2C_TransferHandling(I2C1,adr, 1, I2C_Reload_Mode, I2C_Generate_Start_Write);
     while(!(I2C1->ISR & I2C_ISR_TXIS)){;}
  
    // I2C_SendData(I2C1,*buf++);
     //while(!(I2C1->ISR & I2C_ISR_TCR)){;}
    

    I2C_AutoEndCmd(I2C1, ENABLE);
    while(!(I2C1->ISR & I2C_ISR_TXIS)){;}
     
	 			
		I2C_SendData(I2C1,*buf++); 
    			 

     I2C_ReloadCmd(I2C1, DISABLE);
    
    while(!(I2C1->ISR & I2C_ISR_STOPF)){;}
    I2C1->ICR = I2C_ICR_STOPCF;
		       
	  
  }
	   
   
  
  
}

//читает количество бит по адресу в буффер
void i2c_RdBuf (uint8_t adr, uint8_t *buf, uint32_t cnt) 
{
	uint32_t i=0;
     
	 while(i<cnt)
   {		 
	    I2C_TransferHandling(I2C1,adr,cnt, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);
      while(!(I2C1->ISR & I2C_ISR_RXNE)){;}
      *buf++ = I2C_ReceiveData(I2C1);
    
      while(!(I2C1->ISR & I2C_ISR_STOPF)){;}
      I2C1->ICR = I2C_ICR_STOPCF;
     i++;				
	}
  	
	
}


//

uint8_t I2C_RdBuf(uint8_t adr, uint8_t *Data, uint8_t DCnt)
{
	uint8_t Cnt, SingleData = 0;

	//As per, ensure the I2C peripheral isn't busy!
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) == SET);


	//As per, start another transfer, we want to read DCnt
	//amount of bytes. Generate a start condition and
	//indicate that we want to read.
	I2C_TransferHandling(I2C1,adr, DCnt, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);

	//Read in DCnt pieces of data
	for(Cnt = 0; Cnt<DCnt; Cnt++)
	  {
        //Wait until the RX register is full of luscious data!
        while(I2C_GetFlagStatus(I2C1, I2C_FLAG_RXNE) == RESET); 
        //If we're only reading one byte, place that data direct into the 
        //SingleData variable. If we're reading more than 1 piece of data 
        //store in the array "Data" (a pointer from main) 		
        if(DCnt > 1) Data[Cnt] = I2C_ReceiveData(I2C1);
	      else SingleData = I2C_ReceiveData(I2C1);
     }

     //Wait for the stop condition to be sent
     while(I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF) == RESET);

     //Clear the stop flag for next transfers
     I2C_ClearFlag(I2C1, I2C_FLAG_STOPF);

     //Return a single piece of data if DCnt was
     //less than 1, otherwise 0 will be returned.
	return SingleData;
}

//читает байт из устройства
uint8_t i2c_read(uint8_t adr)
{
	uint8_t data;
	I2C_TransferHandling(I2C1,adr,1, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_RXNE) == RESET); 
	
	data=I2C_ReceiveData(I2C1);
	    //Wait for the stop condition to be sent
     while(I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF) == RESET);

     //Clear the stop flag for next transfers
     I2C_ClearFlag(I2C1, I2C_FLAG_STOPF);
	return data;
	
	
}

void delay_ms(int time)
{
	int cn,z; 
	for(cn=0;cn<time*480;cn++)
  {
	  z++;
  }	 
	
}


