#include <common/lcdWH1602b/lcdWH1602b.h>
#include <common/rtos/rtos.h>
#include <common/keyboard/keyboard.h>

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
	initLcd();
	initKeyboard();

	SEI();

	for (;;)
	{
		taskManager();
	}

	return 0;
}