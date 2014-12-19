#include "ds18b20.h"

#include <thirdParty/oneWire/OWIPolled.h>
#include <thirdParty/oneWire/OWIHighLevelFunctions.h>
#include <thirdParty/oneWire/OWIBitFunctions.h>
#include <thirdParty/oneWire/common_files/OWIcrc.h>

//��� ��������� � ���� ������ ������� DS18B20
//#define DS18B20_FAMILY_ID                0x28 
#define DS18B20_CONVERT_T                0x44
#define DS18B20_READ_SCRATCHPAD          0xbe
//#define DS18B20_WRITE_SCRATCHPAD         0x4e
//#define DS18B20_COPY_SCRATCHPAD          0x48
//#define DS18B20_RECALL_E                 0xb8
//#define DS18B20_READ_POWER_SUPPLY        0xb4

// //////////////////////////////////////////////////////////
// Helpers
//
static uchar DS18B20_ReadTemperature(uchar bus, uchar* id, uint* temper);
static float DS18B20_ConvertTemperature(uint temperature);

// //////////////////////////////////////////////////////////
// Members
//
static OWI_device m_allDevices[DS18B20_MAX_DEVICES];
static uchar m_devicesNum = 0;

// //////////////////////////////////////////////////////////
// Interface Implementation
//
void initDs18b20(void)
{
	INITIALIZE_CHECKING();

	OWI_Init(DS18B20_BUS);
}

uchar ds18b20ReadTemperature(const uchar id, float* temperature)
{
	// if not all devices found then find devices
	if (m_devicesNum != DS18B20_MAX_DEVICES)
	{
		m_devicesNum = 0;
		OWI_SearchDevices(m_allDevices, DS18B20_MAX_DEVICES, DS18B20_BUS, &m_devicesNum);
	}

	// if no devices found return error
	if (!m_devicesNum)
	{
		return DS18B20_READ_CRC_ERROR;
	}

	// read temperature for id device
	uint temper = 0;
	if (DS18B20_ReadTemperature(DS18B20_BUS, m_allDevices[id].id, &temper) == DS18B20_READ_SUCCESSFUL)
	{
		*temperature = DS18B20_ConvertTemperature(temper);
		return DS18B20_READ_SUCCESSFUL;
	}

	--m_devicesNum;
	return DS18B20_READ_CRC_ERROR;
}

// //////////////////////////////////////////////////////////
// Helpers Implementation
//
/*****************************************************************************
*	Function name :	DS18B20_ReadTemperature
*	Returns :		���� - DS18B20_READ_CRC_ERROR, ���� ��������� ������ �� ������ ��������
*					DS18B20_READ_SUCCESSFUL, ���� ������ ������ ��������
*	Parameters :	bus - ����� ����������������, ������� ��������� ���� 1WIRE ����
*					*id - ��� ������� �� 8-�� ���������, � ������� �������� ����� ������� DS18B20
*					*temperature - ��������� �� ����������� ��������� ����������
*									� ������� ����� ��������� ���������� ��. �����������
*	Purpose :		�������� ������ DS18B20, ���� ������� �� �������������� �����������
*					����, ��������� ��� ������ - scratchpad, ��������� CRC,
*					��������� �������� ����������� � ����������, ���������� ��� ������
*****************************************************************************/
static uchar DS18B20_ReadTemperature(uchar bus, uchar* id, uint* temperature)
{
	uchar scratchpad[9];

	// ������ ������ ������
	// ������� ��� ��������� 1-Wire ���������� �� ����
	// ������ ������� - ����� ��������������
	OWI_DetectPresence(bus);
	OWI_MatchRom(id, bus);
	OWI_SendByte(DS18B20_CONVERT_T ,bus);

	// ����, ����� ������ �������� ��������������
	for (; !OWI_ReadBit(bus); ) { /*do nothing*/}

	// ������ ������ ������
	// ������� ��� ��������� 1-Wire ���������� �� ����
	// ������� - ������ ���������� ������
	// ����� ��������� ���������� ������ ������� � ������
	OWI_DetectPresence(bus);
	OWI_MatchRom(id, bus);
	OWI_SendByte(DS18B20_READ_SCRATCHPAD, bus);
	for (uchar i = 0; i <= 8; ++i)
	{
		scratchpad[i] = OWI_ReceiveByte(bus);
	}

	if (OWI_CheckScratchPadCRC(scratchpad) != OWI_CRC_OK)
	{
		return DS18B20_READ_CRC_ERROR;
	}

	*temperature = (uint)scratchpad[0];
	*temperature |= ((uint)scratchpad[1] << 8);

	return DS18B20_READ_SUCCESSFUL;
}

/*****************************************************************************
*	Function name :	DS18B20_ConvertTemperature 
*	Returns :		���
*	Parameters :	temperature - ����������� ������� DS18B20     
*	Purpose :
*****************************************************************************/
float DS18B20_ConvertTemperature(uint temperature)
{
	signed char sign = 1;
	// if temperature is negative
	if (GBI(temperature, 15))
	{
		sign = -1;
		temperature = ~temperature + 1;
	}

	return (float)((temperature >> 1) + (temperature >> 3)) / 10 * sign;
}
