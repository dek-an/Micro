; Тут первичная инициализация всего
			CLEAR_RAM			; Полная очистка оперативки и регистров
	
			USART_INIT			; Запуск USART
	
			INIT_RTOS			; Подготовка к запуску ОС - очистка и инициализация очередей

			OUTI	TIMSK,0<<TOIE0|1<<OCF2|0<<TOIE0		; Разрешаем прерывание RTOS - запуск ОС
			
			OUTI	ADMUX,1<<REFS0|1<<ADLAR|0<<MUX0		; 5v REF, ADC0, 8bit - конфигурируем АЦП
		
			INIT_LCD			; Инициализация и запуск дисплея. Конфигурация дисплея и его выводов в файле
								; lcd4.asm и lcd4_macro.inc

;Инициализация портов ввода вывода. В частности кнопок.
			SBI		BTA_P,BTA
			SBI		BTB_P,BTB
			SBI		BTC_P,BTC
			
			CBI		BTA_D,BTA
			CBI		BTB_D,BTB
			CBI		BTC_D,BTC
			

; Инициализация порта пищалки. 
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


