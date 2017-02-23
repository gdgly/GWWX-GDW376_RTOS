/*
********************************************************************************
*  �� �� ����GDW376_FUN.c   
*
*  �ļ�������GDW376��ع��ܺ��� 
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


/*------------------------------------------------------------------------------
||  F1~F10��ʼ������__CS1
------------------------------------------------------------------------------*/
B1 InP0F1[ 6]  = {50,2,20,0x30,0,0x05};
/*B1 InP0F3[28]  = {112,91 ,151,  2, 0x63, 0x1B,
                 192,168,1  ,  6, 0xe1, 0x10,
                 'C','M','N','E',  'T', 0   ,
                 0  ,0  ,0  ,  0, 0   , 0   ,0, 0, 0, 0}; */
                 
B1 InP0F3[28]  = {112,91 ,151,  2, 0xB1, 0x1F,
                 192,168,1  ,  6, 0xe1, 0x10,
                 'C','M','N','E',  'T', 0   ,
                 0  ,0  ,0  ,  0, 0   , 0   ,0, 0, 0, 0};                 
//B1 InP0F8[8 ]  = {0x13,0x84,0x03,0x03,0x0A,0xFF,0xFF,0xFF};       by zrt 20130822 
B1 InP0F8[8 ]  = {0x11,0x78,0x00,0x03,0x0A,0xFF,0xFF,0xFF}; 
/*B1 InP0F9[16]  = {0x0FF, 0x0FF, 0x0FF, 0x0FF, 0x0FF, 0x0FF, 0x0FF, 0x0FF,
               0x0FF, 0x0FF, 0x0FF, 0x0FF, 0x0FF, 0x0FF, 0x0FF, 0x0FF};
*/
B1 InP0F9[16]  = {0x85, 0x32, 0x99, 0x17, 0x00, 0x00, 0x00, 0x00,
               0x85, 0x32, 0x99, 0x17, 0x00, 0x00, 0x00, 0x00};

/*B1 InP0F10[272]= {0x01, 0x00,
                  1,0, 1,0, 1, 30, 0,0,0,0,0,0, 0,0,0,0,0,0, 4, 0x09, 0,0,0,0,0,0, 0x11,
                  2,0, 2,0, 1, 1, 0,0,0,0,0,0, 0,0,0,0,0,0, 4, 0x09, 0,0,0,0,0,0, 0x11,
                  3,0, 3,0, 1, 1, 0,0,0,0,0,0, 0,0,0,0,0,0, 4, 0x09, 0,0,0,0,0,0, 0x11,
                  4,0, 4,0, 1, 1, 0,0,0,0,0,0, 0,0,0,0,0,0, 4, 0x09, 0,0,0,0,0,0, 0x11,
                  5,0, 5,0, 1, 1, 0,0,0,0,0,0, 0,0,0,0,0,0, 4, 0x09, 0,0,0,0,0,0, 0x11,
                  6,0, 6,0, 1, 1, 0,0,0,0,0,0, 0,0,0,0,0,0, 4, 0x09, 0,0,0,0,0,0, 0x11,
                  7,0, 7,0, 1, 1, 0,0,0,0,0,0, 0,0,0,0,0,0, 4, 0x09, 0,0,0,0,0,0, 0x11,
                  8,0, 8,0, 1, 1, 0,0,0,0,0,0, 0,0,0,0,0,0, 4, 0x09, 0,0,0,0,0,0, 0x11,
                  9,0, 9,0, 1, 1, 0,0,0,0,0,0, 0,0,0,0,0,0, 4, 0x09, 0,0,0,0,0,0, 0x11,
                 10,0,10,0, 1, 1, 0,0,0,0,0,0, 0,0,0,0,0,0, 4, 0x09, 0,0,0,0,0,0, 0x11};
*/
B1 InP0F10[29]= {0x01, 0x00,
                  1,0, 1,0, 1, 30, 0,0,0,0,0,0, 0,0,0,0,0,0, 4, 0x09, 0,0,0,0,0,0, 0x11}; //;--20141226

/*------------------------------------------------------------------------------
||  �ն˲���F33~F39��F57~F61�ĳ�ʼ������
------------------------------------------------------------------------------*/
/*
B1 InP0F33[61]={2, 
                0x01, 0x00,0x00, 0x00,0x00,0x00,0x08, 0x00,0x00, 0x03, 0x00,0x00,0x01,
                0x02,
                0x00,0x07,0x00,0x12, 0x00,0x14,0x00,0x18, 0x00,0x00,0x00,0x00, 
                0x00,0x00,0x00,0x00,
                0x01, 0x00,0x00, 0x00,0x00,0x00,0x08, 0x00,0x00, 0x03, 0x00,0x00,0x01,
                0x02,
                0x00,0x07,0x00,0x12, 0x00,0x14,0x00,0x18, 0x00,0x00,0x00,0x00, 
                0x00,0x00,0x00,0x00};
*/
B1 InP0F33[61]={2, 
                0x01, 0x00,0x00, 0x01,0x00,0x00,0x00, 0x00,0x00, 0x03, 0x00,0x00,0x01,
                0x00,
                0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 
                0x00,0x00,0x00,0x00,
                0x01, 0x00,0x00, 0x01,0x00,0x00,0x00, 0x00,0x00, 0x03, 0x00,0x00,0x01,
                0x00,
                0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 
                0x00,0x00,0x00,0x00};
B1 InP0F57[3]={0,0,0};
B1 InP0F59[4]={0,0,0,0};

/*------------------------------------------------------------------------------
||  ���������F25~F31��F73~F76�ĳ�ʼ������
------------------------------------------------------------------------------*/
/*
B1 InP1F25[ 11]={0x64,0,  0x0A,0, 0x10,0x00, 0x40, 0,0,0x60, 1};


B1 InP1F26[57]={0x00,0x12, 0x00,0x08, 0x00,0x08,
                0x00,0x13, 0x02, 0x00,0x08,
                0x00,0x06, 0x02, 0x00,0x08,
                0x00,0x00,0x10, 0x02, 0x00,0x08,
                0x00,0x00,0x08, 0x02, 0x00,0x08,
                0x00,0x00,0x02, 0x02, 0x00,0x08,
                0x00,0x00,0x80, 0x02, 0x00,0x08,
                0x00,0x00,0x70, 0x02, 0x00,0x08,
                0x00,0x05, 0x02, 0x00,0x08,
                0x00,0x05, 0x02, 0x00,0x08,
                0x04};
*/

B1 InP1F25[ 11]={0x01 , 0 , 0x01 , 0 , 0x00 , 0x22 , 0x15 , 0x00 , 0x99 , 0x00 , 0x02 };

B1 InP1F26[57]={
0x20,  0x24,  0x80 , 0x19 , 0x15 , 0x17 , 
0x40 , 0x26 , 0x05 , 0x00 , 0x09 , 
0x60 , 0x17 , 0x05 , 0x00 , 0x09 , 
0x50 , 0x22 , 0x00 , 0x05 , 0x00 , 0x09, 
0x00 , 0x15 , 0x00 , 0x05 , 0x00 , 0x09 , 
0x75 , 0x03 , 0x00 , 0x05 , 0x00 , 0x09 , 
0x70 , 0x28 , 0x01 , 0x05 , 0x00 , 0x09 , 
0x00 , 0x99 , 0x00 , 0x05 , 0x00 , 0x09 , 
0x00 , 0x02 , 0x05 , 0x00 , 0x09 , 
0x00 , 0x02 , 0x05 , 0x00 , 0x09 , 
0x05 };

B1 InP1F28[ 4]={0x00,0x08,0x00,0x09};



/*��չ��12��ʼ��@1*/
B1 InP0F89[ 4]={0x01,0x11,0x01,0x00};
B1 InP0F90[ 3]={0x05,0x05,01};

B1 InP1F97[16]={
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};


//;�ն�״̬����
B1 ZDSF3[31] = {0xFF, 0xFF, 0x7F, 0x7F, 0xFF, 0xFF, 0x01, 0x1F, 0x0F, 0x0F,
                0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00
               };

B1 ZDSF4[1] = {0x05};   

/*����ͷβ����*/
B2 User_Top[10] = {0x5C0A,0x656C,0x7684,0x7528,0x6237,0xFF1A}; //;�𾴵��û���
B2 User_End[10] = {0x8BF7,0x60A8,0x53CA,0x65F6,0x5904,0x7406}; //;������ʱ�鿴�봦��
B2 Admi_Top[10] = {0x5C0A,0x656C,0x7684,0x7BA1,0x7406,0x5458,0xFF1A}; //;�𾴵Ĺ���Ա��
B2 Admi_End[10] = {0x8BF7,0x60A8,0x53CA,0x65F6,0x5904,0x7406}; //;������ʱ�鿴�봦��

/*������x��x = 1 ~ 10*/
B2 Point_Name[10][10] = {\
{0x6D4B,0x91CF,0x70B9,0x0031}, //;������1 
{0x6D4B,0x91CF,0x70B9,0x0032}, //;������2
{0x6D4B,0x91CF,0x70B9,0x0033}, //;������3
{0x6D4B,0x91CF,0x70B9,0x0034}, //;������4
{0x6D4B,0x91CF,0x70B9,0x0035}, //;������5
{0x6D4B,0x91CF,0x70B9,0x0036}, //;������6
{0x6D4B,0x91CF,0x70B9,0x0037}, //;������7
{0x6D4B,0x91CF,0x70B9,0x0038}, //;������8
{0x6D4B,0x91CF,0x70B9,0x0039}, //;������9
{0x6D4B,0x91CF,0x70B9,0x0031,0x0030},//;������10
};

B1 DaysOfMonth[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};


/* 
********************************************************************************
* �� �� ����ZD_RAM_MAK
* ��    �ܣ�д��Ram�������ı�־�����ڼ��Ram����
* ��    ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void ZD_RAM_MAK(void)
{
    EW.Start[0]  = 'S';     
    EW.End[0] = 'E';
    
    AllBuf.Start[0] = 'S';  
    AllBuf.End[0] = 'E';
    
    AllBuf.St_Spi[0] = 'S'; 
    AllBuf.En_Spi[0] = 'E';
    
    PMD1.Start[0] = 'S';    
    PMD1.End[0] = 'E';
    
//;    PMD2.Start[0] = 'S';    
//;    PMD2.End[0] = 'E';
    
    P1D.Start[0] = 'S';     
    P1D.End[0] = 'E';
    
//;    P2D.Start[0] = 'S';     
//;    P2D.End[0] = 'E';
//;    
//;    P3D.Start[0] = 'S';     
//;    P3D.End[0] = 'E';
//;    
//;    P4D.Start[0] = 'S';     
//;    P4D.End[0] = 'E';
//;    
//;    P5D.Start[0] = 'S';     
//;    P5D.End[0] = 'E';
//;    
//;    P6D.Start[0] = 'S';     
//;    P6D.End[0] = 'E';
//;    
//;    P7D.Start[0] = 'S';     
//;    P7D.End[0] = 'E';
//;    
//;    P8D.Start[0] = 'S';     
//;    P8D.End[0] = 'E';
//;    
//;    P9D.Start[0] = 'S';     
//;    P9D.End[0] = 'E';
//;    
//;    P10D.Start[0] = 'S';    
//;    P10D.End[0] = 'E';
}


/* 
********************************************************************************
* �� �� ����ZD_RAM_CHK
* ��    �ܣ���RAM��־������λ
* ��    ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void ZD_RAM_CHK(void)
{
    u8 Error = false;
    
    if((EW.Start[0] != 'S') || (EW.End[0] != 'E')) 
    {
        Error = true;
    }
    if((P1D.Start[0]!= 'S') || (P1D.End[0]!= 'E'))  
    {
        Error = true;
    }
//;    if ((P2D.Start[0]!= 'S') || (P2D.End[0]!= 'E')) 
//;    {
//;        Error = true;
//;    }
//;    if ((P3D.Start[0]!= 'S') || (P3D.End[0]!= 'E'))  
//;    {
//;        Error = true;
//;    }
//;    if ((P4D.Start[0]!= 'S') || (P4D.End[0]!= 'E')) 
//;    {
//;        Error = true;
//;    }
//;    if ((P5D.Start[0]!= 'S') || (P5D.End[0]!= 'E')) 
//;    {
//;        Error = true;
//;    }
//;    if ((P6D.Start[0]!= 'S') || (P6D.End[0]!= 'E')) 
//;    {
//;        Error = true;
//;    }
//;    if ((P7D.Start[0]!= 'S') || (P7D.End[0]!= 'E'))  
//;    {
//;        Error = true;
//;    }
//;    if ((P8D.Start[0]!= 'S') || (P8D.End[0]!= 'E')) 
//;    {
//;        Error = true;
//;    }
//;    if ((P9D.Start[0]!= 'S') || (P9D.End[0]!= 'E'))  
//;    {
//;        Error = true;
//;    }
//;    if ((P10D.Start[0]!= 'S')|| (P10D.End[0]!='E')) 
//;    {
//;        Error = true;
//;    }
    if((AllBuf.Start[0]!='S') || (AllBuf.End[0] != 'E')) 
    { 
        Error = true;
    }
    if((AllBuf.St_Spi[0] != 'S') || (AllBuf.En_Spi[0] != 'E')) 
    {
        Error = true;
    }
    
    if(Error == true)
    {
        HWareC.EnRes[0] = 'E';
        HWareC.EnRes[1] = 'R';
    }
}


/* 
********************************************************************************
* �� �� ����ZD_HardWareReset
* ��    �ܣ�Ӳ����λ
* ��    ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void ZD_HardWareReset(void)
{
     HWareC.EnRes[0] ='E';  //;Ӳ����ʼ����λ
     HWareC.EnRes[1] ='R';
}


/* 
********************************************************************************
* �� �� ����ZD_HWareRes_CHK
* ��    �ܣ�����ն��Ƿ�ҪӲ����λ
* ��    ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*
void ZD_HWareRes_CHK(void) 
{
    ZD_RAM_CHK();
    if((HWareC.EnRes[0] == 'E') || (HWareC.EnRes[1] == 'R')) //;�յ���λ����
    {   
        HWareC.EnRes[0] = 0; 
        HWareC.EnRes[1] = 0;
        while(1)
        {
             ; //;�ȴ����Ź���λ
        }
    }
}
*/
void ZD_HWareRes_CHK(void) 
{
	ZD_RAM_CHK();
	//;Զ�̷��͸�λ����ն˷���Ӧ��֡ ---20141224
	/*if((!GprsGyC.Status) &&(HWareC.EnRes[0] == 'E') && (HWareC.EnRes[1] == 'R')) //;�յ���λ����
	{   
		HWareC.EnRes[0] = 0; 
		HWareC.EnRes[1] = 0;
		while(1)
		{
			; //;�ȴ����Ź���λ
		}
	}
	*/
	//;20141225 ���͸�λ֡����Ӧ��֡��Ӳ����ʼ��
	if((HWareC.EnRes[0] == 'E') && (HWareC.EnRes[1] == 'R')\
		  &&(!GprsGyC.Status)&&(!GPRSUSARTCTR.SedOrd)\
    	&&(!LocGyC.Status)&&(!InfGyC.Status)\
    	&&(!INFUSARTCTR.SedOrd)&&(!LOCUSARTCTR.SedOrd)\
    	) //;�յ���λ����
    {   
        HWareC.EnRes[0] = 0; 
        HWareC.EnRes[1] = 0;
        while(1)
        {
             ; //;�ȴ����Ź���λ
        }
    }
	if(HWareC.FactoryInit == 'E')  //;�ָ���������
	{
		HWareC.FactoryInit = 'S';
		GprsC.ReConnect = true;
	}
	else if (HWareC.FactoryInit == 'S')
	{
		if((!GprsGyC.Status)&&(!GprsC.ReConnect)&&(EW.CSB1 == 0)\
			&&(!GPRSUSARTCTR.SedOrd)&&(EW.CSB2 == 0)\
			&& (EW.CSB3 == 0)&& (EW.CSB4 == 0) && (EW.CSB5 == 0)\
			&& (EW.CSB6 == 0)&& (EW.CSB7 == 0) && (EW.CSB8 == 0)) //;������Ӧ���ĺͱ��������
		{
			HWareC.FactoryInit = 0;//;
			while(1)
			{
				; //;�ȴ����Ź���λ 
			}
		}
	}
	
	if((GprsC.Re_Connect_states == 'E')\
		   &&(!GprsGyC.Status)&&(!GPRSUSARTCTR.SedOrd)\
		  &&(!LocGyC.Status)&&(!InfGyC.Status)\
    	&&(!INFUSARTCTR.SedOrd)&&(!LOCUSARTCTR.SedOrd))
	{
		GprsC.Re_Connect_states = 0;
		ERCUp.Status = 0;         //;�ر��¼�����
		GprsC.ReConnect = true;   //;�ȴ�GPRS��������ͨ�ŷ�������ٽ�����������GPRS
	}
}


/* 
********************************************************************************
* �� �� ����CalcCRC
* ��    �ܣ�CRCѭ����������֤
* ��    ����InData-����֤�����ݵĵ�ַ��InPW-���ӵ���Ϣ���ַ
* ��    �أ�true-��֤�ɹ�����Ҫ��֤��false-��֤ʧ��
* ˵    �����㷨�Դ�֡��ʽ������C��ʼ����Ӧ�ò��һ�����ݵ�Ԫ��ʶDT2 ������
*           ��12���ֽڽ�����֤��
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
u8 CalcCRC(char *InData, char *InPW)
{
    u8 i, j, data, Plan;
    u16 CRC16, PW, CRC1 = 0;

    Plan = ZDP.F5.Code;  //;��֤����
    if(Plan == 0)
    {
        return true;     //;����֤
    }
    else if(Plan == 1)   //;����1
    {
        CRC16 = ZDP.F5.Para[1];  //;��Կ
        CRC16 <<= 8;
        CRC16 += ZDP.F5.Para[0];
        PW = *(InPW + 1);
        PW <<= 8;
        PW += *(InPW + 0);
        for(i = 0; i < 12; i++)
        {
            data = *(InData + i);

            for(j = 0; j < 8; j++)
            {
                if((data ^ CRC1) & 0x0001)
                {
                    CRC1 = ( CRC1 >> 1 ) ^ CRC16;
                }
                else
                {
                    CRC1 >>= 1;
                }
                data >>= 1;
            }
        }
        if(CRC1 == PW)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}


/* 
********************************************************************************
* �� �� ����ZD_Pn_Find
* ��    �ܣ������ն˲�������Ƿ����
* ��    ����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��2011-09-08 09:33:10
********************************************************************************
*/
s16 ZD_Pn_Find(u16 Pn)
{
    u16 i;
    u16 temp;

    if(Pn == 0) 
    {
        return 0;
    }
    
    for(i=0; i<PNMAXNUM; i++)
    {
        temp = *((u16*)ZDP.F10.Meter[i].Point); //;F10���ܱ����i�Ĳ�����ַ
        if(temp == Pn)
        {
            return i;
        }
    }
	return -1;   //;�����㳭����Ҳ�����ݣ��޸Ĵ�ָ��ֻ֧�ֶ�Ӧ����������20141225
//    return 0xFFFF;
}


/* 
********************************************************************************
* �� �� ������ʼ���ն�״̬
* ��    �ܣ�ZD_Stat_Init
* ��    ����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��2011-09-16 16:14:41 ����GPRS������ʼ��
*           2011-09-20 17:29:54  ����ZDS������ʼ��
********************************************************************************
*/
void ZD_Stat_Init(void)
{   
    GprsLC.CHKLink = true; //;������ר��������ʽΪʱ�����ߣ����ϵ��������ߡ�
    
    EW.MoveByte = 'M';
    MovNByte(6, ZDS.F3, (u8*)ZDSF3);
    MovNByte(1, ZDS.F4, (u8*)ZDSF4);
    
    ByteN_Fill(50, &ZDS.F5.InSign, 0x00);
    ByteN_Fill(64, &ZDS.F6.TelCutStat, 0x00);
    ByteN_Fill(8, ZDS.F8, 0x00);
    ByteN_Fill(2, ZDS.F9, 0x00);
    ByteN_Fill(37, &ZDS.F11.PortN, 0x00);
    
    EW.MoveByte = 0;   
    FluxLimValue = *((u32*)ZDP.F36);
    if(0x00 == FluxLimValue)
    {
        FluxLimValue = 0xFFFFFFFF;  //;���������ƣ�������ֵ
    }
   
    if((SofewareOldVer[0] == 0) && (SofewareOldVer[1] == 0) && 
       (SofewareOldVer[2] == 0) && (SofewareOldVer[3] == 0))
    {
        SofewareOldVer[0] = ZDCFG_F1[12];
        SofewareOldVer[1] = ZDCFG_F1[13];
        SofewareOldVer[2] = ZDCFG_F1[14];
        SofewareOldVer[3] = ZDCFG_F1[15];
        SaveData_FM24(4, Version, Version + E2R_ByteNum, (u8*)ZDCFG_F1 + 12);
    }

    else
    {
        if((SofewareOldVer[0] != ZDCFG_F1[12]) || 
           (SofewareOldVer[1] != ZDCFG_F1[13]) || 
           (SofewareOldVer[2] != ZDCFG_F1[14]) || 
           (SofewareOldVer[3] != ZDCFG_F1[15]))
        {
            SaveData_FM24(4, Version, Version + E2R_ByteNum, (u8*)ZDCFG_F1 + 12);
            ERC1_Judge(2); //;����汾�ű��
        }
    }
}


/* 
********************************************************************************
* �� �� ����Pn_DataInit
* ��    �ܣ���ʼ��������������
* ��    ����Pn-������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void Pn_DataInit(u8 Pn)
{
    u16 E2R_PnDDJ, E2R_PnBDJ, E2R_PnMDJ, E2R_PnQDJ;
    u32 *Addr32;
    struct DJ_Control *PnDDJ,*PnBDJ, *PnMDJ;
    struct QX_Control *PnQDJ;
    struct PointDataStr* PnD;
    
    Addr32 = (u32*)(*(PnDTAddr + Pn));
    PnD = (struct PointDataStr*)(*(Addr32 + _PnD));
    
    ByteN_Fill(PNDSIZE, PnD->CB_Time, 0x00);  //;�������������
    
    PnDDJ = (struct DJ_Control*)(*(Addr32 + _PnDDJ));
    PnBDJ = (struct DJ_Control*)(*(Addr32 + _PnBDJ));
    PnMDJ = (struct DJ_Control*)(*(Addr32 + _PnMDJ));
    PnQDJ = (struct QX_Control*)(*(Addr32 + _PnQDJ));
    E2R_PnDDJ = (u16)(*(Addr32 + _E2R_PnDDJ));
    E2R_PnBDJ = (u16)(*(Addr32 + _E2R_PnBDJ));
    E2R_PnMDJ = (u16)(*(Addr32 + _E2R_PnMDJ));
    E2R_PnQDJ = (u16)(*(Addr32 + _E2R_PnQDJ));
    PnDDJ->Index = 0;  PnDDJ->StInd = 0;   //;�ն������ݽ��ʼ��
    PnBDJ->Index = 0;  PnBDJ->StInd = 0;   //;�����ն������ݽ��ʼ��
    PnMDJ->Index = 0;  PnMDJ->StInd = 0;   //;�¶������ݽ��ʼ��
    PnQDJ->Index = 0;  PnQDJ->StInd = 0;   //;���߶������ݽ��ʼ��
    SaveData_FM24(1, E2R_PnDDJ, E2R_PnDDJ + E2R_ByteNum, &PnDDJ->Index);
    SaveData_FM24(2, E2R_PnBDJ, E2R_PnBDJ + E2R_ByteNum, &PnBDJ->Index);
    SaveData_FM24(2, E2R_PnMDJ, E2R_PnMDJ + E2R_ByteNum, &PnMDJ->Index);
    SaveData_FM24(4, E2R_PnQDJ, E2R_PnQDJ + E2R_ByteNum, (u8*)(&PnQDJ->Index));
}


/* 
********************************************************************************
* �� �� ����ZD_DatArea_Init
* ��    �ܣ���ʼ���ն�������
* ��    ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��2011-09-20 18:37:15
********************************************************************************
*/
void ZD_DataInit(void)
{
    u8 i;

    ERC1.Index = 0;  //;��Ҫ�¼����ݽ��ʼ��
    ERC1.StInd = 0;  
    ERC2.Index = 0;  //;һ���¼����ݽ��ʼ��
    ERC2.StInd = 0;  
    ZDS.F7[0] = 0;   
    ZDS.F7[1] = ERCUp.UpIndex = 0;
    ZDS.F10.Flux_Day = 0x00000000;
    ZDS.F10.Flux_Month = 0x00000000;
    SaveData_FM24(4, E2R_ERC1, E2R_ERC1 + E2R_ByteNum, (u8*)(&ERC1.Index));
    SaveData_FM24(4, E2R_ERC2, E2R_ERC2 + E2R_ByteNum, (u8*)(&ERC2.Index));
    SaveData_FM24(1, E2R_EC1, E2R_EC1 + E2R_ByteNum, ZDS.F7); //;�����¼�������
    SaveData_FM24(1, E2R_EC2, E2R_EC2 + E2R_ByteNum, ZDS.F7+1); //;�����¼�������
    SaveData_FM24(1, E2R_ERCUP, E2R_ERCUP + E2R_ByteNum, (u8*)&ERCUp.UpIndex); //;�����¼�������
    
    for(i = 0; i < PNMAXNUM; i++)
    {
        Pn_DataInit(i);
    }
}


/* 
********************************************************************************
* �� �� ����ZD_CS1_Init
* ��    �ܣ��ն�F1~F10������ʼ��
* ��    ����type == ture--ȫ����ʼ����type==false--��ͨ�Ų�������ȫ����ʼ��
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void ZD_CS1_Init(u8 type)
{
    EW.MoveByte = 'M';
    MovNByte(6, &ZDP.F1.RTS, (u8*)InP0F1);
    
    ByteN_Fill(33, &ZDP.F2.En_Num, 0x00);
    
    if(type == true)
    {
        MovNByte(28, ZDP.F3.IP1, (u8*)InP0F3);
    }
    
    ByteN_Fill(16, ZDP.F4.Tel_Code, 0x00);
    ByteN_Fill(3,  &ZDP.F5.Code   , 0x00);
    ByteN_Fill(16, ZDP.F6.Addr1   , 0x00);
    ByteN_Fill(64, ZDP.F7.ZDIP     , 0x00);
    
    MovNByte(8   , &ZDP.F8.WorkMode , (u8*)InP0F8);
    MovNByte(16  , ZDP.F9.Enable    , (u8*)InP0F9);
    MovN2Byte(29, ZDP.F10.MeterNum, (u8*)InP0F10); //;--20141226
    
    EW.MoveByte = 0;
    EW.CSB1 = 0xC1;
}

/* 
********************************************************************************
* �� �� ����ZD_CS2_Init
* ��    �ܣ�����2��������Ĳ���F11~F23�ĳ�ʼ��
* ��    ������
* ��    �أ��� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void ZD_CS2_Init(void)
{
    EW.MoveByte = 'M';
    ByteN_Fill(CS2NUM, &ZDP.F11.PuNum, 0x00);
    EW.MoveByte = 0;
    EW.CSB2 = 0xC2;
}

/* 
********************************************************************************
* �� �� ����ZD_CS3_Init
* ��    �ܣ��ն˲���F33~F39��F57~F61�ĳ�ʼ��
* ��    ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void ZD_CS3_Init(void)
{
    EW.MoveByte = 'M';
    MovNByte(61, &ZDP.F33.PortN, (u8*)InP0F33);
    MovNByte(3, ZDP.F57, (u8*)InP0F57);
    MovNByte(4, &ZDP.F59.EnergyOver, (u8*)InP0F59);
    
    ByteN_Fill(13 , &ZDP.F34.PortN, 0x00);
    ByteN_Fill(11 , &ZDP.F35.UsrN , 0x00);
    ByteN_Fill(4  , ZDP.F36       , 0x00);
    ByteN_Fill(19 , &ZDP.F37.Port, 0x00);
    ByteN_Fill(138, &ZDP.F38.UsrBIndex, 0x00);
    ByteN_Fill(138, &ZDP.F39.UsrBIndex, 0x00);
    
    ByteN_Fill(1, ZDP.F58, 0x00);
    ByteN_Fill(44, ZDP.F60.XBU_High, 0x00);
    ByteN_Fill(1, ZDP.F61, 0x00);
    
    EW.MoveByte = 0;
    EW.CSB3 = 0xC3;
}

/* 
********************************************************************************
* �� �� ����ZD_CS4_Init
* ��    �ܣ����������F25~F31��F73~F76������ʼ��
* ��    ����Pn-�������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void ZD_CS4_Init(u16 Pn)
{
    
    u32 *Addr32;
    struct PoPStr  *PnP;

    Addr32 = (u32*)(*(PnDTAddr + Pn));
    PnP = (struct PoPStr*)(*(Addr32 + _PnP));
    EW.MoveByte = 'M';
    MovNByte(11, (u8*)(&PnP->F25.UMul), (u8*)InP1F25);
    MovNByte(57, PnP->F26.UHigh, (u8*)InP1F26);
    MovNByte(4,  PnP->F28.Cos1, (u8*)InP1F28);
    
    ByteN_Fill(24 , PnP->F27.RA, 0x00);
    ByteN_Fill(12 , PnP->F29, 0x00);
    ByteN_Fill(1 ,  PnP->F30, 0x00);
    ByteN_Fill(25 , &PnP->F31.NodeN, 0x00);
    ByteN_Fill(48 , &PnP->F73[0].Sign, 0x00);
    ByteN_Fill(10 , PnP->F74.PowerFactor, 0x00);
    ByteN_Fill(16 , PnP->F75.UOut, 0x00);
    ByteN_Fill(1 ,  PnP->F76, 0x00);
    EW.MoveByte = 0;
//    EW.CSB4 |= (0x0001 << (Pn -1)); //;��¼��Ҫ����Ĳ����������
    EW.CSB4 |= (0x0001 << Pn );   //;ԭ����Pn����0ʱû�б������
}

/* 
********************************************************************************
* �� �� ����ZD_CS5_Init
* ��    �ܣ����������ʼ��Ϊ0
* ��    ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void ZD_CS5_Init(void)
{
    EW.MoveByte = 'M';
    ByteN_Fill(CS5NUM , &TaskP.F65[0].Cyc, 0x00);
    EW.CSB5 = 0xC5;
}

/* 
********************************************************************************
* �� �� ����ZD_CS6_Init
* ��    �ܣ�������������س�ʼ��
* ��    ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void ZD_CS6_Init(void)
{
    EW.MoveByte = 'M';
    ByteN_Fill(CS6NUM - 16, &TaskP.F66[0].Cyc, 0x00);//;��ʼ������1,2�����ݸ�8������
    ByteN_Fill(8, TaskP.F67, 0xAA); 
    ByteN_Fill(8, TaskP.F68, 0xAA);  //;��ʼ��û���ú�---20141226
    EW.CSB6 = 0xC6;
}

/* 
********************************************************************************
* �� �� ����ZD_CS7_Init
* ��    �ܣ��ܼ��顢ģ������������������ʼ��
* ��    ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void ZD_CS7_Init(void)
{
    EW.MoveByte = 'M';
    ByteN_Fill(CS7NUM_GRP, &GRPP.F41.Flag, 0x00);
    ByteN_Fill(CS7NUM_DCP, DCPP.F81.ValueS, 0x00);
    ByteN_Fill(CS7NUM_CTR, F49, 0x00);
    EW.CSB7 = 0xC7;
}

/* 
********************************************************************************
* �� �� ����ZD_CS8_Init
* ��    �ܣ���չ������ʼ��
* ��    ����type == ture--ȫ����ʼ����type==false--��ͨ�Ų�������ȫ����ʼ��
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void ZD_CS8_Init(u8 type)
{
    EW.MoveByte = 'M';
    if(type == true)
    {
        MovNByte(4,  ZDPKZ.F89.QuXian,(u8*)InP0F89);
    }
    MovNByte(16, ZDPKZ.F97.UserSend,(u8*)InP1F97);
    MovNByte(20, ZDPKZ.F98,(u8*)User_Top);
    MovNByte(20, ZDPKZ.F99,(u8*)User_End);
    MovNByte(20, ZDPKZ.F100,(u8*)Admi_Top);
    MovNByte(20, ZDPKZ.F101,(u8*)Admi_End);
    EW.MoveByte = 0; 
    EW.CSB8 = 0xC8;
}

/* 
********************************************************************************
* �� �� ����ZD_CS9_Init
* ��    �ܣ����������ʼ��Ϊ0
* ��    ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void ZD_CS9_Init(void)
{
    EW.MoveByte = 'M';
    ByteN_Fill(CS9NUM , &TaskP.F65[4].Cyc, 0x00);
    EW.CSB9 = 0xC9;
}

/* 
********************************************************************************
* �� �� ����ZD_CS10_Init
* ��    �ܣ����������ʼ��Ϊ0
* ��    ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void ZD_CS10_Init(void)
{
    EW.MoveByte = 'M';
    ByteN_Fill(CS10NUM , &TaskP.F66[4].Cyc, 0x00);
    EW.CSB10 = 0xCA;
}

/* 
********************************************************************************
* �� �� ����ZD_CS_DatArea_Init
* ��    �ܣ�������ȫ�����ݳ�ʼ��
* ��    ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void ZD_CSDataInit(void)
{
    u8 i;

    ERC1_Judge(1);
    ZD_CS1_Init(true); //;��ʼ��������1
    ZD_CS2_Init(); //;��ʼ��������2
    ZD_CS3_Init(); //;��ʼ��������4
    ZD_CS5_Init(); //;4������1�����ݳ�ʼ��������5
    ZD_CS6_Init(); //;4������2�����ݳ�ʼ��������6
    ZD_CS7_Init(); //;��ʼ��������5
    ZD_CS8_Init(true); //;��ʼ��������6
    ZD_CS9_Init(); //;����4������1�����ݳ�ʼ��������9
    ZD_CS10_Init(); //;��ʼ��������10
    ZD_DataInit();//;��ʼ���ն�������
    
    for(i=0; i<PNMAXNUM; i++)
    {
        ZD_CS4_Init(i);  //;��ʼ�������������3
    }

    for(i = 0; i < TASK1NUM; i++)
    {
        Task_Init(i+1, 65);//;��ʼ��1������
    }
    for(i = 0; i < TASK2NUM; i++)
    {
        Task_Init(i+1, 66);//;��ʼ��2������
    }
}

/* 
********************************************************************************
* �� �� ����ZD_CS_DatArea_Init
* ��    �ܣ�������ȫ�����ݳ�ʼ��(ͨ�Ų�������)
* ��    ������
* ��    �أ��� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void ZD_NCCSDataInit(void)
{
    u8 i;

    ERC1_Judge(1);
    ZD_CS1_Init(false); //;��ʼ��������1
    ZD_CS2_Init(); //;��ʼ��������2
    ZD_CS3_Init(); //;��ʼ��������4
    ZD_CS5_Init(); //;4�������ʼ��������5
    ZD_CS6_Init(); //;��ʼ��������6
    ZD_CS7_Init(); //;��ʼ��������5
    ZD_CS8_Init(false); //;��ʼ��������6
    ZD_CS9_Init(); //;��ʼ��������9
    ZD_CS10_Init(); //;��ʼ��������10
    ZD_DataInit();//;��ʼ���ն�������
    
    for(i=0; i<PNMAXNUM; i++)
    {
        ZD_CS4_Init(i);  //;��ʼ�������������3
    }

    for(i = 0; i < TASK1NUM; i++)
    {
        Task_Init(i+1, 65);//;��ʼ��1������
    }
    for(i = 0; i < TASK2NUM; i++)
    {
        Task_Init(i+1, 66);//;��ʼ��2������
    }
}


/* 
********************************************************************************
* �� �� ����ZD_Time_Set
* ��    �ܣ��ն�����ʱ��
* ��    ����TimeAddr-��ʱ���ַ
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void ZD_Time_Set(u8 *TimeAddr)
{
    u8 W1 = 0, W2 = 0;
    u8 preyear, premonth, preday, prehour;
    TBCD.Sec = *(TimeAddr + 0);
    TBCD.Min = *(TimeAddr + 1);
    TBCD.Hour = *(TimeAddr + 2);
    TBCD.Day = *(TimeAddr + 3);
    TBCD.Month = *(TimeAddr + 4) & 0x1F;
    TBCD.Year = *(TimeAddr + 5);
    W1 = TBCD.Month >> 5;
    W2 = WeekConut(TimeAddr + 3); //;�������   
    if (W1 = W2)
    {
        Week = W1;
    }
    else
    {
        Week = W2;
    }
    
    preyear = THEX.Year;
    premonth = THEX.Month;
    preday   = THEX.Day;
    prehour  = THEX.Hour;
    
    THEX.Year = BCD1ToU8(&TBCD.Year);
    THEX.Month = BCD1ToU8(&TBCD.Month);
    THEX.Day = BCD1ToU8(&TBCD.Day);
    THEX.Hour = BCD1ToU8(&TBCD.Hour);
    THEX.Min = BCD1ToU8(&TBCD.Min);
    THEX.Sec = BCD1ToU8(&TBCD.Sec);
    
    if(preyear != THEX.Year)
    {
        TimeChgFlag = 1;
    }
    else if(premonth != THEX.Month)
    {
        TimeChgFlag = 2;
    }
    else if(preday  != THEX.Day)
    {
        TimeChgFlag = 3;
    }
    else if(prehour != THEX.Hour)
    {
        TimeChgFlag = 4;
    }
    //EPSON8025_SAVE(0xE0, 0x20); //;24Сʱ��
    //EPSON8025_SAVE(0x60, TBCD.Year); //;��
    //EPSON8025_SAVE(0x50, TBCD.Month); //;��
    //EPSON8025_SAVE(0x40, TBCD.Day); //;��
    //EPSON8025_SAVE(0x30, Week); //;����
    //EPSON8025_SAVE(0x20, TBCD.Hour); //;Сʱ
    //EPSON8025_SAVE(0x10, TBCD.Min); //;����
    //EPSON8025_SAVE(0x00, TBCD.Sec); //;��  
}


///*----------------------------------------------------------------------------*
// |   �ն�ʱ���ȡ
// *----------------------------------------------------------------------------*/
//void ZD_Time_Read(void)
//{
//    u8 Time[7];
//
//    EPSON8025_READ(Time);    
//    if (TIME_CHK(Time) == true) //;������ʱ����ȷ
//    {
//        ZDD.DayTime[0] = TBCD.Sec = Time[0];
//        ZDD.DayTime[1] = TBCD.Min = Time[1];
//        ZDD.DayTime[2] = TBCD.Hour = Time[2];
//        ZDD.DayTime[3] = TBCD.Day = Time[4];
//        ZDD.DayTime[4] = TBCD.Month = Time[5];
//        ZDD.DayTime[5] = TBCD.Year = Time[6];
//        Week = Time[3];
//        ZDD.DayTime[4] |= (Week << 5);
//        THEX.Year = BCD1ToU8(&TBCD.Year);
//        THEX.Month = BCD1ToU8(&TBCD.Month);
//        THEX.Day = BCD1ToU8(&TBCD.Day);
//        THEX.Hour = BCD1ToU8(&TBCD.Hour);
//        THEX.Min = BCD1ToU8(&TBCD.Min);
//        THEX.Sec = BCD1ToU8(&TBCD.Sec);
//        if (Old_Sec != TBCD.Sec)
//        {
//            Old_Sec = TBCD.Sec;
//            RTC_FlagSec = true; //;ÿ��ִ��һ��
//        }
//        if (Old_Min != TBCD.Min) 
//        {
//            Old_Min = TBCD.Min;
//            RTC_FlagMin = true;  //;ÿ����ִ��һ��
//        }
//    }
//    else
//    {
//        u8 In8025Time[6] = { 0x00, 0x00, 0x00, 0x01, 0x01, 0x00};  //;ʱ��оƬ��ʼ��ʱ��,��.��.ʱ.��.��.��
//        
//        ZD_Time_Set((u8*)In8025Time);  //;��ʼ��ʱ��
//    }
//}


/* 
********************************************************************************
* �� �� ����ClockTimer
* ��    �ܣ���ʱ��оƬʱ���ô˺���ʵ��ʱ�ӹ���
* ��    ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��2011-10-13 16:49:46 �޸ĵ�Num025s_WT>8��ʱ������ת�������
********************************************************************************
*/
void ClockTimer(void)
{   
    static u8 Old_Sec, Old_Min;
    u8 Nsec;
	  	  
	  if(Num025s_WT >= 4)
	  { 
	      Nsec = Num025s_WT / 4;
	      Num025s_WT -= Nsec * 4;
	  	  if(THEX.Sec >= 59)  //;�ַ�ת
	  	  {
	  	  	  THEX.Sec = 0;
	  	  	  if(THEX.Min >= 59)  //;Сʱ��ת
	  	  	  {
	  	  	  	  THEX.Min = 0;
	  	  	  	  if(THEX.Hour >= 23)  //;�췭ת
	  	  	  	  {
	  	  	  	  	  THEX.Hour = 0;	  	  	 	  
	  	  	  	  	  if(THEX.Day >= DaysOfMonth[THEX.Month])  //;�ж��Ƿ����·�ת
	  	  	  	  	  {
	  	  	  	  	  	  if(THEX.Month == 2)  //;���·ݵ��������
	  	  	  	  	  	  {
	  	  	  	  	  	  	  if((THEX.Year % 4) == 0x00)  //;���� --@2
	  	  	  	  	  	  	  {
	  	  	  	  	  	  	  	  if(THEX.Day >= 29 )
	  	  	  	  	  	  	  	  {
	  	  	  	  	  	  	  	  	  THEX.Day = 1;
	  	  	  	  	  	  	  	  }
	  	  	  	  	  	  	  	  else   
	  	  	  	  	  	  	  	  {
	  	  	  	  	  	  	  	  	  THEX.Day++;
	  	  	  	  	  	  	  	  }
	  	  	  	  	  	  	  }
	  	  	  	  	  	  	  else
	  	  	  	  	  	  	  {
	  	  	  	  	  	  	  	  THEX.Day = 1;
	  	  	  	  	  	  	  }
	  	  	  	  	  	  }
	  	  	  	  	  	  else
	  	  	  	  	  	  {
	  	  	  	  	  	  	  THEX.Day = 1;
	  	  	  	  	  	  }
	  	  	  	  	  	  
	  	  	  	  	  	  if(THEX.Day == 1) //;�����·�ת
	  	  	  	  	  	  {	  	  	  	  	  	      	  	  	  	  	  	      
	  	  	  	  	  	      if(THEX.Month >= 12) //;�����귭ת
	  	  	  	  	  	      {	  	  	  	  	  	          
	  	  	  	  	  	          THEX.Month = 1;
	  	  	  	  	  	          THEX.Year++;
	  	  	  	  	  	      }
	  	  	  	  	  	      else
	  	  	  	  	  	      {
	  	  	  	  	  	          THEX.Month++;
	  	  	  	  	  	      }
	  	  	  	  	  	  }
	  	  	  	  	  }
	  	  	  	  	  else
	  	  	  	  	  {
	  	  	  	  	  	  THEX.Day++;
	  	  	  	  	  }
	  	  	  	  }
	  	  	  	  else
	  	  	  	  {
	  	  	  	  	  THEX.Hour++;
	  	  	  	  }
	  	  	  }
	  	  	  else
	  	  	  {
	  	  	      THEX.Min++;
	  	  	  }
	  	  }
	  	  else
	  	  {
	  	  	  THEX.Sec += Nsec;
	  	  }
	  }
	  
	  ZDS.F2[0] = TBCD.Sec   = (THEX.Sec/10)*16  + THEX.Sec%10;
	  ZDS.F2[1] = TBCD.Min   = (THEX.Min/10)*16  + THEX.Min%10;
	  ZDS.F2[2] = TBCD.Hour  = (THEX.Hour/10)*16 + THEX.Hour%10;
	  ZDS.F2[3] = TBCD.Day   = (THEX.Day/10)*16  + THEX.Day%10;
	  ZDS.F2[4] = TBCD.Month = (THEX.Month/10)*16+ THEX.Month%10;
	  ZDS.F2[5] = TBCD.Year  = (THEX.Year/10)*16 + THEX.Year%10;
    ZDS.F2[4] |= WeekConut(&TBCD.Day)<<5;    //;by zrt 20130620 �����ն�����ʱ����ʾ����	  
	  if(Old_Sec != TBCD.Sec)
    {
        Old_Sec = TBCD.Sec;
        RTC_FlagSec = true; //;ÿ��ִ��һ��
    }
    if(Old_Min != TBCD.Min) 
    {
        Old_Min = TBCD.Min;
        RTC_FlagMin = true;  //;ÿ����ִ��һ��
    }
}

/* 
********************************************************************************
* �� �� ����CheckTime
* ��    �ܣ���ʱ��������
* ��    ������
* ��    �أ���
* ˵    ��������ϵͳʱ���볭�ػ���ʱ�ӵ�ʱ��Ա�
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void CheckTime(void)
{
    s32 long1;
    
    if((TBCD.Year==PMD1.RC010[3]) && (TBCD.Month==PMD1.RC010[2]) 
       && (TBCD.Day==PMD1.RC010[1]))
    {       
//        long1=Time_distance(&TBCD.Sec, (u8*)PMD1.RC011 + 1);20140116
        long1=Time_distance(&TBCD.Sec, (u8*)PMD1.RC011 );		
        if((long1 > -5) && (long1 < 5))//;5�������ڲ�Уʱ
        {
            return;
        }
    }

    //GprsLC.CHKLink = true; //;���¼�����ߡ�
    GprsLC.CHKLink = false; //;���¼�����ߡ� 20150105 (�ر����¼������)
    Week = PMD1.RC010[0];
    ZDS.F2[3]=TBCD.Day = PMD1.RC010[1];
    ZDS.F2[4]=TBCD.Month = PMD1.RC010[2];
    ZDS.F2[5]=TBCD.Year = PMD1.RC010[3];
    ZDS.F2[0]=TBCD.Sec = PMD1.RC011[0];
    ZDS.F2[1]=TBCD.Min = PMD1.RC011[1];
    ZDS.F2[2]=TBCD.Hour =PMD1.RC011[2];
    THEX.Year=(TBCD.Year&0x0f)+(TBCD.Year>>4)*10;
    THEX.Month=(TBCD.Month&0x0f)+(TBCD.Month>>4)*10;
    THEX.Day=(TBCD.Day&0x0f)+(TBCD.Day>>4)*10;
    THEX.Hour=(TBCD.Hour&0x0f)+(TBCD.Hour>>4)*10;
    THEX.Min=(TBCD.Min&0x0f)+(TBCD.Min>>4)*10;
    THEX.Sec=(TBCD.Sec&0x0f)+(TBCD.Sec>>4)*10;
    if(Old_Sec != TBCD.Sec)
    {
        Old_Sec = TBCD.Sec;
        RTC_FlagSec = true; //;ÿ��ִ��һ��
    }
    if(Old_Min != TBCD.Min) 
    {
        Old_Min = TBCD.Min;
        RTC_FlagMin = true;  //;ÿ����ִ��һ��
    }        
}

/* 
********************************************************************************
* �� �� ����ZD_Relay
* ��    �ܣ�����ת������
* ��    ������
* ��    �أ���
* ˵    ����ÿ250msִ��һ��
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void ZD_Relay(void)
{
    static u8 Sts1, Mark1;
#ifdef PORT2USART     
    static u8 Sts2, Mark2;
#endif    
    
    if(RelayC.Status == 'E') //;���м����ݴ��ն˶˿ڷ���
    {                
        if((RelayC.Port == 1) && ((Port1.Status == 0) || (Port1.Status == 'F')))
        {
            Sts1 = Port1.Status;
            Mark1 = Port1.Mark;
            Port1.Status = 'E';  //;�˿ڿ�ʼ��������
            Port1.Mark = 'R';    //;�����м̷��ͱ�־
            /*�ݲ�֧���޸Ĳ�����*/
//;            if(Port1.BPS != RelayC.BPS)
//;            {
//;                Port1.BPS = RelayC.BPS;
//;                UARTx_BaudRate_Set(RelayC.BPS, 3);
//;            }
            Port1.Buffer = PORT1SEDBUF;  //;ָ��˿�1�Ļ�����
            RelayC.Status = 'S'; //;�м����ݷ������
            Port1.Len = RelayC.Len;  //;д��Ҫ���͵ĳ���
            EW.MoveByte = 'M';
            MovNByte(RelayC.Len, (u8*)Port1.Buffer, (u8*)RelayC.Data); //;������д��˿�1�Ļ�����
            EW.MoveByte = 0;
        }
#ifdef PORT2USART         
        if((RelayC.Port == 2) && (Port2.Status == 0)) 
        {
            Sts2 = Port2.Status;
            Mark2 = Port2.Mark;
            Port2.Status = 'E';  //;�˿ڿ�ʼ��������
            Port2.Mark = 'R';    //;�����м̷��ͱ�־
            Port2.Buffer = PORT2SEDBUF; //;������д��˿�2�Ļ�����
            Port2.Len = RelayC.Len;  //Ҫ���͵ĳ���
            RelayC.Status = 'S'; //;�м����ݷ������
            EW.MoveByte = 'M';
            MovNByte(RelayC.Len, (u8*)Port2.Buffer, (u8*)RelayC.Data); //;������д��˿�2�Ļ�����
            EW.MoveByte = 0;
        }
#endif        
    }   

    if(RelayC.Status == 'S') //;�м����ݷ�����ɣ����ڵȴ��м̷���
    {
        if((RelayC.Port == 1) && (Port1.Status =='R') && (Port1.Mark == 'R')) //;�˿�1���յ��м�����
        {
            Port1.Status = Sts1; //;�ͷŶ˿�1
            Port1.Mark = Mark1;
            EW.MoveByte = 'M';
            MovNByte(Port1.Len, (u8*)RelayC.Data, (u8*)Port1.Buffer); //;�˿�1���յ�����д���м�������
            EW.MoveByte = 0;
            RelayC.Len = Port1.Len; //;�м�Ӧ�����ݵĳ���
            RelayC.Status = right; //;�м̽�����ȷ
        }
#ifdef PORT2USART         
        if((RelayC.Port == 2) && (Port2.Status =='R') && (Port2.Mark == 'R')) //;�˿�2���յ��м�����
        {
            Port2.Status = Sts2; //;�ͷŶ˿�2
            Port2.Mark = Mark2;
            EW.MoveByte = 'M';
            MovNByte(Port2.Len, (u8*)RelayC.Data, (u8*)Port2.Buffer); //;�˿�1���յ�����д���м�������
            EW.MoveByte = 0;
            RelayC.Len = Port2.Len; //;�м�Ӧ�����ݵĳ���
            RelayC.Status = right; //;�м̽�����ȷ
        }
#endif        
    }
    
    if(RelayC.Status != 0) //;���ڴ����м�������
    {
        if(RelayC.OvTime > 0)
        {
            RelayC.OvTime--;
            if(RelayC.OvTime == 0) 
            {
                RelayC.Status = error; //;�м̳�ʱ
                if((RelayC.Port == 1) && (Port1.Status != 0) && (Port1.Mark == 'R')) //;�˿�1���м�ռ��
                {
                    Port1.Status = Sts1; //;�ͷŶ˿�1
                    Port1.Mark = Mark1;
                }
#ifdef PORT2USART                 
                if((RelayC.Port == 2) && (Port2.Status != 0) && (Port2.Mark == 'R')) //;�˿�2���м�ռ��
                {
                    Port2.Status = Sts2; //;�ͷŶ˿�2
                    Port2.Mark = Mark2;
                }
#endif                 
            }
        }
    }
}


/* 
********************************************************************************
* �� �� ����ZD_POW_OFF
* ��    �ܣ��رյ�Դ
* ��    ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void ZD_POW_OFF(void)
{
    if(Modem_OFF_CHK() == false) //;��ǰģ������
    {
        Modem_GDCDOFF();
        Modem_OFF(); //;�ر�ģ��       
    }
    while(1)
    {
        if(RTC_FlagMs)     //;250msִ��һ��
        {
            ;
        }
    }
}


/* 
********************************************************************************
* �� �� ����ZD_POW_CHK
* ��    �ܣ����紦�����
* ��    ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void ZD_POW_CHK(void)
{
    u8 PowLostTime = 0;
//;    u8 saveflag = true;
//;    u8 upflag = true;
    
    if(Power_Modem == _48V)
    {        
			      USART_Cmd(USART1, DISABLE); 
			      USART_Cmd(USART3, DISABLE);           
//;            ERC14_Judge(1);   //;�ն�ͣ��      
            ERCSave.Pn = 0x00; //;����澯����
            if(GprsC.State & _HadLogin)
            {
                GprsC.LoginOut = 'E';
            }
            while (1)
            { 
                USART_Com();        //;����ͨѶ
                Modem_Com();        //;Gprsģ��ͨѶ
                AFNComDeal();       //;GDW130��Լ������Զ��ͨѶ
                if (RTC_FlagMs)     //;250msִ��һ��
                {
                    RTC_FlagMs = 0;
                    ClockTimer();
//;                    if(saveflag) //;�澯�����¼�����
//;                    {
//;                        if(ERC_Deal_Save())
//;                        {
//;                            saveflag = false;
//;                        }
//;                    }                    
//;                    else
//;                    {
//;                        if(!(GprsC.State & _HadLogin))
//;                        {
//;                            ZD_POW_OFF();
//;                        }
//;                        else if(upflag)
//;                        {                           
//;                            if(ZD_ERC())       //;�����¼�
//;                            {
//;                                upflag = false;
//;                                if(GprsC.LoginOut == 0)
//;                                {
//;                                    GprsC.LoginOut = 'E';
//;                                }
//;                            }
//;                        }
//;                    }
                    
                    if(GprsC.LoginOut == 'R')
                    {
                        ZD_POW_OFF();
                    }
                    
                    if (RTC_FlagSec)//;ÿ��ִ��һ��
                    {
                        RTC_FlagSec = 0;
                        PowLostTime ++;
                        Watchdog();       //;ι���Ź�
                        ATComdSetNoRec();     //;��GPRSģ�鷢��AT �����޻�Ӧ����  
                        while(PowLostTime >=30)
                        {
                            ZD_POW_OFF();
                        }
                    }
                }
            }
    }
}


 /* 
********************************************************************************
* �� �� ����Master_HB
* ��    �ܣ���վ��������
* ��    ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��2011-06-21 13:51:35  �޸�����������Ӧ���ز�Ϊ1����
********************************************************************************
*/
void Master_HB(void)
{
    if((ZDP.F8.WorkMode & 0x03) == 0x02) //;��������
    {
        return;
    }
    
    if (GprsC.State & _HadLogin)
    {
        if (GprsC.HBStatus == idle)
        {
            if (GprsC.HBTime == 0)
            {
                if (ZDP.F1.HBeat != 0)
                {
                    GprsC.HBTime = ZDP.F1.HBeat;
                    //GprsC.HBTime *= 60;
                    GprsC.HBTime *= 45;
                }
            }
            else if (GprsC.HBTime > 0)
            {
                GprsC.HBTime--;                                                                                         //;������������ʱ
                if (GprsC.HBTime == 0)
                {
                    GprsC.HBStatus = 'E';
                }
            }
        }

        if (GprsC.HBStatus == 'S') 
        {
//;            u16 Time;

            GprsC.HBOVT++; 
//;            Time = ZDP.F1.HBeat;
//;            Time *= 60;
//;            Time /= 2;
            if (GprsC.HBOVT >= 60) //;������ʱ
            {
//;                GprsC.HBStatus = 'E';
//;               GprsC.HBStatus = 0; //;--@2
//;               GprsC.HBTime = 1;  //;
                GprsC.HBOVT = 0;
                GprsC.HBNumSend++;
                if(GprsC.HBNumSend < 3)
                {
                     GprsC.HBStatus = 'E';
                }
                else
                {
                    if (GprsC.ATStatus == idle)
                    {
                        GprsC.HBNumSend = 0x00;
                        GprsC.HBStatus = 0;
                        GprsC.ATStatus = 'E';                //;�����Ǽ����ӣ������Ͽ�����
                        GprsC.SendAT_Wait1s = 5;
                        GprsC.SendAT_Step = 17;
                    }
                    else
                    {
                        GprsC.HBOVT = 60;                 //;�ȴ�ATStatus����
                    }
                }
            }
        }
        else if(GprsC.HBStatus == 'E') //;��ʱδ����
        {
            GprsC.HBOVT++;
            if(GprsC.HBOVT >= 120)
            {
                GprsC.HBStatus = idle;
                GprsC.HBOVT = 0;
            }
        }
        if (GprsC.HBStatus == 'R')
        {
            GprsC.HBStatus = idle;
            GprsC.HBOVT = 0;
        }
    }
    else
    {
        GprsC.HBStatus = idle;
        GprsC.HBTime = 0;
        GprsC.HBOVT = 0;
    }
}


/* 
********************************************************************************
* �� �� ����Master_Login
* ��    �ܣ���վ��½����,3�ε�½���ɹ��������Ͽ�����
* ��    ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void Master_Login(void)
{
    if(GprsC.LinkState == LINK0)
    {
        if ((GprsC.State & _Connect) && (!(GprsC.State & _HadLogin))) //;��δ��½
        {
            if (GprsC.LoginIn == idle)
            {
                GprsC.LoginIn = 'E';
            }
        
            if (GprsC.LoginIn == 'S')                                                                                       //;��½�����޻�Ӧ��ʱ
            {
                GprsC.LoginTimeOut++;
                if (GprsC.LoginTimeOut > 30)                                                              //;��30���ʱ����û�н��յ���վ�Ե�¼���ĵ�Ӧ��
                {
                    GprsC.LoginTimeOut = 0;
                    GprsC.LoginNumSend++;
                    if (GprsC.LoginNumSend < 3)        //;����ط���¼����3��
                    {
                        GprsC.LoginIn = idle;          //;��Ҫ�ٴη��͵�¼����
                    }
                    //;else if (GprsC.LoginNumSend >= 3)
			              else
			              {
                        if (GprsC.ATStatus == idle)
                        {
                            GprsC.ATStatus = 'E';       //;���ܵǴ���վ��ͨѶ�������������Ͽ�����
                            GprsC.SendAT_Wait1s = 5;
                            GprsC.SendAT_Step = 17;
                        }
                        else
                        {
                            GprsC.LoginNumSend = 3;     //;�ȴ�ATStatus����
                        }
                    }
                }
            }
        
            if(GprsC.LoginIn == 'R') //;�յ���վ�Ե�¼���ĵ���ȷӦ��
            {
                GprsC.LoginIn = idle;
                GprsC.LoginTimeOut = 0;
                GprsC.LoginNumSend = 0;
                GprsC.NLogonMin = 0x00;
                GprsC.State |= _HadLogin;              //;��ǰ���ڵ�½״̬
                GprsC.Status |= _ContHost;              //;��ʾ��ǰ�Ѿ�Ѱ�ҵ���վ
                GprsC.Status &= ~_FindHost;            //;��ʾѰ����վ����
                Modem_GDCDON();                         //;�������
            }
        }    
        else
        {
            GprsC.LoginIn = idle;
            GprsC.LoginTimeOut = 0;
            GprsC.LoginNumSend = 0;
        }
    }
    else
    {
        if ((GprsC.State1 & _Connect) && (!(GprsC.State1 & _HadLogin))) //;��δ��½
        {
            if (GprsC.LoginIn == idle)
            {
                GprsC.LoginIn = 'E';
            }
        
            if (GprsC.LoginIn == 'S')                                                                                       //;��½�����޻�Ӧ��ʱ
            {
                GprsC.LoginTimeOut++;
                if (GprsC.LoginTimeOut > 30)                                                              //;��30���ʱ����û�н��յ���վ�Ե�¼���ĵ�Ӧ��
                {
                    GprsC.LoginTimeOut = 0;
                    GprsC.LoginNumSend++;
                     if (GprsC.LoginNumSend < 3)        //;����ط���¼����3��
                    {
                        GprsC.LoginIn = idle;          //;��Ҫ�ٴη��͵�¼����
                    }
                   // else if (GprsC.LoginNumSend == 3)
			else
			{
//;                        if (GprsC.ATStatus == idle)
//;                        {
//;                            GprsC.ATStatus = 'E';       //;���ܵǴ���վ��ͨѶ�������������Ͽ�����
//;                            GprsC.SendAT_Wait1s = 5;
//;                            GprsC.SendAT_Step = 17;
//;                        }
//;                        else
//;                        {
//;                            GprsC.LoginNumSend = 3;     //;�ȴ�ATStatus����
//;                        }
                    }
                }
            }
        
            if (GprsC.LoginIn == 'R') //;�յ���վ�Ե�¼���ĵ���ȷӦ��
            {
                GprsC.LoginIn = idle;
                GprsC.LoginTimeOut = 0;
                GprsC.LoginNumSend = 0;
                GprsC.State1 |= _HadLogin;  //;��ǰ���ڵ�½״̬
                GprsC.LinkState = LINK0;
            }
        }
        else
        {
            GprsC.LoginIn = idle;
            GprsC.LoginTimeOut = 0;
            GprsC.LoginNumSend= 0;
        }
    }
}

/* 
********************************************************************************
* �� �� ����LinkStat_CHK
* ��    �ܣ��ն�������վ��⡣
* ��    ������
* ��    �أ��� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void LinkStat_CHK(void)
{
    u8 bit, byte, type;
    
    type = ZDP.F8.WorkMode & 0x03;
    
    if(type <= 0x01)
    {
        GprsLC.LinkEnable = true;
    }
    else if(type == 0x02) //;��������ģʽ
    {
        if(GprsLC.PassLinkOrd)
        {
            GprsLC.LinkEnable = true;
        }
        else
        {
            GprsLC.LinkEnable = false;
        }
    }
    else if(type == 0x03) //;ʱ������ģʽ
    {    
        if((THEX.Min==0x00) || GprsLC.CHKLink)
        {
            GprsLC.CHKLink = false;
/*        20130609 zrt
            bit = THEX.Hour & 0x07;  //;�ҳ���ǰСʱ����ʱ����ڼ�λ
            byte = THEX.Hour & 0xF1; //;�ҳ���ǰСʱ����ʱ����ڼ��ֽ�
*/        
            bit = THEX.Hour & 0x07;  //;�ҳ���ǰСʱ����ʱ����ڼ�λ
            byte = THEX.Hour /8;      //;�ҳ���ǰСʱ����ʱ����ڼ��ֽ�
            if(ZDP.F8.AllowTimeF[byte] & (0x01<<bit))
            {
                GprsLC.LinkEnable = true;
            }
            else
            {
                GprsLC.LinkEnable = false;
            }
        
        }
        
    }
    
    if(GprsLC.Link2Switch) //;��������
    {
        GprsLC.LinkEnable = true;
    }
}


/* 
********************************************************************************
* �� �� ����Master_Link
* ��    �ܣ�������վ�ͱ�����վ�����Ӻͼ��
* ��    ������
* ��    �أ���
* ˵    ����ZDP.F62.Type:==1 ��������ģʽ;==2��������ģʽ ; Interval �ز����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��2011-06-09 16:22:59
*           2012-01-31 14:39:35 �����ز�״̬���źż��
********************************************************************************
*/
void Master_Link(void)
{
    u8 type;
    
    type = ZDP.F8.WorkMode & 0x03;
    
    if(GprsC.State & _SetUp)
    {
        if(!(GprsC.State & _Connect))
        {
            if((type==1) || (type==3) || ((type==2)&&(GprsLC.PassLinkOrd))) //;��������ģʽ
            {
                if(!(GprsC.State & _Redial))
                {
                    if(!(GprsC.State & _Link))     //;��δ��������
                    {
                        if(ZDS.F10.Flux_Month < FluxLimValue)
                        {
                            if(GprsC.ATStatus == idle)
                            {
                                GprsC.ATStatus = 'E';      //;���״̬�ز�
                                GprsC.ATSOVT = 0;
                                GprsC.SendAT_Wait1s = 5;
                                GprsC.SendAT_Step = 1;      //;��ʼ��AT�����TCP����
                                GprsC.IP = ZDP.F3.IP1;
                                GprsC.LinkState = LINK0;
                                GprsC.Port = *((u16*)ZDP.F3.Port1);
                                GprsC.APN = ZDP.F3.APN;
                                GprsC.State |= _Link;
                                GprsC.Status = 0; 
                            }
                        }
                    }
                }
                else
                {
                    if(type == 2) //;��������
                    {
                        if(GprsC.MinReCon == 0x00)
                        {
                            GprsLC.PassLinkNum--;
                            if(GprsLC.PassLinkNum >= 0)
                            {
                                GprsC.MinReCon = 10;
                            }
                            else  //;�ﵽ����ز�����
                            {
                                GprsLC.PassLinkOrd = false; //;�����ز�
                                GprsC.State &= (~_Redial); //;�˳��ز�״̬
                                GprsLC.LinkEnable = false;
                            }
                        }
                        else
                        {
                            GprsC.MinReCon--;
                            if(GprsC.MinReCon == 0x00)
                            {
                                GprsC.State &= (~_Redial); //;�˳��ز�״̬                            
                            }
                        }
                    }
                    else
                    {
                        if(GprsC.MinReCon == 0)
                        {
                            if(*((u16*)ZDP.F8.ActRedailSec) != 0)
                            {
                                GprsC.MinReCon = *((u16*)ZDP.F8.ActRedailSec);
                            }
                            else
                            {
                            	  GprsC.MinReCon = 10 ;  //;10���������20131227
                            }
                        }
                        else if(GprsC.MinReCon > 0)
                        {
                            GprsC.MinReCon--;
                            {
                                if(GprsC.MinReCon == 0)
                                {
                                    #ifdef ZS
                                        RecordErr(7);
                                    #endif
                                    GprsC.State &= (~_Redial); //;�˳��ز�״̬
                                }
                            }
                        }
                    }
                }
            }           
        }       
        
        if(GprsC.ReConnect == true) //;��������
        {
            #ifdef ZS
                RecordErr(8);
            #endif
            GprsC.ReConnect = false;
            Modem_GDCDOFF();                 //;�ص������
            GprsC.State &= (~_Connect);      //;�����Ͽ����ӳɹ�
            GprsC.State &= (~_HadLogin);
            GprsC.State &= (~_Link);   //;�˳�����״̬
            GprsC.State &= (~_Redial); //;�˳��ز�״̬
            GprsC.Status = 0x00;       //;���״̬
        }
        
        //;������վ�������� 
        if (GprsLC.Link2Switch == true)
        {
            if(!(GprsC.State1 & _Connect))//;������վ�Ͽ�            
            {
                if(!(GprsC.State1 & _Link)) 
                {
                    if(GprsC.ATStatus == idle)
                    {
                         GprsC.LinkState = LINK1;
                         GprsC.ATStatus = 'E';  
                         GprsC.State1 |= _Link;
                         if(GprsC.State & _HadLogin)
                         //;��վΪ����״̬˵������ֱ�ӷ�������
                         {
                            
                             GprsC.SendAT_Wait1s = 2;
                             GprsC.SendAT_Step = 14;
                         }
                         else
                         {
                             GprsC.SendAT_Wait1s = 5;
                             GprsC.SendAT_Step = 1;
                         }
                    }
                }
            }
        }
    }
    if(TBCD.Sec == 0x40) //;�������ź�
    {
        if(GprsC.LinkState == LINK0)
        {
            if((GprsC.State & _Connect) || (GprsC.State & _Redial))
            {
                if(GprsC.ATStatus == idle)
                {
                
                    GprsC.ATStatus = 'E';
                    GprsC.SendAT_Step = 4;
                    GprsC.SendAT_Wait1s = 1;
                }
            }
        }
    }
}


/* 
********************************************************************************
* �� �� ����ZD_Mast_Link
* ��    �ܣ�����GPRSģ��,ÿ�봦��һ��
* ��    ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void ZD_Mast_Link(void)
{
    Modem_ON_CHK();       //;  ���ƺͼ��GPRSģ������                                                                                                                     //;���GPRSģ���Ƿ�ɹ�����
    Master_Login();       //;  �����ն˵�½��վ                                                                                                                        //;��վ��½����
    Master_HB();          //;  �����ն�����վ��������                                                       //;��������                                                                                                             //;������վ����
    Master_Link();        //;  ����GPRS����
    ATComdSetNoRec();     //;  ��GPRSģ�鷢��AT �����޻�Ӧ����                                                          //;AT�����ʱ
}

/* 
********************************************************************************
* �� �� ����TimeChangeDeal
* ��    �ܣ�ʱ��ı���Ҫ���Ĵ���
* ��    ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��2011-09-29 20:14:30
********************************************************************************
*/
void TimeChangeDeal(void)
{
    u8 i;
    u32 *Addr32;
    struct PointDataStr *PnD;
    
    if(TimeChgFlag != 0x00)
    {
        for(i = 0; i < PNMAXNUM; i++)
        {
            Addr32 = (u32*)(*(PnDTAddr + i));
            PnD = (struct PointDataStr*)(*(Addr32 + _PnD));
            
            switch(TimeChgFlag)
            {
                case 4:
                case 3:
                    ByteN_Fill(228, PnD->DF33.ZPMax, 0x00);
                    ZDS.F10.Flux_Month = 0;
                case 2:
                    ByteN_Fill(207, PnD->DF25.ZPMax, 0x00);
                    ZDS.F10.Flux_Day = 0;
                case 1:
                    ByteN_Fill(366, PnD->ODJ_B630.Td_h, 0x00);
                    break;
                default:
                    break;   
            }
        }
        TimeChgFlag = 0x00;
    }
}

