; ��� ��������� ������������� �����
			CLEAR_RAM			; ������ ������� ���������� � ���������
	
			USART_INIT			; ������ USART
	
			INIT_RTOS			; ���������� � ������� �� - ������� � ������������� ��������

			OUTI	TIMSK,0<<TOIE0|1<<OCF2|0<<TOIE0		; ��������� ���������� RTOS - ������ ��
			
			OUTI	ADMUX,1<<REFS0|1<<ADLAR|0<<MUX0		; 5v REF, ADC0, 8bit - ������������� ���
		
			INIT_LCD			; ������������� � ������ �������. ������������ ������� � ��� ������� � �����
								; lcd4.asm � lcd4_macro.inc

;������������� ������ ����� ������. � ��������� ������.
			SBI		BTA_P,BTA
			SBI		BTB_P,BTB
			SBI		BTC_P,BTC
			
			CBI		BTA_D,BTA
			CBI		BTB_D,BTB
			CBI		BTC_D,BTC
			

; ������������� ����� �������. 
			SBI		PZ_D,PZ
			CBI		PZ_P,PZ

			SBI		PWML_D,PWML
			CBI		PWML_P,PWML

			SBI		PWML_D,PWML2
			CBI		PWML_P,PWML2

			
			OUTI	OCR1AH,0
			OUTI	OCR1BH,0

			OUTI	OCR1AL,127
			OUTI	OCR1BL,127

			OUTI	TCCR1A,1<<COM1A1| 1<<COM1B1|1<<WGM10
			OUTI	TCCR1B,1<<CS10|1<<WGM12

			LDI		OSRG,-1
			STS		PWM_L_V,OSRG

			LDI		OSRG,1
			STS		PWM_L_V2,OSRG


