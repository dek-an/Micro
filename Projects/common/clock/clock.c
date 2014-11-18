#include "clock.h"

#include <common/rtos/rtos.h>

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
	m_time = 0;
	setTimerTaskMS(&clockTimerTask, 0, 1000);
}

void setClock(const uint08 hours, const uint08 minutes, const uint08 seconds)
{
	m_time = (hours % 24) * 3600 + (minutes % 60) * 60 + (seconds % 60);
}

//void setHours(const uint08 hours)
//{
//	
//}

//void setMinutes(const uint08 minutes)
//{
//	
//}

//void setSeconds(const uint08 seconds)
//{
//	
//}

const char* getTimeStrProgmem(void)
{
	return "";
}

const char* getTimeStrHoursMinutesProgMem(void)
{
	return "";
}

uint08 getHours(void)
{
	return (uint08)(m_time / 3600);
}

uint08 getMinutes(void)
{
	return (m_time % 3600) / 60;
}

uint08 getSeconds(void)
{
	return (m_time % 3600) % 60;
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
