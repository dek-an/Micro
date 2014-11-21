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
// Constants
//
static const char menuStr[] PROGMEM = "MENU";
static const char hoursStr[] PROGMEM = "Hours: ";
static const char minutesStr[] PROGMEM = "Minutes: ";
static const char secondsStr[] PROGMEM = "Seconds: ";

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
// IN MENU
#define IN_MENU_BIT 0
#define IN_MENU GBI(m_progFlag, IN_MENU_BIT)
#define IN_MENU_ON() SBI(m_progFlag, IN_MENU_BIT)
#define IN_MENU_OFF() CBI(m_progFlag, IN_MENU_BIT)
// IN KEY HANDLER
#define IN_MENU_TASK_BIT 1
#define IN_MENU_TASK GBI(m_progFlag, IN_MENU_BIT)
#define IN_MENU_TASK_ON() SBI(m_progFlag, IN_MENU_BIT)
#define IN_MENU_TASK_OFF() CBI(m_progFlag, IN_MENU_BIT)

static void displayProgram(const TaskParameter param)
{
	setTimerTaskMS(&displayProgram, 0, DISPLAY_PROGRAM_TASK_TIME);

	if (IN_MENU)
		return;

	lcdClear();
	lcdWriteStr(getTimeStr());

	lcdGoTo(1, 0);
	lcdWriteStrProgMem(menuStr);
}

// //////////////////////////////////////////////////////////
// Tasks Implementation
//
static void miSetHoursTask(const TaskParameter param)
{
	static uint08 m_changedHours = 0;

	if (!IN_MENU_TASK) // if first visit
	{
		if (KBD_KEY_OK != param)
			return;

		m_changedHours = getHours();
		IN_MENU_TASK_ON();
	}
	else // if already here
	{
		switch (param)
		{
		case KBD_KEY_NEXT:
			m_changedHours = increase24(m_changedHours);
			break;
		case KBD_KEY_PREV:
			m_changedHours = decrease24(m_changedHours);
			break;
		case KBD_KEY_OK:
			setHours(m_changedHours);
			keyCancelPressed(KBD_KEY_CANCEL);
			break;
		case KBD_KEY_CANCEL
			m_changedHours = 0;
			IN_MENU_TASK_OFF();
			return; // do not write current setting to lcd
		}
	}

	// write current setting to lcd
	lcdClear();
	lcdWriteStrProgMem(hoursStr);
	lcdWriteUint16(m_changedHours);
}

static void miSetMinutesTask(const TaskParameter param)
{
	static uint08 m_changedMinutes = 0;

	if (!IN_MENU_TASK) // if first visit
	{
		if (KBD_KEY_OK != param)
			return;

		m_changedMinutes = getMinutes();
		IN_MENU_TASK_ON();
	}
	else // if already here
	{
		switch (param)
		{
		case KBD_KEY_NEXT:
			m_changedMinutes = increase60(m_changedMinutes);
			break;
		case KBD_KEY_PREV:
			m_changedMinutes = decrease60(m_changedMinutes);
			break;
		case KBD_KEY_OK:
			setMinutes(m_changedMinutes);
			keyCancelPressed(KBD_KEY_CANCEL);
			break;
		case KBD_KEY_CANCEL
			m_changedMinutes = 0;
			IN_MENU_TASK_OFF();
			return; // do not write current setting to lcd
		}
	}

	// write current setting to lcd
	lcdClear();
	lcdWriteStrProgMem(minutesStr);
	lcdWriteUint16(m_changedMinutes);
}

static void miSetSecondsTask(const TaskParameter param)
{
	static uint08 m_changedSeconds = 0;

	if (!IN_MENU_TASK) // if first visit
	{
		if (KBD_KEY_OK != param)
			return;

		m_changedSeconds = getSeconds();
		IN_MENU_TASK_ON();
	}
	else // if already here
	{
		switch (param)
		{
		case KBD_KEY_NEXT:
			m_changedSeconds = increase60(m_changedSeconds);
			break;
		case KBD_KEY_PREV:
			m_changedSeconds = decrease60(m_changedSeconds);
			break;
		case KBD_KEY_OK:
			setSeconds(m_changedSeconds);
			keyCancelPressed(KBD_KEY_CANCEL);
			break;
		case KBD_KEY_CANCEL
			m_changedSeconds = 0;
			IN_MENU_TASK_OFF();
			return; // do not write current setting to lcd
		}
	}

	// write current setting to lcd
	lcdClear();
	lcdWriteStrProgMem(secondsStr);
	lcdWriteUint16(m_changedSeconds);
}

static void keyNextPreseed(const TaskParameter param)
{
	if (!IN_MENU)
		return;

	menuNext(m_pmPtr, param);
}

static void keyPrevPressed(const TaskParameter param)
{
	if (!IN_MENU)
		return;

	menuPrev(m_pmPtr, param);
}

static void keyOkPressed(const TaskParameter param)
{
	if (IN_MENU)
	{
		menuStepIn(m_pmPtr, param);
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
		menuStepOut(m_pmPtr, param);
	}
}
