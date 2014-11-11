#ifndef EERTOSHAL_H
#define EERTOSHAL_H
#include <HAL.h>

#define STATUS_REG 			SREG
#define Interrupt_Flag		SREG_I
#define Disable_Interrupt	cli();
#define Enable_Interrupt	sei();

//RTOS Config
#define RTOS_ISR  			TIMER2_COMP_vect
#define	TaskQueueSize		20
#define MainTimerQueueSize	15

extern void RunRTOS (void);

#endif
