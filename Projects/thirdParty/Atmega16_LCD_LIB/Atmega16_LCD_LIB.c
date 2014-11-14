

/* Автор:              Papandopala Papandopalavich
 * Имя файла:          Atmega16_LCD_LIB.c
 * Тип МК:			   Atmega16, Flash 16 Kbytes, SRAM 1 Kbytes, EEPROM 512 bytes.
 * Тактовая частота:   F_CPU 8000000 Гц
 * Дата:               28.06.2013 21:17:29
 * Версия ПО:          ATMEL STUDIO VERSION 6.1.2562
 * FUSES:              HIGH xx, LOW xx;
 * Описание:           Тест библиотеки для LCD дисплея.*/ 
                       

#ifndef F_CPU
#define  F_CPU 8000000UL  //Частота кварца
#endif
#include <avr/io.h>       //Выбор типа микроконтроллера.
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>//Хедер прерываний. 
#include "LCD.h"          //Хедер для LCD дисплея
#include "prototip_fun.h" //Прототипы функций


void t(void)
{
	_delay_ms(250);
}
void l(void)
{
	_delay_ms(150);
}
ISR(TIMER0_COMP_vect)
{
	TCNT0=0;
}


int main(void)
{
			init();			//Инициализация МК и периферии.
	        
		
    while(1)
    {
		uint8_t i=0;
		
		#if 1
				LCDGotoXY(2,0);
				t();
				LCDdata(0x50);//P
				t();
				LCDdata(0x61);//а
				t();
				LCDdata(0xb7);//з
				t();
				LCDdata(0x90);//
				LCDdata(0xe3);//д
				t();
				LCDdata(0xb3);//в
				t();
				LCDdata(0x61);//а
				t();
				LCDdata(0x90);//
				LCDdata(0x70);//р
				t();
				LCDdata(0x61);//а
				t();
				LCDdata(0xb7);//з
				t();
		
				LCDGotoXY(4,1);
		
				LCDdata(0xb6);//ж
				t();
				LCDdata(0xb8);//и
				t();
				LCDdata(0x70);//р
				t();
				LCDdata(0x6f);//о
				t();
				LCDdata(0xbf);//т
				t();
				LCDdata(0x70);//р
				t();
				LCDdata(0xc7);//я
				t();
				LCDdata(0x63);//с
				_delay_ms(1000);
				LCDclear();
		#endif
		
		#if 1
			while(i<13)
				{
					LCDscreenr();
					l();
					i++;
				}
		
				i=26;
				while(i!=0)
				{
					LCDscreenl();
					l();
					i--;
				}
		
				while(i<13)
				{
					LCDscreenr();
					l();
					i++;
				}
				LCDclear();
		#endif
		
		#if 1
				LCDGotoXY(11,1);
				LCDacl();
		
				t();
				LCDdata(16);//с
				t();
				LCDdata(16);//я
				t();
				LCDdata(16);//р
				t();
				LCDdata(16);//т
				t();
				LCDdata(16);//о
				t();
				LCDdata(16);//р
				t();
				LCDdata(16);//и
				t();
				LCDdata(16);//ж
				t();
		
				LCDGotoXY(12,0);
		
				LCDdata(16);//з
				t();
				LCDdata(16);//а
				t();
				LCDdata(16);//р
				t();
				LCDdata(16);//
				LCDdata(16);//а
				t();
				LCDdata(16);//в
				t();
				LCDdata(16);//д
				t();
				LCDdata(16);//
				LCDdata(16);//з
				t();
				LCDdata(16);//а
				t();
				LCDdata(16);//P
				_delay_ms(1000);
				LCDacr();
				LCDclear();
		#endif
    }
}

