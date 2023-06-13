#define F_CPU 8000000UL	// ����������� �������� ������� ��� �������� __delay_ms() � __delay_us()
#include <util/twi.h>
#include <avr/pgmspace.h> // ��� ���� ��� �� ����� ��� � flash ������  //http://www.nongnu.org/avr-libc/user-manual/pgmspace.html
#include <Util/delay.h>

//����� �� ������ ���� ��� �� ����� �������� �� I2C ��������� � 10�,����������� ���������� ���������.

#define i2c_PORT	PORTC				// ���� ��� ����� ���� TWI
#define i2c_DDR		DDRC
#define i2c_SCL		5				// ������ ��������������� �������
#define i2c_SDA		4

//������������� �������
void init_display(void);

//�������� ������� ������� ������� ��� �� �����.

// 0(first_column)-����� ���������� ������� � �������� ��������� �����������
// 1(x_size)-������ ������� � ��������, ��� ������ ���� ��� ������� �� x ����������
// 2 (nomber_razrad) -c������ �������� ����� �������� �� �����
// 3 (first_page) -��������� �������� ������ ��� ������ �����������
//4 (end_page)- �������� �������� ������ ��� ������ �����������
//5 (data_array[]) -������ ������, ���������� ����� � ������� ���������������, ��� ������ �� �����
// �������� ������ ��� ��������� �������� 10�16, � ���������� �� ����� 2� ������� � ������.
//6 (where_point) ����� ������ ������� �������� �������? ���� ����� ����� ������ ����� ��������, > nomber_razrad �� ������� �� �����
//7 (where_point_one) ������ �������.
//8 (point_big_on) ���� point_big_on=1 ������ � ������� �������� ����� �������� �����,���� point_big_on=0 ����� ���.


void send_pixel(unsigned char first_column,  unsigned char x_size,unsigned char nomber_razrad,  unsigned char first_page,unsigned char end_page,unsigned char data_array[],
unsigned char where_point,unsigned char where_point_one,unsigned char point_big_on);