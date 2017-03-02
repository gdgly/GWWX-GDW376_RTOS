/*
********************************************************************************
*  �� �� ����GDW376_ZDCB.c 
*
*  �ļ������������ն˳��� f165
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

u8 GprsSign;

/*���ж˲���F10�ı��1�Ĳ�����ǰ�����*/
u8 MeterNumChk(void *save, void *now)
{
	u8 *p,*q,i;
	p = save;
	q = now;
	for(i=0; i<6;i++)
	{
		if (*p != *q)
		{
			return true;
		}
		else
		{
			p++;
			q++;
		}
	}
	return false;
}
/******************************************************************************
//;  ���ܣ��ж������Ƿ�ΪȫΪ��һָ����ֵ;
//;  ���أ�true -- ȫΪָ��ֵ;   || false -- ��;
*******************************************************************************/
u8 f_is_sole_data (u8 b8_buf[],u8 b8_byte)
{
  u8 i;
  
  for(i=0;i<b8_byte;i++)
  {
    if( 0x00 != b8_buf[i] )
      return false;
  }
  return true;
}

/* 
********************************************************************************
* �� �� ����Format6Current
* ��    �ܣ�������ʽת������Ӧ376.1
* ��    ����AddrOut---ת����AddrIn---��ת��
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void Format6Current(u8 *AddrOut, u8 *AddrIn)
{
    *(AddrOut + 0) = 0x00;
    *(AddrOut + 0) |= *(AddrIn + 0) << 4;
    *(AddrOut + 1) = *(AddrIn + 0) >> 4;
    *(AddrOut + 1) |= *(AddrIn + 1) << 4;
    *(AddrOut + 2) = *(AddrIn + 1) >> 4;
    *(AddrOut + 2) &= 0x0F;  
    
    *(AddrOut + 3) = 0x00;
    *(AddrOut + 3) |= *(AddrIn + 2) << 4;
    *(AddrOut + 4) = *(AddrIn + 2) >> 4;
    *(AddrOut + 4) |= *(AddrIn + 3) << 4;
    *(AddrOut + 5) = *(AddrIn + 3) >> 4;
    *(AddrOut + 5) &= 0x0F;  
    
    *(AddrOut + 6) = 0x00;
    *(AddrOut + 6) |= *(AddrIn + 4) << 4;
    *(AddrOut + 7) = *(AddrIn + 4) >> 4;
    *(AddrOut + 7) |= *(AddrIn + 5) << 4;
    *(AddrOut + 8) = *(AddrIn + 5) >> 4;
    *(AddrOut + 8) &= 0x0F;   
}

/* 
********************************************************************************
* �� �� ����Format7Voltage
* ��    �ܣ������ѹ��ʽת������Ӧ376.1
* ��    ����AddrOut---ת����AddrIn---��ת��
* ��    �أ��� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void Format7Voltage(u8 *AddrOut, u8 *AddrIn)
{
    *(AddrOut + 0) = (*(AddrIn + 0) & 0x0F) << 4; //;ȡ����λ
    *(AddrOut + 1) = (*(AddrIn + 0) &0xF0) >> 4;  //;ȡ��ʮλ
    *(AddrOut + 1)|= (*(AddrIn + 1) & 0x0F) << 4; //;ȡ����λ
    
    *(AddrOut + 2) = (*(AddrIn + 2) & 0x0F) << 4;
    *(AddrOut + 3) = (*(AddrIn + 2) &0xF0) >> 4; 
    *(AddrOut + 3)|= (*(AddrIn + 3) & 0x0F) << 4;
    
    *(AddrOut + 4) = (*(AddrIn + 4) & 0x0F) << 4;
    *(AddrOut + 5) = (*(AddrIn + 4) &0xF0) >> 4; 
    *(AddrOut + 5)|= (*(AddrIn + 5) & 0x0F) << 4;
}


/* 
********************************************************************************
* �� �� ����Format9Power
* ��    �ܣ���ʽת�����й����ʲ��䣬�޹����ʺ͵��ܴ������ֽڱ�Ϊ�����ֽڣ�
*            ��λ����
* ��    ����AddrOut---ת����AddrIn---��ת����Type---==1,�й����ʣ�==2�޹�
* ��    �أ��� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void Format9Power(u8 *AddrOut, u8 *AddrIn, u8 Type)
{
    if(Type == 1)        //;�й�����
    {
        *(AddrOut + 0) =  *(AddrIn + 0);
        *(AddrOut + 1) =  *(AddrIn + 1);
        *(AddrOut + 2) = *(AddrIn + 2); 
        
        *(AddrOut + 3) =  *(AddrIn + 3);
        *(AddrOut + 4) =  *(AddrIn + 4);
        *(AddrOut + 5) = *(AddrIn + 5); 
        
        *(AddrOut + 6) =  *(AddrIn + 6);
        *(AddrOut + 7) =  *(AddrIn + 7);
        *(AddrOut + 8) = *(AddrIn + 8); 
        
        *(AddrOut + 9) =  *(AddrIn + 9);
        *(AddrOut + 10) =  *(AddrIn + 10);
        *(AddrOut + 11) = *(AddrIn + 11); 
    }
    else if(Type == 2)   //;�޹�����
    {
        *(AddrOut + 0) =  0x00;
        *(AddrOut + 1) =  *(AddrIn + 0);
        *(AddrOut + 2) = *(AddrIn + 1);
        
        *(AddrOut + 3) =  0x00;
        *(AddrOut + 4) =  *(AddrIn + 3);
        *(AddrOut + 5) = *(AddrIn + 4);
        
        *(AddrOut + 6) =  0x00;
        *(AddrOut + 7) =  *(AddrIn + 6);
        *(AddrOut + 8) = *(AddrIn + 7);
        
        *(AddrOut + 9) =  0x00;
        *(AddrOut + 10) =  *(AddrIn + 9);
        *(AddrOut + 11) = *(AddrIn + 10);
    }
}


/* 
********************************************************************************
* �� �� ����Format13Energy
* ��    �ܣ����ܸ�ʽ xxxxxx.xx -> xxxx.xxxx
* ��    ����AddrOut---ת����AddrIn---��ת����
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void Format13Energy(u8 *AddrOut, u8 *AddrIn)
{         
    *(AddrOut + 0) = 0x00;
    *(AddrOut + 1) = *(AddrIn + 0);
    *(AddrOut + 2) = *(AddrIn + 1);
    *(AddrOut + 3) = *(AddrIn + 2);
}


/* 
********************************************************************************
* �� �� ����Format14Energy
* ��    �ܣ����ݸ�ʽ14�����ܸ�ʽxxxxxx.xxxx
* ��    ����AddrOut---ת����AddrIn---��ת����
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void Format14Energy(u8 *AddrOut, u8 *AddrIn)
{
    *(AddrOut + 0) = 0x00;
    *(AddrOut + 1) = *(AddrIn + 0);
    *(AddrOut + 2) = *(AddrIn + 1);
    *(AddrOut + 3) = *(AddrIn + 2);
    *(AddrOut + 4) = *(AddrIn + 3);   
}


/* 
********************************************************************************
* �� �� ����Format14Energy5
* ��    �ܣ�ͬʱ�ı�5������
* ��    ����AddrOut---ת����AddrIn---��ת����
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void Format14Energy5(u8 *AddrOut, u8 *AddrIn)
{
    Format14Energy(AddrOut, AddrIn);
    Format14Energy(AddrOut + 5 , AddrIn + 4 );
    Format14Energy(AddrOut + 10, AddrIn + 8 );
    Format14Energy(AddrOut + 15, AddrIn + 12);
    Format14Energy(AddrOut + 20, AddrIn + 16);
}



/* 
********************************************************************************
* �� �� ����DL645_CHK
* ��    �ܣ�������DTL645֡�Ƿ���ȷ
* ��    ����Buf_In---�������ݻ��壻Buf_Len---���ճ��ȡ�
* ��    �أ���Ч���ݿ�ʼλ�á�
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
u8 DL645_CHK(char *Buf_In, u8 Buf_Len)
{
    u8 i, sum = 0, len, sign = 0;
  
    while((*(Buf_In + sign) != 0x68) && (sign < Buf_Len))
    {
        sign++;
    }
    
    if((sign + 12) > Buf_Len)
    {
        return 0xFF;
    }
    
    Buf_In = Buf_In + sign;
    
    if (*(Buf_In + 0) != 0x68)
    {
        return 0xFF;
    }

    if (*(Buf_In + 7) != 0x68)
    {
        return 0xFF;
    }

    len = *(Buf_In + 9) + 10;

    if(Buf_Len != (len + 2 + sign))
    {
        return 0xFF;
    }

    for(i = 0; i < len; i++)
    {
        sum += *(Buf_In + i);
    }

    if(sum != *(Buf_In + len))
    {
        return 0xFF;
    }

    if(*(Buf_In + len + 1) != 0x16)
    {
        return 0xFF;
    }

    return sign;
}


/* 
********************************************************************************
* �� �� ����DME_CAL
* ��    �ܣ��������µ�����������(zrt20130815)
* ��    ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*
u8 DME_CAL(u8 *NEP, u8 *OEP, u8 *EP_DM, u8 Type)
{
    u8 EP[4];
    u32 EP_Now, EP_Z, EP_Add;

    if(Type == 1) //;�й�
    {
        EP_Z = BCD4ToU32(OEP + 1);    //;������
        EP_Now = BCD4ToU32(NEP + 1);  //;��ǰ����
    }
    else         //;�޹�
    {
        EP_Z = BCD4ToU32(OEP);
        EP_Now = BCD4ToU32(NEP);
    }

    if((EP_Now >= EP_Z) && (EP_Z != 0))
    {
        EP_Add =  EP_Now - EP_Z;
        U32To4BCD(EP_Add, EP);
        Format13Energy(EP_DM, EP);
        return false;
    }
    else
    {
        *(OEP + 0) = *(NEP + 0);   //;��ǰ������Ϊ������
        *(OEP + 1) = *(NEP + 1);
        *(OEP + 2) = *(NEP + 2);
        *(OEP + 3) = *(NEP + 3);
        if(Type == 1)    //;�й�
        {
            *(OEP + 4) = *(NEP + 4);
        }
        return true;
    }
}
*/
u8 DME_CAL(u8 *NEP, u8 *OEP, u8 *EP_DM, u8 Type)
{
    u8 EP[4];
    u32 EP_Now, EP_Z, EP_Add;

    if(Type == 1) //;�й�
    {
        EP_Z = BCD4ToU32(OEP + 1);    //;������
        EP_Now = BCD4ToU32(NEP + 1);  //;��ǰ����
    }
    else if(Type == 2 )         //;�޹�
    {
        EP_Z = BCD4ToU32(OEP);
        EP_Now = BCD4ToU32(NEP);
    }
    else                           //;���ܱ������¼
    {
        EP_Z = BCD2ToU16(OEP);
        EP_Now = BCD2ToU16(NEP);
    }

//    if((EP_Now >= EP_Z) && (EP_Z != 0))  by zrt 20131009  ����ʱ�������ܿ�Ϊ0
    if(EP_Now >= EP_Z)		
    {
        if(Type != 3)
	{
            EP_Add =  EP_Now - EP_Z;
            U32To4BCD(EP_Add, EP);
            Format13Energy(EP_DM, EP);
	}
       else
       {
            EP_Add = EP_Now - EP_Z;
	     U16To2BCD((u16)EP_Add, EP_DM);
       }
	 return false;
    }
    else
    {
        if(Type != 3)
        {
            *(OEP + 0) = *(NEP + 0);   //;��ǰ������Ϊ������
            *(OEP + 1) = *(NEP + 1);
            *(OEP + 2) = *(NEP + 2);
            *(OEP + 3) = *(NEP + 3);
            if(Type == 1)    //;�й�
            {
                *(OEP + 4) = *(NEP + 4);
            }        
            EP_Add =  0;
            U32To4BCD(EP_Add, EP);
            Format13Energy(EP_DM, EP);			
        }
	 else
	 {
	     *(OEP + 0) = *(NEP + 0); 
            *(OEP + 1) = *(NEP + 1);
            EP_Add = 0;
	     U16To2BCD((u16)EP_Add, EP_DM);			
	 }
        return true;
    }
}

/* 
********************************************************************************
* �� �� ����ZDPN_DME
* ��    �ܣ�������ɺ�ͳ��һ�ε���,���µ�����
* ��    ����Pn---������š�
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void ZDPN_DME(u8 Pn)
{
    u32 *Addr32;
    struct PointDataStr *PnD;

    Addr32 = (u32*)(*(PnDTAddr + Pn));
    PnD = (struct PointDataStr*)(*(Addr32 + _PnD));

    /*��ǰ���ݴ���*/
    if((PnD->CB_Time[0] != 0) || (PnD->CB_Time[1] != 0) || (PnD->CB_Time[2] != 0) ||
        (PnD->CB_Time[3] != 0) || (PnD->CB_Time[4] != 0))
    {
        /*���յ����ǵ�ǰ����������������*/
        DME_CAL(PnD->R9010, PnD->Z9010_D, PnD->R9010_D, 1);  //;�����й��յ���
        DME_CAL(PnD->R9011, PnD->Z9011_D, PnD->R9011_D, 1);
        DME_CAL(PnD->R9012, PnD->Z9012_D, PnD->R9012_D, 1);
        DME_CAL(PnD->R9013, PnD->Z9013_D, PnD->R9013_D, 1);
        DME_CAL(PnD->R9014, PnD->Z9014_D, PnD->R9014_D, 1);

        DME_CAL(PnD->R9020, PnD->Z9020_D, PnD->R9020_D, 1);  //;�����й��յ���
        DME_CAL(PnD->R9021, PnD->Z9021_D, PnD->R9021_D, 1);
        DME_CAL(PnD->R9022, PnD->Z9022_D, PnD->R9022_D, 1);
        DME_CAL(PnD->R9023, PnD->Z9023_D, PnD->R9023_D, 1);
        DME_CAL(PnD->R9024, PnD->Z9024_D, PnD->R9024_D, 1);

        DME_CAL(PnD->R9110, PnD->Z9110_D, PnD->R9110_D, 2);  //;�����޹��յ���
        DME_CAL(PnD->R9111, PnD->Z9111_D, PnD->R9111_D, 2);
        DME_CAL(PnD->R9112, PnD->Z9112_D, PnD->R9112_D, 2);
        DME_CAL(PnD->R9113, PnD->Z9113_D, PnD->R9113_D, 2);
        DME_CAL(PnD->R9114, PnD->Z9114_D, PnD->R9114_D, 2);

        DME_CAL(PnD->R9120, PnD->Z9120_D, PnD->R9120_D, 2);  //;�����޹��յ���
        DME_CAL(PnD->R9121, PnD->Z9121_D, PnD->R9121_D, 2);
        DME_CAL(PnD->R9122, PnD->Z9122_D, PnD->R9122_D, 2);
        DME_CAL(PnD->R9123, PnD->Z9123_D, PnD->R9123_D, 2);
        DME_CAL(PnD->R9124, PnD->Z9124_D, PnD->R9124_D, 2);
        
        /*���µ����ǵ�ǰ���������µ���*/
        DME_CAL(PnD->R9010, PnD->Z9010_M, PnD->R9010_M, 1);  //;�����й��µ���
        DME_CAL(PnD->R9011, PnD->Z9011_M, PnD->R9011_M, 1);
        DME_CAL(PnD->R9012, PnD->Z9012_M, PnD->R9012_M, 1);
        DME_CAL(PnD->R9013, PnD->Z9013_M, PnD->R9013_M, 1);
        DME_CAL(PnD->R9014, PnD->Z9014_M, PnD->R9014_M, 1);

        DME_CAL(PnD->R9020, PnD->Z9020_M, PnD->R9020_M, 1);  //;�����й��µ���
        DME_CAL(PnD->R9021, PnD->Z9021_M, PnD->R9021_M, 1);
        DME_CAL(PnD->R9022, PnD->Z9022_M, PnD->R9022_M, 1);
        DME_CAL(PnD->R9023, PnD->Z9023_M, PnD->R9023_M, 1);
        DME_CAL(PnD->R9024, PnD->Z9024_M, PnD->R9024_M, 1);

        DME_CAL(PnD->R9110, PnD->Z9110_M, PnD->R9110_M, 2);  //;�����޹��µ���
        DME_CAL(PnD->R9111, PnD->Z9111_M, PnD->R9111_M, 2);
        DME_CAL(PnD->R9112, PnD->Z9112_M, PnD->R9112_M, 2);
        DME_CAL(PnD->R9113, PnD->Z9113_M, PnD->R9113_M, 2);
        DME_CAL(PnD->R9114, PnD->Z9114_M, PnD->R9114_M, 2);

        DME_CAL(PnD->R9120, PnD->Z9120_M, PnD->R9120_M, 2);  //;�����޹��µ���
        DME_CAL(PnD->R9121, PnD->Z9121_M, PnD->R9121_M, 2);
        DME_CAL(PnD->R9122, PnD->Z9122_M, PnD->R9122_M, 2);
        DME_CAL(PnD->R9123, PnD->Z9123_M, PnD->R9123_M, 2);
        DME_CAL(PnD->R9124, PnD->Z9124_M, PnD->R9124_M, 2);

        DME_CAL(PnD->DB212, PnD->DB212_M, PnD->DF213.ProgramNum, 3);
        DME_CAL(PnD->RB218, PnD->RB218_M, PnD->DF213.LidOpenNum, 3);
        DME_CAL(PnD->DB217, PnD->DB217_M, PnD->DF214.CLKChangeNum, 3);
        DME_CAL(PnD->RB220, PnD->RB220_M, PnD->DF214.PeriodChangeNum, 3);			
    }
}


/*----------------------------------------------------------------------------*
 |  ��������ڹ���,���ڱ�׼485��
 *----------------------------------------------------------------------------*/
void APow_CAL(u8 index)
{
    u16 Ua, Ub, Uc;
    u32 Ia, Ib, Ic;
    u32 U, I, AP;
    u32 *Addr32;
    struct PointDataStr *PnD;

    Addr32 = (u32*)(*(PnDTAddr + index));
    PnD = (struct PointDataStr*)(*(Addr32 + _PnD));

    Ua = BCD2ToU16(PnD->RB611);
    Ub = BCD2ToU16(PnD->RB612);
    Uc = BCD2ToU16(PnD->RB613);

    Ia = BCD3ToU32(PnD->RB621);
    Ib = BCD3ToU32(PnD->RB622);
    Ic = BCD3ToU32(PnD->RB623);

    U = (u32)Ua;
    I = (u32)Ia;
    AP = U * I;
    U = (u32)Ub;
    I = (u32)Ib;
    AP += U * I;
    U = (u32)Uc;
    I = (u32)Ic;
    AP += U * I;
    AP /= 100;
    U32To3BCD(AP, PnD->RB660);
}

/* 
********************************************************************************
* �� �� ����AsciiToU16
* ��    �ܣ���6λASCIIֵת��Ϊ����ʮ����ʮ������ֵ������'2''2''0''.''0'ת��Ϊ
*            220��
* ��    ����
* ��    �أ� 
* ˵    ��������07��Լ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*
u16 AsciiToU16(u8 *ascii)
{
    u8 end, point, i, j;
    u16 temp16, t16;

    end = 0;
    point = 0;
    temp16 = 0;
    t16 = 1;
    for(i = 0; i < 6; i++)
    {
        if(ascii[i] == 0)
        {
            end = i;
            break;
        }
        else if(ascii[i] == '.')
        {
            point = i;                          
        }
    }
    
    if(point != 0)
    {
        j = point; //;�����10������ʮ����Ӧ376��ʽ
    }
    else
    {
        j = end; //;�����10������ʮ����Ӧ376��ʽ
    }
    
    for(i = 0; i < j; i++) 
    {
        t16 *= 10;
    }
    
    for(i = 0; i < end; i++)
    {
        if(point != 0) //;С���㲻Ϊ0
        {
            if(i == point)
            {
                temp16 += ascii[i+1];   
            }
        }
        temp16 += ascii[i] * t16;
        t16 /= 10;
        
    }
    return temp16; 
}
*/
//by zrt 20130820 
u16 AsciiToU16(u8 *ascii)
{
    u8 end, point, i, j;
    u16 temp16, t16;

    end = 0;
    point = 0;
    temp16 = 0;
    t16 = 1;
    for(i = 0; i < 6; i++)
    {
        if(ascii[5-i] == '.')
        {
            point = i;                          //;�ҵ�С����λ��
        }    
        else if(ascii[5-i] > '9'||ascii[5-i] < '0')
        {
            end = i;
            break;
        }
    }
    
    if(point != 0)
    {
        j = point; //;�����10������ʮ����Ӧ376��ʽ
    }
    else
    {
        j = end; //;�����10������ʮ����Ӧ376��ʽ
    }
    
    for(i = 0; i < j; i++) 
    {
        t16 *= 10;
    }
    
    for(i = 0; i < end; i++)
    {
        if(ascii[5-i]>='0' && ascii[5-i]<='9')
        {
            temp16 += (ascii[5-i] -0x30)* t16;
            t16 /= 10;    
	}
        if(point != 0) //;С���㲻Ϊ0
        {
            if(i == point)
            {
                temp16 += (ascii[5-i-1]-0x30);   
		  break;
            }
        }
        
    }
    return temp16; 
}


/* 
********************************************************************************
* �� �� ����ZDCB_MovDataDI
* ��    �ܣ����������ݱ�ʶд�뵽�˿ڻ���ռ�
* ��    ����
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void ZDCB_MovDataDI(char *Order_address, struct MeterData *PMD, struct PortCtrl *Port)
{
    u8 i, L, Len, *Addr8, j, k;
    u32 *Addr32, StrOT;
    u32 ComTab_Head, DI1DI0;
    
    if(Port->GYType == GYType2007)
    {
        if(*(Order_address + 8) != 0x91)
        {
           return;
        }
        
        L = *(Order_address + 9); //;���ݳ���
        
        for(i = 0; i < L; i++)
        {
            *(Order_address + 10 + i) -= 0x33; //;�������ȥ 0x33
        }
        
        DI1DI0 = *((u32*)(Order_address + 10));
        
        if(DI1DI0 == Port->DI1DI0)
        {
            
            ComTab_Head = (u32)DITab_2007;
            Addr32 = (u32*)(ComTab_Head + Port->IndSend * DITab_Num + 4);       
            Len = (u8)(*Addr32 & 0x0000FFFF);        
            StrOT = (u16)(*Addr32 >> 16);        /*��MeterData�ṹ�е�ƫ��*/
            Addr8 = (u8*)(PMD->Start + StrOT);
            EW.MoveByte = 'M';
            MovNByte(Len, Addr8, (u8*)Order_address + 14);
            EW.MoveByte = 0;              
        }
    }
    else if(Port->GYType == GYType1997)
    {
        if(*(Order_address + 8) != 0x81)
        {
           return;
        }
        
        L = *(Order_address + 9); //;���ݳ���
        
        for(i = 0; i < L; i++)
        {
            *(Order_address + 10 + i) -= 0x33; //;�������ȥ 0x33
        }
        
        DI1DI0 = *(Order_address + 11);
        DI1DI0 <<= 8;
        DI1DI0 += *(Order_address + 10);
        
        if(DI1DI0 == Port->DI1DI0)
        {     
            ComTab_Head = (u32)DITab_1997;
            Addr32 = (u32*)(ComTab_Head + Port->IndSend * DITab_Num + 4);       
            Len = (u8)(*Addr32 & 0x0000FFFF);        
            StrOT = (u16)(*Addr32 >> 16);        /*��MeterData�ṹ�е�ƫ��*/
            Addr8 = (u8*)(PMD->Start + StrOT);
            EW.MoveByte = 'M';
            if(Len == 0xF3) //;��������ʱ��
            {
                j = 12;
                k = 0;
                for(i = 0; i < 5; i++)
                {
                    MovNByte(4, Addr8 + k, (u8*)Order_address + j);
                    j += 4;
                    k += 8; //;��һ������ʱ��λ��4+1+3
                }
            }
            else if(Len == 0xF5) //;����ֵ
            {
                j = 12;
                k = 0;
                for(i = 0; i < 5; i++)
                {
                    MovNByte(3, Addr8 + k, (u8*)Order_address + j);
                    j += 3;
                    k += 8; //;��һ������ʱ��λ��5+3
                }
            }
            else if(Len == 0xF1) //;�������
            {
                j = 12;
                k = 0;
                for(i = 0; i < 5; i++)
                {
                    MovNByte(2, Addr8 + k, (u8*)Order_address + j);
                    j += 2;
                    k += 3; //;��һ������ʱ��λ��5+3
                }
            }
            else
            {
                MovNByte(Len, Addr8, (u8*)Order_address + 12);
            }
            EW.MoveByte = 0;          
        }
    }
}


/* 
********************************************************************************
* �� �� ����ZDCB_MovDataPn
* ��    �ܣ�����ɺ󣬽��������ݴӶ˿ڻ���ռ�д�������ռ�
* ��    ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void ZDCB_MovDataPn(u8 pn, u8 gytype, struct MeterData *PMD)
{
    u8 FirstCB = false, i;
    u16 temp16;
    u32 *Addr32, temp32, t32;
    struct PointDataStr *PnD;
    struct PoPStr *PnP;

    Addr32 = (u32*)(*(PnDTAddr + pn));
    PnD = (struct PointDataStr*)(*(Addr32 + _PnD));
    PnP = (struct PoPStr*)(*(Addr32 + _PnP));

    if ((PnD->CB_Time[0] == 0) && (PnD->CB_Time[1] == 0) &&
        (PnD->CB_Time[2] == 0) && (PnD->CB_Time[3] == 0) &&
        (PnD->CB_Time[4] == 0))
    {
        FirstCB = true; //;�״γ���
    }

    EW.MoveByte = 'M';
    PnD->RC313[0] = 4;
    //;PnD->RC313[0] = PMD->RC313[0];
    PnD->CB_Time[0] = PMD->CB_Time[0];
    PnD->CB_Time[1] = PMD->CB_Time[1];
    PnD->CB_Time[2] = PMD->CB_Time[2];
    PnD->CB_Time[3] = PMD->CB_Time[3];
    PnD->CB_Time[4] = PMD->CB_Time[4];
    /* F25 */
    MovNByte(12, PnD->RB630, PMD->RB630);    //;�й�����        
    MovNByte(8, PnD->RB650, PMD->RB650);     //;��������  
    MovNByte(12, PnD->RB660, PMD->RB660);   //;���ڹ���
    if(gytype == GYType2007)
    {    
        //;F25
        MovNByte(9,PnD->RB621, PMD->RB621);     //;˲ʱ����
        MovNByte(6, PnD->RB611, PMD->RB611);    //;˲ʱ��ѹ
        MovNByte(12, PnD->RB640, PMD->RB640);    //;�޹�����
        
        //;F26
        /*������3���ֽڱ�Ϊ�����ֽ�
          ʱ����6���ֽڱ�Ϊ�ĸ��ֽ�
          ��������ܵĴ�����ʱ�䣬��ʼ�ͽ���*/
        MovNByte(2, PnD->RB311, PMD->RB311);       //;A�������
        temp16 = BCD2ToU16(PnD->RB311);
        MovNByte(2, PnD->RB312, PMD->RB312);       //;B�������
        temp16 += BCD2ToU16(PnD->RB312);
        MovNByte(2, PnD->RB313, PMD->RB313);       //;C�������
        temp16 += BCD2ToU16(PnD->RB313);
        U16To2BCD(temp16, PnD->RB310);              //;�ܶ������   
                  
        MovNByte(3, PnD->RB321, PMD->RB321);       //;A����ʱ��
        temp32 = BCD3ToU32(PnD->RB321);   
        MovNByte(3, PnD->RB322, PMD->RB322);       //;B����ʱ��
        temp32 += BCD3ToU32(PnD->RB322);
        MovNByte(3, PnD->RB323, PMD->RB323);       //;C����ʱ��
        temp32 += BCD3ToU32(PnD->RB323);
        U32To3BCD(temp32, PnD->RB320);                 //;�ܶ���ʱ��
        
        MovNByte(4, PnD->RB331, PMD->RB331 + 1);       //;A�����������ʼʱ��
        temp32 = *((u32*)PnD->RB331);
        MovNByte(4, PnD->RB332, PMD->RB332 + 1);       //;B�����������ʼʱ��
        t32 = *(u32*)PnD->RB332;
        if(t32 > temp32)
        {
            temp32 = t32;
        }
        MovNByte(4, PnD->RB333, PMD->RB333 + 1);       //;C�����������ʼʱ��
        t32 = *(u32*)PnD->RB333;
        if(t32 > temp32)
        {
            temp32 = t32;
        }
        *(u32*)PnD->RB330 = temp32;                 //;���������ʼʱ��
        
        MovNByte(4, PnD->RB341, PMD->RB341 + 1);       //;A������������ʱ��
        temp32 = *((u32*)PnD->RB341);
        MovNByte(4, PnD->RB342, PMD->RB342 + 1);       //;B������������ʱ��
        t32 = *((u32*)PnD->RB342);
        if(t32 > temp32)
        {
            temp32 = t32;
        }
        MovNByte(4, PnD->RB343, PMD->RB343 + 1);       //;C������������ʱ��
        t32 = *(u32*)PnD->RB343;
        if(t32 > temp32)
        {
            temp32 = t32;
        }
        *(u32*)PnD->RB340 = temp32;                 //;����������ʱ��
           
        //;F27
        MovNByte(4, PnD->RB216, PMD->RB216); //;��ع���ʱ��
        MovNByte(3, PnD->RB212, PMD->RB212); //;����ܴ���  
        MovNByte(3, PnD->RB214, PMD->RB214); //;��������ܴ��� 
        MovNByte(3, PnD->RB213, PMD->RB213); //;���������ܴ���   
        MovNByte(3, PnD->RB215, PMD->RB215); //;�¼������ܴ���   
        MovNByte(3, PnD->RB217, PMD->RB217); //;Уʱ�ܴ���
        MovNByte(6, PnD->RB21A, PMD->RB21A); //;���һ�α�̷���ʱ�� 
        MovNByte(6, PnD->RB21C, PMD->RB21C); //;���һ�ε�����㷢��ʱ��  
        MovNByte(6, PnD->RB21B, PMD->RB21B); //;���һ���������㷢��ʱ��  
        MovNByte(6, PnD->RB21D, PMD->RB21D); //;���һ���¼����㷢��ʱ��
        MovNByte(6, PnD->RB21F, PMD->RB21F + 4); //;���һ��Уʱ����ʱ��



        
        //;F28 �������״̬�ּ���λ��־
        MovNByte(14, PnD->RC027, PMD->RC027); //;�������״̬��
        
        //;F31 ��ǰA��B��C������/�����й�����ʾֵ  ��֧�ַ���
        Format14Energy(PnD->R30A1, PMD->R30A1); 
        Format14Energy(PnD->R30B1, PMD->R30B1); 
        Format14Energy(PnD->R30C1, PMD->R30C1); 

        Format14Energy(PnD->R30A2, PMD->R30A2); 
        Format14Energy(PnD->R30B2, PMD->R30B2); 
        Format14Energy(PnD->R30C2, PMD->R30C2);    //by zrt 20130909		

        MovNByte(4, PnD->R30A3, PMD->R30A3);
        MovNByte(4, PnD->R30B3, PMD->R30B3);
        MovNByte(4, PnD->R30C3, PMD->R30C3);	

        MovNByte(4, PnD->R30A4, PMD->R30A4);
        MovNByte(4, PnD->R30B4, PMD->R30B4);
        MovNByte(4, PnD->R30C4, PMD->R30C4);		
    
        //;F32 ��һ������A��B��C������/�����й�����ʾֵ
        Format14Energy(PnD->R31A1, PMD->R31A1); 
        Format14Energy(PnD->R31B1, PMD->R31B1); 
        Format14Energy(PnD->R31C1, PMD->R31C1); 

        Format14Energy(PnD->R31A2, PMD->R31A2); 
        Format14Energy(PnD->R31B2, PMD->R31B2); 
        Format14Energy(PnD->R31C2, PMD->R31C2);    //by zrt 20130909		

        MovNByte(4, PnD->R31A3, PMD->R31A3);
        MovNByte(4, PnD->R31B3, PMD->R31B3);
        MovNByte(4, PnD->R31C3, PMD->R31C3);	

        MovNByte(4, PnD->R31A4, PMD->R31A4);
        MovNByte(4, PnD->R31B4, PMD->R31B4);
        MovNByte(4, PnD->R31C4, PMD->R31C4);				
        
        //;F49 ��ǰ��ѹ��������λ�� ��֧�ֵ�����λ��
        MovNByte(6,  PnD->RB671, PMD->RB671);
        
         //;������ F25
        if(PMD->RC14A[1] == 'S')/*��Դ���߷�ʽ*/
        {
            PnP->F25.Type = 1;  //;��������
        }
        else if(PMD->RC14A[1] == 'T')
        {
            PnP->F25.Type = 2;  //;��������
        }
        
            
        MovNByte(6,  PnD->RC040, PMD->RC040); //;����������
        temp16 = (u16)BCD3ToU32(PMD->RC040); //;��ѹ����������
        PnP->F25.UMul[0] = (u8)temp16;
        PnP->F25.UMul[1] = (u8)(temp16>>8);
        temp16 = (u16)BCD3ToU32(PMD->RC041); //;��������������
        PnP->F25.IMul[0] = (u8)temp16;
        PnP->F25.IMul[1] = (u8)(temp16>>8);
        
         //;���ѹby zrt 20130821
/*         
        if((PMD->RC141[0] != PnD->RC141[0]) || (PMD->RC141[1] != PnD->RC141[1]) ||
           (PMD->RC141[2] != PnD->RC141[2]) || (PMD->RC141[3] != PnD->RC141[3]) ||
           (PMD->RC141[4] != PnD->RC141[4]) || (PMD->RC141[5] != PnD->RC141[5]))
        {
*/        
            for(i = 0; i < 6; i++)
            {
                PnD->RC141[i] = PMD->RC141[i];
            }
            temp16 = AsciiToU16((u8*)PMD->RC141);
            U16To2BCD(temp16, PnP->F25.Un);
/*			
        }
        //;�����
        if((PMD->RC142[0] != PnD->RC142[0]) || (PMD->RC142[1] != PnD->RC142[1]) ||
           (PMD->RC142[2] != PnD->RC142[2]) || (PMD->RC142[3] != PnD->RC142[3]) ||
           (PMD->RC142[4] != PnD->RC142[4]) || (PMD->RC142[5] != PnD->RC142[5]))
        {
*/        
            for(i = 0; i < 6; i++)
            {
                PnD->RC142[i] = PMD->RC142[i];
            }
            temp16 = AsciiToU16((u8*)PMD->RC142);
            U8To1BCD((u8)temp16, &PnP->F25.In);
			
//        }
		
        //;F165  by zrt 20130812
        MovNByte(2, PnD->DB212, PMD->RB212);        //;�ܱ�̴���
        MovNByte(5, PnD->DB21A, PMD->RB21A+1);      //;���һ�α�̷���ʱ��
        MovNByte(2, PnD->RB218, PMD->RB218);        //;���˸��ܴ���                  //;by zrt 20130808
        MovNByte(5, PnD->RB219, PMD->RB219+1);      //;���һ�ο��˸Ƿ���ʱ��	
        //;F166  by zrt 20130814
        MovNByte(2, PnD->DB217, PMD->RB217);        //;Уʱ�ܴ���
        MovNByte(5, PnD->DB21F, PMD->RB21F+5);      //;���һ��Уʱ����ʱ��
        MovNByte(2, PnD->RB220, PMD->RB220);        //;ʱ�β����޸Ĵ���
        MovNByte(5, PnD->RB221, PMD->RB221+1);	    //;ʱ�β����޸ķ���ʱ��
	      MovNByte(3, PnD->RB224, PMD->RB224);        //;by zrt 210130910
    }
    else if(gytype == GYType1997)
    {  
        //;F25
        Format6Current(PnD->RB621, PMD->RB621); //;˲ʱ����
        Format7Voltage(PnD->RB611, PMD->RB611); //;˲ʱ��ѹ
        Format9Power(PnD->RB640, PMD->RB640, 2); //;�޹����� 
        //;F26
        /*������3���ֽڱ�Ϊ�����ֽ�
          ʱ����6���ֽڱ�Ϊ�ĸ��ֽ�*/
        MovNByte(2, PnD->RB310, PMD->RB310);       //;�ܶ������
        MovNByte(2, PnD->RB311, PMD->RB311);       //;A�������
        MovNByte(2, PnD->RB312, PMD->RB312);       //;B�������
        MovNByte(2, PnD->RB313, PMD->RB313);       //;C�������
           
        MovNByte(12, PnD->RB320, PMD->RB320);      //;����ʱ��                
        
        MovNByte(4, PnD->RB330, PMD->RB330);       //;���������ʼʱ��
        MovNByte(4, PnD->RB331, PMD->RB331);       //;A�����������ʼʱ��
        MovNByte(4, PnD->RB332, PMD->RB332);       //;B�����������ʼʱ��
        MovNByte(4, PnD->RB333, PMD->RB333);       //;C�����������ʼʱ��
              
        MovNByte(4, PnD->RB341, PMD->RB341);       //;����������ʱ��
        MovNByte(4, PnD->RB341, PMD->RB341);       //;A������������ʱ��
        MovNByte(4, PnD->RB342, PMD->RB342);       //;B������������ʱ��
        MovNByte(4, PnD->RB343, PMD->RB343);       //;C������������ʱ��
         
        //;F27
        PMD->RB216[3] = 0x00;
        MovNByte(4, PnD->RB216, PMD->RB216); //;��ع���ʱ��
        MovNByte(3, PnD->RB212, PMD->RB212); //;����ܴ���  
        MovNByte(3, PnD->RB214, PMD->RB214); //;��������ܴ��� 
        MovNByte(3, PnD->RB213, PMD->RB213); //;���������ܴ���   
        MovNByte(3, PnD->RB215, PMD->RB215); //;�¼������ܴ���   
        MovNByte(3, PnD->RB217, PMD->RB217); //;Уʱ�ܴ���
        PnD->RB21A[0] = 0x00; 
        MovNByte(5, PnD->RB21A + 1, PMD->RB21A);    //;���һ�α�̷���ʱ�� 
        PnD->RB21B[0] = 0x00;
        MovNByte(5, PnD->RB21B, PMD->RB21B);        //;���һ���������㷢��ʱ��  
        ByteN_Fill(6, PnD->RB21C, 0);               //;���һ�ε�����㷢��ʱ�� 
        ByteN_Fill(6, PnD->RB21D, 0);               //;���һ���¼����㷢��ʱ��
        ByteN_Fill(6, PnD->RB21F, 0);               //;���һ��Уʱ����ʱ��   
        
        //;F28 �������״̬�ּ���λ��־
        MovNByte(1, PnD->RC027, PMD->RC027); //;�������״̬��
        ByteN_Fill(13, PnD->RC027, 0);       //;�������״̬��
        
         //;������ F25
        if(PMD->RC14A[1] == 0x33)/*��Դ���߷�ʽ*/
        {
            PnP->F25.Type = 1;  //;��������
        }
        else if(PMD->RC14A[1] == 0x34)
        {
            PnP->F25.Type = 2;  //;��������
        }
        
//;        MovNByte(6,  PnD->RC040, PMD->RC040); //;����������
//;        temp16 = (u16)BCD3ToU32(PMD->RC040); //;��ѹ����������
//;        PnP->F25.UMul[0] = (u8)temp16;
//;        PnP->F25.UMul[1] = (u8)(temp16>>8);
//;        temp16 = (u16)BCD3ToU32(PMD->RC041); //;��������������
//;        PnP->F25.IMul[0] = (u8)temp16;
//;        PnP->F25.IMul[1] = (u8)(temp16>>8);
         MovNByte(2, PnD->RC141, PMD->RC141 + 1); //;���ѹ
         MovNByte(1, PnD->RC142, PMD->RC142 + 1); //;���ѹ
         
//;         /* F50 */
//;         MovNByte(2, PnD->RB740, PMD->RB740); //;�����ϼ�Ԥ����״̬��
    }
       
    /* F27 */
    CheckTime();
    PnD->RDayTime[0] = PMD->RC011[0];
    PnD->RDayTime[1] = PMD->RC011[1];
    PnD->RDayTime[2] = PMD->RC011[2];
    PnD->RDayTime[3] = PMD->RC010[1];
    PnD->RDayTime[4] = PMD->RC010[2];
    PnD->RDayTime[4] |= (PMD->RC010[0] << 5);   //;����
    PnD->RDayTime[5] = PMD->RC010[3];
          
    /* F33 */
    Format14Energy5(PnD->R9010, PMD->R9010);    //;��ǰ�����й������ʵ���
    MovNByte(20, PnD->R9110, PMD->R9110);       //;��ǰ�����޹������ʵ���
    MovNByte(40, PnD->R9130, PMD->R9130);       //;��ǰһ�������޹������ʵ���

    /* F34 */
    Format14Energy5(PnD->R9020, PMD->R9020);    //;��ǰ�����й������ʵ���
    MovNByte(20, PnD->R9120, PMD->R9120);       //;��ǰ�����޹������ʵ���
    MovNByte(40, PnD->R9150, PMD->R9150);       //;��ǰ���������޹������ʵ���

    /* F35 ����ʱ����λ����λ*/
    MovNByte(3,  PnD->RA010, PMD->RA010);       //;��ǰ�����й��������
    MovNByte(3,  PnD->RA011, PMD->RA011);
    MovNByte(3,  PnD->RA012, PMD->RA012);
    MovNByte(3,  PnD->RA013, PMD->RA013);
    MovNByte(3,  PnD->RA014, PMD->RA014);
    MovNByte(4,  PnD->RB010, PMD->RB010);       //;��ǰ�����й������������ʱ��
    MovNByte(4,  PnD->RB011, PMD->RB011); 
    MovNByte(4,  PnD->RB012, PMD->RB012); 
    MovNByte(4,  PnD->RB013, PMD->RB013); 
    MovNByte(4,  PnD->RB014, PMD->RB014); 
    
    MovNByte(3,  PnD->RA110, PMD->RA110);       //;��ǰ�����޹��������
    MovNByte(3,  PnD->RA111, PMD->RA111);
    MovNByte(3,  PnD->RA112, PMD->RA112);
    MovNByte(3,  PnD->RA113, PMD->RA113);
    MovNByte(3,  PnD->RA114, PMD->RA114);
    MovNByte(4,  PnD->RB110, PMD->RB110);       //;��ǰ�����޹������������ʱ��
    MovNByte(4,  PnD->RB111, PMD->RB111); 
    MovNByte(4,  PnD->RB112, PMD->RB112); 
    MovNByte(4,  PnD->RB113, PMD->RB113); 
    MovNByte(4,  PnD->RB114, PMD->RB114); 

    /* F36 ����ʱ����λ����λ*/
    MovNByte(3,  PnD->RA020, PMD->RA020);       //;��ǰ�����й��������
    MovNByte(3,  PnD->RA021, PMD->RA021);
    MovNByte(3,  PnD->RA022, PMD->RA022);
    MovNByte(3,  PnD->RA023, PMD->RA023);
    MovNByte(3,  PnD->RA024, PMD->RA024);
    MovNByte(4,  PnD->RB020, PMD->RB020);       //;��ǰ�����й������������ʱ��
    MovNByte(4,  PnD->RB021, PMD->RB021); 
    MovNByte(4,  PnD->RB022, PMD->RB022); 
    MovNByte(4,  PnD->RB023, PMD->RB023); 
    MovNByte(4,  PnD->RB024, PMD->RB024); 
    
    MovNByte(3,  PnD->RA120, PMD->RA120);       //;��ǰ�����޹��������
    MovNByte(3,  PnD->RA121, PMD->RA121);
    MovNByte(3,  PnD->RA122, PMD->RA122);
    MovNByte(3,  PnD->RA123, PMD->RA123);
    MovNByte(3,  PnD->RA124, PMD->RA124);
    MovNByte(4,  PnD->RB120, PMD->RB120);       //;��ǰ�����޹������������ʱ��
    MovNByte(4,  PnD->RB121, PMD->RB121); 
    MovNByte(4,  PnD->RB122, PMD->RB122); 
    MovNByte(4,  PnD->RB123, PMD->RB123); 
    MovNByte(4,  PnD->RB124, PMD->RB124); 

    /* F37 */
    Format14Energy5(PnD->R9410, PMD->R9410);
    MovNByte(20, PnD->R9510, PMD->R9510);       //;���������޹������ʵ���
    MovNByte(40, PnD->R9530, PMD->R9530);       //;����һ�������޹������ʵ���

    /* F38 */
    Format14Energy5(PnD->R9420, PMD->R9420);
    MovNByte(20, PnD->R9520, PMD->R9520);       //;���·����޹������ʵ���
    MovNByte(40, PnD->R9550, PMD->R9550);       //;���¶��������޹������ʵ���

   /* F39 ����ʱ����λ����λ*/
    MovNByte(3,  PnD->RA410, PMD->RA410);       //;��һ�����������й��������
    MovNByte(3,  PnD->RA411, PMD->RA411);
    MovNByte(3,  PnD->RA412, PMD->RA412);
    MovNByte(3,  PnD->RA413, PMD->RA413);
    MovNByte(3,  PnD->RA414, PMD->RA414);
    MovNByte(4,  PnD->RB410, PMD->RB410);       //;��һ�����������й������������ʱ��
    MovNByte(4,  PnD->RB411, PMD->RB411); 
    MovNByte(4,  PnD->RB412, PMD->RB412); 
    MovNByte(4,  PnD->RB413, PMD->RB413); 
    MovNByte(4,  PnD->RB414, PMD->RB414); 
    
    MovNByte(3,  PnD->RA510, PMD->RA510);       //;��һ�����������޹��������
    MovNByte(3,  PnD->RA511, PMD->RA511);
    MovNByte(3,  PnD->RA512, PMD->RA512);
    MovNByte(3,  PnD->RA513, PMD->RA513);
    MovNByte(3,  PnD->RA514, PMD->RA514);
    MovNByte(4,  PnD->RB510, PMD->RB510);       //;��һ�����������޹������������ʱ��
    MovNByte(4,  PnD->RB511, PMD->RB511); 
    MovNByte(4,  PnD->RB512, PMD->RB512); 
    MovNByte(4,  PnD->RB513, PMD->RB513); 
    MovNByte(4,  PnD->RB514, PMD->RB514); 

    /* F40 ����ʱ����λ����λ*/
    MovNByte(3,  PnD->RA420, PMD->RA420);       //;��һ�����շ����й��������
    MovNByte(3,  PnD->RA421, PMD->RA421);
    MovNByte(3,  PnD->RA422, PMD->RA422);
    MovNByte(3,  PnD->RA423, PMD->RA423);
    MovNByte(3,  PnD->RA424, PMD->RA424);
    MovNByte(4,  PnD->RB420, PMD->RB420);       //;��һ�����շ����й������������ʱ��
    MovNByte(4,  PnD->RB421, PMD->RB421); 
    MovNByte(4,  PnD->RB422, PMD->RB422); 
    MovNByte(4,  PnD->RB423, PMD->RB423); 
    MovNByte(4,  PnD->RB424, PMD->RB424); 
    
    MovNByte(3,  PnD->RA520, PMD->RA520);       //;��һ�����շ����޹��������
    MovNByte(3,  PnD->RA521, PMD->RA521);
    MovNByte(3,  PnD->RA522, PMD->RA522);
    MovNByte(3,  PnD->RA523, PMD->RA523);
    MovNByte(3,  PnD->RA524, PMD->RA524);
    MovNByte(4,  PnD->RB520, PMD->RB520);       //;��һ�����շ����޹������������ʱ��
    MovNByte(4,  PnD->RB521, PMD->RB521); 
    MovNByte(4,  PnD->RB522, PMD->RB522); 
    MovNByte(4,  PnD->RB523, PMD->RB523); 
    MovNByte(4,  PnD->RB524, PMD->RB524); 
    
    
    MovNByte(12, PnD->RC030, PMD->RC030); //;�����,���
    MovNByte(6,  PnD->RC117, PMD->RC117); //;�Զ�������
    MovNByte(3,  PnD->RB710, PMD->RB710); //;ʧѹ�ܴ���

    if(FirstCB == true)
    {    
        PnD->CB_Time[0] = PMD->RC011[1];
        PnD->CB_Time[1] = PMD->RC011[2];
        PnD->CB_Time[2] = PMD->RC010[1];
        PnD->CB_Time[3] = PMD->RC010[2];
        PnD->CB_Time[4] = PMD->RC010[3];    
/*		
        MovNByte(5, PnD->O9010, PnD->R9010); //;��ǰ�����й��ܵ���  
        MovNByte(6, PnD->OB212, PnD->RB212); //;��̴���       
        MovNByte(6, PnD->OB21A, PnD->RB21A); //;�����һ�α��ʱ��
        MovNByte(3, PnD->OB213, PnD->RB213); //;��������������
        MovNByte(6, PnD->OC117, PnD->RC117); //;�Զ���������
        MovNByte(6, PnD->OC030, PnD->RC030);  //;�����(���޹�)
        MovNByte(6, PnD->OC040, PnD->RC040);  //;����������    
        MovNByte(6, PnD->OB310, PnD->RB310);  //;�����ܴ��� 
        MovNByte(3, PnD->OB710, PMD->RB710); //;ʧѹ�ܴ��� 
*/
        MovNByte(5, PnD->O9010, PnD->R9010); //;��ǰ�����й��ܵ���  
        MovNByte(3, PnD->OB212, PnD->RB212); //;��̴���       
        MovNByte(6, PnD->OB21A, PnD->RB21A); //;�����һ�α��ʱ��
        MovNByte(3, PnD->OB213, PnD->RB213); //;��������������
        MovNByte(6, PnD->OC117, PnD->RC117); //;�Զ���������
        MovNByte(6, PnD->OC030, PnD->RC030);  //;�����(���޹�)
        MovNByte(6, PnD->OC040, PnD->RC040);  //;����������    
        MovNByte(2, PnD->OB310, PnD->RB310);  //;�����ܴ��� 
        MovNByte(3, PnD->OB710, PnD->RB710); //;ʧѹ�ܴ��� 

        MovNByte(2, PnD->RC027, PMD->RC027);//;�����Ƿѹ״̬��

        MovNByte(3, PnD->RB222, PMD->RB222);   //;by zrt 20130828
        MovNByte(5, PnD->OB223, PMD->RB223+1);

        MovNByte(3, PnD->OB224, PnD->RB224);  //;by zrt 210130910
	
        ERC14_Judge(PnD->OB223,2);   //;�ն�����zrt20130801		
        
        HDJ_Read();       //;��Сʱ��������
        MVT_Read();       //;����ͳ�Ƶ���ֵ���ۼ�ʱ��
        PEZ_Read();       //;����������Ϊͳ���գ��µ�����׼��         
    }
    else
    {    
        ERCPn_EnergyJudge(pn); //;��ǰ��������ɳ���,���Զ����ж��¼�
        ERCPn_ParaJudge(pn);
    }
    ZDPN_DME(pn);   //;ͳ�Ƶ����µ�����
    EW.MoveByte = '0';
}


/* 
********************************************************************************
* �� �� ����ZDCB_PortInit
* ��    �ܣ��ն˳�������ֳ�ʼ��
* ��    ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void ZDCB_PortInit(struct PortCtrl *Port)
{
//;    Port->Status = 0;
//;    Port->Mark = 0;
    Port->Index = 0;
    Port->PnOver = 0;
    Port->Over = 0;
//;    Port->MeterNum[0] = 0;
//;    Port->MeterNum[1] = 0;
//;    Port->MeterNum[2] = 0;
//;    Port->MeterNum[3] = 0;
//;    Port->MeterNum[4] = 0;
//;    Port->MeterNum[5] = 0;
    Port->GYType = 0;
    Port->IndSend = 0;
    Port->IndSave = 0;
//;    Port->DI1DI0 = 0;
    Port->WaitAns = 0;
    Port->ReSNum = 0;
    Port->ErrNum = 0;
    Port->PnErrNum = 0;
}


/*----------------------------------------------------------------------------*
 |  ÿ�����жϵ�ǰ�ն˳������Ƿ���
 *----------------------------------------------------------------------------*/
/* 
********************************************************************************
* �� �� ����ZD_CBD_CHK
* ��    �ܣ�ÿ�����жϵ�ǰ�ն˳������Ƿ���
* ��    ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void ZD_CBD_CHK(void)
{
    u32 ch1 = 0, ch2 = 1;
    u8 i, Day, Hour, Min;
    
    Min  = ZDP.F33.PortP[0].RTime[0];  
    Hour = ZDP.F33.PortP[0].RTime[1];
    
    if((TBCD.Sec ==0x00) && (TBCD.Min==Min) && (TBCD.Hour==Hour))
    {
        Day = TBCD.Day;
        ch1 |= ZDP.F33.PortP[0].RDay[3];
        ch1 <<= 8;
        ch1 |= ZDP.F33.PortP[0].RDay[2];
        ch1 <<= 8;
        ch1 |= ZDP.F33.PortP[0].RDay[1];
        ch1 <<= 8;
        ch1 |= ZDP.F33.PortP[0].RDay[0];
        Day = (Day / 0x10) * 10 + (Day % 0x10 );
        
        for(i=1; i<32; i++)
        {
            if(ch1 & ch2)
            {
                if(Day == i)
                {
                    BDJPn.Status = 'E'; //;�����ն���
                    ZDCB_PortInit(&Port1);
                    ZDCB_PortInit(&Port2);
                    ZDCB.Status = 'E';  //;����ʼ
                    ZDCB.Mark = 'B';    //;�����ճ���
                }
            }
            ch2 <<= 1;
        }
    }
}



/* 
********************************************************************************
* �� �� ����ZD_CB_CHK
* ��    �ܣ��ն˳���������ʱ����
* ��    ����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
void ZD_CB_CHK(void)
{
    if(ZDCB.CouDown > 0)
    {
        ZDCB.CouDown--;
    }
    
    if(ZDCB.CouDown == 0) //;���ݳ�����������
    { 
        if(ZDCB.Status == 0)
        {
            if(ZDP.F33.PortP[0].RTimeDis != 0)
            {           
                ZDCB.CouDown = ZDP.F33.PortP[0].RTimeDis;
                ZDCB.CouDown *= 60;               
            }
            else
            {
                ZDCB.CouDown = 120; //;Ĭ��2����
            }

            //���ӳ���ʱ�ο���
			
            ZDCB_PortInit(&Port1);
#ifdef PORT2USART             
            ZDCB_PortInit(&Port2);
#endif             
            ZDCB.Status = 'E';
        }
    }
    
    ZD_CBD_CHK(); //;�жϵ�ǰ�ն˳������Ƿ���
}

/* 
********************************************************************************
* �� �� ����ZDCB_PointFind
* ��    �ܣ��ҳ��˿ڶ�Ӧ�Ĳ��������
* ��    ����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
u8 ZDCB_PointFind(struct PortCtrl *Port)
{
    u8 temp, PortX, BPS;

    if(!(Port->PnOver & (0x0001<<Port->Index)))   //;��ǰ������δ����
    {
        temp = ZDP.F10.Meter[Port->Index].BPS_Port;
        PortX = temp & 0x1F;

        if((PortX == 1) || (PortX == 2))
        {
            BPS = (temp & 0xE0) >> 5;
            Port->GYType = ZDP.F10.Meter[Port->Index].Type; //;��ǰ���õ�ͨ�Ź�Լ����
            EW.MoveByte = 'M';
            MovNByte(6, Port->MeterNum, ZDP.F10.Meter[Port->Index].Addr); //;д�뵱ǰ������ŵı��
            EW.MoveByte = 0;
            if((Port==&Port1) && (PortX==1))
            {   
                if((BPS != 0x03) && (BPS != 0x00))
                {
                    Port->BPS = 0x03;
                    UARTx_BaudRate_Set(0x03, 5);//;�����ͨ��ֻ֧��2400bps --20141229
                }  
                return true;
            }
#ifdef PORT2USART            
            else if((Port==&Port2) && (PortX==2))
            {        
                if(Port->BPS != BPS)
                {     
                    Port->BPS = BPS;
                    UARTx_BaudRate_Set(BPS, 4);
                }
                return true;
            }
#endif            
        }

        Port->PnOver |= (0x0001 << Port->Index);
    }
    return false;
}


/* 
********************************************************************************
* �� �� ����ZDCB_DIFind
* ��    �ܣ��ҳ�����ı�ʶ
* ��    ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void ZDCB_DIFind(struct PortCtrl *Port)
{
    u32 ComTab_Head ;

    if(Port == &Port1)
    {
        if(Port->GYType == GYType2007)
        {
            Port->IndSave = DITab_2007_Num;
            ComTab_Head = (u32)DITab_2007;
            Port->DI1DI0 = *((u32*)(ComTab_Head + Port->IndSend * DITab_Num));
        }
        else if(Port->GYType == GYType1997)
        {
            Port->IndSave = DITab_1997_Num;
            ComTab_Head = (u32)DITab_1997;
            Port->DI1DI0 = *((u16*)(ComTab_Head + Port->IndSend * DITab_Num));
        }
    }
   
}


/* 
********************************************************************************
* �� �� ����ZDCB_OrderSend
* ��    �ܣ���������������֡
* ��    ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void ZDCB_OrderSend(struct PortCtrl *Port)
{
    u8 i, sum = 0, j = 0;

    if(Port == &Port1)
    {
        Port->Buffer = PORT1SEDBUF;
    }
#ifdef PORT2USART
    else if(Port == &Port2)
    {
        Port->Buffer = PORT2SEDBUF;
    }
#endif

    *(Port->Buffer + j++ ) = 0xFE;
    *(Port->Buffer + j++ ) = 0xFE;
    *(Port->Buffer + j++ ) = 0xFE;
    *(Port->Buffer + j++ ) = 0xFE;
    *(Port->Buffer + j++ ) = 0x68;
//;    *(Port->Buffer + 5 ) = Port->MeterNum[0];  //;���
//;    *(Port->Buffer + 6 ) = Port->MeterNum[1];
//;    *(Port->Buffer + 7 ) = Port->MeterNum[2];
//;    *(Port->Buffer + 8 ) = Port->MeterNum[3];
//;    *(Port->Buffer + 9 ) = Port->MeterNum[4];
//;    *(Port->Buffer + 10) = Port->MeterNum[5];
    *(Port->Buffer + j++ ) = 0xAA;  //;���
    *(Port->Buffer + j++ ) = 0xAA;
    *(Port->Buffer + j++ ) = 0xAA;
    *(Port->Buffer + j++ ) = 0xAA;
    *(Port->Buffer + j++ ) = 0xAA;
    *(Port->Buffer + j++) = 0xAA;
    *(Port->Buffer + j++) = 0x68;
    
    if(Port->GYType == GYType2007)
    {
        *(Port->Buffer + j++) = 0x11; //;��׼��Լ������
        *(Port->Buffer + j++) = 0x04;
        *(Port->Buffer + j++) = (u8)Port->DI1DI0 + 0x33;
        *(Port->Buffer + j++) = (u8)(Port->DI1DI0 >> 8) + 0x33;
        *(Port->Buffer + j++) = (u8)(Port->DI1DI0 >> 16) + 0x33;
        *(Port->Buffer + j++) = (u8)(Port->DI1DI0 >> 24) + 0x33;
    }
    else if(Port->GYType == GYType1997)
    {
        *(Port->Buffer + j++) = 0x01; //;��׼��Լ������
        *(Port->Buffer + j++) = 0x02;
        *(Port->Buffer + j++) = (u8)Port->DI1DI0 + 0x33;
        *(Port->Buffer + j++) = (u8)(Port->DI1DI0 >> 8) + 0x33;
    }

    for(i = 4; i < j; i++)
    {
        sum += *(Port->Buffer + i);
    }
    *(Port->Buffer + j++) = sum;
    *(Port->Buffer + j++) = 0x16;
    Port->Len = j;
}


/* 
********************************************************************************
* �� �� ����ZDSB_OrderSend
* ��    �ܣ������������
* ��    ����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��2011-07-19 13:40:09
********************************************************************************
*/
void ZDSB_OrderSend(struct PortCtrl *Port)
{
    u8 i, j = 0, sum = 0, n;
    u32 DI;

    if(Port == &Port1)
    {
        Port->Buffer = PORT1SEDBUF;
    }
#ifdef PORT2USART
    else if(Port == &Port2)
    {
        Port->Buffer = PORT2SEDBUF;
    }
#endif
    
    DI = CtrlOrder.DI[CtrlOrder.IndSend % SBNUM];
    
    *(Port->Buffer + j++ ) = 0xFE;
    *(Port->Buffer + j++ ) = 0xFE;
    *(Port->Buffer + j++ ) = 0xFE;
    *(Port->Buffer + j++ ) = 0xFE;
    *(Port->Buffer + j++ ) = 0x68;
//;    *(Port->Buffer + 5 ) = CtrlOrder.MeterNum[0];  //;���
//;    *(Port->Buffer + 6 ) = CtrlOrder.MeterNum[1];
//;    *(Port->Buffer + 7 ) = CtrlOrder.MeterNum[2];
//;    *(Port->Buffer + 8 ) = CtrlOrder.MeterNum[3];
//;    *(Port->Buffer + 9 ) = CtrlOrder.MeterNum[4];
//;    *(Port->Buffer + 10) = CtrlOrder.MeterNum[5];
    *(Port->Buffer + j++ ) = 0xAA;  //;���
    *(Port->Buffer + j++ ) = 0xAA;
    *(Port->Buffer + j++ ) = 0xAA;
    *(Port->Buffer + j++ ) = 0xAA;
    *(Port->Buffer + j++ ) = 0xAA;
    *(Port->Buffer + j++) = 0xAA;
    *(Port->Buffer + j++) = 0x68; 
    if(Port->GYType == GYType2007)
    {  
        *(Port->Buffer + j++) = 0x14; //;��׼��Լ�����
        n = j;
        *(Port->Buffer + j++) = 12; //;��׼��Լ�����
        *(Port->Buffer + j++) = (u8)DI + 0x33;
        *(Port->Buffer + j++) = (u8)(DI >> 8) + 0x33;
        *(Port->Buffer + j++) = (u8)(DI >> 16) + 0x33;
        *(Port->Buffer + j++) = (u8)(DI >> 24) + 0x33;
        *(Port->Buffer + j++) = 0x33;    //;������Ȩ��
        *(Port->Buffer + j++) = 0x33;
        *(Port->Buffer + j++) = 0x33;
        *(Port->Buffer + j++) = 0x33;
        *(Port->Buffer + j++) = 0x33;    //;�����ߴ���
        *(Port->Buffer + j++) = 0x33;
        *(Port->Buffer + j++) = 0x33;
        *(Port->Buffer + j++) = 0x33;
        
        switch(DI)
        {
            case 0x04001301: //;����״̬
            {
                *(Port->Buffer + j++) = GprsSign + 0x33;    //; �źŵĴ�С
                *(Port->Buffer + n) += 1; //;���ݸ���
            }
            break;
            case 0x04000101: //;����
            {
                *(Port->Buffer + j++) = (TBCD.Month >> 5) + 0x33;
                *(Port->Buffer + j++) = TBCD.Day + 0x33;
                *(Port->Buffer + j++) = (TBCD.Month & 0x1F)  + 0x33;
                *(Port->Buffer + j++) = TBCD.Year + 0x33;
                *(Port->Buffer + n) += 4; //;���ݸ���
            }
            break;
            case 0x04000102: //;����
            {
                *(Port->Buffer + j++) = TBCD.Sec + 0x33 ; //;��
                *(Port->Buffer + j++) = TBCD.Min  + 0x33; //;��
                *(Port->Buffer + j++) = TBCD.Hour + 0x33; //;ʱ
                *(Port->Buffer + n) += 3; //;���ݸ���
            }
            break;
            
            default:
            break;
        }
    }
    else if(Port->GYType == GYType1997)
    {
        *(Port->Buffer + j++) = 0x04; //;��׼��Լ�����
        n = j;
        *(Port->Buffer + j++) = 6; //;��׼��Լ�����
        *(Port->Buffer + j++) = (u8)DI + 0x33;
        *(Port->Buffer + j++) = (u8)(DI >> 8) + 0x33;
        *(Port->Buffer + j++) = 0x33;    //;������Ȩ��
        *(Port->Buffer + j++) = 0x33;
        *(Port->Buffer + j++) = 0x33;
        *(Port->Buffer + j++) = 0x33;
        switch((u16)DI)
        {
            case 0xCA00:
            {
                *(Port->Buffer + j++) = GprsSign + 0x33;    //; �źŵĴ�С
                *(Port->Buffer + n) += 1;
            }
            break;
            case 0xC900: //;IP��ַ1��
            {
                   *(Port->Buffer + n) += 4;                
                   *(Port->Buffer + j++) = ZDP.F3.IP1[0] + 0x33;
                   *(Port->Buffer + j++) = ZDP.F3.IP1[1] + 0x33;
                   *(Port->Buffer + j++) = ZDP.F3.IP1[2] + 0x33;
                   *(Port->Buffer + j++) = ZDP.F3.IP1[3] + 0x33;
               }
               break;
            case 0xC901: //;�˿ڵ�ַ1
               {
                   *(Port->Buffer + n) += 2;
                   *(Port->Buffer + j++) = ZDP.F3.Port1[0] + 0x33;
                   *(Port->Buffer + j++) = ZDP.F3.Port1[1] + 0x33;
//;                   *(Port->Buffer + j++) = ZDP.F3.Port1[1] + 0x33;
//;                   *(Port->Buffer + j++) = ZDP.F3.Port1[0] + 0x33;
               }
               break;
//;            case 0xF313: //;������ C010
//;               {
//;                   j = 13;
//;                   *(Port->Buffer + j++) = 0x0A;
//;                   *(Port->Buffer + j++) = 0x10 + 0x33;
//;                   *(Port->Buffer + j++) = 0xC0 + 0x33;
//;                   j = 20;
//;                   *(Port->Buffer + j++) = 0x00 + 0x33;
//;                   *(Port->Buffer + j++) = TBCD_bak.Day + 0x33;
//;                   *(Port->Buffer + j++) = TBCD_bak.Month & 0x0F + (TBCD_bak.Month & 0x10) * 10  + 0x33;
//;                   *(Port->Buffer + j++) = TBCD_bak.Year + 0x33;
//;
//;               }
//;               break;
//;            case 0xF314: //;��ʱ�� C011
//;               {
//;                    j = 13;
//;                    *(Port->Buffer + j++) = 0x09;
//;                    *(Port->Buffer + j++)  = 0x11 + 0x33;
//;                    *(Port->Buffer + j++)  = 0xC0 + 0x33;
//;                    j = 20;
//;                    *(Port->Buffer + j++) = TBCD_bak.Sec + 0x33 ; //;��
//;                    *(Port->Buffer + j++) = TBCD_bak.Min  + 0x33; //;��
//;                    *(Port->Buffer + j++) = TBCD_bak.Hour + 0x33; //;ʱ
//;               }
//;               break;
            case 0xC902: //;IP��ַ2��
               {
                   *(Port->Buffer + n) += 4;
                   *(Port->Buffer + j++) = ZDP.F3.IP2[0] + 0x33;
                   *(Port->Buffer + j++) = ZDP.F3.IP2[1] + 0x33;
                   *(Port->Buffer + j++) = ZDP.F3.IP2[2] + 0x33;
                   *(Port->Buffer + j++) = ZDP.F3.IP2[3] + 0x33;
               }
               break;
            case 0xC903: //;�˿ڵ�ַ2
               {
                   *(Port->Buffer + n) += 2;
                   
                   *(Port->Buffer + j++) = ZDP.F3.Port2[0] + 0x33;
                   *(Port->Buffer + j++) = ZDP.F3.Port2[1] + 0x33;
               }
               break;
            case 0xC904: //; APN
               {
                   *(Port->Buffer + n) += 16;

                   for (i = 0; i < 16; i++)
                   {
                       *(Port->Buffer + j++) = ZDP.F3.APN[i] + 0x33;
                   }
               }
               break;
            case 0xC905:  //;��վ�ֻ��绰����
               {
                   *(Port->Buffer + n) += 8;
                  
                   for (i = 0; i < 8; i++)
                   {
                       *(Port->Buffer + j++) = ZDP.F4.Tel_Code[7-i] + 0x33;
                   }
               }
               break;
            case 0xC906:  //;��վ�������ĺ���
               {
                   *(Port->Buffer + n) += 8;

                   for(i = 0; i < 8; i++)
                   {
                       *(Port->Buffer + j++) = ZDP.F4.SMS_Code[7-i] + 0x33;
                   }
               }
               break;
//;            case 0xC040: //;��ѹ����������
//;               {
//;                   *(Port->Buffer + j - 7) = 10 ;
//;                  
//;                   *(Port->Buffer + j++) = 0 + 0x33;
//;                   *(Port->Buffer + j++) = P1P.F25.UMul[0] + 0x33; //;PT���
//;                   *(Port->Buffer + j++) = P1P.F25.UMul[1] + 0x33;
//;                   *(Port->Buffer + j++) = 0 + 0x33;
//;               }
//;               break;
//;            case 0xC041:  //;��������������
//;               {
//;                   *(Port->Buffer + j - 7) = 10 ;
//;                  
//;                   *(Port->Buffer + j++) = 0 + 0x33;
//;                   *(Port->Buffer + j++) = P1P.F25.IMul[0] + 0x33; //;CT���
//;                   *(Port->Buffer + j++) = P1P.F25.IMul[1] + 0x33;
//;                   *(Port->Buffer + j++) = 0 + 0x33;
//;               }
//;               break;
//;            case 0xC141:  //;���ѹ
//;               {
//;                   *(Port->Buffer + j - 7) = 9 ;
//;                  
//;                   *(Port->Buffer + j++) = 0 + 0x33;
//;                   *(Port->Buffer + j++) = P1P.F25.Un[0] + 0x33;
//;                   *(Port->Buffer + j++) = P1P.F25.Un[1] + 0x33;
//;                   break;
//;               }
            case 0xC907: //;�ն�����������
            {
                *(Port->Buffer + n) += 2;
              
                *(Port->Buffer + j++) = ZDPKZ.F89.QuXian[0] + 0x33;
                *(Port->Buffer + j++) = ZDPKZ.F89.QuXian[1] + 0x33;
                break;
            }
            case 0xC908: //;�ն˵�ַ
            {
                u8 DiZhi[2];

                *(Port->Buffer + n) += 2;
                
                U16To2BCD(*(u16*)ZDPKZ.F89.DiZhi, DiZhi);
                *(Port->Buffer + j++) = DiZhi[0] + 0x33;
                *(Port->Buffer + j++) = DiZhi[1] + 0x33;
                break;
            }
            case 0xC912:
            {
                *(Port->Buffer + n) += 1;              
                *(Port->Buffer + j++) = (ZDP.F1.HBeat / 10) * 16 + (ZDP.F1.HBeat % 10)+ 0x33;
                break;
            }
            case 0xC910:
            {
                *(Port->Buffer + n) += 1;
                *(Port->Buffer + j++) = (ZDP.F8.WorkMode & 0x03) + 0x33;
                break;
            }
            case 0xC911:
            {
                u16 t;
                *(Port->Buffer + n) += 2;
                t = ZDP.F8.ActRedailSec[0] + (ZDP.F8.ActRedailSec[1] << 8);
                t = t / 60;
                U16To2BCD(t, (u8*)Port->Buffer + j);
                *(Port->Buffer + j++) += 0x33;
                *(Port->Buffer + j++) += 0x33;
                break;
            }
            case 0x6601:
            {
                *(Port->Buffer + n) += 1;
                *(Port->Buffer + j++) = CtrlOrder.Order[CtrlOrder.IndSend % SBNUM] + 0x33;
            }
            default:
               break;
        }
    }
    
    
    for(i = 4; i < j; i++)
    {
        sum += *(Port->Buffer + i);
    }
    *(Port->Buffer + j++) = sum;
    *(Port->Buffer + j++) = 0x16;
    Port->Len = j;
}


/* 
********************************************************************************
* �� �� ����ZDCB_Port
* ��    �ܣ����ƶ˿ڳ���
* ��    ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void ZDCB_Port(struct PortCtrl *Port, struct MeterData *PMD)
{
    u8 Len;
    static u8 comstat = false;
    
    if (Port->Over == false)
    {
        if (Port->Status == idle) //;��ǰ����״̬
        {
            if (ZDCB_PointFind(Port) == true)
            {
                Port->Status = 'F'; //;�ҵ���������г���
                if (Port == &Port1)
                {
                    PMD1.CB_Time[0] = TBCD.Min;   
                    PMD1.CB_Time[1] = TBCD.Hour;  
                    PMD1.CB_Time[2] = TBCD.Day;   
                    PMD1.CB_Time[3] = TBCD.Month; 
                    PMD1.CB_Time[4] = TBCD.Year;
                }
                else
                {
                    PMD2.CB_Time[0] = TBCD.Min;   
                    PMD2.CB_Time[1] = TBCD.Hour;  
                    PMD2.CB_Time[2] = TBCD.Day;   
                    PMD2.CB_Time[3] = TBCD.Month; 
                    PMD2.CB_Time[4] = TBCD.Year;  
                }
            }
            else
            {
                Port->Index++;
                if(Port->Index >= *((u16*)ZDP.F10.MeterNum))
                {
                    Port->Index = 0;
                    Port->Over = true; //;�˿���ɳ���
                }
            }
        }
    }

    if(Port->Status == 'F')
    {
        ZDCB_DIFind(Port); //;�ҵ���ʶ����
        ZDCB_OrderSend(Port);
        Port->Status = 'E'; //;�˿ڷ��Ϳ�ʼ
        Port->Mark = 'B';   //;�˿��ڳ���
    }

    if(Port->Mark == 'B') //;����򳭱��ճ���
    {      
        if(Port->Status == 'S')//;����״̬
        {
            Port->WaitAns++;
            if (Port->WaitAns > 12) //;3���޻�Ӧ
            {
                Port->WaitAns = 0;
                Port->ReSNum++;
                if(Port->ReSNum < 2)
                {
                    Port->Status = 'E'; //;�ط�
                }
                else if(Port->ReSNum < 3)
                {
//;                    if((Port->BPS == 0x06)||(Port->BPS == 0x00))
//;                    {
//;                        Port->BPS = 0x08;
//;                        UARTx_BaudRate_Set(0x08, 3);
//;                    }
//;                    else if(Port->BPS == 0x08)
//;                    {
//;                        Port->BPS = 0x06;
//;                        UARTx_BaudRate_Set(0x06, 3);
//;                    }
                    Port->Status = 'E'; //;�ط�
                }
                else
                {
                    Port->ReSNum = 0;
                    Port->ErrNum++;               
                    if (Port->ErrNum < 2)
                    {
             //           Port->ErrNum = 0x00;
                        Port->IndSend++;
                        Port->Status = 'F';                                         //;ָ����һ����ʶ������
                        if (Port->IndSend >= Port->IndSave)
                        {
                            Port->IndSend = 0;
                            Port->PnOver |= (0x0001 << Port->Index);                //;��ǰ�������ұ����
                            Port->Status = idle;
                        }
                    }
                    else                                                            //;����������ʶδ����������ǰ������
                    {
                        Port->Mark = 0;
                        Port->ErrNum = 0;
                        Port->IndSend = 0;
                        Port->PnOver |= (0x0001 << Port->Index); //;��ǰ�������ұ����
                        Port->Status = idle;
                        Port->PnErrNum++;                       //;�ۼ�û�г����Ĳ��������                   
                        ERC21_4Judge(1);                        //;�ж��Ƿ���485�������
                        if(comstat)
                        {
                            comstat = false;
                            COMLIGHTOFF();  //;ͨ��ʧ��ָʾ
                        }
                    }
                }
            } 
        }
        
        if(Port->Status == 'R')//;���յ�����ظ�
        {
            Port->Mark = 0;
            Port->WaitAns = 0;
            Port->ReSNum = 0;
            Port->ErrNum = 0;
            Len = DL645_CHK(Port->Buffer, Port->Len);
            if (Len != 0xFF) //;����5�յ�654֡ 
            {
               if (MeterNumChk(ZDP.F10.Meter[0].Addr,Port->Buffer + Len + 1))
               {
                  MovNByte(6, ZDP.F10.Meter[0].Addr, (u8*)(Port->Buffer + Len + 1));
                  EW.CSB1 = 0xC1;
                  if(!f_is_sole_data ((u8*)(Port->Buffer + Len + 1),6))/*���Ϊȫ0,���޸�������*/
                  {
                  	u8 buf[4];
                  	buf[0] = 0x25;
                  	buf[1] = ZDP.F10.Meter[0].Addr[3];
                  	buf[2] = ZDP.F10.Meter[0].Addr[2];
                  	buf[3] = ZDP.F10.Meter[0].Addr[1];
                  	EW.MoveByte = 'M';
                  	MovNByte(4,  ZDPKZ.F89.QuXian,(u8*)&buf[0]);
                  	EW.MoveByte = 0;
                  	EW.CSB8 = 0xC8;
                  	GprsC.ReConnect = true; //;�ն���������ģ���Ҫ��������
                  }	
               }            
                if(Port == &Port1)
                {
                    METERDATALIGHTON(); //;���ݵ���
                    PORT1USARTCTR.DataLightMs = 100;
                    if(comstat == false)
                    {
                        comstat = true;
                        COMLIGHTON();  //;ͨ������ָʾ
                    }
                }
                
                ZDCB_MovDataDI(Port->Buffer + Len, PMD, Port); //;���Ӷ˿ڽ��յ�������д�뵽PMD��
            }
            Port->IndSend++;
            Port->Status = 'F'; //;ָ����һ����ʶ������
            if (Port->IndSend >= Port->IndSave)
            {
                Port->IndSend = 0;
                Port->PnOver |= (0x0001 << Port->Index); //;��ǰ�����㳭�����
                ZDCB_MovDataPn(Port->Index, Port->GYType, PMD); //;��ɳ��������д�뵱ǰ��������
                Port->Status = idle;
            }
            ERC21_4Judge(2);   //;�ж�485��������Ƿ�ָ�
        }
    }
}


/* 
********************************************************************************
* �� �� ����ZD_Port1
* ��    �ܣ������ն˶˿�1�����ݷ��������
* ��    ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void ZD_Port1(void)  
{
    if(PORT1USARTCTR.RecOrd == true) //;����5��������
    {
        PORT1USARTCTR.RecOrd = false; 
        if(Port1.Status == 'S') //;�˿�1�����������״̬,�ȴ���������
        {
            Port1.Status = 'R'; //;�˿�1�������ݳɹ�
            Port1.Buffer =  PORT1REVBUF; //;ָ��˿�1�������ݻ�����
            Port1.Len = PORT1USARTCTR.RecLen; //;�˿�1�ӵ��ĳ���
            EW.MoveByte = 'M';
            MovNByte(Port1.Len, (u8*)Port1.Buffer, (u8*)PORT1USARTCTR.BufAdr); //;�˿�1���յ�����д�봮��5������
            EW.MoveByte = 0;
        }        
    }
    
    if(PORT1USARTCTR.SedOrd == 0) //;����5����
    {
        if(Port1.Status == 'E')//;�˿�1������������
        {
            Port1.Status = 'S'; //;�˿�1�����������״̬
            PORT1USARTCTR.SedOrd = 'E';  //;����5������������
            PORT1USARTCTR.BufAdr = Port1.Buffer;
            PORT1USARTCTR.TLong = Port1.Len;
        }
    }
}

#ifdef PORT2USART
/* 
********************************************************************************
* �� �� ����ZD_Port2
* ��    �ܣ������ն˶˿�2�����ݷ��������
* ��    ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void ZD_Port2(void)  
{
    if (PORT2USARTCTR.RecOrd == true) //;����4��������
    {
        PORT2USARTCTR.RecOrd = false; 
        if (Port2.Status == 'S') //;�˿�2�����������״̬,�ȴ���������
        {
            Port2.Status = 'R'; //;�˿�2�������ݳɹ�
            Port2.Buffer =  PORT2REVBUF; //;ָ��˿�2�������ݻ�����
            Port2.Len = PORT2USARTCTR.RecLen; //;�˿�2�ӵ��ĳ���
            EW.MoveByte = 'M';
            MovNByte(Port2.Len, (u8*)Port2.Buffer, (u8*)PORT2USARTCTR.BufAdr); //;�˿�1���յ�����д�봮��4������
            EW.MoveByte = 0;
        }  
    }
    if ((Port2.Status == 'E') && (PORT2USARTCTR.SedOrd == 0))//;�˿�2�������������Ҵ���4����
    {
        Port2.Status = 'S'; //;�˿�2�����������״̬
        PORT2USARTCTR.SedOrd = 'E';  //;����4������������
        PORT2USARTCTR.BufAdr = Port2.Buffer;
        PORT2USARTCTR.TLong = Port2.Len;
    }
}
#endif


/* 
********************************************************************************
* �� �� ����ZD_Port_Com
* ��    �ܣ��ն˶˿�ͨѶ����
* ��    ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void ZD_Port_Com(void)  
{
    ZD_Port1();
#ifdef PORT2USART
    ZD_Port2();
#endif
}


/* 
********************************************************************************
* �� �� ����ZD_CB
* ��    �ܣ��ն˳������
* ��    ������
* ��    �أ���
* ˵    ����ÿ250msִ��һ��
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void ZD_CB(void)
{
    if (ZDCB.Status == 'E') //;�ն˳���ʼ
    {
        ZDCB_Port(&Port1, &PMD1);
#ifdef PORT2USART
        ZDCB_Port(&Port2, &PMD2);
        if((Port1.Over == true) && (Port2.Over == true))
        {
            ZDCB.Status = 'O'; //;�������
        } 
#else
        if((Port1.Over == true))
        {
            ZDCB.Status = 'O'; //;�������
        } 
#endif
        
    }
    
    if(ZDCB.Status == 'O')//;������ɺ���ݲ�ͬ�ĳ������ͽ��д���
    {
        ZDCB.Status = 0;     
        if(ZDCB.Mark == 'B') //;�����ճ���
        {           
            if(BDJPn.Status == 0)
            {
                BDJPn.Pn = 0; //;�ӵ�һ�������㿪ʼ����
                BDJPn.Status = 'E'; //;���Ὺʼ
            }
        }
        ZDCB.Mark = 0;
    }
}


/* 
********************************************************************************
* �� �� ����Send_Modem_Status
* ��    �ܣ�����ģ���ź�״̬�����Ƿ������͸�������
* ��    ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void Send_Modem_Status(void)
{
    static u8 OldStatus = 0, OldSign = 0, OldGprsSign = 0;
    
    if((OldStatus != GprsC.State) || (OldSign != GprsC.Sign))
    {
        OldStatus = GprsC.State;
        OldSign = GprsC.Sign;
        if(GprsC.Sign >= 22)
        {
            GprsSign = 0x04;
        }
        else if(GprsC.Sign >= 14)
        {
            GprsSign = 0x03;
        }
        else if(GprsC.Sign >=7)
        {
            GprsSign = 0x02;
        }
        else if(GprsC.Sign >= 1)
        {
            GprsSign = 0x01;
        }
        else
        {
            GprsSign = 0x00;
        }
        if(GprsC.State & _HadLogin)
        {
            GprsSign |= 0x80;
        }
        else
        {
            GprsSign &= 0x7F;
        }
        if(GprsSign != OldGprsSign)
        {
            OldGprsSign = GprsSign;
            CtrlOrder.Pn[CtrlOrder.IndSave % SBNUM] = 1;
            if(Port1.GYType == GYType2007)
            {
                CtrlOrder.DI[CtrlOrder.IndSave % SBNUM] = 0x04001301;               
            }
            else if(Port1.GYType == GYType1997)
            {
                CtrlOrder.DI[CtrlOrder.IndSave % SBNUM] = 0xCA00;
            }
            CtrlOrder.IndSave++; 
        }
    }
}

/*�����ʱ��*/
void Send_TimetoMeter(void)
{
    CtrlOrder.Pn[CtrlOrder.IndSave % SBNUM] = 1;//;����
    CtrlOrder.DI[CtrlOrder.IndSave % SBNUM] = 0x04000101;
    CtrlOrder.IndSave++; 
    CtrlOrder.Pn[CtrlOrder.IndSave % SBNUM] = 1;//;ʱ��
    CtrlOrder.DI[CtrlOrder.IndSave % SBNUM] = 0x04000102;
    CtrlOrder.IndSave++; 
}

/*��IP��˿� */
void Send_ZDPF3toMeter(void)
{
    CtrlOrder.Pn[CtrlOrder.IndSave % SBNUM] = 1;
    CtrlOrder.DI[CtrlOrder.IndSave % SBNUM] = 0xC900;
    CtrlOrder.IndSave++;
    CtrlOrder.Pn[CtrlOrder.IndSave % SBNUM] = 1;
    CtrlOrder.DI[CtrlOrder.IndSave % SBNUM] = 0xC901;
    CtrlOrder.IndSave++;
    CtrlOrder.Pn[CtrlOrder.IndSave % SBNUM] = 1;
    CtrlOrder.DI[CtrlOrder.IndSave % SBNUM] = 0xC902;
    CtrlOrder.IndSave++;
    CtrlOrder.Pn[CtrlOrder.IndSave % SBNUM] = 1;
    CtrlOrder.DI[CtrlOrder.IndSave % SBNUM] = 0xC903;
    CtrlOrder.IndSave++;
    CtrlOrder.Pn[CtrlOrder.IndSave % SBNUM] = 1;//;APN
    CtrlOrder.DI[CtrlOrder.IndSave % SBNUM] = 0xC904;
    CtrlOrder.IndSave++;
}

/*���������� */
void Send_ZDPF1toMeter(void)
{
    CtrlOrder.Pn[CtrlOrder.IndSave % SBNUM] = 1;
    CtrlOrder.DI[CtrlOrder.IndSave % SBNUM] = 0xC912;
    CtrlOrder.IndSave++;
}

/*������ר��������ʽ */
void Send_ZDPF8toMeter(void)
{
    CtrlOrder.Pn[CtrlOrder.IndSave % SBNUM] = 1;
    CtrlOrder.DI[CtrlOrder.IndSave % SBNUM] = 0xC910;
    CtrlOrder.IndSave++;
    CtrlOrder.Pn[CtrlOrder.IndSave % SBNUM] = 1;
    CtrlOrder.DI[CtrlOrder.IndSave % SBNUM] = 0xC911;
    CtrlOrder.IndSave++;
}

void Send_ZDPF89toMeter(void)
{
    CtrlOrder.Pn[CtrlOrder.IndSave % SBNUM] = 1;//;����������
    CtrlOrder.DI[CtrlOrder.IndSave % SBNUM] = 0xC907;
    CtrlOrder.IndSave++;
    CtrlOrder.Pn[CtrlOrder.IndSave % SBNUM] = 1;//;�ն˵�ַ
    CtrlOrder.DI[CtrlOrder.IndSave % SBNUM] = 0xC908;
    CtrlOrder.IndSave++;
}


/* 
********************************************************************************
* �� �� ����ZD_SB
* ��    �ܣ������Ƴ���
* ��    ����
* ��    �أ� 
* ˵    ����ÿ250msִ��һ��
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��2012-01-31 14:05:00 ���ӳ���״̬�����������ʱ���泭��״̬�ֲ��Ƚ�
*                                �����Ĺ��ܡ�
*           2012-10-09 09:40:42  ���ӷ�����@1
*           2012-10-12 16:07:00  �޸Ĵ��󡣵��������ʱ�����@2
********************************************************************************
*/
void ZD_SB(void)
{
    u8 Pn, Len, i;
    u32 DI;
    static u8 Sts, Mark;
    struct ZDPStr_F10_1 *PnF10;
    
    if(CtrlOrder.IndSave > CtrlOrder.IndSend)
    {
        if(CtrlOrder.Status == idle)
        {
            Pn = CtrlOrder.Pn[CtrlOrder.IndSend % SBNUM];
            Pn -= 0x01;   
            PnF10 = ZDP.F10.Meter + Pn;
            CtrlOrder.Port = PnF10->BPS_Port & 0x1F;
            CtrlOrder.BPS  = (PnF10->BPS_Port & 0xE0) >> 5;
            MovNByte(6, CtrlOrder.MeterNum, PnF10->Addr); //;д�뵱ǰ������ŵı��
            CtrlOrder.Status = 'E';          
        }
        
        if(CtrlOrder.Status == 'E')
        {
            if(CtrlOrder.Port == 0x01) //;�˿�1
            {
                if((Port1.Status == idle) || (Port1.Status == 'F'))
                {
                    Sts = Port1.Status; //;���泭��״̬���������
                    Mark = Port1.Mark;
                    
                    CtrlOrder.Status = 'S';
                    Port1.Status = 'E'; //;�����ͣ���Ϊ���
                    Port1.Mark = 'C';   //;�˿������
                    if(Port1.BPS != CtrlOrder.BPS) //;ͨ�����ʲ��� --20141229
                    {
                        Port1.BPS = 0x03;
                        UARTx_BaudRate_Set(0x03, 5);//;Ĭ��2400bps                     
                    }
                   
                    DI = CtrlOrder.DI[CtrlOrder.IndSend % SBNUM];
                    if(DI == 0x0400050E) //;���¼�״̬
                    {
                        Port1.DI1DI0 = DI;
                        ZDCB_OrderSend(&Port1);
                    }
                    else
                    {
                        ZDSB_OrderSend(&Port1);     
                    }                        
                }
            }
#ifdef PORT2USART             
            else if(CtrlOrder.Port == 0x02) //;�˿�2
            {
                if(Port2.Status == idle)
                {
                    CtrlOrder.Status = 'S';
                    Port2.Status = 'E'; //;�����ͣ���Ϊ���
                    Port2.Mark = 'C';   //;�˿������
                    if(Port2.BPS != CtrlOrder.BPS) //;ͨ�����ʲ���
                    {
                        Port2.BPS = CtrlOrder.BPS;
                        UARTx_BaudRate_Set(CtrlOrder.BPS, 4);                       
                    }
                    ZDSB_OrderSend(&Port2);                             
                }
            }
#endif            
            else 
            {                      
                CtrlOrder.Status = idle;  
                Port1.Status = Sts; //;�ָ�����״̬ //;@1
                Port1.Mark = Mark;
                Port1.WaitAns = 0;
                Port1.ReSNum = 0;
                Port1.ErrNum = 0;
                CtrlOrder.ErrNum = 0x00;
                CtrlOrder.IndSend++;
            }
        }
        
        if(CtrlOrder.Status == 'S')
        {
            if((Port1.Mark != 'C') && (Port2.Mark != 'C')) //;������
            {
                CtrlOrder.Status = idle;
                return;
            }
            
            if(Port1.Mark == 'C')
            {
                if(Port1.Status == 'S')//;����״̬
                {
                    Port1.WaitAns++;
                    if (Port1.WaitAns > 12) //;3���޻�Ӧ
                    {
                        Port1.WaitAns = 0;
                        Port1.ReSNum++;
                        if (Port1.ReSNum < 2)
                        {
                            Port1.Status = 'E'; //;�ط�
                           
                        }
                        else
                        {
                            ERC21_4Judge(1);   //;�ж��Ƿ���485�������
                            Port1.ReSNum = 0;
                            Port1.Status = Sts; //;�ָ�����״̬
                            Port1.Mark = Mark;
                            CtrlOrder.Status = idle;  
                            if(CtrlOrder.DI[CtrlOrder.IndSend % SBNUM] == 0x0400050E)
                            {
                                HWareC.EventFlag = false;
                                HWareC.EventRead = false;
                            }
                            CtrlOrder.IndSend++;     
                        }
                    } 
                }    
                else if(Port1.Status == 'R')//;����״̬
                {
                    CtrlOrder.Status = idle;
                    Port1.Status = Sts; //;�ָ�����״̬
                    Port1.Mark = Mark;
                    Port1.WaitAns = 0;
                    Port1.ReSNum = 0;
                    Port1.ErrNum = 0;
                    Len = DL645_CHK(Port1.Buffer, Port1.Len);
                    if (Len != 0xFF) //;����5�յ�654֡ 
                    {
                        if((*(Port1.Buffer + 8 + Len) == 0x84)||(*(Port1.Buffer + 8 + Len) == 0x94))
                        {
                            CtrlOrder.IndSend++;
                            CtrlOrder.ErrNum = 0x00;
                        }
                        else if((*(Port1.Buffer + 8 + Len) == 0x91)) //;���¼�״̬����
                        {                            
                            for(i = 0; i < 4; i++)
                            {
                                *(Port1.Buffer + 10 + Len + i) -= 0x33; //;�������ȥ 0x33
                            }
                            DI = *((u32*)(Port1.Buffer + 10 + Len));
                            if(DI == 0x0400050E)
                            {
                                
                                HWareC.EventFlag = false;
                                HWareC.EventRead = false;
                            }
                            CtrlOrder.IndSend++;
                            CtrlOrder.ErrNum = 0x00;
                        }
                        else
                        {
                            CtrlOrder.ErrNum++;
                            if(CtrlOrder.ErrNum > 3) //;�����ط�����
                            {
                                if(CtrlOrder.DI[CtrlOrder.IndSend % SBNUM] == 0x0400050E)
                                {
                                    HWareC.EventFlag = false;
                                    HWareC.EventRead = false;
                                }
                                CtrlOrder.ErrNum = 0x00;
                                CtrlOrder.IndSend++;
                            }
                        }
                    }
                    else
                    {
                        CtrlOrder.ErrNum++;
                        if(CtrlOrder.ErrNum > 3) //;�����ط�����
                        {
                            if(CtrlOrder.DI[CtrlOrder.IndSend % SBNUM] == 0x0400050E)
                            {
                                HWareC.EventFlag = false;
                                HWareC.EventRead = false;
                            }
                            CtrlOrder.ErrNum = 0x00;
                            CtrlOrder.IndSend++;
                        }
                    }              
                    ERC21_4Judge(2);   //;�ж�485��������Ƿ�ָ�
                }              
            }     
#ifdef PORT2USART           
            if(Port2.Mark == 'C')
            {    
                if(Port2.Status == 'S')//;����״̬
                {
                    Port2.WaitAns++;
                    if (Port2.WaitAns > 12) //;3���޻�Ӧ
                    {
                        Port2.WaitAns = 0;
                        Port2.ReSNum++;
                        if (Port2.ReSNum < 2)
                        {
                            Port2.Status = 'E'; //;�ط�
                          
                        }
                        else
                        {
                            ERC21_4Judge(1);   //;�ж��Ƿ���485�������
                            Port2.ReSNum = 0;
                            Port2.Status = idle; //;������� 
                            CtrlOrder.Status = idle;                           
                            CtrlOrder.IndSend++;         
                        }
                    } 
                }
                else if(Port2.Status == 'R')//;����״̬
                {
                    CtrlOrder.Status = idle;
                    Port2.Status = idle;
                    Port2.Mark = 0;
                    Port2.WaitAns = 0;
                    Port2.ReSNum = 0;
                    Port2.ErrNum = 0;
                    Len = DL645_CHK(Port2.Buffer, Port2.Len);
                    if (Len != 0xFF) //;����5�յ�654֡ 
                    {
                        if(*(Port2.Buffer + 8 + Len) == 0x84)
                        {
                            CtrlOrder.IndSend++;
                            CtrlOrder.ErrNum = 0x00;
                        }
                        else
                        {
                            CtrlOrder.ErrNum++;
                            if(CtrlOrder.ErrNum > 3) //;�����ط�����
                            {
                                CtrlOrder.ErrNum = 0x00;
                                CtrlOrder.IndSend++;
                            }
                        }
                    }
                    else
                    {
                        CtrlOrder.ErrNum++;
                        if(CtrlOrder.ErrNum > 3) //;�����ط�����
                        {
                            CtrlOrder.ErrNum = 0x00;
                            CtrlOrder.IndSend++;
                        }
                    }              
                    ERC21_4Judge(2);   //;�ж�485��������Ƿ�ָ�
                }
            } 
#endif                     
        }
    }
    else
    {
        CtrlOrder.Status = idle;
        CtrlOrder.IndSave = CtrlOrder.IndSend = 0x00;  //;@2
    }
}

/* 
********************************************************************************
* �� �� ����ZD_EventRead
* ��    �ܣ��ն����¼���Ҫ��ȡ
* ��    ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��2012-03-23 09:05:51
********************************************************************************
*/
void ZD_EventRead(void)
{
    if(HWareC.EventFlag && (!HWareC.EventRead))
    {
        HWareC.EventRead = true;
        
        CtrlOrder.Pn[CtrlOrder.IndSave % SBNUM] = 1;      
        CtrlOrder.DI[CtrlOrder.IndSave % SBNUM] = 0x0400050E;
        CtrlOrder.IndSave++;      
    }
}

