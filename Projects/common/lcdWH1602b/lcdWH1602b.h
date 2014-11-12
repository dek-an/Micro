#ifndef _LCDWH1602B_H_
#define _LCDWH1602B_H_

#include <common/commonHeader.h>

extern void initLcd(void);
extern BOOL lcdIsBusy(void);
extern void lcdWriteChar(const uchar c);
extern void lcdWriteStr(const uchar* str);
extern void lcdClear(void);
extern void lcdGoTo(uchar line, uchar col);

extern void lcdWriteStrProgMem(const uchar* str);

#endif // _LCDWH1602B_H_