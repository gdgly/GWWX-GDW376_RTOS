/*
********************************************************************************
*  �� �� ����
*
*  �ļ�������
*
*  ����оƬ��
*
*  �� �� �ˣ�023
*
*  �� �� �ţ�
*
*  �޸ļ�¼��
********************************************************************************
*/

#include "BSP_CFG.h"

struct SafeWord SW;
struct EnableWordStr EW;
u8 RTC_FlagMs, Num025s_WT, RTC_FlagSec, RTC_FlagMin, RTC_ms, RTC_sec, RTC_min;
u8 Yester_Day,Yester_Month,Yester_Year, Old_Sec, Old_Min, Week;  //;ǰһ�������
struct Time_Type TBCD, THEX;  //;BCD���ʽ��HEX��ʽ������ʱ��
u8 Enable_Save;//;ΪI2C.c�ļ�����ı���
u8 OldStatus_GprsC,OldSign_GprsC;  //;GPRS��֮ǰ��״̬
u8 Power_Modem;//;
struct HardWareControl   HWareC;     //;����Ӳ������
struct USARTDataStream   U1DS, U4DS, U5DS, U2DS, U3DS;   //;Uart1��Uart2 ���������Ŀ��Ʋ���
struct ModemContrl  GprsC, GprsCErr;
struct ModemLinkCtrl GprsLC;
struct Buf_Type AllBuf;  //;������
u16 ReadFlash_Enable;
u16 ADCConvertedValue[2];   
struct BattyCharge  BattyC; //;��س�����  
u8 NUMPOWER;
