#include <common/rtos/rtos.h>
#include <common/lcdWH1602b/lcdWH1602b.h>
#include <common/clock/clock.h>

#include <string.h>

void idleTask(const TaskParameter param) {}

//static char tmBuff[9];
void displayClockTask(const TaskParameter param)
{
	setTimerTaskMS(&displayClockTask, 0, 1000);

	lcdClear();
	lcdGoTo(0, 0);
	static uint16 cnt = 0;
	lcdWriteUint16(++cnt);
	lcdWriteStr("  ");
	//strcpy(tmBuff, getTimeStr());
	lcdWriteStr(getTimeStrHoursMinutes());
	lcdGoTo(1, 0);
	lcdWriteStr(getTimeStr());
}

int main(void)
{
	initRtos();
	initLcd();
	initClock();

	lcdWriteStr("CLOCK TEST");
	_delay_ms(2000);

	setClock(23, 59, 40);

	setTimerTaskMS(&displayClockTask, 0, 0);

	SEI();

	for (;;)
	{
		//setTask(displayClockTask, 0);
		taskManager();
	}

	return 0;
}
