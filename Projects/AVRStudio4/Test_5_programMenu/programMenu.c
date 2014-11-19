#include <common/rtos/rtos.h>
#include <common/lcdWH1602b/lcdWH1602b.h>
#include <common/menu/menu.h>
#include <common/keyboard/keyboard.h>
#include <common/clock/clock.h>

static void miSetHoursTask(const TaskParameter param);
static void miSetMinutesTask(const TaskParameter param);
static void miSetSecondsTask(const TaskParameter param);

//				NAME			NEXT				PREVIOUS			PARENT				CHILD				TASK				TEXT
// 1
MENU_MAKE_ITEM(	miSettings,		EMPTY_MENU_ITEM,	EMPTY_MENU_ITEM,	EMPTY_MENU_ITEM,	miSetClock,			&idleTask,			"Settings");
// 2
MENU_MAKE_ITEM(	miTimeSettings,	EMPTY_MENU_ITEM,	EMPTY_MENU_ITEM,	miSettings,			miSetHours,			&idleTask,			"Time Settings");
// 3
MENU_MAKE_ITEM(	miSetHours,		miSetMinutes,		EMPTY_MENU_ITEM,	miTimeSettings,		EMPTY_MENU_ITEM,	&miSetHoursTask,	"Set Hours");
MENU_MAKE_ITEM(	miSetMinutes,	miSetSeconds,		miSetHours,			miTimeSettings,		EMPTY_MENU_ITEM,	&miSetMinutesTask,	"Set Minutes");
MENU_MAKE_ITEM(	miSetSeconds,	EMPTY_MENU_ITEM,	miSetMinutes,		miTimeSettings,		EMPTY_MENU_ITEM,	&miSetSecondsTask,	"Set Seconds"); 

static MenuObject m_programMenu;
#define MENU_PTR (&m_programMenu);

void initProgramMenu(void)
{
	initRtos();
	initLcd();
	initClock();
	initKeyboard();

	start
}