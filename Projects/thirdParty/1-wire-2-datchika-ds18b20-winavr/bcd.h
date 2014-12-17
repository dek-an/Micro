//***************************************************************************
//
//  File........: bcd.h
//
//  Author(s)...: Pashgan ChipEnable.Ru
//
//  Target(s)...: ��� ������ ����������������
//
//  Compiler....: �������� ��� ������ ����������� 
//
//  Description.: ���������� ��� �������� �������� ����� � ������� � ������ �� �� ��� 
//
//  Revisions...: 
//
//  18.10.2009 - 1.0 - ������                                       - Pashgan.
//
//***************************************************************************
#ifndef BCD_H
#define BCD_H

//����� ���������� ���� ���� ��� ���
#include "lcd_lib.h"

//����� �������������� ������� �� ������ ������� �� �����
#define LcdSendData(data) LCD_WriteData(data)

//���� ����������������, �� ���� � ������� �������� ���������� �� �����
//#define MIRROR_NULL_


/**************************************************************************
*   Function name :   BCD_1Lcd
*   Returns :         ���       
*   Parameters :      �������� ����� �� 0 �� 9    
*   Purpose :         ��������� ����� � ������ � ������� �� ���        
****************************************************************************/
void BCD_1Lcd(unsigned char value);

/**************************************************************************
*   Function name :   BCD_2Lcd
*   Returns :         ���       
*   Parameters :      �������� ����� �� 0 �� 99   
*   Purpose :         ��������� ����� � ������ � ������� �� ���        
****************************************************************************/
void BCD_2Lcd(unsigned char value);

/**************************************************************************
*   Function name :   BCD_3Lcd
*   Returns :         ���       
*   Parameters :      �������� ����� �� 0 �� 255    
*   Purpose :         ��������� ����� � ������ � ������� �� ���        
****************************************************************************/
void BCD_3Lcd(unsigned char value);

/**************************************************************************
*   Function name :   BCD_3IntLcd
*   Returns :         ���       
*   Parameters :      �������� ����� �� 0 �� 999    
*   Purpose :         ��������� ����� � ������ � ������� �� ���        
****************************************************************************/
void BCD_3IntLcd(unsigned int value);

/**************************************************************************
*   Function name :   BCD_4IntLcd
*   Returns :         ���       
*   Parameters :      �������� ����� �� 0 �� 9999    
*   Purpose :         ��������� ����� � ������ � ������� �� ���        
****************************************************************************/
void BCD_4IntLcd(unsigned int value);

#endif //BCD_H

