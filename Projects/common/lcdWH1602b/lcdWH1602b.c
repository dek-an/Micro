#include "lcdWH1602b.h"
#include "lcdWH1602bDef.h"

#include <util/delay.h>

// //////////////////////////////////////////////////////////
// Work Helpers
//
typedef enum
{
	LCD_COMMAND = 0,
	LCD_DATA = 1
} LcdCommandType;

typedef enum
{
	LCD_WRITE = 0,
	LCD_READ = 1
} LcdReadWrite;

// Nibble is 4 low bits of the byte: 0xhhhhLLLL
static void setLcdMode(const LcdCommandType commandType, const LcdReadWrite lcdRW);
static void putNibble(const uchar nibble, const LcdCommandType commandType);
static uchar getNibble(const LcdCommandType commandType);
static void putByte(const uchar byte, const LcdCommandType commandType);
static uchar getByte(const LcdCommandType commandType);
static void putCommand(const uchar command);
static void putData(const uchar data);

// ///////////////////////////////////////////////
// LCD Defines
//
#define LCD_DATA_MASK SFT(LCD_DB7) | SFT(LCD_DB6) | SFT(LCD_DB5) | SFT(LCD_DB4)
#define LCD_SIG_MASK SFT(LCD_RW) | SFT(LCD_RS) | SFT(LCD_E)
#define LCD_SET_STROB() SBI(LCD_SIG_PORT, LCD_E)
#define LCD_CLEAR_STROB() CBI(LCD_SIG_PORT, LCD_E)
#define LCD_BF LCD_DB7
// Due to _delay_us and _delay_ms argument should be Compiler-Known-Constant we use next defines.
// error: __builtin_avr_delay_cycles expects a compile time integer constant
#define LCD_WRITE_DELAY 43
#define LCD_READ_DELAY 43

// //////////////////////////////////////////////////////////
// Interface
//
inline void initLcd(void)
{
	static BOOL isInitialized = 0;
	if (isInitialized)
		return;

	// wait for 15 ms
	_delay_ms(15);
	// 1
	putCommand(0b00110000);
	_delay_us(/*4100 - LCD_WRITE_DELAY*/4057);
	// 2
	putCommand(0b00110000);
	_delay_us(/*100 - LCD_WRITE_DELAY*/57);
	// 3
	putCommand(0b00110000);
	// 4-bit
	putCommand(0b00100000);
	// 4-bit; 2 lines; 5x11 dots
	putCommand(0b00101100);
	// display clear
	lcdClear();
	// display on; cursor off; blinking of cursor off
	putCommand(0b00001100);

	isInitialized = 0xFF;
}

BOOL lcdIsBusy(void)
{
	if (GBI(getByte(LCD_COMMAND), LCD_BF))
		return 0xFF;

	return 0x00;
}

void lcdWriteChar(const uchar c)
{
	putData(c);
}

void lcdWriteStr(const uchar* str)
{
	do
	{
		lcdWriteChar(*str);
	} while (*(str++) != '\0');
}

void lcdClear(void)
{
	putCommand(0b00000001);
	_delay_us(/*1530 - LCD_WRITE_DELAY*/1497);
}

void lcdGoTo(uchar line, uchar col)
{
	putCommand(0b10000000 | ((0x40 * line) + col));
}

void lcdWriteStrProgMem(const uchar* str)
{
	for (register uchar c = pgm_read_byte(str); c != 0; c = pgm_read_byte(str++))
		lcdWriteChar(c);
}

// //////////////////////////////////////////////////////////
// Work Helpers Implementation
//
static void setLcdMode(const LcdCommandType commandType, const LcdReadWrite lcdRW)
{
	// signal port to out
	LCD_SIG_DDR |= LCD_SIG_MASK;

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

	switch (lcdRW)
	{
	case LCD_READ:
		// read mode
		SBI(LCD_SIG_PORT, LCD_RW);
		// data port to in with pull-up
		LCD_DATA_DDR &= ~LCD_DATA_MASK;
		LCD_DATA_PORT |= LCD_DATA_MASK;
		break;
	case LCD_WRITE:
		// write mode
		CBI(LCD_SIG_PORT, LCD_RW);
		// data port to out
		LCD_DATA_DDR |= LCD_DATA_MASK;
		break;
	}
}

static void putNibble(const uchar nibble, LcdCommandType commandType)
{
	setLcdMode(commandType, LCD_WRITE);

	// set strob
	LCD_SET_STROB();

	// clear data port
	LCD_DATA_PORT &= ~LCD_DATA_MASK;

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
	LCD_CLEAR_STROB();
}

static uchar getNibble(const LcdCommandType commandType)
{
	setLcdMode(commandType, LCD_READ);

	LCD_SET_STROB();
	_delay_us(LCD_READ_DELAY);
	LCD_CLEAR_STROB();

	uchar val = 0;
	if (GBI(LCD_DATA_PIN, LCD_DB4))
		val |= SFT(0);
	if (GBI(LCD_DATA_PIN, LCD_DB5))
		val |= SFT(1);
	if (GBI(LCD_DATA_PIN, LCD_DB6))
		val |= SFT(2);
	if (GBI(LCD_DATA_PIN, LCD_DB7))
		val |= SFT(3);

	return val;
}

static void putByte(const uchar byte, LcdCommandType commandType)
{
	// high nibble
	putNibble(byte >> 4, commandType);
	// low nibble
	putNibble(byte, commandType);
}

static uchar getByte(LcdCommandType commandType)
{
	// high nibble
	uchar val = getNibble(commandType) << 4;
	// low nibble
	val |= getNibble(commandType);

	return val;
}

static void putCommand(const uchar command)
{
	putByte(command, LCD_COMMAND);
}

static void putData(const uchar data)
{
	putByte(data, LCD_DATA);
}