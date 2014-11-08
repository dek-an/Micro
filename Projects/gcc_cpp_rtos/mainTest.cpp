#include <rtos/libRTOS.h>

void task1()
{
	volatile uint08 i = SREG;
	++i;
}

int main(void)
{
	cpp_ns::rtos_ns::RtosInterface& rtos = cpp_ns::rtos_ns::RtosInterface::getInstance();
	rtos.setTask(task1);

	while(1)
	{
		rtos.taskManager();
	}

	return 0;
}
