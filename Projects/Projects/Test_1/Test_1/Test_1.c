/*
 * Test_1.c
 *
 * Created: 10/11/2014 13:41:15
 *  Author: dekarand
 */ 

#include <common/rtos/libRtos.h>
#include <common/lcdWH1602b/lcdWH1602b.h>

int main(void)
{
	initRtos();
	initLcd();

    while(1)
    {
		taskManager();
    }

	return 0;
}