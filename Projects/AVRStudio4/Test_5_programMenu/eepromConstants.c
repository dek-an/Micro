#include "eepromConstants.h"

// //////////////////////////////////////////////////////////
// Fan Temperature
//
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

// //////////////////////////////////////////////////////////
// Light Time
//
static uint32 m_lightOnTime EEMEM = 28800; // 8:00:00
static uint32 m_lightOffTime EEMEM = 72000; // 20:00:00

uint32 getLightOnTime(void)
{
	return eeprom_read_dword(&m_lightOnTime);
}

void setLightOnTime(const uint32 time)
{
	eeprom_update_dword(&m_lightOnTime, time);
}

uint32 getLightOffTime(void)
{
	return eeprom_read_dword(&m_lightOffTime);
}

void setLightOffTime(const uint32 time)
{
	eeprom_update_dword(&m_lightOffTime, time);
}
