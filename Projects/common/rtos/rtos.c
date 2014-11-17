#include "rtos.h"
#include "rtosDef.h"

// //////////////////////////////////////////////////////////
// Timer Interrupt
//

// //////////////////////////////////////////////////////////
// Task Queue
//
typedef struct
{
	Task m_task;
	TaskParameter m_param;
} TaskObject;

static const TaskObject idleObject = { &idleTask, 0 };

static void initTaskQueue(void);
static BOOL taskQueueIsEmpty(void);
static BOOL taskQueueIsFull(void);
static void setTaskToTQ(const Task task, const TaskParameter param);
static TaskObject getTaskFromTQ(void);

// //////////////////////////////////////////////////////////
// Timer Task Set
//
static void initTimerTaskSet(void);
static BOOL timerTaskSetIsEmpty(void);
static BOOL timerTaskSetIsFull(void);
static void addUpdateTimerTask(const Task timerTask, const TaskParameter param, const uint16 timeMS);
static void updateTimers(void);

// //////////////////////////////////////////////////////////
// Interface
//
void initRtos(void)
{
	INITIALIZE_CHECKING();

	initTaskQueue();
	initTimerTaskSet();

	// set timer
	// Clear Timer on Compare match; WGM21:0 = 10
	// prescaler 1024; CS22:0 = 111
	TCCR2 = SFT(WGM21) | SFT(CS22) | SFT(CS21) | SFT(CS20);
	// set Output Compare Register value
	OCR2 = RTOS_TIMER_OCR;
	// TC2 Output Compare Math Interrupt Enable
	SBI(TIMSK, OCIE2);
}

// called from everywhere (also from interrupts)
void setTask(const Task task, const TaskParameter param)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		setTaskToTQ(task, param);
	}
}

// called from everywhere (also from interrupts)
void setTimerTaskMS(const Task task, const TaskParameter param, const uint16 timeMS)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		addUpdateTimerTask(task, param, timeMS);
	}
}

// called from main loop only
void taskManager(void)
{
	TaskObject currentTaskObject = idleObject;

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		currentTaskObject = getTaskFromTQ();
	}

	currentTaskObject.m_task(currentTaskObject.m_param);
}

// called from timer interrupt
void timerService(void)
{
	updateTimers();
}

// //////////////////////////////////////////////////////////
// Task Queue Implementation
//
static volatile uint08 m_taskQueueBegin = 0;
static volatile uint08 m_taskQueueEnd = 0;
static volatile uint08 m_taskQueueSize = 0;
static volatile TaskObject m_taskQueueBuffer[TASK_QUEUE_SIZE];

static inline void initTaskQueue(void)
{}

static inline BOOL taskQueueIsEmpty(void)
{
	return !m_taskQueueSize;
}

static inline BOOL taskQueueIsFull(void)
{
	return (m_taskQueueSize == TASK_QUEUE_SIZE);
}

static inline void setTaskToTQ(const Task task, const TaskParameter param)
{
	if (taskQueueIsFull())
		return;

	m_taskQueueBuffer[m_taskQueueBegin].m_task = task;
	m_taskQueueBuffer[m_taskQueueBegin].m_param = param;
	m_taskQueueBegin = (m_taskQueueBegin + 1) & (TASK_QUEUE_SIZE - 1);
	++m_taskQueueSize;
}

static inline TaskObject getTaskFromTQ(void)
{
	if (taskQueueIsEmpty())
		return idleObject;

	const TaskObject retTask = m_taskQueueBuffer[m_taskQueueEnd];
	m_taskQueueEnd = (m_taskQueueEnd + 1) & (TASK_QUEUE_SIZE - 1);
	--m_taskQueueSize;

	return retTask;
}

// //////////////////////////////////////////////////////////
// Timer Task Set Implementation
//
// Timer Task Type
typedef struct
{
	TaskObject m_taskObj;
	uint16 m_time;
} TimerTask;

static volatile uint08 m_activatedTimersCnt = 0;
static volatile TimerTask m_timerTaskSetBuffer[TIMER_TASK_SET_SIZE];

#define TIME_IS_GONE(time) (time) <= RTOS_TIMER_TICK_LENGTH_MS

static inline void initTimerTaskSet(void)
{
	for (uint08 i = 0; i < TIMER_TASK_SET_SIZE; ++i)
	{
		m_timerTaskSetBuffer[i].m_taskObj = idleObject;
		m_timerTaskSetBuffer[i].m_time = 0;
	}
}

static inline BOOL timerTaskSetIsEmpty(void)
{
	return !m_activatedTimersCnt;
}

static inline BOOL timerTaskSetIsFull(void)
{
	return m_activatedTimersCnt == TIMER_TASK_SET_SIZE;
}

static inline void addUpdateTimerTask(const Task timerTask, const TaskParameter param, const uint16 timeMS)
{
	if (TIME_IS_GONE(timeMS))
	{
		setTaskToTQ(timerTask, param);
		return;
	}

	uint08 emptyTimerPos = 0;
	BOOL timerUpdated = 0;
	for (uint08 i = 0; i < TIMER_TASK_SET_SIZE; ++i)
	{
		if (timerTask == m_timerTaskSetBuffer[i].m_taskObj.m_task) // if already exist such task
		{
			m_timerTaskSetBuffer[i].m_time = timeMS; // then update time for this task
			timerUpdated = 0xFF;
			break;
		}
		else if (0 == m_timerTaskSetBuffer[i].m_time)
		{
			emptyTimerPos = i;
		}
	}

	if (!timerUpdated && !timerTaskSetIsFull())
	{
		++m_activatedTimersCnt;
		m_timerTaskSetBuffer[emptyTimerPos].m_taskObj.m_task = timerTask;
		m_timerTaskSetBuffer[emptyTimerPos].m_taskObj.m_param = param;
		m_timerTaskSetBuffer[emptyTimerPos].m_time = timeMS;
	}
}

static inline void updateTimers(void)
{
	// if there is no activated timers
	if (timerTaskSetIsEmpty())
		return;

	for (uint08 i = 0; i < TIMER_TASK_SET_SIZE; ++i)
	{
		if (0 == m_timerTaskSetBuffer[i].m_time) // if empty timer
		{
			continue;
		}
		else if (TIME_IS_GONE(m_timerTaskSetBuffer[i].m_time)) // if time is gone
		{
			setTask(m_timerTaskSetBuffer[i].m_taskObj.m_task, m_timerTaskSetBuffer[i].m_taskObj.m_param); // send to task queue
			// and clear timer
			m_timerTaskSetBuffer[i].m_taskObj = idleObject;
			m_timerTaskSetBuffer[i].m_time = 0;
			// decrease number of activated timers
			--m_activatedTimersCnt;
		}
		else
		{
			m_timerTaskSetBuffer[i].m_time -= RTOS_TIMER_TICK_LENGTH_MS;
		}
	}
}