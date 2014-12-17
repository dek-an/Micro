#ifndef _COMMONHEADER_H_
#define _COMMONHEADER_H_

#ifndef F_CPU
#	define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>

#include <util/atomic.h>
#include <util/delay.h>


// ////////////////////////////////////////
// Controller defines
//
#define STATUS_REG SREG
#define INTERRUPTS_FLAG SREG_I

// ////////////////////////////////////////
// Controller macroses
//
#define CLI() cli()
#define SEI() sei()

// ////////////////////////////////////////
// Typedefs
//
typedef unsigned char BOOL;
typedef unsigned char uchar;
typedef /*unsigned char*/uint8_t uint08;
typedef /*unsigned short*/uint16_t uint16;
typedef /*unsigned long*/uint32_t uint32;
typedef /*unsigned long long*/uint64_t uint64;

#define FALSE 0x00

// ////////////////////////////////////////
// Macroses
//
#define SFT(x) _BV(x)

#define SBI(byte, bit) (byte) |= SFT(bit)
#define CBI(byte, bit) (byte) &= ~SFT(bit)
#define TBI(byte, bit) (byte) ^= SFT(bit)
#define GBI(byte, bit) (((byte) >> (bit)) & 1)

#define MASK_SET(byte, mask) (byte) |= (mask)
#define MASK_CLEAR(byte, mask) (byte) &= ~(mask)
#define MASK_TOGGLE(byte, mask) (byte) ^= (mask)
#define MASK_GET(byte, mask) ((byte) & (mask))

#define INTERRUPTS_ENABLED GBI(STATUS_REG, INTERRUPTS_FLAG)

#define INITIALIZE_CHECKING() \
	static BOOL isInitialized = FALSE; \
	if (isInitialized) \
		return; \
	isInitialized = !FALSE

#define DECLARE_FLAG_BIT(flag, bit_name, bit_num) \
	static const uchar bit_name##_BIT = bit_num; \
	static inline BOOL bit_name() { return GBI(flag, bit_num); } \
	static inline void bit_name##_ON() { SBI(flag, bit_num); } \
	static inline void bit_name##_OFF() { CBI(flag, bit_num); }

#define DECLARE_EEPROM_VAR(varType, varName) \
	extern varType get##varName(void); \
	extern void set##varName(const varType);

#define DECLARE_EEPROM_VAR_METHODS(varType, eepromType, varName, defaultVal) \
	static varType m_##varName EEMEM = defaultVal; \
	static varType m_local##varName = defaultVal; \
	void init##varName(void) { m_local##varName = eeprom_read_##eepromType(&m_##varName); } \
	varType get##varName(void) { return m_local##constName; } \
	void set##varName(const varType val) { eeprom_update_##eepromType(&m_##varName, val); m_local##varName = val; }

// ////////////////////////////////////////
// Constants
//
static const char EMPTY_STRING[] PROGMEM = "";
static const char PROGMEM_DIGITS[] PROGMEM = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};
#define GET_PROGMEM_DIGIT(digit) pgm_read_byte(&PROGMEM_DIGITS[digit])

#endif // _COMMONHEADER_H_
