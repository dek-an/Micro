#include "lcdWH1602b.h"
#include "definesLcdWH1602b.h"

#include <util/delay.h>

#include <common/commonHeader.h>

// ///////////////////////////////////////////////////
// inner functions
//
static void writeCommand(unsigned char command)
{
	CBI(LCD_SIG_PORT, LCD_RW);
	CBI(LCD_SIG_PORT, LCD_RS);
	SBI(LCD_SIG_PORT, LCD_E);
	LCD_DATA_PORT = command;
	_delay_us(39);
	CBI(LCD_SIG_PORT, LCD_E);
}

// ///////////////////////////////////////////////////
// interface
//
void initLCD8b(void)
{
	// set all pins to out
	LCD_DATA_DDR = 0xFF;
	// set signal pins to out
	LCD_SIG_DDR |= SFT(LCD_RS) | SFT(LCD_RW) | SFT(LCD_E);

	// initialisation
	_delay_ms(15);
	// step 1
	// make it not via writeCommand because large delay
	CBI(LCD_SIG_PORT, LCD_RW);
	CBI(LCD_SIG_PORT, LCD_RS);
	SBI(LCD_SIG_PORT, LCD_E);
	LCD_DATA_PORT = 0b00110000;
	_delay_ms(5);
	CBI(LCD_SIG_PORT, LCD_E);
	// step 2
	// make it not via writeCommand because large delay
	CBI(LCD_SIG_PORT, LCD_RW);
	CBI(LCD_SIG_PORT, LCD_RS);
	SBI(LCD_SIG_PORT, LCD_E);
	LCD_DATA_PORT = 0b00110000;
	_delay_us(100);
	CBI(LCD_SIG_PORT, LCD_E);
	// step 3
	writeCommand(0b00110000);
	// DataLength:8-bit
	// numbers of display line (N:2-line/1-line)
	// display font type (F:5x11 dots/5x8 dots)
	writeCommand(0b00111000);
	// Set display (D), cursor (C ), and blinking of cursor (B) on/off control bit.
	writeCommand(0b00001100);
	// clear display
	writeCommand(0b00000001);
	// Entery mode set
	writeCommand(0b00000100);
}

void writeCharToLCD(const unsigned char c)
{
	CBI(LCD_SIG_PORT, LCD_RW);
	SBI(LCD_SIG_PORT, LCD_RS);
	SBI(LCD_SIG_PORT, LCD_E);
	LCD_DATA_PORT = c;
	_delay_us(43);
	CBI(LCD_SIG_PORT, LCD_E);
}

void writeStrToLCD(const unsigned char* str, const unsigned char size)
{
	for (const unsigned char* end = str + size; str < end; ++str)
	{
		writeCharToLCD(*str);
	}
}

void clearLCD(void)
{
	writeCommand(0b00000001);
}
