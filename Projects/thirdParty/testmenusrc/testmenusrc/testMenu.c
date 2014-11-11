/*
 * testMenu.c
 *
 *  ������� ���� �������. ��� ��������� ������������� �, ����������, main()
 *
 */

#include "globals.h"
#include "messages.h"
#include "keyboard.h"
#include "menu.h"

uint8_t selectMenu(msg_par par) {
	switch (par) {
	case MENU_MODE1: { // ��� ���������� ����� ������ Mode 1
		PORTC |= _BV(PC0);
		break;
	}
	case MENU_MODE2: { // ��� ���������� ����� ������ Mode 2
		PORTC |= _BV(PC1);
		break;
	}
	case MENU_MODE3: { // ��� ���������� ����� ������ Mode 3
		PORTC |= _BV(PC2);
		break;
	}
	case MENU_RESET: { // ��� ���������� ����� ������ Reset
		PORTC &= ~(_BV(PC0) | _BV(PC1) | _BV(PC2));
		break;
	}
	// ... � ��� ����� �� ���� �������
	}

	// ����� ������ ������������ � ������� ����.
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
