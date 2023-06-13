#include "eeprom.h"
#include <avr/io.h> 

//‘ункци€ чтени€ из EEPROM
// задаем адресс €чейки в условном фотмате, получаем значение в этой €чейке.
unsigned char ee_read (unsigned int addr)
{
	asm volatile("cli");
/* Wait for completion of previous write */
while(EECR & (1<<EEPE));
/* Set up address register */
EEAR = addr;
/* Start eeprom read by writing EERE */
EECR |= (1<<EERE);
/* Return data from data register */
asm volatile("sei");
return EEDR;
};	

//‘ункци€ записи в €чейку с проверкой содержимого в €чейке на предмет равенства того что записано туда уже.
void ee_write (unsigned int addr, unsigned char data)
{
asm volatile("cli");
unsigned char old_data=0;
/* Wait for completion of previous write */
while(EECR & (1<<EEPE));
/* Set up address  */
old_data=ee_read(addr);

if (old_data != data)
{

EEAR = addr;
EEDR = data;

/* Write logical one to EEMPE */
//EECR |= (1<<EEMPE);
asm volatile ("sbi 0x1F,2  ");  // »наче оно не успевает за 4 такта выполнить команду!!!!!! без включенной оптимизации

/* Start eeprom write by setting EEPE */
//EECR |= (1<<EEPE);
asm volatile ("sbi 0x1F,1");

asm volatile("sei");

}
}


/*
//‘ункци€ чтени€ 16 битного числа из еепром
unsigned short ee_read_too_byte(unsigned char address  )
{
	unsigned char a,b;
	unsigned short data;
	a=ee_read(address);
	b=ee_read(address+1);
	
	data=256*a+b;
	// EEPROM 2-byte Data Read Function - чтение двухбайтового числа из 2 последовательных €чеек с адресом первой addr
	//unsigned int _2breadEEPROM (unsigned int addr)
	//{
		//unsigned int data = 0;
		//		data = 256 * readEEPROM(addr) + readEEPROM(addr + 1);
		//	return data;
	//}
	return data;
}

//‘ункци€ записи 16 битного числа в еепром
void ee_write_too_byte(unsigned short data,unsigned char address  )
{
	
if (ee_read_too_byte(address)!=data)
{	
 ee_write(address,data/256);
 ee_write(address+1,data % 256);
}
	//writeEEPROM (addr, (unsigned char)(data / 256));
	//writeEEPROM (addr + 1, (unsigned char)(data % 256));

}
*/

