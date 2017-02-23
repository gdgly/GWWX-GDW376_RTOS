
#ifndef __BSP_CFG_H
#define __BSP_CFG_H

#include "stm32f10x.h"
#include "USERMATH.h"
#include "string.h"
#include "Function_CFG.h"
#include "I2C_FRAM.h"  //20131025  

#define false   0x00
#define true    0x01
#define nosend  0x00
#define right   0x01
#define error   0x02
#define none    0x03
#define ignore  0x04
#define back    0x55
#define ahead   0xaa
#define same    0x00
#define idle    0x00    //;����
#define busy    0x10
#define OFF     0x00    //;��
#define ON      0x01    //;��

#define _220V   0x00
#define _48V    0x01


typedef const uint32_t B4;  /*!< Read Only */
typedef const uint16_t B2;  /*!< Read Only */
typedef const uint8_t  B1;   /*!< Read Only */

struct Time_Type  //;����ʱ������ݽṹ
{
    u8 Sec;
    u8 Min;
    u8 Hour;
    u8 Day;
    u8 Month;
    u8 Year;
};

struct Buf_Type   //;���еĻ������ķ���
{
    u8   Start[1];
    u8   I2C_1[16];
    u8   I2C_2[16];
    
    char Tx_SB[32];

    char Tx5_1[U5BUFSIZE];  //;����5�Ļ�����
    char Rx5_2[U5BUFSIZE];
    char Rx5_1[U5BUFSIZE];


    char Tx2_1[U2TXBUFSIZE];  //;����2�Ļ�����
    char Rx2_1[U2RXBUFSIZE];
    char Rx2_2[U2RXBUFSIZE];	
    
    char Tx1_1[U1BUFSIZE];  //;����1�Ļ�����
    char Rx1_2[U1BUFSIZE];
    char Rx1_1[U1BUFSIZE];
    
    char Tx3_1[U3BUFSIZE];  //;����3�Ļ�����
    char Rx3_2[U3BUFSIZE];
    char Rx3_1[U3BUFSIZE];

    char Tx4_1[U4BUFSIZE];  //;����4�Ļ�����
    char Rx4_1[U4BUFSIZE];
    char Rx4_2[U4BUFSIZE];

    u8   End[1];

    u8   St_Spi[1];
    u8   Spi1[528];
    u8   En_Spi[1];
};


struct USARTDataStream  //;�����������Ŀ���
{
    u8  Mark;
    u16 RIndex;    //;�������к�
    u16 TIndex;    //;�������к�
    u16 TLong;     //;���ͳ���
    u8  TBusy;     //;����æµ
    u8  RecRun;    //;�Ƿ��ڽ���
    u8  RBet2Byte; //;���������ֽڵ�ʱ����
    u8  RecOrd;    //;�����յ�����֡
    u16 RecLen;    //;���ճ���
    u8  SedOrd;    //;�����ڷ���֡
    u8  DataLightMs;
    char  *BufAdr;   //;��������ջ����ָ��
};


struct HardWareControl  //;����Ӳ������
{
    u8 GIn1;   //;GIn1��״̬
    u8 GIn2;   //;GIn2��״̬  //;���ڱ�ʾ��ǰģ�����ɵ�ع��绹�ǵ�Դ����
    u8 DCD;    //;DCD��״̬
    u8 RI;     //;RI��״̬
    u8 GIn2_Del;   //;GIn2����Ϊ�ߵ�ƽ��ʱ���ʱ	
    u8 DCD_Del;    //;DCD��״̬Ϊ�ߵ�ʱ��
    u8 EventFlag;
    u8 EventRead;
    u8 DCD_Flag;
    u8 EnRes[2]; //;��������
    u8 FactoryInit; //;������ʶby zrt 20130816
};

struct SafeWord //;��ȫ��֤��
{
    u8 Para[2];
    u16 SumT1;
    u16 SumT2;
    u16 SumT3;
    u16 SumT4;
    u16 SumT5;
    u16 SumT6;
    u16 SumT7;
    u16 SumT8;
};

struct EnableWordStr //;����״̬��
{
    u8 Start[1];
    u8 CSB1;
    u8 CSB2;
    u8 CSB3;
    u16 CSB4;
    u8 CSB5;
    u8 CSB6;
    u8 CSB7;
    u8 CSB8;
    u8 CSB9;
    u8 CSB10;
    u8 ZZBock;
    u8 W16;
    u8 R16;
    u8 WFM;
    u8 RFM;
    u8 TaskAUL_P;
    u8 TaskAUL_F;
    u8 TaskSamp;
    u8 MoveByte;
    u8 RFlashNByte;
    u8 TimeSet;
    u8 End[1];
};

struct FluxStat
{
    u32 Day;
    u32 Month;
};

struct  BattyCharge
{
    u16 BT1, BT2;
    u8 Station;
    u8 Switch;
    u16 Time[3];
};

extern struct SafeWord SW;
extern struct EnableWordStr EW;
extern u8 RTC_FlagMs, Num025s_WT, RTC_FlagSec, RTC_FlagMin, RTC_ms, RTC_sec, RTC_min;
extern u8 Enable_Save;//;Ϊ  I2C.c�ļ�����ı���
extern u8 OldStatus_GprsC,OldSign_GprsC;  //;GPRS��֮ǰ��״̬
extern u8 Power_Modem;
extern struct HardWareControl  HWareC; //;����Ӳ������
extern struct USARTDataStream  U5DS, U1DS, U4DS, U2DS, U3DS;  //;Uart1��Uart2 ���������Ŀ��Ʋ���
extern struct ModemContrl  GprsC, GprsCErr;
extern struct ModemLinkCtrl GprsLC;
extern u8 Yester_Day,Yester_Month,Yester_Year, Old_Sec, Old_Min,Week;
extern struct Time_Type TBCD,THEX;  //;BCD���ʽ��HEX��ʽ������ʱ��
extern struct Buf_Type AllBuf;  //;������
extern u16 ReadFlash_Enable;
extern u16 ADCConvertedValue[2];
extern struct BattyCharge  BattyC; //;��س�����
extern u8 NUMPOWER;

/*----------------------------------------------------------------------------*
 | I2C_FM24_8025.c
 *----------------------------------------------------------------------------*/
//;extern void __I2C_FM24_8025_c(void){}
//extern u8 WriteM16_I2C1(u8 n, u16 adr, u8* adr_ram );
extern u8 WriteNByte_I2C1(u8 n, u16 adr, u8* adr_ram );
//extern u8 ReadM16_I2C1(u8 n, u16 adr, u8* adr_ram );
extern u8 ReadNByte_I2C1(u8 n, u16 adr, u8* adr_ram );
extern void SaveData_FM24(u8 n, u16 EEAdr1, u16 EEAdr2, u8* adr_ram);
extern void ReadData_FM24(u8 n, u16 EEAdr1, u16 EEAdr2, u8* adr_ram);
//extern void EPSON8025_SAVE(u8 Addr, u8 Data);
//extern void EPSON8025_READ(u8 *Addr);

/*----------------------------------------------------------------------------*
 |  MODEN_M590.c
 *----------------------------------------------------------------------------*/
//;extern void __MODEN_M590_c(void){}
#define  _Modem_ON            0x00 //;GPRSģ����ض���
#define  _Modem_OFF           0x01
#define  _ModemPowerOn        GPIO_SetBits(GPIOB, GPIO_Pin_12)       //;��ģ�����õ�Դ
#define  _ModemPowerOff       GPIO_ResetBits(GPIOB, GPIO_Pin_12)   //;�ر�ģ�����õ�Դ

#define  _ModemONOFF_High     GPIO_ResetBits(GPIOA, GPIO_Pin_8)      //;G-ON-OFF����
#define  _ModemONOFF_Low      GPIO_SetBits(GPIOA, GPIO_Pin_8)       //;G-ON-OFF����

#define  _GPRS_Modem_Run      GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)  //;���GPRSģ������
                    
#define  LOGONLIGHTON        GPIO_ResetBits(GPIOB, GPIO_Pin_8)  //;�������
#define  LOGONLIGHTOFF       GPIO_SetBits(GPIOB, GPIO_Pin_8)    //;�ر������   

#define  COMLIGHTON()         GPIO_ResetBits(GPIOB, GPIO_Pin_0)  //;ͨ������ָʾ
#define  COMLIGHTOFF()        GPIO_SetBits(GPIOB, GPIO_Pin_0)

#define  TCPDATALIGHTON()    GPIO_ResetBits(GPIOB, GPIO_Pin_9)  //;�������ݵƿ�
#define  TCPDATALIGHTOFF()   GPIO_SetBits(GPIOB, GPIO_Pin_9) 

#define  METERDATALIGHTON()    GPIO_ResetBits(GPIOB, GPIO_Pin_1)  //;�������ݵƿ�
#define  METERDATALIGHTOFF()   GPIO_SetBits(GPIOB, GPIO_Pin_1)  

#define  RS485CTRLRCV()         GPIO_SetBits(GPIOA, GPIO_Pin_4)      
#define  RS485CTRLSED()         GPIO_ResetBits(GPIOA, GPIO_Pin_4)  

#define  EVENTCHECK()      GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)  //;����¼�

//;#define  RS485CTRLRCV()         GPIO_ResetBits(GPIOA, GPIO_Pin_4)      
//;#define  RS485CTRLSED()         GPIO_SetBits(GPIOA, GPIO_Pin_4)           

/*GprsC.Status�����·��������ģ��״̬*/
#define  _NoCard     0x01   //;��SIM�� 
#define  _FindHost   0x02   //;����Ѱ����վ
#define  _ContHost   0x04   //;����վ���ӳɹ�
#define  _FindHFalst 0x08   //;Ѱ����վʧ��

/*GprsC.State*/
#define  _SetUp      0x01   //; ģ���Ѿ�����
#define  _Link       0x02   //; ģ�����ڷ�������
#define  _Connect    0x04   //; ģ���Ѿ�������վ
#define  _HadLogin   0x08   //; ģ���Ѿ���½��վ
#define  _Redial     0x10   //; ģ�����ڵȴ��ز� 

/*GprsC.ATStat*/         
#define  LINK0       0x00
#define  LINK1       0x01


struct ModemContrl       //;GPRSģ��Ŀ���
{
    u8   State;            //;��վ��״̬
    u8   Status;           //;ģ���״̬
    u8   State1;           //;��·1���ӵ�״̬
    u8   ReConnect;
    u8   Re_Connect_states;//;׼����������״̬(ֻ��Զ�̸���IP��ִ��)
    u8   TCPFail;      
    u8   ATStatus;         //;��ǰ���ڷ���AT�����״̬    
    u8   ATLen;            //;��Ҫ���͵�ATָ��ĳ���
    u8   ATSOVT;           //;AT״̬��ʱ����
    u8   SendAT_Wait1s;
    u8   SendAT_Step;      //;�ȴ���GPRSģ�鷢��ATָ��Ĳ���
    u8   ATAns_Wait1s;     //;�ȴ�ATָ��Ӧ����������
    u8   ReSendAT_Num;     //;ATָ���ط��Ĵ���
    u8   ReCon_Num;        //;�ز�����
    u16  MinReCon;         //;��վ���²��ŵ�ʱ����
    u8   CPINErrNum;
    u8   CSQErrNum;
    u8   CREGErrNum;
    u8   XIICErrNum;
    u8   CMGSNum;
    u8   Sign;             //;�źŴ�С
    u8   *IP;
    u16  Port;
    char *APN;
    u8   LinkState;
    u8   LoginIn;          //;�Ƿ�������վ���͵�½����
    u8   LoginTimeOut;     //;���͵�½���������ʱʱ��
    u8   LoginNumSend;
    u8   LoginOut;
    u8   HBStatus;         //;��������״̬
    u8   HBNumSend;        //;�����ط�����
    u16  HBTime;           //;��������ʱ
    u16  HBOVT;            //;������ʱ
    u8   TCPRec;             
    u16  TCPLen;
    char *TCPBuf;
    u16  SMSLen;
    u8   SMSPer;
    u8   SignR;         //;����    
    
    u8   NDataMin1;
    u16  NDataMin0;
    u16  NLogonMin;  
    u8   RailSign;
    u8   EnRes[2];         //;�Ƿ������ģ����и�λ
    char ATStr[50];        //;��Ҫ���͵�ATָ��
      
    u8 DataLightMs;
    
};

struct ModemLinkCtrl  //;GPRS������ز���
{   
    u8   PassLinkOrd;
    s8   PassLinkNum;
    u8   Link2Switch;  
    u8   CHKLink;    //;����Ƿ���������
    u8   LinkEnable; //;�������߱�־
    u8   IP1[21]; //;������վIP
    u8   IP1_long; //;����IP�˿ںų���
};

#ifdef ZS
extern void RecordErr(u8 type);
#endif

extern void Modem_GDCDLightCtr(void);
extern  void Modem_PowON(void);
extern  void Modem_PowOFF(void);
extern  void Modem_PowBKON(void);
extern  void Modem_PowBKOFF(void);
extern  void Modem_GDCDON(void);
extern  void Modem_GDCDOFF(void);
extern  void Modem_ON(void);
extern  void Modem_ON_CHK(void);
extern  u8   Modem_OFF_CHK(void);
extern  void Modem_OFF(void);
extern  void Modem_OFFON(void);
extern  void ATComdSend(char *str1);
extern  u8   Modem_TCP_Send(void);
extern  void ATComdSetDeal(u8 *IP,  u8 Port, char *APN);
extern  void ATComdSetNoRec(void);
extern  u8 Modem_SMS_Send(void);
extern  u8   SMS_DataMove(u8 Len, char *OutAddr, u16 *InAddr);
extern  u8   SMS_CodeData(char *Out_Buf, u8 *SMS_Code, u8 *Tel_Code);
extern  char SMS_HexToASC(u8 HEX);
extern  void Modem_Com(void);
extern void Modem_LinkTime(void);
extern u8 NumIpToChar(char *Buf, u8 *IP, u16 Port);
extern void Modem_AT_Send(char *str1);  //by zrt 20130912
/*----------------------------------------------------------------------------*
 |  Serial.c
 *----------------------------------------------------------------------------*/
extern void SendByte_USART1(void);
extern void SendFirstByte_USART1(void);
extern void ReceiveByte_USART1(void);

extern void SendFirstByte_USART2(void);
extern void SendByte_USART2(void);
extern void ReceiveByte_USART2(void);

extern void SendByte_USART3(void);
extern void SendFirstByte_USART3(void);
extern void ReceiveByte_USART3(void);

extern void SendByte_UART4(void);
extern void SendFirstByte_UART4(void);
extern void ReceiveByte_UART4(void);

extern void SendByte_UART5(void);
extern void SendFirstByte_UART5(void);
extern void ReceiveByte_UART5(void);

extern void USART_Com(void);


/*----------------------------------------------------------------------------*
 |  Setup.c
 *----------------------------------------------------------------------------*/
//;extern void __Setup_c(void){}
extern void InitHardware (void);
extern void IWDG_Configuration(void);
extern void Batty_Charge(void);
extern void Watchdog( void );
extern void RTC_Time(void);
extern void Delay_nms(u16 n);
extern void USART2_BaudRateSet(u32 BaudRate);
extern void UARTx_BaudRate_Set(u8 BPS, u8 UARTx);

/*----------------------------------------------------------------------------*
 |  spi_flash.c
 *----------------------------------------------------------------------------*/
//;extern void __spi_flash_c(void){}
extern void SPI_FLASH_Init(void);
extern void SPI_FLASH_PageErase(u32 page);
extern void SPI_FLASH_PageRead(u8* pBuffer, u16 page );
extern void SPI_FLASH_PageWrite_E(u8* pBuffer, u16 page );
extern void SPI_FLASH_PageWrite_NE(u8* pBuffer, u16 page );
//;extern u8   SPI_FLASH_ReadByte(void);
//;extern u8   SPI_FLASH_SendByte(u8 byte);
//;extern void SPI_FLASH_WriteEnable(u8 M);
extern void SPI_FLASH_ReadProtReg(u8* pBuffer);
//;extern u8   SPI_FLASH_ReadStatus(u8 M);
//;extern u8   SPI_FLASH_CheckProtReg(u8 M);
extern void SPI_FLASH_EraseProtReg(u8 M);
//;extern void SPI_FLASH_WriteDisable(u8 M);
extern void SPI_FLASH_ReadNByte(u8* pBuffer, u16 page,u16 adr_p,u16 n );


#endif //;__BSP_CFG_H




