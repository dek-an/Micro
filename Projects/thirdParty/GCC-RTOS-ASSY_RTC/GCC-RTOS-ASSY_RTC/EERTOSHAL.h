#ifndef EERTOSHAL_H
#define EERTOSHAL_H
#include <HAL.h>

#define STATUS_REG 			SREG
#define Interrupt_Flag		SREG_I
#define Disable_Interrupt	cli();
#define Enable_Interrupt	sei();

//RTOS Config
#define RTOS_ISR  			TIMER0_COMP_vect
#define	TaskQueueSize		30
#define MainTimerQueueSize	30

extern void RunRTOS (void);

#endif
