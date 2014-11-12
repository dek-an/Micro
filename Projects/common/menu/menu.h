#ifndef _MENU_H_
#define _MENU_H_

#include <common/commonHeader.h>
#include <common/rtos/libRtos.h>

typedef struct MenuItem* MenuItemPtr;
typedef struct PROGMEM MenuItem
{
	MenuItemPtr m_next;
	MenuItemPtr m_previous;
	MenuItemPtr m_parent;
	MenuItemPtr m_child;
	Task m_task;
	const uchar m_text[];
} MenuItem;

typedef struct PROGMEM MenuObject
{
	MenuItemPtr m_menuHead;
	MenuItemPtr m_currentItem;
} MenuObject;

extern MenuItem EMPTY_MENU_ITEM;

extern void initMenu(void);
extern void startMenu(void);
extern void resetMenu(MenuObject* menu);

#endif // _MENU_H_