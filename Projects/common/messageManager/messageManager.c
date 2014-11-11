#include "messageManager.h"

void initMessageManager(void)
{
	
}


void registerHandler(MessageType msg, Handler hnd)
{
	
}

void unregisterHandler(MessageType msg, Handler hnd)
{
	
}


void sendMessage(MessageType msg, MessageParameter param)
{
	
}

void dispatchMessages(void)
{
	
}


void setTimer(MessageType msg, MessageParameter param, uint16 timeMs)
{
	
}

void killTimer(MessageType msg)
{
	
}

//// структура записи из списка обработчиков
//typedef struct {
	//msg_num msg; // обрабатываемое событие
	//handler hnd; // собственно сам обработчик
//} iHandler;
//
//// структура события из буфера событий
//typedef struct {
	//msg_num msg; // номер события
	//msg_par par; // параметр
//} iMessage;
//
//// структура таймера
//typedef struct {
	//msg_num msg; // номер генерируемого сообщения
	//msg_par par; // его параметр
	//uint16_t time; // таймер в условных тиках (сейчас 10 мсек)
	//uint16_t timeOrigin; // таймер в условных тиках (сейчас 10 мсек)
//} iTimer;
//
//volatile iTimer lTimer[maxTimers]; // список таймеров
//volatile iHandler lHandler[maxHandlers]; // список обработчиков
//volatile uint8_t numHandlers, numTimers;
//
//volatile iMessage lMessage[maxMessages]; // буфер сообщений
//volatile uint16_t lMesPointer, hMesPointer; // указатели на начало и конец буфера