/*
 * Test_1.c
 *
 * Created: 10/11/2014 13:41:15
 *  Author: dekarand
 */ 

#include <common/lcdWH1602b/lcdWH1602b.h>

static const uchar numbers[] PROGMEM = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};

int main(void)
{
	initLcd();

	lcdWriteStr((const uchar*)"Yoololo!");
	_delay_ms(3000);
	lcdGoTo(2, 3);
	lcdWriteStrProgMem((const uchar*)PSTR("YO ProgMem"));
	_delay_ms(3000);

	uint08 line = 0;
	uint08 col = 0;
	for (;;)
	{
		_delay_ms(1000);
		lcdClear();
		lcdGoTo(line, col);
		lcdWriteChar((uchar)pgm_read_byte(&numbers[col % 10]));
		_delay_ms(1000);
		lcdGoTo(2, 3);
		lcdWriteStr("Ha-HA");
		line = (line + 1) % 5;
		col = (col + 1) % 50;
	}

	return 0;
}