#include <common/rtos/rtos.h>
#include <common/lcdWH1602b/lcdWH1602b.h>
#include <common/menu/menu.h>
#include <common/keyboard/keyboard.h>
#include <common/clock/clock.h>

#include "eepromConstants.h"

void idleTask(const TaskParameter param) {}

// //////////////////////////////////////////////////////////
// Defines
//
#define KBD_KEY_NEXT	KBD_KEY_3
#define KBD_KEY_PREV	KBD_KEY_4
#define KBD_KEY_OK		KBD_KEY_1
#define KBD_KEY_CANCEL	KBD_KEY_2

#define DISPLAY_PROGRAM_TASK_TIME 1000

#define FAN_PORT PORTC
#define FAN_DDR DDRC
#define FAN_PIN PINC
#define FAN_BIT 0

// //////////////////////////////////////////////////////////
// Constants
//
static const char menuStr[] PROGMEM = "MENU";
static const char hoursStr[] PROGMEM = "Hours:";
static const char minutesStr[] PROGMEM = "Minutes:";
static const char secondsStr[] PROGMEM = "Seconds:";
static const char fanOnStr[] PROGMEM = "Fan ON Temp:";
static const char fanOffStr[] PROGMEM = "Fan OFF Temp:";
static const char fanStr[] PROGMEM = "Fan is:";
static const char onStr[] PROGMEM = "ON";
static const char offStr[] PROGMEM = "OFF";

// //////////////////////////////////////////////////////////
// Tasks
//
static void miSetHoursTask(const TaskParameter param);
static void miSetMinutesTask(const TaskParameter param);
static void miSetSecondsTask(const TaskParameter param);
static void miSetFanOnTempTask(const TaskParameter param);
static void miSetFanOffTempTask(const TaskParameter param);
static void miFunctFanOnOff(const TaskParameter param);

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
//				NAME			NEXT				PREVIOUS			PARENT				CHILD				TASK					TEXT
// 1
MENU_MAKE_ITEM(	miFunctions,	miSettings,			EMPTY_MENU_ITEM,	EMPTY_MENU_ITEM,	miFanFunctions,		&idleTask,				"Functions");
MENU_MAKE_ITEM(	miSettings,		EMPTY_MENU_ITEM,	miFunctions,		EMPTY_MENU_ITEM,	miTimeSettings,		&idleTask,				"Settings");
// 2 Functions
MENU_MAKE_ITEM(	miFanFunctions,	EMPTY_MENU_ITEM,	EMPTY_MENU_ITEM,	miFunctions,		EMPTY_MENU_ITEM,	&miFunctFanOnOff,		"Fan ON/OFF");
// 2 Settings
MENU_MAKE_ITEM(	miTimeSettings,	miFanSettings,		EMPTY_MENU_ITEM,	miSettings,			miSetHours,			&idleTask,				"Time Settings");
MENU_MAKE_ITEM(	miFanSettings,	EMPTY_MENU_ITEM,	miTimeSettings,		miSettings,			miSetFanOnTemp,		&idleTask,				"Fan Settings");
// 3 Time Settings
MENU_MAKE_ITEM(	miSetHours,		miSetMinutes,		EMPTY_MENU_ITEM,	miTimeSettings,		EMPTY_MENU_ITEM,	&miSetHoursTask,		"Set Hours");
MENU_MAKE_ITEM(	miSetMinutes,	miSetSeconds,		miSetHours,			miTimeSettings,		EMPTY_MENU_ITEM,	&miSetMinutesTask,		"Set Minutes");
MENU_MAKE_ITEM(	miSetSeconds,	EMPTY_MENU_ITEM,	miSetMinutes,		miTimeSettings,		EMPTY_MENU_ITEM,	&miSetSecondsTask,		"Set Seconds");
// 3 Fan Settings
MENU_MAKE_ITEM(	miSetFanOnTemp,	miSetFanOffTemp,	EMPTY_MENU_ITEM,	miFanSettings,		EMPTY_MENU_ITEM,	&miSetFanOnTempTask,	"Set ON Temp");
MENU_MAKE_ITEM(	miSetFanOffTemp,EMPTY_MENU_ITEM,	miSetFanOnTemp,		miFanSettings,		EMPTY_MENU_ITEM,	&miSetFanOffTempTask,	"Set OFF Temp");

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

	// fan pin to out and off
	CBI(FAN_PORT, FAN_BIT);
	SBI(FAN_DDR, FAN_BIT);

	// keyboard
	initKeyboard();
	kbdRegisterKeyHandler(KBD_KEY_NEXT, &keyNextPreseed);
	kbdRegisterKeyHandler(KBD_KEY_PREV, &keyPrevPressed);
	kbdRegisterKeyHandler(KBD_KEY_OK, &keyOkPressed);
	kbdRegisterKeyHandler(KBD_KEY_CANCEL, &keyCancelPressed);

	// menu
	initMenu();
	startMenu(m_pmPtr, MENU_ITEM_CPTR(miFunctions));

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
#define IN_MENU_TASK GBI(m_progFlag, IN_MENU_TASK_BIT)
#define IN_MENU_TASK_ON() SBI(m_progFlag, IN_MENU_TASK_BIT)
#define IN_MENU_TASK_OFF() CBI(m_progFlag, IN_MENU_TASK_BIT)

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
// Tasks Helpers
typedef uint08 (*Uint08Getter)(void);
typedef void (*Uint08Setter)(const uint08);
typedef uint08 (*Uint08UpChanger)(const uint08);
typedef uint08 (*Uint08DownChanger)(const uint08);

static uint08 m_changedValue = 0;

static inline uint08 increment(const uint08 val)
{
	return val + 1;
}

static inline uint08 decrement(const uint08 val)
{
	return val - 1;
}

static void changeValue(
	TaskParameter param,
	Uint08Getter getter,
	Uint08Setter setter,
	Uint08UpChanger upChanger,
	Uint08DownChanger downChanger,
	const char* strProgMem)
{
	if (!IN_MENU_TASK) // if first visit
	{
		if (KBD_KEY_OK != param)
			return;

		m_changedValue = getter();
		IN_MENU_TASK_ON();
	}
	else // if already here
	{
		switch (param)
		{
		case KBD_KEY_NEXT:
			m_changedValue = upChanger(m_changedValue);
			break;
		case KBD_KEY_PREV:
			m_changedValue = downChanger(m_changedValue);
			break;
		case KBD_KEY_OK:
			setter(m_changedValue);
			keyCancelPressed(KBD_KEY_CANCEL);
			break;
		case KBD_KEY_CANCEL:
			m_changedValue = 0;
			IN_MENU_TASK_OFF();
			return; // do not write current setting to lcd
		}
	}

	lcdClear();
	lcdWriteStrProgMem(strProgMem);
	lcdGoTo(1, 1);
	lcdWriteUint08(m_changedValue);
}

// Tasks
static void miSetHoursTask(const TaskParameter param)
{
	changeValue(
		param,
		&getHours,
		&setHours,
		&increase24,
		&decrease24,
		hoursStr);
}

static void miSetMinutesTask(const TaskParameter param)
{
	changeValue(
		param,
		&getMinutes,
		&setMinutes,
		&increase60,
		&decrease60,
		minutesStr);
}

static void miSetSecondsTask(const TaskParameter param)
{
	changeValue(
		param,
		&getSeconds,
		&setSeconds,
		&increase60,
		&decrease60,
		secondsStr);
}

static void miSetFanOnTempTask(const TaskParameter param)
{
	changeValue(
		param,
		&getFanOnTemperature,
		&setFanOnTemperature,
		&increment,
		&decrement,
		fanOnStr);
}

static void miSetFanOffTempTask(const TaskParameter param)
{
	changeValue(
		param,
		&getFanOffTemperature,
		&setFanOffTemperature,
		&increment,
		&decrement,
		fanOffStr);
}

static void miFunctFanOnOff(const TaskParameter param)
{
	static const char* currentFanStateStr = 0x00;

	if (!IN_MENU_TASK) // if first visit
	{
		if (KBD_KEY_OK != param)
			return;

		if (GBI(FAN_PORT, FAN_BIT))
		{
			currentFanStateStr = onStr;
		}
		else
		{
			currentFanStateStr = offStr;
		}

		IN_MENU_TASK_ON();
	}
	else // if already here
	{
		switch (param)
		{
		case KBD_KEY_NEXT:
		case KBD_KEY_PREV:
			if (onStr == currentFanStateStr)
			{
				currentFanStateStr = offStr;
			}
			else
			{
				currentFanStateStr = onStr;
			}
			break;
		case KBD_KEY_OK:
			if (onStr == currentFanStateStr)
			{
				SBI(FAN_PORT, FAN_BIT);
			}
			else
			{
				CBI(FAN_PORT, FAN_BIT);
			}
			keyCancelPressed(KBD_KEY_CANCEL);
			break;
		case KBD_KEY_CANCEL:
			currentFanStateStr = 0x00;
			IN_MENU_TASK_OFF();
			return; // do not write current setting to lcd
		}
	}

	lcdClear();
	lcdWriteStrProgMem(fanStr);
	lcdGoTo(1, 1);
	lcdWriteStrProgMem(currentFanStateStr);
}

// Keys Tasks
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
