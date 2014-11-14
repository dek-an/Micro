#ifndef _LCDWH1602BDEF_H
#define _LCDWH1602BDEF_H

#include <avr/io.h>

// data port
#define LCD_DATA_PORT PORTB
#define LCD_DATA_DDR DDRB
#define LCD_DATA_PIN PINB

// data pins numbers
#define LCD_DB4 3
#define LCD_DB5 4
#define LCD_DB6 5
#define LCD_DB7 6

// signal port
#define LCD_SIG_PORT PORTB
#define LCD_SIG_DDR DDRB
#define LCD_SIG_PIN PINB

// signal pins numbers
#define LCD_RS 0
#define LCD_RW 1
#define LCD_E 2

#endif // _DEFINESLCDWH1602B_H
