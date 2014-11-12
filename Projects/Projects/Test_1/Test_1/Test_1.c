/*
 * Test_1.c
 *
 * Created: 10/11/2014 13:41:15
 *  Author: dekarand
 */ 

#include <common/commonHeader.h>

// //////////////////////////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////////////////////////
// data port
#define LCD_DATA_PORT PORTB
#define LCD_DATA_DDR DDRB
#define LCD_DATA_PIN PINB

// data pins numbers
#define LCD_DB7 7
#define LCD_DB6 6
#define LCD_DB5 5
#define LCD_DB4 4

// signal port
#define LCD_SIG_PORT PORTA
#define LCD_SIG_DDR DDRA
#define LCD_SIG_PIN PINA

// signal pins numbers
#define LCD_RS 0
#define LCD_RW 1
#define LCD_E 2

#define LCD_SIG_MASK SFT(LCD_RW) | SFT(LCD_RS) | SFT(LCD_E)
#define LCD_DATA_MASK SFT(LCD_DB7) | SFT(LCD_DB6) | SFT(LCD_DB5) | SFT(LCD_DB4)
#define LCD_SET_STROB() SBI(LCD_SIG_PORT, LCD_E)
#define LCD_CLEAR_STROB() CBI(LCD_SIG_PORT, LCD_E)
#define LCD_WRITE_DELAY 43

typedef enum
{
	LCD_COMMAND = 0,
	LCD_DATA = 1
} LcdCommandType;

static void putNibble(const uchar nibble, LcdCommandType commandType)
{
		// signal port to out
//		LCD_SIG_DDR |= LCD_SIG_MASK;

		// set command/data mode
		switch (commandType)
		{
			case LCD_COMMAND:
				CBI(LCD_SIG_PORT, LCD_RS);
				break;
			case LCD_DATA:
				SBI(LCD_SIG_PORT, LCD_RS);
				break;
		}

		//switch (lcdRW)
		//{
			//case LCD_READ:
			//// read mode
			//SBI(LCD_SIG_PORT, LCD_RW);
			//// data port to in with pull-up
			//LCD_DATA_DDR &= ~LCD_DATA_MASK;
			//LCD_DATA_PORT |= LCD_DATA_MASK;
			//break;
			//case LCD_WRITE:
			// write mode
			CBI(LCD_SIG_PORT, LCD_RW);
			// data port to out
			//LCD_DATA_DDR |= LCD_DATA_MASK;
			//break;
		//}

	// clear data port
	LCD_DATA_PORT &= ~LCD_DATA_MASK;
	_delay_us(LCD_WRITE_DELAY);
	
	// set strob
	SBI(LCD_SIG_PORT, LCD_E);

	// set nibble
	if (GBI(nibble, 0))
		SBI(LCD_DATA_PORT, LCD_DB4);
	if (GBI(nibble, 1))
		SBI(LCD_DATA_PORT, LCD_DB5);
	if (GBI(nibble, 2))
		SBI(LCD_DATA_PORT, LCD_DB6);
	if (GBI(nibble, 3))
		SBI(LCD_DATA_PORT, LCD_DB7);

	// delay
	_delay_us(LCD_WRITE_DELAY);

	// clear strob
	CBI(LCD_SIG_PORT, LCD_E);
	_delay_us(LCD_WRITE_DELAY);
}

static void putByte(const uchar byte, LcdCommandType commandType)
{
	// high nibble
	putNibble((byte >> 4), commandType);
	// low nibble
	putNibble(byte, commandType);
}

static void initLcd(void)
{
	LCD_SIG_DDR |= LCD_SIG_MASK;
	LCD_DATA_DDR |= LCD_DATA_MASK;
	
	// wait for 15 ms
	_delay_ms(15);
	// 1
	//putNibble(0b0011, LCD_COMMAND);
	//_delay_us(/*4100 - LCD_WRITE_DELAY*/4057);
	//// 2
	//putNibble(0b0011, LCD_COMMAND);
	//_delay_us(/*100 - LCD_WRITE_DELAY*/57);
	//// 3
	//putNibble(0b0011, LCD_COMMAND);
	// 4-bit
	putNibble(0b0010, LCD_COMMAND);
	// 4-bit; 2 lines; 5x11 dots
	putByte(0b00101100, LCD_COMMAND);
	// display on; cursor off; blinking of cursor off
	putByte(0b00001100, LCD_COMMAND);
	// display clear
	putByte(0b00000001, LCD_COMMAND);
	_delay_ms(2);
	putByte(0b00000100, LCD_COMMAND);
}

void writeChar(char c)
{
	putByte(c, LCD_DATA);
}

// //////////////////////////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////////////////////////

static const uchar numbers[] PROGMEM = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};

int main(void)
{
	initLcd();

	char col = 0;
	for (;;)
	{
		_delay_ms(1000);

		writeChar((uchar)pgm_read_byte(&numbers[col % 10]));
		_delay_ms(1000);
		putByte(0b00000001, LCD_COMMAND);
		_delay_ms(2);
		writeChar((uchar)pgm_read_byte(&numbers[col++ % 10]));
		_delay_ms(1000);
	}

	return 0;
}