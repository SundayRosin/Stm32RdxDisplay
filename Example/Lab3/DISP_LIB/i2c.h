#include "stm32f0xx_i2c.h"
#include "stm32f0xx.h"
#include "stm32f0xx_rcc.h"


//записывает данные data по адресу adr, в регистр reg,
void RegWrite(uint8_t adr, uint8_t reg, uint8_t data);

//отправляет байт устройству
void i2c_write(uint8_t adr,uint8_t data);

//читает байт из устройства
uint8_t i2c_read(uint8_t adr);

//инициализирует интерфейс
void i2c_init();

/*
 Отправляет данный из буфера buf по адресу adr
  cnt-количество байт для отправки
 */
void i2c_WrBuf(uint8_t adr, uint8_t *buf, uint8_t cnt) ;
//читает количество бит по адресу в буффер
uint8_t I2C_RdBuf(uint8_t adr, uint8_t *Data, uint8_t DCnt);

void delay_ms(int time);

