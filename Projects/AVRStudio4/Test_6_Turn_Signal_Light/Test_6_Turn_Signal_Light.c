#include "commonHeader.h"

#ifndef F_CPU
#	define F_CPU 1200000UL
#endif

#define TC_PRESCALER 64
#define TC_PRESCALER_BITS 0x03 // 0:1:1
#define TC_ONE_SEC 73 //F_CPU / TC_PRESCALER / 256

// //////////////////////////////////////////////////////////

#define SS_IMPULSES_NUM 6 // Num of impulses per 1 meter
#define SPEED_THRESHOLD 9 // in m/sec, 32.4 km/h
#define LIGHT_TIME_LENGTH 3 // in sec

// //////////////////////////////////////////////////////////

#define LEFT_ALARM_PORT PORTB
#define LEFT_ALARM_DDR DDRB
#define LEFT_ALARM_PIN PINB
#define LEFT_ALARM_BIT 0

#define RIGHT_ALARM_PORT PORTB
#define RIGHT_ALARM_DDR DDRB
#define RIGHT_ALARM_PIN PINB
#define RIGHT_ALARM_BIT 2

#define LEFT_LIGHT_PORT PORTB
#define LEFT_LIGHT_DDR DDRB
#define LEFT_LIGHT_PIN PINB
#define LEFT_LIGHT_BIT 3

#define RIGHT_LIGHT_PORT PORTB
#define RIGHT_LIGHT_DDR DDRB
#define RIGHT_LIGHT_PIN PINB
#define RIGHT_LIGHT_BIT 4

#define SPEED_SENSOR_PORT PORTB
#define SPEED_SENSOR_DDR DDRB
#define SPEED_SENSOR_PIN PINB
#define SPEED_SENSOR_BIT 1

// //////////////////////////////////////////////////////////

#define LEFT_ALARM_IS_ON_BIT 0
#define LEFT_ALARM_IS_ON() GBI(m_flag, LEFT_ALARM_IS_ON_BIT)
#define LEFT_ALARM_IS_ON_ON() SBI(m_flag, LEFT_ALARM_IS_ON_BIT)
#define LEFT_ALARM_IS_ON_OFF() CBI(m_flag, LEFT_ALARM_IS_ON_BIT)

#define RIGHT_ALARM_IS_ON_BIT 1
#define RIGHT_ALARM_IS_ON() GBI(m_flag, RIGHT_ALARM_IS_ON_BIT)
#define RIGHT_ALARM_IS_ON_ON() SBI(m_flag, RIGHT_ALARM_IS_ON_BIT)
#define RIGHT_ALARM_IS_ON_OFF() CBI(m_flag, RIGHT_ALARM_IS_ON_BIT)

// //////////////////////////////////////////////////////////
//
//
static volatile uint08 m_ssTicks = 0;
static volatile uint08 m_distance = 0; // in m
static volatile uint08 m_speed = 0; // in m/sec
static volatile uint08 m_leftTime = 0;
static volatile uint08 m_rightTime = 0;
static volatile uint08 m_flag = 0;

// //////////////////////////////////////////////////////////
// Interrupts
//
ISR(INT0_vect)
{
	if (GBI(SPEED_SENSOR_PIN, SPEED_SENSOR_BIT))
	{
		m_ssTicks = (++m_ssTicks) % SS_IMPULSES_NUM;
		if (!m_ssTicks)
		{
			++m_distance;
		}
	}
}

ISR(PCINT0_vect)
{
	if (GBI(LEFT_ALARM_PIN, LEFT_ALARM_BIT))
	{
		m_leftTime = LIGHT_TIME_LENGTH;
		m_rightTime = 0;
		LEFT_ALARM_IS_ON_ON();
	}
	else
	{
		LEFT_ALARM_IS_ON_OFF();
	}

	if (GBI(RIGHT_ALARM_PIN, RIGHT_ALARM_BIT))
	{
		m_rightTime = LIGHT_TIME_LENGTH;
		m_leftTime = 0;
		RIGHT_ALARM_IS_ON_ON();
	}
	else
	{
		RIGHT_ALARM_IS_ON_OFF();
	}

	if (LEFT_ALARM_IS_ON() && RIGHT_ALARM_IS_ON())
	{
		m_leftTime = m_rightTime = 0;
	}
}

#if TC_ONE_SEC < 256
	static volatile uint08 m_overflows = 0;
#else
	static volatile uint16 m_overflows = 0;
#endif
// every second
ISR(TIM0_OVF_vect)
{
	if (++m_overflows == TC_ONE_SEC)
	{
		m_overflows = 0;

		m_speed = m_distance;
		m_distance = 0;

		if (m_leftTime)
		{
			--m_leftTime;
		}

		if (m_rightTime)
		{
			--m_rightTime;
		}
	}
}

// //////////////////////////////////////////////////////////
//
//
static inline void init(void)
{
	// ports
	DDRB = SFT(LEFT_LIGHT_BIT) | SFT(RIGHT_LIGHT_BIT);
	PORTB = 0x00;

	// timer
	TCCR0B |= TC_PRESCALER_BITS << CS00;
	TIMSK0 |= SFT(TOIE0); // overflow interrupt

	// external interrupts
	GIMSK |= SFT(INT0) | SFT(PCIE); // INT0; PCINT
	MCUCR |= SFT(ISC00); // INT0; any logical change
	PCMSK |= SFT(LEFT_ALARM_BIT) | SFT(RIGHT_ALARM_BIT);

	// members
	m_ssTicks = 0;
	m_distance = 0; // in m
	m_speed = 0; // in m/sec
	m_leftTime = 0;
	m_rightTime = 0;
	m_flag = 0;

	SEI();
}

int main(void)
{
	init();

	uint08 leftTime = 0;
	uint08 rightTime = 0;
	uint08 speed = 0;

	for (;;)
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			leftTime = m_leftTime;
			rightTime = m_rightTime;
			speed = m_speed;
		}

		if (speed < SPEED_THRESHOLD)
		{
			if (leftTime)
			{
				SBI(LEFT_LIGHT_PORT, LEFT_LIGHT_BIT);
			}
			else
			{
				CBI(LEFT_LIGHT_PORT, LEFT_LIGHT_BIT);
			}

			if (rightTime)
			{
				SBI(RIGHT_LIGHT_PORT, RIGHT_LIGHT_BIT);
			}
			else
			{
				CBI(RIGHT_LIGHT_PORT, RIGHT_LIGHT_BIT);
			}
		}
	}

	return 0;
}
