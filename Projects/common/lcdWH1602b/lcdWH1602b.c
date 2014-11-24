#include "lcdWH1602b.h"
#include "lcdWH1602bDef.h"

#include <stdio.h>

#define LCD_USE_THIRDPARTY

// //////////////////////////////////////////////////////////
// ThirdParty
//
#ifdef LCD_USE_THIRDPARTY

#include <thirdParty/Atmega16_LCD_LIB/LCD.h>

void initLcd(void)
{
	INITIALIZE_CHECKING();

	LCDinit();
}

BOOL lcdIsBusy(void)
{
	return FALSE;
}

void waitWhileBusy()
{
	for (; lcdIsBusy();) { /* do nothing */ }
}

void lcdWriteChar(const char c)
{
	LCDdata(c);
}

void lcdWriteStr(const char* str)
{
	LCDstr((char*)str);
}

void lcdClear(void)
{
	LCDcommand(0b00000001);
}

void lcdGoTo(uint08 line, uint08 col)
{
	LCDGotoXY(col, line);
}

void lcdWriteStrProgMem(const char* str)
{
	LCDstr_of_flash((const uint8_t*)str);
}

static char buffer[6];

void lcdWriteUint08(uint08 val)
{
	sprintf(buffer, "%d", val);
	lcdWriteStr(buffer);
}

void lcdWriteUint16(uint16 val)
{
	sprintf(buffer, "%d", val);
	lcdWriteStr(buffer);
}

#else // !LCD_USE_THIRDPARTY
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
static void putNibble(const char nibble);
static char getNibble();
static void putByte(const char byte, const LcdCommandType commandType);
static char getByte(const LcdCommandType commandType);
static void inline putCommand(const char command);
static void inline putData(const char data);
static inline char getCommand();
//static inline char getData();

// ///////////////////////////////////////////////
// LCD Defines
//
#define LCD_DATA_MASK (SFT(LCD_DB7) | SFT(LCD_DB6) | SFT(LCD_DB5) | SFT(LCD_DB4))
#define LCD_SIG_MASK (SFT(LCD_RW) | SFT(LCD_RS) | SFT(LCD_E))
#define LCD_BF LCD_DB7

// //////////////////////////////////////////////////////////
// Interface
//
inline void initLcd(void)
{
	INITIALIZE_CHECKING();

	// wait for 15 ms
	_delay_ms(100);

	// signal port to out; don't touch signal DDR anymore
	MASK_SET(LCD_SIG_DDR, LCD_SIG_MASK);
	clearLcdMode();

	setLcdMode(LCD_COMMAND, LCD_WRITE);

	// 1
	putNibble(0b0011);
	_delay_us(4100);
	// 2
	putNibble(0b0011);
	_delay_us(100);
	// 3
	putNibble(0b0011);

	//waitWhileBusy();

	// 4-bit
	putNibble(0b0010);

	clearLcdMode();

	// 4-bit; 2 lines; 5x11 dots
	putCommand(0b00101100);
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
		return !FALSE;

	return FALSE;
}

void waitWhileBusy()
{
	for (; lcdIsBusy();) { /* do nothing */ }
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
	putCommand((((line & 1) * 0x40) + (col & 15)) | 0b10000000);
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
		MASK_CLEAR(LCD_DATA_DDR, LCD_DATA_MASK);
		MASK_SET(LCD_DATA_PORT, LCD_DATA_MASK);
		// read mode
		SBI(LCD_SIG_PORT, LCD_RW);
		break;
	case LCD_WRITE:
		// data port to out
		MASK_SET(LCD_DATA_DDR, LCD_DATA_MASK);
		MASK_CLEAR(LCD_DATA_PORT, LCD_DATA_MASK);
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
	MASK_CLEAR(LCD_DATA_DDR, LCD_DATA_MASK);
	MASK_CLEAR(LCD_DATA_PORT, LCD_DATA_MASK);

	// set RW, RS, E to zero
	MASK_CLEAR(LCD_SIG_PORT, LCD_SIG_MASK);
}

static void putNibble(const char nibble)
{
	// ports should be setted!
	MASK_CLEAR(LCD_DATA_PORT, LCD_DATA_MASK);

	// set nibble to data port
	if (GBI(nibble, 0))
		SBI(LCD_DATA_PORT, LCD_DB4);
	if (GBI(nibble, 1))
		SBI(LCD_DATA_PORT, LCD_DB5);
	if (GBI(nibble, 2))
		SBI(LCD_DATA_PORT, LCD_DB6);
	if (GBI(nibble, 3))
		SBI(LCD_DATA_PORT, LCD_DB7);

	LCD_STROB();

	_delay_us(LCD_WRITE_DELAY);
}

static char getNibble()
{
	// ports should be setted!

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

	return val;
}

static void putByte(const char byte, LcdCommandType commandType)
{
	waitWhileBusy();

	setLcdMode(commandType, LCD_WRITE);

	// high nibble
	putNibble(byte >> 4);
	// low nibble
	putNibble(byte);

	clearLcdMode();
}

static char getByte(LcdCommandType commandType)
{
	setLcdMode(commandType, LCD_READ);

	// high nibble
	char val = getNibble() << 4;
	// low nibble
	val |= getNibble();

	clearLcdMode();

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
	getByte(LCD_COMMAND);
	return getByte(LCD_COMMAND);
}

//static inline char getData()
//{
	//return getByte(LCD_DATA);
//}
#endif // LCD_USE_THIRDPARTY
