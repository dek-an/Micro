//***************************************************************************
//
//  Author(s)...: Pashgan    http://ChipEnable.Ru   
//
//  Target(s)...: ATMega8535
//
//  Compiler....: GCC
//
//  Description.: Работа с двумя температурными датчиками DS18B20, находящимися
//                на одной шине. Функция поиска адреса 1Wire устройств.
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

//код семейства и коды команд датчика DS18B20
#define DS18B20_FAMILY_ID                0x28 
#define DS18B20_CONVERT_T                0x44
#define DS18B20_READ_SCRATCHPAD          0xbe
#define DS18B20_WRITE_SCRATCHPAD         0x4e
#define DS18B20_COPY_SCRATCHPAD          0x48
#define DS18B20_RECALL_E                 0xb8
#define DS18B20_READ_POWER_SUPPLY        0xb4

//вывод, к которому подключены 1Wire устройства
#define BUS   OWI_PIN_7

//прототипы функций
unsigned char DS18B20_ReadTemperature(unsigned char bus, unsigned char * id, unsigned int* temperature);
void DS18B20_PrintTemperature(unsigned int temperature);

//количество устройств на шине 1Wire
#define MAX_DEVICES       0x02

//коды ошибок для функции чтения температуры
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
  
  
  /*инициализация*/
  LCD_Init();
  LCD_Clear();
  LCD_Goto(3,0);
  LCD_SendString("Sensors");
  LCD_Goto(14,0);
  LCD_SendString("N");
  OWI_Init(BUS);
  
  while(1){
    /*-флаг сброшен - выполнить поиск 1Wire устройств
    -если количество заданных устройсв совпадает с
    колличеством найденных - устанавливаем флаг, 
    чтобы функция поиска больше не запускалась
    -отобразить количество найденных устройств*/
    if (searchFlag == SEARCH_SENSORS){
      num = 0;
      crcFlag = OWI_SearchDevices(allDevices, MAX_DEVICES, BUS, &num);
      LCD_Goto(14,1);
      BCD_1Lcd(num);
      if ((num == MAX_DEVICES)&&(crcFlag != SEARCH_CRC_ERROR)){
         searchFlag = SENSORS_FOUND;  
      }
    }
    
    /*считать температуру первого найденного датчика
    если возвращена ошибка CRC, вывести строку "not"
    и сбросить флаг, чтобы функция поиска снова произвела
    опрос шины*/
    LCD_Goto(0,1);
    crcFlag = DS18B20_ReadTemperature(BUS, allDevices[0].id, &temperature);
    if (crcFlag != READ_CRC_ERROR){ 
      DS18B20_PrintTemperature(temperature);
    }
    else{
      LCD_SendString("  not  ");
      searchFlag = SEARCH_SENSORS;
    }
    
    /*считать температуру второго найденного датчика
    если возвращена ошибка CRC, вывести строку "not"
    и сбросить флаг, чтобы функция поиска снова произвела
    опрос шины*/
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
*   Returns :       коды - READ_CRC_ERROR, если считанные данные не прошли проверку
*                          READ_SUCCESSFUL, если данные прошли проверку    
*   Parameters :    bus - вывод микроконтроллера, который выполняет роль 1WIRE шины
*                   *id - имя массива из 8-ми элементов, в котором хранится
*                         адрес датчика DS18B20
*                   *temperature - указатель на шестнадцати разрядную переменную
*                                в которой будет сохранено считанного зн. температуры
*   Purpose :      Адресует датчик DS18B20, дает команду на преобразование температуры
*                  ждет, считывает его память - scratchpad, проверяет CRC,
*                  сохраняет значение температуры в переменной, возвращает код ошибки             
*****************************************************************************/
unsigned char DS18B20_ReadTemperature(unsigned char bus, unsigned char * id, unsigned int* temperature)
{
    unsigned char scratchpad[9];
    unsigned char i;
  
    /*подаем сигнал сброса
    команду для адресации 1-Wire устройства на шине
    подаем команду - запук преобразования */
    OWI_DetectPresence(bus);
    OWI_MatchRom(id, bus);
    OWI_SendByte(DS18B20_CONVERT_T ,bus);

    /*ждем, когда датчик завершит преобразование*/ 
    while (!OWI_ReadBit(bus));

    /*подаем сигнал сброса
    команду для адресации 1-Wire устройства на шине
    команду - чтение внутренней памяти
    затем считываем внутреннюю память датчика в массив
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
*   Returns :         нет       
*   Parameters :     temperature - температура датчика DS18B20     
*   Purpose :        Выводит значение температуры датчика DS18B20
*                    на LCD. Адрес знакоместа нужно выставлять заранее.
*****************************************************************************/
void DS18B20_PrintTemperature(unsigned int temperature)
{
  unsigned char tmp = 0;
  /*выводим знак температуры
  *если она отрицательная 
  *делаем преобразование*/  
  if ((temperature & 0x8000) == 0){
    LCD_WriteData('+');
  }
  else{
    LCD_WriteData('-');
    temperature = ~temperature + 1;
  }
        
  //выводим значение целое знач. температуры      
  tmp = (unsigned char)(temperature>>4);
  if (tmp<100){
    BCD_2Lcd(tmp);
  }
  else{
    BCD_3Lcd(tmp);    
  }
        
  //выводим дробную часть знач. температуры
  tmp = (unsigned char)(temperature&15);
  tmp = (tmp>>1) + (tmp>>3);
  LCD_WriteData('.');
  BCD_1Lcd(tmp);
}