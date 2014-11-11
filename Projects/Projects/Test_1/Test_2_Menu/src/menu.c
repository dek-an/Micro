#include "../include/globals.h"
#include "../include/menu.h"
#include "../include/keyboard.h"
#include "../include/messages.h"
#include "../include/lcd.h"


typedef struct PROGMEM{
	void       *Next;
	void       *Previous;
	void       *Parent;
	void       *Child;
	uint8_t     Select;
	const char  Text[];
} menuItem;

menuItem* selectedMenuItem; // текущий пункт меню

menuItem* menuStack[10];
volatile uint8_t menuStackTop;


#define MAKE_MENU(Name, Next, Previous, Parent, Child, Select, Text) \
    extern const menuItem Next;     \
	extern const menuItem Previous; \
	extern const menuItem Parent;   \
	extern const menuItem Child;  \
	const menuItem Name = {(void*)&Next, (void*)&Previous, (void*)&Parent, (void*)&Child, (uint8_t)Select, { Text }}

#define PREVIOUS   ((menuItem*)pgm_read_word(&selectedMenuItem->Previous))
#define NEXT       ((menuItem*)pgm_read_word(&selectedMenuItem->Next))
#define PARENT     ((menuItem*)pgm_read_word(&selectedMenuItem->Parent))
#define CHILD      ((menuItem*)pgm_read_word(&selectedMenuItem->Child))
#define SELECT		(pgm_read_byte(&selectedMenuItem->Select))

const char strNULL[] PROGMEM = "";

#define NULL_ENTRY Null_Menu
const menuItem        Null_Menu = {(void*)0, (void*)0, (void*)0, (void*)0, 0, {0x00}};

//                 NEXT,      PREVIOUS     PARENT,     CHILD
MAKE_MENU(m_s1i1,  m_s1i2,    NULL_ENTRY,  NULL_ENTRY, m_s2i1,       0, "Start");
MAKE_MENU(m_s1i2,  m_s1i3,    m_s1i1,      NULL_ENTRY, m_s3i1,       0, "Settings");
MAKE_MENU(m_s1i3,  NULL_ENTRY,m_s1i2,      NULL_ENTRY, NULL_ENTRY,   MENU_RESET, "Reset");

// подменю Запуск
MAKE_MENU(m_s2i1,  m_s2i2,    NULL_ENTRY,  m_s1i1,     NULL_ENTRY,   MENU_MODE1, "Mode 1");
MAKE_MENU(m_s2i2,  m_s2i3,    m_s2i1,      m_s1i1,     NULL_ENTRY,   MENU_MODE2, "Mode 2");
MAKE_MENU(m_s2i3,  NULL_ENTRY,m_s2i2,      m_s1i1,     NULL_ENTRY,   MENU_MODE3, "Mode 3");

// подменю Настройка
MAKE_MENU(m_s3i1,  m_s3i2,    NULL_ENTRY,  m_s1i2,     m_s4i1,       0, "Pressure");
MAKE_MENU(m_s3i2,  NULL_ENTRY,m_s3i1,      m_s1i2,     m_s5i1,       0, "Time");

// подменю Давление
MAKE_MENU(m_s4i1,  m_s4i2,    NULL_ENTRY,  m_s3i1,     NULL_ENTRY,   MENU_SENS1, "Sensor 1");
MAKE_MENU(m_s4i2,  NULL_ENTRY,m_s4i1,      m_s3i1,     NULL_ENTRY,   MENU_SENS2, "Sensor 2");

// подменю Время
MAKE_MENU(m_s5i1,  m_s5i2,    NULL_ENTRY,  m_s3i2,     NULL_ENTRY,   MENU_WARM, "Warm");
MAKE_MENU(m_s5i2,  NULL_ENTRY,m_s5i1,      m_s3i2,     NULL_ENTRY,   MENU_PROCESS, "Process");


void menuChange(menuItem* NewMenu)
{
	if ((void*)NewMenu == (void*)&NULL_ENTRY)
	  return;

	selectedMenuItem = NewMenu;
}

unsigned char dispMenu(msg_par par) {
	menuItem* tempMenu;

	lcd_clrscr(1);
	// первая строка - заголовок. Или пункт меню верхнего уровня
	lcd_gotoxy(1,0);
	tempMenu = (menuItem*)pgm_read_word(&selectedMenuItem->Parent);
	if ((void*)tempMenu == (void*)&NULL_ENTRY) { // мы на верхнем уровне
		lcd_puts_p(PSTR("MENU:"));
	} else {
		lcd_puts_p((char *)tempMenu->Text);
	}

	lcd_clrscr(2);
	// Вторая строка - текущий пункт меню
	lcd_gotoxy(2,1);
	lcd_puts_p((char *)selectedMenuItem->Text);

	return (1);
}

uint8_t menuKey(msg_par par) {
	switch (par) {
	case 0: {
		return 1;
	}
	case KEY_LEFT: {
		menuChange(PREVIOUS);
		break;
	}
	case KEY_RIGHT: {
		menuChange(NEXT);
		break;
	}
	case KEY_DOWN:
		;
	case KEY_OK:
		{ // выбор пункта
			uint8_t sel;
			sel = SELECT;
			if (sel != 0) {
				sendMessage(MSG_MENU_SELECT, sel);

				killHandler(MSG_KEY_PRESS, &menuKey);
				killHandler(MSG_DISP_REFRESH, &dispMenu);

				return (1);
			} else {
				menuChange(CHILD);
			}
			break;
		}
	case KEY_UP: { // отмена выбора (возврат)
		menuChange(PARENT);
	}
	}
	dispMenu(0);
	return (1);
}

uint8_t startMenu() {
	selectedMenuItem = (menuItem*)&m_s1i1;

	dispMenu(0);
	setHandler(MSG_KEY_PRESS, &menuKey);
	setHandler(MSG_DISP_REFRESH, &dispMenu);
	return (0);
}

void initMenu() {
	lcd_init();
}
