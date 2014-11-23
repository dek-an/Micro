#include "clock.h"

#include <common/commonHeader.h>
#include <string.h> // for strcpy

#ifndef CLOCK_ASYNCRONOUS_MODE
#include <common/rtos/rtos.h>
#endif

// //////////////////////////////////////////////////////////
// Helpers
//
static inline void setTime(const uint32 time);
static inline uint32 getTime(void);

#ifdef CLOCK_ASYNCRONOUS_MODE
ISR(TIMER2_OVF_vect);
#else // not CLOCK_ASYNCRONOUS_MODE
static void clockTimerTask(const TaskParameter param);
#endif

// //////////////////////////////////////////////////////////
// Interface Implementation
//
#ifdef CLOCK_ASYNCRONOUS_MODE
static volatile uint32 m_time = 0; // in seconds
#else // not CLOCK_ASYNCRONOUS_MODE
static uint32 m_time = 0; // in seconds
#endif

void initClock(void)
{
	INITIALIZE_CHECKING();

	setTime(0);

#ifdef CLOCK_ASYNCRONOUS_MODE
	while (GBI(ASSR, TCN2UB) || GBI(ASSR, OCR2UB) || GBI(ASSR, TCR2UB)) // whait while TC2 is ready
	{
		_delay_ms(1);
	}

	// asynchronous mode
	ASSR = SFT(AS2);
	// reset timer counter
	TCNT2 = 0;
	// prescaler 128: TCS22:0 = 101
	TCCR2 = 5 << CS20;
	// Overflow interrupt
	SBI(TIMSK, TOIE2);
#else // not CLOCK_ASYNCRONOUS_MODE
	setTimerTaskMS(&clockTimerTask, 0, 1000);
#endif
}

void setClock(const uint08 hours, const uint08 minutes, const uint08 seconds)
{
	const uint32 newTime = ((uint32)hours % 24) * 3600 + ((uint32)minutes % 60) * 60 + ((uint32)seconds % 60);
	setTime(newTime);
}

void setHours(const uint08 hours)
{
	setClock(hours, getMinutes(), getSeconds());
}

void setMinutes(const uint08 minutes)
{
	setClock(getHours(), minutes, getSeconds());
}

void setSeconds(const uint08 seconds)
{
	setClock(getHours(), getMinutes(), seconds);
}

// returns str of length 9 (with zero symbol)
const char* getTimeStr(void)
{
	static char timeStrBuff[9];

	strcpy(timeStrBuff, getTimeStrHoursMinutes());
	timeStrBuff[5] = ':';
	const uint08 seconds = getSeconds();
	timeStrBuff[6] = GET_PROGMEM_DIGIT(seconds / 10);
	timeStrBuff[7] = GET_PROGMEM_DIGIT(seconds % 10);
	timeStrBuff[8] = '\0';

	return timeStrBuff;
}

// returns str of length 6 (with zero symbol)
const char* getTimeStrHoursMinutes(void)
{
	static char timeStrBuff[6];

	const uint08 hours = getHours();
	timeStrBuff[0] = GET_PROGMEM_DIGIT(hours / 10);
	timeStrBuff[1] = GET_PROGMEM_DIGIT(hours % 10);
	timeStrBuff[2] = ':';
	const uint08 minutes = getMinutes();
	timeStrBuff[3] = GET_PROGMEM_DIGIT(minutes / 10);
	timeStrBuff[4] = GET_PROGMEM_DIGIT(minutes % 10);
	timeStrBuff[5] = '\0';

	return timeStrBuff;
}

uint08 getHours(void)
{
	return (uint08)(getTime() / 3600);
}

uint08 getMinutes(void)
{
	return (uint08)((getTime() % 3600) / 60);
}

uint08 getSeconds(void)
{
	return (uint08)((getTime() % 3600) % 60);
}

uint08 increase24(const uint08 val)
{
	if (val == 23)
		return 0;

	return ((val + 1) % 24);
}

uint08 increase60(const uint08 val)
{
	if (val == 59)
		return 0;

	return ((val + 1) % 60);
}

uint08 decrease24(const uint08 val)
{
	if (val == 0)
		return 23;

	return ((val - 1) % 24);
}

uint08 decrease60(const uint08 val)
{
	if (val == 0)
		return 59;

	return ((val - 1) % 60);
}

// //////////////////////////////////////////////////////////
// Helpers Implementation
//
static inline void setTime(const uint32 time)
{
#ifdef CLOCK_ASYNCRONOUS_MODE
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
#endif
		m_time = time;
#ifdef CLOCK_ASYNCRONOUS_MODE
	}
#endif
}

static inline uint32 getTime(void)
{
#ifdef CLOCK_ASYNCRONOUS_MODE
	uint32 time = 0;

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		time = m_time;
	}

	return time;
#else // not CLOCK_ASYNCRONOUS_MODE
	return m_time;
#endif
}

#define TIME_END 86400 // 60 * 60 * 24 seconds

#ifdef CLOCK_ASYNCRONOUS_MODE
ISR(TIMER2_OVF_vect)
{
	m_time = (m_time + 1) % TIME_END;
}
#else // not CLOCK_ASYNCRONOUS_MODE
static void clockTimerTask(const TaskParameter param)
{
	setTimerTaskMS(&clockTimerTask, 0, 1000);

	m_time = (m_time + 1) % TIME_END;
}
#endif
