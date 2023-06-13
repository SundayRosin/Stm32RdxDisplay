#include "display.h"
#include "Math.h"

// знакогенератор CP1251 5x8 пикселей
const uint8_t znakogen[] = { 
    0x00, 0x00, 0x00, 0x00, 0x00, // 0x20	пробел
    0x00, 0x00, 0xF2, 0x00, 0x00, // 0x21	!
    0x00, 0xE0, 0x00, 0xE0, 0x00, // 0x22	"
    0x28, 0xFE, 0x28, 0xFE, 0x28, // 0x23	#
    0x24, 0x54, 0xFE, 0x54, 0x48, // 0x24	$
    0xC6, 0xC8, 0x10, 0x26, 0xC6, // 0x25	%
    0x6C, 0x92, 0xAA, 0x44, 0x0A, // 0x26	&
    0x00, 0xA0, 0xC0, 0x00, 0x00, // 0x27	'
    0x38, 0x44, 0x82, 0x00, 0x00, // 0x28	(
    0x00, 0x00, 0x82, 0x44, 0x38, // 0x29	)
    0x28, 0x10, 0x7C, 0x10, 0x28, // 0x2A	*
    0x10, 0x10, 0x7C, 0x10, 0x10, // 0x2B	+
    0x00, 0x00, 0x0A, 0x0C, 0x00, // 0x2C	,
    0x10, 0x10, 0x10, 0x10, 0x10, // 0x2D	-
    0x00, 0x00, 0x02, 0x00, 0x00, // 0x2E	.   0x00, 0x00, 0x06, 0x06, 0x06
    0x04, 0x08, 0x10, 0x20, 0x40, // 0x2F	/

    0x7C, 0x8A, 0x92, 0xA2, 0x7C, // 0x30	0
    0x00, 0x42, 0xFE, 0x02, 0x00, // 0x31	1
    0x42, 0x86, 0x8A, 0x92, 0x62, // 0x32	2
    0x84, 0x82, 0xA2, 0xD2, 0x8C, // 0x33	3
    0x18, 0x28, 0x48, 0xFE, 0x08, // 0x34	4
    0xE4, 0xA2, 0xA2, 0xA2, 0x9C, // 0x35	5
    0x3C, 0x52, 0x92, 0x92, 0x0C, // 0x36	6
    0x80, 0x8E, 0x90, 0xA0, 0xC0, // 0x37	7
    0x6C, 0x92, 0x92, 0x92, 0x6C, // 0x38	8
    0x60, 0x92, 0x92, 0x94, 0x78, // 0x39	9
    0x00, 0x00, 0x6C, 0x6C, 0x00, // 0x3A	:
    0x00, 0x00, 0x6A, 0x6C, 0x00, // 0x3B	//
    0x10, 0x28, 0x44, 0x82, 0x00, // 0x3C	<
    0x28, 0x28, 0x28, 0x28, 0x28, // 0x3D	=
    0x00, 0x82, 0x44, 0x28, 0x10, // 0x3E	>
    0x40, 0x80, 0x8A, 0x90, 0x60, // 0x3F	?

    0x4C, 0x92, 0x9C, 0x42, 0x3C, // 0x40	@
    0x7E, 0x88, 0x88, 0x88, 0x7E, // 0x41	А
    0xFE, 0x92, 0x92, 0x92, 0x6C, // 0x42	В
    0x7C, 0x82, 0x82, 0x82, 0x44, // 0x43	С
    0xFE, 0x82, 0x82, 0x44, 0x38, // 0x44	D
    0xFE, 0x92, 0x92, 0x92, 0x82, // 0x45	Е
    0xFE, 0x90, 0x90, 0x90, 0x80, // 0x46	F
    0x7C, 0x82, 0x92, 0x92, 0x5E, // 0x47	G
    0xFE, 0x10, 0x10, 0x10, 0xFE, // 0x48	Н
    0x00, 0x82, 0xFE, 0x82, 0x00, // 0x49	I
    0x04, 0x02, 0x82, 0xFC, 0x80, // 0x4A	J
    0xFE, 0x10, 0x28, 0x44, 0x82, // 0x4B	К
    0xFE, 0x02, 0x02, 0x02, 0x02, // 0x4C	L
    0xFE, 0x40, 0x20, 0x40, 0xFE, // 0x4D	М
    0xFE, 0x20, 0x10, 0x08, 0xFE, // 0x4E	N
    0x7C, 0x82, 0x82, 0x82, 0x7C, // 0x4F	О

    0xFE, 0x90, 0x90, 0x90, 0x60, // 0x50	Р
    0x7C, 0x82, 0x8A, 0x84, 0x7A, // 0x51	Q
    0xFE, 0x90, 0x98, 0x94, 0x62, // 0x52	R
    0x62, 0x92, 0x92, 0x92, 0x8C, // 0x53	S
    0x80, 0x80, 0xFE, 0x80, 0x80, // 0x54	Т
    0xFC, 0x02, 0x02, 0x02, 0xFC, // 0x55	U
    0xF8, 0x04, 0x02, 0x04, 0xF8, // 0x56	V
    0xFC, 0x02, 0x1C, 0x02, 0xFC, // 0x57	W
    0xC6, 0x28, 0x10, 0x28, 0xC6, // 0x58	Х
    0xE0, 0x10, 0x1E, 0x10, 0xE0, // 0x59	Y
    0x86, 0x8A, 0x92, 0xA2, 0xC2, // 0x5A	Z
    0x00, 0xFE, 0x82, 0x82, 0x00, // 0x5B	[
    0x18, 0x24, 0x7E, 0x24, 0x18, // 0x5C	ф
    0x00, 0x82, 0x82, 0xFE, 0x00, // 0x5D	]
    0x20, 0x40, 0x80, 0x40, 0x20, // 0x5E	^
    0x02, 0x02, 0x02, 0x02, 0x02, // 0x5F	_

    0x00, 0x00, 0x80, 0x40, 0x00, // 0x60	'
    0x04, 0x2A, 0x2A, 0x2A, 0x1E, // 0x61	а
    0xFE, 0x12, 0x22, 0x22, 0x1C, // 0x62	b
    0x1C, 0x22, 0x22, 0x22, 0x04, // 0x63	с
    0x1C, 0x22, 0x22, 0x12, 0xFE, // 0x64	d
    0x1C, 0x2A, 0x2A, 0x2A, 0x18, // 0x65	е
    0x10, 0x7E, 0x90, 0x80, 0x40, // 0x66	f
    0x10, 0x2A, 0x2A, 0x2A, 0x3C, // 0x67	g
    0xFE, 0x10, 0x20, 0x20, 0x1E, // 0x68	h
    0x00, 0x22, 0xBE, 0x02, 0x00, // 0x69	i
    0x04, 0x02, 0x22, 0xBC, 0x00, // 0x6A	j
    0x00, 0xFE, 0x08, 0x14, 0x22, // 0x6B	k
    0x00, 0x82, 0xFE, 0x02, 0x00, // 0x6C	l
    0x3E, 0x20, 0x18, 0x20, 0x1E, // 0x6D	m
    0x3E, 0x10, 0x20, 0x20, 0x1E, // 0x6E	n

    0x1C, 0x22, 0x22, 0x22, 0x1C, // 0x6F	o
    0x3E, 0x28, 0x28, 0x28, 0x10, // 0x70	р
    0x10, 0x28, 0x28, 0x28, 0x3E, // 0x71	q
    0x3E, 0x10, 0x20, 0x20, 0x10, // 0x72	r
    0x12, 0x2A, 0x2A, 0x2A, 0x24, // 0x73	s
    0x20, 0xFC, 0x22, 0x02, 0x04, // 0x74	t
    0x3C, 0x02, 0x02, 0x04, 0x3E, // 0x75	u
    0x38, 0x04, 0x02, 0x04, 0x38, // 0x76	v
    0x3C, 0x02, 0x0C, 0x02, 0x3C, // 0x77	w
    0x22, 0x14, 0x08, 0x14, 0x22, // 0x78	х
    0x30, 0x0A, 0x0A, 0x0A, 0x30, // 0x79	у
    0x22, 0x26, 0x2A, 0x32, 0x22, // 0x7A	z
    0x00, 0x10, 0x6C, 0x82, 0x00, // 0x7B	{
    0x00, 0x00, 0xFE, 0x00, 0x00, // 0x7C	|
    0x00, 0x82, 0x6C, 0x10, 0x00, // 0x7D	}
    0x08, 0x10, 0x10, 0x08, 0x08, // 0x7E	~

    0xFF, 0x80, 0x80, 0x80, 0x80, // 0x7F символы псевдографики
    0x80, 0x80, 0x80, 0x80, 0x80, // 0x80
    0x80, 0x80, 0xFF, 0x80, 0x80, // 0x81
    0x80, 0x80, 0x80, 0x80, 0xFF, // 0x82
    0xFF, 0x00, 0x00, 0x00, 0x00, // 0x83
    0x00, 0x00, 0xFF, 0x00, 0x00, // 0x84
    0x00, 0x00, 0x00, 0x00, 0xFF, // 0x85
    0xFF, 0x10, 0x10, 0x10, 0x10, // 0x86
    0x10, 0x10, 0x10, 0x10, 0x10, // 0x87
    0x10, 0x10, 0xFF, 0x10, 0x10, // 0x88
    0x10, 0x10, 0x10, 0x10, 0xFF, // 0x89
    0xFF, 0x10, 0x10, 0x10, 0x10, // 0x8A
    0x10, 0x10, 0x10, 0x10, 0x10, // 0x8B
    0x10, 0x10, 0xFF, 0x10, 0x10, // 0x8C
    0x10, 0x10, 0x10, 0x10, 0xFF, // 0x8D
    0xFF, 0x80, 0xBF, 0xA0, 0xA0, // 0x8E
    0xA0, 0xA0, 0xA0, 0xA0, 0xA0, // 0x8F
    0xA0, 0xBF, 0x80, 0xBF, 0xA0, // 0x90
    0xA0, 0xA0, 0xBF, 0x80, 0xFF, // 0x91
    0xFF, 0x00, 0xFF, 0x00, 0x00, // 0x92
    0x00, 0xFF, 0x00, 0xFF, 0x00, // 0x93
    0x00, 0x00, 0xFF, 0x00, 0xFF, // 0x94
    0xFF, 0x00, 0xEF, 0x28, 0x28, // 0x95
    0x28, 0x28, 0x28, 0x28, 0x28, // 0x96
    0x28, 0xEF, 0x00, 0xEF, 0x28, // 0x97
    0x28, 0x28, 0xEF, 0x00, 0xFF, // 0x98
    0xFF, 0x01, 0xFD, 0x05, 0x05, // 0x99
    0x05, 0x05, 0x05, 0x05, 0x05, // 0x9A
    0x05, 0xFD, 0x01, 0xFD, 0x05, // 0x9B
    0x05, 0x05, 0xFD, 0x01, 0xFF, // 0x9C

    0x00, 0x00, 0x3E, 0x22, 0x3E, // 0x9D 0 маленькие цифры
    0x00, 0x00, 0x00, 0x00, 0x3E, // 0x9E 1
    0x00, 0x00, 0x2E, 0x2A, 0x3A, // 0x9F 2
    0x00, 0x00, 0x2A, 0x2A, 0x3E, // 0xA0 3
    0x00, 0x00, 0x38, 0x08, 0x3E, // 0xA1 4
    0x00, 0x00, 0x3A, 0x2A, 0x2E, // 0xA2 5
    0x00, 0x00, 0x3E, 0x2A, 0x2E, // 0xA3 6
    0x00, 0x00, 0x20, 0x20, 0x3E, // 0xA4 7
    0x00, 0x00, 0x3E, 0x2A, 0x3E, // 0xA5 8
    0x82, 0xBA, 0xAA, 0x92, 0xBA, // 0xA6 dli
    0x8A, 0x8A, 0x82, 0xBA, 0x82, // 0xA7
    0x88, 0x54, 0x22, 0x88, 0x22, // 0xA8 нагрев
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, // 0xA9 дымогенератор
    0x10, 0x20, 0x10, 0x10, 0x20, // 0xAA	~// spe *
    0x20, 0x40, 0xFE, 0x40, 0x20, // 0xAB	su * стрелка верх

    0x08, 0x04, 0xFE, 0x04, 0x08, // 0xAC	sd * // вниз
    0x10, 0x10, 0x54, 0x38, 0x10, // 0xAD	sr * стрелка в право ->
    0x10, 0x38, 0x54, 0x10, 0x10, // 0xAE	sl * <- стрелка в влево
    0x00, 0x07, 0x08, 0x13, 0x24, // 0xAF	верхний левый угол
    0x28, 0x28, 0x28, 0x28, 0x28, // 0xB0	горизонтальные линии верняя
    0x28, 0x28, 0x13, 0x08, 0x07, // 0xB1	верний правый угол
    0x00, 0xFF, 0x00, 0xFF, 0x00, // 0xB2	веритикальные левые линии
    0x00, 0x00, 0xFF, 0x00, 0xFF, // 0xB3	вертикальные правые линии
    0x00, 0xE0, 0x10, 0xC8, 0x24, // 0xB4	нижний левый угол
    0x14, 0x24, 0xC8, 0x10, 0xE0, // 0xB5	нижний правый угол
    0x14, 0x14, 0x14, 0x14, 0x14, // 0xB6	горизонтальные линии нижняя
    0x10, 0x38, 0x7C, 0xFE, 0x00, // 0xB7	треугольник влево
    0x00, 0xFE, 0x7C, 0x38, 0x10, // 0xB8	треугольник вправо
    0x08, 0x78, 0xFC, 0x78, 0x08, // 0xB9	kol * колокольчик

    0x63, 0x25, 0x18, 0x18, 0xA4, // 0xBA	вентилятор
    0x7E, 0x42, 0x42, 0x42, 0x7E, // 0xBB	квадрат пустой
    0x7E, 0x7E, 0x7E, 0x7E, 0x7E, // 0xBC	квадрат полный
    0x30, 0x0C, 0x30, 0x0C, 0x30, // 0xBD	птичка
    0x80, 0xFF, 0x01, 0x01, 0xFF, // 0xBE	импульс
    0x60, 0x90, 0x90, 0x60, 0x00, // 0xBF	градус *

    0x7E, 0x88, 0x88, 0x88, 0x7E, // 0xC0	А  m=160 //192
    0xFE, 0x92, 0x92, 0x92, 0x0C, // Б 193
    0xFE, 0x92, 0x92, 0x92, 0x6C, // В 194
    0xFE, 0x80, 0x80, 0x80, 0xC0, // Г 194
    0x07, 0x8A, 0xF2, 0x82, 0xFF, // Д 196
    0xFE, 0x92, 0x92, 0x92, 0x82, // Е 197
    0xEE, 0x10, 0xFE, 0x10, 0xEE, // 0хС6 Ж 198
    0x92, 0x92, 0x92, 0x92, 0x6C, // З 199
    0xFE, 0x08, 0x10, 0x20, 0xFE, // И 200
    0x3E, 0x84, 0x48, 0x90, 0x3E, // Й 201
    0xFE, 0x10, 0x28, 0x44, 0x82, // К 202
    0x04, 0x82, 0xFC, 0x80, 0xFE, // Л 
    0xFE, 0x40, 0x20, 0x40, 0xFE, // М
    0xFE, 0x10, 0x10, 0x10, 0xFE, // Н
    0x7C, 0x82, 0x82, 0x82, 0x7C, // О
    0xFE, 0x80, 0x80, 0x80, 0xFE, // П
    0xFE, 0x90, 0x90, 0x90, 0x60, // Р
    0x7C, 0x82, 0x82, 0x82, 0x44, // С
    0x80, 0x80, 0xFE, 0x80, 0x80, // Т
    0xE2, 0x14, 0x08, 0x10, 0xE0, // У
    0x18, 0x24, 0xFE, 0x24, 0x18, // ф
    0xC6, 0x28, 0x10, 0x28, 0xC6, // Х

    0xFE, 0x02, 0x02, 0x02, 0xFF, // Ц
    0xE0, 0x10, 0x10, 0x10, 0xFE, // Ч
    0xFE, 0x02, 0xFE, 0x02, 0xFE, // Ш
    0xFE, 0x02, 0xFE, 0x02, 0xFF, // Щ
    0x80, 0xFE, 0x12, 0x12, 0x0C, // Ъ
    0xFE, 0x12, 0x0C, 0x00, 0xFE, // Ы
    0xFE, 0x12, 0x12, 0x12, 0x0C, // Ь
    0x44, 0x82, 0x92, 0x92, 0x7C, // Э
    0xFE, 0x10, 0x7C, 0x82, 0x7C, // Ю
    0x62, 0x94, 0x98, 0x90, 0xFE, // Я
    0x04, 0x2A, 0x2A, 0x2A, 0x1E, // а 224
    0x3C, 0x52, 0x52, 0x92, 0x8C, // б //225
    0x3E, 0x2A, 0x2A, 0x14, 0x00, // в
    0x3E, 0x20, 0x20, 0x20, 0x30, // г

    0x07, 0x2A, 0x32, 0x22, 0x3F, // д
    0x1C, 0x2A, 0x2A, 0x2A, 0x18, // е //229
    0x36, 0x08, 0x3E, 0x08, 0x36, // ж 230
    0x22, 0x22, 0x2A, 0x2A, 0x14, // з 31
    0x3E, 0x04, 0x08, 0x10, 0x3E, // и 2
    0x1E, 0x42, 0x24, 0x48, 0x1E, // й 3
    0x3E, 0x08, 0x14, 0x22, 0x00, // к 4
    0x04, 0x22, 0x3C, 0x20, 0x3E, // л 5
    0x3E, 0x10, 0x08, 0x10, 0x3E, // м 6
    0x3E, 0x08, 0x08, 0x08, 0x3E, // н 7
    0x1C, 0x22, 0x22, 0x22, 0x1C, // o 8
    0x3E, 0x20, 0x20, 0x20, 0x3E, // п 9
    0x3E, 0x28, 0x28, 0x28, 0x10, // р 240
    0x1C, 0x22, 0x22, 0x22, 0x04, // с 241

    0x20, 0x20, 0x3E, 0x20, 0x20, // т 2
    0x30, 0x0A, 0x0A, 0x0A, 0x3C, // у 3
    0x38, 0x44, 0xFE, 0x44, 0x38, // Ф 4
    0x22, 0x14, 0x08, 0x14, 0x22, // х 5
    0x3E, 0x02, 0x02, 0x02, 0x3F, // ц 6
    0x30, 0x08, 0x08, 0x08, 0x3E, // ч 7
    0x3E, 0x02, 0x3E, 0x02, 0x3E, // ш
    0x3E, 0x02, 0x3E, 0x02, 0x3F, // щ
    0x20, 0x3E, 0x0A, 0x0A, 0x04, // ъ
    0x3E, 0x0A, 0x04, 0x00, 0x3E, // ы
    0x3E, 0x0A, 0x0A, 0x04, 0x00, // ь
    0x14, 0x22, 0x2A, 0x2A, 0x1C, // э
    0x3E, 0x08, 0x1C, 0x22, 0x1C, // ю
    0x10, 0x2A, 0x2C, 0x28, 0x3E, // я
 };

 
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
	i2c_read(0x72);//первый раз дисплей вседа возвращает нули 
	tmp=i2c_read(0x72);//читаю содержимое страницы 
	set_page(PageNo,x);//устанавливаю страницу и корд.Х
	
	data=0x01;data<<=bit;//зажигаю нужный пиксель
	//гасить или зажигать пиксель
	if(color==1){data=data|tmp;}
  else {data=(~data)&tmp;};	
	//посылаю дисплею команду	
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

//меняет порядок бит в байте на обратный
uint8_t swap(uint8_t byte)
{
  byte = ((byte & 0x55) << 1) | ((byte & 0xAA) >> 1);
  byte = ((byte & 0xCC) >> 2) | ((byte & 0x33) << 2);
  byte = (byte >> 4) | (byte << 4);
	return byte;	
}

//выводит символ 
void Litera(uint8_t x,uint8_t page,uint8_t bukva,uint8_t mode)
{
  uint8_t b;
  uint16_t m;	
	m = (bukva - 0x20);//начало символов
  m*=5;//позиция в знакогенераторе 
  //устанавливаю начальную страницу и корд.Х
	if(mode==1){set_page(page,x);}	
	
  //в знакогенераторе перевернутые буквы
  i2c_write(0x72,swap(znakogen[m])); //отсылаем адрес устройства,данные,запись
  m++;
		
	//рисуем остальные символы буквы
	for (b = 1; b < 5; b++)
  {
	  i2c_write(0x72,swap(znakogen[m]));m++;		
  }

}



//выводит строку
//x,page начало координат
void pstring(uint8_t x,uint8_t page,uint8_t *str,uint8_t cnt)
{
  uint8_t p=0;//позиция в массиве
  Litera(x,page,str[p],1);
	i2c_write(0x72,0); //пробел в 1 пиксель  
	for(p=1;p<cnt;p++)
	{
		  Litera(x,page,str[p],0);
		 i2c_write(0x72,0); //пробел в 1 пиксель 
	}
	
  	
}


