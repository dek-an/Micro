#include "messageManager.h"

extern void initMessageManager();

extern void registerHandler(MessageType msg, Handler hnd);
extern void unregisterHandler(MessageType msg, Handler hnd);

extern void sendMessage(MessageType msg, MessageParameter param);
extern void dispatchMessages() __attribute__((always_inline));

extern void setTimer(MessageType msg, MessageParameter param, uint16 timeMs);
extern void killTimer(MessageType msg);

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