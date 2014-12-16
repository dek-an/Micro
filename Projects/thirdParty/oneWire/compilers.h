//***************************************************************************
//
//  Author(s)...: Pashgan    http://ChipEnable.Ru   
//
//  Target(s)...: ATMega8535
//
//  Compiler....: IAR, GCC, CodeVision
//
//  Description.: файл для портирования проектов, исходников библиотек
//
//  Data........: 6.10.10 
//
//***************************************************************************
#ifndef COMPILERS_H
#define COMPILERS_H

#include <common/commonHeader.h>

//________________________________________
#ifdef  __ICCAVR__
#include <ioavr.h>
#include <inavr.h>
#include <intrinsics.h>
#endif

//________________________________________
#ifdef  __GNUC__
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define __save_interrupt() SREG
#define __restore_interrupt(var) SREG = (var)
#define __disable_interrupt() cli()
#define __enable_interrupt() sei()
#define __delay_cycles(var) _delay_us((unsigned int)(var)/(F_CPU/1000000))
#endif

//________________________________________
#ifdef __CODEVISIONAVR__
#include <io.h>
#include <delay.h>

#define __save_interrupt() SREG
#define __restore_interrupt(var) SREG = (var)
#define __disable_interrupt() #asm("cli")
#define __enable_interrupt() #asm("sei")
#define __delay_cycles(var) delay_us((unsigned int)(var)/(_MCU_CLOCK_FREQUENCY_/1000000))
#endif

#endif //COMPILERS_H

