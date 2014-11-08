#include <avr/io.h>
#include <util/delay.h>

#include <lcdWH1602b/lcdWH1602b.h>
#include <common/commonHeader.h>

int main(void)
{
	initLCD8b();
	writeStrToLCD((unsigned char*)"Hello yo!!!", 11);
	_delay_ms(3000);

	unsigned char c = -1;
	while (1)
	{
		writeCharToLCD(++c);
		_delay_ms(3000);
		writeStrToLCD((unsigned char*)"t6 mazafaka!", 12);
		_delay_ms(5000);
		clearLCD();
		_delay_ms(3000);
	}

	return 0;
}
