/*
 * Test_1.c
 *
 * Created: 10/11/2014 13:41:15
 *  Author: dekarand
 */ 

#include <common/lcdWH1602b/lcdWH1602b.h>

#define DISPLAY_TEST

volatile uint64 tempVal = 0;

static void displayTest()
{
#ifdef DISPLAY_TEST
	static const char numbers[] PROGMEM = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};

	initLcd();
	char col = 0;
	for (;;)
	{
		lcdWriteStr("Position\0");
		_delay_ms(3000);
		lcdGoTo(1, 3);
		lcdWriteStr("here");
		_delay_ms(1000);
		lcdClear();

		_delay_ms(1000);
		lcdWriteChar(pgm_read_byte(&numbers[col % 10]));
		_delay_ms(1000);
		lcdClear();
		_delay_ms(2);
		lcdWriteChar(pgm_read_byte(&numbers[col++ % 10]));
		_delay_ms(1000);

		lcdClear();
		lcdWriteChar('a');
		lcdWriteChar('b');
		lcdWriteChar('c');
		_delay_ms(1000);
		lcdWriteChar('A');
		lcdWriteChar('B');
		lcdWriteChar('C');
		_delay_ms(1000);
		lcdClear();
		lcdWriteStr("This is a String");
		_delay_ms(2000);
		lcdClear();


		lcdGoTo(0, 4);
		lcdWriteStr("Position 0-4");
		lcdWriteStr("here");
		_delay_ms(1000);
		lcdClear();
	}
#endif
}


int main(void)
{
	displayTest();

	return 0;
}
