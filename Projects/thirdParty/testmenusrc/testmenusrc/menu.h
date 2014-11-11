#ifndef _MENU_H_
#define _MENU_H_

enum {
    MENU_CANCEL=1,
    MENU_RESET,
    MENU_MODE1,
    MENU_MODE2,
    MENU_MODE3,
    MENU_SENS1,
    MENU_SENS2,
    MENU_WARM,
    MENU_PROCESS
};

uint8_t startMenu();
void initMenu();

#endif
