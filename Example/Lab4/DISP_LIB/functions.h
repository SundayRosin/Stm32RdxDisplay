#include "stm32f0xx.h"

//переводит шестнацетиричное число в двоичное
//данные записывает в LCD_buffer[] [0]-старший разряд 
 void hex_to_dec (uint16_t x);
 
 
 //возвращает код цифры
 uint8_t dec_to_char(uint8_t dec);
 
 //отображает на экране двоичное число
 void showhex(uint8_t x,uint8_t page, uint16_t dig);