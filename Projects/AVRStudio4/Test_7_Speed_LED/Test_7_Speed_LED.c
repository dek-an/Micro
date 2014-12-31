#include "commonHeader.h"

#define TC_PRESCALER 1024
#define TC_PRESCALER_BITS 0x05 // 1:0:1
#define TC_ONE_SEC 37 //F_CPU / TC_PRESCALER / 256

// //////////////////////////////////////////////////////////

#define SS_TICKS_NUM 6 // Num of impulses per 1 meter

// //////////////////////////////////////////////////////////

#define SPEED_10 3 // m/sec
#define SPEED_20 6 // m/sec
#define SPEED_30 8 // m/sec
#define SPEED_40 11 // m/sec

// //////////////////////////////////////////////////////////

#define LED_PORT PORTB
#define LED_DDR DDRB
#define LED10_BIT 4
#define LED20_BIT 2
#define LED30_BIT 0
#define LED40_BIT 1
#define LED_MASK (SFT(LED10_BIT) | SFT(LED20_BIT) | SFT(LED30_BIT) | SFT(LED40_BIT))

#define SPEED_SENSOR_PORT PORTB
#define SPEED_SENSOR_DDR DDRB
#define SPEED_SENSOR_PIN PINB
#define SPEED_SENSOR_BIT 3

// //////////////////////////////////////////////////////////
//
//
static volatile uint16 m_ssTicks = 0;
static volatile uint08 m_speed = 0; // in m/sec

// //////////////////////////////////////////////////////////
// Interrupts
//
ISR(PCINT0_vect)
{
	++m_ssTicks;
}

//#if TC_ONE_SEC < 256
	static volatile uint08 m_overflows = 0;
//#else
//	static volatile uint16 m_overflows = 0;
//#endif
// every second
ISR(TIM0_OVF_vect)
{
	if (++m_overflows == TC_ONE_SEC)
	{
		m_overflows = 0;

		m_speed = m_ssTicks / SS_TICKS_NUM;
		m_ssTicks = 0;
	}
}

// //////////////////////////////////////////////////////////
//
//
static inline void init(void)
{
	DDRB = 0x00;
	PORTB = 0x00;

	// led ports
	MASK_SET(LED_DDR, LED_MASK);
	MASK_CLEAR(LED_PORT, LED_MASK);
	// speed sensor port to in with pull-up
	CBI(SPEED_SENSOR_DDR, SPEED_SENSOR_BIT);
	SBI(SPEED_SENSOR_PORT, SPEED_SENSOR_BIT);

	// timer
	TCCR0B |= TC_PRESCALER_BITS << CS00;
	TIMSK0 |= SFT(TOIE0); // overflow interrupt

	// external interrupts
	GIMSK |= SFT(PCIE); // PCINT
	PCMSK |= SFT(SPEED_SENSOR_BIT); // allow PCINT for speed sensor port

	// members
	m_ssTicks = 0;
	m_speed = 0; // in m/sec

	SEI();
}

int main(void)
{
	init();

	const uint08 ledMask = LED_MASK;

	MASK_SET(LED_PORT, ledMask);
	_delay_ms(3000);
	MASK_CLEAR(LED_PORT, ledMask);

	uint08 speed = 0;

	for (;;)
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			speed = m_speed;
		}

		uint08 currentMask = 0;
		if (speed >= SPEED_10)
		{
			SBI(currentMask, LED10_BIT);
			if (speed >= SPEED_20)
			{
				SBI(currentMask, LED20_BIT);
				if (speed >= SPEED_30)
				{
					SBI(currentMask, LED30_BIT);
					if (speed >= SPEED_40)
					{
						SBI(currentMask, LED40_BIT);
					}
				}
			}
		}

		MASK_CLEAR(LED_PORT, ledMask & (~currentMask));
		MASK_SET(LED_PORT, ledMask & currentMask);
	}

	return 0;
}
