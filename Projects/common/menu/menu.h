#ifndef _MENU_H_
#define _MENU_H_

#include <common/commonHeader.h>
#include <common/rtos/libRtos.h>

// //////////////////////////////////////////////////////////
// Menu Macroses
//
#define MENU_MAKE_ITEM(name, next, previous, parent, child, select, text) \
	extern const MenuItem next; \
	extern const MenuItem previous; \
	extern const MenuItem parent; \
	extern const MenuItem child; \
	const MenuItem name = {&next, &previous, &parent, &child, select, { text }}

#define MENU_ITEM_GET_PREVIOUS(menuItem)	((MenuItemPtr)pgm_read_word(&((menuItem)->m_previous)))
#define MENU_ITEM_GET_NEXT(menuItem)		((MenuItemPtr)pgm_read_word(&((menuItem)->m_next)))
#define MENU_ITEM_GET_PARENT(menuItem)	((MenuItemPtr)pgm_read_word(&((menuItem)->m_parent)))
#define MENU_ITEM_GET_CHILD(menuItem)	((MenuItemPtr)pgm_read_word(&((menuItem)->m_child)))
#define MENU_ITEM_GET_TASK(menuItem)	((Task)pgm_read_word(&((menuItem)->m_task)))
// MENU_ITEM_GET_TEXT is no needed as we have lcdWriteStrProgMem() method

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
	const uchar m_text[];
} MenuItem;

typedef struct MenuObject
{
	MenuItemPtr m_menuHead;
	MenuItemPtr m_currentItem;
} MenuObject;

extern const MenuItem EMPTY_MENU_ITEM;

extern void initMenu(void);
extern void startMenu(void);
extern void resetMenu(MenuObject* menu);

#endif // _MENU_H_