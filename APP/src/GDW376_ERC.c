/*
********************************************************************************
*  �� �� ����GDW376_ERC.C
*
*  �ļ�������GDW376.1�涨���쳣�¼��ļ�¼Ԥ����
*
*  ����оƬ��
*
*  �� �� �ˣ�
*
*  �� �� �ţ�
*
*  �޸ļ�¼��1.�����������δ�ɹ����¼���һ�η������򲻷�����һ���¼�
********************************************************************************
*/
#include "GDW376_CFG.h"
#include "GDW376_ERC_SMS.h"
#include "GDW376_MMU.h"

u8 UALostTMin,UBLostTMin,UCLostTMin;


 /* 
********************************************************************************
* �� �� ����ERC1 ���ݳ�ʼ���Ͱ汾�����¼
* ��    �ܣ�
* ��    ����Type = 1 ��������������ʼ��
*           Type = 2 �ն˰汾���
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
void ERC1_Judge(u8 Type)
{
    ERCP0.ERC1.Status[0] = 0x01;
    
    ERCP0.ERC1.OTime[0] = TBCD.Min;
    ERCP0.ERC1.OTime[1] = TBCD.Hour;
    ERCP0.ERC1.OTime[2] = TBCD.Day;
    ERCP0.ERC1.OTime[3] = TBCD.Month;
    ERCP0.ERC1.OTime[4] = TBCD.Year;
    
    ERCP0.ERC1.Mark[0] = Type;

    ERCP0.ERC1.Data[0] = SofewareOldVer[0];
    ERCP0.ERC1.Data[1] = SofewareOldVer[1];
    ERCP0.ERC1.Data[2] = SofewareOldVer[2];
    ERCP0.ERC1.Data[3] = SofewareOldVer[3];

    ERCP0.ERC1.Data[4] = ZDCFG_F1[12];   //;�ն�����汾��
    ERCP0.ERC1.Data[5] = ZDCFG_F1[13];
    ERCP0.ERC1.Data[6] = ZDCFG_F1[14];
    ERCP0.ERC1.Data[7] = ZDCFG_F1[15];
}


 /* 
********************************************************************************
* �� �� ����ERC17_Judge
* ��    �ܣ�����˲ʱ�����ж��Ƿ������ѹ��������ƽ���Խ���¼�ERC17
* ��    ����
* ��    �أ� 
* ˵    ��
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��2011-09-26 09:26:45
********************************************************************************
*/
void ERC17_Judge(struct PointDataStr *PnD, struct ERCPnData *ERCPn)
{   
    u16 Upn, Ipn;
    
    
    ERCPn->ERC17.BPH_OM = 0; //;������쳣��־
    ERCPn->ERC17.BPH_RM = 0;

    if(((ERCPn->ERC17.Status_O & 0x01) != 0x01) && 
        ((ERCPn->ERC17.Status_N & 0x01) == 0x01))
    {
        ERCPn->ERC17.BPH_OM |= 0x01;  //;��ѹ��ƽ�ⷢ��
    }
    else if(((ERCPn->ERC17.Status_O & 0x01) == 0x01) && 
            ((ERCPn->ERC17.Status_N & 0x01) != 0x01))
    {
        ERCPn->ERC17.BPH_RM |= 0x01; //;��ƽ�����
    }
    
    if(((ERCPn->ERC17.Status_O & 0x02) != 0x02) && 
       ((ERCPn->ERC17.Status_N & 0x02) == 0x02))
    {
        ERCPn->ERC17.BPH_OM |= 0x02; //;������ƽ�⿪ʼ
    }
    else if(((ERCPn->ERC17.Status_O & 0x02) == 0x02) && 
            ((ERCPn->ERC17.Status_N & 0x02) != 0x02))
    {
        ERCPn->ERC17.BPH_RM |= 0x02; //;����
    }
    
    if((ERCPn->ERC17.BPH_OM) || (ERCPn->ERC17.BPH_RM))
    {
        if (ERCPn->ERC17.BPH_OM)
        {
            ERCPn->ERC17.Status[0] = 0x01;       //;��ƽ���¼�����
        }
        
        if (ERCPn->ERC17.BPH_RM)
        {
            ERCPn->ERC17.Status[1] = 0x01;       //;��ƽ���¼��ָ�
        }
        
        Upn = ERCPn->ERC17.Upn;
        Ipn = ERCPn->ERC17.Ipn;
        ERCPn->ERC17.Data[1]  = (Upn / 1000) *16+((Upn % 1000) / 100);
        Upn %= 100;
        ERCPn->ERC17.Data[0] = (Upn / 10) *16+(Upn % 10); //;��ѹ��ƽ���
        ERCPn->ERC17.Data[3]  = (Ipn / 1000) *16+((Ipn % 1000) / 100);
        Ipn %= 100;
        ERCPn->ERC17.Data[2]  = (Ipn / 10) *16+(Ipn % 10); //;������ƽ���
        ERCPn->ERC17.Data[4]  = PnD->RB611[0];//;Ua
        ERCPn->ERC17.Data[5]  = PnD->RB611[1]; 
        ERCPn->ERC17.Data[6]  = PnD->RB612[0];//;Ub
        ERCPn->ERC17.Data[7]  = PnD->RB612[1]; 
        ERCPn->ERC17.Data[8]  = PnD->RB613[0];//;Uc
        ERCPn->ERC17.Data[9]  = PnD->RB613[1]; 
        ERCPn->ERC17.Data[10] = PnD->RB621[0]; //;Ia
        ERCPn->ERC17.Data[11] = PnD->RB621[1]; 
        ERCPn->ERC17.Data[12] = PnD->RB621[2];
        ERCPn->ERC17.Data[13] = PnD->RB622[0]; //;Ib
        ERCPn->ERC17.Data[14] = PnD->RB622[1]; 
        ERCPn->ERC17.Data[15] = PnD->RB622[2]; 
        ERCPn->ERC17.Data[16] = PnD->RB623[0]; //;Ic
        ERCPn->ERC17.Data[17] = PnD->RB623[1]; 
        ERCPn->ERC17.Data[18] = PnD->RB623[2];
    }
    
    ERCPn->ERC17.Status_O = ERCPn->ERC17.Status_N;
}



/*----------------------------------------------------------------------------*
 |  ERC21-4: �ն˹��ϼ�¼
 *----------------------------------------------------------------------------*/
void ERC21_4Judge(u8 Occur)
{     
    if (Occur == 1)
    {
        if ((Port1.PnErrNum + Port2.PnErrNum) == PNMAXNUM)
        {
            if (ERCP0.ERC21.State != 0x01)
            { 
                ERCP0.ERC21.State = 0x01;
                ERCP0.ERC21.Status[0] = 0x01; //;�ն˹���
                ERCP0.ERC21.OTime[0] = TBCD.Min;
                ERCP0.ERC21.OTime[1] = TBCD.Hour;
                ERCP0.ERC21.OTime[2] = TBCD.Day;
                ERCP0.ERC21.OTime[3] = TBCD.Month;
                ERCP0.ERC21.OTime[4] = TBCD.Year;
                ERCP0.ERC21.Data[0]  = 4;
            }
        }
    }
    else if (Occur == 2)
    {
        ERCP0.ERC21.State = 0;  
    }
}


/* 
********************************************************************************
* �� �� ����ERC24_Judge
* ��    �ܣ���ѹԽ���¼�
* ��    ����
* ��    �أ� 
* ˵    ����u16 PnSt->DYYX_N : xxxx xxxx xxxx xxxx
*           0~3  bit  ��ʾ��A��Խ�����ޣ����ޣ� �����ޣ�����
*           4~7  bit  ��ʾ��B��Խ�����ޣ����ޣ� �����ޣ�����
*           8~11 bit  ��ʾ��C��Խ�����ޣ����ޣ� �����ޣ�����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
void ERC24_Judge(struct PointDataStr *PnD, struct ERCPnData *ERCPn)
{
    
    ERCPn->ERC24.YSSXOM = 0;
    ERCPn->ERC24.YSSXRM = 0;
    ERCPn->ERC24.YXXXOM = 0;
    ERCPn->ERC24.YXXXRM = 0;
    
    /*------------------- Խ���¼�������ָ� ------------------------------*/
    if(((ERCPn->ERC24.Status_O & 0x01) != 0x01) && ((ERCPn->ERC24.Status_N  & 0x01) == 0x01))
    {
        ERCPn->ERC24.YSSXOM |= 0x01; //;A���ѹԽ�����޷���
    }
    else if(((ERCPn->ERC24.Status_O & 0x01) == 0x01) && ((ERCPn->ERC24.Status_N  & 0x01) != 0x01))
    {
        ERCPn->ERC24.YSSXRM |= 0x01; //;A���ѹԽ�����޻ָ�
    }
    if(((ERCPn->ERC24.Status_O & 0x02) != 0x02) && ((ERCPn->ERC24.Status_N  & 0x02) == 0x02))
    {
        ERCPn->ERC24.YXXXOM |= 0x01;   //;A���ѹԽ�����޷���
    }
    else if(((ERCPn->ERC24.Status_O & 0x02) == 0x02) && ((ERCPn->ERC24.Status_N & 0x02) != 0x02))
    {
        ERCPn->ERC24.YXXXRM |= 0x01;   //;A���ѹԽ�����޻ָ�
    }


    if(((ERCPn->ERC24.Status_O & 0x04) != 0x04) && ((ERCPn->ERC24.Status_N & 0x04) == 0x04))
    {
        ERCPn->ERC24.YSSXOM |= 0x02; //;B���ѹԽ�����޷���
    }
    else if(((ERCPn->ERC24.Status_O &0x04) == 0x04) && ((ERCPn->ERC24.Status_N &0x04) != 0x04))
    {
        ERCPn->ERC24.YSSXRM |= 0x02; //;B���ѹԽ�����޻ָ�
    }
    if(((ERCPn->ERC24.Status_O & 0x08) != 0x08) && ((ERCPn->ERC24.Status_N & 0x08) == 0x08))
    {
        ERCPn->ERC24.YXXXOM |= 0x02; //;B���ѹԽ�����޷���
    }
    else if(((ERCPn->ERC24.Status_O & 0x08) == 0x08) && ((ERCPn->ERC24.Status_N & 0x08) != 0x08))
    {
        ERCPn->ERC24.YXXXRM |= 0x02; //;B���ѹԽ�����޻ָ�
    }

    if(((ERCPn->ERC24.Status_O & 0x10) != 0x10) && ((ERCPn->ERC24.Status_N & 0x10) == 0x10))
    {
        ERCPn->ERC24.YSSXOM |= 0x04; //;C���ѹԽ�����޷���
    }
    else if(((ERCPn->ERC24.Status_O & 0x10) == 0x10) && ((ERCPn->ERC24.Status_N & 0x10) != 0x10))
    {
        ERCPn->ERC24.YSSXRM |= 0x04; //;C���ѹԽ�����޻ָ�
    }
    if(((ERCPn->ERC24.Status_O & 0x20) != 0x20) && ((ERCPn->ERC24.Status_N & 0x20) == 0x20))
    {
        ERCPn->ERC24.YXXXOM |= 0x04; //;C���ѹԽ�����޷���
    }
    else if(((ERCPn->ERC24.Status_O & 0x20) == 0x20) && ((ERCPn->ERC24.Status_N & 0x20) != 0x20))
    {
        ERCPn->ERC24.YXXXRM |= 0x04;  //;C���ѹԽ�����޻ָ�
    }
    
    if(ERCPn->ERC24.YSSXOM || ERCPn->ERC24.YSSXRM || ERCPn->ERC24.YXXXOM || ERCPn->ERC24.YXXXRM)
    {
        if (ERCPn->ERC24.YSSXOM)
        {
            ERCPn->ERC24.Status[0] = 0x01;
        }
        if (ERCPn->ERC24.YSSXRM)
        {
            ERCPn->ERC24.Status[1] = 0x01;
        }
        if (ERCPn->ERC24.YXXXOM)
        {
            ERCPn->ERC24.Status[2] = 0x01;
        }
        if (ERCPn->ERC24.YXXXRM)
        {
            ERCPn->ERC24.Status[3] = 0x01;
        }
        ERCPn->ERC24.Data[0] = PnD->RB611[0];
        ERCPn->ERC24.Data[1] = PnD->RB611[1]; //;Ua
        ERCPn->ERC24.Data[2] = PnD->RB612[0];
        ERCPn->ERC24.Data[3] = PnD->RB612[1]; //;Ub
        ERCPn->ERC24.Data[4] = PnD->RB613[0];
        ERCPn->ERC24.Data[5] = PnD->RB613[1]; //;Uc
    }
    
    ERCPn->ERC24.Status_O = ERCPn->ERC24.Status_N;
}


/* 
********************************************************************************
* �� �� ����ERC25_Judge
* ��    �ܣ�����Խ�޼�¼
* ��    ����P1P: �����������ַ
*           P1D: ���������ݵ�ַ
*           ERCNPn�� ������Pn��ERC25�ķ����¼�
* ��    �أ� 
* ˵    ����u8 ERCPn->ERC25.Status_N :  xxxx xxxx
*           0��1 bit ��A��Խ���ޣ�������  2��3 bit ��B��Խ���ޣ�������
*           4��5 bit ��C��Խ���ޣ�������  6,   bit : �������Խ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
void ERC25_Judge(struct PointDataStr *PnD, struct ERCPnData *ERCPn)
{

    ERCPn->ERC25.YSSXOM = 0;
    ERCPn->ERC25.YSSXRM = 0;
    ERCPn->ERC25.YSXOM = 0;
    ERCPn->ERC25.YSXRM = 0;
    
    /*--------------------------- Խ���¼�������ָ� ------------------------*/
    if(((ERCPn->ERC25.Status_O & 0x01) != 0x01) && ((ERCPn->ERC25.Status_N & 0x01) == 0x01))
    {
        ERCPn->ERC25.YSSXOM |= 0x01; //;A�����Խ�����޷���
    }
    else if(((ERCPn->ERC25.Status_O & 0x01) == 0x01) && ((ERCPn->ERC25.Status_N & 0x01) != 0x01))
    {
        ERCPn->ERC25.YSSXRM |= 0x01;  //;A�����Խ�����޻ָ�
    }

    if(((ERCPn->ERC25.Status_O &0x02) != 0x02) && ((ERCPn->ERC25.Status_N & 0x02) == 0x02))
    {
        ERCPn->ERC25.YSXOM |= 0x01;  //;A�����Խ���޷���
    }
    else if(((ERCPn->ERC25.Status_O & 0x02) == 0x02) && ((ERCPn->ERC25.Status_N & 0x02) != 0x02))
    {
        ERCPn->ERC25.YSXRM |= 0x01;  //;A�����Խ���޻ָ�
    }


    if(((ERCPn->ERC25.Status_O & 0x04) != 0x04) && ((ERCPn->ERC25.Status_N & 0x04) == 0x04))
    {
        ERCPn->ERC25.YSSXOM |= 0x02;   //;B�����Խ�����޷���
    }
    else if(((ERCPn->ERC25.Status_O & 0x04) == 0x04) && ((ERCPn->ERC25.Status_N & 0x04) != 0x04))
    {
        ERCPn->ERC25.YSSXRM |= 0x02; //;B�����Խ�����޻ָ�
    }
    if(((ERCPn->ERC25.Status_O & 0x08) != 0x08) && ((ERCPn->ERC25.Status_N & 0x08) == 0x08))
    {
        ERCPn->ERC25.YSXOM |= 0x02; //;B�����Խ���޷���
    }
    else if(((ERCPn->ERC25.Status_O & 0x08) == 0x08) && ((ERCPn->ERC25.Status_N & 0x08) != 0x08))
    {
        ERCPn->ERC25.YSXRM |= 0x02;  //;B�����Խ���޻ָ�
    }


    if(((ERCPn->ERC25.Status_O & 0x10) != 0x10) && ((ERCPn->ERC25.Status_N & 0x10) == 0x10))
    {
        ERCPn->ERC25.YSSXOM |= 0x04; //;C�����Խ�����޷���
    }
    else if(((ERCPn->ERC25.Status_O & 0x10) == 0x10) && ((ERCPn->ERC25.Status_N & 0x10) != 0x10))
    {
        ERCPn->ERC25.YSSXRM |= 0x04; //;C�����Խ�����޻ָ�
    }
    if(((ERCPn->ERC25.Status_O & 0x20) != 0x20) && ((ERCPn->ERC25.Status_N & 0x20) == 0x20))
    {
        ERCPn->ERC25.YSXOM |= 0x04; //;C�����Խ���޷���
    }
    else if(((ERCPn->ERC25.Status_O & 0x20) == 0x20) && ((ERCPn->ERC25.Status_N & 0x20) != 0x20))
    {
        ERCPn->ERC25.YSXRM |= 0x04;  //;C�����Խ���޻ָ�
    }

    if((ERCPn->ERC25.YSSXOM || ERCPn->ERC25.YSSXRM || ERCPn->ERC25.YSXOM) || (ERCPn->ERC25.YSXRM))
    {
        if (ERCPn->ERC25.YSSXOM) //;zrt_20130614
        {
            ERCPn->ERC25.Status[0] = 0x01;
        }
        if (ERCPn->ERC25.YSSXRM)
        {
            ERCPn->ERC25.Status[1] = 0x01;
        }
        if (ERCPn->ERC25.YSXOM)
        {
            ERCPn->ERC25.Status[2] = 0x01;
        }
        if (ERCPn->ERC25.YSXRM)
        {
            ERCPn->ERC25.Status[3] = 0x01;
        }  //;��������Խ�޲����͵�����
        ERCPn->ERC25.Data[0] = PnD->RB621[0];//;Ia
        ERCPn->ERC25.Data[1] = PnD->RB621[1];
        ERCPn->ERC25.Data[2] = PnD->RB621[2]; 
        ERCPn->ERC25.Data[3] = PnD->RB622[0];//;Ib
        ERCPn->ERC25.Data[4] = PnD->RB622[1]; 
        ERCPn->ERC25.Data[5] = PnD->RB622[2];
        ERCPn->ERC25.Data[6] = PnD->RB623[0];//;Ic
        ERCPn->ERC25.Data[7] = PnD->RB623[1]; 
        ERCPn->ERC25.Data[8] = PnD->RB623[2];
    }
    
    ERCPn->ERC25.Status_O = ERCPn->ERC25.Status_N;
}


/* 
********************************************************************************
* �� �� ����ERC26_Judge
* ��    �ܣ����ڹ���Խ�޼�¼
* ��    ����u8 SYX_N  xxx xxx
*           0 bit ��Խ���� 1 bit :Խ������
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
void ERC26_Judge(struct PoPStr  *PnP, struct PointDataStr *PnD, struct ERCPnData *ERCPn)
{

    if(((ERCPn->ERC26.Status_O & 0x01) != 0x01) && ((ERCPn->ERC26.Status_N & 0x01) == 0x01))
    {
        ERCPn->ERC26.Status[0] |= 0x01; //;Խ���޷���
    }
    else if(((ERCPn->ERC26.Status_O & 0x01) == 0x01) && ((ERCPn->ERC26.Status_N & 0x01) != 0x01))
    {
        ERCPn->ERC26.Status[1] |= 0x01;  //;Խ���޻ָ�
    }
    
    if(((ERCPn->ERC26.Status_O & 0x02) != 0x02) && ((ERCPn->ERC26.Status_N & 0x02) == 0x02))
    {
        ERCPn->ERC26.Status[2] |= 0x01; //;Խ�����޷���
    }
    else if(((ERCPn->ERC26.Status_O & 0x02) == 0x02) && ((ERCPn->ERC26.Status_N & 0x02) != 0x02))
    {
        ERCPn->ERC26.Status[3] |= 0x01; //;Խ�����޻ָ�
    }
    
    if(ERCPn->ERC26.Status[0] || ERCPn->ERC26.Status[1] || ERCPn->ERC26.Status[2] || ERCPn->ERC26.Status[3])
    {
        ERCPn->ERC26.Data[0] = PnD->RB660[0];
        ERCPn->ERC26.Data[1] = PnD->RB660[1];
        ERCPn->ERC26.Data[2] = PnD->RB660[2]; //;����ʱ�����ڹ���
        
        if((ERCPn->ERC26.Status[0]) || (ERCPn->ERC26.Status[1]))
        {
            ERCPn->ERC26.Data[3] = PnP->F26.SH[0];
            ERCPn->ERC26.Data[4] = PnP->F26.SH[1];
            ERCPn->ERC26.Data[5] = PnP->F26.SH[2];
        }
        else
        {
            ERCPn->ERC26.Data[3] = PnP->F26.SHH[0];
            ERCPn->ERC26.Data[4] = PnP->F26.SHH[1];
            ERCPn->ERC26.Data[5] = PnP->F26.SHH[2];
        }
    }
    
    ERCPn->ERC26.Status_O = ERCPn->ERC26.Status_N;
}


/* 
********************************************************************************
* �� �� ����ERC13_Judge
* ��    �ܣ�ERC13��������Ϣ
* ��    ����
* ��    �أ� 
* ˵    ����ͣ���������Ƿѹ����(����by zrt 20130904)
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
void ERC13_Judge(struct PointDataStr *PnD, struct ERCPnData *ERCPn)
{
    if(ERCPn->ERC13.Status[0] == 0x01) //;�ϴν��δ���������ж�
    {
        return;
    }
    ERCPn->ERC13.Mark[0] = 0;
    
    if((PnD->OB212[0]!=PnD->RB212[0]) || (PnD->OB212[1]!=PnD->RB212[1]) ||
       (PnD->OB212[2]!=PnD->RB212[2]))
    {
        ERCPn->ERC13.Mark[0] |= 0x01; //; ��̴������
        MovNByte(3, P1D.OB212, P1D.RB212); //;����ܴ���
    }

    if((PnD->OB213[0]!=PnD->RB213[0]) || (PnD->OB213[1]!=PnD->RB213[1]) ||
       (PnD->OB213[2]!=PnD->RB213[2]))
    {
        ERCPn->ERC13.Mark[0] |= 0x01; //; �����������������
        MovNByte(3, PnD->OB213, PnD->RB213); //;�����������
    }

    if((PnD->OB310[0] != PnD->RB310[0]) || (PnD->OB310[1] != PnD->RB310[1]))
    {
        ERCPn->ERC13.Mark[0] |= 0x02; //; ����������
        MovNByte(2, PnD->OB310, PnD->RB310); //;�������
       
    }

    if((PnD->OB710[0] != PnD->RB710[0]) || (PnD->OB710[1] != PnD->RB710[1]) ||
       (PnD->OB710[2] != PnD->RB710[2]))
    {
        ERCPn->ERC13.Mark[0] |= 0x04; //; ʧѹ�������
        PnD->OB710[0] = PnD->RB710[0]; //;ʧѹ����
        PnD->OB710[1] = PnD->RB710[1];
        PnD->OB710[2] = PnD->RB710[2];
    }

    if((PnD->OB222[0] != PnD->RB222[0]) || (PnD->OB222[1] != PnD->RB222[1]) ||
       (PnD->OB222[2] != PnD->RB222[2]))
    {
        ERCPn->ERC13.Mark[0] |= 0x08; //;����������
        PnD->OB222[0] = PnD->RB222[0]; //;�������
        PnD->OB222[1] = PnD->RB222[1];
        PnD->OB222[2] = PnD->RB222[2];
    }

    if((PnD->RC027[0])&0x08)
    {
        ERCPn->ERC13.Mark[0] |= 0x10; //;�����Ƿѹ
    }	

    if(ERCPn->ERC13.Mark[0])
    {
        ERCPn->ERC13.Status[0] = 0x01;  //; ���ܱ�������
    }
}


/* 
********************************************************************************
* �� �� ����ERC14_Judge
* ��    �ܣ��ն�ͣ��/�ϵ��¼�
* ��    ����Type = 1ͣ�� ��=2�ϵ�
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************

void ERC14_Judge(u8 Type)
{
    if(Type == 1)
    {
        ERCP0.ERC14.Status[1] |= 0x01; //;���ܱ�ͣ��
        ERCP0.ERC14.PTOTime[0] = TBCD.Min;
        ERCP0.ERC14.PTOTime[1] = TBCD.Hour;
        ERCP0.ERC14.PTOTime[2] = TBCD.Day;
        ERCP0.ERC14.PTOTime[3] = TBCD.Month;
        ERCP0.ERC14.PTOTime[4] = TBCD.Year; //;ͣ��ʱ��
    }
    else
    {
        ERCP0.ERC14.Status[0] |= 0x01; //;���ܱ�����
        ERCP0.ERC14.PTOTime[0] = 0xEE;
        ERCP0.ERC14.PTOTime[1] = 0xEE;
        ERCP0.ERC14.PTOTime[2] = 0xEE;
        ERCP0.ERC14.PTOTime[3] = 0xEE;
        ERCP0.ERC14.PTOTime[4] = 0xEE;
        ERCP0.ERC14.PLOTime[0] = TBCD.Min;
        ERCP0.ERC14.PLOTime[1] = TBCD.Hour;
        ERCP0.ERC14.PLOTime[2] = TBCD.Day;
        ERCP0.ERC14.PLOTime[3] = TBCD.Month;
        ERCP0.ERC14.PLOTime[4] = TBCD.Year; //;�ϵ�ʱ��
    }
}
*/
void ERC14_Judge(u8 *Byte1, u8 Type)
{
    if(Type == 1)
    {
        ERCP0.ERC14.Status[1] |= 0x01; //;���ܱ�ͣ��
        ERCP0.ERC14.PTOTime[0] = TBCD.Min;
        ERCP0.ERC14.PTOTime[1] = TBCD.Hour;
        ERCP0.ERC14.PTOTime[2] = TBCD.Day;
        ERCP0.ERC14.PTOTime[3] = TBCD.Month;
        ERCP0.ERC14.PTOTime[4] = TBCD.Year; //;ͣ��ʱ�� 
        SaveData_FM24(5, E2R_PowD, E2R_PowD + E2R_ByteNum, ERCP0.ERC14.PTOTime); //;����ͣ��        
    }
    else
    {
        ERCP0.ERC14.Status[0] |= 0x01; //;���ܱ�����      
        ERCP0.ERC14.PTOTime[0] = *Byte1;
        ERCP0.ERC14.PTOTime[1] = *(Byte1+1);
        ERCP0.ERC14.PTOTime[2] = *(Byte1+2);
        ERCP0.ERC14.PTOTime[3] = *(Byte1+3);
        ERCP0.ERC14.PTOTime[4] = *(Byte1+4); //;ͣ��ʱ��         
        ERCP0.ERC14.PLOTime[0] = TBCD.Min;
        ERCP0.ERC14.PLOTime[1] = TBCD.Hour;
        ERCP0.ERC14.PLOTime[2] = TBCD.Day;
        ERCP0.ERC14.PLOTime[3] = TBCD.Month;
        ERCP0.ERC14.PLOTime[4] = TBCD.Year; //;�ϵ�ʱ��
    }
}

/* 
********************************************************************************
* �� �� ����ERC8_Judge
* ��    �ܣ�����ܲ������
* ��    ����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
void ERC8_Judge(struct PointDataStr *PnD, struct ERCPnData *ERCPn)
{
//;    u8  i;
    
    if(ERCPn->ERC8.Status[0] == 0x01) //;�ϴ��¼�δ����
    {
        return;
    } 
    ERCPn->ERC8.Mark[0] = 0;

    if((PnD->OB224[0] != PnD->RB224[0]) || (PnD->OB224[1] != PnD->RB224[1]) ||
       (PnD->OB224[2] != PnD->RB224[2]))
    {
        ERCPn->ERC8.Mark[0] |= 0x01; //;����ʱ�α��̷���
        PnD->OB224[0] = PnD->RB224[0]; //;����
        PnD->OB224[1] = PnD->RB224[1];
        PnD->OB224[2] = PnD->RB224[2];
    }	
    if((PnD->OB21A[0] != PnD->RB21A[0]) || (PnD->OB21A[1] != PnD->RB21A[1]) || 
       (PnD->OB21A[2] != PnD->RB21A[2]) || (PnD->OB21A[3] != PnD->RB21A[3]) || 
       (PnD->OB21A[4] != PnD->RB21A[4]) || (PnD->OB21A[5] != PnD->RB21A[5]))
    {
        ERCPn->ERC8.Mark[0] |= 0x02; //;���ܱ���ʱ�����
        MovNByte(6, PnD->OB21A, PnD->RB21A); //;�����һ�α��ʱ��
    }

    if((PnD->OC117[0] != PnD->RC117[0])||(PnD->OC117[1] != PnD->RC117[1]) ||
       (PnD->OC118[0] != PnD->RC118[0])||(PnD->OC118[1] != PnD->RC118[1]) ||
       (PnD->OC119[0] != PnD->RC119[0])||(PnD->OC119[1] != PnD->RC119[1]))
    {
        ERCPn->ERC8.Mark[0] |= 0x04; //;���ܱ����ո���
        PnD->OC117[0] = PnD->RC117[0]; //;�Զ���������
        PnD->OC117[1] = PnD->RC117[1];
        PnD->OC118[0] = PnD->RC118[0]; //;�Զ���������
        PnD->OC118[1] = PnD->RC118[1];
        PnD->OC119[0] = PnD->RC119[0]; //;�Զ���������
        PnD->OC119[1] = PnD->RC119[1];
    }

    if((PnD->OC030[0] != PnD->RC030[0]) || (PnD->OC030[1] != PnD->RC030[1])
        || (PnD->OC030[2] != PnD->RC030[2]) || (PnD->OC031[0] != PnD->RC031[0])
        || (PnD->OC031[1] != PnD->RC031[1]) || (PnD->OC031[2] != PnD->RC031[2]))
    {
        ERCPn->ERC8.Mark[0] |= 0x08; //;���ܱ����峣������
        PnD->OC030[0] = PnD->RC030[0]; //;�����(�й�)
        PnD->OC030[1] = PnD->RC030[1];
        PnD->OC030[2] = PnD->RC030[2];
        PnD->OC031[0] = PnD->RC031[0]; //;�����(�޹�)
        PnD->OC031[1] = PnD->RC031[1];
        PnD->OC031[2] = PnD->RC031[2];
    }

//;    for(i = 0; i < 24; i++)
//;    {
//;        if(*(PnD->OC331 + i) != *(PnD->RC331 + i))
//;        {
//;            ERCPn->ERC8.Mark[0] |= 0x01; //;���ܱ�ʱ�λ���ʸ���
//;            MovNByte(24,P1D.OC331,P1D.RC331);//;��ǰʱ�α���ʼʱ�估���ʺ�
//;            break;
//;        }
//;    }

    if((PnD->OC040[0] != PnD->RC040[0]) || (PnD->OC040[1] != PnD->RC040[1]) || 
       (PnD->OC040[2] != PnD->RC040[2]) || (PnD->OC041[0] != PnD->RC041[0]) ||
       (PnD->OC041[1] != PnD->RC041[1]) || (PnD->OC041[2] != PnD->RC041[2]))
    {
        ERCPn->ERC8.Mark[0] |= 0x10; //;���ܱ��������ʸ���
        PnD->OC040[0] = PnD->RC040[0]; //;��ѹ����
        PnD->OC040[1] = PnD->RC040[1];
        PnD->OC040[2] = PnD->RC040[2];
        PnD->OC041[0] = PnD->RC041[0]; //;�����ı���
        PnD->OC041[1] = PnD->RC041[1];
        PnD->OC041[2] = PnD->RC041[2];
    }

     if((PnD->OB213[0]!=PnD->RB213[0]) || (PnD->OB213[1]!=PnD->RB213[1]) ||
       (PnD->OB213[2]!=PnD->RB213[2]))
    {
        ERCPn->ERC8.Mark[0] |= 0x20; //;���ܱ������������
        //;Read_MeterM_Order(); //;����������
        MovNByte(3, PnD->OB213, PnD->RB213);    //;by zrt 20130910        
    }

    if(ERCPn->ERC8.Mark[0])
    {
        ERCPn->ERC8.Status[0] = 0x01; 
    }
}

/* 
********************************************************************************
* �� �� ����ERC10_Judge
* ��    �ܣ�����˲ʱ�����ж��Ƿ������ѹ��·�쳣�¼�,ÿ�����ж�һ��
* ��    ����
* ��    �أ� 
* ˵    ��������: ��ѹС�ڵ�ѹ��������,�ҵ���С�ڶ������5%ʱ
*         ʧѹ����ѹС�ڶ��������ҵ������ڶ������5%ʱʱ���������ʧѹʱ����ֵ
*         E10λ���壺Bit 1:���෢�� Bit 2:����ָ�
*         Bit 3:ʧѹ���� Bit 4 ʧѹ�ָ�
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
void ERC10_Judge(struct PoPStr *PnP, struct PointDataStr *PnD, struct ERCPnData *ERCPn)
{
    u16 Ua, Ub, Uc, UBreak, IBLZZ;   
    u32 Ia, Ib, Ic; 

    
    Ua = BCD2ToU16(PnD->RB611);
    Uc = BCD2ToU16(PnD->RB613);
    UBreak = BCD2ToU16(PnP->F26.UBreak);
    Ia = BCD3ToU32(PnD->RB621) * 10;                  //;I�Ŵ�1000��
    Ic = BCD3ToU32(PnD->RB623) * 10;
    IBLZZ = (u16)BCD1ToU8(&P1P.F25.In);
    IBLZZ = IBLZZ * 50;      //;����������ʧѹ������ֵҲ�Ŵ�1000��5%


    if((Ua < UBreak) && (Ia > IBLZZ) )
    {
        UALostTMin++;
        if(UALostTMin > PnP->F26.ULostT)
        {
            UALostTMin = 0;
            ERCPn->ERC10.Status_N |= 0x10;             //;A��ʧѹ
        }
    }
    else
    {
        UALostTMin = 0;
        ERCPn->ERC10.Status_N &= (~0x10);             //;A��ʧѹ�ָ�
    }
    if((Ua < UBreak) && (Ia < IBLZZ))
    {
        ERCPn->ERC10.Status_N |= 0x01;              //;A�����
    }                                                                           
    else                                                                        
    {                                                                           
        ERCPn->ERC10.Status_N &= (~0x01);          //;A�����ָ�
    }

    if(PnP->F25.Type == 2)
    {
        Ub = BCD2ToU16(PnD->RB612);
        Ib = BCD3ToU32(PnD->RB622);
        
        if((Ub < UBreak) && (Ib < IBLZZ))
        {
            ERCPn->ERC10.Status_N  |= 0x02;       //;B�����
        }                                                                       
        else                                                                    
        {                                                                       
            ERCPn->ERC10.Status_N  &= (~0x02);    //;B�����ָ�
        }
        if((Ub < UBreak) && (Ib > IBLZZ) )
        {
            UBLostTMin++;
            if(UBLostTMin > PnP->F26.ULostT)
            {
                UBLostTMin = 0;
                ERCPn->ERC10.Status_N |= 0x20;             //;B��ʧѹ
            }
        }
        else
        {
            UBLostTMin = 0;
            ERCPn->ERC10.Status_N &= (~0x20);             //;B��ʧѹ�ָ�
        }		
    }

    if((Uc < UBreak) && (Ic > IBLZZ) )
    {
        UCLostTMin++;
	 if(UCLostTMin > PnP->F26.ULostT)
	 {
            UCLostTMin = 0;	 
            ERCPn->ERC10.Status_N |= 0x40;             //;C��ʧѹ
	 }
    }
    else
    {
        UCLostTMin = 0;    
        ERCPn->ERC10.Status_N &= (~0x40);             //;C��ʧѹ�ָ�
    }
    if((Uc < UBreak) && (Ic < IBLZZ))
    {
        ERCPn->ERC10.Status_N |= 0x04;           //;C�����
    }                                                                           
    else                                                                        
    {                                                                           
        ERCPn->ERC10.Status_N &= (~0x04);       //;C�����ָ�
    }

    ERCPn->ERC10.DXOM = 0;
    ERCPn->ERC10.DXRM = 0;
    ERCPn->ERC10.SYOM = 0;
    ERCPn->ERC10.SYRM = 0;	
    
    if(((ERCPn->ERC10.Status_O & 0x01)!= 0x01) && ((ERCPn->ERC10.Status_N & 0x01) == 0x01))      
    {
        ERCPn->ERC10.DXOM |= 0x01;              //;A������¼�����
    }
    else if(((ERCPn->ERC10.Status_O & 0x01) == 0x01) && ((ERCPn->ERC10.Status_N & 0x01) != 0x01))
    {
        ERCPn->ERC10.DXRM |= 0x01;             //;A������¼��ָ�
    }
    if(((ERCPn->ERC10.Status_O & 0x10)!= 0x10) && ((ERCPn->ERC10.Status_N & 0x10) == 0x10))      
    {
        ERCPn->ERC10.SYOM |= 0x01;              //;A��ʧѹ�¼�����
    }
    else if(((ERCPn->ERC10.Status_O & 0x10) == 0x10) && ((ERCPn->ERC10.Status_N & 0x10) != 0x10))
    {
        ERCPn->ERC10.SYRM |= 0x01;             //;A��ʧѹ�¼��ָ�
    }	

    if(PnP->F25.Type == 2)
    {
        if(((ERCPn->ERC10.Status_O & 0x02)!= 0x02) && ((ERCPn->ERC10.Status_N & 0x02) == 0x02))
        {
            ERCPn->ERC10.DXOM |= 0x02;
        }
        else if(((ERCPn->ERC10.Status_O& 0x02) == 0x02) && ((ERCPn->ERC10.Status_N & 0x02) != 0x02))
        {
            ERCPn->ERC10.DXRM |= 0x02;
        }
        if(((ERCPn->ERC10.Status_O & 0x20)!= 0x20) && ((ERCPn->ERC10.Status_N & 0x20) == 0x20))
        {
            ERCPn->ERC10.SYOM |= 0x02;
        }
        else if(((ERCPn->ERC10.Status_O& 0x20) == 0x20) && ((ERCPn->ERC10.Status_N & 0x20) != 0x20))
        {
            ERCPn->ERC10.SYRM |= 0x02;
        }		
    }

    if(((ERCPn->ERC10.Status_O & 0x04)!= 0x04) && ((ERCPn->ERC10.Status_N & 0x04) == 0x04))
    {
        ERCPn->ERC10.DXOM |= 0x04;
    }
    else if(((ERCPn->ERC10.Status_O & 0x04) == 0x04) && ((ERCPn->ERC10.Status_N & 0x04) != 0x04))
    {
        ERCPn->ERC10.DXRM |= 0x04;
    }
    if(((ERCPn->ERC10.Status_O & 0x40)!= 0x40) && ((ERCPn->ERC10.Status_N & 0x40) == 0x40))
    {
        ERCPn->ERC10.SYOM |= 0x04;
    }
    else if(((ERCPn->ERC10.Status_O & 0x40) == 0x40) && ((ERCPn->ERC10.Status_N & 0x40) != 0x40))
    {
        ERCPn->ERC10.SYRM |= 0x04;
    }	

    if(ERCPn->ERC10.DXOM || ERCPn->ERC10.DXRM ||ERCPn->ERC10.SYOM || ERCPn->ERC10.SYRM)
    {        
        if (ERCPn->ERC10.DXOM)
        {
            ERCPn->ERC10.Status[0] = 0x01;
        }
        if (ERCPn->ERC10.DXRM)
        {
            ERCPn->ERC10.Status[1] = 0x01;
        }
        if (ERCPn->ERC10.SYOM)
        {
            ERCPn->ERC10.Status[2] = 0x01;
        }
        if (ERCPn->ERC10.SYRM)
        {
            ERCPn->ERC10.Status[3] = 0x01;
        }		
        ERCPn->ERC10.Data[0]  = PnD->RB611[0];//;Ua
        ERCPn->ERC10.Data[1]  = PnD->RB611[1];             
        ERCPn->ERC10.Data[2]  = PnD->RB612[0];      //;Ub                              
        ERCPn->ERC10.Data[3]  = PnD->RB612[1];            
        ERCPn->ERC10.Data[4]  = PnD->RB613[0];      //;Uc                             
        ERCPn->ERC10.Data[5]  = PnD->RB613[1];             
        ERCPn->ERC10.Data[6]  = PnD->RB621[0];      //;Ia                        
        ERCPn->ERC10.Data[7]  = PnD->RB621[1];  
        ERCPn->ERC10.Data[8]  = PnD->RB621[2];            
        ERCPn->ERC10.Data[9]  = PnD->RB622[0];      //;Ib                              
        ERCPn->ERC10.Data[10] = PnD->RB622[1];   
        ERCPn->ERC10.Data[11] = PnD->RB622[2];         
        ERCPn->ERC10.Data[12] = PnD->RB623[0];      //;Ic                            
        ERCPn->ERC10.Data[13] = PnD->RB623[1];  
        ERCPn->ERC10.Data[14] = PnD->RB623[2];           
        ERCPn->ERC10.Data[15] = PnD->R9010[0];
        ERCPn->ERC10.Data[16] = PnD->R9010[1];
        ERCPn->ERC10.Data[17] = PnD->R9010[2];
        ERCPn->ERC10.Data[18] = PnD->R9010[3];
        ERCPn->ERC10.Data[19] = PnD->R9010[4];             //;�����й��ܵ���;
    }

    ERCPn->ERC10.Status_O = ERCPn->ERC10.Status_N;
}


/*----------------------------------------------------------------------------*
 |  ���ݷ������¼����ú�Ҫ���͵Ķ�������
 *----------------------------------------------------------------------------*/
u16 ERC_SMS_Up(char *Out_Buf)
{
//;    u8   ch1,Len;
//;    u16  j = 0, *addr;
//;    u32  *Addr32;
//;    struct  PoPStr  *PnP;
//;    struct  ERCPnData  *ERCPn;
//;    
//;    Addr32 = (u32*)(*(PnDTAddr + SMS.Pn));
//;    PnP = (struct PoPStr*)(*(Addr32 + _PnP));
//;    ERCPn = (struct ERCPnData*)(*(Addr32 + _ERCPn));
//;    
//;    Len  = SMS_CodeData(Out_Buf, ZDP.F4.SMS_Code, SMS.PhoneNo);                 //;�����˵��ֻ�����
//;    j += Len;
//;    ch1 = j;
//;
//;    j += 2;
//;    if (SMS.Receiver == 'A')                                                    //;����ͷ
//;    {
//;        addr = (u16*)ZDPKZ.F100;   
//;    }
//;    else if (SMS.Receiver == 'U')
//;    {
//;        addr = (u16*)ZDPKZ.F98;
//;    }
//;    Len = SMS_DataMove(10, Out_Buf + j, addr);
//;    j += Len;
//;
//;    Len = SMS_DataMove(10, Out_Buf + j, (u16*)PnP->F31);                        //;����������
//;    j += Len;
//;
//;    switch (SMS.ERCNo)                                                          //;�¼�������
//;    {
//;        case 8:
//;        {
//;            if (ERCPn->ERC8.Mark[0] & 0x01)
//;            {
//;                Len = SMS_DataMove(6, Out_Buf + j, (u16*)ERC8MS + 3);           //;����ʱ�α仯
//;                j += Len;
//;            }
//;            if (ERCPn->ERC8.Mark[0] & 0x02)
//;            {
//;                Len = SMS_DataMove(6, Out_Buf + j, (u16*)ERC8MS + 9);           //;���ʱ�����
//;                j += Len;
//;            }
//;            if (ERCPn->ERC8.Mark[0] & 0x04)
//;            {
//;                Len = SMS_DataMove(5, Out_Buf + j, (u16*)ERC8MS + 14);          //;�����ո���
//;                j += Len;
//;            }
//;            if (ERCPn->ERC8.Mark[0] & 0x08)
//;            {
//;                Len = SMS_DataMove(6, Out_Buf + j, (u16*)ERC8MS + 20);          //;���峣������
//;                j += Len;
//;            }
//;            if (ERCPn->ERC8.Mark[0] & 0x10)
//;            {
//;                Len = SMS_DataMove(7, Out_Buf + j, (u16*)ERC8MS + 27);          //;���������ʸ���
//;                j += Len;
//;            }
//;            if (ERCPn->ERC8.Mark[0] & 0x20)
//;            {
//;                Len = SMS_DataMove(6, Out_Buf + j, (u16*)ERC8MS + 33);          //;�����������
//;                j += Len;
//;            }
//;            break;
//;        }
//;        case 10:
//;        {
//;            if (ERCPn->ERC10.DXOM)
//;            {
//;                if (ERCPn->ERC10.DXOM & 0x01)  
//;                {
//;                    Len = SMS_DataMove(1, Out_Buf + j, (u16*)PhaseMS);          //;   A
//;                    j += Len;
//;                }
//;                
//;                if (ERCPn->ERC10.DXOM & 0x02)
//;                {
//;                    Len = SMS_DataMove(1, Out_Buf + j, (u16*)PhaseMS + 1);      //;   B
//;                    j += Len;
//;                }            
//;                
//;                if (ERCPn->ERC10.DXOM & 0x04)
//;                {
//;                    Len = SMS_DataMove(1, Out_Buf + j, (u16*)PhaseMS + 2);      //;   C
//;                    j += Len;
//;                } 
//;                Len = SMS_DataMove(1, Out_Buf + j, (u16*)PhaseMS + 3);          //;   ��
//;                j += Len;
//;                Len = SMS_DataMove(2, Out_Buf + j, (u16*)ERC10MS);              //;   ����
//;                j += Len;      
//;            }
//;            else if (ERCPn->ERC10.SYOM)
//;            {
//;                if (ERCPn->ERC10.SYOM & 0x01)  
//;                {
//;                    Len = SMS_DataMove(1, Out_Buf + j, (u16*)PhaseMS);          //;   A
//;                    j += Len;
//;                }                
//;                if (ERCPn->ERC10.SYOM & 0x02)
//;                {
//;                    Len = SMS_DataMove(1, Out_Buf + j, (u16*)PhaseMS + 1);      //;   B
//;                    j += Len;
//;                }                
//;                if (ERCPn->ERC10.SYOM & 0x04)
//;                {
//;                    Len = SMS_DataMove(1, Out_Buf + j, (u16*)PhaseMS + 2);      //;   C
//;                    j += Len;
//;                } 
//;                Len = SMS_DataMove(1, Out_Buf + j, (u16*)PhaseMS + 3);          //;   ��
//;                j += Len;
//;                Len = SMS_DataMove(2, Out_Buf + j, (u16*)ERC10MS + 2);          //;   ʧѹ
//;                j += Len;
//;            }
//;            break;
//;        }
//;        case 12:
//;        {
//;            Len = SMS_DataMove(7, Out_Buf + j, (u16*)ERC12MS);                  //;���ܱ�ʱ�䳬��
//;            j += Len;
//;            break;
//;        }
//;        case 17:
//;        {
//;            if (ERCPn->ERC17.BPH_OM & 0x01)
//;            {
//;                Len = SMS_DataMove(7, Out_Buf + j, (u16*)ERC17MS);              //;��ѹ��ƽ��Խ��
//;                j += Len;
//;            }
//;            if (ERCPn->ERC17.BPH_OM & 0x02)
//;            {
//;                Len = SMS_DataMove(7, Out_Buf + j, (u16*)ERC17MS + 7);          //;������ƽ��Խ��
//;                j += Len;
//;            }
//;            break;
//;        }
//;        case 24:
//;        {
//;            if (ERCPn->ERC24.YSSXOM)
//;            {
//;                if (ERCPn->ERC24.YSSXOM & 0x01)  
//;                {
//;                    Len = SMS_DataMove(1, Out_Buf + j, (u16*)PhaseMS);          //;   A
//;                    j += Len;
//;                }                
//;                if (ERCPn->ERC24.YSSXOM & 0x02)
//;                {
//;                    Len = SMS_DataMove(1, Out_Buf + j, (u16*)PhaseMS + 1);      //;   B
//;                    j += Len;
//;                }                
//;                if (ERCPn->ERC24.YSSXOM & 0x04)
//;                {
//;                    Len = SMS_DataMove(1, Out_Buf + j, (u16*)PhaseMS + 2);      //;   C
//;                    j += Len;
//;                } 
//;                Len = SMS_DataMove(1, Out_Buf + j, (u16*)PhaseMS + 3);          //;   ��
//;                j += Len;
//;                Len = SMS_DataMove(6, Out_Buf + j, (u16*)ERC24MS);              //;   ��ѹԽ������
//;                j += Len;
//;            }
//;            else if (ERCPn->ERC24.YXXXOM)
//;            {
//;                if (ERCPn->ERC24.YXXXOM & 0x01)  
//;                {
//;                    Len = SMS_DataMove(1, Out_Buf + j, (u16*)PhaseMS);          //;   A
//;                    j += Len;
//;                }                
//;                if (ERCPn->ERC24.YXXXOM & 0x02)
//;                {
//;                    Len = SMS_DataMove(1, Out_Buf + j, (u16*)PhaseMS + 1);      //;   B
//;                    j += Len;
//;                }                
//;                if (ERCPn->ERC24.YXXXOM & 0x04)
//;                {
//;                    Len = SMS_DataMove(1, Out_Buf + j, (u16*)PhaseMS + 2);      //;   C
//;                    j += Len;
//;                } 
//;                Len = SMS_DataMove(1, Out_Buf + j, (u16*)PhaseMS + 3);          //;   ��
//;                j += Len;
//;                Len = SMS_DataMove(6, Out_Buf + j, (u16*)ERC24MS + 6);          //;   ��ѹԽ������
//;                j += Len;
//;            }
//;            break;
//;        }
//;        case 25:
//;        {
//;            if (ERCPn->ERC25.YSSXOM)
//;            {
//;                if (ERCPn->ERC25.YSSXOM & 0x01)  
//;                {
//;                    Len = SMS_DataMove(1, Out_Buf + j, (u16*)PhaseMS);          //;   A
//;                    j += Len;
//;                }                
//;                if (ERCPn->ERC25.YSSXOM & 0x02)
//;                {
//;                    Len = SMS_DataMove(1, Out_Buf + j, (u16*)PhaseMS + 1);      //;   B
//;                    j += Len;
//;                }                
//;                if (ERCPn->ERC25.YSSXOM & 0x04)
//;                {
//;                    Len = SMS_DataMove(1, Out_Buf + j, (u16*)PhaseMS + 2);      //;   C
//;                    j += Len;
//;                } 
//;                Len = SMS_DataMove(1, Out_Buf + j, (u16*)PhaseMS + 3);          //;   ��
//;                j += Len;
//;                Len = SMS_DataMove(6, Out_Buf + j, (u16*)ERC25MS + 5);          //;����Խ������
//;                j += Len;
//;            }
//;            else if (ERCPn->ERC25.YSXOM)
//;            {
//;                if (ERCPn->ERC25.YSXOM & 0x01)  
//;                {
//;                    Len = SMS_DataMove(1, Out_Buf + j, (u16*)PhaseMS);          //;   A
//;                    j += Len;
//;                }                
//;                if (ERCPn->ERC25.YSXOM & 0x02)
//;                {
//;                    Len = SMS_DataMove(1, Out_Buf + j, (u16*)PhaseMS + 1);      //;   B
//;                    j += Len;
//;                }                
//;                if (ERCPn->ERC25.YSXOM & 0x04)
//;                {
//;                    Len = SMS_DataMove(1, Out_Buf + j, (u16*)PhaseMS + 2);      //;   C
//;                    j += Len;
//;                } 
//;                Len = SMS_DataMove(1, Out_Buf + j, (u16*)PhaseMS + 3);          //;   ��
//;                Len = SMS_DataMove(5, Out_Buf + j, (u16*)ERC25MS);              //;����Խ����
//;                j += Len;
//;            }
//;            break;
//;        }
//;        case 26:
//;        {
//;            if (ERCPn->ERC26.Status[0] & 0x01)
//;            {
//;                 Len = SMS_DataMove(7, Out_Buf + j, (u16*)ERC26MS);             //;���ڹ���Խ����
//;                 j += Len;
//;            }
//;            else if (ERCPn->ERC26.Status[2] & 0x01)
//;            {
//;                Len = SMS_DataMove(8, Out_Buf + j, (u16*)ERC26MS + 7);          //;���ڹ���Խ������
//;                j += Len;
//;            }
//;            break;
//;        }
//;        case 27:
//;        {
//;            Len = SMS_DataMove(7, Out_Buf + j, (u16*)ERC27MS);                  //;���ܱ�ʾ���½�
//;            j += Len;
//;            break;
//;        }
//;        case 29:
//;        {
//;            Len = SMS_DataMove(7, Out_Buf + j, (u16*)ERC29MS);                  //;���ܱ����
//;            j += Len;
//;            break;
//;        }
//;        case 30:                                                                //;���ܱ�ͣ��
//;        {
//;            Len = SMS_DataMove(5, Out_Buf + j, (u16*)ERC30MS);
//;            j += Len;
//;            break;
//;        }
//;        //;case 31:
//;        //;{
//;        //;    if (AlarmF.ERC31[7] & 0x80)
//;        //;    {
//;        //;        Len = SMS_DataMove(2, Out_Buf + j, (u16*)OccRecMS);             //;����
//;        //;    }
//;        //;    else
//;        //;    {
//;        //;        Len = SMS_DataMove(2, Out_Buf + j, (u16*)OccRecMS + 2);         //;�ָ�
//;        //;    }
//;        //;    j += Len;
//;        //;    Len = SMS_DataMove(3, Out_Buf + j, (u16*)ERC31MS);                  //;���ܱ�
//;        //;    j += Len;
//;        //;
//;        //;    if (AlarmF.ERC31[8] & 0x01)
//;        //;    {
//;        //;        Len = SMS_DataMove(2, Out_Buf + j, (u16*)PhaseMS);
//;        //;        j += Len;
//;        //;    }
//;        //;    if (AlarmF.ERC31[8] & 0x02)
//;        //;    {
//;        //;        Len = SMS_DataMove(2, Out_Buf + j, (u16*)PhaseMS + 2);
//;        //;        j += Len;
//;        //;    }
//;        //;    if (AlarmF.ERC31[8] & 0x04)
//;        //;    {
//;        //;        Len = SMS_DataMove(2, Out_Buf + j, (u16*)PhaseMS + 4);
//;        //;        j += Len;
//;        //;    }
//;        //;    Len = SMS_DataMove(2, Out_Buf + j, (u16*)ERC31MS + 3);              //;ʧѹ
//;        //;    j += Len;
//;        //;    break;
//;        //;}
//;        //;case 32:                                                                //;���ܱ�ʧ��
//;        //;{
//;        //;    if (AlarmF.ERC32[7] & 0x80)
//;        //;    {
//;        //;        Len = SMS_DataMove(2, Out_Buf + j, (u16*)OccRecMS);             //;����
//;        //;    }
//;        //;    else
//;        //;    {
//;        //;        Len = SMS_DataMove(2, Out_Buf + j, (u16*)OccRecMS + 2);         //;�ָ�
//;        //;    }
//;        //;    j += Len;
//;        //;
//;        //;    Len = SMS_DataMove(3, Out_Buf + j, (u16*)ERC32MS);
//;        //;    j += Len;
//;        //;
//;        //;    if (AlarmF.ERC32[8] & 0x01)
//;        //;    {
//;        //;        Len = SMS_DataMove(2, Out_Buf + j, (u16*)PhaseMS);
//;        //;        j += Len;
//;        //;    }
//;        //;    if (AlarmF.ERC32[8] & 0x02)
//;        //;    {
//;        //;        Len = SMS_DataMove(2, Out_Buf + j, (u16*)PhaseMS + 2);
//;        //;        j += Len;
//;        //;    }
//;        //;    if (AlarmF.ERC32[8] & 0x04)
//;        //;    {
//;        //;        Len = SMS_DataMove(2, Out_Buf + j, (u16*)PhaseMS + 4);
//;        //;        j += Len;
//;        //;    }
//;        //;    Len = SMS_DataMove(2, Out_Buf + j, (u16*)ERC32MS + 3);
//;        //;    j += Len;
//;        //;    break;
//;        //;}
//;        //;case 33:                                                                //;���ܱ����
//;        //;{
//;        //;    if (AlarmF.ERC33[7] &0x80)
//;        //;    {
//;        //;        Len = SMS_DataMove(2, Out_Buf + j, (u16*)OccRecMS);             //;����
//;        //;        j += Len;
//;        //;    }
//;        //;    else
//;        //;    {
//;        //;        Len = SMS_DataMove(2, Out_Buf + j, (u16*)OccRecMS + 2);         //;�ָ�
//;        //;        j += Len;
//;        //;    }
//;        //;    Len = SMS_DataMove(3, Out_Buf + j, (u16*)ERC33MS);
//;        //;    j += Len;
//;        //;
//;        //;    if (AlarmF.ERC33[8] & 0x01)
//;        //;    {
//;        //;        Len = SMS_DataMove(2, Out_Buf + j, (u16*)PhaseMS);
//;        //;        j += Len;
//;        //;    }
//;        //;    if (AlarmF.ERC33[8] & 0x02)
//;        //;    {
//;        //;        Len = SMS_DataMove(2, Out_Buf + j, (u16*)PhaseMS + 2);
//;        //;        j += Len;
//;        //;    }
//;        //;    if (AlarmF.ERC33[8] & 0x04)
//;        //;    {
//;        //;        Len = SMS_DataMove(2, Out_Buf + j, (u16*)PhaseMS + 4);
//;        //;        j += Len;
//;        //;    }
//;        //;    Len = SMS_DataMove(2, Out_Buf + j, (u16*)ERC33MS + 3);
//;        //;    j += Len;
//;        //;    break;
//;        //;}
//;        case 34: //;���ܱ�485�������
//;        {
//;            Len = SMS_DataMove(10, Out_Buf + j, (u16*)ERC34MS);
//;            j += Len;
//;            break;
//;        }
//;        case 35://;���ܱ�Ӳ������
//;        {
//;            Len = SMS_DataMove(7, Out_Buf + j, (u16*)ERC35MS);
//;            j += Len;
//;            break;
//;        }
//;        case 36://;���ܱ�ʣ����С�ڸ澯���
//;        {
//;            Len = SMS_DataMove(13, Out_Buf + j, (u16*)ERC36MS);
//;            j += Len;
//;            break;
//;        }
//;        case 37://;���ܱ�ʣ����С��0
//;        {
//;            Len = SMS_DataMove(10, Out_Buf + j, (u16*)ERC37MS);
//;            j += Len;
//;            break;
//;        }
//;        case 38://;���ܱ�ʣ����С��͸֧���
//;        {
//;            Len = SMS_DataMove(13, Out_Buf + j, (u16*)ERC38MS);
//;            j += Len;
//;            break;
//;        }
//;        case 39://;���ܱ���բ
//;        {
//;            Len = SMS_DataMove(5, Out_Buf + j, (u16*)ERC39MS);
//;            j += Len;
//;            break;
//;        }
//;        case 40: //;���ܱ���ɹ�
//;        {
//;            Len = SMS_DataMove(7, Out_Buf + j, (u16*)ERC40MS);
//;            j += Len;
//;            break;
//;        }
//;        case 41://;���ܱ������բ
//;        {
//;            Len = SMS_DataMove(7, Out_Buf + j, (u16*)ERC41MS);
//;            j += Len;
//;            break;
//;        }
//;        default:
//;        {
//;            break;
//;        }
//;    }
//;
//;    if (SMS.Receiver == 'A')                                                    //;д�����β
//;    {
//;        addr = (u16*)ZDPKZ.F101;  
//;    }
//;    else if (SMS.Receiver == 'U')
//;    {
//;        addr = (u16*)ZDPKZ.F99; 
//;    }
//;    Len = SMS_DataMove(10, Out_Buf + j, addr);
//;    j += Len;
//;
//;    *(Out_Buf + ch1 + 0) = SMS_HexToASC((( (j - 2 - ch1) / 2) & 0xF0) >> 4);
//;    *(Out_Buf + ch1 + 1) = SMS_HexToASC((((j - 2 - ch1) / 2) & 0x0F));
//;
//;    SMS.Len = ((j - 2 - ch1) / 2) + 13;
//;
//;    *(Out_Buf + j++) = 0x1A;              //;  ctrl+Z
//;    return j;                             //;һ��д���˶��ٸ�Ascii��
return 0;
}

 /* 
********************************************************************************
* �� �� ����ERC_DTF
* ��    �ܣ�
* ��    ����
* ��    �أ� 
* ˵    ����DT=1����¼; DT=2������; DT=3: ���Ͷ��Ÿ�����Ա; DT=4: ���Ͷ��Ÿ��û�
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
u8 ERC_DTF(u8 ERCNo, u8 Status, u8 DT)
{
   u8  *DTAdr;
   u32  Adr[5] = {0,(u32)(ZDP.F9.Enable),(u32)(ZDP.F9.Rating),
                    (u32)(ZDPKZ.F97.AdmiSend),(u32)ZDPKZ.F97.UserSend};
    
   DTAdr = (u8 *)(Adr[DT]);

   if(Status & 0x01) 
   {
       if(DTAdr[(ERCNo - 1) / 8] & (0x01 << ((ERCNo - 1) % 8)))  //;��Ҫ����
       {
           if(!(Status & (0x01 << DT)))  
           {
               return true;  //;��δ����
           }
       }
   }
   
   return false;
}



/* 
********************************************************************************
* �� �� ����ERC_DTS
* ��    �ܣ��¼�������ɺ��־��״̬������
* ��    ����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
u8 ERC_DTS(u8 ERCNo, u8 *Status, u8 DT)
{
   u8  *DTAdr;
   u32  Adr[5] = {0,(u32)(ZDP.F9.Enable),(u32)(ZDP.F9.Rating),(u32)(ZDPKZ.F97.AdmiSend),
                    (u32)ZDPKZ.F97.UserSend};
    
   DTAdr = (u8 *)(Adr[DT]);
   
   if(*Status & 0x01) 
   {
       if(DTAdr[(ERCNo - 1) / 8] & (0x01 << ((ERCNo - 1) % 8)))       //;��Ҫ����
       {
           if(!(*Status & (0x01 << DT)))  
           {
               *(Status + 0) |= (0x01 << DT);                          //;��ǰ״̬�������
               
#ifdef ERC_SMS 
               
               if((*(Status + 0) & 0x1F) == 0x1F)                     //;���¼���������еĴ���
               {
                   *(Status + 0) = 0;                                  //;���ȫ���¼���־            
               }
             
#else
               
              // if((*(Status + 0) & 0x07) == 0x07)                     //;���¼���������еĴ���
              if((*(Status + 0) & 0x03) == 0x03)                     //;���¼���������еĴ���
               {
                   *(Status + 0) = 0;                                  //;���ȫ���¼���־            
               }
             
#endif 
               
               return true;
           }
       }
   }
   return false;
}



/*----------------------------------------------------------------------------*
 |  �ҳ��������¼�
 *----------------------------------------------------------------------------*/
u8 ERC_Find(u8 Pn, u8 DT, u8 *status)
{
	  *status = 0x00;
    if (Pn == 0)                                                                //;�ҳ��ն��¼�
    {
        if (ERC_DTF(1,  ERCP0.ERC1.Status[0], DT) == true)  return 1;
        if (ERC_DTF(3,  ERCP0.ERC3.Status[0], DT) == true)  return 3; 
        if (ERC_DTF(14, ERCP0.ERC14.Status[0],DT) == true)  
        {
        	return 14;
        }  
        if (ERC_DTF(14, ERCP0.ERC14.Status[1],DT) == true)
        {
        	  *status = 0x01;
        	  return 14; 
        }
        if (ERC_DTF(20, ERCP0.ERC20.Status[0],DT) == true)  return 20; 
        if (ERC_DTF(21, ERCP0.ERC21.Status[0],DT) == true)  return 21; 
    } 
    else if (Pn <= 1)                                                          //;�ҳ��������¼�
    {           
        u32     *Addr32;
        struct  ERCPnData  *ERCPn;
        
        Addr32 = (u32*)(*(PnDTAddr + Pn - 1));
        ERCPn = (struct ERCPnData*)(*(Addr32 + _ERCPn));
        
        if (ERC_DTF(8,  ERCPn->ERC8.Status[0], DT) == true) return 8;
                           
        if (ERC_DTF(10, ERCPn->ERC10.Status[0], DT) == true) 
        {
        	
        	return 10;
        }
        if (ERC_DTF(10, ERCPn->ERC10.Status[1], DT) == true) 
        {
        	*status = 0x01;
        	return 10;
        }
        if (ERC_DTF(10, ERCPn->ERC10.Status[2], DT) == true) 
        {
        	*status = 0x02;
        	return 10;
        }
        if (ERC_DTF(10, ERCPn->ERC10.Status[3], DT) == true)
        {
        	*status = 0x03;
        	return 10;
        }
                           
        if (ERC_DTF(13, ERCPn->ERC13.Status[0], DT) == true) return 13;     
                           
        if (ERC_DTF(17, ERCPn->ERC17.Status[0], DT) == true) return 17; 
        if (ERC_DTF(17, ERCPn->ERC17.Status[1], DT) == true) 
        {
        	*status = 0x01;
        	return 17;
        } 
                           
        if (ERC_DTF(24, ERCPn->ERC24.Status[0], DT) == true) return 24; 
        if (ERC_DTF(24, ERCPn->ERC24.Status[1], DT) == true) 
        {
        	*status = 0x01;
        	return 24;
        } 
        if (ERC_DTF(24, ERCPn->ERC24.Status[2], DT) == true) 
        {
        	*status = 0x02;
        	return 24;
        }  
        if (ERC_DTF(24, ERCPn->ERC24.Status[3], DT) == true) 
        {
        	*status = 0x03;
        	return 24;
        }  
                           
        if (ERC_DTF(25, ERCPn->ERC25.Status[0], DT) == true) return 25; 
        if (ERC_DTF(25, ERCPn->ERC25.Status[1], DT) == true) 
        {
        	*status = 0x01;
        	return 25;
        }  
        if (ERC_DTF(25, ERCPn->ERC25.Status[2], DT) == true)
        {
        	*status = 0x02;
        	return 25;
        }   
        if (ERC_DTF(25, ERCPn->ERC25.Status[3], DT) == true) 
        {
        	*status = 0x03;
        	return 25;
        }  
                           
        if (ERC_DTF(26, ERCPn->ERC26.Status[0], DT) == true) return 26; 
        if (ERC_DTF(26, ERCPn->ERC26.Status[1], DT) == true) 
        {
        	*status = 0x01;
        	return 26;
        }   
        if (ERC_DTF(26, ERCPn->ERC26.Status[2], DT) == true) 
        {
        	*status = 0x02;
        	return 26;
        }   
        if (ERC_DTF(26, ERCPn->ERC26.Status[3], DT) == true) 
        {
        	*status = 0x03;
        	return 26;
        }   
                           
        if (ERC_DTF(27, ERCPn->ERC27.Status[0], DT) == true) return 27;
        if (ERC_DTF(29, ERCPn->ERC29.Status[0], DT) == true) return 29;
    }
    
    return 0xFF;
}


void ERC_Finish(u8 Pn, u8 ERCNo, u8 DT)
{
    u32     *Addr32;
    struct  ERCPnData  *ERCPn;
    
    if (Pn > 0)                                                          //;�ҳ��������¼�
    {           
        Addr32 = (u32*)(*(PnDTAddr + Pn - 1));
        ERCPn = (struct ERCPnData*)(*(Addr32 + _ERCPn));
    }

    switch (ERCNo)
    {
        case 1:  ERC_DTS(1, ERCP0.ERC1.Status, DT);   break;
        case 3:  ERC_DTS(3, ERCP0.ERC3.Status, DT);   break;
        case 8:  ERC_DTS(8, ERCPn->ERC8.Status, DT);  break;
        case 10: 
        {
            if (ERC_DTS(10, ERCPn->ERC10.Status + 0, DT) == true) return; 
            if (ERC_DTS(10, ERCPn->ERC10.Status + 1, DT) == true) return; 
            if (ERC_DTS(10, ERCPn->ERC10.Status + 2, DT) == true) return;
            if (ERC_DTS(10, ERCPn->ERC10.Status + 3, DT) == true) return; 
            break;
        }
        case 13: ERC_DTS(13, ERCPn->ERC13.Status, DT); break;
        case 17: 
        {
            if (ERC_DTS(17, ERCPn->ERC17.Status + 0, DT) == true) return; 
            if (ERC_DTS(17, ERCPn->ERC17.Status + 1, DT) == true) return; 
            break;
        }
        case 14: 
        {
            if (ERC_DTS(14,ERCP0.ERC14.Status + 0, DT) == true) return;
            if (ERC_DTS(14,ERCP0.ERC14.Status + 1, DT) == true) return;
            break;
        }
        case 20: ERC_DTS(20,ERCP0.ERC20.Status, DT); break;
        case 21: ERC_DTS(21,ERCP0.ERC21.Status, DT); break;
        case 24:
        {
            if (ERC_DTS(24, ERCPn->ERC24.Status + 0, DT) == true) return; 
            if (ERC_DTS(24, ERCPn->ERC24.Status + 1, DT) == true) return; 
            if (ERC_DTS(24, ERCPn->ERC24.Status + 2, DT) == true) return;
            if (ERC_DTS(24, ERCPn->ERC24.Status + 3, DT) == true) return; 
            break;
        }
        case 25:
        {
            if (ERC_DTS(25, ERCPn->ERC25.Status + 0, DT) == true) return; 
            if (ERC_DTS(25, ERCPn->ERC25.Status + 1, DT) == true) return; 
            if (ERC_DTS(25, ERCPn->ERC25.Status + 2, DT) == true) return;
            if (ERC_DTS(25, ERCPn->ERC25.Status + 3, DT) == true) return; 
            break;
        }
        case 26:
        {
            if (ERC_DTS(26, ERCPn->ERC26.Status + 0, DT) == true) return; 
            if (ERC_DTS(26, ERCPn->ERC26.Status + 1, DT) == true) return; 
            if (ERC_DTS(26, ERCPn->ERC26.Status + 2, DT) == true) return;
            if (ERC_DTS(26, ERCPn->ERC26.Status + 3, DT) == true) return; 
            break;
        }
        case 27: ERC_DTS(27,ERCPn->ERC27.Status, DT); break;
        case 29: ERC_DTS(29,ERCPn->ERC29.Status, DT); break;
        default:  break;
    }
}

/* 
********************************************************************************
* �� �� ����ERC_DataPre
* ��    �ܣ���֯�¼����ݽṹ
* ��    ����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
u8 ERC_DataPre(u16 Pn, u8 ERCNo, char *OutData)
{
    u8 j = 0;
    u32     *Addr32;
    struct  ERCPnData  *ERCPn;
    
    if(Pn > 0)                                                          //;�ҳ��������¼�
    {           
        Addr32 = (u32*)(*(PnDTAddr + Pn - 1));
        ERCPn = (struct ERCPnData*)(*(Addr32 + _ERCPn));
    }
    
    switch(ERCNo)
    {
        case 1:
        {
            *(OutData + j++) = 1;
            *(OutData + j++) = 14;
            EW.MoveByte = 'M'; 
            MovNByte(14, (u8*)OutData + j, ERCP0.ERC1.OTime);
            EW.MoveByte = 0;  
            j += 14;
            break;
        }
        case 3:
        {
            u8 Le;
            *(OutData + j++) = 3;
            *(OutData + j++) = Le = ERCP0.ERC3.Num * 4 + 6;
            EW.MoveByte = 'M'; 
            MovNByte(Le, (u8*)OutData + j, ERCP0.ERC3.OTime);
            EW.MoveByte = 0;  
            j += Le;
            break;
        }
        case 8:
        {
            *(OutData + j++) = 8;
            *(OutData + j++) = 8;
            *(OutData + j++) = ERCPn->OTime[0];
            *(OutData + j++) = ERCPn->OTime[1];
            *(OutData + j++) = ERCPn->OTime[2];
            *(OutData + j++) = ERCPn->OTime[3];
            *(OutData + j++) = ERCPn->OTime[4]; 
            *(OutData + j++) = (u8)ERCPn->Pn;
            *(OutData + j++) = (u8)(ERCPn->Pn >> 8);
            *(OutData + j++) = ERCPn->ERC8.Mark[0];
            j += 7;
            break;
        }
        case 10:
        {
            *(OutData + j++) = 10;
            *(OutData + j++) = 28;
            *(OutData + j++) = ERCPn->OTime[0];
            *(OutData + j++) = ERCPn->OTime[1];
            *(OutData + j++) = ERCPn->OTime[2];
            *(OutData + j++) = ERCPn->OTime[3];
            *(OutData + j++) = ERCPn->OTime[4]; 
            *(OutData + j++) = (u8)ERCPn->Pn;
            *(OutData + j) = (u8)(ERCPn->Pn >> 8);
            if (ERCPn->ERC10.Status[0] & 0x01)
            {
                *(OutData + j++) |= 0x80;
                *(OutData + j) = ERCPn->ERC10.DXOM;
                *(OutData + j++) |= 0x40;
            }
            else if (ERCPn->ERC10.Status[1] & 0x01) 
            {
                *(OutData + j++) &= (~0x80);
                *(OutData + j) = ERCPn->ERC10.DXRM;
                *(OutData + j++) |= 0x40;
            }
            else if (ERCPn->ERC10.Status[2] & 0x01) 
            {
                *(OutData + j++) |= 0x80;
                *(OutData + j) = ERCPn->ERC10.SYOM;
                *(OutData + j++) |= 0x80;
            }
            else if (ERCPn->ERC10.Status[3] & 0x01) 
            {
                *(OutData + j++) &= (~0x80);
                *(OutData + j) = ERCPn->ERC10.SYRM;
                *(OutData + j++) |= 0x80;
            }
            EW.MoveByte = 'M'; 
            MovNByte(20, (u8*)OutData + j, ERCPn->ERC10.Data);
            EW.MoveByte = 0;  
            j += 20;
            break;
        }
        case 13:
        {
            *(OutData + j++) = 13;
            *(OutData + j++) = 8;
            *(OutData + j++) = ERCPn->OTime[0];
            *(OutData + j++) = ERCPn->OTime[1];
            *(OutData + j++) = ERCPn->OTime[2];
            *(OutData + j++) = ERCPn->OTime[3];
            *(OutData + j++) = ERCPn->OTime[4]; 
            *(OutData + j++) = (u8)ERCPn->Pn;
            *(OutData + j++) = (u8)(ERCPn->Pn >> 8);
            *(OutData + j++) = ERCPn->ERC13.Mark[0];
            break;
        }
        case 14:
        {
            *(OutData + j++) = 14;
            *(OutData + j++) = 10;
            EW.MoveByte = 'M'; 
            MovNByte(10, (u8*)OutData + j, ERCP0.ERC14.PTOTime);
            EW.MoveByte = 0; 
            j += 10;
            break;
        }
        case 17:
        {
            *(OutData + j++) = 17;
            *(OutData + j++) = 27;
            *(OutData + j++) = ERCPn->OTime[0];
            *(OutData + j++) = ERCPn->OTime[1];
            *(OutData + j++) = ERCPn->OTime[2];
            *(OutData + j++) = ERCPn->OTime[3];
            *(OutData + j++) = ERCPn->OTime[4]; 
            *(OutData + j++) = (u8)ERCPn->Pn;
            *(OutData + j) = (u8)(ERCPn->Pn >> 8);
            if(ERCPn->ERC17.Status[0] & 0x01)
            {
                *(OutData + j++) |= 0x80;
                *(OutData + j++) = ERCPn->ERC17.BPH_OM;
            }
            else if (ERCPn->ERC17.Status[1] & 0x01) 
            {
                *(OutData + j++) &= (~0x80);
                *(OutData + j++) = ERCPn->ERC17.BPH_RM;
            }
            EW.MoveByte = 'M'; 
            MovNByte(19, (u8*)OutData + j, ERCPn->ERC17.Data);
            EW.MoveByte = 0;  
            j += 19;
            break; 
        }
        case 20:
        {
            *(OutData + j++) = 20;
            *(OutData + j++) = 22;
            EW.MoveByte = 'M'; 
            MovNByte(22, (u8*)OutData + j, ERCP0.ERC20.OTime);
            EW.MoveByte = 0; 
            j += 22;
            break;  
        }
        case 21:
        {
            *(OutData + j++) = 21;
            *(OutData + j++) = 6;
            EW.MoveByte = 'M'; 
            MovNByte(6, (u8*)OutData + j, ERCP0.ERC21.OTime);
            EW.MoveByte = 0;
            j += 6;
            break;  
        }
        case 24:
        {
            *(OutData + j++) = 24;
            *(OutData + j++) = 14;
            *(OutData + j++) = ERCPn->OTime[0];
            *(OutData + j++) = ERCPn->OTime[1];
            *(OutData + j++) = ERCPn->OTime[2];
            *(OutData + j++) = ERCPn->OTime[3];
            *(OutData + j++) = ERCPn->OTime[4]; 
            *(OutData + j++) = (u8)ERCPn->Pn;
            *(OutData + j) = (u8)(ERCPn->Pn >> 8);
            if (ERCPn->ERC24.Status[0] & 0x01)
            {
                *(OutData + j++) |= 0x80;
                *(OutData + j) = ERCPn->ERC24.YSSXOM;
                *(OutData + j++) |= 0x40;
            }
            else if (ERCPn->ERC24.Status[1] & 0x01) 
            {
                *(OutData + j++) &= (~0x80);
                *(OutData + j) = ERCPn->ERC24.YSSXRM;
                *(OutData + j++) |= 0x40;
            }
            else if (ERCPn->ERC24.Status[2] & 0x01) 
            {
                *(OutData + j++) |= 0x80;
                *(OutData + j) = ERCPn->ERC24.YXXXOM;
                *(OutData + j++) |= 0x80;
            }
            else if (ERCPn->ERC24.Status[3] & 0x01) 
            {
                *(OutData + j++) &= (~0x80);
                *(OutData + j) = ERCPn->ERC24.YXXXRM;
                *(OutData + j++) |= 0x80;
            }
            EW.MoveByte = 'M'; 
            MovNByte(6, (u8*)OutData + j, ERCPn->ERC24.Data);
            EW.MoveByte = 0;  
            j += 6;
            break;
            
        }
        case 25:
        {
            *(OutData + j++) = 25;
            *(OutData + j++) = 17;
            *(OutData + j++) = ERCPn->OTime[0];
            *(OutData + j++) = ERCPn->OTime[1];
            *(OutData + j++) = ERCPn->OTime[2];
            *(OutData + j++) = ERCPn->OTime[3];
            *(OutData + j++) = ERCPn->OTime[4]; 
            *(OutData + j++) = (u8)ERCPn->Pn;
            *(OutData + j) = (u8)(ERCPn->Pn >> 8);
            if (ERCPn->ERC25.Status[0] & 0x01)
            {
                *(OutData + j++) |= 0x80;
                *(OutData + j) = ERCPn->ERC25.YSSXOM;
                *(OutData + j++) |= 0x40;
            }
            else if (ERCPn->ERC25.Status[1] & 0x01) 
            {
                *(OutData + j++) &= (~0x80);
                *(OutData + j) = ERCPn->ERC25.YSSXRM;
                *(OutData + j++) |= 0x40;
            }
            else if (ERCPn->ERC25.Status[2] & 0x01) 
            {
                *(OutData + j++) |= 0x80;
                *(OutData + j) = ERCPn->ERC25.YSXOM;
                *(OutData + j++) |= 0x80;
            }
            else if (ERCPn->ERC25.Status[3] & 0x01) 
            {
                *(OutData + j++) &= (~0x80);
                *(OutData + j) = ERCPn->ERC25.YSXRM;
                *(OutData + j++) |= 0x80;
            }
            EW.MoveByte = 'M'; 
            MovNByte(9, (u8*)OutData + j, ERCPn->ERC25.Data);
            EW.MoveByte = 0;  
            j += 9;
            break;
        }
        case 26:
        {
            *(OutData + j++) = 26;
            *(OutData + j++) = 14;
            *(OutData + j++) = ERCPn->OTime[0];
            *(OutData + j++) = ERCPn->OTime[1];
            *(OutData + j++) = ERCPn->OTime[2];
            *(OutData + j++) = ERCPn->OTime[3];
            *(OutData + j++) = ERCPn->OTime[4]; 
            *(OutData + j++) = (u8)ERCPn->Pn;
            *(OutData + j) = (u8)(ERCPn->Pn >> 8);
            if (ERCPn->ERC26.Status[0] & 0x01)
            {
                *(OutData + j++) |= 0x80;          //;��ֹ��ʶ
//;                *(OutData + j++) = 0x40;        Խ�ޱ�ʶ0x80Ϊ���ޣ�0x40Ϊ������ by zrt 20130906
                *(OutData + j++) = 0x80;				
            }
            else if (ERCPn->ERC26.Status[1] & 0x01) 
            {
                *(OutData + j++) &= (~0x80);
//;                *(OutData + j++) = 0x40;
                *(OutData + j++) = 0x80;				
            }
            else if (ERCPn->ERC26.Status[2] & 0x01) 
            {
                *(OutData + j++) |= 0x80;
//;                *(OutData + j++) = 0x80;
                *(OutData + j++) = 0x40;				
            }
            else if (ERCPn->ERC26.Status[3] & 0x01) 
            {
                *(OutData + j++) &= (~0x80);
//;                *(OutData + j++) = 0x80;
                *(OutData + j++) = 0x40;				
            }
            EW.MoveByte = 'M'; 
            MovNByte(6, (u8*)OutData + j, ERCPn->ERC26.Data);
            EW.MoveByte = 0;  
            j += 6;
            break;
        }
        case 27:
        {
            *(OutData + j++) = 27;
            *(OutData + j++) = 17;
            *(OutData + j++) = ERCPn->OTime[0];
            *(OutData + j++) = ERCPn->OTime[1];
            *(OutData + j++) = ERCPn->OTime[2];
            *(OutData + j++) = ERCPn->OTime[3];
            *(OutData + j++) = ERCPn->OTime[4]; 
            *(OutData + j++) = (u8)ERCPn->Pn;
            *(OutData + j) = (u8)(ERCPn->Pn >> 8);
            *(OutData + j++) |= 0x80; 			//;by zrt 20130924
            EW.MoveByte = 'M'; 
            MovNByte(10, (u8*)OutData + j, ERCPn->ERC27.Data);
            EW.MoveByte = 0;  
            j += 10;
            break;   
        }
        case 29:
        {
            *(OutData + j++) = 29;
            *(OutData + j++) = 18;
            *(OutData + j++) = ERCPn->OTime[0];
            *(OutData + j++) = ERCPn->OTime[1];
            *(OutData + j++) = ERCPn->OTime[2];
            *(OutData + j++) = ERCPn->OTime[3];
            *(OutData + j++) = ERCPn->OTime[4]; 
            *(OutData + j++) = (u8)ERCPn->Pn;
            *(OutData + j) = (u8)(ERCPn->Pn >> 8);
            *(OutData + j++) |= 0x80; 			//;by zrt 20130924			
            EW.MoveByte = 'M'; 
            MovNByte(11, (u8*)OutData + j, ERCPn->ERC29.Data);
            EW.MoveByte = 0;  
            j += 11;
            break;   
        }
        default:
        {   
            break;
        }
    }
    
    return j;
}


/*----------------------------------------------------------------------------*
 |  �¼���������1
 *----------------------------------------------------------------------------*/
void ERC_Add(u8 ERC, u8 status)
{
    u8 ch1, ch2;

    if (ERC <= 8)
    {
        ch1 = ZDP.F9.Rating[0];
    }
    else if (ERC <= 16)
    {
        ch1 = ZDP.F9.Rating[1];
        ERC = ERC - 8;
    }
    else if (ERC <= 24)
    {
        ch1 = ZDP.F9.Rating[2];
        ERC = ERC - 16;
    }
    else if (ERC <= 32)
    {
        ch1 = ZDP.F9.Rating[3];
        ERC = ERC - 24;
    }
    ch2 = 0x01;
    ch2 <<= ERC - 1;
    if(ch1 &ch2)
    {

        ZDS.F7[0]++; //;��Ҫ�¼���1
       
        if (ZDS.F7[0] != ERC1.Index % ERCReadNum) //;����
        {
            ZDS.F7[0] = ERC1.Index % ERCReadNum;
        }
        SaveData_FM24(1, E2R_EC1, E2R_EC1 + E2R_ByteNum, ZDS.F7); //;�����¼�������
    }
    else
    {
        ZDS.F7[1]++; //;һ���¼���1
        if (ZDS.F7[1] != ERC2.Index % ERCReadNum)
        {
            ZDS.F7[1] = ERC2.Index % ERCReadNum;
        }
        SaveData_FM24(1, E2R_EC2, E2R_EC2 + E2R_ByteNum, ZDS.F7 + 1); //;�����¼�������
    }
}


/* 
********************************************************************************
* �� �� ����ERC_Deal_Save
* ��    �ܣ������¼�
* ��    ����
* ��    �أ�true--�¼�������ϡ�false--�����¼���Ҫ����
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��2011-06-09 10:48:18
********************************************************************************
*/
void ERC_Deal_Save(void)
{
    u8 R, Len, ERCNo,i;
    u8 status;
//;    char ERCData[100];	
    char ERCData[100]={0};   //by zrt 20130907,
    
    for(i = 0; i <= PNMAXNUM; i++ )
    {
        ERCNo = ERC_Find(i, 1, &status); //;�ҳ�Ҫ��¼���¼�
        if(ERCNo != 0xFF)
        {
            Len = ERC_DataPre(i, ERCNo, ERCData); //;���ú÷����¼�������
            R = ZDP.F9.Rating[(ERCNo -1) / 8] & (0x01 << ((ERCNo -1) % 8));  //;�¼���Ҫ�Եȼ���־
            R = R && 1;  //;R = 1ʱ��ʾ�¼���Ҫ��R = 0Ϊһ���¼�
            ERC_Save(Len, ERCData, R);  //;�����¼�
            ERC_Add(ERCNo, status);   //;�¼���������1
            ERC_Finish(i, ERCNo, 1); //;�¼����
        }
    }
}


/*----------------------------------------------------------------------------*
 |  �����¼����ŷ���
 |  return:  true ���¼��������  false ���¼�û�д������
 *----------------------------------------------------------------------------*/
void ERC_Deal_SMS(void)
{        
  #ifdef  ERC_SMS    
    u8 ERCNo;
    u8 status;
    u32 *Addr32;
    struct PoPStr  *PnP;
    
    if (!(GprsC.State & _SetUp))
    {
        return; //;GPRSģ��δ����
    }
    
    if (SMS.Status == idle)
    {
        if (SMS.Receiver == 0)
        {
            SMS.Receiver = 'A';
        }
        
        if (SMS.Receiver == 'A')
        {
            ERCNo = ERC_Find(SMS.Pn + 1, 3, &status);                                        //;���Ҳ������¼�
            if (ERCNo != 0xFF)                                                      //;��ǰ���¼�������Ҫ���͸�����Ա
            {
                SMS.ERCNo = ERCNo;
                SMS.Status = 'F';                                                   //;����״̬Ϊ���ҵ���
                SMS.Index = 0;
            }
        }
        else if (SMS.Receiver == 'U')
        {
            ERCNo = ERC_Find(SMS.Pn + 1, 4, &status);                                    //;�����¼�
            if (ERCNo != 0xFF)                                                  //;��ǰ���¼�������Ҫ���͸��û�
            {
                SMS.ERCNo = ERCNo;
                SMS.Status = 'F';                                               //;״̬Ϊ���ҵ���
                SMS.Index = 0;
            }
        }
    }
    
    if (SMS.Status == idle)                                                     //;��ǰ������û�ҵ��¼�
    {
        SMS.Pn++;                                                               //;ָ����һ��������
        SMS.ERCNo = 0;
        SMS.Index = 0;
        SMS.Receiver = 0;
        if ((SMS.Pn + 1) >= PnMaxNum)
        {
            SMS.Pn = 0;
            PLC.ERC |= 0x04;                                                    //;���ŷ������
        }
    }
    
    if (SMS.Status == 'F')                                                      //;�ҳ������ߵ��ֻ�����
    {
        Addr32 = (u32*)(*(PnDTAddr + SMS.Pn));
        PnP = (struct PoPStr*)(*(Addr32 + _PnP));
        if (SMS.Receiver == 'A')
        {    
            if (SMS_NumCheck(PnP->F32.Admi_MobNo1 + SMS.Index * 8) == true) 
            {
                EW.MoveByte = 'M';
                MovNByte (8, SMS.PhoneNo, PnP->F32.Admi_MobNo1 + SMS.Index * 8);
                EW.MoveByte = 0;
                SMS.Status = 'E';                                               //;�ҵ������ߵĺ��룬���롮���͡�״̬
            }
        }
        else if (SMS.Receiver == 'U')
        {
            if (SMS_NumCheck(PnP->F32.User_MobNo1 + SMS.Index * 8) == true) 
            {
                EW.MoveByte = 'M';
                MovNByte (8, SMS.PhoneNo, PnP->F32.User_MobNo1 + SMS.Index * 8);
                EW.MoveByte = 0;
                SMS.Status = 'E';                                               //;�ҵ������ߵĺ��룬���롮���͡�״̬
            }
        }
    }
    
    if (SMS.Status == 'E')  
    {
        PLC.ERC &= (~0x04); //;δ������
    }
    
    if ((SMS.Status == 'S') || (SMS.Status == 'F'))                             //;������ɻ���û�ҵ��ֻ�����
    {
        SMS.Index++;                                                            //;ָ����һ�������ַ
        if (SMS.Index > 4)
        {
            SMS.Index = 0;
            SMS.Status = 0;
            if (SMS.Receiver == 'A')
            {
                ERC_Finish(SMS.Pn + 1, SMS.ERCNo, 3);
                SMS.Receiver = 'U';                                             //;������Ϊ�û�
            }
            else if (SMS.Receiver == 'U')
            {
                ERC_Finish(SMS.Pn + 1, SMS.ERCNo, 4);
                SMS.Receiver = 0;                    
            }
        }
        else
        {
            SMS.Status = 'F';                                                  //;�����ҳ������˵ĺ���
        }
    }
  
  #endif
}


/*----------------------------------------------------------------------------*
 |  �����¼�������
 *----------------------------------------------------------------------------*/
u8 ERC_Deal_Up(void)
{          
//;  u8 status;
//;  u8 num;
    
    if((ERCUp.Status == 'L') || (ERCUp.Status == 'G'))
    {
#ifdef LocalUpERC  
    ;
#else 
    if(!(GprsC.State & _HadLogin))
    {
        ERCUp.Status = 0;
        return true; //;GPRSģ��δ��½��վ
    }
#endif 
        ERCUp.OVT++;
        if(ERCUp.OVT > 30)  
        { 
            ERCUp.OVT = 0;                                //;��ʱ
            ERCUp.Status = 'S';
        }
    }
    
    if(ERCUp.Status == 'S')  //;�������
    {
        if(ZDP.F1.CON & 0x04)  //;��������ϱ���Ҫ�¼���Ҫȷ��
        {
            u16 OVT, temp;                                                                
            u8  RNum;
            
            temp = *((u16*)ZDP.F1.OVT_RNum);
            RNum = (temp & 0x3000) >> 12; //;����ط�����
            if (RNum != 0)  //;�ط�������Ϊ��
            {
                OVT = temp & 0xFFF; //;������վ��Ӧ��ʱʱ��
                if (OVT == 0)
                {
                    OVT = 1;
                }
                //;OVT *= 4;
                ERCUp.WatiAns++;
                if (ERCUp.WatiAns > OVT)
                {
                    ERCUp.WatiAns = 0x00;
                    ERCUp.SendNum++;
                    if (ERCUp.SendNum < RNum)
                    {
                        ERCUp.Status = 0;
                        ERCUp.WatiAns = 0;
                        #ifdef LocalUpERC 
                        ERCUp.Status = 'L';  //;ͨ������ͨ������                        
                        #else 
                        ERCUp.Status = 'G';  //;ͨ��Զ��ͨ������
                        #endif 
                        ERCUp.ReSedFlag = true;
                    }
                    else
                    {
                        ERCUp.SendNum = 0x00;                      
                        ERCUp.Status = 'R';
                    }           
                }
            }
            else
            {
                ERCUp.Status = 'R';
            }
        }
        else
        {
            ERCUp.Status = 'R';
        }
    }
    
    if(ERCUp.Status == 'R') //;�յ���ȷӦ��
    {
        ERCUp.Status = 0;
        ERCUp.WatiAns = 0;
        ERCUp.UpIndex = ERCUp.SendERCNum;
        SaveData_FM24(1, E2R_ERCUP, E2R_ERCUP + E2R_ByteNum, &ERCUp.UpIndex); //;�����¼�������
    }
    
    if (ERCUp.Status == idle) //;��ǰû�в������¼�����
    {
#ifdef LocalUpERC  
    ;
#else 
    if(!(GprsC.State & _HadLogin))
    {
        ERCUp.Status = 0;
        return true; //;GPRSģ��δ��½��վ
    }
#endif         
        if(ERCUp.UpIndex != ZDS.F7[0])
        {
//;            ERCUp.ERCNo = num; //;�ҳ�Ҫ���͵��¼�
//;            ERCUp.PmStat = status;
          #ifdef LocalUpERC 
            ERCUp.Status = 'L';  //;ͨ������ͨ������
          #else 
            ERCUp.Status = 'G';  //;ͨ��Զ��ͨ������
          #endif 
          ERCUp.ReSedFlag = false;
        }
        else
        {
            return true;
        }
    }   
    
    return false;
}




/* 
********************************************************************************
* �� �� ����ERCPn_ParaJudge
* ��    �ܣ�ERC8��ERC13�ж�
* ��    �����������
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
void ERCPn_ParaJudge(u16 index)
{
    u32     *Addr32;
    struct  PointDataStr *PnD;
    struct  ERCPnData  *ERCPn;
    
    if(index > 0)
    {
        index -= 1;
    }
    Addr32 = (u32*)(*(PnDTAddr + index));
    PnD = (struct PointDataStr*)(*(Addr32 + _PnD));
    ERCPn = (struct ERCPnData*)(*(Addr32 + _ERCPn));
   
    ERCPn->Pn = *((u16*)ZDP.F10.Meter[index].Point);
    ERCPn->OTime[0] = TBCD.Min;   
    ERCPn->OTime[1] = TBCD.Hour;  
    ERCPn->OTime[2] = TBCD.Day;   
    ERCPn->OTime[3] = TBCD.Month; 
    ERCPn->OTime[4] = TBCD.Year;  
                                  
    ERC8_Judge (PnD, ERCPn);
    ERC13_Judge(PnD, ERCPn);
}

/* 
********************************************************************************
* �� �� ����ERCPn_EnergyJudge
* ��    �ܣ�ERC27��ERC29
* ��    ����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
void ERCPn_EnergyJudge(u16 index)
{
    u8  FLY;
    u32 *Addr32;
    u32 NEP,OEP;
    struct  PointDataStr *PnD;
    struct  ERCPnData  *ERCPn;
    
  
    Addr32 = (u32*)(*(PnDTAddr + index));
    PnD = (struct PointDataStr*)(*(Addr32 + _PnD));
    ERCPn = (struct ERCPnData*)(*(Addr32 + _ERCPn));
   
    ERCPn->Pn = *((u16*)ZDP.F10.Meter[index].Point);
    ERCPn->OTime[0] = TBCD.Min;   
    ERCPn->OTime[1] = TBCD.Hour;  
    ERCPn->OTime[2] = TBCD.Day;   
    ERCPn->OTime[3] = TBCD.Month; 
    ERCPn->OTime[4] = TBCD.Year;  

    FLY = BCD1ToU8(&ZDP.F59.MeterFly); //;���ܷ���բֵ
    NEP = BCD4ToU32(PnD->R9010 + 1);  //;��ǰ�й��ܵ���ʾֵ
    OEP = BCD4ToU32(PnD->O9010 + 1);  //;��һ�����й��ܵ���ʾֵ
    
    if(NEP < OEP) //;���ܱ�ʾ���½�
    {
        if(ERCPn->ERC27.Status[0] == 0x01) //;�ϴ��¼�δ������
        {
            return;
        }
        ERCPn->ERC27.Status[0] = 0x01;        //; ���ܱ�ʾ���½�
        ERCPn->ERC27.Data[0] = PnD->O9010[0]; //;�½���ǰ���й�����ʾֵ
        ERCPn->ERC27.Data[1] = PnD->O9010[1];
        ERCPn->ERC27.Data[2] = PnD->O9010[2];
        ERCPn->ERC27.Data[3] = PnD->O9010[3];
        ERCPn->ERC27.Data[4] = PnD->O9010[4];
        ERCPn->ERC27.Data[5] = PnD->R9010[0]; //;�½��������й�����ʾֵ
        ERCPn->ERC27.Data[6] = PnD->R9010[1];
        ERCPn->ERC27.Data[7] = PnD->R9010[2];
        ERCPn->ERC27.Data[8] = PnD->R9010[3];
        ERCPn->ERC27.Data[9] = PnD->R9010[4];
        
        
    }
    else if(NEP > OEP)  //;���ܱ����
    {
        if(FLY != 0)
        {
            if((NEP - OEP) > FLY)
            {
                if(ERCPn->ERC29.Status[0] == 0x01) //;�ϴ��¼�δ������
                {
                    return;
                }
                ERCPn->ERC29.Status[0] = 0x01;
                ERCPn->ERC29.Data[0] = PnD->O9010[0];//;���ߺ�ǰ���й�����ʾֵ
                ERCPn->ERC29.Data[1] = PnD->O9010[1];
                ERCPn->ERC29.Data[2] = PnD->O9010[2];
                ERCPn->ERC29.Data[3] = PnD->O9010[3];
                ERCPn->ERC29.Data[4] = PnD->O9010[4];
                
                ERCPn->ERC29.Data[5] = PnD->R9010[0];//;���ߺ������й�����ʾֵ
                ERCPn->ERC29.Data[6] = PnD->R9010[1];
                ERCPn->ERC29.Data[7] = PnD->R9010[2];
                ERCPn->ERC29.Data[8] = PnD->R9010[3];
                ERCPn->ERC29.Data[9] = PnD->R9010[4];
                ERCPn->ERC29.Data[10] =  ZDP.F59.MeterFly;
            }
        }
    }
    
    PnD->O9010[0] = PnD->R9010[0]; //;������һ�ε�ǰ�����й��ܵ���
    PnD->O9010[1] = PnD->R9010[1];
    PnD->O9010[2] = PnD->R9010[2];
    PnD->O9010[3] = PnD->R9010[3];
    PnD->O9010[4] = PnD->R9010[4];
}

/*----------------------------------------------------------------------------*
 |  ÿ250ms����һ���¼�
 *----------------------------------------------------------------------------*/
u8 ZD_ERC(void)
{
    u8 ch1;
    ERC_Deal_Save();   
    ch1 = ERC_Deal_Up(); 
    ERC_Deal_SMS();
    return ch1;
}
