/*
********************************************************************************
*  �� �� ����GDW376_AFN.c
*
*  �ļ�������GDW376.1Ӧ�ù��ܲ㴦����غ���  �ն˵�ַ F10
*
*  ����оƬ��
*
*  �� �� �ˣ�023 
*
*  �� �� �ţ�
*    
*  �޸ļ�¼��2012-03-26 11:15:34 ���ӿ��������F38                                                                           
********************************************************************************
*/
#include "GDW376_CFG.h"
#include "GDW376_MMU.h"

/* 
********************************************************************************
* �� �� ����Find_Fn
* ��    �ܣ�����DT1��DT2�ҳ���Ӧ��Fn
* ��    ����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
/*
u8 Find_Fn(u8 DT1, u8 DT2)
{
    u8 Fn, j;

    if(DT2 <= 30)
    {
        Fn = DT2 * 8;
    }
    else
    {
        return 0xFF; //;�Ƿ������
    }
    
    if(DT1 == 0x00)
    {
        return 0xFF; //;�Ƿ������
    }

    for(j = 0; j < 8; j++)
    {
        if(DT1 & 0x01)
        {
            if(DT1 & 0xFE)
            {
                return 0xFF; //;DT1ֻ����һλΪ1
            }

            break;
        }
        Fn++;
        DT1 >>= 1;
    }
    Fn++;
    return (Fn);
}
*/
u16 Find_Fn(u8 DT1, u8 DT2)		//;qiu20131120
{
	u16 StartFn = 0;
	u16 RetFn = 0xFFFF;	//;���ص����ݣ�[(��Ч���� << 8) + (��ʼDT)]
	u8 FnNum = 0;
	u8 i;
	
	if(DT1 == 0x00)		//;DT1���������0�����
	{
		return 0xFFFF;	//;�Ƿ������
	}
	
	for(i = 1; i < 9; i++)
	{
		if(DT1 & 0x01)	//;��Ч�ı��λ
		{
			if (StartFn == 0)	//;��δ��¼��ʼFn
			{
				if(DT2 <= 30)
				{
					StartFn = DT2 * 8;
					StartFn += i;
				}
				else
				{
					return 0xFFFF; //;�Ƿ������
				}
			}
			FnNum ++;
		}
		DT1 >>= 1;
	}
	RetFn = (FnNum << 8) | (StartFn & 0xff);
	return (RetFn);
}

/* 
********************************************************************************
* �� �� ����Find_FnI
* ��    �ܣ�����DT1�ҳ���Ӧ��Fn�����ڵ����
* ��    ����
* ��    �أ� 0xFF---�Ƿ����
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
u8 Find_FnI(u8 DT1)
{
    u8 FnI = 0, j;

    if(DT1 == 0x00)
    {
        return 0xFF; //;�Ƿ������
    }
    for(j = 0; j < 8; j++)
    {
        if(DT1 & 0x01)
        {
            if(DT1 & 0xFE)
            {
                return 0xFF; //;�Ƿ������,DT1ֻ����һλΪ1
            }
            break;
        }
        FnI++;
        DT1 >>= 1;
    }
    return (FnI);
}

/* 
********************************************************************************
* �� �� ����Find_Pn
* ��    �ܣ�����DA1��DA2�ҳ���Ӧ��Pn
* ��    ����
* ��    �أ�0xFFFF---�Ƿ����;0---�ն�;����---������ 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��2011-09-07 20:25:04
********************************************************************************
*/
u16 Find_Pn(u8 DA1, u8 DA2)
{
    u8  j;
    u16 Pn;

    if((DA1 == 0) && (DA2 == 0))  //;�ն�
    {
        return 0;
    }       
    else if((DA1==0) || (DA2==0))   //;DA1��DA2���ܵ���Ϊ0
    {
        return 0xFFFF;
    }
    
    Pn = (DA2 - 1) * 8;     
    for(j = 0; j < 8; j++)
    {
        if(DA1 & 0x01)
        {
            if(DA1 & 0xFE)
            {
                return 0xFFFF; //;DA1ֻ����һλΪ1
            }
            break;
        }
        Pn++;
        DA1 >>= 1;
    }
    Pn++;
    return (Pn);
}

/* 
********************************************************************************
* �� �� ����Find_DA
* ��    �ܣ����ݲ�����ż����DA2��DA1
* ��    ����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��2011-09-07 20:25:02
********************************************************************************
*/
void Find_DA(u16 Pn, char * DA)
{
    u8 da1 = 1, da2 = 1;
    
    if(Pn > 2040)
    {
        return;
    }
    if(Pn == 0x00)  //;�ն�
    {
        *DA = 0x00;
        *(DA + 1) = 0x00;
        return;
    }
    
    Pn -= 1;    
    da1 <<= (Pn % 8);
    da2 = (Pn / 8) + 1;
    
    *DA = da1;
    *(DA + 1) = da2;
    
}


/* 
********************************************************************************
* �� �� ����AFN04H_SET
* ��    �ܣ��������������Ҫ����Flash�����һЩRAM�����ĳ�ʼ��
* ��    ����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��2011-09-07 20:42:57
*           2011-09-29 15:20:53  �������������޸ĺ�Ĵ���
********************************************************************************
*/
void AFN04H_SET(u16 Pn, u8 Fn)
{
    s16 index;
  
    if((Fn>=1) && (Fn<=10))
    {
        EW.CSB1 = 0xC1; //;�������1���б䶯���������1��
        if(Fn == 3) //;GPRS�����ı䣬��������������
        {                                              
            GprsC.Re_Connect_states = 'E';
            //GprsC.ReConnect = true;                
        }
        else if(Fn == 8)
        {
            if((ZDP.F8.WorkMode&0x03) == 0x03)
            {
                GprsLC.CHKLink = true; //;��Ҫ����Ƿ�������ʱ��
            }
        }
    }
    else if((Fn>=10) && (Fn<=23))
    {
        EW.CSB2 = 0xC2; 
    }
    else if(((Fn>=33) && (Fn<=39)) || ((Fn>=57) && (Fn<=61)))
    {
        EW.CSB3 = 0xC3; 
	 if(33 == Fn) //;�˿ںŶ�Ϊ1 by zrt 20131022
	 {
	     ZDP.F33.PortP[0].ComPort = 1;
	     ZDP.F33.PortP[1].ComPort = 1;
	 }
        if(36 == Fn) //;��������
        {
            FluxLimValue = *((u32*)ZDP.F36);
            if(0x00 == FluxLimValue)
            {
                FluxLimValue = 0xFFFFFFFF;  //;���������ƣ�������ֵ
            } 
        }
    }
    else if(((Fn>=25) && (Fn<=31)) || ((Fn>=73) && (Fn<=76)))
    {
        /*index = ZD_Pn_Find(Pn);
        if(index < 10)
        {
            EW.CSB4 |= (0x0001 << index);          
        }
        */
        index = ZD_Pn_Find(Pn);//;--20141225
        if(index != -1)
        {
            EW.CSB4 |= (0x0001 << index);         
        }
    }
    else if(Fn == 65)
    {
        EW.CSB5 = 0xC5;
        EW.CSB9 = 0xC9;
        Task_Init(Pn, Fn); //;;��������ı��Ҫ���������������Ͳ���
    }
    else if((Fn>=66) && (Fn<=68))
    {
        EW.CSB6 = 0xC6;
        EW.CSB10 = 0xCA;
        Task_Init(Pn, Fn);
    }
    else if(((Fn>=41) && (Fn<=48)) || ((Fn>=81) && (Fn<=83)) || (Fn == 49))
    {
        EW.CSB7 = 0xC7;
    }
    else if((Fn>=89) && (Fn<=101))
    {
        EW.CSB8 = 0xC8;
    }
}

/* 
********************************************************************************
* �� �� ����AFN04H_PreData
* ��    �ܣ�ΪӦ��AFN=0x04 ������׼������
* ��    ����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
u8 AFN04H_PreData(char *S_Buf, u16 *Ind_S_Adr, char *R_Buf, u16 *Ind_R_Adr)
{
    u8 i, SET = false, temp;
    u8 Fn, FnI, DA1, DA2, DT1, DT2;
    u8 *Adr8;
    u8 T, N, L, X, N1;
    u16 Ind_S, Ind_R, Pn, Len, Len1;
    u32 *Adr32;

    Ind_R = *Ind_R_Adr; //;���뻺����λ�� 
    Ind_S = *Ind_S_Adr; //;���������λ��

    DA1 = *(R_Buf + Ind_R + 0);
    DA2 = *(R_Buf + Ind_R + 1);
    DT1 = *(R_Buf + Ind_R + 2);
    DT2 = *(R_Buf + Ind_R + 3);

    Pn = Find_Pn(DA1, DA2);     //;;�����Pn--��Ϣ��
    FnI = Find_FnI(DT1);        //;;�����Fn�����ڵ�λ��
    Fn = Find_Fn(DT1, DT2);     //;;�����Fn--��Ϣ��

    if((Fn==0xff) || (FnI==0xff) || (DT2>13))
    {
        return 0xFF; //; �ն��޷�ʶ�����Ϣ��
    }
 
    Adr8 = (u8*)ZDCSZU + Fn * 8;    //;�ҳ���Ӧ�Ĳ������õ�ַ
    T =  *Adr8;                 //;�洢����
    N = *(Adr8 + 1);            //;֧�ֵĵ�Ԫ��
    L = *(Adr8 + 2);            //;ÿ����Ԫ����
    X = *(Adr8 + 3);

    if (N == 0)   //;֧�����ò����������ݳ���
    {
        Len = L;
    }
    else
    {
        Len = (N * L) + X; //;;������ݵ�Ԫ
    }
    
    if((Len==0) || ((T!=1) && (T!=2) && (T!=3)))
    {
        return 0xFF;  //;;�ն˲�֧�ָò���
    }

    *(S_Buf + Ind_S++) = *(R_Buf + Ind_R++); //;;д���ʶ
    *(S_Buf + Ind_S++)=  *(R_Buf + Ind_R++);
    *(S_Buf + Ind_S++)=  *(R_Buf + Ind_R++);
    *(S_Buf + Ind_S++)=  *(R_Buf + Ind_R++);

    if(T == 1)      //;�ն˲���
    {
        if(N != 0) //;������ݵ�Ԫ����
        {
            N1 = *(R_Buf + Ind_R);  //;������Ҫ���õĵ�Ԫ��
            if(Fn == 2)
            {
                N1 &= 0x7F; //;F2��һ���ֽڵ����λΪ������ֹת����־
            }
            Len1 = N1 * L + X; //;��վҪ�����ò����ĳ���
        }
        else  //;�������ȹ̶�
        {
            if(Fn == 7)
            {
                Len1 = 21; //;���û�������
            }
            else
            {
                Len1 = Len; //;��վҪ�����ò����ĳ���
            }
        }
        
        Adr32 = (u32*)(Adr8 + 4);
        
        //;��վҪ�����ò����ĳ��ȴ����ն�֧�ֵĳ���,����Ϣ�㲻֧��
        if((Len1>Len) || (Pn!=0) || (*Adr32==0)) 
        {
            *(S_Buf + Ind_S++) = 1; //;����ʧ��
            *Ind_R_Adr = Ind_R + Len1;
            *Ind_S_Adr = Ind_S;
            return false;
        }
        
        Adr8 = (u8*)(*Adr32);//;ָ����Ӧ������ַ
        if(10 == Fn)    //;�����⴦�� qiu20140416
        {
            u8 F10_Result = true;
            if (((R_Buf[Ind_R + 6] & 0x7f) == 1)||((R_Buf[Ind_R + 6] & 0x7f) == 0x61))
            {
               F10_Result = true;     //;������ΪĬ��0��2400bps,�˿�Ϊ1�����óɹ�����������ʧ��--20141229
            }
            else
            {
            	F10_Result = false;     //;ͨ�ſڱ���Ϊ1,�����ʱ���Ϊ2400bps
            }
            
            if (R_Buf[Ind_R + 0] != 1)
            {
                F10_Result = false;     //;��������������Ϊ1
            }
            if (R_Buf[Ind_R + 2] != 1)
            {
                F10_Result = false;     //;��������װ����ű���Ϊ1
            }
            if (R_Buf[Ind_R + 4] != 1)
            {
                F10_Result = false;     //;������ű���Ϊ1
            }
            
            if(R_Buf[Ind_R + 7] != GYType2007)
            {
                F10_Result = false;     //;�������Լ����Ϊ07
            }
            if (false == F10_Result)
            {
                *(S_Buf + Ind_S++) = 1; //;����ʧ��
                *Ind_R_Adr = Ind_R + Len1;
                *Ind_S_Adr = Ind_S;
                return false;
            }
        }
        
        for(i=0; i<Len1; i++) //;���޸ĵĲ��������Ӧ����Ϣ��
        {
            if(false == SET) //;����Ϊ�����仯��ʱ���ȥ�ж�
            {
                if(*(Adr8+i) != *(R_Buf+Ind_R))
                {
                    SET = true; //;���������ı�
                }
            }
            *(Adr8 + i) = *(R_Buf + Ind_R++);
        }
        
        if(Fn == 7)
        {
            temp = *(Adr8 + 20) + Len1; //;�û�������
            for(i = Len1; i < temp; i++)   //;�û���
            {
                if(false == SET) //;����Ϊ�����仯��ʱ���ȥ�ж�
                {
                    if(*(Adr8+i) != *(R_Buf+Ind_R))
                    {
                        SET = true; //;���������ı�
                    }
                }
                *(Adr8 + i) = *(R_Buf + Ind_R++);
            }
/*
            temp = *(R_Buf + Ind_R++);
            *(Adr8 + 41) = temp;
            temp += 42;
            for(i = 42; i < temp; i++)
            {
                if(false == SET) //;����Ϊ�����仯��ʱ���ȥ�ж�
                {
                    if(*(Adr8 + i) != *(R_Buf+Ind_R))
                    {
                        SET = true; //;���������ı�
                    }
                }
                *(Adr8 + i) = *(R_Buf + Ind_R++);
            }
            *(Adr8 + 62) = *(R_Buf + Ind_R++); //;�ն������˿�
            *(Adr8 + 63) = *(R_Buf + Ind_R++);            
*/
//by zrt 20130618
            int pass_word_len =0;
            pass_word_len = *(Adr8 + temp) = *(R_Buf + Ind_R++);  //;���볤��
            for(i = temp+1; i < temp+1+pass_word_len; i++)
            {
                if(false == SET) //;����Ϊ�����仯��ʱ���ȥ�ж�
                {
                    if(*(Adr8 + i) != *(R_Buf+Ind_R))
                    {
                        SET = true; //;���������ı�
                    }
                }
                *(Adr8 + i) = *(R_Buf + Ind_R++);
            }
            
            *(Adr8 + temp+pass_word_len+1) = *(R_Buf + Ind_R++); //;�ն������˿�
            *(Adr8 + temp+pass_word_len+2) = *(R_Buf + Ind_R++);
            
        }
        
        *(S_Buf + Ind_S++) = 0;//;;���óɹ�
    }
    else if(T == 2) //;���������
    {
        u16 OffSet;
        u32 *Addr32;
        struct PoPStr  *PnP;

        if(N != 0)
        {
            N1 = *(R_Buf + Ind_R);
            Len1 = N1 * L + X;
        }
        else
        {
            Len1 = Len;
        }
        
        if(Pn == 0)
        {
            Pn = 1; //;��ֹ��վ���淶
        }

        Adr32 = (u32*)(Adr8 + 4);
        if((Len >Len) || (ZD_Pn_Find(Pn)<0))  //;--20141225��վҪ�����ò����ĳ��ȴ����ն�֧�ֵĳ��ȣ�����Ϣ�㲻֧��
        {  
            *(S_Buf + Ind_S++) = 1; //;;����ʧ��
            *Ind_R_Adr = Ind_R + Len1;
            *Ind_S_Adr = Ind_S;
            return false;
        }

        OffSet = *(Adr32 + 0);
        Addr32 = (u32*)(*(PnDTAddr + ZD_Pn_Find(Pn)));
        PnP = (struct PoPStr*)(*(Addr32 + _PnP));
        Adr8 = PnP->F25.UMul + OffSet;
        
        for(i=0; i<Len1; i++)
        {
            if(false == SET) //;���������ı�֮�������Ƚ�
            {
                if(*(Adr8+i) != *(R_Buf+Ind_R))
                {
                    SET = true;
                }
            }
            *(Adr8 + i) = *(R_Buf + Ind_R++);
        }
        *(S_Buf + Ind_S++) = 0;
    }
    else if(T == 3)  //;�������
    {
        if(Pn == 0)
        {
            Pn = 1;
        }
        
        if((Fn == 67) || (Fn == 68))
        {
            Len = 1; //;;��������ֹͣ���� ---@2
            Len1 = 1;
        }
        else 
        {
        		N1 = *(R_Buf + Ind_R + 8);
        		Len1 = N1 * L + 9;
        }
        
        /*if((Len1>Len) || ((Fn==65) && (Pn>TASK1NUM)) ||
            ((Fn ==66) && (Pn>TASK2NUM)))*/
        if((Len1>Len) || ((Fn==65) && (Pn>TASK1NUM)) ||
            ((Fn ==66) && (Pn>TASK2NUM))||((Fn ==67) && (Pn>TASK1NUM))\
            ||((Fn ==68) && (Pn>TASK2NUM)))    //;�����������ֻ��8��,20141230
        {
            //;;��վҪ�����ò����ĳ��ȴ����ն�֧�ֵĳ��ȣ�����Ϣ�㲻֧��
            *(S_Buf + Ind_S++) = 1; //;;����ʧ��
            *Ind_R_Adr = Ind_R + Len1;
            *Ind_S_Adr = Ind_S;
            return false;
        }

        Adr32 = (u32*)*(u32*)(Adr8 + 4);
        Adr8 = (u8*)((u8*)Adr32 + (Pn-1) * Len);
        for(i=0; i<Len1; i++)  //;;  --@2
        {
            if(false == SET)
            {
                if(*(Adr8 + i) != *(R_Buf + Ind_R))
                {
                    SET = true;
                }
            }
            *(Adr8 + i) = *(R_Buf + Ind_R++);
        }
        *(S_Buf + Ind_S++) = 0; //;;--@2
    }
 
    if(SET == true)
    {
        if(ERCP0.ERC3.Num < 8)
        {
            ERCP0.ERC3.Data[ERCP0.ERC3.Num][0] = DA1;
            ERCP0.ERC3.Data[ERCP0.ERC3.Num][1] = DA2;
            ERCP0.ERC3.Data[ERCP0.ERC3.Num][2] = DT1;
            ERCP0.ERC3.Data[ERCP0.ERC3.Num][3] = DT2;
            ERCP0.ERC3.Num++;
        }
        else
        {
            ERCP0.ERC3.Num = 0;
        }

        AFN04H_SET(Pn ,Fn);
        
    }

    *Ind_R_Adr = Ind_R;
    *Ind_S_Adr = Ind_S;
    return true;
}


/* 
********************************************************************************
* �� �� ����GDW_AFN04H
* ��    �ܣ��� AFN=0x04 ��֡���д���   ���ò���
* ��    ����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
u16 GDW_AFN04H(char *R_Buf, char *S_Buf)
{
    u16 i, len, Ind_S, Ind_R, Status;
    u8 right_num = 0;
    u8 error_num = 0;
    u8 CRC1 = true, SEQ;
    //s32 long1=0;

    SEQ = *(R_Buf + 13); //;֡������
    
    len = *(R_Buf + 1) + (*(R_Buf + 2) << 8);
    len >>= 2;   //;�û����������ܳ���
    len -= 8;    //;���ݵ�Ԫ����=len-1(������)-5(��ַ��)-1(AFN)-1(SEQ)
    len -= 16; //;PW����
    if(SEQ & 0x80) //;��ȥTpV
    {
        len -= 6;
    }
    /* //;���Ӵ˳��򣬽����������������ѭ�� 20141225
    if(SEQ & 0x80)                                   //;����ʱ���ǩ
    {
        len -= 6;                                   //;ȥ��ʱ���ǩ����
        if(*(R_Buf+19+len+16)>0)
        {
           long1=Time_distance(&TBCD.Sec, (u8*)(R_Buf + 15 + len+16));   //;tpv�����ʹ�����ʱ�ж�zrt20140107
           if (long1 < -(*(R_Buf + 19 + len+16)*60) || long1 > (*(R_Buf+19+len+16)*60) )
	   {
              return 0;
           }
        }
    }
    */ 		

    if(CRC1 == false)
    {
        return 0;  //;��֤ʧ��
    }

    Ind_S = 19;
    ERCP0.ERC3.Num = 0; //;ERC3�ĳ�ʼ���ݳ���Ϊ0����������ɺ���Ϊ6��˵��û�в����ı�
    for(Ind_R=14; Ind_R<len+14; )
    {        
        Status = AFN04H_PreData(S_Buf, &Ind_S, R_Buf, &Ind_R);
        if(Status == 0xFF) //;;�޷�ʶ����Ϣ��
        {
            break;
        }
    }

    if(ERCP0.ERC3.Num > 0)
    {
        ERCP0.ERC3.Status[0] = 0x01;      //;�в��������ı䣬ERC3����
        ERCP0.ERC3.OTime[0] = TBCD.Min;
        ERCP0.ERC3.OTime[1] = TBCD.Hour;
        ERCP0.ERC3.OTime[2] = TBCD.Day;
        ERCP0.ERC3.OTime[3] = TBCD.Month;
        ERCP0.ERC3.OTime[4] = TBCD.Year; //;д���������ʱ��
        ERCP0.ERC3.HostAdr = (*(R_Buf + 11) >> 1); //;д������վ��ַ
    }

    if(!(SEQ & 0x10)) //;CON--����Ҫȷ��
    {
        return 0;
    }
    
    for(i=23; i<=Ind_S; i+=5)
    {
        if(*(S_Buf+i) == 0x00)
        {
            right_num++;
        }
        else if(*(S_Buf+i) == 0x01)
        {
            error_num++;
        }
    }

    if(right_num == 0)  //;ȫ��������ȫ��������Ӧ��
    {
        Ind_S = 18;
        *(S_Buf + 16) = 0x02; //;AFN
    }
    else if(error_num == 0)  //;ȫ����ȷ����ȫ��ȷ����Ӧ��
    {
        Ind_S = 18;
        *(S_Buf + 16) = 0x01; //;AFN
    }
    else //;�������ݵ�Ԫ��ʶȷ�Ϻͷ���
    {
        *(S_Buf + 16) = 0x04; 
        *(S_Buf + 18) = 0x04; //;AFN
    }
    
    *(S_Buf + 0 ) = *(S_Buf + 5) = 0x68;
    *(S_Buf + 6 ) = 0x88;                         //;DIR=1,PRM=0,ACD=0; CID=8
    *(S_Buf + 7 ) = *(R_Buf + 7 );        //;�ն˵�ַ
    *(S_Buf + 8 ) = *(R_Buf + 8 );
    *(S_Buf + 9 ) = *(R_Buf + 9 );
    *(S_Buf + 10) = *(R_Buf + 10);
    *(S_Buf + 11) = *(R_Buf + 11);
    *(S_Buf + 13) = *(R_Buf + 13) & 0x0f; //;֡������SEQ
    *(S_Buf + 13) |= 0x60;                 //;Tpv=0;FIR=1,FIN=1,CON=0;
    *(S_Buf + 12) = 0x00; //;Ӧ�ò㹦���� AFN
    *(S_Buf + 14) = 0x00;
    *(S_Buf + 15) = 0x00; //;p0
    *(S_Buf + 17) = 0x00; //;f3

    if(SEQ & 0x80)//;20131102
	{
		*(S_Buf + Ind_S++) = *(R_Buf + 14 + len + 16); //;16λ����pw
                *(S_Buf + Ind_S++) = *(R_Buf + 15 + len + 16);
                *(S_Buf + Ind_S++) = *(R_Buf + 16 + len + 16);
                *(S_Buf + Ind_S++) = *(R_Buf + 17 + len + 16);
                *(S_Buf + Ind_S++) = *(R_Buf + 18 + len + 16);
		/**(S_Buf + Ind_S++) = TBCD.Sec;
		*(S_Buf + Ind_S++) = TBCD.Min;
		*(S_Buf + Ind_S++) = TBCD.Hour;
		*(S_Buf + Ind_S++) = TBCD.Day;*/
		*(S_Buf + Ind_S++) = *(R_Buf + 19 + len + 16);;
                *(S_Buf + 13) |= 0x80;
	}  
   
    //;�����򳤶�
    len = Ind_S - 6;
    len *= 4;
    len += 2;  
    *(S_Buf + 1) = *(S_Buf + 3) = (u8)len;
    *(S_Buf + 2) = *(S_Buf + 4) = (u8)(len>>8);
     
    //;У���
    *(S_Buf + Ind_S) = 0;
    for(i=6; i<Ind_S; i++)
    {
        *(S_Buf + Ind_S) += *(S_Buf + i);
    }
    
    //;�����ֽ�
    *(S_Buf + Ind_S + 1) = 0x16;
    return (Ind_S + 2);
}

/* 
********************************************************************************
* �� �� ����PreData_AFN09H
* ��    �ܣ�ΪӦ��AFN=0x09 ������׼������
* ��    ����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��2011-09-21 10:07:51
********************************************************************************
*/
u8 PreData_AFN09H(char *S_Buf, u16 *Ind_S_Adr, char *R_Buf, u16 *Ind_R_Adr)
{
    u8 i, Fn, FnI, DA1, DA2, DT1, DT2;
    u8 T, N, L, X, N1, Len;
    u8  *Adr8;
    u16 Ind_R, Ind_S, Pn;
    u32 *Adr32;

    Ind_R = *Ind_R_Adr;
    Ind_S = *Ind_S_Adr;
    
    DA1 = *(R_Buf + Ind_R++);
    DA2 = *(R_Buf + Ind_R++);
    DT1 = *(R_Buf + Ind_R++);
    DT2 = *(R_Buf + Ind_R++);

    Pn = Find_Pn(DA1, DA2); //;�����Pn--��Ϣ��
    FnI = Find_FnI(DT1); //;�����Fn--��Ϣ��
    Fn = Find_Fn(DT1, DT2); //;�����Fn--��Ϣ��

    if((Fn==0xff) || (FnI==0xff) || (DT2!=0))
    {
        *Ind_R_Adr = Ind_R ;
        *Ind_S_Adr = Ind_S;
        return false; //; �ն��޷�ʶ�����Ϣ��
    }
    
    Adr8 = ((u8*)ZDCFGZU) + Fn * 8;    //;�ҳ�������׵�ַ
    T =  *Adr8;                 //;�洢����
    N = *(Adr8 + 1);            //;֧�ֵĵ�Ԫ��
    L = *(Adr8 + 2);            //;ÿ����Ԫ����
    X = *(Adr8 + 3);


    Len = L;

    if(Len == 0)
    {
        *Ind_R_Adr = Ind_R ;
        *Ind_S_Adr = Ind_S;
        return false; //;;�ն˲�֧�ָò���
    }

    if(T == 1) //;�ն˲���
    {
        Adr32 = (u32*)(Adr8 + 4);
        if((*Adr32==0) || (Pn!=0)) //;;�ն˲�֧�ָò���
        {
            *Ind_R_Adr = Ind_R ;
            *Ind_S_Adr = Ind_S;
            return false; //;;�ն˲�֧�ָò���
        }                 
     
            
        *(S_Buf + Ind_S++) = DA1; 
        *(S_Buf + Ind_S++) = DA2; 
        *(S_Buf + Ind_S++) = DT1; 
        *(S_Buf + Ind_S++) = DT2;        
        Adr8 = (u8*)(*Adr32);
        if(N != 0)
        {
            N1 = *(Adr8 + 0);
            Len = N1 * L + X;
        }
        for(i = 0; i < Len; i++)
        {
            *(S_Buf + Ind_S++) = *(Adr8 + i);
        }
    }
    else
    {
        *Ind_R_Adr = Ind_R ;
        *Ind_S_Adr = Ind_S;
        return false; //;;�ն˲�֧�ָò���
    }
    
    *Ind_R_Adr = Ind_R;
    *Ind_S_Adr = Ind_S;
    return true;
}

/* 
********************************************************************************
* �� �� ����PreData_AFN0AH
* ��    �ܣ�ΪӦ��AFN=0x0A ������׼������
* ��    ����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
u8 PreData_AFN0AH(char *S_Buf, u16 *Ind_S_Adr, char *R_Buf, u16 *Ind_R_Adr)
{
    u8 i, Fn, FnI, DA1, DA2, DT1, DT2, temp;
    u8 x8, y8, v8, k, z8, *p;
    u8 T, N, L, X, N1;
    u8  *Adr8;
    u16 Ind_R, Ind_S, Pn, x16, y16, v16, m, n, z16, Len,j;
    u32 *Adr32;

    Ind_R = *Ind_R_Adr;
    Ind_S = *Ind_S_Adr;
    
    DA1 = *(R_Buf + Ind_R++);
    DA2 = *(R_Buf + Ind_R++);
    DT1 = *(R_Buf + Ind_R++);
    DT2 = *(R_Buf + Ind_R++);

    Pn = Find_Pn(DA1, DA2); //;�����Pn--��Ϣ��
    FnI = Find_FnI(DT1); //;�����Fn--��Ϣ��
    Fn = Find_Fn(DT1, DT2); //;�����Fn--��Ϣ��

    if((Fn==0xff) || (FnI==0xff) || (DT2>=13))
    {
        *Ind_R_Adr = Ind_R ;
        *Ind_S_Adr = Ind_S;
        return false; //; �ն��޷�ʶ�����Ϣ��
    }
    
    Adr8 = (u8*)ZDCSZU + Fn * 8;    //;�ҳ���Ӧ�Ĳ������õ�ַ
    T =  *Adr8;                 //;�洢����
    N = *(Adr8 + 1);            //;֧�ֵĵ�Ԫ��
    L = *(Adr8 + 2);            //;ÿ����Ԫ����
    X = *(Adr8 + 3);

    if(N == 0)   //;֧�����ò����������ݳ���
    {
        Len = L;
    }
    else
    {
        Len = (N * L) + X;
    }
    
    if(Len == 0)
    {
        *Ind_R_Adr = Ind_R ;
        *Ind_S_Adr = Ind_S;
        return false; //;;�ն˲�֧�ָò���
    }

    if(T == 1) //;;�ն˲���
    {
        Adr32 = (u32*)(Adr8 + 4);
        if((*Adr32==0) || (Pn!=0)) //;;�ն˲�֧�ָò���
        {
            *Ind_R_Adr = Ind_R ;
            *Ind_S_Adr = Ind_S;
            return false; //;;�ն˲�֧�ָò���
        }  
            
        if(Fn == 10)
        {            
            x16 = *(R_Buf + Ind_R) + ((*(R_Buf + Ind_R + 1)) << 8); //;���β�ѯ����
            Ind_R += 2;
            
            y16 = *(u16*)ZDP.F10.MeterNum;
            
            if(x16 > y16) //;��ѯ��������ʵ������
            {
                x16 = y16;
            }
            
          
            z16 = 0;  
            *(S_Buf + Ind_S++) = DA1; 
            *(S_Buf + Ind_S++) = DA2; 
            *(S_Buf + Ind_S++) = DT1; 
            *(S_Buf + Ind_S++) = DT2;   
            Ind_S += 2; //;ǰ�����ֽڷ�ʵ�����͵�����������
            
            for(m=0; m<x16; m++)
            {
                v16 = *(R_Buf + Ind_R) + ((*(R_Buf + Ind_R + 1)) << 8);
                Ind_R +=2;
                for(n=0; n<y16; n++) //;Ѱ�����ѯ�Ķ������
                {
                    if(*((u16*)ZDP.F10.Meter[n].Index) == v16)
                    {
                        break;
                    }
                }
                
                if(n < y16)  //;�鵽�����
                {
                    z16++;
                    Adr8 = ZDP.F10.Meter[n].Index;
                    for(i=0; i<L; i++)
                    {
                        *(S_Buf + Ind_S++) = *(Adr8 + i);
                    }
                }
            }
            
            if(z16 != 0) //;�ҵ�z16����Ҫ���͵���������
            {
                *(S_Buf + *Ind_S_Adr + 4) = (u8)z16;
                *(S_Buf + *Ind_S_Adr + 5) = (u8)(z16 >> 8);
            }
            else
            {
                Ind_S -= 6; //;�����ش����κ�����
            }
        }
        else if((Fn==11) || (Fn==13) || (Fn==14) || (Fn==38) ||
                (Fn==15) || (Fn==33) || (Fn==34) || (Fn==39))
        {                      
            z8 = 0;  
            *(S_Buf + Ind_S++) = DA1; 
            *(S_Buf + Ind_S++) = DA2; 
            *(S_Buf + Ind_S++) = DT1; 
            *(S_Buf + Ind_S++) = DT2;
            if((Fn==38) || (Fn==39))  //;��ȥ�ж��û������
            {
                *(S_Buf + Ind_S++) = *(R_Buf + Ind_R++);
            }  
            Ind_S += 1; //;ǰ�����ֽڷ�ʵ�����͵���������
             
            Adr8 = (u8*)(*Adr32); 
            x8 = *(R_Buf + Ind_R++); //;���β�ѯ����
            y8 = *Adr8;
            
            if(x8 > y8) //;��ѯ��������ʵ������
            {
                x8 = y8;
            }           
          
           
            
            for(j=0; j<x8; j++)
            {
                p = Adr8 + 1;
                v8 = *(R_Buf + Ind_R++);
                for(k=0; k<y8; k++, p += L) //;Ѱ�����ѯ�Ķ������
                {
                    if(*(p) == v8)
                    {
                        break;
                    }
                }
                
                if(k < y8)  //;�鵽�����
                {
                    z8++;
                    p = Adr8 + 1 + k * L;;
                    for(i=0; i<L; i++)
                    {
                        *(S_Buf + Ind_S++) = *(p + i);
                    }
                }
            }
            
            if(z8 != 0) //;�ҵ�z8����Ҫ���͵���������
            {
                if((Fn==38) || (Fn==39))  
                {
                    *(S_Buf + *Ind_S_Adr + 5) = z8;;
                }  
                else
                {
                    *(S_Buf+ *Ind_S_Adr + 4) = z8;
                }
            }
            else
            {
                if((Fn==38) || (Fn==39))
                {
                    Ind_S -= 6; //;�����ش����κ�����
                }
                else
                {
                    Ind_S -= 5; //;�����ش����κ�����
                }
            }
        }       
        else
        {
            
            *(S_Buf + Ind_S++) = DA1; 
            *(S_Buf + Ind_S++) = DA2; 
            *(S_Buf + Ind_S++) = DT1; 
            *(S_Buf + Ind_S++) = DT2;        
            Adr8 = (u8*)(*Adr32);
            if(N != 0)
            {
                N1 = *(Adr8 + 0);
                if(Fn == 0x02)
                {
                    N1 &= 0x7F;
                }
                Len = N1 * L + X;
            }
            
            if(Fn == 7)
            {
                Len = 21;
            }
            
            for(i = 0; i < Len; i++)
            {
                *(S_Buf + Ind_S++) = *(Adr8 + i);
            }
            
            if(Fn == 7)
            {
/*
                temp = *(Adr8 + 20) + Len; //;�û�������
                for(i = Len; i < temp; i++)
                {                
                    *(S_Buf + Ind_S++) = *(Adr8 + i);
                }
                
                temp = *(Adr8 + 41);
                temp += 41;
                for(i = 41; i < temp; i++)
                {
                    *(S_Buf + Ind_S++) = *(Adr8 + i);
                }
*/ 
//by zrt 20130717

                temp = *(Adr8 + 20) + Len;
                for(i = Len; i < temp; i++)   //;�û���
                {
                    *(S_Buf + Ind_S++) = *(Adr8 + i);
                }
                int pass_word_len = 0 ;
                pass_word_len = *(S_Buf + Ind_S++) = *(Adr8 + temp); //;���볤��
                for(i = temp + 1 ; i < temp+1+pass_word_len ; i++)
                {
                    *(S_Buf + Ind_S++) = *(Adr8 + i);
                }
                *(S_Buf + Ind_S++) = *(Adr8 + temp+pass_word_len+1); //;�ն������˿�
                *(S_Buf + Ind_S++) = *(Adr8 + temp+pass_word_len+2);;
                
            }
        }
    }
    else if(T == 2) //;;���������
    {
        u16 OffSet;
        u32 *Addr32;
        struct PoPStr  *PnP;

        Adr32 = (u32*)(Adr8 + 4);
        
        if(ZD_Pn_Find(Pn) < 0) //;;�ն˲�֧�ָò���
        {
            *Ind_R_Adr = Ind_R ;
            *Ind_S_Adr = Ind_S;
            return false; //;;�ն˲�֧�ָò���
        }
        
        *(S_Buf + Ind_S++) = DA1; 
        *(S_Buf + Ind_S++) = DA2; 
        *(S_Buf + Ind_S++) = DT1; 
        *(S_Buf + Ind_S++) = DT2; 
         
        //;�ҳ��������������λ��
        OffSet = *(Adr32 + 0);
        Addr32 = (u32*)(*(PnDTAddr + ZD_Pn_Find(Pn)));
        PnP = (struct PoPStr*)(*(Addr32 + _PnP));
        Adr8 = PnP->F25.UMul + OffSet; 
        if(N != 0)
        {
            N1 = *(Adr8 + 0);
            Len = N1 * L + X;
        }
        
        for(i = 0; i < Len ; i++)
        {
            *(S_Buf + Ind_S++) = *(Adr8 + i);
        }
    }
    else if(T == 3) //;�������
    {
        
        
        if((Adr32==0) || ((Fn==65) && (Pn>TASK1NUM)) ||
            ((Fn==66) && (Pn>TASK2NUM)) || (Pn==0x00))
        {
            *Ind_R_Adr = Ind_R;
            *Ind_S_Adr = Ind_S;
            return false;
        }
        
        *(S_Buf + Ind_S++) = DA1; 
        *(S_Buf + Ind_S++) = DA2; 
        *(S_Buf + Ind_S++) = DT1; 
        *(S_Buf + Ind_S++) = DT2; 
        
        Adr32 = (u32*)*(u32*)(Adr8 + 4);
        Adr8 = (u8*)((u8*)Adr32 + (Pn-1) * Len);
        if(N != 0)
        {
            N1 = *(Adr8 + 8);
            Len = N1 * L + X;
        }

        for(i = 0; i < Len; i++)
        {
            *(S_Buf + Ind_S++) = *(Adr8 + i);
        }
    }
    else if(T == 9)  //;��̨����(ֻ��)
    {
        u8	FM24TestBz[4];
        u8  TestMark = 0;    //;���Ա�ʶ��
        *(S_Buf + Ind_S++) = DA1; 
        *(S_Buf + Ind_S++) = DA2; 
        *(S_Buf + Ind_S++) = DT1; 
        *(S_Buf + Ind_S++) = DT2;       
        if(Fn == 53)
        {
            //;дFLASH
            j = 0;
            *(AllBuf.Spi1 + j++) = TestBiaoZhi1;
            *(AllBuf.Spi1 + j++) = TestBiaoZhi2;
            *(AllBuf.Spi1 + j++) = TestBiaoZhi3;
            *(AllBuf.Spi1 + j++) = TestBiaoZhi4;
            for(; j<527; j++)
            {
                *(AllBuf.Spi1 + j) = 0x00;
            }     
            SPI_FLASH_PageWrite_E(AllBuf.Spi1, FLASH_TEST);
            for(j=0; j<527; j++)
            {
                *(AllBuf.Spi1 + j) = 0x00;
            } 			
            //;��FLASH
            ReadFlash_Verify(FLASH_TEST, FLASH_TEST);
            if((*AllBuf.Spi1==TestBiaoZhi1) && (*(AllBuf.Spi1+1)==TestBiaoZhi2) &&
               (*(AllBuf.Spi1+2)==TestBiaoZhi3)&& (*(AllBuf.Spi1+3)==TestBiaoZhi4) )
            {
                TestMark |= 0x01; //;FLASH ��д��ȷ��D0λ��1��������0
            }	
            else
            {
            	  TestMark |= 0x00;
            }
            //;дFM24
            FM24TestBz[0] = TestBiaoZhi1;
            FM24TestBz[1] = TestBiaoZhi2;
            FM24TestBz[2] = TestBiaoZhi3;
            FM24TestBz[3] = TestBiaoZhi4;
            EW.WFM = 'W';
            WriteNByte_I2C1(4, _FM24_TEST, FM24TestBz);
            EW.WFM = 0;
            for(j=0; j<4;j++)
            {
                FM24TestBz[j] = 0;
            }
            //;��FM24
            EW.RFM = 'R';
            ReadNByte_I2C1(4, _FM24_TEST, FM24TestBz);
            EW.RFM = 0;
            if((FM24TestBz[0]==TestBiaoZhi1) && (FM24TestBz[1]==TestBiaoZhi2) && 
               (FM24TestBz[2]==TestBiaoZhi3) && (FM24TestBz[3]==TestBiaoZhi4))
            {
                TestMark |= (0x01<<1); //;FM24 ��д��ȷ��D1λ��1��������0
            }
            else
            {
            	  TestMark |= 0x00;
            }
            *(S_Buf + Ind_S++) = TestMark;
        }
        else if(Fn == 54) //;����GPRS�źŴ�С��ֵ
        {
            *(S_Buf + Ind_S++) = GprsC.Sign;
        }         
        else if(Fn == 55) //;F1�ն�����ͨ�ſ�ͨ�Ų�������FLASH�����ͣ�
        {
            EW.MoveByte = 'M';
            SW.Para[0] = 'P';
            SW.Para[1] = 'a'; //;������ն˲����Ͳ�����������в���
            
            //;��1��
            ReadFlash_Verify(CSBock1, CSBock1Bak);
            if((*AllBuf.Spi1==CSBiaoZhi1) && (*(AllBuf.Spi1+1)==CSBiaoZhi2) &&
               (*(AllBuf.Spi1+2)==CSBiaoZhi3))
            {
                MovN2Byte(CS1NUM, &ZDP.F1.RTS, AllBuf.Spi1 + 3); //;F1~F10   
                  
                MovN2Byte(6, (u8*)S_Buf + Ind_S, &ZDP.F1.RTS); //;F1
                Ind_S += 6;   
            }
            else
            {
                SW.Para[0] = 0;
                SW.Para[1] = 0;
                EW.MoveByte = 0;
                *Ind_R_Adr = Ind_R ;
                *Ind_S_Adr = Ind_S - 4;
                return false; //;;�ն˲�֧�ָò���               
            }
            SW.Para[0] = 0;
            SW.Para[1] = 0;
            EW.MoveByte = 0;
        }
        else if(Fn == 56) //;�����¼���������ֵ
        {
            //;�����¼�������EC
            ReadData_FM24(1, E2R_EC1,  E2R_EC1 + E2R_ByteNum,  (u8*) ZDS.F7);    
            ReadData_FM24(1, E2R_EC2,  E2R_EC2 + E2R_ByteNum,  (u8*) ZDS.F7+1);
            
            *(S_Buf + Ind_S++) = ZDS.F7[0];
            *(S_Buf + Ind_S++) = ZDS.F7[1];
            
        }   		
	      else
	      {
             //;���������        
             Adr8 = (u8 *)&TaskC.Switch;
             for(i = 0; i < 87; i++)
             {
                 *(S_Buf + Ind_S++) = *(Adr8 + i);
             }
             //;�澯������
             *(S_Buf + Ind_S++) = (u8)ERC1.Index;
             *(S_Buf + Ind_S++) = (u8)(ERC1.Index>>8); 
             *(S_Buf + Ind_S++) = (u8)ERC1.StInd;
             *(S_Buf + Ind_S++) = (u8)(ERC1.StInd>>8); 
             *(S_Buf + Ind_S++) = (u8)ERC2.Index;
             *(S_Buf + Ind_S++) = (u8)(ERC2.Index>>8); 
             *(S_Buf + Ind_S++) = (u8)ERC2.StInd;
             *(S_Buf + Ind_S++) = (u8)(ERC2.StInd>>8); 
             //;�ն��������
             *(S_Buf + Ind_S++) = P1DDJ.Index;
             *(S_Buf + Ind_S++) = P1DDJ.StInd;
             //;�����ն��������
             *(S_Buf + Ind_S++) = P1BDJ.Index;
             *(S_Buf + Ind_S++) = P1BDJ.StInd;
             //;�¶��������
             *(S_Buf + Ind_S++) = P1MDJ.Index;
             *(S_Buf + Ind_S++) = P1MDJ.StInd;
             //;���߶���
             *(S_Buf + Ind_S++) = (u8)P1QDJ.Index;
             *(S_Buf + Ind_S++) = (u8)(P1QDJ.Index>>8); 
             *(S_Buf + Ind_S++) = (u8)P1QDJ.StInd;
             *(S_Buf + Ind_S++) = (u8)(P1QDJ.StInd>>8); 
	      }
	 
    }
    else
    {
        *Ind_R_Adr = Ind_R ;
        *Ind_S_Adr = Ind_S;
        return false; //;;�ն˲�֧�ָò���
    }
    
    *Ind_R_Adr = Ind_R;
    *Ind_S_Adr = Ind_S;
    return true;
}


/* 
********************************************************************************
* �� �� ����GDW_AFN0AH
* ��    �ܣ���ѯ����
* ��    ����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
u16 GDW_AFN090AH(char *R_Buf, char *S_Buf, struct GuiYueCtrl *GYCtrl)
{
    u16 i, len, Ind_S, Ind_R;
    u8 SEQ;
    u8 AFN;
    //s32 long1=0;

    len = *(R_Buf + 1) + (*(R_Buf + 2) << 8);
    len >>= 2; //;�û����������ܳ���
    len -= 8; //;��·�û����ݲ���ܳ���,ȥ��������͵�ַ����ȥ��AFN �� SEQ �����ֽ��Ժ�����ݳ���
    
    SEQ = *(R_Buf + 13);
	
    if(SEQ & 0x80)                                   //;����ʱ���ǩ
    {
        len -= 6;                                   //;ȥ��ʱ���ǩ����
    }
    /*
    if(SEQ & 0x80)                                   //;����ʱ���ǩ
    {
        len -= 6;                                   //;ȥ��ʱ���ǩ����
        if(*(R_Buf+19+len)>0)
        {
           long1=Time_distance(&TBCD.Sec, (u8*)(R_Buf + 15 + len));   //;tpv�����ʹ�����ʱ�ж�zrt20140107
           if (long1 < -(*(R_Buf + 19 + len)*60) || long1 > (*(R_Buf+19+len)*60) )
	         {
              return 0;
           }
        }
    }
    */
    Ind_R = GYCtrl->Index_Answer;
    
    if(Ind_R == 0)
    {
        Ind_R = 14;
        GYCtrl->ISeq = (*(R_Buf + 13)) &0x0f;
    }
    
    Ind_S = 14;
    AFN = GYCtrl->AFNList[GYCtrl->ListNo];
    if(AFN == 0x0A)
    {
        for(; Ind_R<len+14;)
        {
            PreData_AFN0AH(S_Buf, &Ind_S, R_Buf, &Ind_R);
        }
    }
    else if(AFN == 0x09)
    {
        for(; Ind_R<len+14;)
        {
            PreData_AFN09H(S_Buf, &Ind_S, R_Buf, &Ind_R);
        }
    }
    
    *(S_Buf + 0) = *(S_Buf + 5) = 0x68;
    *(S_Buf + 6) = 0x88;      //;DIR=1,PRM=0,ACD=0; CID=8
    *(S_Buf + 7) = *(R_Buf + 7);
    *(S_Buf + 8) = *(R_Buf + 8);
    *(S_Buf + 9) = *(R_Buf + 9);
    *(S_Buf + 10) = *(R_Buf + 10);
    *(S_Buf + 11) = *(R_Buf + 11);    
    *(S_Buf + 12) = *(R_Buf + 12);
    *(S_Buf + 13) = 0x00; //;Tvp=0

//    *(S_Buf + 13) += (GYCtrl->ISeq &0x0f);
//    GYCtrl->ISeq++; //;����֡����
    
    if(GYCtrl->Index_Answer == 0x00) //;��һ֡
    {
        *(S_Buf + 13) |= 0x40;
    }
    GYCtrl->Index_Answer = Ind_R; //;��ǰ����������ĵ�i���ֽ�
    
    if(Ind_R >= len+14) //;���һ֡
    {
        *(S_Buf + 13) |= 0x20;
        GYCtrl->Index_Answer = 0; //;������Ӧ���Ѿ���ɣ���һ������Ҫ�ӿ�ʼ��Ӧ��
        GYCtrl->AFNList[0] = 0;
        GYCtrl->AFNNum = 0;
    }
    else
    {
        GYCtrl->AFNNum++;
        if(GYCtrl->AFNNum < NFRAME)  //;--@2
        {
            GYCtrl->AFNList[0] = 0x0A;
        }
        else
        {  //;��֡�������ܳ��� MaxNum_Frame ��
            *(S_Buf + 13) |= 0x20;
            GYCtrl->Index_Answer = 0;
            GYCtrl->AFNList[0] = 0;
            GYCtrl->AFNNum = 0;
        }
    }

    if(Ind_S <= 14)
    //;���û��Ҫ������ݣ�����ȷ�Ϸ���֡��ȫ������
    {
        *(S_Buf + 12) = 0x00;
        *(S_Buf + 13) = 0x60;
        Ind_S = 14;
        *(S_Buf + Ind_S++) = 0x00;
        *(S_Buf + Ind_S++) = 0x00;
        *(S_Buf + Ind_S++) = 0x02;
        *(S_Buf + Ind_S++) = 0x00;
        GYCtrl->Index_Answer = 0;
        GYCtrl->AFNList[0] = 0;
        GYCtrl->AFNNum = 0;
    }

    *(S_Buf + 13) += (GYCtrl->ISeq &0x0f);
    GYCtrl->ISeq++; //;����֡����	

    if(SEQ & 0x80)//;20131102
	{
		*(S_Buf + Ind_S++) = *(R_Buf + 14 + len);
                *(S_Buf + Ind_S++) = *(R_Buf + 15 + len);
                *(S_Buf + Ind_S++) = *(R_Buf + 16 + len);
                *(S_Buf + Ind_S++) = *(R_Buf + 17 + len);
                *(S_Buf + Ind_S++) = *(R_Buf + 18 + len);
		/**(S_Buf + Ind_S++) = TBCD.Sec;
		*(S_Buf + Ind_S++) = TBCD.Min;
		*(S_Buf + Ind_S++) = TBCD.Hour;
		*(S_Buf + Ind_S++) = TBCD.Day;*/
		*(S_Buf + Ind_S++) = *(R_Buf + 19 + len);;
                *(S_Buf + 13) |= 0x80;
	}    	

    len = Ind_S - 6;
    len <<= 2;
    len += 2;
    //;�����򳤶�
    *(S_Buf + 1) = *(S_Buf + 3) = (u8)len;
    *(S_Buf + 2) = *(S_Buf + 4) = (u8)(len >> 8);
    
    //;У���
    *(S_Buf + Ind_S) = 0;
    for(i=6; i<Ind_S; i++)
    {
        *(S_Buf + Ind_S) += *(S_Buf + i);
    }
    //;�����ֽ�
    *(S_Buf + Ind_S + 1) = 0x16;
    return (Ind_S + 2);
}



/* 
********************************************************************************
* �� �� ����GDW_AFN0CH
* ��    �ܣ�������1�����ݵ�Ӧ����
* ��    ����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��2011-09-20 20:07:13
********************************************************************************
*/
u16 GDW_AFN0CH(char *R_Buf, char *S_Buf, struct GuiYueCtrl *GYCtrl)
{
    u16 i, len, Ind_S;
    u32 Index_R_S;
    u8 SEQ;    
    //s32 long1=0;

    len = *(R_Buf + 1) + (*(R_Buf + 2) << 8);
    len >>= 2; //;�û����������ܳ���
    len -= 8; //;��·�û����ݲ���ܳ���,ȥ��������͵�ַ����ȥ��AFN �� SEQ �����ֽ��Ժ�����ݳ���
    
    SEQ = *(R_Buf + 13);
    //;ȥ��������ʱ���ǩ�ĳ���
	
    if(SEQ & 0x80)                                   //;����ʱ���ǩ
    {
        len -= 6;                                   //;ȥ��ʱ���ǩ����
    }
    /* //;�뽨�������ӳ�����ѭ��--20141225
    if(SEQ & 0x80)                                   //;����ʱ���ǩ
    {
        len -= 6;                                   //;ȥ��ʱ���ǩ����
        if(*(R_Buf+19+len)>0)
        {
           long1=Time_distance(&TBCD.Sec, (u8*)(R_Buf + 15 + len));   //;tpv�����ʹ�����ʱ�ж�zrt20140107
           if (long1 < -(*(R_Buf + 19 + len)*60) || long1 > (*(R_Buf+19+len)*60) )
	   {
              return 0;
           }
        }
    }
     */  
    i = GYCtrl->Index_Answer;
    if(i == 0)
    {
        i = 14;
        GYCtrl->ISeq = (*(R_Buf + 13)) &0x0f; //;��һ֡ʱȡ��֡���
    }

    Ind_S = 14;
    for(; i<len+14; )
    {
        Index_R_S = PreData_AFN0CH(Ind_S, i, S_Buf, R_Buf);
        if((Index_R_S&0xffff) < FRAMESIZE)
        {
            Ind_S = (u16)Index_R_S;
            i = Index_R_S >> 16;
        }
        else
        {
            break;
        }
    }
    
    *(S_Buf + 0) = *(S_Buf + 5) = 0x68;
    *(S_Buf + 6) = 0x88; //;DIR=1,PRM=0,ACD=0; CID=8
    *(S_Buf + 7) = *(R_Buf + 7);
    *(S_Buf + 8) = *(R_Buf + 8);
    *(S_Buf + 9) = *(R_Buf + 9);
    *(S_Buf + 10) = *(R_Buf + 10);
    *(S_Buf + 11) = *(R_Buf + 11);
    //;�ն˵�ַһ��
    *(S_Buf + 12) = *(R_Buf + 12);
    *(S_Buf + 13) = 0x00; //;Tpv=0 //;����ʱ���ǩ
//    *(S_Buf + 13) |= (GYCtrl->ISeq &0x0f);   
//    GYCtrl->ISeq++; //;����֡����
    
    //;��һ֡
    if(GYCtrl->Index_Answer == 0x00)   
    {
        *(S_Buf + 13) |= 0x40;
    }
    GYCtrl->Index_Answer = i; //;��ǰ����������ĵ�i���ֽ�

    if((i >= len + 14)) //;���һ֡
    {
        *(S_Buf + 13) |= 0x20;
        GYCtrl->Index_Answer = 0; //;������Ӧ���Ѿ���ɣ���һ������Ҫ�ӿ�ʼ��Ӧ��
        GYCtrl->AFNList[0] = 0;
        GYCtrl->AFNNum = 0;
    }
    else
    {
        GYCtrl->AFNNum++;
        if(GYCtrl->AFNNum < NFRAME) //;--@2
        {
             GYCtrl->AFNList[0] = 0x0C;
        }
        else
        {
            *(S_Buf + 13) |= 0x20;  //;��֡�������ܳ���MaxNum_Frame��
            GYCtrl->Index_Answer = 0;
            GYCtrl->AFNList[0] = 0;
            GYCtrl->AFNNum = 0;
        }
    }
    
    //;���û��Ҫ������ݣ�����ȷ�Ϸ���֡��ȫ������
    if(Ind_S <= 14) 
    {
        *(S_Buf + 12) = 0x00;
        *(S_Buf + 13) = 0x60;
        Ind_S = 14;
        *(S_Buf + Ind_S++) = 00;
        *(S_Buf + Ind_S++) = 00;
        *(S_Buf + Ind_S++) = 02;
        *(S_Buf + Ind_S++) = 00;
        GYCtrl->Index_Answer = 0;
        GYCtrl->AFNList[0] = 0;
        GYCtrl->AFNNum = 0;
    }

    *(S_Buf + 13) |= (GYCtrl->ISeq &0x0f);   
    GYCtrl->ISeq++; //;����֡����	

    if(SEQ & 0x80)//;20131102
	{
		*(S_Buf + Ind_S++) = *(R_Buf + 14 + len);
                *(S_Buf + Ind_S++) = *(R_Buf + 15 + len);
                *(S_Buf + Ind_S++) = *(R_Buf + 16 + len);
                *(S_Buf + Ind_S++) = *(R_Buf + 17 + len);
                *(S_Buf + Ind_S++) = *(R_Buf + 18 + len);
		/**(S_Buf + Ind_S++) = TBCD.Sec;
		*(S_Buf + Ind_S++) = TBCD.Min;
		*(S_Buf + Ind_S++) = TBCD.Hour;
		*(S_Buf + Ind_S++) = TBCD.Day;*/
		*(S_Buf + Ind_S++) = *(R_Buf + 19 + len);
                *(S_Buf + 13) |= 0x80;
	}   	
    
    len = Ind_S - 6;
    len <<= 2;
    len += 2;
    //;�����򳤶�
    *(S_Buf + 1) = *(S_Buf + 3) =(u8)len;
    *(S_Buf + 2) = *(S_Buf + 4) = (u8)(len >> 8);
    
    *(S_Buf + Ind_S) = 0; //;У���
    for(i=6; i<Ind_S; i++)
    {
        *(S_Buf + Ind_S) += *(S_Buf + i);
    }
    
    *(S_Buf + Ind_S + 1) = 0x16; //;�����ֽ�
    return (Ind_S + 2);
}

/* 
********************************************************************************
* �� �� ����GDW_AFN0CH_Up
* ��    �ܣ���һ������������������
* ��    ����
* ��    �أ� 
* ˵    ������֧�ַ�֡����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
u16 GDW_AFN0CH_Up(char *S_Buf, struct GuiYueCtrl *GYCtrl)
{
    u16 i, len, Ind_S; 
    u32 Index_R_S;   

    len = TaskP.F65[TaskC.TaskNO].PnFnNum * 4;
    i = 0;
    Ind_S = 14;
    for(; i < len;)
    {
        Index_R_S = PreData_AFN0CH(Ind_S, i, S_Buf, (char*)TaskP.F65[TaskC.TaskNO].PnFn[0]); 
        if((Index_R_S &0xffff) < FRAMESIZE)
        {
            Ind_S = (u16)Index_R_S;
            i = Index_R_S >> 16;
        }
        else
        {
            break;
        }
    }

    if(Ind_S <= 14)
    {
        return (0); //;û������
    }
    
    *(S_Buf + 0) = *(S_Buf + 5) = 0x68;
    *(S_Buf + 6) = 0xC4; //;DIR=1;PRM=1,ACD=0 CID=4

    *(S_Buf + 7) = ZDPKZ.F89.QuXian[0];
    *(S_Buf + 8) = ZDPKZ.F89.QuXian[1]; //;������
    *(S_Buf + 9) = ZDPKZ.F89.DiZhi[0];
    *(S_Buf + 10) = ZDPKZ.F89.DiZhi[1]; //;�ն˵�ַ
    *(S_Buf + 11) = 0; //;��վMSA
    *(S_Buf + 12) = 0x0C;
    *(S_Buf + 13) = (GYCtrl->ISeq) &0x0F;
    *(S_Buf + 13) |= 0x60; //;Tpv=0;FIR=1��FIN=1,CON=0
    GYCtrl->ISeq++;


    len = Ind_S - 6;
    len *= 4;
    len += 2;
    //;�����򳤶�
    *(S_Buf + 1) = *(S_Buf + 3) =(u8)len;
    *(S_Buf + 2) = *(S_Buf + 4) = (u8)(len >> 8);
    //;У���
    *(S_Buf + Ind_S) = 0;
    for(i=6; i<Ind_S; i++)
    {
        *(S_Buf + Ind_S) += *(S_Buf + i);
    }
    //;�����ֽ�
    *(S_Buf + Ind_S + 1) = 0x16;
    return (Ind_S + 2);
}

/* 
********************************************************************************
* �� �� ����AFN0DH_DJ
* ��    �ܣ��������ݵĶ�ȡ��֯����
* ��    ����
* ��    �أ�0xFF--���ݷ��ͻ��治�㣬�˳����ٲ�����һ����Ϣ�ࡣ
*           
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��2011-09-23 10:42:18 ����ֻ�е����ͻ��治��Ų��ٲ�����һ��Ϣ��
********************************************************************************
*/
u8 AFN0DH_DJ(char *S_Buf, u16 *Ind_S_Adr, char *R_Buf, u16 *Ind_R_Adr)
{
    u8 temp, readtime, ratenum;
    u8 Fn, type, num, j, multiple;
    u8 i, len, Td[7];
    u16  Buf_Add = 0;
    u16 Ind_S, Ind_R, SumLen = 0, Pn, Le;
    u32 *adr32;
    static u8 signnum[5] = {0x00, 0x07, 0x07, 0x06, 0x0B};

    Ind_S = *Ind_S_Adr;
    Ind_R = *Ind_R_Adr;
    
    Pn = Find_Pn(*(R_Buf + Ind_R + 0), *(R_Buf + Ind_R + 1));
    Fn = Find_Fn(*(R_Buf + Ind_R + 2), *(R_Buf + Ind_R + 3));
    
    adr32 = (u32*)(PN0DZU + Fn);  //;�ҳ���Ϣ��
    temp = *((u8*)adr32 + 3); //;
    type = temp & 0x07; 
    readtime = temp & 0x08;
    ratenum  = temp & 0x10;
    len  = *((u8*)adr32 + 2);
    
    //;��Ϣ������޷�ʶ��Ϣ����󣬻��ն˲�֧�ָ���Ϣ
    if((Fn==0xFF) || (Fn>218) || (type==0) || (len==0) ||
       (Pn==0xFF) || (ZD_Pn_Find(Pn) < 0))  
    {                                         //;��F13���ݵ��³���λ,---20141225
        *Ind_R_Adr = Ind_R + signnum[type]+4; //;ָ����һ����Ϣ���������
        return false;                         //;����Ϣ������ݲ�����
    }
     
    if(readtime)   //;����ʱ�䳤��
    {
       SumLen += 5;
    }
    if(ratenum)  //;����������
    {
       SumLen += 1;
    }
    
    if(type == 0x04)
    {
        num = *(R_Buf + Ind_R + 10);
        SumLen += Ind_S + 11 + len * num;
    }
    else
    {
        SumLen += (Ind_S + signnum[type] + len);
    }
    
    if(SumLen > FRAMESIZE)
    {
        return 0xFF;  //;���ͻ������ռ䲻�㣬�˳����ټ�������
    }
    
    *(S_Buf + Ind_S++) = *(R_Buf + Ind_R++);
    *(S_Buf + Ind_S++) = *(R_Buf + Ind_R++); //;Pn
    *(S_Buf + Ind_S++) = *(R_Buf + Ind_R++);
    *(S_Buf + Ind_S++) = *(R_Buf + Ind_R++); //;Fn

    if(type == 1)  //;�ն���
    {            
        Td[0] = *(R_Buf + Ind_R++);  //;��������
        Td[1] = *(R_Buf + Ind_R++);
        Td[2] = *(R_Buf + Ind_R++);
        
        Buf_Add = DDJ_Read(S_Buf, Ind_S + 3, Pn, Fn, Td); //;�������ڲ����ն�������
        *(S_Buf + Ind_S++) = Td[0];  //;д��ʵ���ҵ����ݵ�����
        *(S_Buf + Ind_S++) = Td[1];
        *(S_Buf + Ind_S++) = Td[2];        
        if(Buf_Add==0xFF)    //;Flash�л�û�����ݣ����ݲ�����
        {
            Ind_S -= 7;          
        }         
        else
        {
            Ind_S += Buf_Add;
        }  
    }
    else if(type == 2)  //; ������
    {
        Td[0] = *(R_Buf + Ind_R++);  //;��������
        Td[1] = *(R_Buf + Ind_R++);
        Td[2] = *(R_Buf + Ind_R++);

        Buf_Add = BDJ_Read(S_Buf, Ind_S + 3, Pn, Fn, Td); //;�������ڲ����ն�������
               
        *(S_Buf + Ind_S++) = Td[0];  //;д��ʵ���ҵ����ݵ�����
        *(S_Buf + Ind_S++) = Td[1];
        *(S_Buf + Ind_S++) = Td[2];
        
        //;Flash�л�û�����ݣ����ݲ�����δ�ҵ���ʱ��������
        if(Buf_Add==0xFF)
        {
            Ind_S -= 7;
        }      
        else
        {
            Ind_S += Buf_Add;
        }
    }
    else if(type == 3)  //;�¶���
    {
        Td[0] = *(R_Buf + Ind_R++);  //;��������2011-05-17 22:36:59
        Td[1] = *(R_Buf + Ind_R++);

        Buf_Add = MDJ_Read(S_Buf, Ind_S + 2, Pn, Fn, Td); //;�������ڲ����ն�������
        
        *(S_Buf + Ind_S++) = Td[0];  //;д��ʵ���ҵ����ݵ�����
        *(S_Buf + Ind_S++) = Td[1];
        //;Flash�л�û�����ݣ����ݲ�����,δ�ҵ���ʱ��������
        if(Buf_Add == 0xFF)
        {
            Ind_S -= 6;
        }        
        else
        {
            Ind_S += Buf_Add;
        }
    }
/*	
    else if(type == 4) //;��������
    {
        Td[0] = *(R_Buf + Ind_R++); //;Td_c
        Td[1] = *(R_Buf + Ind_R++);
        Td[2] = *(R_Buf + Ind_R++);
        Td[3] = *(R_Buf + Ind_R++);
        Td[4] = *(R_Buf + Ind_R++);
        Td[5] = *(R_Buf + Ind_R++);
        Td[6] = *(R_Buf + Ind_R++);
        
        *(S_Buf + Ind_S + 0) = Td[0];
        *(S_Buf + Ind_S + 1) = Td[1];
        *(S_Buf + Ind_S + 2) = Td[2];
        *(S_Buf + Ind_S + 3) = Td[3];
        *(S_Buf + Ind_S + 4) = Td[4];
        *(S_Buf + Ind_S + 5) = Td[5];
        
        Time_15Min(Td); //;����ʱ��Ϊ15���ӵı���
        
        if((Td[5] == 0) || (Td[6] == 0))  //;�����ܶȺͶ������Ϊ0
        {
            *Ind_S_Adr = Ind_S - 4;
            *Ind_R_Adr = Ind_R;
            return false;
        }
        
        num = 0;
               
        for(j=0; j<Td[6]; j++)
        {
            if(j == 0) //;�޵�һ�����ݣ��򷵻ط�֡
            {
                if(TimeCompare_5Byte(Td, &TBCD.Min) == back) //;�����˵�ǰʱ��
                {
                    *Ind_S_Adr = Ind_S - 4;
                    *Ind_R_Adr = Ind_R;
                    return false;
                }
            }
             
            Le = QDJ_Read(S_Buf, Ind_S + 7 + Buf_Add, Pn, Fn, Td);
            //;Flash�л�û������,Flash�������ݣ�����δ�ҵ���ʱ��������
            if(Le == 0xFF)
            {
                for(i=0; i<len; i++)
                {
                    *(S_Buf + Ind_S + 7 + Buf_Add + i) = 0xEE;  //;��0xEE���
                }
                Le = len;
            }
            else
            {
                num += 1; //;ʵ���ҵ��ĵ���
            }
                      
            Buf_Add += Le;
            multiple = Td[5];
            if(multiple >= 3)
            {
                multiple = 4;
            }
            Time_CRAdd(Td, 0, 15, multiple);
        }
        
        if(num == 0) //;δ�ҵ�һ������
        {
            *Ind_S_Adr = Ind_S - 4;
            *Ind_R_Adr = Ind_R;
            return false;
        }
        
        
        *(S_Buf + Ind_S + 6) = Td[6]; //;;д��ʵ���ҵ��ĵ���
        Buf_Add += 7;
        Ind_S += Buf_Add;
    }
*/
	else if(type == 4) //;��������
	{
		Td[0] = *(R_Buf + Ind_R++); //;Td_c
		Td[1] = *(R_Buf + Ind_R++);
		Td[2] = *(R_Buf + Ind_R++);
		Td[3] = *(R_Buf + Ind_R++);
		Td[4] = *(R_Buf + Ind_R++);
		Td[5] = *(R_Buf + Ind_R++);
		Td[6] = *(R_Buf + Ind_R++);
		if(Td[0] != 0)
		{
			Time_15Min(Td); //;����ʱ��Ϊ15���ӵı���
			Time_CRAdd(Td, 0, 15, 1);
		}
		
		*(S_Buf + Ind_S + 0) = Td[0];
		*(S_Buf + Ind_S + 1) = Td[1];
		*(S_Buf + Ind_S + 2) = Td[2];
		*(S_Buf + Ind_S + 3) = Td[3];
		*(S_Buf + Ind_S + 4) = Td[4];
		*(S_Buf + Ind_S + 5) = Td[5];
		
		
		
		if((Td[5] == 0) || (Td[6] == 0))  //;�����ܶȺͶ������Ϊ0
		{
			*Ind_S_Adr = Ind_S - 4;
			*Ind_R_Adr = Ind_R;
			return false;
		}
		
		num = 0;
		       
		for(j=0; j<Td[6]; j++)
		{
//			if(j == 0) //;�޵�һ�����ݣ��򷵻ط�֡
//			{
//				if(TimeCompare_5Byte(Td, &TBCD.Min) == back) //;�����˵�ǰʱ��
//				{
//					*Ind_S_Adr = Ind_S - 4;
//					*Ind_R_Adr = Ind_R;
//					return false;
//				}
//			}
			if(TimeCompare_5Byte(Td, &TBCD.Min) == back) //;�����˵�ǰʱ�� qiu_20130516
			{
				break;
			}
			Le = QDJ_Read(S_Buf, Ind_S + 7 + Buf_Add, Pn, Fn, Td);
			//;Flash�л�û������,Flash�������ݣ�����δ�ҵ���ʱ��������
			if(Le == 0xFF)
			{
				for(i=0; i<len; i++)
				{
					*(S_Buf + Ind_S + 7 + Buf_Add + i) = 0xEE;  //;��0xEE���
				}
				Le = len;
			}
			else
			{
				num += 1; //;ʵ���ҵ��ĵ���
			}
			          
			Buf_Add += Le;
			multiple = Td[5];
			if(multiple >= 3)
			{
				multiple = 4;
			}
			Time_CRAdd(Td, 0, 15, multiple);
		}
		
		if(num == 0) //;δ�ҵ�һ������
		{
			*Ind_S_Adr = Ind_S - 4;
			*Ind_R_Adr = Ind_R;
			return false;
		}
//		*(S_Buf + Ind_S + 6) = Td[6]; //;;д��ʵ���ҵ��ĵ���
		*(S_Buf + Ind_S + 6) = num; //;;д��ʵ���ҵ��ĵ��� qiu_20130516
		Buf_Add += 7;
		Ind_S += Buf_Add;
	}

    *Ind_S_Adr = Ind_S;
    *Ind_R_Adr = Ind_R; //;ָ����һ����Ϣ���������
    return true; //;����Ϣ������ݴ���
}

/* 
********************************************************************************
* �� �� ����GDW_AFN0DH_Up
* ��    �ܣ��ڶ���������������
* ��    ����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
/*
u16 GDW_AFN0DH_Up(char *S_Buf, struct GuiYueCtrl *GYCtrl)
{
    u8  Pn, Fn;
    u16 i, len, Ind_S, Buf_Add = 0;

    *(S_Buf + 0 ) = *(S_Buf + 5) = 0x68;
    *(S_Buf + 6 ) = 0xC4;                              //;DIR=1;PRM=1,ACD=0 CID=4
    *(S_Buf + 7 ) = ZDPKZ.F89.QuXian[0];
    *(S_Buf + 8 ) = ZDPKZ.F89.QuXian[1];               //;������
    *(S_Buf + 9 ) = ZDPKZ.F89.DiZhi[0];
    *(S_Buf + 10) = ZDPKZ.F89.DiZhi[1];           //;�ն˵�ַ
    *(S_Buf + 11) = 0;                                 //;��վMSA
    *(S_Buf + 12) = 0x0d;
    *(S_Buf + 13) = (GYCtrl->ISeq) & 0x0F;
    *(S_Buf + 13) |= 0x60;                             //; Tpv=0; FIR=1;FIN=1,CON=0
    GYCtrl->ISeq++;
    Ind_S = 14;

    for(i=0; i<TaskP.F66[TaskC.TaskNO].PnFnNum; i++)
    {
        Pn = Find_Pn(TaskP.F66[TaskC.TaskNO].PnFn[i][0], TaskP.F66[TaskC.TaskNO].PnFn[i][1]);
        Fn = Find_Fn(TaskP.F66[TaskC.TaskNO].PnFn[i][2], TaskP.F66[TaskC.TaskNO].PnFn[i][3]);
        Buf_Add = T2Task_FnPn(S_Buf, Ind_S + 4, Pn, Fn);
        if (Buf_Add != 0)
        {
            *(S_Buf + Ind_S++) = TaskP.F66[TaskC.TaskNO].PnFn[i][0];
            *(S_Buf + Ind_S++) = TaskP.F66[TaskC.TaskNO].PnFn[i][1];
            *(S_Buf + Ind_S++) = TaskP.F66[TaskC.TaskNO].PnFn[i][2];
            *(S_Buf + Ind_S++) = TaskP.F66[TaskC.TaskNO].PnFn[i][3];
            Ind_S += Buf_Add;
            if(Ind_S >= 500)
            {
                break;
            }
        }
    }

    if(Ind_S <= 14)
    {
        return (0); //;û������
    }

    len = Ind_S - 6;
    len *= 4; //;������λ
    len += 2; //;����Լʹ��
    //;�����򳤶�
    *(S_Buf + 1) = *(S_Buf + 3) =(u8)len;
    *(S_Buf + 2) = *(S_Buf + 4) = (u8)(len >> 8);
    //;У���
    *(S_Buf + Ind_S) = 0;
    for (i = 6; i < Ind_S; i++)
    {
        *(S_Buf + Ind_S) += *(S_Buf + i);
    }
    //;�����ֽ�
    *(S_Buf + Ind_S + 1) = 0x16;
    return (Ind_S + 2);
}
*/
u16 GDW_AFN0DH_Up(char *S_Buf, struct GuiYueCtrl *GYCtrl) 
{
	u8  Pn, Fn, FnNum, temp;
	u16  FnInfo;			//;qiu20131120
	u16 i, j, len, Ind_S, Buf_Add = 0;

	*(S_Buf + 0 ) = *(S_Buf + 5) = 0x68;
	*(S_Buf + 6 ) = 0xC4;                              //;DIR=1;PRM=1,ACD=0 CID=4
  *(S_Buf + 7 ) = ZDPKZ.F89.QuXian[0];
  *(S_Buf + 8 ) = ZDPKZ.F89.QuXian[1];               //;������
  *(S_Buf + 9 ) = ZDPKZ.F89.DiZhi[0];
  *(S_Buf + 10) = ZDPKZ.F89.DiZhi[1];           //;�ն˵�ַ
	*(S_Buf + 11) = 0;                                 //;��վMSA
	*(S_Buf + 12) = 0x0d;
	*(S_Buf + 13) = (GYCtrl->ISeq) & 0x0F;
	*(S_Buf + 13) |= 0x60;                             //; Tpv=0; FIR=1;FIN=1,CON=0
	if (ZDP.F1.CON != 0)			//;qiu20131119	
	{
		*(S_Buf + 13) |= 0x10;		//;��Ҫ��վӦ��
	}
	GYCtrl->ISeq++;
	Ind_S = 14;
	
	for(i=0; i<TaskP.F66[TaskC.TaskNO].PnFnNum; i++)
	{
		Pn = Find_Pn(TaskP.F66[TaskC.TaskNO].PnFn[i][0], TaskP.F66[TaskC.TaskNO].PnFn[i][1]);
		FnInfo = Find_Fn(TaskP.F66[TaskC.TaskNO].PnFn[i][2], TaskP.F66[TaskC.TaskNO].PnFn[i][3]);
		
		Fn = (FnInfo & 0xff) - 1;	//;��ʼFn - 1
		FnNum = (FnInfo >> 8) & 0xff;	//;���ֽ�ΪFn����
		for (j = 0; j <8; j++)
		{
			Fn++;
			temp =  (Fn - 1) % 8;		//;��Ҫ������
			if (TaskP.F66[TaskC.TaskNO].PnFn[i][2] & (1 << temp))		//;��ǰ����Fn��Ҫ����
			{
				Buf_Add = T2Task_FnPn(S_Buf, Ind_S + 4, Pn, Fn);
				if (Buf_Add != 0)
				{
					*(S_Buf + Ind_S++) = TaskP.F66[TaskC.TaskNO].PnFn[i][0];
					*(S_Buf + Ind_S++) = TaskP.F66[TaskC.TaskNO].PnFn[i][1];
					*(S_Buf + Ind_S++) = 1 << ((Fn-1) % 8);
					*(S_Buf + Ind_S++) = (Fn - 1) / 8;
					Ind_S += Buf_Add;
					if(Ind_S >= 500)
					{
						break;
					}
				}
				FnNum--;
				if (FnNum == 0)
				{
					break;
				}
			}			
		}
	}
	
	if(Ind_S <= 14)
	{
		return (0); //;û������
	}
		
	len = Ind_S - 6;
	len *= 4; //;������λ
	len += 2; //;����Լʹ��
	//;�����򳤶�
	*(S_Buf + 1) = *(S_Buf + 3) = (u8)len;
	*(S_Buf + 2) = *(S_Buf + 4) = (u8)(len >> 8);
	//;У���
	*(S_Buf + Ind_S) = 0;
	for (i = 6; i < Ind_S; i++)
	{
		*(S_Buf + Ind_S) += *(S_Buf + i);
	}
	//;�����ֽ�
	*(S_Buf + Ind_S + 1) = 0x16;
	return (Ind_S + 2);
}


/* 
********************************************************************************
* �� �� ����GDW_AFN0DH
* ��    �ܣ�
* ��    ����
* ��    �أ� 
* ˵    ������ѯ��ʷʱ,���ʱ�䳬���洢��Χ,�ն�Ӧ�ӷ������һ����ʼ����,
*           ��Ӧ���ط���֡��
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
u16 GDW_AFN0DH(char *R_Buf, char *S_Buf, struct GuiYueCtrl *GYCtrl)
{
    u8  Status, SEQ;
    u16 i, len, Ind_S, Ind_R;
    //s32 long1=0;


    len = *(R_Buf + 1) + (*(R_Buf + 2) << 8);
    len >>= 2;  //;�û����������ܳ���
    len -= 8;   //;��·�û����ݲ���ܳ���
    SEQ = *(R_Buf + 13);
	
    if(SEQ & 0x80)                                   //;����ʱ���ǩ
    {
        len -= 6;                                   //;ȥ��ʱ���ǩ����
    }
    /* //;�����������������֡������ѭ�� --20141225
    if(SEQ & 0x80)                                   //;����ʱ���ǩ
    {
        len -= 6;                                   //;ȥ��ʱ���ǩ����
        if(*(R_Buf+19+len)>0)
        {
           long1=Time_distance(&TBCD.Sec, (u8*)(R_Buf + 15 + len));   //;tpv�����ʹ�����ʱ�ж�zrt20140107
           if (long1 < -(*(R_Buf + 19 + len)*60) || long1 > (*(R_Buf+19+len)*60) )
	   {
              return 0;
           }
        }
    }
    */

    Ind_R = GYCtrl->Index_Answer;
    if(Ind_R == 0)
    {
        Ind_R = 14;
        GYCtrl->ISeq = (*(R_Buf + 13)) & 0x0f;
    }
    Ind_S = 14;

    for( ; Ind_R<len+14; )
    {
        Status = AFN0DH_DJ(S_Buf, &Ind_S, R_Buf, &Ind_R);
        if(Status == 0xFF) //;;�洢���ݲ����ڣ����ط�֡
        {
            break;
        }
        else if(Status == 0xFE) //;; ����ռ䲻�㣬��Ҫ��֡
        {
            break;
        }
        else if(Status == 0xFD) //;; ��Ϣ���޷�ʶ���޷���������
        {
            break;
        }
    }

    *(S_Buf + 0 ) = 0x68;
    *(S_Buf + 5 ) = 0x68;
    *(S_Buf + 6 ) = 0x88;
    *(S_Buf + 7 ) = *(R_Buf + 7 );
    *(S_Buf + 8 ) = *(R_Buf + 8 );
    *(S_Buf + 9 ) = *(R_Buf + 9 );
    *(S_Buf + 10) = *(R_Buf + 10);
    *(S_Buf + 11) = *(R_Buf + 11);
    *(S_Buf + 12) = *(R_Buf + 12);
    *(S_Buf + 13) = 0x00;

//    *(S_Buf + 13) |= (GYCtrl->ISeq &0x0f);
//    GYCtrl->ISeq++; //;����֡����

    if(GYCtrl->Index_Answer == 0x00)   //;��һ֡
    {
        *(S_Buf + 13) |= 0x40;
    }
    GYCtrl->Index_Answer = Ind_R;      //;��ǰ����������ĵ�i���ֽ�

    if((Ind_R >= len + 14))  //;���һ֡
    {
        *(S_Buf + 13) |= 0x20;
        GYCtrl->Index_Answer = 0; //;������Ӧ���Ѿ���ɣ���һ������Ҫ�ӿ�ʼ��Ӧ��
        GYCtrl->AFNList[0] = 0;
        GYCtrl->AFNNum = 0;
    }
    else
    {
        GYCtrl->AFNNum++;
        if(GYCtrl->AFNNum < NFRAME) //;��֡�������ܳ���MaxNum_Frame��
        {
            GYCtrl->AFNList[0] = 0x0D;
        }
        else
        {
            *(S_Buf + 13) |= 0x20;
            GYCtrl->Index_Answer = 0;
            GYCtrl->AFNList[0] = 0;
            GYCtrl->AFNNum = 0;
        }
    }

    if(Ind_S <= 14)   //;���û��Ҫ������ݣ�����ȷ�Ϸ���֡��ȫ������
    {
        *(S_Buf + 12) = 0x00;
        *(S_Buf + 13) = 0x60;
        *(S_Buf + 13) |= (SEQ &0x0F);	//qiu20131112
        Ind_S = 14;
        *(S_Buf + Ind_S++) = 00;
        *(S_Buf + Ind_S++) = 00;
        *(S_Buf + Ind_S++) = 02;
        *(S_Buf + Ind_S++) = 00;
        GYCtrl->Index_Answer = 0; //;������Ӧ���Ѿ���ɣ���һ������Ҫ�ӿ�ʼ��Ӧ��
        GYCtrl->AFNList[0] = 0;
        GYCtrl->AFNNum = 0;
    }

    *(S_Buf + 13) |= (GYCtrl->ISeq &0x0f);
    GYCtrl->ISeq++; //;����֡����	

    if(SEQ & 0x80)//;20131102
	  {
	  	*(S_Buf + Ind_S++) = *(R_Buf + 14 + len);
      *(S_Buf + Ind_S++) = *(R_Buf + 15 + len);
      *(S_Buf + Ind_S++) = *(R_Buf + 16 + len);
      *(S_Buf + Ind_S++) = *(R_Buf + 17 + len);
      *(S_Buf + Ind_S++) = *(R_Buf + 18 + len);
	  	/**(S_Buf + Ind_S++) = TBCD.Sec;
	  	*(S_Buf + Ind_S++) = TBCD.Min;
	  	*(S_Buf + Ind_S++) = TBCD.Hour;
	  	*(S_Buf + Ind_S++) = TBCD.Day;*/
	  	*(S_Buf + Ind_S++) = *(R_Buf + 19 + len);;
      *(S_Buf + 13) |= 0x80;
	  }

    len = Ind_S - 6;
    len *= 4;
    len += 2;      //;�����򳤶�

    *(S_Buf + 1) = *(S_Buf + 3) = (u8)len;
    *(S_Buf + 2) = *(S_Buf + 4) = (u8)(len >> 8);

    *(S_Buf + Ind_S) = 0;//;У���
    for(i=6; i<Ind_S; i++)
    {
        *(S_Buf + Ind_S) += *(S_Buf + i);
    }
    *(S_Buf + Ind_S + 1) = 0x16;//;�����ֽ�
    return (Ind_S + 2);
}





/*--------------------------------------------------------------------------
  ������GDW_AFN0EH
  ���ܣ��� AFN=0x0E ��֡���д��� ������������
  ������
 ---------------------------------------------------------------------------*/
u16 GDW_AFN0EH(char *R_Buf, char *S_Buf, struct GuiYueCtrl *GYCtrl)
{
    u8 SEQ;
    u16 i, len, Ind_S;
    u32 Index_R_S;
    //s32 long1=0;
	
    len = *(R_Buf + 1) + *(R_Buf + 2) *256;
    len >>= 2; //;�û����������ܳ���
    len -= 8; //;��·�û����ݲ���ܳ���,ȥ��������͵�ַ����ȥ��AFN �� SEQ �����ֽ��Ժ�����ݳ���

    SEQ = *(R_Buf + 13);
	
    if(SEQ & 0x80)                                   //;����ʱ���ǩ
    {
        len -= 6;                                   //;ȥ��ʱ���ǩ����
    }
    /*   //;����������������� ������ѭ�� --20141225
    if(SEQ & 0x80)                                   //;����ʱ���ǩ
    {
        len -= 6;                                   //;ȥ��ʱ���ǩ����
        if(*(R_Buf+19+len)>0)
        {
           long1=Time_distance(&TBCD.Sec, (u8*)(R_Buf + 15 + len));   //;tpv�����ʹ�����ʱ�ж�zrt20140107
           if (long1 < -(*(R_Buf + 19 + len)*60) || long1 > (*(R_Buf+19+len)*60) )
	         {
              return 0;
           }
        }
    }
     */
    *(S_Buf + 0) = *(S_Buf + 5) = 0x68;
    *(S_Buf + 6) = 0x88; //;DIR=1,PRM=0,ACD=0; CID=8
    *(S_Buf + 7) = *(R_Buf + 7);
    *(S_Buf + 8) = *(R_Buf + 8);
    *(S_Buf + 9) = *(R_Buf + 9);
    *(S_Buf + 10) = *(R_Buf + 10);
    *(S_Buf + 11) = *(R_Buf + 11); //;�ն˵�ַҲ��һ����
    *(S_Buf + 12) = *(R_Buf + 12); //;AFNһ��
    *(S_Buf + 13) = 0x00; //;Tvp=0
    Ind_S = i = 14;
    Index_R_S = PreData_AFN0EH(Ind_S, i, S_Buf, R_Buf, GYCtrl);
    Ind_S = (u16)Index_R_S;

    if(GYCtrl->ERC_F == 1)
    //;��һ֡
    {
        GYCtrl->ISeq = (*(R_Buf + 13)) &0x0f; //;�ݴ�֡���
        *(S_Buf + 13) |= 0x40;
    }
    if (GYCtrl->ERC_L == 1)
    //;���һ֡
    {
        *(S_Buf + 13) |= 0x20;
        GYCtrl->ERC_F = 0;
        GYCtrl->ERC_L = 0;
        GYCtrl->AFNList[0] = 0;
        GYCtrl->AFNNum = 0;
    }
    else
    {
        GYCtrl->AFNNum++;
        if(GYCtrl->AFNNum < NFRAME)  //;--@2
        {
           GYCtrl->AFNList[0] = 0x0E; //;�¼���֡����
        }
        else
        {  //;��֡�������ܳ���MaxNum_Frame��
            *(S_Buf + 13) |= 0x20;
            GYCtrl->ERC_Index = 0;
            GYCtrl->ERC_F = 0;
            GYCtrl->ERC_L = 0;
            GYCtrl->AFNList[0] =  0;
            GYCtrl->AFNNum = 0;
        }
    }

    if (Ind_S <= 14)
    //;���û��Ҫ������ݣ�����ȷ�Ϸ���֡��ȫ������
    {
        *(S_Buf + 12) = 0x00;
        Ind_S = 14;
        *(S_Buf + Ind_S++) = 00;
        *(S_Buf + Ind_S++) = 00;
        *(S_Buf + Ind_S++) = 02;
        *(S_Buf + Ind_S++) = 00;
        *(S_Buf + 13) = 0x60;
        GYCtrl->ERC_F = 0;
        GYCtrl->ERC_L = 0;
        GYCtrl->AFNList[0] = 0;
        GYCtrl->AFNNum = 0;
    }

    *(S_Buf + 13) |= (GYCtrl->ISeq &0x0f);
    GYCtrl->ISeq++; //;����֡����

    if(SEQ & 0x80)//;20131102
	{
		*(S_Buf + Ind_S++) = *(R_Buf + 14 + len);
                *(S_Buf + Ind_S++) = *(R_Buf + 15 + len);
                *(S_Buf + Ind_S++) = *(R_Buf + 16 + len);
                *(S_Buf + Ind_S++) = *(R_Buf + 17 + len);
                *(S_Buf + Ind_S++) = *(R_Buf + 18 + len);
		/**(S_Buf + Ind_S++) = TBCD.Sec;
		*(S_Buf + Ind_S++) = TBCD.Min;
		*(S_Buf + Ind_S++) = TBCD.Hour;
		*(S_Buf + Ind_S++) = TBCD.Day;*/
		*(S_Buf + Ind_S++) = *(R_Buf + 19 + len);;
                *(S_Buf + 13) |= 0x80;
	}      
	
    len = Ind_S - 6;
    len *= 4;
    len += 2;
    //;------�����򳤶�
    *(S_Buf + 1) = *(S_Buf + 3) =(u8)len;
    *(S_Buf + 2) = *(S_Buf + 4) = (u8)(len >> 8);
    //;------У���
    *(S_Buf + Ind_S) = 0;
    for (i = 6; i < Ind_S; i++)
    {
        *(S_Buf + Ind_S) += *(S_Buf + i);
    }
    //;------�����ֽ�
    *(S_Buf + Ind_S + 1) = 0x16;
    return (Ind_S + 2);
}


/* 
********************************************************************************
* �� �� ����GDW_AFN0EH_Up
* ��    �ܣ������ϱ���Ҫ�¼�
* ��    ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
u16 GDW_AFN0EH_Up(char *S_Buf, struct GuiYueCtrl *GYCtrl)
{

    u16 index_out, i, len, R_Index, Page, Area_ind, Len;
		u8 Pm, Y, k;
		static u8 ISeq;
		
    *(S_Buf + 0 ) = *(S_Buf + 5) = 0x68;
    *(S_Buf + 6 ) = 0xC4;                             //;DIR=1;PRM=1,ACD=0 CID=0
    *(S_Buf + 7 ) = ZDPKZ.F89.QuXian[0];
    *(S_Buf + 8 ) = ZDPKZ.F89.QuXian[1];              //;������
    *(S_Buf + 9 ) = ZDPKZ.F89.DiZhi[0];
    *(S_Buf + 10) = ZDPKZ.F89.DiZhi[1];          //;�ն˵�ַ
    *(S_Buf + 11) = 0;                                //;��վMSA
    *(S_Buf + 12) = 0x0E;                             //;AFN����3������ 
    
    if(!ERCUp.ReSedFlag)
    {
        ISeq =  GYCtrl->ISeq;
    }
    *(S_Buf + 13) = ISeq & 0x0F;
    *(S_Buf + 13) |= 0x60;                            //;  Tpv=0;   FIR=1,FIN=1(��֡);
    
    
    GYCtrl->ISeq++;
    Pm = ERCUp.UpIndex;
    if(ZDP.F1.CON & 0x04)                              //;��������ϱ���Ҫ�¼���Ҫȷ��
    {
        *(S_Buf + 13) |= 0x10;
    }
    *(S_Buf + 14) = 0x00;                             //;P0
    *(S_Buf + 15) = 0x00;
    *(S_Buf + 16) = 0x01;                             //;F1��Ҫ�¼�
    *(S_Buf + 17) = 0x00;
    *(S_Buf + 18) = ZDS.F7[0];
    *(S_Buf + 19) = ZDS.F7[1];
    *(S_Buf + 20) = Pm;                             //;Pm=0;
    
    
    index_out = 22;
    if(Pm < ZDS.F7[0])
    {
        Y = ZDS.F7[0] - Pm; //;������¼�����
    }
    else
    {
        Y = ERCReadNum + ZDS.F7[0] - Pm;
    }
    
    for(i = 0; i < Y; i++)
    {
       /*----�����ڱ����264���¼��ж�ȡ���µ�256��----*/
        if(ZDS.F7[0] == 0)
        //;��ǰ��256��0�������¼�����
        {
             if(ERC1.Index == 0)
             {
                ERCUp.SendERCNum = ZDS.F7[0];
                return 0; //;���¼�����
             }
             R_Index = (ERC1.StInd + Pm) % ERCPointNum; //;��StIndΪ��׼���Զ�ȡ0~255�������¼�
        }
        else
        //;��ǰ���·������¼�С��256��
        {
            if(Pm < ZDS.F7[0])
            //;��ǰ��Ҫ��ȡ���¼����С�������¼�����
            {
                R_Index = (ERC1.Index - ZDS.F7[0] + Pm) % ERCPointNum;  //;��Index����ECNumΪ��׼��ȡ���¼�
            }
            else
            //;�� StInd Ϊ��׼��ȡ���¼�
            {
                if((ERC1.Index - ERC1.StInd) >= ERCReadNum )
                {
                    R_Index = (ERC1.StInd + ERC1.Index % ERCPageNum + Pm - ZDS.F7[0]) % ERCPointNum ;
                }
                else
                {
                    ERCUp.SendERCNum = ZDS.F7[0];
                    return 0; //;���¼�����//;��û�п�ʼ���ǣ����¼�������
                }
            }
        }
      /*------------------------------------------------*/

        Page = ERC1Home + R_Index / ERCPagePoint; //;�¼���Flash�еĵڼ�ҳ
        
        Area_ind = R_Index % ERCPagePoint; //;��ҳ�ĵڼ�����
        R_Index = Area_ind * ERCAreaNum; //;��ҳ�ľ���λ��
        SPI_FLASH_PageRead(AllBuf.Spi1, Page); //;��������������ҳ����ҳ����
        Len = *(AllBuf.Spi1 + R_Index + 1);
        if (Len < ERCAreaNum) //;�¼���¼���ݳ��Ȳ��ᳬ�� 66 ���ֽ�     
        {
            if(index_out + Len + 2 < FRAMESIZE)
            //;д����һ���¼����������֡
            {
                *(S_Buf + index_out++) = *(AllBuf.Spi1 + R_Index + 0); //;д���¼�����
                *(S_Buf + index_out++) = Len; //;д���¼�����
                for (k = 0; k < Len; k++)
                {
                    *(S_Buf + index_out++) = *(AllBuf.Spi1 + R_Index + 2 + k); //;��������
                }
                Pm++;
                Pm %= ERCPointNum;             
            }
           
        }
        else
        {
            ERCUp.SendERCNum = ZDS.F7[0]; //;���һ֡
            return 0; //;���¼�����; //;�¼�������
        }
    }
    
    *(S_Buf + 21) = Pm;                       //;Pn=1;
    ERCUp.SendERCNum = (Pm % ERCPointNum);
    len = index_out - 6;
    len *= 4;
    len += 2;
    *(S_Buf + 1) = *(S_Buf + 3) =(u8)len;
    *(S_Buf + 2) = *(S_Buf + 4) = (u8)(len >> 8);
    *(S_Buf + index_out) = 0;

    for (i = 6; i < index_out; i++)
    {
        *(S_Buf + index_out) += *(S_Buf + i);       //;У���
    }

    *(S_Buf + index_out + 1) = 0x16;                  //;�����ֽ�
    return (index_out + 2);
}


/* 
********************************************************************************
* �� �� ����GDW_AFN0FH
* ��    �ܣ��ļ����䴦��
* ��    ����
* ��    �أ�ʧ�ܷ��ط�֡��
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��2012-05-02 15:31:26
********************************************************************************
*/
u16 GDW_AFN0FH(char *R_Buf, char *S_Buf, struct GuiYueCtrl *GYCtrl)
{
    u16 i, len, frameind, size;
    u16 crc, crctemp,Pn;
    u8 SEQ, filesign, DA1, DA2, DT1, DT2, Fn;
    u32 filesize;
        
    len = *(R_Buf + 1) + (*(R_Buf + 2) << 8);
    len >>= 2;   //;�û����������ܳ���
    len -= 8;    //;���ݵ�Ԫ����=len-1(������)-5(��ַ��)-1(AFN)-1(SEQ)
    len -= 16; //;PW����
    
    SEQ = *(R_Buf + 13); //;֡������
    if(SEQ & 0x80) //;��ȥTpV
    {
        len -= 6;
    } 
    
    DA1 = *(R_Buf + 14);
    DA2 = *(R_Buf + 15);
    DT1 = *(R_Buf + 16);
    DT2 = *(R_Buf + 17);

    Pn = Find_Pn(DA1, DA2);     //;;�����Pn--��Ϣ��
    Fn = Find_Fn(DT1, DT2);     //;;�����Fn--��Ϣ��

    if((Pn != 0x00) || (Fn != 0x02)) //;ֻ֧���Զ����ļ�����
    {
        goto ErrDeal; //; �ն��޷�ʶ�����Ϣ��
    }
    
    filesign = *(R_Buf + 18); //;�ļ���ʶ
    
    if(*(R_Buf + 19) != 'B')
    {
        goto ErrDeal;
    }
    
    filesize =  *(R_Buf + 20) << 24; //;����
    filesize += *(R_Buf + 21) << 16;
    filesize += *(R_Buf + 22) << 8;
    filesize += *(R_Buf + 23);
    if(filesize > PROG_FLASH_SIZE)
    {
        goto ErrDeal;
    }
    
    frameind =  *(R_Buf + 24) << 8; //;�ļ����
    frameind += *(R_Buf + 25);   
    i = *(R_Buf + 26) << 8;
    i += *(R_Buf + 27);
    
    i = ~i;
    if(frameind != i)
    {
        goto ErrDeal;
    }
    
    
     //;��֡����
    size =  *(R_Buf + 28) << 8;
    size += *(R_Buf + 29);
    if(size > 512)
    {
        goto ErrDeal;
    }
    
    if(SEQ & 0x40) //;��֡
    {
        GYCtrl->FileSign = filesign; 
        
        GYCtrl->FileSize = filesize; //;����
        
        GYCtrl->FrameIndex = frameind;
        
        if(size != 512)
        {
            goto ErrDeal;
        }
        
        GYCtrl->PageCur = PROG0_PAGE_START;
        GYCtrl->SizeCur = 0x0;
    }
    if(SEQ & 0x20) //;ĩ֡
    {
        if(frameind != 0xFFFF) //;���һ֡��
        {
            goto ErrDeal;
        }
        
    }
    if(!(SEQ & 0x60)) //;�м�֡
    {
        if(size != 512)
        {
            goto ErrDeal;
        }
        
        if(GYCtrl->FileSign != filesign)
        {
            goto ErrDeal;
        }
        if(GYCtrl->FileSize != filesize)
        {
            goto ErrDeal;
        }
        if((GYCtrl->FrameIndex) != frameind)
        {
            goto ErrDeal;
        }
         
       
    }
    
    crctemp =  *(R_Buf + 30 + size) << 8;
    crctemp += *(R_Buf + 30 + size + 1);  
    crc = Cal_CRC16((u8 *)R_Buf + 30, size);
    if(crc == crctemp)
    {
        //;д���ʶ
        AllBuf.Spi1[0] = 0;
        AllBuf.Spi1[1] = 0;
        AllBuf.Spi1[2] = 0;
        AllBuf.Spi1[3] = 0;
        
        //;����
        *(u32*)(AllBuf.Spi1 + 4) = filesize;
        //;���
        *(u16*)(AllBuf.Spi1 + 8) = frameind;
        *(u16*)(AllBuf.Spi1 + 10) = ~frameind;
        //;�ļ����ݼ�CRC����
        MovN2Byte(size, (u8*)AllBuf.Spi1 + PAGE_HEAD_SIZE, (u8*)R_Buf + 30);
        
        if(frameind == 0xFFFF)
        {
            crc = Cal_CRC16(AllBuf.Spi1 + PAGE_HEAD_SIZE, PAGE_DATA_SIZE);
        }

        *(u16*)(AllBuf.Spi1 + PAGE_HEAD_SIZE + PAGE_DATA_SIZE) = crc;

        
        //;д��FLASH
        SPI_FLASH_PageWrite_E(AllBuf.Spi1, GYCtrl->PageCur++);
//;        
//;        for(i = 0; i < 200; i++)
//;        {
//;            AllBuf.Spi1[i] = 0x00;
//;        }
//;        
//;        SPI_FLASH_PageRead(AllBuf.Spi1, PROG0_PAGE_START);
        
        
        GYCtrl->SizeCur += size;
    }
    else
    {
        goto ErrDeal;
    }
    
    GYCtrl->FrameIndex++;
    if(frameind == 0xFFFF) //;���һ֡
    {     
        if(GYCtrl->SizeCur != GYCtrl->FileSize)
        {
            goto ErrDeal;
        }
        
 //;       SPI_FLASH_PageRead(AllBuf.Spi1, --GYCtrl->PageCur);
        
        
        
        SPI_FLASH_PageRead(AllBuf.Spi1, PROG0_PAGE_START);
        
        AllBuf.Spi1[0] = UPDATASIG0;
        AllBuf.Spi1[1] = UPDATASIG1;
        AllBuf.Spi1[2] = UPDATASIG2;
        AllBuf.Spi1[3] = UPDATASIG3;
        
        //;д��FLASH���±�ʶ
        SPI_FLASH_PageWrite_E(AllBuf.Spi1, PROG0_PAGE_START);
        
        GYCtrl->FileSign = 0;
        GYCtrl->FileSize = 0;
        GYCtrl->FrameIndex = 0;
    }
    *(S_Buf + 16) = 0x01; //;AFN  
    
    *(S_Buf + 0 ) = *(S_Buf + 5) = 0x68;
    *(S_Buf + 6 ) = 0x88;                         //;DIR=1,PRM=0,ACD=0; CID=8
    *(S_Buf + 7 ) = *(R_Buf + 7 );        //;�ն˵�ַ
    *(S_Buf + 8 ) = *(R_Buf + 8 );
    *(S_Buf + 9 ) = *(R_Buf + 9 );
    *(S_Buf + 10) = *(R_Buf + 10);
    *(S_Buf + 11) = *(R_Buf + 11);
    *(S_Buf + 13) = *(R_Buf + 13) & 0x0f; //;֡������SEQ
    *(S_Buf + 13) |= 0x60;                 //;Tpv=0;FIR=1,FIN=1,CON=0;
    *(S_Buf + 12) = 0x00; //;Ӧ�ò㹦���� AFN
    *(S_Buf + 14) = 0x00;
    *(S_Buf + 15) = 0x00; //;p0
    *(S_Buf + 17) = 0x00; //;f3

   
    //;�����򳤶�
    len = 12;
    len *= 4;
    len += 2;  
    *(S_Buf + 1) = *(S_Buf + 3) = (u8)len;
    *(S_Buf + 2) = *(S_Buf + 4) = (u8)(len>>8);
     
    //;У���
    *(S_Buf + 18) = 0;
    for(i=6; i<18; i++)
    {
        *(S_Buf + 18) += *(S_Buf + i);
    }
    
    //;�����ֽ�
    *(S_Buf + 19) = 0x16;
    return 20;
    
ErrDeal:
   
    //;            GYCtrl->PageCur--;
//;            GYCtrl->SizeCur -= size;

    *(S_Buf + 16) = 0x02; //;��֡
 
    *(S_Buf + 0 ) = *(S_Buf + 5) = 0x68;
    *(S_Buf + 6 ) = 0x88;                         //;DIR=1,PRM=0,ACD=0; CID=8
    *(S_Buf + 7 ) = *(R_Buf + 7 );        //;�ն˵�ַ
    *(S_Buf + 8 ) = *(R_Buf + 8 );
    *(S_Buf + 9 ) = *(R_Buf + 9 );
    *(S_Buf + 10) = *(R_Buf + 10);
    *(S_Buf + 11) = *(R_Buf + 11);
    *(S_Buf + 13) = *(R_Buf + 13) & 0x0f; //;֡������SEQ
    *(S_Buf + 13) |= 0x60;                 //;Tpv=0;FIR=1,FIN=1,CON=0;
    *(S_Buf + 12) = 0x00; //;Ӧ�ò㹦���� AFN
    *(S_Buf + 14) = 0x00;
    *(S_Buf + 15) = 0x00; //;p0
    *(S_Buf + 17) = 0x00; //;f3

    //;�����򳤶�
    len = 12;
    len *= 4;
    len += 2;  
    *(S_Buf + 1) = *(S_Buf + 3) = (u8)len;
    *(S_Buf + 2) = *(S_Buf + 4) = (u8)(len>>8);
     
    //;У���
    *(S_Buf + 18) = 0;
    for(i=6; i<18; i++)
    {
        *(S_Buf + 18) += *(S_Buf + i);
    }
    
    //;�����ֽ�
    *(S_Buf + 19) = 0x16;
    return 20;

}


/* 
********************************************************************************
* �� �� ����GDW_AFN10H
* ��    �ܣ�����ת����������
* ��    ����R_Buf-���ջ�������GYCtrl-��Լ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void GDW_AFN10H(char *R_Buf, struct GuiYueCtrl *GYCtrl)
{
    u8  t, DA1, DA2, DT1, DT2, Fn;
    u16 i, Pn;

    if(RelayC.Status != idle)
    {
        return;
    }
    
    if(GYCtrl == &LocGyC)
    {
        RelayC.Channel = 'L';
    }
    else if(GYCtrl == &GprsGyC)
    {
        RelayC.Channel = 'G';
    }
    
    RelayC.Port = *(R_Buf + 18);
    RelayC.BPS  = *(R_Buf + 19) >> 5;
    RelayC.A3   = *(R_Buf + 11);
    RelayC.SEQ  = *(R_Buf + 13);
    
    DA1 = *(R_Buf + 14);
    DA2 = *(R_Buf + 15);
    DT1 = *(R_Buf + 16);
    DT2 = *(R_Buf + 17);

    Pn = Find_Pn(DA1, DA2); //;�����Pn--��Ϣ��
    Fn = Find_Fn(DT1, DT2); //;�����Fn--��Ϣ��

    if((Pn != 0x00) || (Fn != 0x01)) //;��֧��F1��Ϣ��
    {
        RelayC.Status = error; //;��
        return;
    }
    
    //if((RelayC.Port != 1) && (RelayC.Port != 2)) //20150103 �رն˿�2��������͸��
    if(RelayC.Port != 1)
    {
        RelayC.Status = error; //;��
        return;
    }

    
    t = *(R_Buf + 20);
    if(t & 0x80)
    {
        RelayC.OvTime = (t & 0x7F) * 4; //;OvTime��λΪ250ms
    }
    else
    {
        RelayC.OvTime = (t & 0x7F) / 25; //;*10ms/250
    }
    
    if(RelayC.OvTime < 4)
    {
        RelayC.OvTime = 4; //;�����1s
    }
    
    RelayC.Len   = *(u16*)(R_Buf + 22);
    
    if(RelayC.Len > 250)
    {
        return;
    }
    
    for(i = 0; i < RelayC.Len; i++)
    {
        RelayC.Data[i] = *(R_Buf + 24 + i);
    }

    for(i = 0; i < 6; i++)
    {
        RelayC.Tp[i] = *(R_Buf + 24 + RelayC.Len + 16 + i); //20131105
    }	
   
    
    RelayC.Status = 'E'; //;ռ���м̿�����
}


/* 
********************************************************************************
* �� �� ����GDW_AFN10H_Up
* ��    �ܣ�Ӧ������ת������
* ��    ����S-Buf-���ͻ�������GYCtrl-��Լ������
* ��    �أ�Ҫ���͵����ݳ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
u16  GDW_AFN10H_Up(char *S_Buf, struct GuiYueCtrl *GYCtrl)
{
    u8  i;
    u16 len, Ind_S;

    *(S_Buf + 0) = *(S_Buf + 5) = 0x68;
    *(S_Buf + 6) = 0x88; //;DIR=1,PRM=0,ACD=0; CID=8

    /*�ն˵�ַҲ��һ����*/
    *(S_Buf + 7 ) = ZDPKZ.F89.QuXian[0];
    *(S_Buf + 8 ) = ZDPKZ.F89.QuXian[1];
    *(S_Buf + 9 ) = ZDPKZ.F89.DiZhi[0];
    *(S_Buf + 10) = ZDPKZ.F89.DiZhi[1];
    *(S_Buf + 11) = RelayC.A3;
    *(S_Buf + 13) = RelayC.SEQ & 0x0f; //;֡������SEQ
    *(S_Buf + 13) |= 0x60;             //;Tpv=0;FIR=1,FIN=1,CON=0;

    if(RelayC.Status == error)
    {
        *(S_Buf + 12) = 0x00; //;Ӧ�ò㹦���� AFN
        *(S_Buf + 14) = 0x00;  //;P0
        *(S_Buf + 15) = 0x00;
        *(S_Buf + 16) = 0x02;  //;F2ȫ������
        *(S_Buf + 17) = 0x00;
        Ind_S = 18;
    }
    else if(RelayC.Status == right)
    {
        *(S_Buf + 12) = 0x10; //;Ӧ�ò㹦���� AFN
        *(S_Buf + 14) = 0x00;  //;P0
        *(S_Buf + 15) = 0x00;
        *(S_Buf + 16) = 0x01;  //;F1
        *(S_Buf + 17) = 0x00;
        *(S_Buf + 18) = RelayC.Port;
        *(S_Buf + 19) = (u8)RelayC.Len;
        *(S_Buf + 20) = (u8)(RelayC.Len >> 8);
        EW.MoveByte = 'M';
        MovNByte(RelayC.Len, (u8*)S_Buf + 21, (u8*)RelayC.Data);
        EW.MoveByte = 0;
//        Ind_S = RelayC.Len + 19;   //;��2���ֽ�  20131106
        Ind_S = RelayC.Len + 21;		
    }

    if(RelayC.SEQ & 0x80)//;20131104
	{
		*(S_Buf + Ind_S++) = RelayC.Tp[0];
                *(S_Buf + Ind_S++) = RelayC.Tp[1];
                *(S_Buf + Ind_S++) = RelayC.Tp[2];
                *(S_Buf + Ind_S++) = RelayC.Tp[3];
                *(S_Buf + Ind_S++) = RelayC.Tp[4];
		/**(S_Buf + Ind_S++) = TBCD.Sec;
		*(S_Buf + Ind_S++) = TBCD.Min;
		*(S_Buf + Ind_S++) = TBCD.Hour;
		*(S_Buf + Ind_S++) = TBCD.Day;*/
		*(S_Buf + Ind_S++) = RelayC.Tp[5];
                *(S_Buf + 13) |= 0x80;
	}     	

    len = Ind_S - 6;
    len <<= 2;
    len += 2;
    //;------�����򳤶�
    *(S_Buf + 1) = *(S_Buf + 3) =(u8)len;
    *(S_Buf + 2) = *(S_Buf + 4) = (u8)(len >> 8);
    *(S_Buf + Ind_S) = 0;
    //;------У���
    for(i = 6; i < Ind_S; i++)
    {
        *(S_Buf + Ind_S) += *(S_Buf + i);
    }
    //;------�����ֽ�
    *(S_Buf + Ind_S + 1) = 0x16;
    return (Ind_S + 2);
}


//;/*----------------------------------------------------------------------------*
//; |  �յ���վȷ������Ĵ���
//; *----------------------------------------------------------------------------*/
//;void GDW_AFN00H(void)
//;{
//;    if (!(GprsC.State & Login))                                            //;��½�ɹ���������
//;    {
//;
//;    }
//;    else
//;    {
//;        if (GprsUp.Status == 'W')                                          //;�¼��ȴ�Ӧ��
//;        {
//;            GprsUp.Status = 'R';                                           //;�յ�Ӧ��
//;        }
//;        if (GprsC.HB_TimeStart == true)
//;        {
//;            GprsC.HB_TimeStart = false;                                     //;�����ظ�����
//;            GprsC.HB_Time = 0;
//;        }
//;    }
//;}


/*----------------------------------------------------------------------------*
 |  �յ���վ��ȷ������
 *----------------------------------------------------------------------------*/
void GDW_AFN00H(struct GuiYueCtrl *GYCtrl)
{
    if(GYCtrl == &GprsGyC)
    {
        if(GprsC.LoginIn == 'S')
        {
            GprsC.LoginIn = 'R';
        }
        else if(GprsC.HBStatus == 'S')
        {
            GprsC.HBStatus = 'R';
        }
        else if(ERCUp.Status == 'S')
        {
            ERCUp.Status = 'R';
        }
        else if(GprsC.LoginOut == 'S')
        {
            GprsC.LoginOut = 'R';
        }
    }
    else if(GYCtrl == &LocGyC)
    {
        if(ERCUp.Status == 'S')
        {
            ERCUp.Status = 'R';
        }
    }
}


 /* 
********************************************************************************
* �� �� ����GDW_AFN01H
* ��    �ܣ��� AFN=0x01 ��֡���д���   ��λ����
* ��    ����
* ��    �أ� 
* ˵    ������Ӧ
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
u16 GDW_AFN01H(char *R_Buf, char *S_Buf)
{
    u16 i, len, Ind_S;
    u8 Fn, Pn, DA1, DA2, DT1, DT2;
    u8 Set = false,SEQ;
    u8 Legalize = true;
    //s32 long1=0;

    len = *(R_Buf + 1) + (*(R_Buf + 2)<<8);
    len >>= 2; //;�û����������ܳ���

    len -= 8; //;��·�û����ݲ���ܳ���,ȥ��������͵�ַ����ȥ��AFN �� SEQ �����ֽ��Ժ�����ݳ���
 
    SEQ = *(R_Buf + 13);
    if(SEQ & 0x80)                                   //;����ʱ���ǩ
    {
        len -= 6;                                   //;ȥ��ʱ���ǩ����
    }    
    /*
    if(SEQ & 0x80)                                   //;����ʱ���ǩ
    {
        len -= 6;                                   //;ȥ��ʱ���ǩ����
        if(*(R_Buf+19+len)>0)
        {
           long1=Time_distance(&TBCD.Sec, (u8*)(R_Buf + 15 + len));   //;tpv�����ʹ�����ʱ�ж�zrt20140107
           if (long1 < -(*(R_Buf + 19 + len)*60) || long1 > (*(R_Buf+19+len)*60) )
	         {
              return 0;
           }
        }
    } 
    */	
    
    //;Legalize = CalcCRC(R_Buf+6, R_Buf+6+len-2); //;��Ϣ��֤

    *(S_Buf + 0) = *(S_Buf + 5) = 0x68;
    *(S_Buf + 6) = 0x88; //;DIR=1,PRM=0,ACD=0; CID=8

    /*�ն˵�ַҲ��һ����*/
    *(S_Buf + 7 ) = *(R_Buf + 7 );
    *(S_Buf + 8 ) = *(R_Buf + 8 );
    *(S_Buf + 9 ) = *(R_Buf + 9 );
    *(S_Buf + 10) = *(R_Buf + 10);
    *(S_Buf + 11) = *(R_Buf + 11);

    *(S_Buf + 12) = 0x00; //;Ӧ�ò㹦���� AFN

    *(S_Buf + 13) = *(R_Buf + 13) &0x0f; //;֡������SEQ
    *(S_Buf + 13) |= 0x60;                       //;Tpv=0;FIR=1,FIN=1,CON=0;
    
  

    if(Legalize == true)
    {
        DA1 = *(R_Buf + 14);
        DA2 = *(R_Buf + 15);
        DT1 = *(R_Buf + 16);
        DT2 = *(R_Buf + 17);
        Pn = Find_Pn(DA1, DA2); //;�����Pn--��Ϣ��
        Fn = Find_Fn(DT1, DT2); //;�����Fn--��Ϣ��

        if(Pn == 0)
        {
            if(Fn == 1)
            {
                ZD_HardWareReset();
                Set = true;
            }
            else if(Fn == 2)
            {
                ZD_DataInit();        //;ȫ����������ʼ��
                Set = true;
                HWareC.FactoryInit ='E';  //;�ָ��������ñ��				
            }
            else if(Fn == 3)
            {              
                ZD_CSDataInit();        //;������ȫ����������ʼ��
                Set = true;
                HWareC.FactoryInit ='E';  //;�ָ��������ñ��
				
            }
            else if(Fn == 4)
            {
                ZD_NCCSDataInit();
                Set = true;
                HWareC.FactoryInit ='E';  //;�ָ��������ñ��
				
            }
        }
    }

    if(Set == true)
    {
        *(S_Buf + 14) =0x00;  //;P0
        *(S_Buf + 15) =0x00;
        *(S_Buf + 16) =0x01;  //;F1ȫ��ȷ��
        *(S_Buf + 17) =0x00;
    }
    else
    {
        *(S_Buf + 14) =0x00;  //;P0
        *(S_Buf + 15) =0x00;
        *(S_Buf + 16) =0x02;  //;F2ȫ������
        *(S_Buf + 17) =0x00;
    }

    Ind_S = 18;	

    if(SEQ & 0x80)//;20131104
	{
           *(S_Buf + Ind_S++) = *(R_Buf + 14 + len);
           *(S_Buf + Ind_S++) = *(R_Buf + 15 + len);
           *(S_Buf + Ind_S++) = *(R_Buf + 16 + len);
           *(S_Buf + Ind_S++) = *(R_Buf + 17 + len);
           *(S_Buf + Ind_S++) = *(R_Buf + 18 + len);
           *(S_Buf + Ind_S++) = *(R_Buf + 19 + len);
           *(S_Buf + 13) |= 0x80;
	}        	
    

    len = Ind_S - 6;
    len <<= 2;
    len += 2;  //;����Լ�ñ�ʶ
    
    *(S_Buf + 1) = *(S_Buf + 3) =(u8)len; //;�����򳤶�
    *(S_Buf + 2) = *(S_Buf + 4) = (u8)(len >> 8);

    *(S_Buf + Ind_S) = 0;
    
    //;У���
    for(i=6; i<Ind_S; i++)
    {
        *(S_Buf + Ind_S) += *(S_Buf + i);
    }
    
    *(S_Buf + Ind_S + 1) = 0x16; //;�����ֽ�
    return (Ind_S + 2);
}

/* 
********************************************************************************
* �� �� ����GDW_AFN02H
* ��    �ܣ���·�ӿڼ��
* ��    ����
* ��    �أ� 
* ˵    ��������֡
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
u8 GDW_AFN02H(char *R_Buf, struct GuiYueCtrl *GYCtrl)
{
    u8 i;

    *(R_Buf + 0) = 0x68;
    *(R_Buf + 1) = 0x32;
    *(R_Buf + 2) = 0;
    *(R_Buf + 3) = 0x32;
    *(R_Buf + 4) = 0;
    *(R_Buf + 5) = 0x68;
    
    *(R_Buf + 6) = 0xC9;  //;������C [c9]--->DIR=1,PRM=1,ACD=0,CID=9
    
    *(R_Buf + 7) = ZDPKZ.F89.QuXian[0];  //;����������
    *(R_Buf + 8) = ZDPKZ.F89.QuXian[1];
    *(R_Buf + 9) = ZDPKZ.F89.DiZhi[0];
    *(R_Buf + 10) = ZDPKZ.F89.DiZhi[1];  //;�ն˵�ַ
    
    *(R_Buf + 11) = 0;   //;��վ��ַ�����־[00]--->��վ��ַ(0),���־(0)
    *(R_Buf + 12) = 0x02;                  //;������
    *(R_Buf + 13) = (GYCtrl->ISeq) & 0x0F; //;SEQ
    *(R_Buf + 13) |= 0x70;                 //;֡������[70]TpV=0,FIR=1,FIN=1,CON=1
    GYCtrl->ISeq++;
    *(R_Buf + 14) = 0x00;                     //;���ݵ�Ԫ��ʶ
    *(R_Buf + 15) = 0x00;
    *(R_Buf + 16) = GYCtrl->AFN02_FN;
    *(R_Buf + 17) = 0x00;
    *(R_Buf + 18) = 0x00;                     //;У���

    for(i=6; i<18; i++)
    {
        *(R_Buf + 18) += *(R_Buf + i);
    }
    
    *(R_Buf + 19) = 0x16;
    
    return 20;
}

/* 
********************************************************************************
* �� �� ����PreData_AFN05H
* ��    �ܣ������������
* ��    ����
* ��    �أ� u32 --��16λΪ���ͻ���λ�ã���16ΪΪ���ջ���λ��
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��2011-09-15 20:18:08
********************************************************************************
*/
u32 PreData_AFN05H(u16 Ind_S, u16 Ind_R, char *S_Buf, char *R_Buf)
{
    u8 Fn, FnI, DA1, DA2, DT1, DT2;
    u16 Pn;
    u32 Index_R_S;

    *(S_Buf + Ind_S++) = DA1 = *(R_Buf + Ind_R++);
    *(S_Buf + Ind_S++) = DA2 = *(R_Buf + Ind_R++);
    *(S_Buf + Ind_S++) = DT1 = *(R_Buf + Ind_R++);
    *(S_Buf + Ind_S++) = DT2 = *(R_Buf + Ind_R++);

    Pn = Find_Pn(DA1, DA2); //;�����Pn--��Ϣ��
    FnI = Find_FnI(DT1); //;�����Fn--��Ϣ��
    Fn = Find_Fn(DT1, DT2); //;�����Fn--��Ϣ��

    *(S_Buf + Ind_S) = 1; //;���ȼ���ò������ò���ȷ

    if((Pn==0xff) || (Fn==0xff) || (FnI==0xff)) //;Pn����Fn��ʾ����    
    {
        Ind_S -= 4;
    }
    else
    {
        /*��ʱ����*/
        if(Fn == 31)
        {
            ZD_Time_Set((u8*)R_Buf + Ind_R);
            Send_TimetoMeter();
            Ind_R += 6;
            *(S_Buf + Ind_S) = 0;
        }

        /* �������� */
        else if(Fn == 38)
        {
            if((ZDP.F8.WorkMode & 0x03) == 0x02)
            {
                GprsLC.PassLinkOrd = true; //;���յ���������
                GprsLC.PassLinkNum = ZDP.F8.RedailNum;
                *(S_Buf + Ind_S) = 0;  //;������ȷ
            }
            else
            {       
                if(!GprsLC.Link2Switch)
                {
                    GprsLC.Link2Switch = true;
                    //;��������վIP���˿ں�д��
                    GprsLC.IP1_long = NumIpToChar((char *)GprsLC.IP1, 
                                                  ZDP.F3.IP2, 
                                                  *(u16 *)ZDP.F3.Port2);
                    
                    *(S_Buf + Ind_S) = 0;  //;������ȷ
                }         
            }
        }
/*
	 else if(Fn == 39)  //;����Ͽ�����by zrt20130912
	 {
               u8 i = 0;       
               strcpy(GprsC.ATStr,"AT+TCPCLOSE=");
               i += 12;
               *(GprsC.ATStr + i++) = 0 + 0x30;
               *(GprsC.ATStr + i++) = '\r';
               *(GprsC.ATStr + i++) = '\0';
               Modem_AT_Send(GprsC.ATStr);
               *(S_Buf + Ind_S) = 0;  //;������ȷ			   
	 }
*/	 
    }
    Ind_S++;
    Index_R_S = Ind_R << 16;
    Index_R_S += Ind_S;
    return (Index_R_S);
}

/* 
********************************************************************************
* �� �� ����GDW_AFN05H
* ��    �ܣ���������
* ��    ����
* ��    �أ� 
* ˵    ������Ӧ֡
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��2011-09-15 20:14:19
********************************************************************************
*/
u16 GDW_AFN05H(char *R_Buf, char *S_Buf)
{
    u16 i, len, Ind_S;
    u32 Index_R_S;
    u8 right_num = 0;
    u8 error_num = 0;
    u8 Legalize = true;
    u8 SEQ;
    //s32 long1=0;

    SEQ = *(R_Buf + 13); //;֡������
    
    len = *(R_Buf + 1) + (*(R_Buf + 2) << 8);
    len >>= 2; //;�û�����������
    len -= 8;  //;���ݵ�Ԫ����=len-1(������)-5(��ַ��)-1(AFN)-1(SEQ)
    len -= 16; //;PW����
    if(SEQ & 0x80) //;��ȥTpV
    {
        len -= 6;
    }
    /*
    if(SEQ & 0x80)                                   //;����ʱ���ǩ
    {
        len -= 6;                                   //;ȥ��ʱ���ǩ����
        if(*(R_Buf+19+len+16)>0)
        {
           long1=Time_distance(&TBCD.Sec, (u8*)(R_Buf + 15 + len+16));   //;tpv�����ʹ�����ʱ�ж�zrt20140107
           if (long1 < -(*(R_Buf + 19 + len+16)*60) || long1 > (*(R_Buf+19+len+16)*60) )
	   {
              return 0;
           }
        }
    }
	  */
    //;Legalize = CalcCRC(R_Buf + 6, R_Buf + 6 + len - 2 ); //;��Ϣ��֤
    if(Legalize == false)  //;��֤ʧ��
    {
        return 0;
    }       
         
    Ind_S = 19;  
    for(i=14; i<len+14; ) //;�����������
    {
        Index_R_S = PreData_AFN05H(Ind_S, i, S_Buf, R_Buf);
        
        if((Index_R_S&0xffff) < FRAMESIZE)
        {
            Ind_S = (u16)Index_R_S;
            i = Index_R_S >> 16;
        }
        else
        {
            break;
        }
    }
    
    if(!(SEQ & 0x10)) //;CON--����Ҫȷ��
    {
        return 0;
    }
    
    for(i=23; i<Ind_S; i+=5)
    {
        if(*(S_Buf + i) == 0x00)
        {
            right_num++;
        }
        else if(*(S_Buf + i) == 0x01)
        {
            error_num++;
        }
    }
    
    if(right_num == 0) //;ȫ��������ȫ��������Ӧ��    
    {
        Ind_S = 18;
        *(S_Buf + 16) = 0x02; //;AFN
    }
    else if(error_num == 0) //;ȫ����ȷ����ȫ��ȷ����Ӧ��    
    {
        Ind_S = 18;
        *(S_Buf + 16) = 0x01; //;AFN
    }
    else //;�������ݵ�Ԫ��ʶȷ�Ϻͷ���
    {
        *(S_Buf + 16) = 0x04; 
        *(S_Buf + 18) = 0x05; //;AFN
    }
    
    
    
    *(S_Buf + 0) = *(S_Buf + 5) = 0x68; 
    *(S_Buf + 6) = 0x80; //;DIR=1,PRM=0,ACD=0; CID=0  
    *(S_Buf + 7 ) = *(R_Buf + 7 ); //;�ն˵�ַ
    *(S_Buf + 8 ) = *(R_Buf + 8 );
    *(S_Buf + 9 ) = *(R_Buf + 9 );
    *(S_Buf + 10) = *(R_Buf + 10);
    *(S_Buf + 11) = *(R_Buf + 11);   
    *(S_Buf + 12) = 0x00; //;Ӧ�ò㹦���� AFN
    *(S_Buf + 13) = *(R_Buf + 13) &0x0f; //;֡������SEQ
    *(S_Buf + 13) |= 0x60;               //;Tpv=0;FIR=1,FIN=1,CON=0;
    
    *(S_Buf + 14) = 0x00;//;p0
    *(S_Buf + 15) = 0x00; 
    *(S_Buf + 17) = 0x00; //;F3 �����ݱ�ʶȷ��

    if(SEQ & 0x80)//;20131102
	{
		*(S_Buf + Ind_S++) = *(R_Buf + 14 + len + 16);
                *(S_Buf + Ind_S++) = *(R_Buf + 15 + len + 16);
                *(S_Buf + Ind_S++) = *(R_Buf + 16 + len + 16);
                *(S_Buf + Ind_S++) = *(R_Buf + 17 + len + 16);
                *(S_Buf + Ind_S++) = *(R_Buf + 18 + len + 16);
		/**(S_Buf + Ind_S++) = TBCD.Sec;
		*(S_Buf + Ind_S++) = TBCD.Min;
		*(S_Buf + Ind_S++) = TBCD.Hour;
		*(S_Buf + Ind_S++) = TBCD.Day;*/
		*(S_Buf + Ind_S++) = *(R_Buf + 19 + len + 16);;
                *(S_Buf + 13) |= 0x80;
	}  	
    
    //;�����򳤶�
    len = Ind_S - 6; //;��ȥ���Ⱥͱ�ʶ
    len *= 4;
    len += 2;        //;376��ʶ
    *(S_Buf + 1) = *(S_Buf + 3) = (u8)len;
    *(S_Buf + 2) = *(S_Buf + 4) = (u8)(len >> 8);
    
    //;У���
    *(S_Buf + Ind_S) = 0;   
    for(i=6; i<Ind_S; i++)
    {
        *(S_Buf + Ind_S) += *(S_Buf + i);
    }
    //;------�����ֽ�
    *(S_Buf + Ind_S + 1) = 0x16;
    return (Ind_S + 2);    
}


/* 
********************************************************************************
* �� �� ����PreData_AFN0CH
* ��    �ܣ�ΪӦ��AFN=0x0C ������׼������    ��һ������
* ��    ����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
u32 PreData_AFN0CH(u16 Ind_S, u16 Ind_R, char *S_Buf, char *R_Buf)
{
    u8  i, j;
    u8  Fn, FnI, DA1, DA2, DT1, DT2;  
    u8 T, N, L; 
    u8  *Adr8;
    u16 Pn;
    s16 index;
    u32 *Adr32;
    u32 Index_R_S;
    
    *(S_Buf + Ind_S++) = DA1 = *(R_Buf + Ind_R++);
    *(S_Buf + Ind_S++) = DA2 = *(R_Buf + Ind_R++);
    *(S_Buf + Ind_S++) = DT1 = *(R_Buf + Ind_R++);
    *(S_Buf + Ind_S++) = DT2 = *(R_Buf + Ind_R++);

    Pn = Find_Pn(DA1, DA2); //;�����Pn--��Ϣ��
    FnI = Find_FnI(DT1); //;�����Fn--��Ϣ��
    Fn = Find_Fn(DT1, DT2); //;�����Fn--��Ϣ��

    index = ZD_Pn_Find(Pn);
    if((Pn==0xFFFF) || (Fn==0xFF) || (FnI==0xFF)||(index<0)) //;Pn����Fn��ʾ����
    {
        Ind_S -= 4;
        Index_R_S = Ind_R << 16;
        Index_R_S += Ind_S;
        return (Index_R_S);
    }
 
    Adr8 = (u8*)PN0CZU + Fn * 8;
    T =  *Adr8; //;�洢����
    N = *(Adr8 + 1); //;����
    L = *(Adr8 + 2); //;����
    
    if(N == 0)  //;��������ڵı�ʶҲ�ܷ��ط���֡ 20141225
    {
        Ind_S -= 4;
        Index_R_S = Ind_R << 16;
        Index_R_S += Ind_S;
        return (Index_R_S);
    }
    Adr32 = (u32*)(Adr8 + 4);
    Adr8 = (u8*)(*Adr32);
    if(T == 1)       //;����1--��Ӧ�ն�1������
    {
        if(Pn != 0)
        {
            Ind_S -= 4; //;����
        }
        else
        {
            for(i=0; i<N; i++)
            {               
                *(S_Buf + Ind_S++) = *(Adr8 + i);
            }
            
        }
    }
    else if(T == 5)  //;����5--��Ӧ���������Ϣ��
    {
        u32    *Addr32;
        u32    Offset = 0;
        struct PointDataStr *PnD;

        Addr32 = (u32*)(*(PnDTAddr + index));
        PnD = (struct PointDataStr*)(*(Addr32 + _PnD));

        Offset = *(Adr32 + 0); //;�ڽṹ�е�ƫ��
        if(L & 0x01) //;д�볭��ʱ��
        {
            *(S_Buf + Ind_S++) = PnD->CB_Time[0];
            *(S_Buf + Ind_S++) = PnD->CB_Time[1];
            *(S_Buf + Ind_S++) = PnD->CB_Time[2];
            *(S_Buf + Ind_S++) = PnD->CB_Time[3];
            *(S_Buf + Ind_S++) = PnD->CB_Time[4];
        }
        if(L & 0x02) //;д�����
        {
            *(S_Buf + Ind_S++) = 4;
        }
        
        if(Fn>=145&&Fn<=152)     //;�˷�Χ��Fn��ʽ��Ҫ���� by zrt 20130808
        {
            for(i=0; i<5; i++)
            {
              for(j=0; j<3; j++)
              {
                  *(S_Buf + Ind_S++) = *(PnD->Start + Offset + j + 3*i);
              }
              for(j=0; j<4; j++)
              {
                  *(S_Buf + Ind_S++) = *(PnD->Start + Offset + j + 15 + 4*i);
              }
            }
        }
        else
        {
            for (i=0; i< N; i++)
            {
                *(S_Buf + Ind_S++) = *(PnD->Start + Offset + i);
            }
        }
/*
        for (i=0; i< N; i++)
        {
            *(S_Buf + Ind_S++) = *(PnD->Start + Offset + i);
        }        
*/       
    }
    else if(T == 6)  //;����6--��Ӧ������Сʱ�����������������һ�����Сʱ����
    {
        u32 *Addr32;
        u32 Offset ;
        struct PointDataStr *PnD;

        Addr32 = (u32*)(*(PnDTAddr + index));
        PnD = (struct PointDataStr*)(*(Addr32 + _PnD));

        Offset = *(Adr32 + 0); //;�ڽṹ�е�ƫ��
       
        for(i=0; i<N; i++)
        {
            *(S_Buf + Ind_S++) = *(PnD->Start + Offset + i);
        }

    }
    else
    {
        Ind_S -= 4;
    }

    Index_R_S = Ind_R << 16;
    Index_R_S += Ind_S;
    return (Index_R_S);
}


/*******************************************************************************
 * ��������:
 * ����    :  ΪӦ��AFN=0x0E ������׼������
 * ����    :
 * ���    :
 * ����    :
 *******************************************************************************/
u32 PreData_AFN0EH(u16 Ind_S, u16 Ind_R, char *S_Buf, char *R_Buf, struct GuiYueCtrl *GYCtrl)
{
    u16 i, j, Area_ind, Index, StInd, R_Index, Len, Page;
    u8 Fn, Pn, FnI, DA1, DA2, DT1, DT2, ECNum;
    u32 Index_R_S;
    s16 Pn1, Pm1, Y = 0; //;Pn1�������¼���¼����ָ�룬Pm1�������¼���¼��ʼָ��
    s16 Homepage;

    *(S_Buf + Ind_S++) = DA1 = *(R_Buf + Ind_R++);
    *(S_Buf + Ind_S++) = DA2 = *(R_Buf + Ind_R++);
    *(S_Buf + Ind_S++) = DT1 = *(R_Buf + Ind_R++);
    *(S_Buf + Ind_S++) = DT2 = *(R_Buf + Ind_R++);
    Pn = Find_Pn(DA1, DA2); //;�����Pn--��Ϣ��
    FnI = Find_FnI(DT1);    //;�����Fn--��Ϣ��
    Fn = Find_Fn(DT1, DT2); //;�����Fn--��Ϣ��
    if ((Pn == 0xff) || (Fn == 0xff) || (FnI == 0xff))
    //;Pn����Fn��ʾ����
    {
        Ind_R -= 3;
        Ind_S -= 4;
    }
    else if ((Fn == 1) || (Fn == 2))
    {
        *(S_Buf + Ind_S++) = ZDS.F7[0]; //;��ǰ��Ҫ������
        *(S_Buf + Ind_S++) = ZDS.F7[1]; //;��ǰһ�������
        *(S_Buf + Ind_S++) = Pm1 = *(R_Buf + Ind_R++);
        *(S_Buf + Ind_S++) = Pn1 = *(R_Buf + Ind_R++);

        if (Pm1 < Pn1)
        {
            Y = Pn1 - Pm1; //;������¼�����
        }
        if (Pm1 > Pn1)
        {
            Y = ERCReadNum + Pn1 - Pm1;
        }
        if (Fn == 1)
        {
            StInd = ERC1.StInd;  //;�¼��������ʼλ��
            Index = ERC1.Index ; //;�¼�����Ľ���λ��
            Homepage = ERC1Home; //;�¼���Flash�е���ʼҳ
            ECNum = ZDS.F7[0];  //;��ǰ�����ǵ��¼�����
        }
        else if (Fn == 2)
        {
            StInd = ERC2.StInd;
            Index = ERC2.Index;
            Homepage = ERC2Home;
            ECNum = ZDS.F7[1];
        }
        /*--- ��֡���� ---*/
        i = GYCtrl->ERC_Index;
        if (i == 0)
        {
            GYCtrl->ERC_F = 1; //;��һ֡
        }
        else
        {
            GYCtrl->ERC_F = 0; //;�м�֡
        }
        if(Y == 0)
        {
            GYCtrl->ERC_L = 1; //;���һ֡
        }
        Pm1 += i;

        for (  ; i < Y; i++)
        {
           /*----�����ڱ����264���¼��ж�ȡ���µ�256��----*/
            if (ECNum == 0)
            //;��ǰ��256��0�������¼�����
            {
                 if(Index == 0)
                 {
                     GYCtrl->ERC_L = 1; //;���һ֡
                     break; //;���¼�����
                 }
                 R_Index = (StInd + Pm1) % ERCPointNum; //;��StIndΪ��׼���Զ�ȡ0~255�������¼�
            }
            else
            //;��ǰ���·������¼�С��256��
            {
                if( Pm1 < ECNum )
                //;��ǰ��Ҫ��ȡ���¼����С�������¼�����
                {
                    R_Index = (Index - ECNum + Pm1) % ERCPointNum;  //;��Index����ECNumΪ��׼��ȡ���¼�
                }
                else
                //;�� StInd Ϊ��׼��ȡ���¼�
                {
                    if((Index - StInd) >= ERCReadNum )
                    {
                        R_Index = (StInd + Index % ERCPageNum + Pm1 - ECNum) % ERCPointNum ;
                    }
                    else
                    {
                        GYCtrl->ERC_L = 1; //;���һ֡
                        break;//;��û�п�ʼ���ǣ����¼�������
                    }
                }
            }
          /*------------------------------------------------*/

            Page = Homepage + R_Index / ERCPagePoint; //;�¼���Flash�еĵڼ�ҳ
            
            Area_ind = R_Index % ERCPagePoint; //;��ҳ�ĵڼ�����
            R_Index = Area_ind * ERCAreaNum; //;��ҳ�ľ���λ��
            SPI_FLASH_PageRead(AllBuf.Spi1, Page); //;��������������ҳ����ҳ����
            Len = *(AllBuf.Spi1 + R_Index + 1);
            if (Len < ERCAreaNum)
            //;�¼���¼���ݳ��Ȳ��ᳬ�� 66 ���ֽ�
            {
                if(Ind_S + Len + 2 < FRAMESIZE)
                //;д����һ���¼����������֡
                {
                    *(S_Buf + Ind_S++) = *(AllBuf.Spi1 + R_Index + 0); //;д���¼�����
                    *(S_Buf + Ind_S++) = Len; //;д���¼�����
                    for (j = 0; j < Len; j++)
                    {
                        *(S_Buf + Ind_S++) = *(AllBuf.Spi1 + R_Index + 2 + j); //;��������
                    }
                    Pm1++;
                    Pm1 %= ERCPointNum;
                    GYCtrl->ERC_Index = 0;
                    GYCtrl->ERC_L = 1; //;���һ֡
                }
                else
                //;��Ҫ��֡
                {
                    GYCtrl->ERC_Index = i; //;������Ϊ��һ�η����Ŀ�ʼ
                    GYCtrl->ERC_L = 0; //;�����һ֡
                    break;
                }
            }
            else
            {
                GYCtrl->ERC_L = 1; //;���һ֡
                break; //;�¼�������
            }
        }
    }
    else
    {
        Ind_S -= 4;
    }
    Index_R_S = Ind_R << 16;
    Index_R_S += Ind_S;
    return (Index_R_S);
}


/* 
********************************************************************************
* �� �� ����GDW_AFN_CHK
* ��    �ܣ����֡�����Ƿ����376.1��Լ
* ��    ����R_Buf-�������ݻ�����
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
u8 GDW_AFN_CHK(char *R_Buf)
{
    u8 sum;
    u16 i, len;

    if((*(R_Buf + 0) != 0x68) || (*(R_Buf + 5) != 0x68))
    {
        return false; //;;֡ͷ����ȷ
    }

    if((*(R_Buf + 1) != *(R_Buf + 3)) || (*(R_Buf + 2) != *(R_Buf + 4)))
    {
        return false; //;;���Ȳ���ȷ
    }

    if((*(R_Buf + 7) != ZDPKZ.F89.QuXian[0]) || (*(R_Buf + 8) != ZDPKZ.F89.QuXian[1]))
    {
        return false; //;;���������벻��ȷ
    }

    if(!(ZDPKZ.F89.DiZhi[0] == *(R_Buf + 9) && ZDPKZ.F89.DiZhi[1] == *(R_Buf + 10)) &&
        !(*(R_Buf + 9) == 0xff && *(R_Buf + 10) == 0xff && (*(R_Buf + 11) & 0x01)))
    {
        return false; //;;�ն˵�ַ����ȷ��Ҳ���ǹ㲥��ַ
    }

    len = *(R_Buf + 1) + *(R_Buf + 2) * 0x100;
    len >>= 2;

    if(len > 550)
    {
        return false;
    }

    sum = 0;

    for(i = 6; i < len + 6; i++)
    {
        sum += *(R_Buf + i);
    }

    if(sum != *(R_Buf + len + 6))
    {
        return false; //;;У��Ͳ����
    }

    return true;
}


/* 
********************************************************************************
* �� �� ����GDW_AFN_CHK_HW
* ��    �ܣ�����ͨѶ֡ͷ���
* ��    ����R_Buf-���յ�������
* ��    �أ�-1-����֡��������-��һ��68���ֵ�λ��
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
s8 GDW_AFN_CHK_HW(char *R_Buf, u16 BufLen)
{
    u8 sum;
    u16 i, len;
    s8 sign = 0;
    
    while((*(R_Buf + sign) != 0x68) && (sign < BufLen))
    {
        sign++;
    }
    
    R_Buf += sign;
    if((*(R_Buf + 0) != 0x68) || (*(R_Buf + 5) != 0x68))
    {
        return -1; //;֡ͷ����ȷ
    }

    if((*(R_Buf + 1) != *(R_Buf + 3)) || (*(R_Buf + 2) != *(R_Buf + 4)))
    {
        return -1; //;���Ȳ���ȷ
    }
    
     len = *(R_Buf + 1) + *(R_Buf + 2) * 0x100;
     len >>= 2;
     if(len > 550)
     {
         return -1;
     }

     sum = 0;
     for(i = 6; i < len + 6; i++)
     {
         sum += *(R_Buf + i);
     }

     if(sum != *(R_Buf + len + 6))
     {
         return -1; //;У��Ͳ����
     }
    
    if(*(R_Buf + 12) == 0x0A) //;��ѯ����
    {
        if((*(R_Buf + 14) == 0x00) && (*(R_Buf + 15) == 0x00) &&
           (*(R_Buf + 16) == 0x01) && (*(R_Buf + 17) == 0x0b)) //;��ѯF89�ն˵�ַ
        {
            if((*(R_Buf + 7) == 0xAA) && (*(R_Buf + 8) == 0xAA) &&
               (*(R_Buf + 9) == 0xAA) && (*(R_Buf + 10) == 0xAA))
            {     
                return sign;  //; �ն˵�ַΪ AAAA AAAA ʱ���Զ�����ǰ���ն˵�ַ
            }
        }
    }

    if((*(R_Buf + 7) != ZDPKZ.F89.QuXian[0]) || (*(R_Buf + 8) != ZDPKZ.F89.QuXian[1]))
    {
        return -1; //;���������벻��ȷ
    }

    if(!(ZDPKZ.F89.DiZhi[0] == *(R_Buf + 9) && ZDPKZ.F89.DiZhi[1] == *(R_Buf + 10)) &&
        !(*(R_Buf + 9) == 0xff && *(R_Buf + 10) == 0xff && (*(R_Buf + 11) & 0x01)))
    {
        return -1; //;�ն˵�ַ����ȷ��Ҳ���ǹ㲥��ַ
    }

    return sign;
}


/* 
********************************************************************************
* �� �� ����AFN_FlagFind
* ��    �ܣ��ҳ�Ҫͨ�����ڷ��͵��¼����־
* ��    ����GYCtrl-��Լ������
* ��    �أ�0xFF-����Ҫ�������ݣ�����-����ʱ�����־
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
u8 AFN_FlagFind(struct GuiYueCtrl *GYCtrl)
{
    if(GYCtrl->AFNList[0])
    {
        return  0;
    }

    if(GYCtrl->AFNList[1])
    {
        return  1;
    }

    if(GYCtrl->AFNList[2])
    {
        return  2;
    }

    if(GYCtrl->AFNList[3])
    {
        return  3;
    }

    if(GYCtrl->AFNList[4])
    {
        return  4;
    }

    if(GYCtrl->AFNList[5])
    {
        return  5;
    }

    if(GYCtrl->AFNList[6])
    {
        return  6;
    }

    return  0xFF;
}


/* 
********************************************************************************
* �� �� ����AFN_DataPre
* ��    �ܣ���Լ���ݵ�Ӧ�����ʹ���
* ��    ����ListNo-�¼���־�飻Buf_Rx-���ջ�������Buf_Tx-���ͻ�����
*           GYCtrl-��Լ���������
* ��    �أ��跢�����ݵĳ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
u16 AFN_DataPre(u8 ListNo, char *Buf_Rx, char *Buf_Tx, struct GuiYueCtrl *GYCtrl)
{
    u16 TLong = 0;

    switch(GYCtrl->AFNList[ListNo])
    {
        case 0xEE:
        {
            GDW_AFN00H(GYCtrl);
            break;
        }
        case 0x01: //;��λ
        {
            TLong =  GDW_AFN01H(Buf_Rx, Buf_Tx);
            break;
        }
        case 0x02: //;��·�ڼ��
        {
            TLong =  GDW_AFN02H(Buf_Tx, GYCtrl);
            break;
        }
        case 0x04:
        {
            TLong = GDW_AFN04H(Buf_Rx, Buf_Tx);
            break;
        }
        case 0x05:
        {
            TLong = GDW_AFN05H(Buf_Rx, Buf_Tx);
            break;
        }
        case 0x09:
        case 0x0A:
        {
            TLong = GDW_AFN090AH(Buf_Rx, Buf_Tx, GYCtrl);
            break;
        }
        case 0x0B:
        {
            //;EW.TaskAUL_P = 'P';
            //;TLong = Task_AUpLoad_Program(Buf_Tx, GYCtrl);
            //;EW.TaskAUL_P = 0;
            break;
        }
        case 0x0C:
        {
            if((ListNo == 0) || (ListNo == 1))
            {
                TLong = GDW_AFN0CH(Buf_Rx, Buf_Tx, GYCtrl);
            }
            else if(ListNo == 4)
            {
                TLong = GDW_AFN0CH_Up(Buf_Tx, GYCtrl);
            }
            break;
        }
        case 0x0D:
        {
            if((ListNo == 0) || (ListNo == 1))
            {
                TLong = GDW_AFN0DH(Buf_Rx, Buf_Tx, GYCtrl);
            }
            else if(ListNo == 4)
            {
                TLong = GDW_AFN0DH_Up(Buf_Tx, GYCtrl);
            }
            break;
        }
        case 0x0E:
        {
            if((ListNo == 0) || (ListNo == 1))
            {
                TLong = GDW_AFN0EH(Buf_Rx, Buf_Tx, GYCtrl);
            }
            else if(ListNo == 3)
            {
                TLong = GDW_AFN0EH_Up(Buf_Tx, GYCtrl);
            }
            else if(ListNo == 6)
            {
                if(GYCtrl == &GprsGyC)
                {
                    //;;TLong = ERC_SMS_Up(Buf_Tx);
                }
            }
            break;
        }
        /*case 0x0F: //;�ر��ļ�����--20141225
        {
            TLong = GDW_AFN0FH(Buf_Rx, Buf_Tx, GYCtrl);
            break;    
        }
        */
        case 0x10:
        {
            if(ListNo == 1)
            {
                GDW_AFN10H(Buf_Rx, GYCtrl);              //;����ת��
            }
            else if(ListNo == 2)
            {
                TLong = GDW_AFN10H_Up(Buf_Tx, GYCtrl);
            }
            break;
        }
        default:
        {
            break;
        }
    }

    return  TLong;
}


/* 
********************************************************************************
* �� �� ����AFN_Gprs_Flag
* ��    �ܣ�д���Լ�����Ҫ������¼���־
* ��    ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void AFN_Gprs_Flag(void)
{
    if(GprsGyC.RecOrd == 'E')
    {
        GprsGyC.RecOrd = 0;
        if(!GprsGyC.AFNList[1])
        {
            if(*(GPRSREVBUF + 12) == 0)
            {
                GprsGyC.AFNList[1] = 0xEE;
            }
            else
            {
                GprsGyC.AFNList[1] = *(GPRSREVBUF + 12);
            }
        }
    }

    if((RelayC.Status == right) || (RelayC.Status == error))  //;����ת��
    {
        if(RelayC.Channel == 'G')
        {
            if(!GprsGyC.AFNList[2])
            {
                GprsGyC.AFNList[2] = 0x10;
            }
        }
    }

    if(ERCUp.Status == 'G') //;Ҫ�����¼�
    {

        if(!GprsGyC.AFNList[3])
        {
            GprsGyC.AFNList[3] = 0x0E;
        }
    }

    if(TaskC.Status == 'G')   //;Ҫ����1������
    {
        if(!GprsGyC.AFNList[4])
        {
            if(TaskC.TaskTn == '1')
            {
                GprsGyC.AFNList[4] = 0x0C;
            }
            else if(TaskC.TaskTn == '2')
            {
                GprsGyC.AFNList[4] = 0x0D;
            }
        }
    }

    if(GprsC.LoginIn == 'E')  //;��½
    {
        if (!GprsGyC.AFNList[5])
        {
            GprsGyC.AFNList[5] = 0x02;
            GprsGyC.AFN02_FN = 0x01;
        }
    }
    else if(GprsC.LoginOut == 'E')       //;�˳���½
    {
        if(!GprsGyC.AFNList[5])
        {
            GprsGyC.AFNList[5] = 0x02;
            GprsGyC.AFN02_FN = 0x02;
        }
    }
    else if(GprsC.HBStatus == 'E')    //;����
    {
        if(!GprsGyC.AFNList[5])
        {
            GprsGyC.AFNList[5] = 0x02;
            GprsGyC.AFN02_FN = 0x04;
        }
    }

    if(SMS.Status == 'E') //;���������¼�
    {
        if(!GprsGyC.AFNList[6])
        {
            GprsGyC.AFNList[6] = 0x0E;
        }
    }
}


/* 
********************************************************************************
* �� �� ����AFN_Gprs_FlagDel
* ��    �ܣ����Ҫͨ�����ڷ��͵Ĺ�Լ������ݵ�APN��־
* ��    ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void AFN_Gprs_FlagDel(u8 ListNo)
{
    if(ListNo > 0)
    {
        GprsGyC.AFNList[ListNo] = 0;
    }

    if(ListNo == 2)
    {
        if((RelayC.Status == right) || (RelayC.Status == error))  //;����ת��
        {
            if (RelayC.Channel == 'G')
            {
                RelayC.Status = idle;
                RelayC.Channel = 0;
            }
        }
    }

    if(ListNo == 3)
    {
        if(ERCUp.Status == 'G')
        {
            ERCUp.Status = 'S';
        }
    }

    if(ListNo == 4)
    {
        if(TaskC.Status == 'G')
        {
            TaskC.Status = 'S'; //;�����������
        }
    }

    if(ListNo == 5)
    {
        GprsGyC.AFN02_FN = 0;
        if(GprsC.LoginIn == 'E')
        {
            GprsC.LoginIn = 'S';  //;��½���
        }
        else if(GprsC.LoginOut == 'E')  //;�˳���½
        {
            GprsC.LoginOut = 'S';   //;��½���
        }
        else if(GprsC.HBStatus == 'E')    //;����
        {
            GprsC.HBStatus = 'S';
            GprsC.HBOVT = 0;
        }
    }

    if(ListNo == 6)
    {
        if(SMS.Status == 'E')
        {
            SMS.Status = 'S'; //;���Ͷ������
        }
    }
}


/* 
********************************************************************************
* �� �� ����AFN_Gprs_Rec
* ��    �ܣ�����GPRSͨ��ͨѶ֡
* ��    ����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
void AFN_Gprs_Rec(void)
{
     u16 i, len;
     
    if(GprsC.TCPRec == true)
    {
        GprsC.TCPRec = false;

        GprsC.TCPBuf = strstr(GprsC.TCPBuf, "h"); //;;Ѱ���״γ���֡ͷ68��λ��

        if(GDW_AFN_CHK(GprsC.TCPBuf) == true)
        {
            len = *(GprsC.TCPBuf + 1) + *(GprsC.TCPBuf + 2) * 0x100;
            len >>= 2;
/*
            ZDS.F10.Flux_Day += len;
            ZDS.F10.Flux_Day += 50;
            ZDS.F10.Flux_Month += len;
            ZDS.F10.Flux_Month += 50; //;;����ͳ��
by  zrt 20130710
*/
            ZDS.F10.Flux_Day += (len+8);                       //;�����ݳ���len+6+У����+������
            ZDS.F10.Flux_Month += (len+8);
            if(ZDS.F10.Flux_Month > FluxLimValue)
            {
                return; //;;��������
            }

            for(i = 0; i < GprsC.TCPLen; i++)
            {
                *(GPRSREVBUF + i) = *(GprsC.TCPBuf + i);
            }

            GprsGyC.RecOrd = 'E'; //;;�յ�Զ��GDWͨѶ֡
#ifdef JTDK_USART1_ON             
            if(LOCUSARTCTR.RecOrd == false) //;---20141215
            {
              LOCUSARTCTR.TLong = len+8;
              MovNByte(LOCUSARTCTR.TLong, (u8*)&AllBuf.Tx1_1[0],(u8*)&AllBuf.Rx2_2[0]);
            	LOCUSARTCTR.SedOrd = 'E';
              LOCUSARTCTR.BufAdr = LOCSEDBUF;
              USART1_Send();
              if((GprsC.HBStatus == 0)&&( GprsC.LoginIn ==0 ))/*�������͵�½ʱ�򣬲���Ҫ��ʱ�ȴ����ڷ���*/
              {
              	Delay_nms(50);
              	Watchdog(); 
              	Delay_nms(50);
              } 
            }
            else
            {
            	LOCUSARTCTR.SedOrd = 0;
            }
#endif            
        }
    }
}



/* 
********************************************************************************
* �� �� ����AFN_Gprs_Com
* ��    �ܣ����ƹ�Լ����ͨ��GPRSͨ������
* ��    ����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
void AFN_Gprs_Com(void)
{
    AFN_Gprs_Rec();
    AFN_Gprs_Flag();

    if(GprsGyC.Status == idle)
    {
        if(GPRSUSARTCTR.SedOrd == 0x00)
        {
            GprsGyC.ListNo = AFN_FlagFind(&GprsGyC); //;�ҳ�Ҫ���͵�APN
            
            if(GprsGyC.ListNo != 0xFF)
            {
                u16  Len;
            
                Len = AFN_DataPre(GprsGyC.ListNo, GPRSREVBUF, GPRSSEDBUF, &GprsGyC);   //;lenΪ��һ֡����
            
                if((Len != 0) && (Len < U2TXBUFSIZE))
                {
                    if(GprsGyC.ListNo == 6)
                    {
                        GprsC.SMSLen = Len; //;;д�뷢�ͳ���
                        GprsGyC.Status = 'M';  //;������
                    }
                    else
                    {
                        if(ZDS.F10.Flux_Month < FluxLimValue)
                        {
/*                        
                            ZDS.F10.Flux_Day += Len;
                            ZDS.F10.Flux_Day += 50;                                                 //; ��������
                            ZDS.F10.Flux_Month += Len;
                            ZDS.F10.Flux_Month += 50;                                               //; ������ͳ��
by zrt 20130710                            
*/                            
                            ZDS.F10.Flux_Day += Len;
                            ZDS.F10.Flux_Month += Len;
                            GprsC.TCPLen = Len; //;;д�뷢�ͳ���
                            GprsGyC.Status = 'E';  //;;TCP����
#ifdef JTDK_USART1_ON
                            if(LOCUSARTCTR.RecOrd == false) //;---20141215
                            {
                            	LOCUSARTCTR.TLong = GprsC.TCPLen;
                            	MovNByte(LOCUSARTCTR.TLong, (u8*)&AllBuf.Tx1_1[0],(u8*)&AllBuf.Tx2_1[0]);
                            }
#endif                            
                            
                        }
                        else
                        {
                            GprsGyC.Status = idle;
                            AFN_Gprs_FlagDel(GprsGyC.ListNo);
                        }
                    }
                }
                else
                {
                    GprsGyC.Status = idle;
                    AFN_Gprs_FlagDel(GprsGyC.ListNo);
                }
            }     
        }    
    }

    if (GprsGyC.Status == 'E')
    {
        if (Modem_TCP_Send() != 0xEF) //;;ͨ��TCP����
        {
            GprsGyC.Status = idle;
            AFN_Gprs_FlagDel(GprsGyC.ListNo);  //;APNͨ��TCP������� ---20141215
#ifdef JTDK_USART1_ON            
            if(LOCUSARTCTR.RecOrd == false)    //;GPRS������Ϻ󣬴��ڷ������ݣ������н�������������ֹ���ͣ��ȴ�����������
            {
            	LOCUSARTCTR.BufAdr = LOCSEDBUF;
            	LOCUSARTCTR.SedOrd = 'E';
            }
            else
            {
            	LOCUSARTCTR.SedOrd = 0;
            }
#endif            
        }
    }

    if (GprsGyC.Status == 'M')
    {
        //;if (Modem_SMS_Send() == true) //;;ͨ��SMS����
        //;{
        //;    U2GY.Status = idle;
        //;    AFN_Gprs_FlagDel(U2GY.ListNo);  //;APNͨ�����ŷ������
        //;}
    }
}


/* 
********************************************************************************
* �� �� ����AFN_Local_Flag
* ��    �ܣ�д�뱾��ͨѶ��Լ��ʶ
* ��    ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void AFN_Local_Flag(void)
{   
    if(LocGyC.RecOrd == 'E') //;;���ݵ㳭
    {
        LocGyC.RecOrd = 0;
        if(!LocGyC.AFNList[1])
        {
            if(LOCREVBUF[12] == 0)
            {
                LocGyC.AFNList[1] = 0xEE;
            }
            else
            {
                LocGyC.AFNList[1] = LOCREVBUF[12];
            }
        }
    }  

    if ((RelayC.Status == right) || (RelayC.Status == error)) //;����ת��
    {
        if (RelayC.Channel == 'L')
        {
            if (!LocGyC.AFNList[2])
            {
                LocGyC.AFNList[2] = 0x10;
            }
        }
    }

    if (ERCUp.Status == 'L') //;Ҫ�����¼�
    {
        if (!LocGyC.AFNList[3])
        {
            LocGyC.AFNList[3] = 0x0E;
        }
    }

    if (TaskC.Status == 'L') //;Ҫ����1������
    {
        if (!LocGyC.AFNList[4])
        {
            if (TaskC.TaskTn == '1')
            {
                LocGyC.AFNList[4] = 0x0C;
            }
            else if (TaskC.TaskTn == '2')
            {
                LocGyC.AFNList[4] = 0x0D;
            }
        }
    }

    if (InfGyC.RecOrd == 'E') //;;����ͨѶ֡
    {
        InfGyC.RecOrd = 0;
        if (!InfGyC.AFNList[1])
        {
            if (INFREVBUF[12] == 0)
            {
                InfGyC.AFNList[1] = 0xEE;
            }
            else
            {
                InfGyC.AFNList[1] = INFREVBUF[12];
            }
        }
    }

}


/* 
********************************************************************************
* �� �� ����AFN_Local_FlagDel
* ��    �ܣ�����¼���־��
* ��    ����GYCtrl-��Լ���������
* ��    �أ���
* ˵    �����¼�������ɺ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void AFN_Local_FlagDel(struct GuiYueCtrl *GYCtrl)
{
    if(GYCtrl->ListNo > 0)
    {
        GYCtrl->AFNList[GYCtrl->ListNo] = 0;
    }
    
    if(GYCtrl->ListNo == 2)
    {
        if(GYCtrl == &LocGyC)
        {
            if((RelayC.Status == right) || (RelayC.Status == error))  //;����ת��
            {
                if (RelayC.Channel == 'L')
                {
                    RelayC.Status = idle;
                    RelayC.Channel = 0;
                }
            }
        }
    }
    else if(GYCtrl->ListNo == 3)
    {
        if (GYCtrl == &LocGyC)
        {
            if (ERCUp.Status == 'L')
            {
                ERCUp.Status = 'S';
            }
        }
    }
    else if(GYCtrl->ListNo == 4)
    {
        if (GYCtrl == &LocGyC)
        {
            if (TaskC.Status == 'L')
            {
                TaskC.Status = 'S'; //;�����������
            }
        }
    }
}


/* 
********************************************************************************
* �� �� ����AFN_Local_Rec
* ��    �ܣ����ձ���ͨ��ͨѶ֡
* ��    ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void AFN_Local_Rec(void)
{    
    s8 n;
    u16 i, k;
    
#ifdef Local_USART1_ON    
    if(LOCUSARTCTR.RecOrd == true)
    {            
        LOCUSARTCTR.RecOrd = false;//;--20150105
        if((n = (u16)GDW_AFN_CHK_HW(LOCUSARTCTR.BufAdr, LOCUSARTCTR.RecLen)) != -1)
        {  
            for(i = n, k = 0; i < LOCUSARTCTR.RecLen; i++, k++)
            {
                LOCREVBUF[k] = *(LOCUSARTCTR.BufAdr + i);
            }
            LocGyC.RecOrd = 'E';
            LocGyC.AFNList[1] = 0;
        }
        /*if(GDW_AFN_CHK(LOCUSARTCTR.BufAdr) == true)
        {
            for(i = 0; i < LOCUSARTCTR.RecLen; i++)
            {
                LOCREVBUF[i] = *(LOCUSARTCTR.BufAdr + i);
            }

            LocGyC.RecOrd = 'E';
            LocGyC.AFNList[1] = 0;
        }*/
    }
#endif

    if(INFUSARTCTR.RecOrd == true)
    {
        INFUSARTCTR.RecOrd = false;          
        if((n = (u16)GDW_AFN_CHK_HW(INFUSARTCTR.BufAdr, INFUSARTCTR.RecLen)) != -1)
        {  
            for(i = n, k = 0; i < INFUSARTCTR.RecLen; i++, k++)
            {
                INFREVBUF[k] = *(INFUSARTCTR.BufAdr + i);
            }
    
            InfGyC.RecOrd = 'E';
            InfGyC.AFNList[1] = 0;
        }
    }
}


/* 
********************************************************************************
* �� �� ����AFN_Local_Com
* ��    �ܣ����ƹ�Լ����ͨ������ͨ�����ͣ��������ڵ����������ͨѶ
* ��    ����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/ 
void AFN_Local_Com(void)
{
    
    AFN_Local_Rec();
    AFN_Local_Flag();
    
#ifdef Local_USART1_ON
    if(LocGyC.Status == idle) //;;���ش���1ͨѶ
    {
        LocGyC.ListNo = AFN_FlagFind(&LocGyC); //;�ҳ�Ҫ���͵�APN
        if(LocGyC.ListNo != 0xFF)
        {
            u16 Len;
            Len = AFN_DataPre(LocGyC.ListNo, LOCREVBUF, LOCSEDBUF, &LocGyC);
            if((Len != 0) && (Len < FRAMESIZE))
            {
                if(LOCUSARTCTR.SedOrd == 0)
                {
                    LOCUSARTCTR.SedOrd = 'E';
                    LOCUSARTCTR.TLong = Len;
                    LOCUSARTCTR.BufAdr = LOCSEDBUF;
                    LOCUSARTCTR.Mark = 'G';
                }
            }
            //;����ת�����ݲ��ɹ�
            AFN_Local_FlagDel(&LocGyC);   //;--20141229 APNͨ�����ص��Կڷ������
        }
    }
#endif

    if(InfGyC.Status == idle) //; ���غ���ͨѶ
    {
        InfGyC.ListNo = AFN_FlagFind(&InfGyC); //;�ҳ�Ҫ���͵�APN
        if(InfGyC.ListNo != 0xFF)
        {
            u16 Len;
    
            Len = AFN_DataPre(InfGyC.ListNo, INFREVBUF, INFSEDBUF, &InfGyC);
            if((Len != 0) && (Len < 512))
            {
                if(INFUSARTCTR.SedOrd == 0) //;ͨ������3����
                {
                    INFUSARTCTR.SedOrd = 'E';
                    INFUSARTCTR.TLong = Len;
                    INFUSARTCTR.BufAdr = INFSEDBUF;
                }
            }
            AFN_Local_FlagDel(&InfGyC);   //;APNͨ�����ص��Կڷ������
        }
    }
}


/* 
********************************************************************************
* �� �� ����AFNComDeal
* ��    �ܣ����Կں�GPRSͨ����GDW376.1��ԼAFN��������Ӧ���������͵ȡ�
* ��    ����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
void AFNComDeal(void)
{
    AFN_Gprs_Com(); //;Զ��ͨѶ
    AFN_Local_Com(); //;����ͨѶ
}
