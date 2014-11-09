/* AVR GCC, ATmega8, WH1602B */



#include <avr/io.h>

#include <util/delay.h>

         

// порт к которому подключена шина данных ЖКД

#define PORT_DATA PORTC

#define PIN_DATA  PINC

#define DDRX_DATA DDRC



// порт к которому подключены управляющие выводы ЖКД

#define PORT_SIG PORTA

#define PIN_SIG  PINA

#define DDRX_SIG DDRA



// номера выводов к которым подключены управляющие выводы ЖКД 

#define RS 0

#define RW 1

#define EN 2



// макросы для работы с битами

#define ClearBit(reg, bit)       reg &= (~(1<<(bit)))

#define SetBit(reg, bit)         reg |= (1<<(bit))	



// функция записи команды 

void LcdWriteCom(unsigned char data)

{

  ClearBit(PORT_SIG, RS);	// установка RS в 0 - команды

  PORT_DATA = data;		// вывод данных на шину индикатора 

  SetBit(PORT_SIG, EN);	        // установка EN в 1

  _delay_us(2);                 // ждем 2 мкс

  ClearBit(PORT_SIG, EN);	// установка EN в 0

  _delay_us(40);                // ждем 40 мкс

}



// функция записи данных

void LcdWriteData(unsigned char data)

{

  SetBit(PORT_SIG, RS);	        // установка RS в 1 - данные

  PORT_DATA = data;		// вывод данных на шину индикатора 

  SetBit(PORT_SIG, EN);	        // установка EN в 1

  _delay_us(2);                 // ждем 2 мкс

  ClearBit(PORT_SIG, EN);	// установка EN в 0

  _delay_us(40);                // ждем 40 мкс

}



// функция очистки дисплея

void Lcd_Clear(void)

{

  LcdWriteCom(0x01);

  _delay_ms(2);                 // ждем 2 мс

}



// позиционирование курсора

void LCD_Goto(unsigned char x, unsigned char y)

{

  LcdWriteCom(((((y)& 1)*0x40)+((x)& 15))|128);

}



// функция инициализации

void InitLcd(void)

{

  DDRX_DATA = 0xff;

  PORT_DATA = 0xff;	

  DDRX_SIG = 0xff;

  PORT_SIG |= (1<<RW)|(1<<RS)|(1<<EN); // установка RW, RS, EN в 1

  ClearBit(PORT_SIG, RW);              // установка RW в 0



  _delay_ms(40);           // ждем 40 мс

  LcdWriteCom(0b00111000); // 8 разрядная шина, 5*8, 2 строки

  LcdWriteCom(0b00001100); // дисплей включен, курсор выключен, мерцание выключено

  LcdWriteCom(0b00000001); // очистка дисплея

  _delay_ms(2);            // ждем 2 мс

  LcdWriteCom(0b00000110); // курсор движется вправо, сдвига нет

}



// основная функция программы

int main( void )

{

  InitLcd();



  LcdWriteData('T');

  LcdWriteData('e');

  LcdWriteData('s');

  LcdWriteData('t');

  LcdWriteData('.');



  _delay_ms(3000);



  Lcd_Clear();



  LcdWriteData('0');

  LcdWriteData('1');

  LcdWriteData('2');

  LcdWriteData('3');

  LcdWriteData('4');

  LcdWriteData('5');

  LcdWriteData('6');

  LcdWriteData('7');



  _delay_ms(3000);



  LCD_Goto(0,1);



  LcdWriteData('8');

  LcdWriteData('9');

  LcdWriteData('.');

  LcdWriteData('1');

  LcdWriteData(' ');

  LcdWriteData('e');

  LcdWriteData('f');

  LcdWriteData('g');



  while(1);



  return 0;

}

/*#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// macroses
#define SBI(port, pin) \
	(port) |= _BV(pin)
#define CBI(port, pin) \
	(port) &= ~_BV(pin)

#define HI(x) ((x) >> 8)
#define LO(x) ((x) & 0xFF)

// display
#define DISPLAY_DATA_PORT PORTC
#define DISPLAY_DATA_DIR DDRC

#define DISPLAY_CONTROL_PORT PORTA
#define DISPLAY_CONTROL_DIR DDRA

#define DISPLAY_RS_PIN 0
#define DISPLAY_RW_PIN 1
#define DISPLAY_E_PIN 2

// ///////////////////////////////////////////////
// interrupts
//
// USART, Rx Complete
ISR(USARTRXC_vect);

// ///////////////////////////////////////////////
// functions
//
void init(void);
void initUSART(void);

void applyDisplayCommand(int delayUs);
void switchOnDisplay(void);
void clearDisplay(void);
void writeToDisplay(unsigned char c);
void writeToDisplayStr(unsigned char* str, unsigned char strSize);

int main(void)
{
	init();

	switchOnDisplay();

	while (1)
	{
		clearDisplay();
		writeToDisplayStr((unsigned char*)"ololo privet yo", (unsigned char)15);
		_delay_ms(10000);
	}

	return 0;
}

void init(void)
{
	initUSART();

	// init display port
	DISPLAY_DATA_DIR = 255; // all pins to out
	DISPLAY_CONTROL_DIR |= _BV(DISPLAY_RS_PIN) | _BV(DISPLAY_RW_PIN) | _BV(DISPLAY_E_PIN);

	CBI(DISPLAY_CONTROL_PORT, DISPLAY_RS_PIN);
	CBI(DISPLAY_CONTROL_PORT, DISPLAY_RW_PIN);
	DISPLAY_DATA_PORT = 3 << 4;
	applyDisplayCommand(500);

	sei();
}

void initUSART(void)
{
#define BAUD_RATE 9600L
#define BAUD_VALUE F_CPU / (16 * BAUD_RATE) - 1

	// set Baud Rate Register
	//
	UBRRL = LO(BAUD_VALUE);

	char highBaudVal = HI(BAUD_VALUE);
	// Register Select UBRRH (URSEL = 0)
	highBaudVal &= ~(_BV(URSEL));
	// Reserved 14:12(6:4) Bits in UBRRH
	highBaudVal &= ~(_BV(6) | _BV(5) | _BV(4));

	UBRRH = highBaudVal;

	// set Control and Status Registers
	//
	UCSRA = 0;

	// Receive Enable (RXEN)
	// Receive Complete Interrupt Enable (RXCIE)
	// Transmitt Enable (TXEN)
	// Transmitt Complete Interrupt Enable (TXCIE)
	UCSRB = _BV(RXEN) | _BV(RXCIE);

	// Register Select UCSRC (URSEL = 1)
	// Character Size 8
	//     see table from datasheet:
	//         UCSZ2 = 0 (in UCSRB), UCSZ1 = 1, UCSZ0 = 1
	// Mode Select Asynchronous (UMSEL = 0)
	UCSRC = _BV(URSEL) | _BV(UCSZ1) | _BV(UCSZ0);
}


void applyDisplayCommand(int delayUs)
{
	SBI(DISPLAY_CONTROL_PORT, DISPLAY_E_PIN);
	_delay_us(delayUs);
	CBI(DISPLAY_CONTROL_PORT, DISPLAY_E_PIN);
}

void switchOnDisplay(void)
{
	DISPLAY_DATA_PORT = 0;

	SBI(DISPLAY_DATA_PORT, 3);
	SBI(DISPLAY_DATA_PORT, 2);

	CBI(DISPLAY_CONTROL_PORT, DISPLAY_RS_PIN);
	CBI(DISPLAY_CONTROL_PORT, DISPLAY_RW_PIN);

	applyDisplayCommand(37);
}

void clearDisplay(void)
{
#define DISPLAY_CLEAR_DELAY_US 153
	DISPLAY_DATA_PORT = 1;
	CBI(DISPLAY_CONTROL_PORT, DISPLAY_RS_PIN);
	CBI(DISPLAY_CONTROL_PORT, DISPLAY_RW_PIN);

	applyDisplayCommand(DISPLAY_CLEAR_DELAY_US);
}

void writeToDisplay(unsigned char c)
{
#define DISPLAY_WRITE_DELAY_US 43
	DISPLAY_DATA_PORT = c;
	SBI(DISPLAY_CONTROL_PORT, DISPLAY_RS_PIN);
	CBI(DISPLAY_CONTROL_PORT, DISPLAY_RW_PIN);

	applyDisplayCommand(DISPLAY_WRITE_DELAY_US);
}

void writeToDisplayStr(unsigned char* str, unsigned char strSize)
{
	for (const unsigned char* strEnd = str + strSize; str < strEnd; ++str)
	{
		writeToDisplay(*str);
	}
}

// ///////////////////////////////////////////////
// interrupts implementation
//
// USART, Rx Complete
ISR(USARTRXC_vect)
{
	unsigned char udrVal = UDR;

	switch (udrVal)
	{
		case 'C':
			clearDisplay();
			break;
		default:
			writeToDisplay(udrVal);
			break;
	}
}
*/
