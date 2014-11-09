#ifndef _LCDWH1602b_H_
#define _LCDWH1602b_H_

void initLCD8b(void);
void writeCharToLCD(const unsigned char c);
void writeStrToLCD(const unsigned char* str, const unsigned char size);
void clearLCD(void);

#endif // _LCDWH1602b_H_
