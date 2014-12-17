#include "eepromConstants.h"

DECLARE_EEPROM_VAR_METHODS(uint08, byte, FanOnTemperature, 27)
DECLARE_EEPROM_VAR_METHODS(uint08, byte, FanOffTemperature, 26)
DECLARE_EEPROM_VAR_METHODS(uint32, dword, LightOnTime, 28800) // 8:00:00
DECLARE_EEPROM_VAR_METHODS(uint32, dword, LightOffTime, 72000) // 20:00:00

void initEeprom(void)
{
	initLightOnTime();
	initLightOffTime();
	initFanOnTemperature();
	initFanOffTemperature();
}