#include "stm32f0xx.h"
#include "stm32f0xx_conf.h"

//настрока УАРТ 
void ow_init(void);

//формирует сигнал "сброс"
//если на шине есть устройтсво =1
uint8_t ow_reset(void);

//отправка данных из массива 
void ow_write(const uint8_t *command,uint16_t len);

//читает данные
void ow_read(const uint8_t *command, uint8_t *buf, uint16_t len) ;

