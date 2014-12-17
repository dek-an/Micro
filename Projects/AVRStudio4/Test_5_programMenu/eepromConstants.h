#ifndef _EEPROMCONSTANTS_H_
#define _EEPROMCONSTANTS_H_

#include <common/commonHeader.h>

extern void initEeprom(void);

DECLARE_EEPROM_VAR(uint08, FanOnTemperature)
DECLARE_EEPROM_VAR(uint08, FanOffTemperature)

DECLARE_EEPROM_VAR(uint32, LightOnTime)
DECLARE_EEPROM_VAR(uint32, LightOffTime)

#endif // _EEPROMCONSTANTS_H_
