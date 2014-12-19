#ifndef _DS18b20DEF_H_
#define _DS18b20DEF_H_

#define DS18B20_BUS OWI_PIN_7
//���������� ��������� �� ���� 1Wire
#define DS18B20_MAX_DEVICES 2

//���� ������ ��� ������� ������ �����������
#define DS18B20_READ_SUCCESSFUL   0x00
#define DS18B20_READ_CRC_ERROR    0x01

#endif // _DS18b20DEF_H_
