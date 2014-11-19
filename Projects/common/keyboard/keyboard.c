#include "keyboard.h"
#include "keyboardDef.h"

// //////////////////////////////////////////////////////////
// Interface Implementation
//
// keys and handlers add one by one
static KeyType m_keys[KBD_KEYS_NUMBER];
static Task m_handlers[KBD_KEYS_NUMBER];
static uint08 m_handlersCnt = 0;

#define KBD_KEY_EMPTY 0xFF

void initKeyboard(void)
{
	INITIALIZE_CHECKING();

	// Port to in with pull-up
	MASK_CLEAR(KBD_DDR, KBD_MASK);
	MASK_SET(KBD_PORT, KBD_MASK);

	for (uint08 i = 0; i < KBD_KEYS_NUMBER; ++i)
	{
		m_keys[i] = KBD_KEY_EMPTY;
		m_handlers[i] = &idleTask;
	}

	m_handlersCnt = 0;

	setTimerTaskMS(&kbdTimerTask, 0, KBD_TIMER_TASK_PERIOD);
}

void kbdTimerTask(const TaskParameter param)
{
	// kbdTimerTask is periodic task; set kbdTimerTask task
	setTimerTaskMS(&kbdTimerTask, 0, KBD_TIMER_TASK_PERIOD);

	// if no registered handlers
	if (!m_handlersCnt)
		return;

	// if no pressed keys
	uint08 kbdPin = KBD_PIN;
	MASK_CLEAR(kbdPin, !KBD_MASK); // clear all except mask
	MASK_TOGGLE(kbdPin, KBD_MASK); // toggle pins so "key pressed" = 1
	if (!kbdPin)
		return;

	// find handler for pressed key
	uint08 thisKeyHandlerPos = 0;
	for (; thisKeyHandlerPos < m_handlersCnt; ++thisKeyHandlerPos)
	{
		if (GBI(kbdPin, m_keys[thisKeyHandlerPos])) // this key pressed
			break;
	}

	// if there is no handler for this key
	if (thisKeyHandlerPos >= m_handlersCnt)
		return;

	setTask(m_handlers[thisKeyHandlerPos], m_keys[thisKeyHandlerPos]);
}

extern void kbdRegisterKeyHandler(const KeyType key, Task handler)
{
	// find handler if exist
	uint08 handlerPos = 0;
	for (; handlerPos < m_handlersCnt; ++handlerPos)
	{
		if (key == m_keys[handlerPos]) // handler for this key already registered; update it
			break;
	}

	if (handlerPos < m_handlersCnt) // if handler for this key exist then update
	{
		m_keys[handlerPos] = key;
		m_handlers[handlerPos] = handler;
	}
	else if (m_handlersCnt < KBD_KEYS_NUMBER) // if there is no handler for this key and handlers storage is not full
	{
		m_keys[m_handlersCnt] = key;
		m_handlers[m_handlersCnt] = handler;
		++m_handlersCnt;
	}
}