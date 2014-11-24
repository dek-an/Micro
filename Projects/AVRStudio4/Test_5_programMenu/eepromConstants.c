#include "eepromConstants.h"

static uint08 m_fanOnTemp EEMEM = 27;
static uint08 m_fanOffTemp EEMEM = 26;

uint08 getFanOnTemperature(void)
{
	return eeprom_read_byte(&m_fanOnTemp);
}

void setFanOnTemperature(const uint08 temp)
{
	eeprom_update_byte(&m_fanOnTemp, temp);
}

uint08 getFanOffTemperature(void)
{
	return eeprom_read_byte(&m_fanOffTemp);
}

void setFanOffTemperature(const uint08 temp)
{
	eeprom_update_byte(&m_fanOffTemp, temp);
}