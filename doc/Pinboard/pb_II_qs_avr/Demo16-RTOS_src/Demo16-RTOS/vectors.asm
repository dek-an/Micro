			.CSEG

			.ORG $000			; (RESET) External Pin, Power-on Reset, Brown-out Reset, Watchdog Reset and JTAG AVR Reset
			RJMP	Reset

			.ORG $002 
			RETI 				; (INT0) External Interrupt Request 0

			.ORG $004 
			RETI 				; (INT1) External Interrupt Request 1

			.ORG $006 
			RJMP	OutComp2Int	; (TIMER2 COMP) Timer/Counter2 Compare Match

			.ORG $008 
			RETI 				; (TIMER2 OVF) Timer/Counter2 Overflow

			.ORG $00A 
			RETI 				; (TIMER1 CAPT) Timer/Counter1 Capture Event

			.ORG $00C 
			RETI 				; (TIMER1 COMPA) Timer/Counter1 Compare Match A

			.ORG $00E 
			RETI 				; (TIMER1 COMPB) Timer/Counter1 Compare Match B

			.ORG $010 
			RETI 				; (TIMER1 OVF) Timer/Counter1 Overflow

			.ORG $012 
			RETI				; (TIMER0 OVF) Timer/Counter0 Overflow

			.ORG $014 
			RETI				; (SPI,STC) Serial Transfer Complete

			.ORG $016 
			RJMP	RX_OK		; (USART,RXC) USART, Rx Complete

			.ORG $018 
			RETI 				; (USART,UDRE) USART Data Register Empty

			.ORG $01A 
			RETI 				; (USART,TXC) USART, Tx Complete

			.ORG $01C 
			RJMP	ADC_INT		; (ADC) ADC Conversion Complete

			.ORG $01E 
			RETI 				; (EE_RDY) EEPROM Ready

			.ORG $020 
			RETI 				; (ANA_COMP) Analog Comparator

			.ORG $022 
			RETI 				; (TWI) 2-wire Serial Interface

			.ORG $024 
			RETI 				; (INT2) External Interrupt Request 2

			.ORG $026 
			RETI 				; (TIMER0 COMP) Timer/Counter0 Compare Match

			.ORG $028 
			RETI 				; (SPM_RDY) Store Program Memory Ready
