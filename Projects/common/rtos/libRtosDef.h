#ifndef _LIBRTOSDEF_H_
#define _LIBRTOSDEF_H_

// NOTE: Values should be power of two!
// Handlers Set Size
#define HANDLERS_SET_SIZE 16
// Task Queue size
#define TASK_QUEUE_SIZE 32
// Timer Task Set size
#define TIMER_TASK_SET_SIZE 16

typedef enum
{
	MSG_EMPTY = 0
} MessageType;

#endif // _LIBRTOSDEF_H_