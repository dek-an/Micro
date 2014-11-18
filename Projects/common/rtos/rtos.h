#ifndef _LIBRTOS_H_
#define _LIBRTOS_H_

#include <common/commonHeader.h>

// //////////////////////////////////////////////////////////
// Definitions
//
// Task type
typedef uint16 TaskParameter; // should be uint16 as pointer size
typedef void (*Task)(const TaskParameter);
// empty task; should be defined by user
extern void idleTask(const TaskParameter);

// //////////////////////////////////////////////////////////
// Interface
//
// should be called once
extern void initRtos(void) /*__attribute__((always_inline))*/;

// called from everywhere (also from interrupts)
extern void setTask(const Task task, const TaskParameter param);

// called from everywhere (also from interrupts)
extern void setTimerTaskMS(const Task task, const TaskParameter param, const uint16 timeMS);

// called from main loop only
extern void taskManager(void) /*__attribute__((always_inline))*/;

// called from timer interrupt;
// timer tasks removed from timers task set, so for task cycling it should be
// re-added to the timer set in the task
extern void timerService(void) /*__attribute__((always_inline))*/;

#endif // _LIBRTOS_H_