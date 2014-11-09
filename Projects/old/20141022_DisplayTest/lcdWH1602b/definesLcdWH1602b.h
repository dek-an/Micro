#ifndef _DEFINESWH1602B_H
#define _DEFINESWH1602B_H

#include <avr/io.h>

// data port
#define LCD_DATA_PORT PORTB
#define LCD_DATA_DDR DDRB
#define LCD_DATA_PIN PINB

// signal port
#define LCD_SIG_PORT PORTA
#define LCD_SIG_DDR DDRA
#define LCD_SIG_PIN PINA

// signals numbers
#define LCD_RS 0
#define LCD_RW 1
#define LCD_E 2

#endif // _DEFINESWH1602B_H
