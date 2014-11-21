#ifndef _MENU_H_
#define _MENU_H_

#include <common/commonHeader.h>
#include <common/rtos/rtos.h>

// //////////////////////////////////////////////////////////
// Menu Macroses
//
#define MENU_MAKE_ITEM(name, next, previous, parent, child, task, text) \
	extern const MenuItem next; \
	extern const MenuItem previous; \
	extern const MenuItem parent; \
	extern const MenuItem child; \
	const MenuItem name = {(const MenuItemPtr)&next, (const MenuItemPtr)&previous, (const MenuItemPtr)&parent, (const MenuItemPtr)&child, task, { text }}

#define MENU_ITEM_CPTR(menuItem) (const MenuItemPtr)(&(menuItem))

// //////////////////////////////////////////////////////////
// Menu Types
//
typedef struct MenuItem* MenuItemPtr;
typedef struct PROGMEM MenuItem
{
	MenuItemPtr m_next;
	MenuItemPtr m_previous;
	MenuItemPtr m_parent;
	MenuItemPtr m_child;
	Task m_task;
	const char m_text[];
} MenuItem;

typedef struct MenuObject
{
	MenuItemPtr m_menuHead;
	MenuItemPtr m_currentItem;
	MenuItemPtr m_lastDisplayed;
	Task m_invokedTask;
} MenuObject;

extern const MenuItem EMPTY_MENU_ITEM;

extern void initMenu(void) /*__attribute__((always_inline))*/;
extern void startMenu(MenuObject* menu, const MenuItemPtr head);
extern void resetMenu(MenuObject* menu);
extern BOOL menuIsHead(MenuObject* menu);
// use param in next 4 methods to send key number
extern void menuNext(MenuObject* menu, const TaskParameter param);
extern void menuPrev(MenuObject* menu, const TaskParameter param);
extern void menuStepOut(MenuObject* menu, const TaskParameter param);
extern void menuStepIn(MenuObject* menu, const TaskParameter param);

extern void updateMenuTask(const TaskParameter param); // should be private?

#endif // _MENU_H_
