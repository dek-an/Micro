#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "keyboardDef.h"
#include <common/rtos/rtos.h>

typedef uint08 KeyType;

// //////////////////////////////////////////////////////////
// Interface
//
extern void initKeyboard(void) __attribute__((always_inline));
extern void kbdTimerTask(const TaskParameter param);
extern void kbdRegisterKeyHandler(KeyType key, Task task);

#endif // _KEYBOARD_H_
