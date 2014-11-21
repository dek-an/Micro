#include "clock.h"

#include <common/commonHeader.h>
#include <common/rtos/rtos.h>
#include <string.h> // for strcpy

// //////////////////////////////////////////////////////////
// Helpers
//
static void clockTimerTask(const TaskParameter param);

// //////////////////////////////////////////////////////////
// Interface Implementation
//
static uint32 m_time = 0; // in seconds

void initClock(void)
{
	INITIALIZE_CHECKING();

	m_time = 0;
	setTimerTaskMS(&clockTimerTask, 0, 1000);
}

void setClock(const uint08 hours, const uint08 minutes, const uint08 seconds)
{
	m_time = ((uint32)hours % 24) * 3600 + ((uint32)minutes % 60) * 60 + ((uint32)seconds % 60);
}

void sethours(const uint08 hours)
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
	return (uint08)(m_time / 3600);
}

uint08 getMinutes(void)
{
	return (uint08)((m_time % 3600) / 60);
}

uint08 getSeconds(void)
{
	return (uint08)((m_time % 3600) % 60);
}

uint08 increase24(const uint08 val)
{
	return (++val % 24);
}

uint08 increase60(const uint08 val)
{
	return (++val % 60);
}

uint08 decrease24(const uint08 val)
{
	return (--val % 24);
}

uint08 decrease60(const uint08 val)
{
	return (--val % 60);
}

// //////////////////////////////////////////////////////////
// Helpers Implementation
//
#define TIME_END 86400 // 60 * 60 * 24 seconds

static void clockTimerTask(const TaskParameter param)
{
	setTimerTaskMS(&clockTimerTask, 0, 1000);

	m_time = (m_time + 1) % TIME_END;
}
