#include <HAL.h>

inline void InitAll(void)
{

//InitUSART
UBRRL = LO(bauddivider);
UBRRH = HI(bauddivider);
UCSRA = 0;
UCSRB = 1<<RXEN|1<<TXEN|0<<RXCIE|0<<TXCIE;
UCSRC = 1<<URSEL|1<<UCSZ0|1<<UCSZ1;

//InitPort
LED_DDR |= 1<<LED1|1<<LED2|1<<LED3|1<<I_L|1<<I_C;


}



