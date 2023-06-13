#include "i2c.h"

//инициализирует дисплей
void init_display();

//очистка дисплея
//tip==0 очистка,
//tip==1 черный,
//tip==2 шахматы.
void ClearLCD(uint8_t tip);

//отображает пиксель в заданных координатах x,y
//color=0 гасит пиксель,=1 зажигает
void ShowPixel(uint8_t x, uint8_t y,uint8_t color);

//Функция установки страницы памяти(0-8) дисплея и столбцов памяти 0-132
void set_page(uint8_t page,uint8_t column);


//выводит горизонтальную линию начальная координата x0,конечная x1
void Hline(uint8_t x0, uint8_t x1,uint8_t y);

//рисует круг
void Circle(uint8_t x, uint8_t y,uint8_t r,uint8_t color);


//меняет порядок бит в байте на обратный
uint8_t swap(uint8_t byte);

//выводит символ bukva,mode=1 устанавливать страницу
//mode=0 не устанавливать страницу
void Litera(uint8_t x,uint8_t page,uint8_t bukva,uint8_t mode);

//выводит строку
//x,page начало координат, str массив с кодами символов 
//cnt количество символов которые нужно выводить
void pstring(uint8_t x,uint8_t page,uint8_t *str);






