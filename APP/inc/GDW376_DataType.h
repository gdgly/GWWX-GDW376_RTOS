/*
********************************************************************************
*  �� �� ����
*
*  �ļ������� GDW130��Լ�������ݽṹ 
*
*  ����оƬ��
*
*  �� �� �ˣ�
*
*  �� �� �ţ�
*
*  �޸ļ�¼��1.
              2.
              3.ZDPStr_F13��Լ֧����������·��Ϊ64����Ϊ4
              4.ZDPStr_F14_1��Լ֧��64�������㣬��Point_Op��Ϊ8����
              5.ZDPStr_F22��Լ֧��48�����ʣ���Ϊ14����
              6.ZDPStr_F33��Լ֧��32�������飬��Ϊ4��
********************************************************************************
*/

#ifndef __GDW130_DataType_h
#define __GDW130_DataType_h

#include "stm32f10x.h"

#define SBNUM 30

#define  METERNUM  1   //;ZDPStr_F10��Լ֧��2040����ܱ��ڴ�Ϊ1�� //;ģ��ֻ֧��1��������--20141226
#define  PULSENUM  4    //;ZDPStr_F11��Լ֧����������·��Ϊ64����Ϊ4
#define  SIMULNUM  4    //;ZDPStr_F13��Լ֧����������·��Ϊ64����Ϊ4
#define  GRPPTNUM  8    //;ZDPStr_F14_1��Լ֧��64�������㣬��Point_Op��Ϊ8����
#define  RATENUM   4    //;ZDPStr_F22��Լ֧��48�����ʣ���Ϊ4����
#define  NODENUM   4    //;ZDPStr_F31��Լ֧��20���ڵ㣬��Ϊ4����
#define  RMTNUM    4    //;ZDPStr_F33_1_1��Լ֧��24������ʱ�Σ���Ϊ4����
#define  PORTNUM   2    //;ZDPStr_F33_1��Լ֧��31���˿ںţ���Ϊ2����F34���á�
#define  KEYMETNUM 5    //;ZDPStr_F35��Լ֧��20���ص㻧����Ϊ5����
#define  USRSNUM   4    //;ZDPStr_F38��F39��Լ֧��16���û�С��ţ���Ϊ4����
#define  TASK1NUM   8    //;ZDPStr_F65���������
#define  TASK2NUM   8    //;ZDPStr_F66���������

//;6(F1)+33(F2)+28(F3)+16(F4)+3(F5)+16(F6)+64(F7)+8(F8)+16(F9)+272(F10) = 462Byte
#define  CS1NUM    462  
//;21(F11)+2(F12)+13(F13)+81(F14)+73(F15)+64(F16)+2(F17)+12(F18)+1(F19)+1(F20)
//;  +25(F21)+17(F22)+3(F23) = 315Byte
#define  CS2NUM    315  
//;61(F33)+13(F34)+11(F35)+4(F36)+19(F37)+138(F38)+138(F39)+3(F57)+1(F58)+4(F59)
//; +44(F60)+1(F61) = 413Byte
#define  CS3NUM    437  
//;11(F25)+57(F26)+24(F27)+4(F28)+12(F29)+1(F30)+25(F31)+48(F73)+10(F74)+16(F76)
//;  +1(F76) = 209Byte
#define  CS4NUM    209  
//;365(F65)1~4 = 356Byte
#define  CS5NUM    356  
//;356(F66)+8(F67)+8(F68) = 372Byte
#define  CS6NUM    372  
//;52(F41)+6(F42)+1(F43)+8(F44)+1(F45)+4(F46)+17(F47)+1(F48) = 90Byte
#define  CS7NUM_GRP    90  
//;4(F81)+4(F82)+1(F83) = 9Byte
#define  CS7NUM_DCP    9  
//;1(F149) = 1Byte
#define  CS7NUM_CTR    1  
//;365(F65)5~8 = 365Byte
#define  CS9NUM    356  
//;365(F66)5~8 = 365Byte
#define  CS10NUM    356  

#define  ZD      1
#define  POINT   2
#define  TASK    3
#define  GRP     4
#define  CTR     5
#define  DCP     6


struct ZDPStr_F1  //;�ն�ͨѶ���� 6B
{
    u8  RTS;       //;��������ʱʱ��
    u8  DELT;      //;��Ϊ����վ��ʱʱ��
    u8  OVT_RNum[2];  //;��ʱʱ����ط�����
    u8  CON;       //;��Ҫ��վȷ�ϵı�־
    u8  HBeat;     //;��������
};

struct ZDPStr_F2  //;�м�ת������ 33B
{
    u8 En_Num;  //;�����ת���ĵ�ַ��
    u8 Adr[16][2];  //;Ҫת���ĵ�ַ
};

struct ZDPStr_F3  //;��վIP��ַ�Ͷ˿� 28B
{
    u8  IP1[4];
    u8 Port1[2];
    u8  IP2[4];
    u8 Port2[2];   
    char APN[16];
};

struct ZDPStr_F4  //;��վ�绰����Ͷ������ĺ��� 16B
{
    u8 Tel_Code[8];
    u8 SMS_Code[8];
};

struct ZDPStr_F5  //;�ն���Ϣ��֤�������� 3B
{
    u8  Code;
    u8  Para[2];
};

struct ZDPStr_F6  //;�ն����ַ 16B
{
    u8 Addr1[2];
    u8 Addr2[2];
    u8 Addr3[2];
    u8 Addr4[2];
    u8 Addr5[2];
    u8 Addr6[2];
    u8 Addr7[2];
    u8 Addr8[2];
};

struct ZDPStr_F7  //;�ն�IP��ַ�Ͷ˿� 64B
{
    u8  ZDIP[4];
    u8  SubnetIP[4]; //;��������
    u8  GateIP[4];   //;����
    u8  DNSType;
    u8  DNSIP[4];    //;���������
    u8  DNSPort[2];     //;����������˿�
    u8  DNSLinkType; //;������������ӷ�ʽ
    u8  UsrLen;      //;�û�������
    u8  UsrName[20]; //;�û���
    u8  PasswordLen; //;���볤��
    u8  Password[20];
    u8  ZDPort[2];      //;�ն������˿�  
};

struct ZDPStr_F8  //;�ն�����ͨ�Ź�����ʽ 8B
{
    u8  WorkMode;   //;����ģʽ
    u8  ActRedailSec[2]; //;�������ߡ�ʱ������ģʽ�ز����
    u8  RedailNum; //;��������ģʽ�ز�����
    u8  NoComBreakMin;  //;��������ģʽ��ͨ���Զ�����ʱ��
    u8  AllowTimeF[3];    //;ʱ������ģʽ��������ʱ�α�־
};

struct ZDPStr_F9  //;�¼���¼���� 16B
{
    u8  Enable[8];
    u8  Rating[8];
};

struct ZDPStr_F10_1 //;27B
{
    u8  Index[2];
    u8  Point[2];
    u8  BPS_Port;
    u8  Type;
    u8  Addr[6];
    u8  PW[6];
    u8  Rate_Digit;
    u8  WEnergyPN; //;�й�����ʾֵ����λ��С��λ����
    u8  CJQComAddr[6]; //;�����ɼ���ͨ�ŵ�ַ
    u8  UsrN;          //;�û�����ż�С��ŷ��䡣
};

struct  ZDPStr_F10   //;�ն˵��ܱ���������װ�����ò��� 272B
{
    u8     MeterNum[2];
    struct ZDPStr_F10_1 Meter[METERNUM]; 
};

struct ZDPStr_F11_1  //;5B
{
    u8  PuPort;
    u8  Point;
    u8  PuType;
    u8  Imp[2];
};
struct  ZDPStr_F11    //;�ն��������� 21B
{
    u8 PuNum;
    struct ZDPStr_F11_1 Pulse[PULSENUM];
};

struct ZDPStr_F12    //;�ն�״̬������ 2B
{
    u8 ZTL_Flag;
    u8 ZTL_Type;
};
struct ZDPStr_F13_1  //;3B
{
    u8 Port;
    u8 Point;
    u8 Type;
};

struct ZDPStr_F13 //;�ն˵�ѹ������ģ�������ò��� 13B
{
    u8 SimulNum;
    struct ZDPStr_F13_1 Simul[SIMULNUM];
};

struct ZDPStr_F14_1 //;10B
{
    u8 Index;        //;�ܼ������
    u8 Point_Num;    //;��������
    u8 Point_Op[GRPPTNUM];  //;���㷨��
};

struct ZDPStr_F14  //;�ܼ������ò��� 81B
{
    u8 GRPNum;
    struct ZDPStr_F14_1 ADDGRP[8];
};

struct ZDPStr_F15_1 //;9B
{
    u8 Index;
    u8 QIndex;
    u8 qIndex;
    u8 Type;
    u8 Qr;
    u8 Qa[4];
};
struct ZDPStr_F15 //;�й��ܵ������Խ���¼��������� 73B
{
    u8 PEENum;
    struct ZDPStr_F15_1 PEEvent[8];
};

struct ZDPStr_F16  //;����ר���û��������� 64B
{
    u8 User[32];
    u8 PW[32];
};

struct ZDPStr_F22 //;�ն˵��������� 17B
{
    u8 RateN;
    u8 Rate[RATENUM][4];
};

struct PoPStr_F25 //;������������� 11B  
{
    u8 UMul[2];
    u8 IMul[2];
    u8 Un[2];
    u8 In;
    u8 KVAMax[3];
    u8 Type;
};

struct PoPStr_F26 //;��������ֵ���� 57B
{
    u8 UHigh[2];      //;��ѹ�ϸ����б����
    u8 ULow[2];
    u8 UBreak[2];
    u8 UHH[2];       //;��ѹ����
    u8 GY_DLY;
    u8 GY_Ratio[2];
    u8 ULL[2];       //;Ƿѹ����
    u8 QY_DLY;
    u8 QY_Ratio[2];
    u8 IHH[3];      //;��������    
    u8 GL_DLY;
    u8 GL_Ratio[2]; 
    u8 IH[3];        //;�����������
    u8 GBL_DLY;
    u8 GBL_Ratio[2]; 
    u8 I0High[3];   //;������������б����
    u8 G0L_DLY;
    u8 G0L_Ratio[2];     
    u8 SHH[3];      //;���ڹ��ʳ��������б����
    u8 SHH_DLY;
    u8 SHH_Ratio[2];  
    u8 SH[3];       //;���ڹ��ʳ������б����
    u8 SH_DLY;
    u8 SH_Ratio[2];  
    u8 UImb[2];     //;�����ѹ��ƽ�ⳬ���б����
    u8 UImb_DLY;
    u8 UImb_Ratio[2];  
    u8 IImb[2];     //;���������ƽ�ⳬ���б����
    u8 IImb_DLY;
    u8 IImb_Ratio[2];
    u8 ULostT;
};

struct PoPStr_F27 //;������ͭ��������� 24B
{
    u8 RA[2];  //;A�����
    u8 XA[2];  //;A��翹
    u8 GA[2];  //;A��絼
    u8 BA[2];  //;A�����
    u8 RB[2];  
    u8 XB[2];  
    u8 GB[2];  
    u8 BB[2];  
    u8 RC[2];  
    u8 XC[2];  
    u8 GC[2];  
    u8 BC[2];    
};

struct PoPStr_F28 //;�����㹦�������ֶ���ֵ 4B
{
    u8 Cos1[2];
    u8 Cos2[2];
};

struct PoPStr_F31 //;�ز��ӽڵ㸽���ڵ��ַ  25B
{
    u8 NodeN;
    u8 NodeAddr[NODENUM][6];
};

struct ZDPStr_F33_1_1 //;4B
{   
    u8 RTDStartT[2]; //;������ʱ�ο�ʼʱ��
    u8 RTDEndT[2];   //;������ʱ�ν���ʱ��
};

struct ZDPStr_F33_1     //;30B
{
    u8 ComPort;
    u8 RCtr[2];
    u8 RDay[4];
    u8 RTime[2];
    u8 RTimeDis;
    u8 BroadcastT[3];
    u8 RMTNum;
    struct ZDPStr_F33_1_1  RMTSet[RMTNUM];
};

struct ZDPStr_F33 //;�ն˳������в������� 61B
{
    u8 PortN;
    struct ZDPStr_F33_1 PortP[PORTNUM];  
};

struct ZDPStr_F34_1  //;6B
{
    u8 ComPort;
    u8 ComCtr;
    u8 BaudRate[4];  
};

struct ZDPStr_F34 //;����������ͨ��ģ��Ĳ�������  13B
{
    u8 PortN;
    struct ZDPStr_F34_1 PortP[PORTNUM];  
};

struct ZDPStr_F35 //;̨�����г����ص㻧���� 11B
{
    u8 UsrN;
    u8 UsrIndex[KEYMETNUM][2];  
};

struct ZDPStr_F37_1 //;�ն˼���ͨ�Ų���
{
    u8 A1[2];
    u8 A2[2];
};

struct ZDPStr_F37 //;�ն˼���ͨ�Ų��� 19B
{
    u8 Port;
    u8 Control; 
    u8 MesOut100ms;
    u8 ByteOut10ms;
    u8 ReSendNum;
    u8 CysMin;
    u8 ZDNum;
    struct ZDPStr_F37_1 JLZDAddr[3];
     
};

struct ZDPStr_F38_1 //;1�������������� //;34B
{   
    u8 UsrSIndex;
    u8 InfoGrpN;
    u8 GrpFlag[32]; //;��Ϣ��ı�־
};

struct ZDPStr_F38 //;1���2�������������� 
{
    u8 UsrBIndex;
    u8 GroupN;
    struct ZDPStr_F38_1 InfoGrp[USRSNUM];  //;��ֵ��Χ��1~16 138B
     
};

struct GroupStr_F41_1 //;ʱ�ι��ض�ֵ
{
    u8 SDIndex;
    u8 Value[8][2];
};

struct GroupStr_F41   //;ʱ�ι��ض�ֵ 52B
{
    u8 Flag; //;������־
    struct GroupStr_F41_1 Group[3];
};

struct GroupStr_F42 //;���ݹ��ز��� 6B
{
    u8 PValue[2];
    u8 Time[2];
    u8 DLY05H;
    u8 CtrFlag;
};

struct GroupStr_F44 //;Ӫҵ��ͣ�ز��� 8B
{
    u8 TimeS[3];
    u8 TimeE[3];
    u8 PValue[2];
};

struct GroupStr_F47 //;�������ز��� 17B
{
    u8 BuyIndex[4];
    u8 Flag;
    u8 BuyValue[4];
    u8 ArmValue[4];
    u8 CutValue[4];
};

struct ZDPStr_F59 //;���ܱ��쳣�б���ֵ�趨 4B 
{
    u8  EnergyOver;
    u8  MeterFly;
    u8  MeterStop;
    u8  MeterTime;
};

struct ZDPStr_F60 //;г����ֵ 44B
{
    u8 XBU_High[2];
    u8 XBU_High_Odd[2];
    u8 XBU_High_Even[2];
    u8 XBI_High[2];
    u8 XBI_High_2[2];
    u8 XBI_High_4[2];
    u8 XBI_High_6[2];
    u8 XBI_High_8[2];
    u8 XBI_High_10[2];
    u8 XBI_High_12[2];
    u8 XBI_High_14[2];
    u8 XBI_High_16[2];
    u8 XBI_High_18[2];
    u8 XBI_High_3[2];
    u8 XBI_High_5[2];
    u8 XBI_High_7[2];
    u8 XBI_High_9[2];
    u8 XBI_High_11[2];
    u8 XBI_High_13[2];
    u8 XBI_High_15[2];
    u8 XBI_High_17[2];
    u8 XBI_High_19[2];
};

struct ZDPStr_F62
{
    u8   Type;
    u16  Interval;
    u8   Retry;
    u8   DisconT;
};

struct TaskPStr_F65 //;89B
{
    u8  Cyc;
    u8  BaseTime[6];
    u8  Rate;
    u8  PnFnNum;
    u8  PnFn[20][4];
};   
    
struct PoPStr_F73 //;���������� 3B
{
    u8 Sign;
    u8 Cap[2];    
};    
    
struct PoPStr_F74 //;������Ͷ�����в��� 10B
{
    u8 PowerFactor[2];
    u8 InWP[3];
    u8 OutWP[3];
    u8 DLYSec;
    u8 DisMin;
};

struct PoPStr_F75 //;�������������� 16B
{
    u8 UOut[2];
    u8 UOutDis[2];
    u8 ULess[2];
    u8 ULessDis[2];
    u8 AberI[2];
    u8 AberIDis[2];
    u8 AberU[2];
    u8 AberUDis[2];  
};

struct DCPortStr_F81 //;ֱ��ģ����������
{
  u8 ValueS[2];
  u8 ValueE[2];  
};

struct DCPortStr_F82 //;ֱ��ģ������ֵ
{
  u8 ValueU[2];
  u8 ValueD[2];  
};
    
struct ZDPStr //;�ն˲����ṹ��
{
    struct ZDPStr_F1  F1 ;
    struct ZDPStr_F2  F2 ;
    struct ZDPStr_F3  F3 ;
    struct ZDPStr_F4  F4 ;
    struct ZDPStr_F5  F5 ;
    struct ZDPStr_F6  F6 ;
    struct ZDPStr_F7  F7 ;
    struct ZDPStr_F8  F8 ;
    struct ZDPStr_F9  F9 ;
    struct ZDPStr_F10 F10;
    struct ZDPStr_F11 F11;
    struct ZDPStr_F12 F12;
    struct ZDPStr_F13 F13;
    struct ZDPStr_F14 F14;
    struct ZDPStr_F15 F15;
    struct ZDPStr_F16 F16;
    u8 F17[2];
    u8 F18[12];
    u8 F19[1];
    u8 F20[1];
    u8 F21[25];
    struct ZDPStr_F22 F22;
    u8 F23[3];
    struct ZDPStr_F33 F33;
    struct ZDPStr_F34 F34;
    struct ZDPStr_F35 F35;
    u8 F36[4];
    struct ZDPStr_F37 F37;
    struct ZDPStr_F38 F38;
    struct ZDPStr_F38 F39;
    u8 F57[3];
    u8 F58[1];
    struct ZDPStr_F59 F59;
    struct ZDPStr_F60 F60;
    u8 F61[1];  
};

struct TaskPStr //;��������ṹ��
{
    struct TaskPStr_F65 F65[TASK1NUM];
    struct TaskPStr_F65 F66[TASK2NUM];
    u8 F67[TASK1NUM];
    u8 F68[TASK2NUM];
    
};

struct PoPStr  //;����������ṹ��
{
    struct PoPStr_F25 F25;
    struct PoPStr_F26 F26;
    struct PoPStr_F27 F27;
    struct PoPStr_F28 F28;
    u8 F29[12];  
    u8 F30[1]; 
    struct PoPStr_F31 F31; 
    struct PoPStr_F73 F73[16]; 
    struct PoPStr_F74 F74;
    struct PoPStr_F75 F75;
    u8 F76[1];
};

struct GroupStr //;�ܼ������
{
    struct GroupStr_F41 F41;
    struct GroupStr_F42 F42;
    u8 F43[1];
    struct GroupStr_F44 F44;
    u8 F45[1];
    u8 F46[4];
    struct GroupStr_F47 F47;
    u8 F48[1];
};

struct DCPortStr //;ֱ��ģ��������
{
    struct DCPortStr_F81 F81;
    struct DCPortStr_F82 F82;
    u8 F83[1];
};

//;struct GprsComStr
//;{
//;    u8 QuXian[2];
//;    u16 DiZhi;
//;};

struct ZDSStr_F5_1
{
    u8 PCtrNum;
    u8 PCtrESign;
    u8 PCtrStat;
    u8 EleCtrStat;
    u8 PCtrTStat;
    u8 EleCtrTStat;
};

struct ZDSStr_F5  //;50B
{
    u8 InSign;
    u8 AddSign;
    struct ZDSStr_F5_1 AddGRP[8]; 
};

struct ZDSStr_F6_1
{
    u8 PValue[2];
    u8 PDown;
    u8 PCutStat;
    u8 MonthCutStat;
    u8 BuyCutStat;
    u8 PCtrAStat;
    u8 EleCtrAStat;
};

struct ZDSStr_F6  //;64B
{
    u8 TelCutStat;
    u8 AlarmStat;
    u8 AddSign;
    struct ZDSStr_F6_1 AddGRP[8]; 
};

struct ZDSStr_F10 //;�ն�����վ���ա�����ͨ������
{
    u32 Flux_Day;
    u32 Flux_Month;
};

struct ZDSStr_F11_1  //;�ն˼��г���״̬��Ϣ 
{
    u8 PortInd;
    u8 RMeterN[2];
    u8 Sign;
    u8 SucceedN;
    u8 ImpSucN;
    u8 RStartT[6];
    u8 REndT[6];  
};

struct ZDSStr_F11  //;�ն˼��г���״̬��Ϣ 37B
{
    u8 PortN;
    struct ZDSStr_F11_1 PortS[PORTNUM];  
};

struct ZDStatStr  //;�ն�����
{   
    u8 F2[6];           //;�ն�����ʱ��            F2
    u8 F3[31];       //;����ӳ���              F3
    u8 F4[1];       //;�ն�ͨ��״̬            F4
    struct ZDSStr_F5 F5;     //;�ն˿�������״̬        F5
    struct ZDSStr_F6 F6;     //;�ն˵�ǰ����״̬        F6
    u8 F7[2];               //;�ն���Ҫ�¼���������ǰֵ F7
    u8 F8[8];         //;�ն��¼���־״̬         F8
    u8 F9[2];            //;�ն�״̬������λ��־     F9
    struct ZDSStr_F10 F10;  //;�ն�����վ���ա�������   F10
    struct ZDSStr_F11 F11;  //;�ն˼��г���״̬��Ϣ    F11
};

struct Alarm_CS //;35Bye
{
    u8 Start[1];
    u16 BACOM; //;A����B��ͨ���ж�
    u16 BCCOM; //;C����B��ͨ���ж�
    u16 ZBCOM; //;�ն���B��ͨ���ж�
    u16 ZMCOM; //;�ն�����վͨ���ж�    
    u16 ZDOFF;
    u16 UA125;
    u16 UA75;
    u16 UA50;
    u16 UA0;
    u16 IA125;
    u16 IA0;
    u16 UC125;
    u16 UC75;
    u16 UC50;
    u16 UC0;
    u16 IC125;
    u16 IC0;
};

/*----------------ɽ��ʡ������˾ͨѶ��Լ����@1-----------------------*/
struct ZDPStr_F89
{
    u8 QuXian[2];
    u8 DiZhi[2];
};

struct ZDPStr_F90 //;ȱ�ࡢ������¼��ж���ʱ�����ߡ�ͣ���¼��Ƚϲ�����
{
    u8 ERCOCT[1]; //;�¼������ж�ʱ��
    u8 ERCOVT[1]; //;�¼��ָ��ж�ʱ��
    u8 FlyPOI[1]; //;���ߡ�ͣ���¼��Ƚϲ�����
};

struct ZDPStr_F91  //;��ǰ�����¸��صĸ澯ʱ��Ϳ���ʱ��
{
    u8 XFKT1[1]; //;��ǰ�����¸��ص�1�ָ澯ʱ��
    u8 XFKT2[1]; //;��ǰ�����¸��ص�2�ָ澯ʱ��
    u8 XFKT3[1]; //;��ǰ�����¸��ص�3�ָ澯ʱ��
    u8 XFKT4[1]; //;��ǰ�����¸��ص�4�ָ澯ʱ��
    u8 XFKT[1];  //;��ǰ�����¸��صĿ���ʱ��
};

/*----------------����˾ͨѶ��Լ����-----------------------*/
struct ZDPStr_F97  //;�ն��¼�������������
{
    u8  UserSend[8];
    u8  AdmiSend[8];
};

struct  ZDPStrKZ   //;���ն˲�����չ��12
{
    //;---------------��12-----------
    struct ZDPStr_F89 F89;
    struct ZDPStr_F90 F90;
    struct ZDPStr_F91 F91;
    //;---------------��13-----------
    struct ZDPStr_F97 F97; //;�ն��¼�������������
    u8 F98[20]; //;�û�����ͷ
    u8 F99[20]; //;�û�����β
    u8 F100[20];//;����Ա����ͷ
    u8 F101[20];//;����Ա����β
};


struct AlarmHVM_Control  //;��ѹ���ܱ�澯���״̬
{
    u8 Status[15];  
    u8 ComAC;       
    u8 BAE,BAER;
    u8 BCE,BCER;
    u16 BJE,BJER;
    u16 PWROFF,PWROFFR;
    u8 AUH,AUHR;
    u8 AUL,AULR;
    u8 CUH,CUHR;
    u8 CUL,CULR;
    u8 AIH,AIHR;
    u8 AIL,AILR;
    u8 CIH,CIHR;
    u8 CIL,CILR;
    u8 PhaseA,PhaseC;
    u8 EnUpL;       
    u8 ComRev;     
    u8 UpCnt;       
    u8 IdleCnt;     
    u8 SendIdle;     
    u8 State;       
    u8 SendFlag;    
    u16 WaitAns;    
};

//;void __Point_struct(void){}
struct DJ_B2
{
    u8 Td_h[2];      
    u8 Data[4][2];
};

struct DJ_B3
{
    u8 Td_h[2];
    u8 Data[4][3];
};

struct DJ_B4
{
    u8 Td_h[2];
    u8 Data[4][4];
};



struct DF25DF33Str  //; T2_F25   32bye
{
    u8 ZPMax[3],ZPMax_T[3];   //;������й����ʼ�����ʱ��
    u8 APMax[3],APMax_T[3];   //;A����й����ʼ�����ʱ��
    u8 BPMax[3],BPMax_T[3];   //;B����й����ʼ�����ʱ��
    u8 CPMax[3],CPMax_T[3];   //;C����й����ʼ�����ʱ��
    u8 ZP0Min[2];
    u8 AP0Min[2];
    u8 BP0Min[2];
    u8 CP0Min[2];   //;��ABC�й�����Ϊ0�ĳ���ʱ��
};


struct DF26F34Str   //; T2_F26
{
    u8 ZPMN[3],ZPMN_T[3];     //;���й��������������ʱ��
    u8 APMN[3],APMN_T[3];     //;A�й��������������ʱ��
    u8 BPMN[3],BPMN_T[3];     //;B�й��������������ʱ��
    u8 CPMN[3],CPMN_T[3];     //;C�й��������������ʱ��
};

struct DF27F35Str    //; T2_F27
{
    u8 AVUUMin[2], AVLLMin[2], AVUMin[2], AVLMin[2], AVHGMin[2];  //;A���ѹͳ��
    u8 BVUUMin[2], BVLLMin[2], BVUMin[2], BVLMin[2], BVHGMin[2];  //;B���ѹͳ��
    u8 CVUUMin[2], CVLLMin[2], CVUMin[2], CVLMin[2], CVHGMin[2];  //;C���ѹͳ��
    u8 AVMax[2],AVMax_T[3],AVMini[2],AVMini_T[3]; //;A���ѹ��ֵ��¼
    u8 BVMax[2],BVMax_T[3],BVMini[2],BVMini_T[3]; //;B���ѹ��ֵ��¼
    u8 CVMax[2],CVMax_T[3],CVMini[2],CVMini_T[3]; //;C���ѹ��ֵ��¼
    u8 AAver[2],BAver[2],CAver[2];   //;�����ƽ����ѹ
};

struct DF28Str  //; T2_F28 14B
{
    u8 IImbMin[2],VImbMin[2];
    u8 IImbV[2], IImbT[3];
    u8 UImbV[2], UImbT[3];
};

struct DF36Str  //; T2_F28 16B
{
    u8 IImbMin[2],VImbMin[2];
    u8 IImbV[2], IImbT[4];
    u8 UImbV[2], UImbT[4];
};

struct DF29F37Str  //; T2_F29 38B
{
    u8 AIUUMin[2],AIUMin[2];  //;A�����ͳ��
    u8 BIUUMin[2],BIUMin[2];  //;B�����ͳ��
    u8 CIUUMin[2],CIUMin[2];  //;C�����ͳ��
    u8 I0UMin[2];  //;�������Խ�����ۼ�ʱ��
    u8 AIMax[3],AIMax_T[3]; //;A��������ֵ��¼
    u8 BIMax[3],BIMax_T[3]; //;B��������ֵ��¼
    u8 CIMax[3],CIMax_T[3]; //;C��������ֵ��¼
    u8 I0Max[3],I0Max_T[3]; //;����������ֵ��¼
};

struct DF30F38Str //; T2_F30 4B
{
    u8 SUUMin[2], SUMin[2];    //;���ڹ���Խ���ۼ�ʱ��
};

struct DF31Str  //; T2_F31 �ո�����ͳ�� 10B
{
    u8 Vmax[2];
    u8 VmaxT[3];
    u8 Vmin[2];
    u8 VminT[3];  
};

struct DF39Str  //; T2_F39 �ո�����ͳ�� 12B
{
    u8 Vmax[2];
    u8 VmaxT[4];
    u8 Vmin[2];
    u8 VminT[4];  
};

struct DF49F51Str  //;����ʱ�䣬��λ�ۼƴ��� 4B
{
    u8 PowerT[2];
    u8 Reset[2];
};

struct DF50F52Str  //;����ʱ�䣬��λ�ۼƴ��� 4B
{
    u8 MonthCutNum[1];
    u8 BuyCutNum[1];
    u8 PCutNum[1];
    u8 TelCutNum[1];
};

struct DF209Str  //;���ܱ�Զ�̿���״̬����Ϣ
{
    u8 MeterPowerStat[1];
    u8 MPCloseT[5];
    u8 MPCutT[5];  
};

struct DF213Str  //;�¶�����ܱ��ز���������ʱ��
{
    u8 ProgramNum[2];
    u8 LastProgramT[5];
    u8 LidOpenNum[2];
    u8 LastOpenT[5];  
};

struct DF214Str  //;�¶�����ܱ�����޸Ĵ�����ʱ��
{
    u8 CLKChangeNum[2];
    u8 CLastChangeT[5];
    u8 PeriodChangeNum[2];
    u8 PLastChangeT[5];  
};

struct PointDataStr  //;����������
{
    u8 Start[1];
    u8 CB_Time[5];//;����ʱ��
    u8 RC313[1];//;������
    
                          /*GDW376.1��Լ����*/
    /*------------- F25 -----------*/
    u8 RB630[3],RB631[3],RB632[3],RB633[3];//;��,A,B,C˲ʱ�й�����(������)
    u8 RB640[3],RB641[3],RB642[3],RB643[3];//;˲ʱ�޹�����(������)
    u8 RB650[2],RB651[2],RB652[2],RB653[2];//;��������
    u8 RB611[2],RB612[2],RB613[2];         //;��ѹ
    u8 RB621[3],RB622[3],RB623[3];         //;����˲ʱ����
    u8 RB6E5[3];                           //;�������
    u8 RB660[3],RB661[3],RB662[3],RB663[3];//;���ڹ���

    /*------------- F26 -----------*/
    u8 RB310[2],RB311[2],RB312[2],RB313[2];//;�����ۼƴ���
    u8 RB320[3],RB321[3],RB322[3],RB323[3];//;�����ۼ�ʱ��
    u8 RB330[4],RB331[4],RB332[4],RB333[4];//;���һ�ζ��࿪ʼʱ��
    u8 RB340[4],RB341[4],RB342[4],RB343[4];//;���һ�ζ������ʱ��

    /*------------- F27 -----------*/
    u8 RDayTime[6];       //;���ܱ�����ʱ��
    u8 RB216[4];          //;��ع���ʱ��
    u8 RB212[3],RB21A[6]; //;��ǰ��̴��������һ�α�̷���ʱ��
    u8 RB214[3],RB21C[6]; //;��������ܴ��������һ�η���ʱ��
    u8 RB213[3],RB21B[6]; //;���������ܴ��������һ�η���ʱ��
    u8 RB215[3],RB21D[6]; //;�¼������ܴ��������һ�η���ʱ��
    u8 RB217[3],RB21F[6]; //;Уʱ�ܴ��������һ�η���ʱ��
    
    /*-------------- F28 ---------*/
    u8 RC020[2]; //;�������״̬�ֱ�λ��־1
    u8 RC021[2]; //;�������״̬�ֱ�λ��־2
    u8 RC022[2]; //;�������״̬�ֱ�λ��־3
    u8 RC023[2]; //;�������״̬�ֱ�λ��־4
    u8 RC024[2]; //;�������״̬�ֱ�λ��־5
    u8 RC025[2]; //;�������״̬�ֱ�λ��־6
    u8 RC026[2]; //;�������״̬�ֱ�λ��־7
    u8 RC027[2]; //;�������״̬��1
    u8 RC028[2]; //;�������״̬��2
    u8 RC029[2]; //;�������״̬��3
    u8 RC02A[2]; //;�������״̬��4
    u8 RC02B[2]; //;�������״̬��5
    u8 RC02C[2]; //;�������״̬��6
    u8 RC02D[2]; //;�������״̬��7
    
    /*------------- F29 -----------*/
    u8 R2000[5],R2001[5]; //;��ǰͭ�������й��ܵ���ʾֵ
    
    /*------------- F30 -----------*/
    u8 R2010[5],R2011[5]; //;��һ��ͭ�������й��ܵ���ʾֵ
    
    /*------------- F31 -----------*/
    u8 R30A1[5],R30A2[5]; //;A�����򡢷����й��ܵ���ʾֵ
    u8 R30A3[4],R30A4[4]; //;A������޹�1/2����ʾֵ
    u8 R30B1[5],R30B2[5]; //;B�����򡢷����й��ܵ���ʾֵ
    u8 R30B3[4],R30B4[4]; //;B������޹�1/2����ʾֵ
    u8 R30C1[5],R30C2[5]; //;C�����򡢷����й��ܵ���ʾֵ
    u8 R30C3[4],R30C4[4]; //;C������޹�1/2����ʾֵ
    
     /*------------- F32 -----------*/
    u8 R31A1[5],R31A2[5]; //;��һ������A�����򡢷����й��ܵ���ʾֵ
    u8 R31A3[4],R31A4[4]; //;��һ������A������޹�1/2����ʾֵ
    u8 R31B1[5],R31B2[5]; //;��һ������B�����򡢷����й��ܵ���ʾֵ
    u8 R31B3[4],R31B4[4]; //;��һ������B������޹�1/2����ʾֵ
    u8 R31C1[5],R31C2[5]; //;��һ������C�����򡢷����й��ܵ���ʾֵ
    u8 R31C3[4],R31C4[4]; //;��һ������C������޹�1/2����ʾֵ
    
    /*------------- F33 -----------*/
    u8 R9010[5],R9011[5],R9012[5],R9013[5],R9014[5]; //; ��ǰ�����й������ʵ���
    u8 R9110[4],R9111[4],R9112[4],R9113[4],R9114[4]; //;��ǰ�����޹������ʵ���
    u8 R9130[4],R9131[4],R9132[4],R9133[4],R9134[4]; //;һ�����޹������ʵ���
    u8 R9140[4],R9141[4],R9142[4],R9143[4],R9144[4]; //;�������޹������ʵ���
    
    /*------------- F34 -----------*/
    u8 R9020[5],R9021[5],R9022[5],R9023[5],R9024[5]; //;��ǰ�����й������ʵ���
    u8 R9120[4],R9121[4],R9122[4],R9123[4],R9124[4]; //;��ǰ�����޹������ʵ���
    u8 R9150[4],R9151[4],R9152[4],R9153[4],R9154[4]; //;�������޹������ʵ���
    u8 R9160[4],R9161[4],R9162[4],R9163[4],R9164[4]; //;�������޹������ʵ���
    
    /*------------- F35 -----------*/
    u8 RA010[3],RA011[3],RA012[3],RA013[3],RA014[3];//;��ǰ�����й��������
    u8 RB010[4],RB011[4],RB012[4],RB013[4],RB014[4];//;��ǰ�����й������������ʱ��
    u8 RA110[3],RA111[3],RA112[3],RA113[3],RA114[3];//;��ǰ�����޹��������
    u8 RB110[4],RB111[4],RB112[4],RB113[4],RB114[4];//;��ǰ�����޹������������ʱ��
    
    /*------------- F36 -----------*/
    u8 RA020[3],RA021[3],RA022[3],RA023[3],RA024[3];//;��ǰ�����й��������������
    u8 RB020[4],RB021[4],RB022[4],RB023[4],RB024[4];//;��ǰ�����й������������������ʱ��
    u8 RA120[3],RA121[3],RA122[3],RA123[3],RA124[3];//;��ǰ�����޹��������������
    u8 RB120[4],RB121[4],RB122[4],RB123[4],RB124[4];//;��ǰ�����޹������������������ʱ��
    
    /*------------- F37 -----------*/
    u8 R9410[5],R9411[5],R9412[5],R9413[5],R9414[5];//;���������й������ʵ���
    u8 R9510[4],R9511[4],R9512[4],R9513[4],R9514[4];//;���������޹������ʵ���
    u8 R9530[4],R9531[4],R9532[4],R9533[4],R9534[4];//;����һ�����޹������ʵ���
    u8 R9540[4],R9541[4],R9542[4],R9543[4],R9544[4];//;�����������޹������ʵ���
    
    /*------------- F38 -----------*/
    u8 R9420[5],R9421[5],R9422[5],R9423[5],R9424[5];//;���·����й������ʵ���
    u8 R9520[4],R9521[4],R9522[4],R9523[4],R9524[4];//;���·����޹������ʵ���
    u8 R9550[4],R9551[4],R9552[4],R9553[4],R9554[4];//;���¶������޹������ʵ���
    u8 R9560[4],R9561[4],R9562[4],R9563[4],R9564[4];//;�����������޹������ʵ���
    
    /*------------- F39 -----------*/
    u8 RA410[3],RA411[3],RA412[3],RA413[3],RA414[3];//;���������й��������
    u8 RB410[4],RB411[4],RB412[4],RB413[4],RB414[4];//;���������й������������������ʱ��
    u8 RA510[3],RA511[3],RA512[3],RA513[3],RA514[3];//;���������޹��������
    u8 RB510[4],RB511[4],RB512[4],RB513[4],RB514[4];//;���������޹������������������ʱ��
    
    /*------------- F40 -----------*/
    u8 RA420[3],RA421[3],RA422[3],RA423[3],RA424[3];//;���·����й��������������
    u8 RB420[4],RB421[4],RB422[4],RB423[4],RB424[4];//;���·����й������������������ʱ��
    u8 RA520[3],RA521[3],RA522[3],RA523[3],RA524[3];//;���·����޹��������������
    u8 RB520[4],RB521[4],RB522[4],RB523[4],RB524[4];//;���·����޹������������������ʱ��
    

    /*------------- F41~F48 -----------*/
    u8 R9010_D[4],R9011_D[4],R9012_D[4],R9013_D[4],R9014_D[4];//;���������й������ʵ���
    u8 R9110_D[4],R9111_D[4],R9112_D[4],R9113_D[4],R9114_D[4];//;���������޹������ʵ���
    u8 R9020_D[4],R9021_D[4],R9022_D[4],R9023_D[4],R9024_D[4];//;���շ����й������ʵ���
    u8 R9120_D[4],R9121_D[4],R9122_D[4],R9123_D[4],R9124_D[4];//;���շ����޹������ʵ���
    u8 R9010_M[4],R9011_M[4],R9012_M[4],R9013_M[4],R9014_M[4];//;���������й������ʵ���
    u8 R9110_M[4],R9111_M[4],R9112_M[4],R9113_M[4],R9114_M[4];//;���������޹������ʵ���
    u8 R9020_M[4],R9021_M[4],R9022_M[4],R9023_M[4],R9024_M[4];//;���·����й������ʵ���
    u8 R9120_M[4],R9121_M[4],R9122_M[4],R9123_M[4],R9124_M[4];//;���·����޹������ʵ���
    
    /*-------------- F49 ------------*/
    u8 RB671[2],RB672[2],RB673[2]; //;A��B��C��ѹ��λ��
    u8 RB681[2],RB682[2],RB683[2]; //;A��B��C������λ��

    //;*------------- F89~F116 ----------- 366B
    struct DJ_B3  ODJ_B630,  ODJ_B631,  ODJ_B632,  ODJ_B633;  //;��һСʱ������,A,B,C�й�����
    struct DJ_B3  ODJ_B640,  ODJ_B641,  ODJ_B642,  ODJ_B643;  //;��һСʱ������,A,B,C�޹�����
    struct DJ_B2  ODJ_B611,  ODJ_B612,  ODJ_B613;             //;��һСʱ������,A,B,C���ѹ
    struct DJ_B3  ODJ_B621,  ODJ_B622,  ODJ_B623,  ODJ_B6E5;  //;��һСʱ������,A,B,C������Լ����ߵ���
    struct DJ_B4  ODJ_9010,  ODJ_9110,  ODJ_9020,  ODJ_9120;  //;��һСʱ�����յ�����
    struct DJ_B4  ODJ_9010_1,ODJ_9110_1,ODJ_9020_1,ODJ_9120_1;//;��һСʱ�����ܵ�����ʾֵ
    struct DJ_B2  ODJ_B650,  ODJ_B651,  ODJ_B652  ,ODJ_B653;  //;��һСʱ������,A,B,C�๦������

    struct DJ_B3  NDJ_B630  ,NDJ_B631  ,NDJ_B632  ,NDJ_B633;  //;��ǰСʱ������,A,B,C�й�����
    struct DJ_B3  NDJ_B640  ,NDJ_B641  ,NDJ_B642  ,NDJ_B643;  //;��ǰСʱ������,A,B,C�޹�����
    struct DJ_B2  NDJ_B611  ,NDJ_B612  ,NDJ_B613  ;           //;��ǰСʱ����A,B,C���ѹ
    struct DJ_B3  NDJ_B621  ,NDJ_B622  ,NDJ_B623  ,NDJ_B6E5;  //;��ǰСʱ����A,B,C������Լ����ߵ���
    struct DJ_B4  NDJ_9010  ,NDJ_9110  ,NDJ_9020  ,NDJ_9120;  //;��ǰСʱ�����յ�����
    struct DJ_B4  NDJ_9010_1,NDJ_9110_1,NDJ_9020_1,NDJ_9120_1;//;��ǰСʱ�����ܵ���ʾֵ
    struct DJ_B2  NDJ_B650  ,NDJ_B651  ,NDJ_B652  ,NDJ_B653;  //;��ǰСʱ������,A,B,C�๦������
    
    /*----------- F153~F160 ------------*/
    u8 R5010[5],R5011[5],R5012[5],R5013[5],R5014[5]; //;��һʱ�����������й�����ʾֵ
    u8 R5020[5],R5021[5],R5022[5],R5023[5],R5024[5]; //;�ڶ�ʱ�����������й�����ʾֵ
    u8 R5030[5],R5031[5],R5032[5],R5033[5],R5034[5]; //;����ʱ�����������й�����ʾֵ
    u8 R5040[5],R5041[5],R5042[5],R5043[5],R5044[5]; //;����ʱ�����������й�����ʾֵ
    u8 R5050[5],R5051[5],R5052[5],R5053[5],R5054[5]; //;����ʱ�����������й�����ʾֵ
    u8 R5060[5],R5061[5],R5062[5],R5063[5],R5064[5]; //;����ʱ�����������й�����ʾֵ
    u8 R5070[5],R5071[5],R5072[5],R5073[5],R5074[5]; //;����ʱ�����������й�����ʾֵ
    u8 R5080[5],R5081[5],R5082[5],R5083[5],R5084[5]; //;�ڰ�ʱ�����������й�����ʾֵ

    /*--------------F165~F166-----------------*/
    u8 DB212[2],DB21A[5];  //;����ܴ��������һ��ʱ�� by zrt 20130809
    u8 RB218[2],RB219[5];  //;��β�Ǵ��������һ�δ�ʱ��by zrt 20130808		
    u8 DB217[2],DB21F[5]; //;Уʱ�ܴ��������һ��ʱ��by zrt 20130814
    u8 RB220[2],RB221[5];	//;ʱ�β����޸��ܴ��������һ��ʱ��by zrt 20130814

    u8 RB222[3],RB223[5];  //;�ܵ�����������һ��ʱ��by zrt 20310828
    u8 RB224[3];                 //;����ʱ�α��̴���by zrt 20130910
    
    //;T2_F31 ��ͳ�����ݣ���207B
    struct DF25DF33Str  DF25;  //;�����ն������¶���
    struct DF26F34Str   DF26;
    struct DF27F35Str   DF27;
    struct DF28Str      DF28;
    struct DF29F37Str   DF29;
    struct DF30F38Str   DF30;
    struct DF31Str      DF31;
    struct DF49F51Str   DF49;
    struct DF50F52Str   DF50;
    struct DF209Str     DF209;
    //;��ͳ�����ݣ���228B
    struct DF25DF33Str  DF33;  //;�����ն������¶���
    struct DF26F34Str   DF34;
    struct DF27F35Str   DF35;
    struct DF36Str      DF36;
    struct DF29F37Str   DF37;
    struct DF30F38Str   DF38;
    struct DF39Str      DF39;
    struct DF49F51Str   DF51;
    struct DF50F52Str   DF52;
    struct DF213Str     DF213;
    struct DF214Str     DF214;

    u8 Z9010_D[5],Z9011_D[5],Z9012_D[5],Z9013_D[5],Z9014_D[5];//;������������й������ʵ���
    u8 Z9020_D[5],Z9021_D[5],Z9022_D[5],Z9023_D[5],Z9024_D[5];//;������㷴���й������ʵ���
    u8 Z9110_D[4],Z9111_D[4],Z9112_D[4],Z9113_D[4],Z9114_D[4];//;������������޹������ʵ���
    u8 Z9120_D[4],Z9121_D[4],Z9122_D[4],Z9123_D[4],Z9124_D[4];//;������㷴���޹������ʵ���

    u8 Z9010_M[5],Z9011_M[5],Z9012_M[5],Z9013_M[5],Z9014_M[5];//;������������й������ʵ���
    u8 Z9020_M[5],Z9021_M[5],Z9022_M[5],Z9023_M[5],Z9024_M[5];//;������㷴���й������ʵ���
    u8 Z9110_M[4],Z9111_M[4],Z9112_M[4],Z9113_M[4],Z9114_M[4];//;������������޹������ʵ���
    u8 Z9120_M[4],Z9121_M[4],Z9122_M[4],Z9123_M[4],Z9124_M[4];//;������㷴���޹������ʵ���

    u8 DB212_M[2],DB21A_M[5];  //;	���±���ܴ��������һ��ʱ�� by zrt 20130815
    u8 RB218_M[2],RB219_M[5];	  //;���¿�β�Ǵ��������һ�δ�ʱ��
    u8 DB217_M[2],DB21F_M[5];  //;����Уʱ�ܴ��������һ��ʱ��
    u8 RB220_M[2],RB221_M[5];	  //;����ʱ�β����޸��ܴ��������һ��ʱ��

    /*others*/
//;    u8 RB660[3]; //;���ڹ���
//;    u8 RB670[1]; //;��Դ���߷�ʽ
//;    u8 RB710[2]; //;ʧѹ����
    u8 RC040[3],RC041[3]; //;��ѹ���������,
    u8 RC030[3],RC031[3],RC032[6];  //;�����,���
    u8 RC141[6]; //;�궨��ѹ
    u8 RC142[6]; //;�궨���� ASCII
    u8 RC117[2],RC118[2],RC119[2];//;�Զ���������
    u8 RB710[3]; //;ʧѹ����
//;    u8 RC141[3];//;�궨��ѹ
//;    u8 RC331[3],RC332[3],RC333[3],RC334[3],RC335[3],RC336[3],RC337[3],RC338[3];//;��ǰʱ�α���ʼʱ�估���ʺ�

    /*ǰһ��������*/
    u8 O9010[5]; //;�����й��ܵ��ʾֵ�����ڼ�����ʾ���½���
//;    u8 OC331[3],OC332[3],OC333[3],OC334[3],OC335[3],OC336[3],OC337[3],OC338[3];//;ǰһ����ʱ�α���ʼʱ�估���ʺ�
    u8 OC040[3],OC041[3]; //;��ѹ���������,
    u8 OA010[3],OA020[3]; //;�����й����޹��������
    u8 OB212[3]; //;����ܴ���
    u8 OB21A[6]; //;�����һ�α��ʱ��
    u8 OB213[3]; //;��������������
    u8 OB310[2]; //;�ܶ������
    u8 OB710[3]; //;ʧѹ����
    u8 OC030[3],OC031[3]; //;���峣��
    u8 OC117[2],OC118[2],OC119[2];//;�Զ���������

    u8 OB222[3],OB223[5];  // �����ܴ��������һ�η���ʱ��by zrt 20130828

    u8 OB224[3];                 //ʱ�α��̴���

//    u8 OC027[2];  //;�����Ƿѹ��ʶby zrt 20130829
	
    u8 End[1];
};

struct MeterData /*���Ե��ܱ�������ʺ�485��Լ*/
{
    u8 Start[1];
    u8 GYType[1]; //;��ǰ���õĹ�Լ����
    u8 CB_Time[5];
    
    u8 R9010[4],R9011[4],R9012[4],R9013[4],R9014[4];//;��ǰ�����й������ʵ���
    u8 R9020[4],R9021[4],R9022[4],R9023[4],R9024[4];//;��ǰ�����й������ʵ���

    u8 R9110[4],R9111[4],R9112[4],R9113[4],R9114[4];//;��ǰ�����޹������ʵ���
    u8 R9120[4],R9121[4],R9122[4],R9123[4],R9124[4];//;��ǰ�����޹������ʵ���

    u8 R9130[4],R9131[4],R9132[4],R9133[4],R9134[4];//;��ǰһ�����޹������ʵ���
    u8 R9140[4],R9141[4],R9142[4],R9143[4],R9144[4];//;��ǰ�������޹������ʵ���
    u8 R9150[4],R9151[4],R9152[4],R9153[4],R9154[4];//;��ǰ�������޹������ʵ���
    u8 R9160[4],R9161[4],R9162[4],R9163[4],R9164[4];//;��ǰ�������޹������ʵ���
    
    u8 R30A1[4]; //;A�����й��ܵ���ʾֵ  ��ǰ
    u8 R30B1[4]; //;B�����й��ܵ���ʾֵ
    u8 R30C1[4]; //;C�����й��ܵ���ʾֵ

    u8 R30A2[4]; //;A�෴�й��ܵ���ʾֵ  by zrt 20130909
    u8 R30B2[4]; //;B�෴�й��ܵ���ʾֵ
    u8 R30C2[4]; //;C�෴�й��ܵ���ʾֵ	

    u8 R30A3[4]; //;A�����޹��ܵ���ʾֵ
    u8 R30B3[4]; //;B�����޹��ܵ���ʾֵ
    u8 R30C3[4]; //;C�����޹��ܵ���ʾֵ

    u8 R30A4[4]; //;A�෴�޹��ܵ���ʾֵ  
    u8 R30B4[4]; //;B�෴�޹��ܵ���ʾֵ
    u8 R30C4[4]; //;C�෴�޹��ܵ���ʾֵ		

    u8 R9410[4],R9411[4],R9412[4],R9413[4],R9414[4];//;���������й������ʵ���
    u8 R9420[4],R9421[4],R9422[4],R9423[4],R9424[4];//;���·����й������ʵ���

    u8 R9510[4],R9511[4],R9512[4],R9513[4],R9514[4];//;���������޹������ʵ���
    u8 R9520[4],R9521[4],R9522[4],R9523[4],R9524[4];//;���·����޹������ʵ���

    u8 R9530[4],R9531[4],R9532[4],R9533[4],R9534[4];//;����һ�����޹������ʵ���
    u8 R9540[4],R9541[4],R9542[4],R9543[4],R9544[4];//;�����������޹������ʵ���
    u8 R9550[4],R9551[4],R9552[4],R9553[4],R9554[4];//;���¶������޹������ʵ���
    u8 R9560[4],R9561[4],R9562[4],R9563[4],R9564[4];//;�����������޹������ʵ���

    u8 R31A1[4]; //;A�����й��ܵ���ʾֵ  ����
    u8 R31B1[4]; //;B�����й��ܵ���ʾֵ
    u8 R31C1[4]; //;C�����й��ܵ���ʾֵ

    u8 R31A2[4]; //;A�෴�й��ܵ���ʾֵ  by zrt 20130909
    u8 R31B2[4]; //;B�෴�й��ܵ���ʾֵ
    u8 R31C2[4]; //;C�෴�й��ܵ���ʾֵ		

    u8 R31A3[4]; //;A�����޹��ܵ���ʾֵ 
    u8 R31B3[4]; //;B�����޹��ܵ���ʾֵ
    u8 R31C3[4]; //;C�����޹��ܵ���ʾֵ

    u8 R31A4[4]; //;A�෴�޹��ܵ���ʾֵ 
    u8 R31B4[4]; //;B�෴�޹��ܵ���ʾֵ
    u8 R31C4[4]; //;C�෴�޹��ܵ���ʾֵ		
    
    u8 RA010[3],RB010[5],RA011[3],RB011[5],RA012[3],RB012[5];//;���й����������ʱ��
    u8 RA013[3],RB013[5],RA014[3],RB014[5];
    u8 RA020[3],RB020[5],RA021[3],RB021[5],RA022[3],RB022[5];//;���й����������ʱ��
    u8 RA023[3],RB023[5],RA024[3],RB024[5];
    
    u8 RA110[3],RB110[5],RA111[3],RB111[5],RA112[3],RB112[5];//;���޹����������ʱ��
    u8 RA113[3],RB113[5],RA114[3],RB114[5];
    u8 RA120[3],RB120[5],RA121[3],RB121[5],RA122[3],RB122[5];//;���޹����������ʱ��
    u8 RA123[3],RB123[5],RA124[3],RB124[5];

    u8 RA410[3],RB410[5],RA411[3],RB411[5],RA412[3],RB412[5];//;�������й����������ʱ��
    u8 RA413[3],RB413[5],RA414[3],RB414[5];
    u8 RA420[3],RB420[5],RA421[3],RB421[5],RA422[3],RB422[5];//;���·��й����������ʱ��
    u8 RA423[3],RB423[5],RA424[3],RB424[5];
    
    u8 RA510[3],RB510[5],RA511[3],RB511[5],RA512[3],RB512[5];//;�������޹����������ʱ��
    u8 RA513[3],RB513[5],RA514[3],RB514[5];
    u8 RA520[3],RB520[5],RA521[3],RB521[5],RA522[3],RB522[5];//;���·��޹����������ʱ��
    u8 RA523[3],RB523[5],RA524[3],RB524[5];

    u8 RC010[4],RC011[3]; //;���ڼ��ܴ�,ʱ��
    u8 RB216[4];          //;��ع���ʱ��
    u8 RB212[3],RB21A[6]; //;��ǰ��̴��������һ�α�̷���ʱ��
    u8 RB214[3],RB21C[6]; //;��������ܴ��������һ�η���ʱ��
    u8 RB213[3],RB21B[6]; //;���������ܴ��������һ�η���ʱ��
    u8 RB215[3],RB21D[6]; //;�¼������ܴ��������һ�η���ʱ��
    u8 RB217[3],RB21F[10]; //;Уʱ�ܴ��������һ�η���ʱ��

     u8 RB310[3],RB311[3],RB312[3],RB313[3];//;�����ۼƴ���
    u8 RB320[3],RB321[3],RB322[3],RB323[3];//;�����ۼ�ʱ��
    u8 RB330[6],RB331[6],RB332[6],RB333[6];//;���һ�ζ��࿪ʼʱ��
    u8 RB340[6],RB341[6],RB342[6],RB343[6];//;���һ�ζ������ʱ��

    u8 RB611[2],RB612[2],RB613[2];//;����˲ʱ��ѹ
    u8 RB621[3],RB622[3],RB623[3];//;����˲ʱ����
    u8 RB630[3],RB631[3],RB632[3],RB633[3];//;�й�����
    u8 RB640[3],RB641[3],RB642[3],RB643[3];//;�޹�����
    u8 RB660[3],RB661[3],RB662[3],RB663[3];//;���ڹ���
    u8 RB650[2],RB651[2],RB652[2],RB653[2];//;��������
    u8 RB671[2],RB672[2],RB673[2]; //;A��B��C��ѹ��λ��
    
    u8 RC027[2]; //;�������״̬��1
    u8 RC028[2]; //;�������״̬��2
    u8 RC029[2]; //;�������״̬��3
    u8 RC02A[2]; //;�������״̬��4
    u8 RC02B[2]; //;�������״̬��5
    u8 RC02C[2]; //;�������״̬��6
    u8 RC02D[2]; //;�������״̬��7

    u8 RC02E[1];//���¼�״̬
    
    u8 RC030[3],RC031[3],RC032[6];  //;�����,���
    u8 RC117[2],RC118[2],RC119[2];//;�Զ���������
    u8 RC313[1];//;������
    u8 RC141[6]; //;�궨��ѹ
    u8 RC142[6]; //;�궨���� ASCII
    u8 RC14A[10]; //;����ͺ�DTZ25 �������ߣ�DSZ25 ��������
    u8 RC040[3], RC041[3]; //;��ѹ���������ʣ���������������
    u8 RB710[3]; //;ʧѹ�ܴ���
    
    u8 RB218[3],RB219[6];  //;��β�Ǵ��������һ�δ�ʱ��by zrt 20130808    
    u8 RB220[3],RB221[6];  //;���ܱ�ʱ�β����޸Ĵ��������һ���޸�ʱ��by zrt20130814

    u8 RB222[3],RB223[6];  //;�ܵ�����������һ��ʱ��by zrt 20130828

    u8 RB224[3];                 //;����ʱ�α��̴���
    
    u8 End[1];
};

/*----------------------------------------------------------------------------*
 |  �¼���ؽṹ
 *----------------------------------------------------------------------------*/
//;void __ERC_struct(void) {}
struct ERCSMSCtrl     //;�¼����Ͷ��ſ�����
{
    u16 Pn;
    u8 ERCNo;        //;��ǰ���ڴ����¼��Ĵ���
    u8 Status;       //;�¼�����״̬
    u8 Receiver;
    u8 Index;        //;���͵��ڼ�������
    u8 PhoneNo[8];   //;��ǰ���͵��ֻ�����
    u8 Len;
};

struct ERCUpCtrl     //;�¼�������վ������
{
    u8 UpIndex;
    u8 SendERCNum;       //;����ERC����
    u8 Status;      //;�¼�����״̬
    u8 ReSedFlag;   //;�ط���־
    u8 SendNum;      //;���ʹ����ۼ�
    u8 WatiAns;      //;�ȴ�ʱ���ۼ�
    u8 PmStat;       //;����״̬
    u8 OVT;
};


struct ERCSaveCtrl   //;�¼����������
{
    u16  Pn;     //;�¼�����״̬
};

struct ERC1Data
{
    u8 Status[1];
    u8 OTime[5];
    u8 Mark[1];
    u8 Data[8];
};

struct ERC3Data
{
    u8 Status[1];
    u8 Num;           //;��ʶ����
    u8 OTime[5];      //;����ʱ��
    u8 HostAdr;       //;��վ��ַ
    u8 Data[8][4];    //;һ������¼8����Ԫ��ʶ
};

struct ERC8Data
{
    u8 Status[1];
    u8 Mark[1];      //;�쳣��־
};

struct ERC10Data    //;�����ַ����¼�
{
    u8 Status[4];    //;���෢���Ĵ���״̬  bit0:��ʾ�¼�����; bit1:��ʾ�¼��������; bit2��ʾ�¼��������; bit3��ʾ�¼����Ͷ������
    u8 Status_O;    //;�¼��ľ�״̬
    u8 Status_N;    //;�¼�����״̬
    u8 DXOM;        //;���෢������״̬
    u8 DXRM;        //;����ָ�����״̬
    u8 SYOM;        //;ʧѹ��������״̬
    u8 SYRM;        //;ʧѹ�ָ�����״̬
    u8 Data[20];    //;�¼�������
};

struct ERC13Data
{
    u8 Status[1];
    u8 Mark[1];
};


struct ERC14Data
{
    u8 Status[2];
    u8 PTOTime[5];  //;ͣ�緢��ʱ��
    u8 PLOTime[5];  //;���緢��ʱ��
};


struct ERC17Data
{
    u8 Status[2];
    u8 Status_O;
    u8 Status_N;
    u8 BPH_OM;
    u8 BPH_RM;
    u8 Data[19];
    u16 Upn;
    u16 Ipn;
};

struct ERC20Data
{
    u8 Status[1];
    u8 OTime[5];
    u8 PW[16];
    u8 MSA[1];
};

struct ERC21Data
{
    u8 Status[1];
    u8 State; //;Bit0 :Port1���ϣ�Bit1: Port2����
    u8 OTime[5];
    u8 Data[1];
};

struct ERC24Data
{
    u8 Status[4];
    u8 Status_O;
    u8 Status_N;
    u8 YSSXOM;   //;(bit0:2)Խ�������¼�������־Occur Mark;  (bit3:7) Խ�����޷����Ĵ����־
    u8 YSSXRM;   //;Խ������Recor Mark
    u8 YXXXOM;   //;Խ������Occur Mark
    u8 YXXXRM;   //;Խ������Recor Mark
    u8 Data[6];
};

struct ERC25Data
{
    u8 Status[4];
    u8 Status_O;
    u8 Status_N;
    u8 YSSXOM;   //;(bit0:2)Խ������Occur Mark;  (bit3:7) Խ�����޴����־
    u8 YSSXRM;   //;Խ������Recor Mark
    u8 YSXOM;    //;Խ����Occur   Mark
    u8 YSXRM;    //;Խ������Recor Mark
    u8 Data[9];
};


struct ERC26Data
{
    u8 Status[4];
    u8 Status_O;
    u8 Status_N;
    u8 Data[6];
};

struct ERC27Data
{
    u8 Status[1];
    u8 Data[10];
};

struct ERC29Data
{
    u8 Status[1];
    u8 Data[11];
};

struct ERCPnData            //;�������¼����ݽṹ
{
    u16 Pn;                  //;�������
    u8 OTime[5];            //;����ʱ��
    struct ERC8Data  ERC8;
    struct ERC10Data ERC10;
    struct ERC13Data ERC13;
    struct ERC17Data ERC17;
    struct ERC24Data ERC24;
    struct ERC25Data ERC25;
    struct ERC26Data ERC26;
    struct ERC27Data ERC27;
    struct ERC29Data ERC29;
};

struct ERCP0Data           //;�ն��¼����ݽṹ
{
    struct ERC1Data  ERC1;
    struct ERC3Data  ERC3;
    struct ERC14Data ERC14;
    struct ERC20Data ERC20;
    struct ERC21Data ERC21;
};


/*----------------------------------------------------------------------------*
 |  ������ؽṹ
 *----------------------------------------------------------------------------*/
//;void __Task_struct(void) {}
struct Task1_Time
{
    u8 Td1[5];      //;1����������ʱ��
    u8 Td2[5];      //;2����������ʱ��
    u8 Td3[5];      //;3����������ʱ��
    u8 Td4[5];      //;4����������ʱ�� 
};

struct Task2_Td
{
    u8 Td_S[5];    //;����ʱ��
    u8 Td_SO[5];    //;��һ�����͵��ն����ʱ��
    u8 Td_C[5];    //;��һ�����͵����߶����ʱ��
};

struct Task2_Time
{
    struct Task2_Td Td1; //;1����������ʱ��
    struct Task2_Td Td2; //;2����������ʱ��
    struct Task2_Td Td3; //;3����������ʱ��
    struct Task2_Td Td4; //;4����������ʱ�� 
};

struct Task_Control             //;������Ʋ���
{
    u8 Switch;                  //;���񿪹�
    u8 Status;                  //;������״̬
    u8 TaskTn;                  //;��ǰ�������������
    u8 TaskNO;                  //;��ǰ����������
    u8 T1Over[1];               //;��ǰ1����������ɱ�־
    u8 T2Over[1];               //;��ǰ2����������ɱ�־
    struct Task1_Time T1;       //;1����������ʱ��
    struct Task2_Time T2;       //;2����������ʱ��
    u8 OVT;                     //;������ʱ 
};


//;void __DJ_struct(void){}
struct DJ_Control //;������Ʋ���
{
    u8 Index;      //;���ڱ��涳������Ҫ���浽��һС��
    u8 StInd;      //;�������ݵ���ʼ˳���
};
struct QX_Control //;���߿��Ʋ���
{
    u16 Index;      //;���ڱ�����������Ҫ���浽��һС��
    u16 StInd;      //;�������ݵ���ʼ˳���
};

struct ERC_Control //;���߿��Ʋ���
{
    u16 Index;      //;���ڱ����¼�����Ҫ���浽��һС��
    u16 StInd;      //;�¼����ݵ���ʼ˳���
};

struct DJ_SAVE_CTRL
{
    u8  Status;
    u16 Pn;
};

/*----------------------------------------------------------------------------*
 |  ��Լ����Ŀ���
 *----------------------------------------------------------------------------*/
//;void __Ctrl_struct(void){}
struct GuiYueCtrl    //;��Լ����Ŀ���
{
    u8  RecOrd;      //;��ʾ��ǰ���յ���վ��������
    u16 Index_Answer;//;����ָʾ��ǰӦ����һ������
    u8  FSeq;        //;���ڴ洢��ǰ��֡���
    u8  ISeq;        //;����վ֡������PFC
    u8  UpLNum;      //;���ڼ�¼ͬһ��ͨ����������ͬһ������Ĵ���
    u8  WaitUpAnsT;  //;���ڵȴ���������Ӧ���ʱ���ʱ
    u8  TaskUL;      //;���ڱ�ʾ��ǰһ���������Ƿ��Ѿ�ͨ��ִ�й������������͵ĳ���
    u8  ERC_Index;   //;��ǰ���͵��ڼ����¼�
    u8  ERC_Num;     //;�¼�����
    u8  ERC_F;       //;��һ֡��֡��־first
    u8  ERC_L;       //;���һ֡��֡��־Last
    u8  TaskNum;     //;��ǰ��һ�������ڷ�֡�ϴ�
    u8  Status;
    u8  ListNo;
    u8  AFNList[7];
    u8  AFN02_FN;    //;
    u8  AFN10;
    u8  AFNNum;      //;��֡�������ܳ���10��
    u8  FileSign;    //;�����ļ���ʶ
    u16 FrameIndex;  //;�ѽ���֡���
    u32  FileSize;    //;�ļ���С
    u32  PageCur;  //;��ǰд��ҳƫ��
    u32  SizeCur;  //;��ǰ�����ֽ���
    
};

struct Relay_Control  //;�м̿��Ʋ���
{
    u8   Status; //;�յ��м�����ı�־ idle/busy
    u8   Channel;   //;right / wrong
    u8   A3;     //;��վ��ַ�����ַ
    u8   SEQ;    //;֡������
    u8   Port;
    u8   OvTime;
    u8   BPS;
    u8   Len;
    char Data[256];
    u8   Tp[6];   //20131106
};

struct ZDCBCtrl
{
    u8   Status;        //;����
    u8   Mark;          //;��������
    u16  CouDown;       //;����ʱ
};

struct PortCtrl
{
   u8  Status;        //;�˿ڷ���״̬
   u8  Mark;          //;�˿ڴ�������
   u16  Index;         //;��ǰ�����ڼ����
   u16 PnOver;        //;
   u8  Over;
   u8  MeterNum[6];   //;��ǰ���ڲɼ��ı��
   u8  GYType;        //;���ù�Լ����
   u8  BPS;            //;������
   u16 IndSend;
   u16 IndSave;
   u32 DI1DI0;   //;��ǰ���ڳ���ı�ʶ
   u8  WaitAns;
   u8  ReSNum;       //;�ط�����
   u8  ErrNum;       //;
   u8  PnErrNum;
   char *Buffer;     //;�˿ڷ�����������ݵĻ�������ָ��
   char Len;         //;Ҫ�������ݵĳ���
};

struct ControlOrder //;��բ��բ�������ݽṹ
{
    u8 IndSave;
    u8 IndSend;
    u8 Status;
    u8 Port;
    u8 BPS;
    u8 ErrNum;
    u8 MeterNum[6];
    u32 DI[SBNUM];
    u8 Pn[SBNUM];
    u8 Order[SBNUM];
};

struct GprsToMeter  //;gprsģ�������ͨѶ����
{
	u16 Order[10];  
	u8  IndSave;    
	u8  IndSend;    
	u8  Status;     
	u8  WaitAns;	  
	u8  ErrorCount;    
	u8  IgnoreCount;   
	u8  RightCount;    
  	u8  SendFlag;  
  	u8  RMeter_Enable;
    u8  Bao1,Bao2, Bao3;
    u16 Bao4;
};






#endif /*__GDW130_DataType_h */
