#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// ///////////////////////////////////////////////
// define
//
// LED1: USART; '1' - ON, '0' - OFF
#define LED1_PORT PORTD
#define LED1_DDR DDRD
#define LED1 7
// LED2: always blink (2 bl/sec); TC0
#define LED2_PORT PORTD
#define LED2_DDR DDRD
#define LED2_PIN PIND
#define LED2 4
// LED3: always blink (1 bl/sec); TC1
#define LED3_PORT PORTD
#define LED3_DDR DDRD
#define LED3_PIN PIND
#define LED3 5
// TC 0
#define TC0_PRESCALER 5 // prescaler = 1024; CS02:CS01:CS00 - 1:0:1
#define TC0_PRESCALER_NUM 1024
// TC 1
#define TC1_PRESCALER 5 // prescaler = 1024; CS02:CS01:CS00 - 1:0:1
#define TC1_PRESCALER_NUM 1024

// ///////////////////////////////////////////////
// macroses
//
#define HI(x) ((x) >> 8)
#define LO(x) ((x) & 0xFF)

// set
#define sbi(port, pin) ((port) |= _BV(pin))
// clear
#define cbi(port, pin) ((port) &= ~(_BV(pin)))
// toggle
#define tbi(port, pin) ((port) ^= _BV(pin))
// get
#define gbi(port, pin) ((port) >> (pin)) & 1

// ///////////////////////////////////////////////
// functions
//
void init(void);
void initUSART(void);
void initPorts(void);
void initGlobal(void);
void initTimers(void);

// ///////////////////////////////////////////////
// interrupts
//
// USART, Rx Complete
ISR(USARTRXC_vect);
// USART, Tx Complete
ISR(USARTTXC_vect);
// Timer/Counter0 Overflow
ISR(TIMER0_OVF_vect);
// Timer/Counter1 Compare Match A
ISR(TIMER1_COMPA_vect);

// ///////////////////////////////////////////////
// main
//
int main(void)
{
	init();

	volatile char tmp = 0;
	while (1)
	{
		// do nothing
		++tmp;
	}

	return 0;
}


// ///////////////////////////////////////////////
// functions implementation
//
void init(void)
{
	initUSART();
	initPorts();
	initGlobal();
	initTimers();
}

void initUSART(void)
{
#define BAUD_RATE 9600L
#define BAUD_VALUE F_CPU / (16 * BAUD_RATE) - 1

	// set Baud Rate Register
	//
	UBRRL = LO(BAUD_VALUE);

	char highBaudVal = HI(BAUD_VALUE);
	// Register Select UBRRH (URSEL = 0)
	highBaudVal &= ~(_BV(URSEL));
	// Reserved 14:12(6:4) Bits in UBRRH
	highBaudVal &= ~(_BV(6) | _BV(5) | _BV(4));

	UBRRH = highBaudVal;

	// set Control and Status Registers
	//
	UCSRA = 0;

	// Receive Enable (RXEN)
	// Receive Complete Interrupt Enable (RXCIE)
	// Transmitt Enable (TXEN)
	// Transmitt Complete Interrupt Enable (TXCIE)
	UCSRB = _BV(RXEN) | _BV(RXCIE) | _BV(TXEN) | _BV(TXCIE);

	// Register Select UCSRC (URSEL = 1)
	// Character Size 8
	//     see table from datasheet:
	//         UCSZ2 = 0 (in UCSRB), UCSZ1 = 1, UCSZ0 = 1
	// Mode Select Asynchronous (UMSEL = 0)
	UCSRC = _BV(URSEL) | _BV(UCSZ1) | _BV(UCSZ0);
}

void initPorts(void)
{
	// port direction of LED1 pin is OUT
	sbi(LED1_DDR, LED1);
	// initial value of LED1 pin is one
	sbi(LED1_PORT, LED1);

	// port direction of LED2 pin is OUT
	sbi(LED2_DDR, LED2);
	// initial value of LED2 pin is zero
	cbi(LED2_PORT, LED2);

	// port direction of LED3 pin is OUT
	sbi(LED3_DDR, LED3);
	// initial value of LED3 pin is zero
	cbi(LED3_PORT, LED3);
}

void initGlobal(void)
{
	sei();
}

void initTimers(void)
{
	// TC0 - LED2 blinking
	// TCCR0 - Timer Counter Control Register
	// prescaler = 1024; CS02:CS01:CS00 - 1:0:1
	TCCR0 = TC0_PRESCALER;
	// TC Interrupt Mask Register (TCMSK): set TC0 Overflow Interrupt Enable
	sbi(TIMSK, TOIE0);

	// TC1 - LED3 blinking
	// TCCR1B - Timer Counter Control Register
	// prescaler = 1024; CS02:CS01:CS00 - 1:0:1
	TCCR1B = TC1_PRESCALER;

	// OCR1A - Output Compare Register A
#define TC1_WORK_PERIOD_PER_SEC 1
#define TC1_WORK_TICK F_CPU / TC1_PRESCALER_NUM / TC1_WORK_PERIOD_PER_SEC
	OCR1AH = HI(TC1_WORK_TICK);
	OCR1AL = LO(TC1_WORK_TICK);
	// set Output Compare A Interrupt Enable
	sbi(TIMSK, OCIE1A);
}


// ///////////////////////////////////////////////
// interrupts implementation
//
// USART, Rx Complete
ISR(USARTRXC_vect)
{
	unsigned char udrVal = UDR;

	switch (udrVal)
	{
		case '1':
			sbi(LED1_PORT, LED1);
			break;
		case '0':
			cbi(LED1_PORT, LED1);
			break;
	}

	UDR = udrVal;
}

// USART, Tx Complete
// TODO: for debug
ISR(USARTTXC_vect)
{
	// do nothing
	UDR;
}

// Timer/Counter0 Overflow
ISR(TIMER0_OVF_vect)
{
// work each 0.5 second
#define OVF_DIVIDER 256
#define TC0_WORK_PERIOD_PER_SEC 2
#define TC0_WORK_TICK F_CPU / TC0_PRESCALER_NUM / TC0_WORK_PERIOD_PER_SEC / OVF_DIVIDER

	UDR = 'Z';

	static int tcCnt = 0;

	if (++tcCnt == TC0_WORK_TICK)
	{
		tbi(LED2_PORT, LED2);
		tcCnt = 0;

		UDR = 'X';
	}
}

// Timer/Counter1 Compare Match A
ISR(TIMER1_COMPA_vect)
{
	UDR = 'C';
	tbi(LED3_PORT, LED3);
	// reset TC High and Low Registers
	TCNT1H = 0;
	TCNT1L = 0;
}
