#ifndef _D18b20_H_
#define _D18b20_H_

#include <common/commonHeader.h>

extern void initDs18b20(void);
extern uchar ds18b20ReadTemperature(uchar * id, uint32* temperature);

#endif // _D18b20_H_