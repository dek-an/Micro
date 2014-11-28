#ifndef _EEPROMCONSTANTS_H_
#define _EEPROMCONSTANTS_H_

#include <common/commonHeader.h>

extern uint08 getFanOnTemperature(void);
extern void setFanOnTemperature(const uint08 temp);
extern uint08 getFanOffTemperature(void);
extern void setFanOffTemperature(const uint08 temp);

extern uint32 getLightOnTime(void);
extern void setLightOnTime(const uint32 time);
extern uint32 getLightOffTime(void);
extern void setLightOffTime(const uint32 time);

#endif // _EEPROMCONSTANTS_H_
