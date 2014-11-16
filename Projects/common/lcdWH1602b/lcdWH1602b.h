#ifndef _LCDWH1602B_H_
#define _LCDWH1602B_H_

#include <common/commonHeader.h>

extern void initLcd(void);
extern BOOL lcdIsBusy(void);
extern void waitWhileBusy();
extern void lcdWriteChar(const char c);
extern void lcdWriteStr(const char* str);
extern void lcdClear(void);
extern void lcdGoTo(uint08 line, uint08 col);

extern void lcdWriteStrProgMem(const char* str);

#endif // _LCDWH1602B_H_
