/*
********************************************************************************
*  �� �� ����GDW376_CFG.C
*
*  �ļ�������376.1��Լ����ȫ�ֱ���
*
*  ����оƬ��
*
*  �� �� �ˣ�
*
*  �� �� �ţ�
*
*  �޸ļ�¼��
********************************************************************************
*/
#include "GDW376_CFG.h"
#include "GDW376_MMU.h"

u8 TimeChgFlag;
u8 F49[1];
struct ZDPStr ZDP;         //;�ն˲���
struct ZDStatStr ZDS;      //;�ն�״̬����
struct ZDPStrKZ ZDPKZ;     //;�ն���չ���� 
struct GroupStr GRPP;      //;�ܼ������
struct DCPortStr DCPP;     //;ֱ��ģ��������
struct TaskPStr TaskP;     //;�������ò���
struct MeterData PMD1, PMD2;  //;��������
struct PoPStr P1P;//;P2P,P3P,P4P,P5P,P6P,P7P,P8P,P9P,P10P;//;���������                 
struct PointDataStr P1D;//;P2D,P3D,P4D,P5D,P6D,P7D,P8D,P9D,P10D; //;����������                                                                                               

struct DJ_Control P1DDJ;//;P2DDJ,P3DDJ,P4DDJ,P5DDJ,P6DDJ,P7DDJ,P8DDJ,P9DDJ,P10DDJ;    
struct DJ_Control P1BDJ;//;P2BDJ,P3BDJ,P4BDJ,P5BDJ,P6BDJ,P7BDJ,P8BDJ,P9BDJ,P10BDJ;    
struct DJ_Control P1MDJ;//;P2MDJ,P3MDJ,P4MDJ,P5MDJ,P6MDJ,P7MDJ,P8MDJ,P9MDJ,P10MDJ;    
struct QX_Control P1QDJ;//;P2QDJ,P3QDJ,P4QDJ,P5QDJ,P6QDJ,P7QDJ,P8QDJ,P9QDJ,P10QDJ;   
struct DJ_SAVE_CTRL DDJPn,  BDJPn, MDJPn, QDJPn,DDLDJPn,MDLDJPn;  //;������Ʋ���                                                                                  

u8 OldZDP[54];
struct ZDCBCtrl     ZDCB; //;�ն˳�����Ʋ���
struct PortCtrl     Port1,  Port2;
struct ControlOrder CtrlOrder;   //;��բ��բ���Ʋ���
struct GuiYueCtrl   LocGyC, GprsGyC, InfGyC;
struct Relay_Control RelayC;       //;�м̿��Ʋ���
struct Task_Control TaskC;         //;������Ʋ���
u8 SofewareOldVer[4]; //;���ǰ������İ汾��
u32 FluxLimValue; //;��������ֵ

u8 ERCIMP_Pm[32][4];
struct ERC_Control  ERC1,ERC2; //;�¼�����
struct ERCUpCtrl   ERCUp; //;�¼����Ϳ���
struct ERCSMSCtrl  SMS;      //;���ſ���                                                        
struct ERCSaveCtrl ERCSave;     //;�¼��������                                                    
struct ERCP0Data   ERCP0; //;�ն��¼�����
struct ERCPnData   ERCP1;//;ERCP2,ERCP3,ERCP4,ERCP5,ERCP6,ERCP7,ERCP8,ERCP9,ERCP10; //;����������
struct AlarmHVM_Control AlarmHVM; //;��ѹ��澯����
struct GprsToMeter  G2M;//;��ѹ�������

B4 P1DTAddr[] = {\
(u32)(&P1P),(u32)(&P1D),(u32)(&ERCP1),(u32)(&P1DDJ),(u32)(&P1BDJ),(u32)(&P1MDJ),(u32)(&P1QDJ),\
P1DDJHome,P1BDJHome,P1MDJHome,P1QDJHome,P1NHDJHome,P1OHDJHome,P1MVTHome,P1PEZHome,\
E2R_P1DDJ, E2R_P1BDJ, E2R_P1MDJ, E2R_P1QDJ};

//;B4 P2DTAddr[] = {\
//;(u32)(&P2P),(u32)(&P2D),(u32)(&ERCP2),(u32)(&P2DDJ),(u32)(&P2BDJ),(u32)(&P2MDJ),(u32)(&P2QDJ),\
//;P2DDJHome,P2BDJHome,P2MDJHome,P2QDJHome,P2NHDJHome,P2OHDJHome,P2MVTHome,P2PEZHome,\
//;E2R_P2DDJ,E2R_P2BDJ,E2R_P2MDJ,E2R_P2QDJ};
//;
//;B4 P3DTAddr[] = {\
//;(u32)(&P3P),(u32)(&P3D),(u32)(&ERCP3),(u32)(&P3DDJ),(u32)(&P3BDJ),(u32)(&P3MDJ),(u32)(&P3QDJ),\
//;P3DDJHome,P3BDJHome,P3MDJHome,P3QDJHome,P3NHDJHome,P3OHDJHome,P3MVTHome,P3PEZHome,\
//;E2R_P3DDJ,E2R_P3BDJ,E2R_P3MDJ,E2R_P3QDJ};
//;
//;B4 P4DTAddr[] = {\
//;(u32)(&P4P),(u32)(&P4D),(u32)(&ERCP4),(u32)(&P4DDJ),(u32)(&P4BDJ),(u32)(&P4MDJ),(u32)(&P4QDJ),\
//;P4DDJHome,P4BDJHome,P4MDJHome,P4QDJHome,P4NHDJHome,P4OHDJHome,P4MVTHome,P4PEZHome,\
//;E2R_P4DDJ,E2R_P4BDJ,E2R_P4MDJ,E2R_P4QDJ};
//;
//;B4 P5DTAddr[] = {\
//;(u32)(&P5P),(u32)(&P5D),(u32)(&ERCP5),(u32)(&P5DDJ),(u32)(&P5BDJ),(u32)(&P5MDJ),(u32)(&P5QDJ),\
//;P5DDJHome,P5BDJHome,P5MDJHome,P5QDJHome,P5NHDJHome,P5OHDJHome,P5MVTHome,P5PEZHome,\
//;E2R_P5DDJ,E2R_P5BDJ,E2R_P5MDJ,E2R_P5QDJ};
//;
//;B4 P6DTAddr[] = {\
//;(u32)(&P6P),(u32)(&P6D),(u32)(&ERCP6),(u32)(&P6DDJ),(u32)(&P6BDJ),(u32)(&P6MDJ),(u32)(&P6QDJ),\
//;P6DDJHome,P6BDJHome,P6MDJHome,P6QDJHome,P6NHDJHome,P6OHDJHome,P6MVTHome,P6PEZHome,\
//;E2R_P6DDJ,E2R_P6BDJ,E2R_P6MDJ,E2R_P6QDJ};
//;
//;B4 P7DTAddr[] = {\
//;(u32)(&P7P),(u32)(&P7D),(u32)(&ERCP7),(u32)(&P7DDJ),(u32)(&P7BDJ),(u32)(&P7MDJ),(u32)(&P7QDJ),\
//;P7DDJHome,P7BDJHome,P7MDJHome,P7QDJHome,P7NHDJHome,P7OHDJHome,P7MVTHome,P7PEZHome,\
//;E2R_P7DDJ,E2R_P7BDJ,E2R_P7MDJ,E2R_P7QDJ};
//;
//;B4 P8DTAddr[] = {\
//;(u32)(&P8P),(u32)(&P8D),(u32)(&ERCP8),(u32)(&P8DDJ),(u32)(&P8BDJ),(u32)(&P8MDJ),(u32)(&P8QDJ),\
//;P8DDJHome,P8BDJHome,P8MDJHome,P8QDJHome,P8NHDJHome,P8OHDJHome,P8MVTHome,P8PEZHome,\
//;E2R_P8DDJ,E2R_P8BDJ,E2R_P8MDJ,E2R_P8QDJ};
//;
//;B4 P9DTAddr[] = {\
//;(u32)(&P9P) ,(u32)(&P9D),(u32)(&ERCP9),(u32)(&P9DDJ),(u32)(&P9BDJ),(u32)(&P9MDJ),(u32)(&P9QDJ),\
//;P9DDJHome,P9BDJHome,P9MDJHome,P9QDJHome,P9NHDJHome,P9OHDJHome,P9MVTHome,P9PEZHome,\
//;E2R_P9DDJ,E2R_P9BDJ,E2R_P9MDJ,E2R_P9QDJ};
//;
//;B4 P10DTAddr[] = {\
//;(u32)(&P10P),(u32)(&P10D),(u32)(&ERCP10),(u32)(&P10DDJ),(u32)(&P10BDJ),(u32)(&P10MDJ),(u32)(&P10QDJ),\
//;P10DDJHome,P10BDJHome,P10MDJHome,P10QDJHome,P10NHDJHome,P10OHDJHome,P10MVTHome,P10PEZHome,\
//;E2R_P10DDJ,E2R_P10BDJ,E2R_P10MDJ,E2R_P10QDJ};

/*���и���������ص�DataType��ŵĵ�ַ*/
B4 PnDTAddr[] ={   
(u32)P1DTAddr};
//;(u32)P2DTAddr,\
//;(u32)P3DTAddr,\
//;(u32)P4DTAddr,\
//;(u32)P5DTAddr,\
//;(u32)P6DTAddr,\
//;(u32)P7DTAddr,\
//;(u32)P8DTAddr,\
//;(u32)P9DTAddr,\
//;(u32)P10DTAddr,\
//;};


           

