#ifndef _LIBRTOS_H_
#define _LIBRTOS_H_

#include "libRtosDef.h"

#include <common/commonHeader.h>

// //////////////////////////////////////////////////////////
// Definitions
//
// Task type
typedef uint16 TaskParameter;
typedef void (*Task)(const TaskParameter);
// empty task
extern void idle(const TaskParameter);

// //////////////////////////////////////////////////////////
// Interface
//
// should be called once
extern void initRtos(void);

// called from everywhere (also from interrupts)
extern void setTask(const Task task, const TaskParameter param);

// called from everywhere (also from interrupts);
// handler should be registered for msgType
extern void setMessage(const MessageType msgType, const TaskParameter param);

// called from everywhere (also from interrupts)
extern void setTimerTaskMS(const Task task, const TaskParameter param, const uint16 timeMS);

// called from everywhere (also from interrupts);
// handler should be registered for msgType
extern void setTimerMessageMS(const MessageType msgType, const TaskParameter param, const uint16 timeMS);

// called from main loop only
extern void taskManager(void);

// called from timer interrupt;
// timer tasks removed from timers task set, so for task cycling it should be
// re-added to the timer set in the task
extern void timerService(void);

extern void registerHandler(const MessageType msgType, const Task handler);
extern void unregisterHandler(const Task handler);

#endif // _LIBRTOS_H_