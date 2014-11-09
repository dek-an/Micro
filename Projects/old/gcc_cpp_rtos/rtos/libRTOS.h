#ifndef _LIBRTOS_H_
#define _LIBRTOS_H_

#include <common/cpp/queue.h>

namespace cpp_ns
{
namespace rtos_ns
{

// //////////////////////////////////////////////////////////
// Definitions
//
// Task Queue size
#define TASK_QUEUE_SIZE 15
// Task type
typedef void (*Task)(void);
// empty task
inline void idle() {}

// Timer Task Set size
#define TIMER_TASK_SET_SIZE 10
// Timer Task Type
struct TimerTask
{
	Task m_task;
	uint16 m_time;
};

// //////////////////////////////////////////////////////////
// RtosInterface
//
class RtosInterface
{
private:
	RtosInterface();

public:
	~RtosInterface();

public:
	static RtosInterface& getInstance();

public:
	// called from everywhere (also from interrupts)
	void setTask(const Task& task);
	// called from everywhere (also from interrupts)
	void setTimerTaskMS(const Task& task, uint16 timeMS);

	// called from main loop only
	void taskManager();
	// called from timer interrupt
	void timerService();

private:
	static RtosInterface m_instance;

	typedef Queue<Task, TASK_QUEUE_SIZE, idle> TaskQueue;
	TaskQueue m_taskQueue;

	typedef TimerTask TimersSet[TIMER_TASK_SET_SIZE];
	volatile TimersSet m_timersSet;
	volatile uint08 m_activatedTimersCnt;
};


}// namespace rtos_ns
} // namespace Rtos

#endif // _LIBRTOS_H_
