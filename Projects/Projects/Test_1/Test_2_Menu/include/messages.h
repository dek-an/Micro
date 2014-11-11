#ifndef _MESSAGES_H_
#define _MESSAGES_H_

#include <avr/io.h>

#define maxHandlers 64
#define maxMessages 128
#define maxTimers	32

//���������
enum {
	MSG_KEY_PRESS=1,
	MSG_KEY_REPEAT,
	MSG_KEY_SCAN,
	MSG_LCD_REFRESH,
	MSG_MENU_SELECT,
	MSG_DISP_REFRESH,
	MSG_TIMER_SEC
};

typedef uint8_t msg_num; // ��� ������� - ��� ���� ������� ������ �����
typedef uint16_t msg_par; // ��� ��������� �������
typedef uint8_t (*handler)(msg_par); // �������� �������-�����������

// ��������� ������ �� ������ ������������
typedef struct {
	msg_num msg; // �������������� �������
	handler hnd; // ���������� ��� ����������
} iHandler;

// ��������� ������� �� ������ �������
typedef struct {
	msg_num msg; // ����� �������
	msg_par par; // ��������
} iMessage;

// ��������� �������
typedef struct {
	msg_num msg; // ����� ������������� ���������
	msg_par par; // ��� ��������
	uint16_t time; // ������ � �������� ����� (������ 10 ����)
	uint16_t timeOrigin; // ������ � �������� ����� (������ 10 ����)
} iTimer;

extern volatile iTimer lTimer[maxTimers]; // ������ ��������
extern volatile iHandler lHandler[maxHandlers]; // ������ ������������
extern volatile uint8_t numHandlers, numTimers;

extern volatile iMessage lMessage[maxMessages]; // ����� ���������
extern volatile uint16_t lMesPointer, hMesPointer; // ��������� �� ������ � ����� ������

// ��������� ����������� �������
// ����������: setHandler(MSG_KEY_PRESS, &checkKey);
void setHandler(msg_num msg, handler hnd);

// ������ ����������� �������
// ����������: killHandler(MSG_KEY_PRESS, &checkKey);
void killHandler(msg_num msg, handler hnd);

// ������� ������� � �������
// ������ ������: sendMessage(MSG_KEY_PRESS, KEY_MENU)
void sendMessage(msg_num msg, msg_par par);

// ��������� �������
void dispatchMessage();// __attribute__((always_inline));

// ���������� ������
// ������ ������: setTimer(MSG_LCD_REFRESH, 0, 50);
void setTimer(msg_num msg, msg_par par, uint16_t time);

// ����� ������
// ����������� - ������� ��� ������������� ������� �� ������ �������,
// �� �������� �� ��������� �������
void killTimer(msg_num msg);

// ��������� �������������
void initMessages();

#endif
