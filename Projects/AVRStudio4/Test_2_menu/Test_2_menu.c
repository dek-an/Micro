#include <common/lcdWH1602b/lcdWH1602b.h>
#include <common/menu/menu.h> 

volatile uint64 tempVal = 0;

void idleTask(TaskParameter param)
{
	tempVal &= param;
}

void taskTM22(TaskParameter param)
{
	lcdClear();
	lcdWriteStr("from task");
	_delay_ms(3000);
}

//				NAME			NEXT				PREVIOUS			PARENT				CHILD				TASK		TEXT
MENU_MAKE_ITEM(	startMenu1,		startMenu2,			EMPTY_MENU_ITEM,	EMPTY_MENU_ITEM,	tableMenu1,			&idleTask,	"SM1");
MENU_MAKE_ITEM(	startMenu2,		EMPTY_MENU_ITEM,	startMenu1,			EMPTY_MENU_ITEM,	EMPTY_MENU_ITEM,	&idleTask,	"SM2");
MENU_MAKE_ITEM(	tableMenu1,		tableMenu2,			EMPTY_MENU_ITEM,	startMenu1,			EMPTY_MENU_ITEM,	&idleTask,	"TM1");
MENU_MAKE_ITEM(	tableMenu2,		EMPTY_MENU_ITEM,	tableMenu1,			startMenu1,			tableMenu21,		&idleTask,	"TM2");
MENU_MAKE_ITEM(	tableMenu21,	tableMenu22,		EMPTY_MENU_ITEM,	tableMenu2,			EMPTY_MENU_ITEM,	&idleTask,	"TM21");
MENU_MAKE_ITEM(	tableMenu22,	EMPTY_MENU_ITEM,	tableMenu21,		tableMenu2,			EMPTY_MENU_ITEM,	&taskTM22,	"TM22");

int main(void)
{
	initLcd();
	lcdWriteStr("MENU TEST");
	_delay_ms(3000);
	lcdClear();

	initRtos();
	initMenu();

	MenuObject menu;
	MenuObject* menuPtr = &menu;

	for (;;)
	{
		startMenu(menuPtr, (const MenuItemPtr)&startMenu1);
		taskManager();
		_delay_ms(2000);

		menuNext(menuPtr); // sm2
		taskManager();
		_delay_ms(2000);

		menuNext(menuPtr); // sm2
		taskManager();
		_delay_ms(2000);

		menuPrev(menuPtr); // sm1
		taskManager();
		_delay_ms(2000);

		menuStepIn(menuPtr); // tm1
		taskManager();
		_delay_ms(2000);

		menuStepIn(menuPtr); // tm1
		taskManager();
		_delay_ms(2000);

		menuStepOut(menuPtr); // sm1
		taskManager();
		_delay_ms(2000);

		menuStepOut(menuPtr); // sm1
		taskManager();
		_delay_ms(2000);

		menuStepIn(menuPtr); // tm1
		taskManager();
		_delay_ms(2000);

		menuNext(menuPtr); // tm2
		taskManager();
		_delay_ms(2000);

		menuNext(menuPtr); // tm2
		taskManager();
		_delay_ms(2000);

		menuStepIn(menuPtr); // tm21
		taskManager();
		_delay_ms(2000);

		menuStepOut(menuPtr); // tm2
		taskManager();
		_delay_ms(2000);

		menuStepIn(menuPtr); // tm21
		taskManager();
		_delay_ms(2000);

		menuStepIn(menuPtr); // tm21; no task
		taskManager();
		_delay_ms(2000);

		menuNext(menuPtr); // tm22
		taskManager();
		_delay_ms(2000);

		menuStepIn(menuPtr); // tm22; taskTM22
		taskManager();
		_delay_ms(2000);

		menuStepIn(menuPtr); // tm22; taskTM22
		taskManager();
		_delay_ms(2000);

		menuStepIn(menuPtr); // tm22; taskTM22
		taskManager();
		_delay_ms(2000);

		menuNext(menuPtr); // tm22
		taskManager();
		_delay_ms(2000);

		menuPrev(menuPtr); // tm22
		taskManager();
		_delay_ms(2000);

		menuStepOut(menuPtr); // tm22
		taskManager();
		_delay_ms(2000);

	} 

	return 0;
}
