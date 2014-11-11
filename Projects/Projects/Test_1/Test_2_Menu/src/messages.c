#include <avr/io.h>
#include <util/atomic.h>
#include "../include/messages.h"
#include "../include/globals.h"

volatile iTimer lTimer[maxTimers]; // список таймеров
volatile iHandler lHandler[maxHandlers]; // список обработчиков
volatile uint8_t numHandlers, numTimers;

volatile iMessage lMessage[maxMessages]; // буфер сообщений
volatile uint16_t lMesPointer, hMesPointer; // указатели на начало и конец буфера

// установка обработчика события
// вызывается: setHandler(MSG_KEY_PRESS, &checkKey);
void setHandler(msg_num msg, handler hnd) {
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		if (numHandlers < maxHandlers) {
			lHandler[numHandlers].hnd = hnd; // и регистрирем обработчик
			lHandler[numHandlers].msg = msg;
			numHandlers++;
		}
	}
}

// снятие обработчика события
// вызывается: killHandler(MSG_KEY_PRESS, &checkKey);
// удаляет один последний обработчик.
void killHandler(msg_num msg, handler hnd) {
	if (numHandlers==0)
		return;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		int8_t i, j;
		j = 0;
		for (i = numHandlers-1; i >= 0; i--) {

			if ((lHandler[i].msg == msg) && (lHandler[i].hnd == hnd)) {

				// сдвигаем все записи к началу списка, чтобы дырок не было
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

// занести событие в очередь
// пример вызова: sendMessage(MSG_KEY_PRESS, KEY_MENU)
void sendMessage(msg_num msg, msg_par par) {

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		hMesPointer = (hMesPointer + 1) & (maxMessages - 1); // сдвигаем указатель головы

		lMessage[hMesPointer].msg = msg; // заносим событие и параметр
		lMessage[hMesPointer].par = par;
		if (hMesPointer == lMesPointer) { // догнали начало очереди, убиваем необработанное сообытие
			lMesPointer = (lMesPointer + 1) & (maxMessages - 1);
		}
	}
}
;

// обработка событий
void dispatchMessage(void) {
	int8_t i;
	uint8_t res;
	msg_num msg;
	msg_par par;

	if (hMesPointer == lMesPointer) { // если пустая очередь - возврат
		return;
	}

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		lMesPointer = (lMesPointer + 1) & (maxMessages - 1); // сдвинем указатель

		msg = lMessage[lMesPointer].msg;
		par = lMessage[lMesPointer].par;
	}

	if (msg != 0 && numHandlers > 0) {
		for (i = numHandlers - 1; i >= 0; i--) { // просматриваем обработчики с конца
			if (lHandler[i].msg == msg) { // последний занесенный имеет приоритет
				res = lHandler[i].hnd(par); // вызываем обработчик
				if (res) { // если обработчик вернул 1, перываем обработку события
					break;
				}
			}
		}
	}
}

// установить таймер
// пример вызова: setTimer(MSG_LCD_REFRESH, 0, 50);
void setTimer(msg_num msg, msg_par par, uint16_t time) {
	uint8_t i, firstFree;
	if (time == 0) {
		sendMessage(msg, par);
	} else {

		ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
			firstFree = numTimers + 1;

			for (i = 0; i <= numTimers; i++) { // ищем установленный таймер
				if (lTimer[i].msg == 0) {
					firstFree = i;
				} else { // если нашли - обновляем время
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
			if (firstFree <= maxTimers) { // иначе - просто добавляем новый
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

// убить таймер
// особенность - убивает все установленные таймеры на данное событие,
// не зависимо от параметра события
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

// диспетчер таймеров

void dispatchTimer() {
	uint8_t i;

	for (i = 0; i <= numTimers; i++) {
		if (lTimer[i].msg == 0)
			continue;

		if (lTimer[i].time > 0) { // если не пришло время
			lTimer[i].time--; // просто уменьшаем время
		} else {
			sendMessage(lTimer[i].msg, lTimer[i].par); // создаем событие
			lTimer[i].time = lTimer[i].timeOrigin;
		}
	}
}

void initMessages() {
	numHandlers = 0;
	numTimers = 0;
	lMesPointer = 0;
	hMesPointer = 0;

	// главный таймер
	TCCR2 = _BV(WGM21) | _BV(CS22) | _BV(CS20);
	OCR2 = F_CPU / 102400; // период 1/100 сек, делитель 1/1024
	TIMSK |= _BV(OCIE2);
}

ISR(TIMER2_COMP_vect)
{
	dispatchTimer(); // а вот и диспетчер
}
