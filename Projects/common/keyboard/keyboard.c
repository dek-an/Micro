#include "keyboard.h"
#include "keyboardDef.h"

// //////////////////////////////////////////////////////////
// Interface Implementation
//
// keys and handlers add one by one
static KeyType m_keys[KBD_KEYS_NUMBER];
static Task m_handlers[KBD_KEYS_NUMBER];

#define KBD_EMPTY_HANDLER 0xFF

void initKeyboard(void)
{
	INITIALIZE_CHECKING();

	// Port to in with pull-up
	MASK_CLEAR(KBD_DDR, KBD_MASK);
	MASK_SET(KBD_PORT, KBD_MASK);

	for (uint08 i = 0; i < KBD_KEYS_NUMBER; ++i)
	{
		m_keys[i] = KBD_EMPTY_HANDLER;
		m_handlers[i] = &idleTask;
	}
}

void kbdTimerTask(const TaskParameter param)
{
	// kbdTimerTask is periodic task; set kbdTimerTask task
	setTimerTaskMS(&kbdTimerTask, 0, KBD_TIMER_TASK_TIME);

	// if no registered handlers
	if (KBD_EMPTY_HANDLER == m_keys[0])
		return;

	// if no pressed keys
	uint08 kbdPin = KBD_PIN;
	MASK_TOGGLE(kbdPin, KBD_MASK);
	if (!kbdPin)
		return;

	uint08 thisKeyHandlerPos = 0;
	for (; thisKeyHandlerPos < KBD_KEYS_NUMBER; ++thisKeyHandlerPos)
	{
		const KeyType currentKey = m_keys[thisKeyHandlerPos];
		if (KBD_EMPTY_HANDLER == currentKey) // we didn't found necessary handler before in registered handlers
			return;

		if (!GBI(KBD_PIN, currentKey)) // this key pressed
			break;
	}

	// if there is no handler for this key
	if (thisKeyHandlerPos >= KBD_KEYS_NUMBER)
		return;

	setTask(m_handlers[thisKeyHandlerPos], 0);
}

extern void kbdRegisterKeyHandler(const KeyType key, Task handler)
{
	uint08 handlerPos = 0;
	// find last empty position for handler
	for (; handlerPos < KBD_KEYS_NUMBER; ++handlerPos)
	{
		const KeyType currentKey = m_keys[handlerPos];
		if (currentKey == key) // handler for this key already registered; update it
			break;

		if (currentKey == KBD_EMPTY_HANDLER)
			break;
	}

	// if there is no empty place
	if (handlerPos >= KBD_KEYS_NUMBER)
		return;

	m_keys[handlerPos] = key;
	m_handlers[handlerPos] = handler;
}