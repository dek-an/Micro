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

//// ��������� ������ �� ������ ������������
//typedef struct {
	//msg_num msg; // �������������� �������
	//handler hnd; // ���������� ��� ����������
//} iHandler;
//
//// ��������� ������� �� ������ �������
//typedef struct {
	//msg_num msg; // ����� �������
	//msg_par par; // ��������
//} iMessage;
//
//// ��������� �������
//typedef struct {
	//msg_num msg; // ����� ������������� ���������
	//msg_par par; // ��� ��������
	//uint16_t time; // ������ � �������� ����� (������ 10 ����)
	//uint16_t timeOrigin; // ������ � �������� ����� (������ 10 ����)
//} iTimer;
//
//volatile iTimer lTimer[maxTimers]; // ������ ��������
//volatile iHandler lHandler[maxHandlers]; // ������ ������������
//volatile uint8_t numHandlers, numTimers;
//
//volatile iMessage lMessage[maxMessages]; // ����� ���������
//volatile uint16_t lMesPointer, hMesPointer; // ��������� �� ������ � ����� ������