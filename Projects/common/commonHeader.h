#ifndef _COMMONHEADER_H_
#define _COMMONHEADER_H_

#include <avr/io.h>
#include <avr/interrupt.h>

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
typedef unsigned char uint08;
typedef unsigned short uint16;
typedef unsigned long uint32;
typedef unsigned long long uint64;

// ////////////////////////////////////////
// Macroses
//
#define SFT(x) _BV(x)

#define SBI(byte, bit) (byte) |= SFT(bit)
#define CBI(byte, bit) (byte) &= ~SFT(bit)
#define TBI(byte, bit) (byte) ^= SFT(bit)
#define GBI(byte, bit) ((byte) >> (bit)) & 1

#define INTERRUPTS_ENABLED GBI(STATUS_REG, INTERRUPTS_FLAG)

#endif // _COMMONHEADER_H_
