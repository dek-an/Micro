#include <common/lcdWH1602b/lcdWH1602b.h>
#include <common/rtos/rtos.h>
#include <common/keyboard/keyboard.h>

#define KBD_KEY_NEXT	KBD_KEY_1
#define KBD_KEY_PREV	KBD_KEY_2
#define KBD_KEY_OK		KBD_KEY_3
#define KBD_KEY_CANCEL	KBD_KEY_4

//#define SIMULATOR

void idleTask(TaskParameter param) {}

void keyNextHandler(TaskParameter param)
{
#ifndef SIMULATOR
	lcdClear();
	lcdWriteStr("NEXT pressed");

	static uint16 times = 0;
	++times;
	lcdGoTo(1, 0);
	lcdWriteUint16(times);
#endif
}

void keyPrevHandler(TaskParameter param)
{
#ifndef SIMULATOR
	lcdClear();
	lcdWriteStr("PREV pressed");

	static uint16 times = 0;
	++times;
	lcdGoTo(1, 0);
	lcdWriteUint16(times);
#endif
}

void keyOKHandler(TaskParameter param)
{
#ifndef SIMULATOR
	lcdClear();
	lcdWriteStr("OK pressed");

	static uint16 times = 0;
	++times;
	lcdGoTo(1, 0);
	lcdWriteUint16(times);
#endif
}

void keyCancelHandler(TaskParameter param)
{
#ifndef SIMULATOR
	lcdClear();
	lcdWriteStr("CANCEL pressed");

	static uint16 times = 0;
	++times;
	lcdGoTo(1, 0);
	lcdWriteUint16(times);
#endif
}

int main(void)
{
	initRtos();
#ifndef SIMULATOR
	initLcd();
#endif
	initKeyboard();

	kbdRegisterKeyHandler(KBD_KEY_NEXT, &keyNextHandler);
	kbdRegisterKeyHandler(KBD_KEY_PREV, &keyPrevHandler);
	kbdRegisterKeyHandler(KBD_KEY_OK, &keyOKHandler);
	kbdRegisterKeyHandler(KBD_KEY_CANCEL, &keyCancelHandler);

	setTimerTaskMS(&kbdTimerTask, 0, 2000);

#ifndef SIMULATOR
	lcdWriteStr("KEYBOARD TEST");
#endif

	SEI();

	for (;;)
	{
#ifdef SIMULATOR
		kbdTimerTask(0);
#endif
		taskManager();
	}

	return 0;
}
