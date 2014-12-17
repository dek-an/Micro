#include "ds18b20.h"

#include <thirdParty/oneWire/OWIPolled.h>
#include <thirdParty/oneWire/OWIHighLevelFunctions.h>
#include <thirdParty/oneWire/OWIBitFunctions.h>

//код семейства и коды команд датчика DS18B20
#define DS18B20_FAMILY_ID                0x28 
#define DS18B20_CONVERT_T                0x44
#define DS18B20_READ_SCRATCHPAD          0xbe
#define DS18B20_WRITE_SCRATCHPAD         0x4e
#define DS18B20_COPY_SCRATCHPAD          0x48
#define DS18B20_RECALL_E                 0xb8
#define DS18B20_READ_POWER_SUPPLY        0xb4

//вывод, к которому подключены 1Wire устройства
#define BUS DS18B20_BUS

static OWI_device m_allDevices[DS18B20_MAX_DEVICES];
static uchar rom[8];

static uchar m_flag = 0;

// All devices found
DECLARE_FLAG_BIT(m_flag, ALL_DEVICES_FOUND, 0)

void initDs18b20(void)
{
	INITIALIZE_CHECKING();

	OWI_Init(BUS);
}

uchar ds18b20ReadTemperature(uchar * id, uint32* temperature)
{
	static uchar devicesNum = 0;
	static uchar crcFlag = 0;

	// -флаг сброшен - выполнить поиск 1Wire устройств
	// -если количество заданных устройсв совпадает с
	// колличеством найденных - устанавливаем флаг, 
	// чтобы функция поиска больше не запускалась
	if (!ALL_DEVICES_FOUND())
	{
		devicesNum = 0;
		crcFlag = OWI_SearchDevices(m_allDevices, DS18B20_MAX_DEVICES, BUS, &devicesNum);
		if (devicesNum == DS18B20_MAX_DEVICES && crcFlag != SEARCH_CRC_ERROR)
		{
			ALL_DEVICES_FOUND_ON();
		}
	}

	
}
