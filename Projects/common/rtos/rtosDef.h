#ifndef _RTOSDEF_H_
#define _RTOSDEF_H_

// NOTE: Values should be power of two!
// Task Queue size
#define TASK_QUEUE_SIZE 32
// Timer Task Set size
#define TIMER_TASK_SET_SIZE 16

#define RTOS_TIMER_TICK_LENGTH_MS 1 // one operation per RTOS_TIMER_TICK_LENGTH_MS; no more than 30
#define RTOS_TIMER_OCR F_CPU / 1024000 * RTOS_TIMER_TICK_LENGTH_MS

#endif // _RTOSDEF_H_