

/* �����:              Papandopala Papandopalavich
 * ��� �����:          Atmega16_LCD_LIB.c
 * ��� ��:			   Atmega16, Flash 16 Kbytes, SRAM 1 Kbytes, EEPROM 512 bytes.
 * �������� �������:   F_CPU 8000000 ��
 * ����:               28.06.2013 21:17:29
 * ������ ��:          ATMEL STUDIO VERSION 6.1.2562
 * FUSES:              HIGH xx, LOW xx;
 * ��������:           ���� ���������� ��� LCD �������.*/ 
                       

#ifndef F_CPU
#define  F_CPU 8000000UL  //������� ������
#endif
#include <avr/io.h>       //����� ���� ����������������.
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>//����� ����������. 
#include "LCD.h"          //����� ��� LCD �������
#include "prototip_fun.h" //��������� �������


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
			init();			//������������� �� � ���������.
	        
		
    while(1)
    {
		uint8_t i=0;
		
		#if 1
				LCDGotoXY(2,0);
				t();
				LCDdata(0x50);//P
				t();
				LCDdata(0x61);//�
				t();
				LCDdata(0xb7);//�
				t();
				LCDdata(0x90);//
				LCDdata(0xe3);//�
				t();
				LCDdata(0xb3);//�
				t();
				LCDdata(0x61);//�
				t();
				LCDdata(0x90);//
				LCDdata(0x70);//�
				t();
				LCDdata(0x61);//�
				t();
				LCDdata(0xb7);//�
				t();
		
				LCDGotoXY(4,1);
		
				LCDdata(0xb6);//�
				t();
				LCDdata(0xb8);//�
				t();
				LCDdata(0x70);//�
				t();
				LCDdata(0x6f);//�
				t();
				LCDdata(0xbf);//�
				t();
				LCDdata(0x70);//�
				t();
				LCDdata(0xc7);//�
				t();
				LCDdata(0x63);//�
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
				LCDdata(16);//�
				t();
				LCDdata(16);//�
				t();
				LCDdata(16);//�
				t();
				LCDdata(16);//�
				t();
				LCDdata(16);//�
				t();
				LCDdata(16);//�
				t();
				LCDdata(16);//�
				t();
				LCDdata(16);//�
				t();
		
				LCDGotoXY(12,0);
		
				LCDdata(16);//�
				t();
				LCDdata(16);//�
				t();
				LCDdata(16);//�
				t();
				LCDdata(16);//
				LCDdata(16);//�
				t();
				LCDdata(16);//�
				t();
				LCDdata(16);//�
				t();
				LCDdata(16);//
				LCDdata(16);//�
				t();
				LCDdata(16);//�
				t();
				LCDdata(16);//P
				_delay_ms(1000);
				LCDacr();
				LCDclear();
		#endif
    }
}

