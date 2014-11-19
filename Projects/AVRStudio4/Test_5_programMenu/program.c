#include <common/rtos/rtos.h>
#include <common/lcdWH1602b/lcdWH1602b.h>
#include <common/menu/menu.h>
#include <common/keyboard/keyboard.h>
#include <common/clock/clock.h>

void idleTask(const TaskParameter param) {}

// //////////////////////////////////////////////////////////
// Defines
//
#define KBD_KEY_NEXT	KBD_KEY_3
#define KBD_KEY_PREV	KBD_KEY_4
#define KBD_KEY_OK		KBD_KEY_1
#define KBD_KEY_CANCEL	KBD_KEY_2

#define DISPLAY_PROGRAM_TASK_TIME 1000

// //////////////////////////////////////////////////////////
// Tasks
//
static void miSetHoursTask(const TaskParameter param);
static void miSetMinutesTask(const TaskParameter param);
static void miSetSecondsTask(const TaskParameter param);

static void keyNextPreseed(const TaskParameter param);
static void keyPrevPressed(const TaskParameter param);
static void keyOkPressed(const TaskParameter param);
static void keyCancelPressed(const TaskParameter param);

// //////////////////////////////////////////////////////////
// Helpers
//
static void displayProgram(const TaskParameter param);

// //////////////////////////////////////////////////////////
// MENU definition
//
//				NAME			NEXT				PREVIOUS			PARENT				CHILD				TASK				TEXT
// 1
MENU_MAKE_ITEM(	miSettings,		EMPTY_MENU_ITEM,	EMPTY_MENU_ITEM,	EMPTY_MENU_ITEM,	miTimeSettings,			&idleTask,			"Settings");
// 2
MENU_MAKE_ITEM(	miTimeSettings,	EMPTY_MENU_ITEM,	EMPTY_MENU_ITEM,	miSettings,			miSetHours,			&idleTask,			"Time Settings");
// 3
MENU_MAKE_ITEM(	miSetHours,		miSetMinutes,		EMPTY_MENU_ITEM,	miTimeSettings,		EMPTY_MENU_ITEM,	&miSetHoursTask,	"Set Hours");
MENU_MAKE_ITEM(	miSetMinutes,	miSetSeconds,		miSetHours,			miTimeSettings,		EMPTY_MENU_ITEM,	&miSetMinutesTask,	"Set Minutes");
MENU_MAKE_ITEM(	miSetSeconds,	EMPTY_MENU_ITEM,	miSetMinutes,		miTimeSettings,		EMPTY_MENU_ITEM,	&miSetSecondsTask,	"Set Seconds"); 

static MenuObject m_programMenu;
static MenuObject* m_pmPtr = &m_programMenu;

// //////////////////////////////////////////////////////////
// Interface Implementation
//
void initProgram(void)
{
	initRtos();
	initLcd();
	initClock();

	// keyboard
	initKeyboard();
	kbdRegisterKeyHandler(KBD_KEY_NEXT, &keyNextPreseed);
	kbdRegisterKeyHandler(KBD_KEY_PREV, &keyPrevPressed);
	kbdRegisterKeyHandler(KBD_KEY_OK, &keyOkPressed);
	kbdRegisterKeyHandler(KBD_KEY_CANCEL, &keyCancelPressed);

	// menu
	initMenu();
	startMenu(m_pmPtr, MENU_ITEM_CPTR(miSettings));

	// start
	displayProgram(0);

	SEI();
}

int programRun(void)
{
	for (;;)
	{
		taskManager();
	}

	return 0;
}

// //////////////////////////////////////////////////////////
// Helpers Implementation
//
static uint08 m_progFlag = 0;
// flags
#define IN_MENU_BIT 0
#define IN_MENU GBI(m_progFlag, IN_MENU_BIT)
#define IN_MENU_ON() SBI(m_progFlag, IN_MENU_BIT)
#define IN_MENU_OFF() CBI(m_progFlag, IN_MENU_BIT)

static void displayProgram(const TaskParameter param)
{
	setTimerTaskMS(&displayProgram, 0, DISPLAY_PROGRAM_TASK_TIME);

	if (IN_MENU)
		return;

	lcdClear();
	lcdWriteStr(getTimeStr());

	lcdGoTo(1, 0);
	static const char menuStr[] PROGMEM = "MENU";
	lcdWriteStrProgMem(menuStr);
}

// //////////////////////////////////////////////////////////
// Tasks Implementation
//
static void miSetHoursTask(const TaskParameter param)
{
	lcdClear();
	lcdWriteStr("In SetHours");
}

static void miSetMinutesTask(const TaskParameter param)
{
	lcdClear();
	lcdWriteStr("In SetMinutes");
}

static void miSetSecondsTask(const TaskParameter param)
{
	lcdClear();
	lcdWriteStr("In SetSeconds");
}

static void keyNextPreseed(const TaskParameter param)
{
	if (!IN_MENU)
		return;

	menuNext(m_pmPtr);
}

static void keyPrevPressed(const TaskParameter param)
{
	if (!IN_MENU)
		return;

	menuPrev(m_pmPtr);
}

static void keyOkPressed(const TaskParameter param)
{
	if (IN_MENU)
	{
		menuStepIn(m_pmPtr);
	}
	else
	{
		IN_MENU_ON();
		resetMenu(m_pmPtr);
	}
}

static void keyCancelPressed(const TaskParameter param)
{
	if (!IN_MENU)
		return;

	if (menuIsHead(m_pmPtr))
	{
		IN_MENU_OFF();
		resetMenu(m_pmPtr);
		displayProgram(0);
	}
	else
	{
		menuStepOut(m_pmPtr);
	}
}
