#include <avr/io.h>
#include <util/atomic.h>
#include "../include/messages.h"
#include "../include/globals.h"

volatile iTimer lTimer[maxTimers]; // ������ ��������
volatile iHandler lHandler[maxHandlers]; // ������ ������������
volatile uint8_t numHandlers, numTimers;

volatile iMessage lMessage[maxMessages]; // ����� ���������
volatile uint16_t lMesPointer, hMesPointer; // ��������� �� ������ � ����� ������

// ��������� ����������� �������
// ����������: setHandler(MSG_KEY_PRESS, &checkKey);
void setHandler(msg_num msg, handler hnd) {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		if (numHandlers < maxHandlers) {
			lHandler[numHandlers].hnd = hnd; // � ����������� ����������
			lHandler[numHandlers].msg = msg;
			numHandlers++;
		}
	}
}

// ������ ����������� �������
// ����������: killHandler(MSG_KEY_PRESS, &checkKey);
// ������� ���� ��������� ����������.
void killHandler(msg_num msg, handler hnd) {
	if (numHandlers==0)
		return;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		int8_t i, j;
		j = 0;
		for (i = numHandlers-1; i >= 0; i--) {

			if ((lHandler[i].msg == msg) && (lHandler[i].hnd == hnd)) {

				// �������� ��� ������ � ������ ������, ����� ����� �� ����
				for (j = i; j < numHandlers-1 ; j++) {
					lHandler[j].msg = lHandler[j + 1].msg;
					lHandler[j].hnd = lHandler[j + 1].hnd;
				}
				numHandlers--;
				break;
			}
		}

	}
}

// ������� ������� � �������
// ������ ������: sendMessage(MSG_KEY_PRESS, KEY_MENU)
void sendMessage(msg_num msg, msg_par par) {

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		hMesPointer = (hMesPointer + 1) & (maxMessages - 1); // �������� ��������� ������

		lMessage[hMesPointer].msg = msg; // ������� ������� � ��������
		lMessage[hMesPointer].par = par;
		if (hMesPointer == lMesPointer) { // ������� ������ �������, ������� �������������� ��������
			lMesPointer = (lMesPointer + 1) & (maxMessages - 1);
		}
	}
}
;

// ��������� �������
void dispatchMessage(void) {
	int8_t i;
	uint8_t res;
	msg_num msg;
	msg_par par;

	if (hMesPointer == lMesPointer) { // ���� ������ ������� - �������
		return;
	}

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		lMesPointer = (lMesPointer + 1) & (maxMessages - 1); // ������� ���������

		msg = lMessage[lMesPointer].msg;
		par = lMessage[lMesPointer].par;
	}

	if (msg != 0 && numHandlers > 0) {
		for (i = numHandlers - 1; i >= 0; i--) { // ������������� ����������� � �����
			if (lHandler[i].msg == msg) { // ��������� ���������� ����� ���������
				res = lHandler[i].hnd(par); // �������� ����������
				if (res) { // ���� ���������� ������ 1, �������� ��������� �������
					break;
				}
			}
		}
	}
}

// ���������� ������
// ������ ������: setTimer(MSG_LCD_REFRESH, 0, 50);
void setTimer(msg_num msg, msg_par par, uint16_t time) {
	uint8_t i, firstFree;
	if (time == 0) {
		sendMessage(msg, par);
	} else {

		ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
			firstFree = numTimers + 1;

			for (i = 0; i <= numTimers; i++) { // ���� ������������� ������
				if (lTimer[i].msg == 0) {
					firstFree = i;
				} else { // ���� ����� - ��������� �����
					//if ((lTimer[i].msg == msg) && (lTimer[i].par == par)) {
					if (lTimer[i].msg == msg) {
						lTimer[i].time = time;
						lTimer[i].timeOrigin = time;
						lTimer[i].par = par;
						firstFree = maxTimers + 1;
						break;
					}
				}
			}
			if (firstFree <= maxTimers) { // ����� - ������ ��������� �����
				lTimer[firstFree].msg = msg;
				lTimer[firstFree].par = par;
				lTimer[firstFree].time = time;
				lTimer[firstFree].timeOrigin = time;

				if (firstFree > numTimers)
					numTimers = firstFree;
			}
		}
	}
}

// ����� ������
// ����������� - ������� ��� ������������� ������� �� ������ �������,
// �� �������� �� ��������� �������
void killTimer(msg_num msg) {
	uint8_t i;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		for (i = 0; i < maxTimers; i++) {
			if (lTimer[i].msg == msg) {
				lTimer[i].msg = 0;
			}
		}
		while ((lTimer[numTimers].msg == 0) && (numTimers > 0))
			numTimers--;
	}
}

// ��������� ��������

void dispatchTimer() {
	uint8_t i;

	for (i = 0; i <= numTimers; i++) {
		if (lTimer[i].msg == 0)
			continue;

		if (lTimer[i].time > 0) { // ���� �� ������ �����
			lTimer[i].time--; // ������ ��������� �����
		} else {
			sendMessage(lTimer[i].msg, lTimer[i].par); // ������� �������
			lTimer[i].time = lTimer[i].timeOrigin;
		}
	}
}

void initMessages() {
	numHandlers = 0;
	numTimers = 0;
	lMesPointer = 0;
	hMesPointer = 0;

	// ������� ������
	TCCR2 = _BV(WGM21) | _BV(CS22) | _BV(CS20);
	OCR2 = F_CPU / 102400; // ������ 1/100 ���, �������� 1/1024
	TIMSK |= _BV(OCIE2);
}

ISR(TIMER2_COMP_vect)
{
	dispatchTimer(); // � ��� � ���������
}
