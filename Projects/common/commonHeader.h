#ifndef _COMMONHEADER_H_
#define _COMMONHEADER_H_

#ifndef F_CPU
#	define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

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
#define GBI(byte, bit) ((byte) >> (bit)) & 1

#define MASK_SET(byte, mask) (byte) |= (mask)
#define MASK_CLEAR(byte, mask) (byte) &= ~(mask)

#define INTERRUPTS_ENABLED GBI(STATUS_REG, INTERRUPTS_FLAG)

// ////////////////////////////////////////
// Constants
//
static const uchar EMPTY_STRING[] PROGMEM = "";

#endif // _COMMONHEADER_H_
