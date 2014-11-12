#ifndef _MENUDEF_H_
#define _MENUDEF_H_

// //////////////////////////////////////////////////////////
// Menu Macroses
//
#define MENU_MAKE_ITEM(name, next, previous, parent, child, select, text) \
	extern const MenuItem next; \
	extern const MenuItem previous; \
	extern const MenuItem parent; \
	extern const MenuItem child; \
	const MenuItem name = {&next, &previous, &parent, &child, select, { text }}

#define MENU_GET_PREVIOUS(menuItem)	((MenuItemPtr)pgm_read_word(&menuItem->m_previous))
#define MENU_GET_NEXT(menuItem)		((MenuItemPtr)pgm_read_word(&menuItem->m_next))
#define MENU_GET_PARENT(menuItem)	((MenuItemPtr)pgm_read_word(&menuItem->m_parent))
#define MENU_GET_CHILD(menuItem)	((MenuItemPtr)pgm_read_word(&menuItem->m_child))
#define MENU_GET_SELECT(menuItem)	((Task)pgm_read_word(&menuItem->m_task))

#endif // _MENUDEF_H_