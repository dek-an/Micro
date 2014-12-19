#ifndef _D18b20_H_
#define _D18b20_H_

#include <common/commonHeader.h>

#include "ds18b20Def.h"

extern void initDs18b20(void);
extern uchar ds18b20ReadTemperature(const uchar id, float* temperature);

#endif // _D18b20_H_
