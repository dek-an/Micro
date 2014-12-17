//***************************************************************************
//
//  Author(s)...: Pashgan    http://ChipEnable.Ru   
//
//  Target(s)...: ATMega8535
//
//  Compiler....: GCC
//
//  Description.: ������ � ����� �������������� ��������� DS18B20, ������������
//                �� ����� ����. ������� ������ ������ 1Wire ���������.
//
//  Data........: 9.10.10 
//
//**************************************************************************
#include <avr/io.h>
#include <util/delay.h>
#include "lcd_lib.h"
#include "bcd.h"

#include "OWIPolled.h"
#include "OWIHighLevelFunctions.h"
#include "OWIBitFunctions.h"
#include "common_files\OWIcrc.h"

//��� ��������� � ���� ������ ������� DS18B20
#define DS18B20_FAMILY_ID                0x28 
#define DS18B20_CONVERT_T                0x44
#define DS18B20_READ_SCRATCHPAD          0xbe
#define DS18B20_WRITE_SCRATCHPAD         0x4e
#define DS18B20_COPY_SCRATCHPAD          0x48
#define DS18B20_RECALL_E                 0xb8
#define DS18B20_READ_POWER_SUPPLY        0xb4

//�����, � �������� ���������� 1Wire ����������
#define BUS   OWI_PIN_7

//��������� �������
unsigned char DS18B20_ReadTemperature(unsigned char bus, unsigned char * id, unsigned int* temperature);
void DS18B20_PrintTemperature(unsigned int temperature);

//���������� ��������� �� ���� 1Wire
#define MAX_DEVICES       0x02

//���� ������ ��� ������� ������ �����������
#define READ_SUCCESSFUL   0x00
#define READ_CRC_ERROR    0x01

#define SEARCH_SENSORS 0x00
#define SENSORS_FOUND 0xff

OWI_device allDevices[MAX_DEVICES];
unsigned char rom[8];

int main( void )
{
  unsigned int temperature = 0;
  unsigned char searchFlag = SEARCH_SENSORS;
  unsigned char crcFlag = 0;
  unsigned char num = 0;
  
  
  /*�������������*/
  LCD_Init();
  LCD_Clear();
  LCD_Goto(3,0);
  LCD_SendString("Sensors");
  LCD_Goto(14,0);
  LCD_SendString("N");
  OWI_Init(BUS);
  
  while(1){
    /*-���� ������� - ��������� ����� 1Wire ���������
    -���� ���������� �������� �������� ��������� �
    ������������ ��������� - ������������� ����, 
    ����� ������� ������ ������ �� �����������
    -���������� ���������� ��������� ���������*/
    if (searchFlag == SEARCH_SENSORS){
      num = 0;
      crcFlag = OWI_SearchDevices(allDevices, MAX_DEVICES, BUS, &num);
      LCD_Goto(14,1);
      BCD_1Lcd(num);
      if ((num == MAX_DEVICES)&&(crcFlag != SEARCH_CRC_ERROR)){
         searchFlag = SENSORS_FOUND;  
      }
    }
    
    /*������� ����������� ������� ���������� �������
    ���� ���������� ������ CRC, ������� ������ "not"
    � �������� ����, ����� ������� ������ ����� ���������
    ����� ����*/
    LCD_Goto(0,1);
    crcFlag = DS18B20_ReadTemperature(BUS, allDevices[0].id, &temperature);
    if (crcFlag != READ_CRC_ERROR){ 
      DS18B20_PrintTemperature(temperature);
    }
    else{
      LCD_SendString("  not  ");
      searchFlag = SEARCH_SENSORS;
    }
    
    /*������� ����������� ������� ���������� �������
    ���� ���������� ������ CRC, ������� ������ "not"
    � �������� ����, ����� ������� ������ ����� ���������
    ����� ����*/
    LCD_Goto(7,1);     
    crcFlag = DS18B20_ReadTemperature(BUS, allDevices[1].id, &temperature);
    if (crcFlag != READ_CRC_ERROR){ 
      DS18B20_PrintTemperature(temperature);
    }
    else{
      LCD_SendString("  not  ");
      searchFlag = SEARCH_SENSORS;
    }
    
  };
  
  return 0;
}


/*****************************************************************************
*   Function name :   DS18B20_ReadTemperature
*   Returns :       ���� - READ_CRC_ERROR, ���� ��������� ������ �� ������ ��������
*                          READ_SUCCESSFUL, ���� ������ ������ ��������    
*   Parameters :    bus - ����� ����������������, ������� ��������� ���� 1WIRE ����
*                   *id - ��� ������� �� 8-�� ���������, � ������� ��������
*                         ����� ������� DS18B20
*                   *temperature - ��������� �� ����������� ��������� ����������
*                                � ������� ����� ��������� ���������� ��. �����������
*   Purpose :      �������� ������ DS18B20, ���� ������� �� �������������� �����������
*                  ����, ��������� ��� ������ - scratchpad, ��������� CRC,
*                  ��������� �������� ����������� � ����������, ���������� ��� ������             
*****************************************************************************/
unsigned char DS18B20_ReadTemperature(unsigned char bus, unsigned char * id, unsigned int* temperature)
{
    unsigned char scratchpad[9];
    unsigned char i;
  
    /*������ ������ ������
    ������� ��� ��������� 1-Wire ���������� �� ����
    ������ ������� - ����� �������������� */
    OWI_DetectPresence(bus);
    OWI_MatchRom(id, bus);
    OWI_SendByte(DS18B20_CONVERT_T ,bus);

    /*����, ����� ������ �������� ��������������*/ 
    while (!OWI_ReadBit(bus));

    /*������ ������ ������
    ������� ��� ��������� 1-Wire ���������� �� ����
    ������� - ������ ���������� ������
    ����� ��������� ���������� ������ ������� � ������
    */
    OWI_DetectPresence(bus);
    OWI_MatchRom(id, bus);
    OWI_SendByte(DS18B20_READ_SCRATCHPAD, bus);
    for (i = 0; i<=8; i++){
      scratchpad[i] = OWI_ReceiveByte(bus);
    }
    
    if(OWI_CheckScratchPadCRC(scratchpad) != OWI_CRC_OK){
      return READ_CRC_ERROR;
    }
    
    *temperature = (unsigned int)scratchpad[0];
    *temperature |= ((unsigned int)scratchpad[1] << 8);
    
    return READ_SUCCESSFUL;
}

/*****************************************************************************
*   Function name :  DS18B20_PrintTemperature 
*   Returns :         ���       
*   Parameters :     temperature - ����������� ������� DS18B20     
*   Purpose :        ������� �������� ����������� ������� DS18B20
*                    �� LCD. ����� ���������� ����� ���������� �������.
*****************************************************************************/
void DS18B20_PrintTemperature(unsigned int temperature)
{
  unsigned char tmp = 0;
  /*������� ���� �����������
  *���� ��� ������������� 
  *������ ��������������*/  
  if ((temperature & 0x8000) == 0){
    LCD_WriteData('+');
  }
  else{
    LCD_WriteData('-');
    temperature = ~temperature + 1;
  }
        
  //������� �������� ����� ����. �����������      
  tmp = (unsigned char)(temperature>>4);
  if (tmp<100){
    BCD_2Lcd(tmp);
  }
  else{
    BCD_3Lcd(tmp);    
  }
        
  //������� ������� ����� ����. �����������
  tmp = (unsigned char)(temperature&15);
  tmp = (tmp>>1) + (tmp>>3);
  LCD_WriteData('.');
  BCD_1Lcd(tmp);
}