#include <HAL.h>

inline void InitAll(void)
{

//InitUSART
UBRRL = LO(bauddivider);
UBRRH = HI(bauddivider);
UCSRA = 0;
UCSRB = 1<<RXEN|1<<TXEN|0<<RXCIE|0<<TXCIE|0<<UDRIE;
UCSRC = 1<<URSEL|1<<UCSZ0|1<<UCSZ1;

//InitPort


//Init ADC


//Init PWM


//Init Interrupts


//Init Timers


}
