#include "commonHeader.h"

#define TC_PRESCALER 1024
#define TC_PRESCALER_BITS 0x05 // 1:0:1
#define TC_ONE_SEC 37 //F_CPU / TC_PRESCALER / 256

// //////////////////////////////////////////////////////////

// Should be used as: speed_in_m_per_sec = impulses_within_1_sec / SS_IMPULSES_NUM
// But we will use: speed = impulses_within_1_sec >> 3 <=> speed = impulses_within_1_sec / 8.
// So speed_in_m_per_sec = speed * (8 / 6) = 1.334 * speed
//#define SS_IMPULSES_NUM 6 // Num of impulses per 1 meter
#define SPEED_THRESHOLD 8 // in 1.334m/sec, 8 * 1.334 * 3600 / 1000 = 38.4 km/h
#define LIGHT_TIME_LENGTH 4 // in sec

// //////////////////////////////////////////////////////////

#define LEFT_ALARM_PORT PORTB
#define LEFT_ALARM_DDR DDRB
#define LEFT_ALARM_PIN PINB
#define LEFT_ALARM_BIT 4

#define RIGHT_ALARM_PORT PORTB
#define RIGHT_ALARM_DDR DDRB
#define RIGHT_ALARM_PIN PINB
#define RIGHT_ALARM_BIT 3

#define LEFT_LIGHT_PORT PORTB
#define LEFT_LIGHT_DDR DDRB
#define LEFT_LIGHT_PIN PINB
#define LEFT_LIGHT_BIT 0

#define RIGHT_LIGHT_PORT PORTB
#define RIGHT_LIGHT_DDR DDRB
#define RIGHT_LIGHT_PIN PINB
#define RIGHT_LIGHT_BIT 2

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
static volatile uint08 m_speed = 0; // in 1.334m/sec
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
		++m_ssTicks;
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

//#if TC_ONE_SEC < 256
	static volatile uint08 m_overflows = 0;
//#else
//	static volatile uint16 m_overflows = 0;
//#endif
ISR(TIM0_OVF_vect)
{
	if (++m_overflows == TC_ONE_SEC) // every second
	{
		m_overflows = 0;

		m_speed = m_ssTicks >> 3;
		m_ssTicks = 0;

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
	//
	// ports
	// speed sensor port to in with pull-up;
	// left and right alarm ports to in with pull-up;
	// left and right light ports to out
	// 
	DDRB = SFT(LEFT_LIGHT_BIT) | SFT(RIGHT_LIGHT_BIT);
	PORTB = SFT(SPEED_SENSOR_BIT) | SFT(LEFT_ALARM_BIT) | SFT(RIGHT_ALARM_BIT);

	//
	// timer
	//
	TCCR0B |= TC_PRESCALER_BITS << CS00;
	TIMSK0 |= SFT(TOIE0); // overflow interrupt

	//
	// external interrupts
	//
	GIMSK |= SFT(INT0) | SFT(PCIE); // INT0; PCINT
	MCUCR |= SFT(ISC00); // INT0; any logical change
	PCMSK |= SFT(LEFT_ALARM_BIT) | SFT(RIGHT_ALARM_BIT); // allow PCINT for left and right alarm ports

	//
	// members
	//
	m_ssTicks = 0;
	m_speed = 0;
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
			
			if (rightTime)
			{
				SBI(RIGHT_LIGHT_PORT, RIGHT_LIGHT_BIT);
			}
		}

		if (!leftTime)
		{
			CBI(LEFT_LIGHT_PORT, LEFT_LIGHT_BIT);
		}

		if (!rightTime)
		{
			CBI(RIGHT_LIGHT_PORT, RIGHT_LIGHT_BIT);
		}
	}

	return 0;
}
