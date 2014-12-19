#include <common/rtos/rtos.h>
#include <common/lcdWH1602b/lcdWH1602b.h>
#include <common/menu/menu.h>
#include <common/keyboard/keyboard.h>
#include <common/clock/clock.h>
#include <common/ds18b20/ds18b20.h>

#include <stdio.h>

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
#define CHECK_LIGHT_TASK_TIME 2000
#define CHECK_FAN_TASK_TIME 2000
#define UPDATE_TEMPERATURE_TASK_TIME 2000

#define FAN_PORT PORTC
#define FAN_DDR DDRC
#define FAN_PIN PINC
#define FAN_BIT 0

#define LIGHT_PORT PORTC
#define LIGHT_DDR DDRC
#define LIGHT_PIN PINC
#define LIGHT_BIT 1

// //////////////////////////////////////////////////////////
// Constants
//
static const char menuStr[] PROGMEM = "MENU";
static const char hoursStr[] PROGMEM = "Hours:";
static const char minutesStr[] PROGMEM = "Minutes:";
static const char secondsStr[] PROGMEM = "Seconds:";
static const char fanOnStr[] PROGMEM = "Fan ON Temp:";
static const char fanOffStr[] PROGMEM = "Fan OFF Temp:";
static const char lightOnStr[] PROGMEM = "Light ON Time:";
static const char lightOffStr[] PROGMEM = "Light OFF Time:";
static const char lightStr[] PROGMEM = "Turn Light:";
static const char fanStr[] PROGMEM = "Turn Fan:";
static const char lightProcessingStr[] PROGMEM = "Light Proc is:";
static const char fanProcessingStr[] PROGMEM = "Fan Proc is:";
static const char onStr[] PROGMEM = "ON";
static const char offStr[] PROGMEM = "OFF";

// //////////////////////////////////////////////////////////
// Tasks
//
static void miSetTimeTask(const TaskParameter param);
static void miActivateLightTask(const TaskParameter param);
static void miSetLightOnTimeTask(const TaskParameter param);
static void miSetLightOffTimeTask(const TaskParameter param);
static void miActivateFanTask(const TaskParameter param);
static void miSetFanOnTempTask(const TaskParameter param);
static void miSetFanOffTempTask(const TaskParameter param);
static void miFunctLightOnOff(const TaskParameter param);
static void miFunctFanOnOff(const TaskParameter param);

static void keyNextPreseed(const TaskParameter param);
static void keyPrevPressed(const TaskParameter param);
static void keyOkPressed(const TaskParameter param);
static void keyCancelPressed(const TaskParameter param);

// //////////////////////////////////////////////////////////
// Helpers
//
static void displayProgram(const TaskParameter param);
static void checkLight(const TaskParameter param);
static void checkFan(const TaskParameter param);
static void updateTemperature(const TaskParameter param);

// //////////////////////////////////////////////////////////
// MENU definition
//
//				NAME				NEXT				PREVIOUS			PARENT				CHILD				TASK					TEXT
// 1
MENU_MAKE_ITEM(	miFunctions,		miSettings,			EMPTY_MENU_ITEM,	EMPTY_MENU_ITEM,	miLightFunctions,	&idleTask,				"Functions");
MENU_MAKE_ITEM(	miSettings,			EMPTY_MENU_ITEM,	miFunctions,		EMPTY_MENU_ITEM,	miTimeSettings,		&idleTask,				"Settings");
// 2 Functions
MENU_MAKE_ITEM(	miLightFunctions,	miFanFunctions,		EMPTY_MENU_ITEM,	miFunctions,		EMPTY_MENU_ITEM,	&miFunctLightOnOff,		"Light ON/OFF");
MENU_MAKE_ITEM(	miFanFunctions,		EMPTY_MENU_ITEM,	miLightFunctions,	miFunctions,		EMPTY_MENU_ITEM,	&miFunctFanOnOff,		"Fan ON/OFF");
// 2 Settings
MENU_MAKE_ITEM(	miTimeSettings,		miLightSettings,	EMPTY_MENU_ITEM,	miSettings,			miSetTime,			&idleTask,				"Time Settings");
MENU_MAKE_ITEM(	miLightSettings,	miFanSettings,		miTimeSettings,		miSettings,			miActivateLight,	&idleTask,				"Light Settings");
MENU_MAKE_ITEM(	miFanSettings,		EMPTY_MENU_ITEM,	miLightSettings,	miSettings,			miActivateFan,		&idleTask,				"Fan Settings");
// 3 Time Settings
MENU_MAKE_ITEM(	miSetTime,			EMPTY_MENU_ITEM,	EMPTY_MENU_ITEM,	miTimeSettings,		EMPTY_MENU_ITEM,	&miSetTimeTask,			"Set Time");
// 3 Light Settings
MENU_MAKE_ITEM(	miActivateLight,	miSetLightOnTime,	EMPTY_MENU_ITEM,	miLightSettings,	EMPTY_MENU_ITEM,	&miActivateLightTask,	"Activate Light"); // Turn ON/OFF Light Processing
MENU_MAKE_ITEM(	miSetLightOnTime,	miSetLightOffTime,	miActivateLight,	miLightSettings,	EMPTY_MENU_ITEM,	&miSetLightOnTimeTask,	"Set ON Time");
MENU_MAKE_ITEM(	miSetLightOffTime,	EMPTY_MENU_ITEM,	miSetLightOnTime,	miLightSettings,	EMPTY_MENU_ITEM,	&miSetLightOffTimeTask,	"Set OFF Time");
// 3 Fan Settings
MENU_MAKE_ITEM(	miActivateFan,		miSetFanOnTemp,		EMPTY_MENU_ITEM,	miFanSettings,		EMPTY_MENU_ITEM,	&miActivateFanTask,		"Activate Fan"); // Turn ON/OFF FAN Processing
MENU_MAKE_ITEM(	miSetFanOnTemp,		miSetFanOffTemp,	miActivateFan,		miFanSettings,		EMPTY_MENU_ITEM,	&miSetFanOnTempTask,	"Set ON Temp");
MENU_MAKE_ITEM(	miSetFanOffTemp,	EMPTY_MENU_ITEM,	miSetFanOnTemp,		miFanSettings,		EMPTY_MENU_ITEM,	&miSetFanOffTempTask,	"Set OFF Temp");

// //////////////////////////////////////////////////////////
// Members
//
static MenuObject m_programMenu;
static MenuObject* m_pmPtr = &m_programMenu;

static float m_waterTemp = 0;
static float m_outTemp = 0;

static volatile uint08 m_progFlag = 0;
// flags
// IN MENU
DECLARE_FLAG_BIT(m_progFlag, IN_MENU, 0)
// In Menu Task handler
DECLARE_FLAG_BIT(m_progFlag, IN_MENU_TASK, 1)
// Light is on
DECLARE_FLAG_BIT(m_progFlag, LIGHT_IS_ON, 2)
// Fan is on
DECLARE_FLAG_BIT(m_progFlag, FAN_IS_ON, 3)
// Temperature correctness
DECLARE_FLAG_BIT(m_progFlag, TEMP_WATER_IS_OK, 4)
DECLARE_FLAG_BIT(m_progFlag, TEMP_OUT_IS_OK, 5)
// Light and Fan Processing
DECLARE_FLAG_BIT(m_progFlag, LIGHT_PROC_ACTIVATED, 6)
DECLARE_FLAG_BIT(m_progFlag, FAN_PROC_ACTIVATED, 7)

// //////////////////////////////////////////////////////////
// Interface Implementation
//
void initProgram(void)
{
	initRtos();
	initLcd();
	initClock();

	// fan port pin to out and off
	CBI(FAN_PORT, FAN_BIT);
	SBI(FAN_DDR, FAN_BIT);

	// light port pin to out and off
	CBI(LIGHT_PORT, LIGHT_BIT);
	SBI(LIGHT_DDR, LIGHT_BIT);

	// keyboard
	initKeyboard();
	kbdRegisterKeyHandler(KBD_KEY_NEXT, &keyNextPreseed);
	kbdRegisterKeyHandler(KBD_KEY_PREV, &keyPrevPressed);
	kbdRegisterKeyHandler(KBD_KEY_OK, &keyOkPressed);
	kbdRegisterKeyHandler(KBD_KEY_CANCEL, &keyCancelPressed);

	// menu
	initMenu();
	startMenu(m_pmPtr, MENU_ITEM_CPTR(miFunctions));

	// eeprom constants
	initEeprom();

	// temperature sensors
	initDs18b20();

	LIGHT_PROC_ACTIVATED_ON();
	FAN_PROC_ACTIVATED_ON();

	// set tasks
	setTimerTaskMS(&displayProgram, 0, 0/*DISPLAY_PROGRAM_TASK_TIME*/);
	setTimerTaskMS(&checkLight, 0, 1200/*CHECK_LIGHT_TASK_TIME*/);
	setTimerTaskMS(&checkFan, 0, 1300/*CHECK_FAN_TASK_TIME*/);
	setTimerTaskMS(&updateTemperature, 0, 1400);

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
static void displayProgram(const TaskParameter param)
{
	setTimerTaskMS(&displayProgram, 0, DISPLAY_PROGRAM_TASK_TIME);

	if (IN_MENU())
		return;

	lcdClear();

	static char strBuff[32];
	// first line
	sprintf(strBuff, "%s %c %c", getTimeStr(), LIGHT_IS_ON() ? 15u : 1u, FAN_IS_ON() ? '#' : 1u);
	lcdWriteStr(strBuff);
	// second line
	int numChars = sprintf(strBuff, "W: %.1f%c O: %.1f%c", m_waterTemp, 176u, m_outTemp, 176u);
	if (numChars > 16 || numChars < 0)
	{
		sprintf(strBuff, "W:%.1f O:%.1f", m_waterTemp, m_outTemp);
	}
	lcdGoTo(1, 0);
	lcdWriteStr(strBuff);

	//lcdWriteStrProgMem(menuStr);
}

static void checkLight(const TaskParameter param)
{
	setTimerTaskMS(&checkLight, 0, CHECK_LIGHT_TASK_TIME);

	if (!LIGHT_PROC_ACTIVATED())
		return;

	const uint32 currentTime = getRawTime();

	if (getLightOnTime() < getLightOffTime())
	{
		// off
		if (LIGHT_IS_ON() && currentTime >= getLightOffTime())
		{
			CBI(LIGHT_PORT, LIGHT_BIT);
			LIGHT_IS_ON_OFF();
		}
		// on
		else if (!LIGHT_IS_ON() && getLightOnTime() <= currentTime && currentTime < getLightOffTime())
		{
			SBI(LIGHT_PORT, LIGHT_BIT);
			LIGHT_IS_ON_ON();
		}
	}
	else
	{
		// off
		if (LIGHT_IS_ON() && getLightOffTime() <= currentTime && currentTime < getLightOnTime())
		{
			CBI(LIGHT_PORT, LIGHT_BIT);
			LIGHT_IS_ON_OFF();
		}
		// on
		else if (!LIGHT_IS_ON() && (getLightOnTime() <= currentTime || currentTime < getLightOffTime()))
		{
			SBI(LIGHT_PORT, LIGHT_BIT);
			LIGHT_IS_ON_ON();
		}
	}
}

static void checkFan(const TaskParameter param)
{
	setTimerTaskMS(&checkFan, 0, CHECK_FAN_TASK_TIME);

	if (!FAN_PROC_ACTIVATED())
		return;

	// off
	if (m_waterTemp <= getFanOffTemperature() && FAN_IS_ON())
	{
		CBI(FAN_PORT, FAN_BIT);
		FAN_IS_ON_OFF();
	}
	// on
	else if (m_waterTemp >= getFanOnTemperature() && !FAN_IS_ON())
	{
		SBI(FAN_PORT, FAN_BIT);
		FAN_IS_ON_ON();
	}
}

static void updateTemperature(const TaskParameter param)
{
	setTimerTaskMS(&updateTemperature, 0, UPDATE_TEMPERATURE_TASK_TIME);

	if (ds18b20ReadTemperature(0, &m_waterTemp) == DS18B20_READ_SUCCESSFUL)
	{
		TEMP_WATER_IS_OK_ON();
	}
	else
	{
		TEMP_WATER_IS_OK_OFF();
	}

	if (ds18b20ReadTemperature(1, &m_outTemp) == DS18B20_READ_SUCCESSFUL)
	{
		TEMP_OUT_IS_OK_ON();
	}
	else
	{
		TEMP_OUT_IS_OK_OFF();
	}
}

// //////////////////////////////////////////////////////////
// Tasks Implementation
//
// Tasks Helpers
typedef uint08 (*Uint08Getter)(void);
typedef void (*Uint08Setter)(const uint08);
typedef uint08 (*Uint08UpChanger)(const uint08);
typedef uint08 (*Uint08DownChanger)(const uint08);
typedef uint32 (*TimeGetter)(void);
typedef void (*TimeSetter)(const uint32);

static inline uint08 increment(const uint08 val)
{
	return val + 1;
}

static inline uint08 decrement(const uint08 val)
{
	return val - 1;
}

static void changeUint08Value(
	TaskParameter param,
	Uint08Getter getter,
	Uint08Setter setter,
	Uint08UpChanger upChanger,
	Uint08DownChanger downChanger,
	const char* strProgMem)
{
	static uint08 changedValue = 0;

	if (!IN_MENU_TASK()) // if first visit
	{
		if (KBD_KEY_OK != param)
			return;

		changedValue = getter();
		IN_MENU_TASK_ON();
	}
	else // if already here
	{
		switch (param)
		{
		case KBD_KEY_NEXT:
			changedValue = upChanger(changedValue);
			break;
		case KBD_KEY_PREV:
			changedValue = downChanger(changedValue);
			break;
		case KBD_KEY_OK:
			setter(changedValue);
			keyCancelPressed(KBD_KEY_CANCEL);
			break;
		case KBD_KEY_CANCEL:
			changedValue = 0;
			IN_MENU_TASK_OFF();
			return; // do not write current setting to lcd
		}
	}

	lcdClear();
	lcdWriteStrProgMem(strProgMem);
	lcdGoTo(1, 1);
	lcdWriteUint08(changedValue);
}

static void changeTimeValue(const TaskParameter param, TimeGetter tmGetter, TimeSetter tmSetter)
{
	static uint32 changedTime = 0;
	static const char* currentStr = 0x00;
	static uint08 currentVal = 0;

	if (!IN_MENU_TASK()) // if first visit
	{
		if (KBD_KEY_OK != param)
			return;

		changedTime = tmGetter();
		currentStr = hoursStr;
		currentVal = getHoursFrom(changedTime);

		IN_MENU_TASK_ON();
	}
	else // if already here
	{
		switch (param)
		{
		case KBD_KEY_NEXT:
			if (hoursStr == currentStr) // if hours
			{
				currentVal = increase24(currentVal);
			}
			else // if minutes or seconds
			{
				currentVal = increase60(currentVal);
			}
			break;
		case KBD_KEY_PREV:
			if (hoursStr == currentStr) // if hours
			{
				currentVal = decrease24(currentVal);
			}
			else // if minutes or seconds
			{
				currentVal = decrease60(currentVal);
			}
			break;
		case KBD_KEY_OK:
			if (hoursStr == currentStr) // if hours
			{
				changedTime = updateHours(changedTime, currentVal);
				currentVal = getMinutesFrom(changedTime);
				currentStr = minutesStr;
			}
			else if (minutesStr == currentStr)// if minutes
			{
				changedTime = updateMinutes(changedTime, currentVal);
				currentVal = getSecondsFrom(changedTime);
				currentStr = secondsStr;
			}
			else // if seconds
			{
				changedTime = updateSeconds(changedTime, currentVal);
				tmSetter(changedTime);

				keyCancelPressed(KBD_KEY_CANCEL);
			}
			break;
		case KBD_KEY_CANCEL:
			changedTime = 0;
			currentStr = 0x00;
			currentVal = 0;
			IN_MENU_TASK_OFF();
			return; // do not write current setting to lcd
		}
	}

	lcdClear();
	lcdWriteStrProgMem(currentStr);
	lcdGoTo(1, 1);
	lcdWriteUint08(currentVal);
}

static void changeOnOffValue(const TaskParameter param, volatile uint08* port, const uint08 bit, const char* str)
{
	static const char* currentStateStr = 0x00;

	if (!IN_MENU_TASK()) // if first visit
	{
		if (KBD_KEY_OK != param)
			return;

		if (GBI(*port, bit))
		{
			currentStateStr = onStr;
		}
		else
		{
			currentStateStr = offStr;
		}

		IN_MENU_TASK_ON();
	}
	else // if already here
	{
		switch (param)
		{
		case KBD_KEY_NEXT:
		case KBD_KEY_PREV:
			if (onStr == currentStateStr)
			{
				currentStateStr = offStr;
			}
			else
			{
				currentStateStr = onStr;
			}
			break;
		case KBD_KEY_OK:
			if (onStr == currentStateStr)
			{
				SBI(*port, bit);
			}
			else
			{
				CBI(*port, bit);
			}
			keyCancelPressed(KBD_KEY_CANCEL);
			break;
		case KBD_KEY_CANCEL:
			currentStateStr = 0x00;
			IN_MENU_TASK_OFF();
			return; // do not write current setting to lcd
		}
	}

	lcdClear();
	lcdWriteStrProgMem(str);
	lcdGoTo(1, 1);
	lcdWriteStrProgMem(currentStateStr);
}

// Tasks
static void miSetTimeTask(const TaskParameter param)
{
	changeTimeValue(param, &getRawTime, &setRawTime);
}

static void miActivateLightTask(const TaskParameter param)
{
	changeOnOffValue(param, &m_progFlag, LIGHT_PROC_ACTIVATED_BIT, lightProcessingStr);
}

static void miSetLightOnTimeTask(const TaskParameter param)
{
	changeTimeValue(param, &getLightOnTime, &setLightOnTime);
}

static void miSetLightOffTimeTask(const TaskParameter param)
{
	changeTimeValue(param, &getLightOffTime, &setLightOffTime);
}

static void miActivateFanTask(const TaskParameter param)
{
	changeOnOffValue(param, &m_progFlag, FAN_PROC_ACTIVATED_BIT, fanProcessingStr);
}

static void miSetFanOnTempTask(const TaskParameter param)
{
	changeUint08Value(
		param,
		&getFanOnTemperature,
		&setFanOnTemperature,
		&increment,
		&decrement,
		fanOnStr);
}

static void miSetFanOffTempTask(const TaskParameter param)
{
	changeUint08Value(
		param,
		&getFanOffTemperature,
		&setFanOffTemperature,
		&increment,
		&decrement,
		fanOffStr);
}

static void miFunctLightOnOff(const TaskParameter param)
{
	changeOnOffValue(param, &LIGHT_PORT, LIGHT_BIT, lightStr);
}

static void miFunctFanOnOff(const TaskParameter param)
{
	changeOnOffValue(param, &FAN_PORT, FAN_BIT, fanStr);
}

// Keys Tasks
static void keyNextPreseed(const TaskParameter param)
{
	if (!IN_MENU())
		return;

	menuNext(m_pmPtr, param);
}

static void keyPrevPressed(const TaskParameter param)
{
	if (!IN_MENU())
		return;

	menuPrev(m_pmPtr, param);
}

static void keyOkPressed(const TaskParameter param)
{
	if (IN_MENU())
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
	if (!IN_MENU())
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
