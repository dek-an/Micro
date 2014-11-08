#include <common/commonHeader.h>
#include <common/cpp/queue.h>

#include "libRTOS.h"

namespace
{
class InterruptsGuard
{
public:
	InterruptsGuard()
		: m_interruptsEn(false)
	{
		if (INTERRUPTS_ENABLED)
		{
			CLI();
			m_interruptsEn = true;
		}
	}

	~InterruptsGuard()
	{
		if (m_interruptsEn)
		{
			SEI();
		}
	}

private:
	bool m_interruptsEn;
};
} // anonymus namespace

namespace cpp_ns
{
namespace rtos_ns
{

RtosInterface::RtosInterface RtosInterface::m_instance;

RtosInterface::RtosInterface()
	: m_activatedTimersCnt(0)
{
	for (uint08 i = 0; i < TIMER_TASK_SET_SIZE; ++i)
	{
		m_timersSet[i].m_task = idle;
		m_timersSet[i].m_time = 0;
	}
}

RtosInterface::~RtosInterface() {}

RtosInterface::RtosInterface& RtosInterface::getInstance()
{
	return m_instance;
}


// called from everywhere (also from interrupts)
void RtosInterface::setTask(const Task& task)
{
	InterruptsGuard ig;
	m_taskQueue.setTask(task);
}

// called from everywhere (also from interrupts)
void RtosInterface::setTimerTaskMS(const Task& task, uint16 timeMS)
{
	InterruptsGuard ig;

	uint08 emptyTimerPos = 0;
	bool timerUpdated = false;
	for (uint08 i = 0; i < TIMER_TASK_SET_SIZE; ++i)
	{
		if (task == m_timersSet[i].m_task) // if already exist such task
		{
			m_timersSet[i].m_time = timeMS; // then update time for this task
			timerUpdated = true;
			break;
		}
		else if (0 == m_timersSet[i].m_time)
		{
			emptyTimerPos = i;
		}
	}

	if (!timerUpdated && m_activatedTimersCnt < TIMER_TASK_SET_SIZE)
	{
		++m_activatedTimersCnt;
		m_timersSet[emptyTimerPos].m_task = task;
		m_timersSet[emptyTimerPos].m_time = timeMS;
	}
}

// call from main loop
void RtosInterface::taskManager()
{
	CLI();
	Task currentTask = m_taskQueue.getTask();
	SEI();
	currentTask();
}

void RtosInterface::timerService()
{
	for (uint08 i = 0; i < TIMER_TASK_SET_SIZE; ++i)
	{
		if (0 == m_timersSet[i].m_time) // if empty timer
		{
			continue;
		}
		else if (1 == m_timersSet[i].m_time) // if time is gone
		{
			setTask((Task)m_timersSet[i].m_task); // send to task queue
			// and clear timer
			m_timersSet[i].m_task = idle;
			m_timersSet[i].m_time = 0;
			// decrease number of activated timers
			--m_activatedTimersCnt;
		}
		else
		{
			--m_timersSet[i].m_time;
		}
	}
}

} // namespace rtos_ns
} //namespace cpp_ns
