#include "functions.h"
#include "display.h"


uint8_t LCD_buffer[5];

//переводит шестнацетиричное число в двоичное
//данные записывает в LCD_buffer[] [0]-старший разряд 
 void hex_to_dec (uint16_t x)
 {

	 LCD_buffer[0]=0;
	 LCD_buffer[1]=0;
	 LCD_buffer[2]=0;
	 LCD_buffer[3]=0;
	 LCD_buffer[4]=0;
	
  while (x>=10000)
	{x-=10000;
		LCD_buffer[0]++;
	}
	 
	 while (x>=1000)
	  {x-=1000;
		LCD_buffer[1]++;
	  }
	 
	 while (x>=100)
	 {
		 x-=100;
		 LCD_buffer[2]++;
	 }
	 
	 while (x>=10)
	 {
		 x-=10;
		 LCD_buffer[3]++;
	 }
	 
	 
	 while (x>=1)
	 {
		 x-=1;
		 LCD_buffer[4]++;
	 }
	 
	 
 };
 

  //возвращает код цифры
 uint8_t dec_to_char(uint8_t dec)
 {
	 uint8_t d[]={'0','1','2','3','4','5','6','7','8','9'}; 
	 return d[dec];
 }
 
 
  //отображает на экране двоичное число
 void showhex(uint8_t x,uint8_t page, uint16_t dig)
 {
	 uint8_t i,r=0,zero=0; 
	 uint8_t tmp[5]={0,0,0,0,0};
	 hex_to_dec(dig);	
	 //преобразование чисел в коды символов
	 for(i=0;i<5;i++)
	 {
		  if(LCD_buffer[i]!=0)//гашение нулей	
      {
				tmp[r]=dec_to_char(LCD_buffer[i]); 
        r++;
				zero=1; //флаг наличия не нуля				
      } 
			else 
			{ 
				 if(zero==1)
				{
          tmp[r]=dec_to_char(LCD_buffer[i]); 
          r++;
        }

      }		
	 }	 
	 if(r!=0){pstring(x,page,tmp,r);}
	 else {pstring(x,page,tmp,r);}//отображает 0	 
 }
 
 
