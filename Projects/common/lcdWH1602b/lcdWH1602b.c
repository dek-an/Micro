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
static void clearLcdMode();
static void putNibble(const char nibble, const LcdCommandType commandType);
static char getNibble(const LcdCommandType commandType);
static void putByte(const char byte, const LcdCommandType commandType);
static char getByte(const LcdCommandType commandType);
static void inline putCommand(const char command);
static void inline putData(const char data);
static inline char getCommand();
//static inline char getData();
static void inline waitWhileBusy();

// ///////////////////////////////////////////////
// LCD Defines
//
#define LCD_DATA_MASK SFT(LCD_DB7) | SFT(LCD_DB6) | SFT(LCD_DB5) | SFT(LCD_DB4)
#define LCD_SIG_MASK SFT(LCD_RW) | SFT(LCD_RS) | SFT(LCD_E)
#define LCD_BF LCD_DB7

// //////////////////////////////////////////////////////////
// Interface
//
inline void initLcd(void)
{
	static BOOL isInitialized = 0x00;
	if (isInitialized)
		return;

	isInitialized = 0xFF;

	// wait for 15 ms
	_delay_ms(100);

	// signal port to out; don't touch signal DDR anymore
	SET_MASK(LCD_SIG_DDR, LCD_SIG_MASK);
	clearLcdMode();

	// 1
	putNibble(0b0011, LCD_COMMAND);
	_delay_us(4100);
	// 2
	putNibble(0b0011, LCD_COMMAND);
	_delay_us(100);
	// 3
	putNibble(0b0011, LCD_COMMAND);

	//waitWhileBusy();

	// 4-bit
	putNibble(0b0010, LCD_COMMAND);
	// 4-bit; 2 lines; 5x11 dots
	putCommand(0b00101100);
	// display on; cursor off; blinking of cursor off
	putCommand(0b00001100);
	// display clear
	lcdClear();
	// return home
	putCommand(0b00000010);
}

BOOL lcdIsBusy(void)
{
	if (GBI(getCommand(), LCD_BF))
		return 0xFF;

	return 0x00;
}

void lcdWriteChar(const char c)
{
	putData(c);
}

void lcdWriteStr(const char* str)
{
	for (char c = *str; c != '\0'; c = *(++str))
	{
		lcdWriteChar(c);
	}
}

void lcdClear(void)
{
	putCommand(0b00000001);
	_delay_us(1530);
}

void lcdGoTo(uint08 line, uint08 col)
{
	putCommand((0b10000000) | ((0x40 * (line & 1)) + col));
}

void lcdWriteStrProgMem(const char* str)
{
	for (register char c = pgm_read_byte(str); c != 0; c = pgm_read_byte(str++))
		lcdWriteChar(c);
}

// //////////////////////////////////////////////////////////
// Work Helpers Defines
//
#define LCD_SET_STROB() SBI(LCD_SIG_PORT, LCD_E)
#define LCD_CLEAR_STROB() CBI(LCD_SIG_PORT, LCD_E)
#define LCD_STROB() LCD_SET_STROB(); _delay_us(LCD_STROB_DELAY); LCD_CLEAR_STROB()
// Due to _delay_us and _delay_ms argument should be Compiler-Known-Constant we use next defines.
// error: __builtin_avr_delay_cycles expects a compile time integer constant
#define LCD_WRITE_DELAY 43
#define LCD_READ_DELAY 43
#define LCD_STROB_DELAY 5

// //////////////////////////////////////////////////////////
// Work Helpers Implementation
//
static void setLcdMode(const LcdCommandType commandType, const LcdReadWrite lcdRW)
{
	switch (lcdRW)
	{
	case LCD_READ:
		// data port to in with pull-up
		CLEAR_MASK(LCD_DATA_DDR, LCD_DATA_MASK);
		SET_MASK(LCD_DATA_PORT, LCD_DATA_MASK);
		// read mode
		SBI(LCD_SIG_PORT, LCD_RW);
		break;
	case LCD_WRITE:
		// data port to out
		SET_MASK(LCD_DATA_DDR, LCD_DATA_MASK);
		CLEAR_MASK(LCD_DATA_PORT, LCD_DATA_MASK);
		// write mode
		CBI(LCD_SIG_PORT, LCD_RW);
		break;
	}

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

	LCD_CLEAR_STROB();
}

static void clearLcdMode()
{
	// data port to Hi-Z in
	CLEAR_MASK(LCD_DATA_DDR, LCD_DATA_MASK);
	CLEAR_MASK(LCD_DATA_PORT, LCD_DATA_MASK);

	// set RW, RS, E to zero
	CLEAR_MASK(LCD_SIG_PORT, LCD_SIG_MASK);
}

static void putNibble(const char nibble, LcdCommandType commandType)
{
	setLcdMode(commandType, LCD_WRITE);

	// set nibble to data port
	if (GBI(nibble, 0))
		SBI(LCD_DATA_PORT, LCD_DB4);
	if (GBI(nibble, 1))
		SBI(LCD_DATA_PORT, LCD_DB5);
	if (GBI(nibble, 2))
		SBI(LCD_DATA_PORT, LCD_DB6);
	if (GBI(nibble, 3))
		SBI(LCD_DATA_PORT, LCD_DB7);

	//LCD_STROB();

	LCD_SIG_PORT |= SFT(LCD_E);
	_delay_us(LCD_STROB_DELAY);
	LCD_SIG_PORT &= ~SFT(LCD_E);

	_delay_us(LCD_WRITE_DELAY);

	clearLcdMode();
}

static char getNibble(const LcdCommandType commandType)
{
	setLcdMode(commandType, LCD_READ);

	LCD_SET_STROB();
	_delay_us(LCD_READ_DELAY);

	char val = 0;
	if (GBI(LCD_DATA_PIN, LCD_DB4))
		val |= SFT(0);
	if (GBI(LCD_DATA_PIN, LCD_DB5))
		val |= SFT(1);
	if (GBI(LCD_DATA_PIN, LCD_DB6))
		val |= SFT(2);
	if (GBI(LCD_DATA_PIN, LCD_DB7))
		val |= SFT(3);

	LCD_CLEAR_STROB();

	clearLcdMode();

	return val;
}

static void putByte(const char byte, LcdCommandType commandType)
{
	waitWhileBusy();
	// high nibble
	putNibble(byte >> 4, commandType);
	// low nibble
	putNibble(byte, commandType);
}

static char getByte(LcdCommandType commandType)
{
	// high nibble
	char val = getNibble(commandType) << 4;
	// low nibble
	val |= getNibble(commandType);

	return val;
}

static inline void putCommand(const char command)
{
	putByte(command, LCD_COMMAND);
}

static inline void putData(const char data)
{
	putByte(data, LCD_DATA);
}

static inline char getCommand()
{
	return getByte(LCD_COMMAND);
}

//static inline char getData()
//{
	//return getByte(LCD_DATA);
//}

static void inline waitWhileBusy()
{
	for (; lcdIsBusy();) { /* do nothing */ }
}
