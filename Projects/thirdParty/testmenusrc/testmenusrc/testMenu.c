/*
 * testMenu.c
 *
 *  Главный файл проекта. Тут процедуры инициализации и, собственно, main()
 *
 */

#include "globals.h"
#include "messages.h"
#include "keyboard.h"
#include "menu.h"

uint8_t selectMenu(msg_par par) {
	switch (par) {
	case MENU_MODE1: { // тут обработать выбор пункта Mode 1
		PORTC |= _BV(PC0);
		break;
	}
	case MENU_MODE2: { // тут обработать выбор пункта Mode 2
		PORTC |= _BV(PC1);
		break;
	}
	case MENU_MODE3: { // тут обработать выбор пункта Mode 3
		PORTC |= _BV(PC2);
		break;
	}
	case MENU_RESET: { // тут обработать выбор пункта Reset
		PORTC &= ~(_BV(PC0) | _BV(PC1) | _BV(PC2));
		break;
	}
	// ... и так далее по всем пунктам
	}

	// после выбора возвращаемся в главное меню.
	startMenu();
	return 1;
}

int main() {
	initMessages();
	initMenu();
	KBD_init();
	sei();
	DDRC = _BV(PC0) | _BV(PC1) | _BV(PC2);

	setHandler(MSG_MENU_SELECT, &selectMenu);
	startMenu();

	for(;;) {
		dispatchMessage();
	}
}
