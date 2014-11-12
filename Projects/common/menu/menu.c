#include "menu.h"
#include "menuDef.h"

#include <common/lcdWH1602b/lcdWH1602b.h>

static MenuItemPtr k_currentMenuItem = 0;

#define MENU_STACK_SIZE
static MenuItemPtr k_menuStack[MENU_STACK_SIZE];
static volatile uint08 k_menuStackTop;

MenuItem EMPTY_MENU_ITEM;// = {0, 0, 0, 0, 0, {0x00}};
//static MenuItemPtr EMPTY_MENU_ITEM_PTR = &EMPTY_MENU_ITEM;

// //////////////////////////////////////////////////////////
//
//
static BOOL menuItemIsHead(const MenuItemPtr menuItem)
{
	return 0/*EMPTY_MENU_ITEM_PTR*/ == MENU_GET_PARENT(menuItem);
}

static void displayMenu(const MenuObject* menu)
{
	lcdClear();

	// display parent item or "MENU:" str
	lcdGoTo(1, 0);
	if (menuItemIsHead(menu->m_currentItem))
	{
		lcdWriteStrProgMem((uchar*)PSTR("MENU:"));
	}
	else
	{
		lcdWriteStrProgMem((uchar*)(MENU_GET_PARENT(menu->m_currentItem)->m_text));
	}

	// Вторая строка - текущий пункт меню
	lcdGoTo(2, 1);
	lcdWriteStrProgMem((uchar*)menu->m_currentItem->m_text);
}

// //////////////////////////////////////////////////////////
// Interface
//
void initMenu(void)
{
	static BOOL isInitialized = 0;
	if (isInitialized)
		return;

	initLcd();
	isInitialized = 0xFF;
}

void startMenu(void)
{
	
}

void updateMenuTask(TaskParameter param)
{
	const MenuObject* menu = (const MenuObject*)param;
	displayMenu(menu);
}

void resetMenu(MenuObject* menu)
{
	menu->m_currentItem = menu->m_menuHead;
	setTask(&updateMenuTask, (TaskParameter)menu);
}