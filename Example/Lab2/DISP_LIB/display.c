#include "display.h"
#include "Math.h"

//инициализирует дисплей
void init_display()
{
	uint8_t cmd[9],i;//массив с командами инициализации

	cmd[0]=0xe2; //сброс дисплея
	cmd[1]=0x24; //установить термокомпенсацию
	cmd[2]=0xC6;// настройка типа разветки свеху в низ, и слево на право
	cmd[3]=0xA0;// 80 кадров в сек// 1010000# // Установить Frame Rate
	cmd[4]=0xEB;// BIAS 9  //111010##  Установить LCD Bias Ratio
	cmd[5]=0x81;// настройка Vbias
	cmd[6]=0x80;//Vbias=200
	cmd[7]=0x89;// Set RAM Address Control
	cmd[8]=0xAF;//Включить дисплей

	for(i=0;i<9;i++)
	{
		 i2c_write(0x70,cmd[i]);
		 delay_ms(10);
	}
		
}


//tip==0 очистка,
//tip==1 черный,
//tip==2 шахматы.
void ClearLCD(uint8_t tip)
{
	uint16_t a;	
	i2c_write(0x70,0xB0); 	//  Установить адрес страницы 0	0 1	0 1	1 #	# #	# Set PA[3:0]  страница 0
	i2c_write(0x70,0x00); 	// Установить адрес столбца LSB  000000####	Set CA [3:0] колонка 0
	i2c_write(0x70,0x10); 	//Установить адрес столбца MSB   000001####	Set CA [3:0] колонка 0
	
	// выбор типа очистки
	if (tip==0)
	{ 
		for (a=0;a<1056;a++)
		i2c_write(0x72,0x00);
				
	}	
	else
  {
     if(tip==1) 
		 {
		   for (a=0;a<1056;a++)
		    {			
		 	  if (a!=0) {i2c_write(0x72,0xFF);}
		    i2c_write(0x70,0xFF);
	      }				
     }
	   else 
	   {	
  		  for (a=0;a<528;a++)
		    {    
			   i2c_write(0x72,0xAA);
			   i2c_write(0x72,0x55);		
			  }
		 }
  }


}

//отображает пиксель
void ShowPixel(uint8_t x, uint8_t y,uint8_t color)
{
  //номер страницы,разряд пикселя в странице	
	uint8_t PageNo,bit;
	uint8_t data,tmp;//данные страницы
	
 	bit=y%8;//определяю разряд пикселя в странице 
	PageNo=y/8; //определяю с какой страницей работать
	
  set_page(PageNo,x);//устанавливаю страницу и корд.Х
	i2c_read(0x72);//первый раз дислей вседа возвращает нули 
	tmp=i2c_read(0x72);//читаю содержимое страницы 
	set_page(PageNo,x);//устанавливаю страницу и корд.Х
	
	data=0x01;data<<=bit;//зажигаю нужный пиксель
	//гасить или зажигать пиксель
	if(color==1){data=data|tmp;}
  else {data=(~data)&tmp;};	
	//посылаю дислею команду	
  i2c_write(0x72,data);	
 
}


//Функция установки страницы памяти(0-8) дисплея и столбцов памяти 0-132
void set_page(uint8_t page,uint8_t column)
{ 
	uint8_t buf[3],i;	
 	
	buf[0]=page|0xB0; //  Установить адрес страницы  1011 #	# #	# Set PA[3:0]  страница 0
  buf[1]=column & 0x0F;// Установить адрес столбца LSB  000000####	Set CA [3:0] колонка 0
  	column=column>>4 ;
  buf[2]=column|0x10;//Установить адрес столбца MSB   000001####	Set CA [7:4] колонка 0  	
	 for(i=0;i<3;i++) {i2c_write(0x70,buf[i]);} 
	
}




//выводит горизонтальную линию начальная координата x0,конечная x1
void Hline(uint8_t x0, uint8_t x1,uint8_t y)
{
	uint8_t i=x0; 
	for (;i<x1;i++){ShowPixel(i,y,1); }
}


//рисует круг
void Circle(uint8_t x, uint8_t y,uint8_t r,uint8_t color)
{
	double fi=0,X,Y,dFi=0.0025;
	uint8_t x1,y1;
	uint16_t i;
	
	 for(i=0;i<400;i++)
	 {
    X=cos(dFi)*r;
		Y=sin(dFi)*r;
		dFi++; 
		x1=(uint8_t)(x+X); 
		y1=(uint8_t)(y+Y);  
		ShowPixel(x1,y1,color); 
 	}
}

