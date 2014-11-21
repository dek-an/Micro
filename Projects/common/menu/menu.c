#include "menu.h"
#include "menuDef.h"

#include <common/lcdWH1602b/lcdWH1602b.h>

// //////////////////////////////////////////////////////////
// Defines
//
#define EMPTY_MENU_ITEM_PTR (const MenuItemPtr)&EMPTY_MENU_ITEM

#define MENU_ITEM_GET_PREVIOUS(menuItem)	((MenuItemPtr)pgm_read_word(&((menuItem)->m_previous)))
#define MENU_ITEM_GET_NEXT(menuItem)		((MenuItemPtr)pgm_read_word(&((menuItem)->m_next)))
#define MENU_ITEM_GET_PARENT(menuItem)	((MenuItemPtr)pgm_read_word(&((menuItem)->m_parent)))
#define MENU_ITEM_GET_CHILD(menuItem)	((MenuItemPtr)pgm_read_word(&((menuItem)->m_child)))
#define MENU_ITEM_GET_TASK(menuItem)	((Task)pgm_read_word(&((menuItem)->m_task)))
// MENU_ITEM_GET_TEXT is no needed as we have lcdWriteStrProgMem() method

// //////////////////////////////////////////////////////////
// Helpers
//
static inline BOOL menuItemIsHead(const MenuItemPtr menuItem);
static inline BOOL menuItemIsEmpty(const MenuItemPtr menuItem);
static inline BOOL menuIsInvoked(const MenuObject* menu);
static inline void updateCurrentItem(MenuObject* menu, const MenuItemPtr newCurrent);
static inline void clearInvokedTask(MenuObject* menu);
static inline void setInvokedTask(MenuObject* menu, Task task);
static void displayMenu(MenuObject* menu);

// //////////////////////////////////////////////////////////
// Interface Implementation
//
const MenuItem EMPTY_MENU_ITEM = {0, 0, 0, 0, &idleTask, {0x00}};

void initMenu(void)
{
	INITIALIZE_CHECKING();

	initLcd();
}

void startMenu(MenuObject* menu, const MenuItemPtr head)
{
	menu->m_menuHead = head;
	menu->m_lastDisplayed = EMPTY_MENU_ITEM_PTR;
	clearInvokedTask(menu);

	updateCurrentItem(menu, head);
}

void updateMenuTask(const TaskParameter param)
{
	displayMenu((MenuObject*)param);
}

void resetMenu(MenuObject* menu)
{
	clearInvokedTask(menu);
	menu->m_lastDisplayed = EMPTY_MENU_ITEM_PTR;
	updateCurrentItem(menu, menu->m_menuHead);
}

void menuNext(MenuObject* menu, const TaskParameter param)
{
	if (menuIsInvoked(menu))
	{
		setTask(menu->m_invokedTask, param);
		return;
	}

	const MenuItemPtr next = MENU_ITEM_GET_NEXT(menu->m_currentItem);
	if (menuItemIsEmpty(next))
		return;

	updateCurrentItem(menu, next);
}

void menuPrev(MenuObject* menu, const TaskParameter param)
{
	if (menuIsInvoked(menu))
	{
		setTask(menu->m_invokedTask, param);
		return;
	}

	const MenuItemPtr prev = MENU_ITEM_GET_PREVIOUS(menu->m_currentItem);
	if (menuItemIsEmpty(prev))
		return;

	updateCurrentItem(menu, prev);
}

void menuStepOut(MenuObject* menu, const TaskParameter param)
{
	if (menuIsInvoked(menu))
	{
		setTask(menu->m_invokedTask, param);
		clearInvokedTask(menu);
		setTask(&updateMenuTask, (const TaskParameter)menu);
	}
	else
	{
		const MenuItemPtr parent = MENU_ITEM_GET_PARENT(menu->m_currentItem);
		if (menuItemIsEmpty(parent))
			return;

		updateCurrentItem(menu, parent);
	}
}

void menuStepIn(MenuObject* menu, const TaskParameter param)
{
	if (menuIsInvoked(menu))
	{
		setTask(menu->m_invokedTask, param);
		return;
	}

	const MenuItemPtr child = MENU_ITEM_GET_CHILD(menu->m_currentItem);
	if (menuItemIsEmpty(child)) // if this is lowest item
	{
		const Task currentTask = MENU_ITEM_GET_TASK(menu->m_currentItem);
		if (currentTask != &idleTask)
		{
			setInvokedTask(menu, currentTask);
			menu->m_lastDisplayed = EMPTY_MENU_ITEM_PTR;
			setTask(currentTask, param);
		}
	}
	else
	{
		updateCurrentItem(menu, child);
	}
}

BOOL menuIsHead(MenuObject* menu)
{
	return menuItemIsHead(menu->m_currentItem);
}

// //////////////////////////////////////////////////////////
// Helpers Defines
//

// //////////////////////////////////////////////////////////
// Helpers Implementation
//
static inline BOOL menuItemIsHead(const MenuItemPtr menuItem)
{
	return EMPTY_MENU_ITEM_PTR == MENU_ITEM_GET_PARENT(menuItem);
}

static inline BOOL menuItemIsEmpty(const MenuItemPtr menuItem)
{
	return EMPTY_MENU_ITEM_PTR == menuItem;
}

static inline BOOL menuIsInvoked(const MenuObject* menu)
{
	return (menu->m_invokedTask != &idleTask);
}

static inline void updateCurrentItem(MenuObject* menu, const MenuItemPtr newCurrent)
{
	if (newCurrent == EMPTY_MENU_ITEM_PTR)
		return;

	menu->m_currentItem = newCurrent;
	setTask(&updateMenuTask, (const TaskParameter)menu);
}

static inline void clearInvokedTask(MenuObject* menu)
{
	menu->m_invokedTask = &idleTask;
}

static inline void setInvokedTask(MenuObject* menu, Task task)
{
	menu->m_invokedTask = task;
}

static void displayMenu(MenuObject* menu)
{
	const MenuItemPtr currentItem = menu->m_currentItem;
	if (currentItem == menu->m_lastDisplayed)
		return;

	lcdClear();

	// display parent item or "MENU:" str
	lcdGoTo(0, 0);
	const MenuItemPtr parentItem = MENU_ITEM_GET_PARENT(currentItem);
	if (menuItemIsEmpty(parentItem))
	{
		lcdWriteStrProgMem(PSTR("MENU:"));
	}
	else
	{
		lcdWriteStrProgMem(parentItem->m_text);
	}

	// display current item
	lcdGoTo(1, 2);
	lcdWriteStrProgMem(currentItem->m_text);

	menu->m_lastDisplayed = currentItem;
}
