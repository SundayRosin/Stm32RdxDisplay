#include "stm32f0xx.h"

//Функция инициализации ADC
//buf[2]-куда сохранять данные двух каналов
void init_ADC(uint16_t *buf);