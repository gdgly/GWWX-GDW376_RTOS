/*----------------------------------------------------------------------------*
 |  ���Ʋ��������ݶ���
 *----------------------------------------------------------------------------*/
#include "GDW376_CFG.h"


/* 
********************************************************************************
* �� �� ����MVTD_Res
* ��    �ܣ�ÿ�չ����ʱ ������ֵ���ۼ�ʱ��ͳ����������
* ��    ����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
void MVTD_Res(u8 Pn)
{
    u32 *Addr32;
    struct PointDataStr *PnD;

    Addr32 = (u32*)(*(PnDTAddr + Pn));
    PnD = (struct PointDataStr*)(*(Addr32 + _PnD));

    if(DDLDJPn.Status != 'F')    //��û���ᣬ��Ҫ��¼
    {
      MovNByte(25, PnD->Z9010_D, PnD->R9010); //;����ǰ����ʾֵ��Ϊ������ʾֵ
      MovNByte(25, PnD->Z9020_D, PnD->R9020);
      MovNByte(20, PnD->Z9110_D, PnD->R9110);
      MovNByte(20, PnD->Z9120_D, PnD->R9120);  
     }
     PEZD_Save(Pn, &TBCD.Day);   //;���浱��������
     ZDPN_DME(Pn);               //;���̼���һ���յ���

    //;by zrt 20130924  ֻ���ڹ����ż������
    ByteN_Fill(32, PnD->DF25.ZPMax  , 0x00); 
    ByteN_Fill(24, PnD->DF26.ZPMN   , 0x00);
    ByteN_Fill(66, PnD->DF27.AVUUMin, 0x00);
    ByteN_Fill(14, PnD->DF28.IImbMin, 0x00);
    ByteN_Fill(38, PnD->DF29.AIUUMin, 0x00);
    ByteN_Fill(4,  PnD->DF30.SUUMin , 0x00);
    ByteN_Fill(10, PnD->DF31.Vmax , 0x00);
    ByteN_Fill(4,  PnD->DF49.PowerT , 0x00);
    ByteN_Fill(4,  PnD->DF50.MonthCutNum , 0x00);
    ByteN_Fill(4,  (u8*)&ZDS.F10.Flux_Day , 0x00);      //;����������
    ByteN_Fill(11,  PnD->DF209.MeterPowerStat , 0x00); 
}


/* 
********************************************************************************
* �� �� ����MVTM_Res
* ��    �ܣ�ÿ�¹����ʱ ������ֵ���ۼ�ʱ����������
* ��    ����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
void MVTM_Res(u8 Pn)
{
    u32 *Addr32;
    struct PointDataStr *PnD;

    Addr32 = (u32*)(*(PnDTAddr + Pn));
    PnD = (struct PointDataStr*)(*(Addr32 + _PnD));

    MovNByte(14, PnD->DB212_M, PnD->DB212);  //; by zrt 20130815 
    MovNByte(14, PnD->DB217_M, PnD->DB217);
    if(MDLDJPn.Status != 'F') 
    {
      MovNByte(25, PnD->Z9010_M, PnD->R9010);
      MovNByte(25, PnD->Z9020_M, PnD->R9020);
      MovNByte(20, PnD->Z9110_M, PnD->R9110);
      MovNByte(20, PnD->Z9120_M, PnD->R9120); //;����ǰ����ʾֵ��Ϊ������ʾֵ
    }
      PEZM_Save(Pn, &TBCD.Month); //;���浱��������
      ZDPN_DME(Pn);               //;���̼���һ���µ���

    //;by zrt 20130924   
    ByteN_Fill(32, PnD->DF33.ZPMax  , 0x00); 
    ByteN_Fill(24, PnD->DF34.ZPMN   , 0x00);
    ByteN_Fill(66, PnD->DF35.AVUUMin, 0x00);
    ByteN_Fill(16, PnD->DF36.IImbMin, 0x00);
    ByteN_Fill(38, PnD->DF37.AIUUMin, 0x00);
    ByteN_Fill(4,  PnD->DF38.SUUMin , 0x00);
    ByteN_Fill(12, PnD->DF39.Vmax , 0x00);
    ByteN_Fill(4,  PnD->DF51.PowerT , 0x00);
    ByteN_Fill(4,  PnD->DF52.MonthCutNum , 0x00);
    ByteN_Fill(4,  (u8*)&ZDS.F10.Flux_Month , 0x00);     //;����������
    ByteN_Fill(14,  PnD->DF213.ProgramNum , 0x00);
    ByteN_Fill(14,  PnD->DF214.CLKChangeNum , 0x00);
}



/* 
********************************************************************************
* �� �� ����HDJ_Pre
* ��    �ܣ���ʼ��Сʱ�������Td_h
* ��    ����
* ��    �أ� 
* ˵    ���������ʼ��������Ϊ15���ӣ��ݲ�֧�ַ��Ӷ���
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
void HDJ_Pre(u8 Pn)
{
    u8 i;
    u16 temp;
    u8 *adr8;
    u32 *Addr32;
    struct PointDataStr *PnD;

    Addr32 = (u32*)(*(PnDTAddr + Pn));
    PnD = (struct PointDataStr*)(*(Addr32 + _PnD));

    for(i=0; i<28; i++)//;��27��Сʱ�����ʶ�Ķ����ܶȳ�ʼ��Ϊ15����
    {
        if(i == 15) continue;
        temp = PnHDJ_Adr[i];
        adr8 = (u8*)(PnD->Start + temp); //;�׵�ַ+ƫ��
        *(adr8 + 1) = 0x01;
        *(adr8 + 0) = 0x00; 
    }
}


/* 
********************************************************************************
* �� �� ����HDJ_Move_T2
* ��    �ܣ������ֽ�Сʱ��������д���Ӧ�����ݱ�ʶ��
* ��    ����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
void HDJ_Move_T2(struct DJ_B2 *NDJ, u8 Date_DJ[])
{
    u8 Td, Min_Hex;
    u8 index;
    u8 Hour_Bcd;

    Min_Hex = THEX.Min;
    Hour_Bcd = TBCD.Hour;
	
    if(Min_Hex == 0)
    {
        Min_Hex = 60;
    }
//by zrt 20130719 �����һ�����Сʱʱ��	
     if(Hour_Bcd == 0) 
	 	NDJ->Td_h[0] = 0x23;
     else if(Hour_Bcd == 0x10 || Hour_Bcd == 0x20)
	 	NDJ->Td_h[0] = Hour_Bcd -7;
     else 
	 	NDJ->Td_h[0] = Hour_Bcd- 1;	

    Td = NDJ->Td_h[1];
    if(Td == 1)
    {
        if((Min_Hex % 15) == 0)
        {
            index = Min_Hex / 15;
        }
        else
        {
            index = 0;
        }
    }
    else if(Td == 2)
    {
        if ((Min_Hex % 30) == 0)
        {
            index = Min_Hex / 30;
        }
        else
        {
            index = 0;
        }
    }
    else if(Td == 3)
    {
        index = Min_Hex / 60;
    }
    else
    {
        index = 0;
    }
    if(index != 0)
    {
        index--;
        NDJ->Data[index][0] = Date_DJ[0];
        NDJ->Data[index][1] = Date_DJ[1];
    }
}

/* 
********************************************************************************
* �� �� ����HDJ_Move_T3
* ��    �ܣ������ֽ�Сʱ��������д���Ӧ�����ݱ�ʶ��
* ��    ����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
void HDJ_Move_T3(struct DJ_B3 *NDJ, u8 Date_DJ[])
{
    u8 Td, Min_Hex;
    u8 index;
    u8 Hour_Bcd;

    Min_Hex = THEX.Min;
    Hour_Bcd = TBCD.Hour;
	
    if(Min_Hex == 0)
    {
        Min_Hex = 60;
    }
//by zrt 20130719 �����һ�����Сʱʱ��	
     if(Hour_Bcd == 0) 
	 	NDJ->Td_h[0] = 0x23;
     else if(Hour_Bcd == 0x10 || Hour_Bcd == 0x20)
	 	NDJ->Td_h[0] = Hour_Bcd -7;
     else 
	 	NDJ->Td_h[0] = Hour_Bcd- 1;	
	
    Td = NDJ->Td_h[1];
    if(Td == 1)
    {
        if((Min_Hex % 15) == 0)
        {
            index = Min_Hex / 15;
        }
        else
        {
            index = 0;
        }
    }
    else if(Td == 2)
    {
        if((Min_Hex % 30) == 0)
        {
            index = Min_Hex / 30;
        }
        else
        {
            index = 0;
        }
    }
    else if(Td == 3)
    {
        index = Min_Hex / 60;
    }
    else
    {
        index = 0;
    }
    if(index != 0)
    {
        index--;
        NDJ->Data[index][0] = Date_DJ[0];
        NDJ->Data[index][1] = Date_DJ[1];
        NDJ->Data[index][2] = Date_DJ[2];
    }
}


/* 
********************************************************************************
* �� �� ����HDJ_Move_T4
* ��    �ܣ������ֽ�Сʱ��������д���Ӧ�����ݱ�ʶ��
* ��    ����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
void HDJ_Move_T4(struct DJ_B4 *NDJ, u8 *Date_DJ)
{
    u8 Td, Min_Hex;
    u8 index;
    u8 Hour_Bcd;

    Min_Hex = THEX.Min;
    Hour_Bcd = TBCD.Hour;
	
    if(Min_Hex == 0)
    {
        Min_Hex = 60;
    }
//by zrt 20130719 �����һ�����Сʱʱ��	
     if(Hour_Bcd == 0) 
	 	NDJ->Td_h[0] = 0x23;
     else if(Hour_Bcd == 0x10 || Hour_Bcd == 0x20)
	 	NDJ->Td_h[0] = Hour_Bcd -7;
     else 
	 	NDJ->Td_h[0] = Hour_Bcd- 1;	
	
    Td = NDJ->Td_h[1];
    if(Td == 1)
    {
        if((Min_Hex % 15) == 0)
        {
            index = Min_Hex / 15;
        }
        else
        {
            index = 0;
        }
    }
    else if(Td == 2)
    {
        if((Min_Hex % 30) == 0)
        {
            index = Min_Hex / 30;
        }
        else
        {
            index = 0;
        }
    }
    else if(Td == 3)
    {
        index = Min_Hex / 60;
    }
    else
    {
        index = 0;
    }
    if(index != 0)
    {
        index--;
        NDJ->Data[index][0] = *(Date_DJ + 0);
        NDJ->Data[index][1] = *(Date_DJ + 1);
        NDJ->Data[index][2] = *(Date_DJ + 2);
        NDJ->Data[index][3] = *(Date_DJ + 3);
    }
}


/* 
********************************************************************************
* �� �� ����HDJ_Res_T2
* ��    �ܣ�Сʱ����2�ֽ�����д��0xEE
* ��    ����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
void HDJ_Res_T2(struct DJ_B2 *NDJ)
{
    u8 i;
    NDJ->Td_h[0] &= 0xC0;
    NDJ->Td_h[0] += TBCD.Hour;
    for(i=0; i<4; i++)
    {
        NDJ->Data[i][0] = 0xEE;
        NDJ->Data[i][1] = 0xEE;
    }
}

/* 
********************************************************************************
* �� �� ����HDJ_Res_T3
* ��    �ܣ�Сʱ����3�ֽ�����д��0xEE
* ��    ����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
void HDJ_Res_T3(struct DJ_B3 *NDJ)
{
    u8 i;
    NDJ->Td_h[0] &= 0xC0;
    NDJ->Td_h[0] += TBCD.Hour;
    for(i=0; i<4; i++)
    {
        NDJ->Data[i][0] = 0xEE;
        NDJ->Data[i][1] = 0xEE;
        NDJ->Data[i][2] = 0xEE;
    }
}

/* 
********************************************************************************
* �� �� ����HDJ_Res_T4
* ��    �ܣ�Сʱ����4�ֽ�����д��0xEE
* ��    ����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
void HDJ_Res_T4(struct DJ_B4 *NDJ)
{
    u8 i;
    NDJ->Td_h[0] &= 0xC0;
    NDJ->Td_h[0] += TBCD.Hour;
    for(i=0; i<4; i++)
    {
        NDJ->Data[i][0] = 0xEE;
        NDJ->Data[i][1] = 0xEE;
        NDJ->Data[i][2] = 0xEE;
        NDJ->Data[i][3] = 0xEE;
    }
}

/* 
********************************************************************************
* �� �� ����HDJ_Reset
* ��    �ܣ�Сʱ�������ݸ�λ
* ��    ����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
void HDJ_Reset(u8 Pn)
{
    u32 *Addr32;
    struct PointDataStr *PnD;

    Addr32 = (u32*)(*(PnDTAddr + Pn));
    PnD = (struct PointDataStr*)(*(Addr32 + _PnD));

    HDJ_Res_T3(&PnD->NDJ_B630);
    HDJ_Res_T3(&PnD->NDJ_B631);
    HDJ_Res_T3(&PnD->NDJ_B632);
    HDJ_Res_T3(&PnD->NDJ_B633);
    HDJ_Res_T3(&PnD->NDJ_B640);
    HDJ_Res_T3(&PnD->NDJ_B641);
    HDJ_Res_T3(&PnD->NDJ_B642);
    HDJ_Res_T3(&PnD->NDJ_B643);
    HDJ_Res_T2(&PnD->NDJ_B611);
    HDJ_Res_T2(&PnD->NDJ_B612);
    HDJ_Res_T2(&PnD->NDJ_B613);
    HDJ_Res_T3(&PnD->NDJ_B621);
    HDJ_Res_T3(&PnD->NDJ_B622);
    HDJ_Res_T3(&PnD->NDJ_B623);
    HDJ_Res_T3(&PnD->NDJ_B6E5);
    HDJ_Res_T4(&PnD->NDJ_9010_1);
    HDJ_Res_T4(&PnD->NDJ_9110_1);
    HDJ_Res_T4(&PnD->NDJ_9020_1);
    HDJ_Res_T4(&PnD->NDJ_9120_1);
    HDJ_Res_T4(&PnD->NDJ_9010);
    HDJ_Res_T4(&PnD->NDJ_9110);
    HDJ_Res_T4(&PnD->NDJ_9020);
    HDJ_Res_T4(&PnD->NDJ_9120);
    HDJ_Res_T2(&PnD->NDJ_B650);
    HDJ_Res_T2(&PnD->NDJ_B651);
    HDJ_Res_T2(&PnD->NDJ_B652);
    HDJ_Res_T2(&PnD->NDJ_B653);
}


/* 
********************************************************************************
* �� �� ����HDJ_Program
* ��    �ܣ�Сʱ�������
* ��    ����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
void HDJ_Program(u8 Pn)
{
    u32 *Addr32;
    struct PointDataStr *PnD;

    Addr32 = (u32*)(*(PnDTAddr + Pn));
    PnD = (struct PointDataStr*)(*(Addr32 + _PnD));
    if((PnD->CB_Time[0] != 0) || (PnD->CB_Time[1] != 0) || (PnD->CB_Time[2] != 0) ||
        (PnD->CB_Time[3] != 0) || (PnD->CB_Time[4] != 0)) //;��ǰ���ݴ���ʱ���ܶ���
    {
        HDJ_Move_T3(&PnD->NDJ_B630, PnD->RB630);
        HDJ_Move_T3(&PnD->NDJ_B631, PnD->RB631);
        HDJ_Move_T3(&PnD->NDJ_B632, PnD->RB632);
        HDJ_Move_T3(&PnD->NDJ_B633, PnD->RB633);
        HDJ_Move_T3(&PnD->NDJ_B640, PnD->RB640);
        HDJ_Move_T3(&PnD->NDJ_B641, PnD->RB641);
        HDJ_Move_T3(&PnD->NDJ_B642, PnD->RB642);
        HDJ_Move_T3(&PnD->NDJ_B643, PnD->RB643);

        HDJ_Move_T2(&PnD->NDJ_B611, PnD->RB611);
        HDJ_Move_T2(&PnD->NDJ_B612, PnD->RB612);
        HDJ_Move_T2(&PnD->NDJ_B613, PnD->RB613);
        HDJ_Move_T3(&PnD->NDJ_B621, PnD->RB621);
        HDJ_Move_T3(&PnD->NDJ_B622, PnD->RB622);
        HDJ_Move_T3(&PnD->NDJ_B623, PnD->RB623);
        HDJ_Move_T3(&PnD->NDJ_B6E5, PnD->RB6E5);

        HDJ_Move_T4(&PnD->NDJ_9010, PnD->R9010_D); //;��������յ���
        HDJ_Move_T4(&PnD->NDJ_9110, PnD->R9110_D);
        HDJ_Move_T4(&PnD->NDJ_9020, PnD->R9020_D);
        HDJ_Move_T4(&PnD->NDJ_9120, PnD->R9120_D);

        HDJ_Move_T4(&PnD->NDJ_9010_1, PnD->R9010 + 1); //;�������ʾֵ
        HDJ_Move_T4(&PnD->NDJ_9110_1, PnD->R9110);
        HDJ_Move_T4(&PnD->NDJ_9020_1, PnD->R9020 + 1);
        HDJ_Move_T4(&PnD->NDJ_9120_1, PnD->R9120);

        HDJ_Move_T2(&PnD->NDJ_B650, PnD->RB650);
        HDJ_Move_T2(&PnD->NDJ_B651, PnD->RB651);
        HDJ_Move_T2(&PnD->NDJ_B652, PnD->RB652);
        HDJ_Move_T2(&PnD->NDJ_B653, PnD->RB653);
        
        if(TBCD.Min == 0x00) //;����Сʱ�л�ʱ
        {
            EW.MoveByte = 'M';
            MovNByte(56, PnD->ODJ_B630.Td_h,   PnD->NDJ_B630.Td_h);    //;��,A,B,C�й�����
            MovNByte(56, PnD->ODJ_B640.Td_h,   PnD->NDJ_B640.Td_h);    //;��,A,B,C�޹�����
            MovNByte(30, PnD->ODJ_B611.Td_h,   PnD->NDJ_B611.Td_h);    //;A,B,C���ѹ
//;by zrt 20130719            MovNByte(40, PnD->ODJ_B621.Td_h,   PnD->NDJ_B621.Td_h);    //;A,B,C������Լ����ߵ���
            MovNByte(56, PnD->ODJ_B621.Td_h,   PnD->NDJ_B621.Td_h);    //;A,B,C������Լ����ߵ���
            MovNByte(72, PnD->ODJ_9010.Td_h,   PnD->NDJ_9010.Td_h);    //;Сʱ�����յ�����
            MovNByte(72, PnD->ODJ_9010_1.Td_h, PnD->NDJ_9010_1.Td_h);  //;Сʱ�����ܵ���ʾֵ
            MovNByte(40, PnD->ODJ_B650.Td_h,   PnD->NDJ_B650.Td_h);    //;��,A,B,C�๦������
            EW.MoveByte = 0;
            HDJ_Reset(Pn); //;���¿�ʼһ��Сʱ��Сʱ����ǰ��Сʱ�������ݸ�λΪû�и�����
        }
    }
}

/* 
********************************************************************************
* �� �� ����
* ��    �ܣ��ն��ն��ᴦ����
* ��    ����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
void DDJ_Ctrl(void)
{
    if(DDJPn.Status == 'E')
    {
        u8 Date[3];
        Day_Last(&TBCD.Day, Date);      //;�������һ�յ�����         
        DDJ_Save(DDJPn.Pn,  Date);      //;�ն���
        MVTD_Res(DDJPn.Pn);             //;��ͳ������
        DDJPn.Status = 'S';             //;�������          
    }
    
    if(DDJPn.Status == 'S')
    {
        DDJPn.Pn++;
        DDJPn.Status = 'E';
        if(DDJPn.Pn >= PNMAXNUM)
        {
            DDJPn.Pn = 0;
            DDJPn.Status = 0; //;���в����㶳�����
        }
    }
}


/* 
********************************************************************************
* �� �� ����BDJ_Ctrl
* ��    �ܣ��ն˳����ն���
* ��    ����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
void BDJ_Ctrl(void)
{
    if(BDJPn.Status == 'E')
    {
        BDJ_Save(BDJPn.Pn, &TBCD.Day);
        BDJPn.Status = 'S';//;�������
    }
   
    if(BDJPn.Status == 'S')
    {
        BDJPn.Pn++;
        BDJPn.Status = 'E';
        if(BDJPn.Pn >= PNMAXNUM)
        {
            BDJPn.Pn = 0;
            BDJPn.Status = 0;
        }
    }
}



/* 
********************************************************************************
* �� �� ����MDJ_Ctrl
* ��    �ܣ��¶��ᴦ����
* ��    ����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
void MDJ_Ctrl(void)
{
    if(MDJPn.Status == 'E')
    {
        u8 Date[2];        
        Month_Last(&TBCD.Month, Date);    //;�������һ�µ��·�  
        MDJ_Save(MDJPn.Pn, Date);         //;�¶���
        MVTM_Res(DDJPn.Pn);               //;��ͳ������  
        MDJPn.Status = 'S';//;�������
    }
   
    if(MDJPn.Status == 'S')
    {
        MDJPn.Pn++;
        MDJPn.Status = 'E';
        if(MDJPn.Pn >= PNMAXNUM)
        {
            MDJPn.Pn = 0;
            MDJPn.Status = 0;
        }
    }
}


/* 
********************************************************************************
* �� �� ����QDJ_Ctrl
* ��    �ܣ���0�֣�15�֣�30�֣�45�ֶԲ�����������߶���
* ��    ����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
void QDJ_Ctrl(void)
{
    if(QDJPn.Status == 'E')
    {
        QDJPn.Status = 'S';//;�������
        CDJ_Save(QDJPn.Pn, &TBCD.Min); //;���߶���
        HDJ_Program(QDJPn.Pn); //;Сʱ����
        HDJ_Save(QDJPn.Pn); //;Сʱ���ᱣ��
        MVT_Save(QDJPn.Pn); //;����һ��ͳ��ʱ��      
//;        PEZD_Save(QDJPn.Pn, &TBCD.Min);    by zrt 20130907
        PEZD_Save(QDJPn.Pn, &TBCD.Day);
    }
    
    if(QDJPn.Status == 'S')
    {
        QDJPn.Pn++;
        QDJPn.Status = 'E';
        if(QDJPn.Pn >= PNMAXNUM)
        {
            QDJPn.Pn = 0;
            QDJPn.Status = 0;
        }
    }
}


/* 
********************************************************************************
* �� �� ����ZD_DJPN_CHK
* ��    �ܣ�����ʹ�ܼ��
* ��    ����
* ��    �أ� 
* ˵    ����ÿ��ִ��һ��
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
/*
void ZD_DJPN_CHK(void)
{
    if(((THEX.Min%15)==0) && (TBCD.Sec==0x00))  //;���߶���
    {
        if(QDJPn.Status == 0)
        {
            QDJPn.Pn = 0; //;�ӵ�һ�������㿪ʼ����
            QDJPn.Status = 'E'; //;���Ὺʼ
        }
    }    
    
    if((TBCD.Hour==0x00) && (TBCD.Min==0x00) && (TBCD.Sec==0x00)) //;�������㣬�ն���
    {
        if(DDJPn.Status == 0)
        {
            DDJPn.Pn = 0; //;�ӵ�һ�������㿪ʼ����
            DDJPn.Status = 'E'; //;���Ὺʼ
        }
    }
        
    if((TBCD.Day==0x01) && (TBCD.Hour==0x00) && 
       (TBCD.Min==0x00) && (TBCD.Sec==0x00)) //;���µĿ�ʼ
    {
        if(MDJPn.Status == 0) 
        {
            MDJPn.Pn = 0; //;�ӵ�һ�������㿪ʼ����
            MDJPn.Status = 'E'; //;���Ὺʼ
        }
    }
    
    ZD_CBD_CHK();
}
*/
void ZD_DJPN_CHK(void)
{
	static struct
	{
		u8 ChkDDJ	:1;
		u8 AddDDJ	:1;
		u8 DDJcnt	:2;
		u8 ChkMDJ	:1;
		u8 AddMDJ	:1;
		u8 MDJcnt	:2;
	} AddDJ;	//;��������
	if ((TBCD.Sec == 0x00) &&(TBCD.Day !=0x00))  //;ÿ���Ӽ��һ��
	{
		if(!AddDJ.ChkDDJ) //;��û����ն���
		{
			u8 Date[3];
			u8 node;
			AddDJ.ChkDDJ = true;
			Day_Last(&TBCD.Day, Date);      //;�������һ�յ�����
			node = DDJ_FTP(1,Date);
			if (node == 0xFF)  //;����0������ݻ�δ����
			{
				AddDJ.AddDDJ = true;
//				AddDJ.DDJcnt = 1;
				AddDJ.DDJcnt = 0;				
             			DDLDJPn.Status = 'T'; //;���һ����־λ'T'��ǰ�����by zrt 20130930							
			}
			else     //;����0���Ѷ���
			{
				AddDJ.AddDDJ = false;
				AddDJ.DDJcnt = 0;
				DDLDJPn.Status = 'F'; //;���һ����־λ'F'��ǰ�������by zrt 20130930				
			}
		}
		if (AddDJ.AddDDJ) //;��Ҫ����������
		{
			if (AddDJ.DDJcnt  != 0)
			{
				AddDJ.DDJcnt --;
			}
			else if(DDJPn.Status == 0)  //;�����ܹ��ȴ�1���ӣ���֤���ػ�������
			{
				AddDJ.AddDDJ = false;
				DDJPn.Pn = 0; //;�ӵ�һ�������㿪ʼ����
				DDJPn.Status = 'E'; //;���Ὺʼ
			}
		}
		if(!AddDJ.ChkMDJ) //;��û����¶���
		{
			u8 Date[2];
			u8 node;
			AddDJ.ChkMDJ = true;
			Month_Last(&TBCD.Month, Date);    //;�������һ�µ��·� 
			node = MDJ_FTP(1,Date);
			if (node == 0xFF)  //;����0������ݻ�δ����
			{
				AddDJ.AddMDJ = true;
//				AddDJ.MDJcnt = 1;
				AddDJ.MDJcnt = 0;				
             			MDLDJPn.Status = 'T'; //;���һ����־λ'T'��ǰ�����by zrt 20130930						
			}
			else
			{
				AddDJ.AddMDJ = false;
				AddDJ.MDJcnt = 0;
				MDLDJPn.Status = 'F'; //;���һ����־λ'F'��ǰ�������by zrt 20130930				
			}
		}
		if (AddDJ.AddMDJ) //;��Ҫ����������
		{
			if (AddDJ.MDJcnt  != 0)
			{
				AddDJ.MDJcnt --;
			}
			else if(MDJPn.Status == 0)  //;�����ܹ��ȴ�1���ӣ���֤���ػ�������
			{
				AddDJ.AddMDJ = false;
				MDJPn.Pn = 0; //;�ӵ�һ�������㿪ʼ����
				MDJPn.Status = 'E'; //;���Ὺʼ		
			}
		}
	}
	if(((THEX.Min%15)==0) && (TBCD.Sec==0x00))  //;���߶���
	{
		if(QDJPn.Status == 0)
		{
			QDJPn.Pn = 0; //;�ӵ�һ�������㿪ʼ����
			QDJPn.Status = 'E'; //;���Ὺʼ
		}
	}    
	
	if((TBCD.Hour==0x00) && (TBCD.Min==0x00) && (TBCD.Sec==0x00) && (TBCD.Day!=0x00)) //;�������㣬�ն���
	{
		if(DDJPn.Status == 0)
		{
			DDJPn.Pn = 0; //;�ӵ�һ�������㿪ʼ����
			DDJPn.Status = 'E'; //;���Ὺʼ
           		DDLDJPn.Status = 'Y'; //;���һ����־λ'T'��ǰ�����by zrt 20130930				
		}
	}
	    
	if((TBCD.Day==0x01) && (TBCD.Hour==0x00) && 
	   (TBCD.Min==0x00) && (TBCD.Sec==0x00)) //;���µĿ�ʼ
	{
		if(MDJPn.Status == 0) 
		{
			MDJPn.Pn = 0; //;�ӵ�һ�������㿪ʼ����
			MDJPn.Status = 'E'; //;���Ὺʼ
      MDLDJPn.Status = 'Y'; //;���һ����־λ'T'��ǰ�����by zrt 20130930				
		}
	}

	ZD_CBD_CHK();
}

/* 
********************************************************************************
* �� �� ����ZD_DJPN
* ��    �ܣ������㶳�����
* ��    ����ÿ250msִ��һ��
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
void ZD_DJPN(void)
{
    QDJ_Ctrl(); //;���߶���
    DDJ_Ctrl(); //;�ն���
    BDJ_Ctrl(); //;�����ն���
    MDJ_Ctrl(); //;�¶��� 
}


