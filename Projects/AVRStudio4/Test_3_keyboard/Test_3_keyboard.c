#include <common/lcdWH1602b/lcdWH1602b.h>
#include <common/rtos/rtos.h>
#include <common/keyboard/keyboard.h>

#define KBD_KEY_NEXT	KBD_KEY_1
#define KBD_KEY_PREV	KBD_KEY_2
#define KBD_KEY_OK		KBD_KEY_3
#define KBD_KEY_CANCEL	KBD_KEY_4

void idleTask(TaskParameter param) {}

void keyNextHandler(TaskParameter param)
{
	lcdClear();
	lcdWriteStr("NEXT pressed");
}

void keyPrevHandler(TaskParameter param)
{
	lcdClear();
	lcdWriteStr("PREV pressed");
}

void keyOKHandler(TaskParameter param)
{
	lcdClear();
	lcdWriteStr("OK pressed");
}

void keyCancelHandler(TaskParameter param)
{
	lcdClear();
	lcdWriteStr("CANCEL pressed");
}

int main(void)
{
	initRtos();
	//initLcd();
	initKeyboard();

	kbdRegisterKeyHandler(KBD_KEY_NEXT, &keyNextHandler);
	kbdRegisterKeyHandler(KBD_KEY_PREV, &keyPrevHandler);
	kbdRegisterKeyHandler(KBD_KEY_OK, &keyOKHandler);
	kbdRegisterKeyHandler(KBD_KEY_CANCEL, &keyCancelHandler);

	setTimerTaskMS(&kbdTimerTask, 0, 5000);

	//lcdWriteStr("KEYBOARD TEST");
	//_delay_ms(2000);
	//lcdClear();

	SEI();

	for (;;)
	{
		kbdTimerTask(0);
		taskManager();
	}

	return 0;
}
