#include "menu.h"
#include "menuDef.h"

#include <common/lcdWH1602b/lcdWH1602b.h>

const MenuItem EMPTY_MENU_ITEM = {0, 0, 0, 0, 0, {0x00}};
static MenuItemPtr EMPTY_MENU_ITEM_PTR = &EMPTY_MENU_ITEM;

// //////////////////////////////////////////////////////////
//
//
static BOOL menuItemIsHead(const MenuItemPtr menuItem)
{
	return EMPTY_MENU_ITEM_PTR == MENU_ITEM_GET_PARENT(menuItem);
}

static void displayMenu(const MenuObject* menu)
{
	lcdClear();

	// display parent item or "MENU:" str
	lcdGoTo(1, 0);
	const MenuItemPtr currentItem = menu->m_currentItem;
	if (menuItemIsHead(currentItem))
	{
		lcdWriteStrProgMem((uchar*)PSTR("MENU:"));
	}
	else
	{
		lcdWriteStrProgMem((uchar*)(MENU_ITEM_GET_PARENT(currentItem)->m_text));
	}

	// display current item
	lcdGoTo(2, 1);
	lcdWriteStrProgMem((uchar*)currentItem->m_text);
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
}