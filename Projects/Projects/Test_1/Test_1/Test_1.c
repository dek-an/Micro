/*
 * Test_1.c
 *
 * Created: 10/11/2014 13:41:15
 *  Author: dekarand
 */ 

#include <common/lcdWH1602b/lcdWH1602b.h>
#include <common/menu/menu.h>

#define DISPLAY_TEST
//#define MENU_TEST
//#define LIBRTOS_TEST

volatile uint64 tempVal = 0;

void idleTask(TaskParameter param)
{
	//tempVal &= param;
}

void taskTM22(TaskParameter param)
{
	//MenuObject* menu = (MenuObject*)param;
	//tempVal = (uint16)menu->m_invokedItem;
}

void testTask(TaskParameter param)
{
	tempVal = param;
}


static void displayTest()
{
#ifdef DISPLAY_TEST
	static const char numbers[] PROGMEM = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};

//lcdGoTo(1, 3);

	_delay_ms(100);
	initLcd();
	char col = 0;
	for (;;)
	{
		_delay_ms(1000);
		lcdWriteChar(pgm_read_byte(&numbers[col % 10]));
		_delay_ms(1000);
		lcdClear();
		_delay_ms(2);
		lcdWriteChar(pgm_read_byte(&numbers[col++ % 10]));
		_delay_ms(1000);

		lcdClear();
		lcdWriteChar('a');
		lcdWriteChar('b');
		lcdWriteChar('c');
		_delay_ms(1000);
		lcdWriteChar('A');
		lcdWriteChar('B');
		lcdWriteChar('C');
		_delay_ms(1000);
		lcdClear();
		lcdWriteStr("This is a String");
		_delay_ms(2000);
		lcdClear();

		lcdWriteStr("Position\0");
		_delay_ms(3000);
		lcdGoTo(1, 3);
		lcdWriteStr("here");
		_delay_ms(1000);
		lcdClear();

		lcdGoTo(0, 4);
		lcdWriteStr("Position 0-4");
		lcdWriteStr("here");
		_delay_ms(1000);
		lcdClear();
	}
#endif
}

#ifdef MENU_TEST
//				NAME			NEXT				PREVIOUS			PARENT				CHILD				TASK		TEXT
MENU_MAKE_ITEM(	startMenu1,		startMenu2,			EMPTY_MENU_ITEM,	EMPTY_MENU_ITEM,	tableMenu1,			&idleTask,	"SM1");
MENU_MAKE_ITEM(	startMenu2,		EMPTY_MENU_ITEM,	startMenu1,			EMPTY_MENU_ITEM,	EMPTY_MENU_ITEM,	&idleTask,	"SM2");
MENU_MAKE_ITEM(	tableMenu1,		tableMenu2,			EMPTY_MENU_ITEM,	startMenu1,			EMPTY_MENU_ITEM,	&idleTask,	"TM1");
MENU_MAKE_ITEM(	tableMenu2,		EMPTY_MENU_ITEM,	tableMenu1,			startMenu1,			tableMenu21,		&idleTask,	"TM2");
MENU_MAKE_ITEM(	tableMenu21,	tableMenu22,		EMPTY_MENU_ITEM,	tableMenu2,			EMPTY_MENU_ITEM,	&idleTask,	"TM21");
MENU_MAKE_ITEM(	tableMenu22,	EMPTY_MENU_ITEM,	tableMenu21,		tableMenu2,			EMPTY_MENU_ITEM,	&taskTM22,	"TM22");
#endif

static void menuTest()
{
#ifdef MENU_TEST
	initLcd();
	lcdWriteStr("MENU TEST");
	_delay_ms(3000);
	lcdClear();

	initRtos();
	initMenu();

	MenuObject menu;
	MenuObject* menuPtr = &menu;

	lcdWriteStr("First Item");
	lcdGoTo(1, 0);
	lcdWriteStrProgMem(menuPtr->m_currentItem->m_text);
	_delay_ms(3000);

	for (;;)
	{
		startMenu(menuPtr, (const MenuItemPtr)&startMenu1);
		taskManager();
		_delay_ms(1000);

		menuNext(menuPtr); // sm2
		taskManager();
		_delay_ms(1000);

		menuNext(menuPtr); // sm2
		taskManager();
		_delay_ms(1000);

		menuPrev(menuPtr); // sm1
		taskManager();
		_delay_ms(1000);

		menuStepIn(menuPtr); // tm1
		taskManager();
		_delay_ms(1000);

		menuStepIn(menuPtr); // tm1
		taskManager();
		_delay_ms(1000);

		menuStepOut(menuPtr); // sm1
		taskManager();
		_delay_ms(1000);

		menuStepOut(menuPtr); // sm1
		taskManager();
		_delay_ms(1000);

		menuStepIn(menuPtr); // tm1
		taskManager();
		_delay_ms(1000);

		menuNext(menuPtr); // tm2
		taskManager();
		_delay_ms(1000);

		menuNext(menuPtr); // tm2
		taskManager();
		_delay_ms(1000);

		menuStepIn(menuPtr); // tm21
		taskManager();
		_delay_ms(1000);

		menuStepOut(menuPtr); // tm2
		taskManager();
		_delay_ms(1000);

		menuStepIn(menuPtr); // tm21
		taskManager();
		_delay_ms(1000);

		menuStepIn(menuPtr); // tm21; no task
		taskManager();
		_delay_ms(1000);

		menuNext(menuPtr); // tm22
		taskManager();
		_delay_ms(1000);

		menuStepIn(menuPtr); // tm22; taskTM22
		taskManager();
		_delay_ms(1000);

		menuStepIn(menuPtr); // tm22; taskTM22
		taskManager();
		_delay_ms(1000);

		menuStepIn(menuPtr); // tm22; taskTM22
		taskManager();
		_delay_ms(1000);

		menuNext(menuPtr); // tm22
		taskManager();
		_delay_ms(1000);

		menuPrev(menuPtr); // tm22
		taskManager();
		_delay_ms(1000);

		menuStepOut(menuPtr); // tm22
		taskManager();
		_delay_ms(1000);

	}
#endif
}

static void libRtosTest()
{
#ifdef LIBRTOS_TEST
	initRtos();
	const uint08 tn = 20;
	for (char i = 0; i < tn; ++i)
	setTask(&testTask, i);
	for (char i = 0; i < tn; ++i)
	taskManager();
	taskManager();
	taskManager();
	for (char i = 0; i < tn; ++i)
	setTask(&testTask, i);
	for (char i = 0; i < tn; ++i)
	taskManager();
#endif
}


int main(void)
{
	displayTest();
	menuTest();
	libRtosTest();

	return 0;
}