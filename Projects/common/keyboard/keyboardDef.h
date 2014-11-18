#ifndef _KEYBOARDDEF_H_
#define _KEYBOARDDEF_H_

#include <common/commonHeader.h>

// Keyboard Port
#define KBD_PORT PORTA
#define KBD_DDR DDRA
#define KBD_PIN PINA

// Key pins
#define KBD_KEY_1 0
#define KBD_KEY_2 1
#define KBD_KEY_3 2
#define KBD_KEY_4 3

// Number of keys
#define KBD_KEYS_NUMBER 4

// Every new key should be added here
#define KBD_MASK \
	(SFT(KBD_KEY_1) | SFT(KBD_KEY_2) | SFT(KBD_KEY_3) | SFT(KBD_KEY_4))

// Keyboard timer task period (ms)
#define KBD_TIMER_TASK_PERIOD 150

#endif // _KEYBOARDDEF_H_
