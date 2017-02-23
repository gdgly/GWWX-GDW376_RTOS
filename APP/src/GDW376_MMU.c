/*
********************************************************************************
*  �� �� ����GDW376_MMU.c
*
*  �ļ�������Flash��������Ĵ洢�Ͷ�ȡ���������ݵĴ洢�Ͷ�ȡ
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

extern B1 DaysOfMonth[13];

/* 
********************************************************************************
* �� �� ����Buf_Sum
* ��    �ܣ���AllBuf.Spi1������0~526���ݺ�
* ��    ������
* ��    �أ�����1Byte���ݺ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��2011-09-14 11:31:29
********************************************************************************
*/
u8 FlashBufSum(void)
{
    u8  sum = 0x55;
    u16 j;
    
    for(j = 0; j < 527; j++)
    {
        sum += AllBuf.Spi1[j];
    }
    
    return sum;
}


/* 
********************************************************************************
* �� �� ����ReadFlash_Verify
* ��    �ܣ�Page1����У�鲻��ȷ�������������Page2���ݡ��Դ��򷵻�Page1.
* ��    ����Page1-Ҫ����ҳ��Page2-��Ӧ����ҳ
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void ReadFlash_Verify(u16 Page1, u16 Page2)
{ 
    SPI_FLASH_PageRead(AllBuf.Spi1, Page1); //;����������1������
    
    if(FlashBufSum()!= AllBuf.Spi1[527])
    {
        SPI_FLASH_PageRead(AllBuf.Spi1, Page2); //;����������2������
        
        if(FlashBufSum() != AllBuf.Spi1[527])
        {
            SPI_FLASH_PageRead(AllBuf.Spi1, Page1); //;����������1������
        }
    }
}

/* 
********************************************************************************
* �� �� ����MMU_FM24_CHK
* ��    �ܣ����FM24�Ƿ��һ���ϵ�
* ��    ������
* ��    �أ�true--FM24��д�����ݣ�flase--FM24δд������
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
u8 MMU_FM24_CHK(void)
{
    u8 ch1, FM24BZ[4];
    
    for(ch1=0; ch1<16; ch1++)  //;����16��δ������־�����ж�flase
    {
        EW.RFM = 'R';
        ReadNByte_I2C1(4, E2R_BiaoZhi, FM24BZ);
        EW.RFM = 0;
        if((FM24BZ[0]==_FM24_BZ1) && (FM24BZ[1]==_FM24_BZ2) && 
           (FM24BZ[2]==_FM24_BZ3) && (FM24BZ[3]==_FM24_BZ4))
        {
            return true;
        }
    }
    
    return false;
}


/* 
********************************************************************************
* �� �� ����MMU_FM24_Read
* ��    �ܣ��ϵ�ʱ�̶���FM24�б�������ݡ�
* ��    ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void MMU_FM24_Read(void)
{
    u8  i, FM24BZ[4];
    u16 E2R_PnDDJ, E2R_PnBDJ, E2R_PnMDJ, E2R_PnQDJ;
    u32 *Addr32;
    struct DJ_Control *PnDDJ,  *PnBDJ,  *PnMDJ;
    struct QX_Control *PnQDJ;

    if(MMU_FM24_CHK() == true)
    {
        ReadData_FM24(4, E2R_ERC1, E2R_ERC1 + E2R_ByteNum, (u8*) &ERC1.Index); //;������Ҫ�¼����Ʋ���
        ReadData_FM24(4, E2R_ERC2, E2R_ERC2 + E2R_ByteNum, (u8*) &ERC2.Index); //;����һ���¼����Ʋ���
        ReadData_FM24(1, E2R_EC1,  E2R_EC1 + E2R_ByteNum,  (u8*) ZDS.F7);    //;�������ݼ�����EC
        ReadData_FM24(1, E2R_EC2,  E2R_EC2 + E2R_ByteNum,  (u8*) ZDS.F7+1);
        ReadData_FM24(1, E2R_ERCUP,  E2R_ERCUP + E2R_ByteNum,  (u8*)&ERCUp.UpIndex);//;�������
        ReadData_FM24(4, Version,  Version + E2R_ByteNum,  SofewareOldVer); //;�����ɵ�����汾��

        for(i=0; i<PNMAXNUM; i++)
        {
            Addr32 = (u32*)(*(PnDTAddr + i));
            PnDDJ = (struct DJ_Control*)(*(Addr32 + _PnDDJ));
            PnBDJ = (struct DJ_Control*)(*(Addr32 + _PnBDJ));
            PnMDJ = (struct DJ_Control*)(*(Addr32 + _PnMDJ));
            PnQDJ = (struct QX_Control*)(*(Addr32 + _PnQDJ));
            E2R_PnDDJ = (u16)(*(Addr32 + _E2R_PnDDJ));
            E2R_PnBDJ = (u16)(*(Addr32 + _E2R_PnBDJ));
            E2R_PnMDJ = (u16)(*(Addr32 + _E2R_PnMDJ));
            E2R_PnQDJ = (u16)(*(Addr32 + _E2R_PnQDJ));
            ReadData_FM24(2, E2R_PnDDJ, E2R_PnDDJ + E2R_ByteNum, &PnDDJ->Index); //;�����ն�����Ʋ���
            ReadData_FM24(2, E2R_PnBDJ, E2R_PnBDJ + E2R_ByteNum, &PnBDJ->Index); //;���������ն�����Ʋ���
            ReadData_FM24(2, E2R_PnMDJ, E2R_PnMDJ + E2R_ByteNum, &PnMDJ->Index); //;�����¶�����Ʋ���
            ReadData_FM24(4, E2R_PnQDJ, E2R_PnQDJ + E2R_ByteNum, (u8*) &PnQDJ->Index); //;�������߶�����Ʋ���
        } 
    }
    else
    {
        FM24BZ[0] = _FM24_BZ1;
        FM24BZ[1] = _FM24_BZ2;
        FM24BZ[2] = _FM24_BZ3;
        FM24BZ[3] = _FM24_BZ4;
        EW.WFM = 'W';
        WriteNByte_I2C1(4, E2R_BiaoZhi, FM24BZ);
        EW.WFM = 0;
    }
}


/* 
********************************************************************************
* �� �� ����CS_PnSave
* ��    �ܣ��������������
* ��    ����Pn-�������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void CS_PnSave(u8 Pn)
{
    u16 j = 0;
    u32 *Addr32;
    struct PoPStr  *PnP;

    Addr32 = (u32*)(*(PnDTAddr + Pn));
    PnP = (struct PoPStr*)(*(Addr32 + _PnP));
    *(AllBuf.Spi1 + j++) = CSBiaoZhi1;
    *(AllBuf.Spi1 + j++) = CSBiaoZhi2;
    *(AllBuf.Spi1 + j++) = CSBiaoZhi3;
    MovN2Byte(CS4NUM, AllBuf.Spi1 + j, (u8*)(&PnP->F25.UMul));
    j += CS4NUM;

    for(; j < 527; j++)
    {
        *(AllBuf.Spi1 + j) = 0x00;
    }
    AllBuf.Spi1[527] = FlashBufSum();
    if((SW.Para[0]!='P') || (SW.Para[1]!='a'))
    {
        while (1)
        {
            ;
        }
    }
    SPI_FLASH_PageWrite_E(AllBuf.Spi1, P1CSBock4 + Pn);
    SPI_FLASH_PageWrite_E(AllBuf.Spi1, P1CSBock4Bak + Pn);
}


/* 
********************************************************************************
* �� �� ����CS_Save
* ��    �ܣ��ն˲�������FLASH
* ��    ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void CS_Save(void)
{
    u8  i;
    u16 j;

    EW.MoveByte = 'M';

    //;�����ն˲���F1~F10
    if(EW.CSB1 == 0xC1)
    {
        EW.CSB1 = 0x00;
        j = 0;
        *(AllBuf.Spi1 + j++) = CSBiaoZhi1;
        *(AllBuf.Spi1 + j++) = CSBiaoZhi2;
        *(AllBuf.Spi1 + j++) = CSBiaoZhi3;
        
        MovN2Byte(CS1NUM, AllBuf.Spi1 + j, &ZDP.F1.RTS);//;F1~F10
        j += CS1NUM;
       
        for(; j<527; j++)
        {
            *(AllBuf.Spi1 + j) = 0x00;
        }
        
        //;����У��
        AllBuf.Spi1[527] = FlashBufSum();
        
        if((SW.Para[0]!='P') || (SW.Para[1]!='a'))
        {
            while (1)
            {
               ; //;�ȴ�CPU��λ
            }
        }
        
        SPI_FLASH_PageWrite_E(AllBuf.Spi1, CSBock1);  
        SPI_FLASH_PageWrite_E(AllBuf.Spi1, CSBock1Bak);
       
    }

    //;�����ն˲���F11~F23
    if(EW.CSB2 == 0xC2)
    {
        EW.CSB2 = 0x00;
        j = 0;
        *(AllBuf.Spi1 + j++) = CSBiaoZhi1;
        *(AllBuf.Spi1 + j++) = CSBiaoZhi2;
        *(AllBuf.Spi1 + j++) = CSBiaoZhi3;
        
        MovN2Byte(CS2NUM, AllBuf.Spi1 + j, &ZDP.F11.PuNum);
        j += CS2NUM;
       
        for(; j < 527; j++)
        {
            *(AllBuf.Spi1 + j) = 0x00;
        }
        
        AllBuf.Spi1[527] = FlashBufSum();
        if((SW.Para[0]!='P') || (SW.Para[1]!='a'))
        {
            while (1)
            {
                ;
            }
        }      
        SPI_FLASH_PageWrite_E(AllBuf.Spi1, CSBock2);
        SPI_FLASH_PageWrite_E(AllBuf.Spi1, CSBock2Bak);
    }

    //;�����ն˲���F33~F39��F57~F61
    if(EW.CSB3 == 0xC3)
    {
        EW.CSB3 = 0x00;
        j = 0;
        *(AllBuf.Spi1 + j++) = CSBiaoZhi1;
        *(AllBuf.Spi1 + j++) = CSBiaoZhi2;
        *(AllBuf.Spi1 + j++) = CSBiaoZhi3;
        
        MovN2Byte(CS3NUM, AllBuf.Spi1 + j, &ZDP.F33.PortN);
        j += CS3NUM;
             
        for(; j<527; j++)
        {
            *(AllBuf.Spi1 + j) = 0x00;
        }
        
        AllBuf.Spi1[527] = FlashBufSum();
        if((SW.Para[0]!='P') || (SW.Para[1]!='a'))
        {
            while (1)
            {
                ;
            }
        }        
        SPI_FLASH_PageWrite_E(AllBuf.Spi1, CSBock3);
        SPI_FLASH_PageWrite_E(AllBuf.Spi1, CSBock3Bak);       
    }

    //;�������������
    for(i=0; i<PNMAXNUM; i++)
    {
        if(EW.CSB4 & (0x0001<<i))
        {
            CS_PnSave(i);
            EW.CSB4 &= (~(0x0001 << i));//;��¼��ɺ������־λ
        }
    }

   
    //;����F65�������
    if(EW.CSB5 == 0xC5)
    {
        EW.CSB5 = 0x00;
        j = 0;
        *(AllBuf.Spi1 + j++) = CSBiaoZhi1;
        *(AllBuf.Spi1 + j++) = CSBiaoZhi2;
        *(AllBuf.Spi1 + j++) = CSBiaoZhi3;
        MovN2Byte(CS5NUM, AllBuf.Spi1 + j, &TaskP.F65[0].Cyc);
        j += CS5NUM;
       
        for(; j<527; j++)
        {
            *(AllBuf.Spi1 + j) = 0x00;
        }
        
        AllBuf.Spi1[527] = FlashBufSum();
        if((SW.Para[0]!='P') || (SW.Para[1]!='a'))
        {
            while (1)
            {
                ;
            }
        }

        SPI_FLASH_PageWrite_E(AllBuf.Spi1, CSBock5);
        SPI_FLASH_PageWrite_E(AllBuf.Spi1, CSBock5Bak);

    }
    
    //;����F66��F67��F68��F69����
    if(EW.CSB6 == 0xC6)
    {
        EW.CSB6 = 0x00;
        j = 0;
        *(AllBuf.Spi1 + j++) = CSBiaoZhi1;
        *(AllBuf.Spi1 + j++) = CSBiaoZhi2;
        *(AllBuf.Spi1 + j++) = CSBiaoZhi3;
        
        MovN2Byte(CS6NUM-16, AllBuf.Spi1 + j, &TaskP.F66[0].Cyc);
        j += CS6NUM-16;
        MovN2Byte(16, AllBuf.Spi1 + j, &TaskP.F67[0]);
        j += 16;
       
        for(; j< 27; j++)
        {
            *(AllBuf.Spi1 + j) = 0x00;
        }
        
        AllBuf.Spi1[527] = FlashBufSum();
        if((SW.Para[0]!='P') || (SW.Para[1]!='a'))
        {
            while (1)
            {
                ;
            }
        }

        SPI_FLASH_PageWrite_E(AllBuf.Spi1, CSBock6);
        SPI_FLASH_PageWrite_E(AllBuf.Spi1, CSBock6Bak);

    }
    
    //;����F41~F48��F81~F83��F49����
    if(EW.CSB7 == 0xC7)
    {
        EW.CSB7 = 0x00;
        j = 0;
        *(AllBuf.Spi1 + j++) = CSBiaoZhi1;
        *(AllBuf.Spi1 + j++) = CSBiaoZhi2;
        *(AllBuf.Spi1 + j++) = CSBiaoZhi3;
        
        MovN2Byte(CS7NUM_GRP, AllBuf.Spi1 + j, &GRPP.F41.Flag);
        j += CS7NUM_GRP;
        MovNByte(CS7NUM_DCP, AllBuf.Spi1 + j, DCPP.F81.ValueS);
        j += CS7NUM_DCP;
        MovNByte(CS7NUM_CTR, AllBuf.Spi1 + j, F49);
        j += CS7NUM_CTR;
       
        for(; j<527; j++)
        {
            *(AllBuf.Spi1 + j) = 0x00;
        }
        
        AllBuf.Spi1[527] = FlashBufSum();
        if((SW.Para[0]!='P') || (SW.Para[1]!='a'))
        {
            while(1)
            {
                ;
            }
        }

        SPI_FLASH_PageWrite_E(AllBuf.Spi1, CSBock7);
        SPI_FLASH_PageWrite_E(AllBuf.Spi1, CSBock7Bak);

    }
    
    //;�����ն���չ����
    if(EW.CSB8 == 0xC8)
    {
        EW.CSB8 = 0x00;
        j = 0;
        *(AllBuf.Spi1 + j++) = CSBiaoZhi1;
        *(AllBuf.Spi1 + j++) = CSBiaoZhi2;
        *(AllBuf.Spi1 + j++) = CSBiaoZhi3;
        
        MovNByte(4, AllBuf.Spi1 + j, ZDPKZ.F89.QuXian);
        j += 4;
        MovNByte(96, AllBuf.Spi1 + j, ZDPKZ.F97.UserSend);
        j += 96;
        
        for(; j<527; j++)
        {
            *(AllBuf.Spi1 + j) = 0x00;
        }
        
        AllBuf.Spi1[527] = FlashBufSum();
        if((SW.Para[0]!='P') || (SW.Para[1]!='a'))
        {
            while(1)
            {
                ;
            }
        }
        SPI_FLASH_PageWrite_E(AllBuf.Spi1, CSBock8);
        SPI_FLASH_PageWrite_E(AllBuf.Spi1, CSBock8Bak);
    }
    
    //;����F65�������
    if(EW.CSB9 == 0xC9)
    {
        EW.CSB9 = 0x00;
        j = 0;
        *(AllBuf.Spi1 + j++) = CSBiaoZhi1;
        *(AllBuf.Spi1 + j++) = CSBiaoZhi2;
        *(AllBuf.Spi1 + j++) = CSBiaoZhi3;
        MovN2Byte(CS9NUM, AllBuf.Spi1 + j, &TaskP.F65[4].Cyc);
        j += CS9NUM;
       
        for(; j<527; j++)
        {
            *(AllBuf.Spi1 + j) = 0x00;
        }
        
        AllBuf.Spi1[527] = FlashBufSum();
        if((SW.Para[0]!='P') || (SW.Para[1]!='a'))
        {
            while (1)
            {
                ;
            }
        }

        SPI_FLASH_PageWrite_E(AllBuf.Spi1, CSBock9);
        SPI_FLASH_PageWrite_E(AllBuf.Spi1, CSBock9Bak);

    }
    
     //;����F65�������
    if(EW.CSB10 == 0xCA)
    {
        EW.CSB10 = 0x00;
        j = 0;
        *(AllBuf.Spi1 + j++) = CSBiaoZhi1;
        *(AllBuf.Spi1 + j++) = CSBiaoZhi2;
        *(AllBuf.Spi1 + j++) = CSBiaoZhi3;
        MovN2Byte(CS10NUM, AllBuf.Spi1 + j, &TaskP.F66[4].Cyc);
        j += CS10NUM;
       
        for(; j<527; j++)
        {
            *(AllBuf.Spi1 + j) = 0x00;
        }
        
        AllBuf.Spi1[527] = FlashBufSum();
        if((SW.Para[0]!='P') || (SW.Para[1]!='a'))
        {
            while (1)
            {
                ;
            }
        }

        SPI_FLASH_PageWrite_E(AllBuf.Spi1, CSBock10);
        SPI_FLASH_PageWrite_E(AllBuf.Spi1, CSBock10Bak);

    }
    EW.MoveByte = 0;
}

/* 
********************************************************************************
* �� �� ����ZD_CSSAVE_CHK
* ��    �ܣ�����Ƿ���Ҫ���ն˲���д��FLASH
* ��    ������
* ��    �أ��� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void ZD_CSSAVE_CHK(void)
{
    /*if ((EW.CSB1==0xC1) || (EW.CSB2==0xC2) || ((EW.CSB4>0)&&(EW.CSB4<=0x3FF)) || 
        (EW.CSB3==0xC3) || (EW.CSB5==0xC5) || (EW.CSB6==0xC6)||
        (EW.CSB7==0xC7) || (EW.CSB8==0xC8))  //;�ж��Ƿ���Ҫ�����ն˵Ĳ���
    */
    //;����CS9��CS10 Ҳ���Ƕ����ӵ�1,2�������еĸ�4����Ҫ��������--20141226
    if ((EW.CSB1==0xC1) || (EW.CSB2==0xC2) || ((EW.CSB4>0)&&(EW.CSB4<=0x3FF)) || 
        (EW.CSB3==0xC3) || (EW.CSB5==0xC5) || (EW.CSB6==0xC6)||
        (EW.CSB7==0xC7) || (EW.CSB8==0xC8)|| (EW.CSB9==0xC9)|| (EW.CSB10==0xCA))
    {   
        SW.Para[0] = 'P'; SW.Para[1] = 'a';
        CS_Save(); 
        SW.Para[0] = 0;   SW.Para[1] = 0;
    }
}


/* 
********************************************************************************
* �� �� ����CS_Read
* ��    �ܣ���Flash�б�����ն˲�������
* ��    ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void CS_Read(void)
{
    u8 i;
    u16 j;
    u32 *Addr32;
    struct PoPStr  *PnP;

    EW.MoveByte = 'M';
    SW.Para[0] = 'P';
    SW.Para[1] = 'a'; //;������ն˲����Ͳ�����������в���

    //;��1��
    ReadFlash_Verify(CSBock1, CSBock1Bak);
    if((*AllBuf.Spi1==CSBiaoZhi1) && (*(AllBuf.Spi1+1)==CSBiaoZhi2) &&
       (*(AllBuf.Spi1+2)==CSBiaoZhi3))
    {
        j = 3;
        MovN2Byte(CS1NUM, &ZDP.F1.RTS, AllBuf.Spi1 + j); //;F1~F10           
    }
    else
    {
        ZD_CS1_Init(true); //;��ʼ��������1
    }

    //;��2��
    ReadFlash_Verify(CSBock2, CSBock2Bak);
    if((*AllBuf.Spi1==CSBiaoZhi1) && (*(AllBuf.Spi1 + 1)==CSBiaoZhi2) && 
       (*(AllBuf.Spi1+2) == CSBiaoZhi3))
    {
        j = 3;
        MovN2Byte(CS2NUM, &ZDP.F11.PuNum, AllBuf.Spi1 + j);//;F11~F23
    }
    else
    {
        ZD_CS2_Init(); //;��ʼ��������2
    }

    //;��3��
    ReadFlash_Verify(CSBock3, CSBock3Bak);
    if((*AllBuf.Spi1==CSBiaoZhi1) && (*(AllBuf.Spi1+1)==CSBiaoZhi2) &&
       (*(AllBuf.Spi1+2) == CSBiaoZhi3))
    {
        j = 3;
        MovN2Byte(CS3NUM, &ZDP.F33.PortN, AllBuf.Spi1 + j); //;F33~F39,F57~F61
       
    }
    else
    {
        ZD_CS3_Init(); //;��ʼ��������3
    }

    //;��4��
    for(i=0; i<PNMAXNUM; i++)
    {
        Addr32 = (u32*)(*(PnDTAddr + i));
        PnP = (struct PoPStr*)(*(Addr32 + _PnP));

        ReadFlash_Verify(P1CSBock4 + i, P1CSBock4Bak + i);

        if((*AllBuf.Spi1==CSBiaoZhi1) && (*(AllBuf.Spi1+1)==CSBiaoZhi2) && 
           (*(AllBuf.Spi1+2)==CSBiaoZhi3))
        {
            j = 3;
            MovN2Byte(CS4NUM, (u8*)(&PnP->F25.UMul), AllBuf.Spi1 + j); //;F25~F31,F73~F76
        }
        else
        {
            ZD_CS4_Init(i); //;��ʼ��������4
        }
    }

   
    //;��5��
    ReadFlash_Verify(CSBock5, CSBock5Bak);
    if ((*AllBuf.Spi1==CSBiaoZhi1) && (*(AllBuf.Spi1+1)==CSBiaoZhi2) && 
        (*(AllBuf.Spi1+2)==CSBiaoZhi3))
    {
        j = 3;
        MovN2Byte(CS5NUM, &TaskP.F65[0].Cyc, AllBuf.Spi1 + j);
    }
    else
    {
        ZD_CS5_Init(); //;��ʼ��������5
    }

    //;��6��
    ReadFlash_Verify(CSBock6, CSBock6Bak);
    if ((*AllBuf.Spi1==CSBiaoZhi1) && (*(AllBuf.Spi1+1)==CSBiaoZhi2) && 
        (*(AllBuf.Spi1+2)==CSBiaoZhi3))
    {
        j = 3;
        MovN2Byte(CS6NUM-16, &TaskP.F66[0].Cyc, AllBuf.Spi1 + j);
        j += CS6NUM-16;
        MovN2Byte(16, &TaskP.F67[0], AllBuf.Spi1 + j);
    }
    else
    {
        ZD_CS6_Init(); //;��ʼ��������6
    }
    
     //;��7��
    ReadFlash_Verify(CSBock7, CSBock7Bak);
    if ((*AllBuf.Spi1==CSBiaoZhi1) && (*(AllBuf.Spi1+1)==CSBiaoZhi2) && 
    (*(AllBuf.Spi1+2)==CSBiaoZhi3))
    {
        j = 3;
        MovNByte(CS7NUM_GRP, &GRPP.F41.Flag, AllBuf.Spi1 + j); //;F41~F48
        j += CS7NUM_GRP;
        MovNByte(CS7NUM_DCP, DCPP.F81.ValueS, AllBuf.Spi1 + j); //;F81~F83
        j += CS7NUM_DCP;
        MovNByte(CS7NUM_CTR, F49, AllBuf.Spi1 + j); //;F49
        
    }
    else
    {
        ZD_CS7_Init(); //;��ʼ��������7
    }
    
    //;��8��
    ReadFlash_Verify(CSBock8, CSBock8Bak);
    if ((*AllBuf.Spi1==CSBiaoZhi1) && (*(AllBuf.Spi1+1)==CSBiaoZhi2) && 
        (*(AllBuf.Spi1+2)==CSBiaoZhi3))
    {
        j = 3;
        MovNByte(4, ZDPKZ.F89.QuXian, AllBuf.Spi1 + j);
        j += 4;
        MovNByte(96,ZDPKZ.F97.UserSend, AllBuf.Spi1 + j);
        j += 96;
    }
    else
    {
        ZD_CS8_Init(true); //;��ʼ��������8
    }
    
    
    //;��9��
    ReadFlash_Verify(CSBock9, CSBock9Bak);
    if ((*AllBuf.Spi1==CSBiaoZhi1) && (*(AllBuf.Spi1+1)==CSBiaoZhi2) && 
        (*(AllBuf.Spi1+2)==CSBiaoZhi3))
    {
        j = 3;
        MovN2Byte(CS9NUM, &TaskP.F65[4].Cyc, AllBuf.Spi1 + j);
    }
    else
    {
        ZD_CS9_Init(); //;��ʼ��������9
    }
    
    //;��10��
    ReadFlash_Verify(CSBock10, CSBock10Bak);
    if ((*AllBuf.Spi1==CSBiaoZhi1) && (*(AllBuf.Spi1+1)==CSBiaoZhi2) && 
        (*(AllBuf.Spi1+2)==CSBiaoZhi3))
    {
        j = 3;
        MovN2Byte(CS10NUM, &TaskP.F66[4].Cyc, AllBuf.Spi1 + j);
    }
    else
    {
        ZD_CS10_Init(); //;��ʼ��������10
    }

    
    SW.Para[0] = 0;
    SW.Para[1] = 0;
    EW.MoveByte = 0;
}


/* 
********************************************************************************
* �� �� ����DDJ_FTP
* ��    �ܣ����ݸ�����ʱ���ҳ��������ݶ�Ӧ��ʱ������к�
* ��    ����Pn-������ţ�Td_d-��Ҫ��ȡ�������ݵ�ʱ��
* ��    �أ�0xFF--δ�ҵ���ʱ�������� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
u8 DDJ_FTP(u8 Pn, u8 *Td_d)
{
    u8 PointPage, compare;
    s16 Ind_Low, Ind_High, Ind_Mid,index;
    u8 T_Adr[3];
    u16 temp, page, PnDJHome, DJPointNum;
    u32 *Addr32;
    struct DJ_Control *PnDJ;

    index = ZD_Pn_Find(Pn);//;--20141225
    if(index < 0)          //;�����㲻����
    {
      return 0xFF;
    }
    Addr32 =(u32*)(*(PnDTAddr + index));
    PnDJ = (struct DJ_Control*)(*(Addr32 + _PnDDJ));
    PnDJHome = (u16)(*(Addr32 + _PnDDJHome));
    DJPointNum = DDJPointNum; //;�ܹ���ĵ���
    PointPage = DDJPointPage; //;ÿ���ҳ��

    if(PnDJ->Index == PnDJ->StInd)  //;�������ݻ�������
    {
        return 0xFF;
    }
    
    Ind_Low = PnDJ->StInd;
    Ind_High = PnDJ->Index - 1;
    while(Ind_High >= Ind_Low)
    {
        Ind_Mid = (Ind_Low + Ind_High) / 2;
        temp = Ind_Mid % DJPointNum; //;�����������ݱ�����еĽ���λ��
        page = temp * PointPage; //;���ڵ�ҳ��
        page += PnDJHome;
        EW.RFlashNByte = 'R';
        SPI_FLASH_ReadNByte(T_Adr, page, 0, 3); //;������ʼλ�õ�ʱ��
        EW.RFlashNByte = 0;
        
        compare = TimeCompare_3Byte(Td_d, T_Adr);
        if(compare == same)
        {
            return Ind_Mid;
        }
        else if(compare == ahead)
        {
            Ind_High = Ind_Mid - 1;
        }
        else
        {
            Ind_Low = Ind_Mid + 1;
        }
    }

    return 0xFF; //;��������δ�ҵ�

}


/* 
********************************************************************************
* �� �� ����BDJ_FTP
* ��    �ܣ����ݸ�����ʱ���ҳ��������ݶ�Ӧ��ʱ������к�
* ��    ����Pn-������ţ�Td_d-��Ҫ��ȡ�������ݵ�ʱ��
* ��    �أ�0xFF--δ�ҵ���ʱ�������� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
u8 BDJ_FTP(u8 Pn, u8 *Td_d)
{
    u8 PointPage, compare;
    s16 Ind_Low, Ind_High, Ind_Mid, index;
    u8 T_Adr[3];
    u16 temp, page, PnDJHome, DJPointNum;
    u32 *Addr32;
    struct DJ_Control *PnDJ;

    index = ZD_Pn_Find(Pn);
    if( index < 0)  //;--20141225
    	return 0xFF;  //;�����㲻����
    Addr32 =(u32*)(*(PnDTAddr + index));
    PnDJ = (struct DJ_Control*)(*(Addr32 + _PnBDJ));
    PnDJHome = (u16)(*(Addr32 + _PnBDJHome));
    DJPointNum = BDJPointNum;
    PointPage = BDJPointPage;
    if(PnDJ->Index == PnDJ->StInd) //;�������ݻ�������  
    { 
        return 0xFF;
    }

    Ind_Low = PnDJ->StInd;
    Ind_High = PnDJ->Index - 1;
    while(Ind_High >= Ind_Low)  
    {
        Ind_Mid = (Ind_Low + Ind_High) / 2;
        temp = Ind_Mid % DJPointNum; //;�����������ݱ�����еĽ���λ��
        page = temp * PointPage; //;���ڵ�ҳ��
        page += PnDJHome;
        EW.RFlashNByte = 'R';
        SPI_FLASH_ReadNByte(T_Adr, page, 0, 3); //;������ʼλ�õ�ʱ��
        EW.RFlashNByte = 0;
/*        
        if ((T_Adr[0] == 0xFF) || (T_Adr[1] == 0xFF) || (T_Adr[2] == 0xFF))  
        {
            Ind_Low = Ind_Mid;  //;--@6
        }
*/
        compare = TimeCompare_3Byte(Td_d, T_Adr);
        if(compare == same)  
        {
            return Ind_Mid;
        }  
        else if(compare == ahead)  
        {
            Ind_High = Ind_Mid - 1;
        }  
        else  
        {
            Ind_Low = Ind_Mid + 1;
        }
    }

    return 0xFF; //;��������δ�ҵ�
}


/* 
********************************************************************************
* �� �� ����MDJ_FTP
* ��    �ܣ����ݸ�����ʱ���ҳ��������ݶ�Ӧ��ʱ������к�
* ��    ����Pn-������ţ�Td_m-��Ҫ��ȡ�������ݵ�ʱ��
* ��    �أ�0xFF--δ�ҵ���ʱ�������� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��2011-09-23 13:23:11 �޸��㷨
********************************************************************************
*/
u8 MDJ_FTP(u8 Pn, u8 *Td_m)
{
    u8 PointPage, compare;
    s16 Ind_Low, Ind_High, Ind_Mid, index;
    u8 T_Adr[2];
    u16 temp, page, PnDJHome, DJPointNum;
    u32 *Addr32;
    struct DJ_Control *PnDJ;

    index = ZD_Pn_Find(Pn);
    if( index < 0)  //;--20141225
    	return 0xFF;  //;�����㲻����
    Addr32 =(u32*)(*(PnDTAddr + index));
    PnDJ = (struct DJ_Control*)(*(Addr32 + _PnMDJ));
    PnDJHome = (u16)(*(Addr32 + _PnMDJHome));
    DJPointNum = MDJPointNum;
    PointPage = MDJPointPage;
    if (PnDJ->Index == PnDJ->StInd) //;�������ݻ�������  
    { 
        return 0xFF;
    }


    Ind_Low = PnDJ->StInd;
    Ind_High = PnDJ->Index - 1;
    while(Ind_High >= Ind_Low)  
    {
        Ind_Mid = (Ind_Low + Ind_High) / 2;
        temp = Ind_Mid % DJPointNum; //;�����������ݱ�����еĽ���λ��
        page = temp * PointPage; //;���ڵ�ҳ��
        page += PnDJHome;
        EW.RFlashNByte = 'R';
        SPI_FLASH_ReadNByte(T_Adr, page, 0, 2); //;������ʼλ�õ�ʱ��
        EW.RFlashNByte = 0;
       
        compare = TimeCompare_2Byte(Td_m, T_Adr);
        if(compare == same)  
        {
            return Ind_Mid;
        } 
        else if(compare == ahead)  
        {
            Ind_High = Ind_Mid - 1;
        }  
        else  
        {
            Ind_Low = Ind_Mid + 1;
        }
    }

    return 0xFF; //;��������δ�ҵ�

}


/* 
********************************************************************************
* �� �� ����QDJ_FTP
* ��    �ܣ����ݸ�����ʱ���ҳ��������ݶ�Ӧ��ʱ������к�
* ��    ����Pn-������ţ�Time-��Ҫ��ȡ�������ݵ�ʱ��
* ��    �أ�0xFF--δ�ҵ���ʱ�������� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��2011-09-23 13:23:11 �޸��㷨
********************************************************************************
*/
u16 QDJ_FTP(u8 Pn, u8 *Time)
{
    u8 T_Adr[5];
    u8 area_ind,PagePoint, AreaNum, compare;
    s16 Ind_Low, Ind_High, Ind_Mid, index;
    u16 temp, page, PnDJHome, DJPointNum;
    u32 *Addr32;
    struct QX_Control *PnDJ;

    index = ZD_Pn_Find(Pn);
    if( index < 0)  //;--20141225
    	return 0xFF;  //;�����㲻����
    Addr32 =(u32*)(*(PnDTAddr + index));
    PnDJ = (struct QX_Control*)(*(Addr32 + _PnQDJ));
    PnDJHome = (u16)(*(Addr32 + _PnQDJHome));
    DJPointNum = QDJPointNum; //;���߶����ܹ��ĵ�
    PagePoint = QDJPagePoint; //;ÿҳ��ĵ�
    AreaNum = QDJAreaNum;     //;ÿ�����ֽ�
    
    if (PnDJ->Index == PnDJ->StInd)  
    {
        return 0xFFFF;
    }

    Ind_Low = PnDJ->StInd;
    Ind_High = PnDJ->Index - 1;
    while(Ind_High >= Ind_Low)  
    {
        Ind_Mid = (Ind_Low + Ind_High) / 2;
        temp = Ind_Mid % DJPointNum;
        page = temp / PagePoint;
        page += PnDJHome;
        area_ind = temp % PagePoint;
        EW.RFlashNByte = 'R';
        SPI_FLASH_ReadNByte(T_Adr, page, area_ind * AreaNum, 5);
        EW.RFlashNByte = 0;
        
        compare = TimeCompare_5Byte(Time, T_Adr);
        if(compare == same)  
        {
            return Ind_Mid;
        }  
        else if(compare == ahead)  
        {
            Ind_High = Ind_Mid - 1;
        }  
        else  
        {
            Ind_Low = Ind_Mid + 1;
        }
    }

    return 0xFFFF;
}


/* 
********************************************************************************
* �� �� ����DDJ_Read
* ��    �ܣ������������ڵ�; �ҳ����ն�������
* ��    ����Buf_Out-�������ݴ�����飻Buf_Ind-�������
*           Pn-������ţ�Td_d-ʱ��
* ��    �أ�0xFF-���ݲ����ڣ�����Ϊ��ȡ�����ݳ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��2011-09-22 20:27:39
********************************************************************************
*/
u8 DDJ_Read(char *Buf_Out, u16 Buf_Ind, u8 Pn, u8 Fn, u8 *Td_d)
{
    u8  j,DJIndex;
    u16 Buf_Add = 0;
    u8  len, expage, readtime, ratenum, temp;
    u16 page, PnDJHome, stindex;
    u32 *adr32;
    u8  i;
    s16 index;

    DJIndex = DDJ_FTP(Pn, Td_d);
    if(DJIndex == 0xFF)   //;������
    {
        return 0xFF;
    }  

    adr32 = (u32*)(PN0DZU + Fn);
    temp = *((u8*)adr32 + 3); 
    readtime = temp & 0x08;
    ratenum  = temp & 0x10;
    len  = *((u8*)adr32 + 2);
    expage = temp >> 5;
    stindex = *(u16*)(adr32);
    
    index = ZD_Pn_Find(Pn);
    if( index < 0)  //;--20141225
    	return 0xFF;  //;�����㲻����
    adr32 = (u32*)(*(PnDTAddr + index));
    PnDJHome = (u16)(*(adr32 + _PnDDJHome));
    page = (DJIndex %DDJPointNum)* DDJPointPage;
    page += PnDJHome + expage;
    
    
    SPI_FLASH_PageRead(AllBuf.Spi1, page);

    *(Td_d + 0) = *(AllBuf.Spi1 + 0); //;д���ҵ����ݵ�����
    *(Td_d + 1) = *(AllBuf.Spi1 + 1);
    *(Td_d + 2) = *(AllBuf.Spi1 + 2);

    if(readtime)   //;����������
    {  
        *(Buf_Out + Buf_Ind++) = *(AllBuf.Spi1 + 3);
        *(Buf_Out + Buf_Ind++) = *(AllBuf.Spi1 + 4);
        *(Buf_Out + Buf_Ind++) = *(AllBuf.Spi1 + 5);
        *(Buf_Out + Buf_Ind++) = *(AllBuf.Spi1 + 6);
        *(Buf_Out + Buf_Ind++) = *(AllBuf.Spi1 + 7);
        Buf_Add += 5;
    }
    if(ratenum)   //;����������
    {  
        *(Buf_Out + Buf_Ind++) = *(AllBuf.Spi1 + 8);
        Buf_Add += 1;
    }

	Buf_Add += 1; //;����һ���ֽڿռ�  //;20130926
    if(*(AllBuf.Spi1 + 9) == 'T') //�������ʶ
    {
        if(Fn >=5 && Fn <=8)
	      {
	        return 0xFF;
        }
    }
//////////////////////////////////////////////////////// by zrt 20130808 F185-F188��ʽ��Ҫ����
    if(Fn>=185&&Fn<=188)
    {
      for(i=0; i<5; i++)
      {
        for(j=0; j<3; j++)
        {
            *(Buf_Out + Buf_Ind++) = *(AllBuf.Spi1 + stindex + j + 3*i);
        }
        for(j=0; j<4; j++)
        {
            *(Buf_Out + Buf_Ind++) = *(AllBuf.Spi1 + stindex + j + 15 + 4*i);
        }
      }
    }
    else 
    {
      for(j=0; j<len; j++)  
      {
          *(Buf_Out + Buf_Ind++) = *(AllBuf.Spi1 + stindex + j);
      }
    }
////////////////////////////////////////////////////////////
/*    
    for(j=0; j<len; j++)  
    {
        *(Buf_Out + Buf_Ind++) = *(AllBuf.Spi1 + stindex + j);
    }
*/    
    Buf_Add += len;
    
    return Buf_Add;
}


/* 
********************************************************************************
* �� �� ����BDJ_Read
* ��    �ܣ������������� �ҳ������ն�������
* ��    ����Buf_Out-�������ݴ�����飻Buf_Ind-�������
*           Pn-������ţ�Td_d-ʱ��
* ��    �أ�0xFF-���ݲ����ڣ�����Ϊ��ȡ�����ݳ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
u8 BDJ_Read(char *Buf_Out, u16 Buf_Ind, u8 Pn, u8 Fn, u8 *Td_d)
{
    u8 j, DJIndex;
    u16 Buf_Add = 0;
    u8  len, temp, readtime, ratenum, expage;
    u16 page, PnDJHome, stindex;
    u32 *adr32;
    u8 i;
    s16 index;

    DJIndex = BDJ_FTP(Pn, Td_d);
    if (DJIndex == 0xFF)
    {
        return 0xFF; //;������
    }

    adr32 = (u32*)(PN0DZU + Fn);
    temp = *((u8*)adr32 + 3); 
    readtime = temp & 0x08;
    ratenum  = temp & 0x10;
    len  = *((u8*)adr32 + 2);
    expage = temp >> 5;
    stindex = *(u16*)(adr32);

    index = ZD_Pn_Find(Pn);
    if( index < 0)  //;--20141225
    	return 0xFF;  //;�����㲻����
    
    adr32 = (u32*)(*(PnDTAddr + index));
    PnDJHome = (u16)(*(adr32 + _PnBDJHome));
    page = (DJIndex % BDJPointNum) * BDJPointPage;
    page += PnDJHome + expage;
    SPI_FLASH_PageRead(AllBuf.Spi1, page);

    *(Td_d + 0) = *(AllBuf.Spi1 + 0);
    *(Td_d + 1) = *(AllBuf.Spi1 + 1);
    *(Td_d + 2) = *(AllBuf.Spi1 + 2);

    if(readtime)  //;����������
    {
        *(Buf_Out + Buf_Ind++) = *(AllBuf.Spi1 + 3);
        *(Buf_Out + Buf_Ind++) = *(AllBuf.Spi1 + 4);
        *(Buf_Out + Buf_Ind++) = *(AllBuf.Spi1 + 5);
        *(Buf_Out + Buf_Ind++) = *(AllBuf.Spi1 + 6);
        *(Buf_Out + Buf_Ind++) = *(AllBuf.Spi1 + 7);
        Buf_Add += 5;
    }
    if(ratenum)
    {
        *(Buf_Out + Buf_Ind++) = *(AllBuf.Spi1 + 8);   //;����������
        Buf_Add += 1;
    }
//////////////////////////////////////////////////////// by zrt 20130808 F189-F192��ʽ��Ҫ����
    if(Fn>=189&&Fn<=192)
    {
      for(i=0; i<5; i++)
      {
        for(j=0; j<3; j++)
        {
            *(Buf_Out + Buf_Ind++) = *(AllBuf.Spi1 + stindex + j + 3*i);
        }
        for(j=0; j<4; j++)
        {
            *(Buf_Out + Buf_Ind++) = *(AllBuf.Spi1 + stindex + j + 15 + 4*i);
        }
      }
    }
    else 
    {
      for(j=0; j<len; j++)  
      {
          *(Buf_Out + Buf_Ind++) = *(AllBuf.Spi1 + stindex + j);
      }
    }
////////////////////////////////////////////////////////////
/*    
    for(j=0; j<len; j++)  
    {
        *(Buf_Out + Buf_Ind++) = *(AllBuf.Spi1 + stindex + j);
    }
*/
    Buf_Add += len;
    
    return Buf_Add;
}


/* 
********************************************************************************
* �� �� ����MDJ_Read
* ��    �ܣ���������ʱ��, Pn, Fn; �ҳ��¶�������
* ��    ����Buf_Out-�������ݴ�����飻Buf_Ind-�������
*           Pn-������ţ�Td_d-ʱ��
* ��    �أ�0xFF-���ݲ����ڣ�����Ϊ��ȡ�����ݳ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
u8 MDJ_Read(char *Buf_Out, u16 Buf_Ind, u8 Pn, u8 Fn, u8 *Td_m)
{
    u8 j, DJIndex;
    u16 Buf_Add = 0;
    u8 len, expage, temp, readtime, ratenum;
    u16 page, PnDJHome, stindex;
    u32 *adr32;
    u8 i;
    s16 index;

    DJIndex = MDJ_FTP(Pn, Td_m);
    if (DJIndex == 0xFF)  
    {
        return 0xFF; //;������
    }

    adr32 = (u32*)(PN0DZU + Fn);
    temp = *((u8*)adr32 + 3); 
    readtime = temp & 0x08;
    ratenum  = temp & 0x10;
    len  = *((u8*)adr32 + 2);
    expage = temp >> 5;
    stindex = *(u16*)(adr32);

    index = ZD_Pn_Find(Pn);
    if( index < 0)  //;--20141225
    	return 0xFF;  //;�����㲻����
    adr32 = (u32*)(*(PnDTAddr + index));
    PnDJHome = (u16)(*(adr32 + _PnMDJHome));
    page = (DJIndex % MDJPointNum) * MDJPointPage;
    page += PnDJHome + expage;
    
    SPI_FLASH_PageRead(AllBuf.Spi1, page);

    *(Td_m + 0) = *(AllBuf.Spi1 + 0);
    *(Td_m + 1) = *(AllBuf.Spi1 + 1);
    
    if(readtime)   //;����������
    {  
        *(Buf_Out + Buf_Ind++) = *(AllBuf.Spi1 + 2);
        *(Buf_Out + Buf_Ind++) = *(AllBuf.Spi1 + 3);
        *(Buf_Out + Buf_Ind++) = *(AllBuf.Spi1 + 4);
        *(Buf_Out + Buf_Ind++) = *(AllBuf.Spi1 + 5);
        *(Buf_Out + Buf_Ind++) = *(AllBuf.Spi1 + 6);
        Buf_Add += 5;
    }
    if(ratenum) //;���������� 
    { 
        *(Buf_Out + Buf_Ind++) = *(AllBuf.Spi1 + 7);
        Buf_Add += 1;
    }

    if(*(AllBuf.Spi1 + 8) == 'T') //�������ʶ
    {
        if(Fn >=21 && Fn <=24)
	 {
	     return 0xFF;
        }
    }

    Buf_Add += 1; //;����һ���ֽڿռ�  //;20130926	
	
//////////////////////////////////////////////////////// by zrt 20130808 F193-F196��ʽ��Ҫ����
    if(Fn>=193&&Fn<=196)
    {
      for(i=0; i<5; i++)
      {
        for(j=0; j<3; j++)
        {
            *(Buf_Out + Buf_Ind++) = *(AllBuf.Spi1 + stindex + j + 3*i);
        }
        for(j=0; j<4; j++)
        {
            *(Buf_Out + Buf_Ind++) = *(AllBuf.Spi1 + stindex + j + 15 + 4*i);
        }
      }
    }
    else 
    {
      for(j=0; j<len; j++)  
      {
          *(Buf_Out + Buf_Ind++) = *(AllBuf.Spi1 + stindex + j);
      }
    }
////////////////////////////////////////////////////////////
/*    
    for(j=0; j<len; j++)  
    {
        *(Buf_Out + Buf_Ind++) = *(AllBuf.Spi1 + stindex + j);
    }
*/
    
    Buf_Add += len;
    
    return Buf_Add;
}

/* 
********************************************************************************
* �� �� ����QDJ_Read
* ��    �ܣ������������ڣ� �ҳ����߶�������
* ��    ����Buf_Out-�������ݴ�����飻Buf_Ind-�������
*           Pn-������ţ�Td_d-ʱ��
* ��    �أ�0xFF-���ݲ����ڣ�����Ϊ��ȡ�����ݳ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
/*
u8 QDJ_Read(char *Buf_Out, u16 Buf_Ind, u8 Pn, u8 Fn, u8 *Td_c)
{
    u8 j, index, Buf_Add = 0;
    u8 area_ind,  len;
    u16 page, PnDJHome, DJIndex, stindex;
    u32 *adr32;

    DJIndex = QDJ_FTP(Pn, Td_c);
    if(DJIndex == 0xFFFF)   //;������ 
    { 
        return 0xFF;
    }

    adr32 = (u32*)(PN0DZU + Fn); 
    len  = *((u8*)adr32 + 2);
    stindex = *(u16*)(adr32);
    
    index = ZD_Pn_Find(Pn);
    adr32 = (u32*)(*(PnDTAddr + index));
    PnDJHome = (u16)(*(adr32 + _PnQDJHome));
    area_ind = (DJIndex % QDJPointNum) % QDJPagePoint;
    page = (DJIndex % QDJPointNum) / QDJPagePoint;
    page += PnDJHome;    
    EW.RFlashNByte = 'R';
    SPI_FLASH_ReadNByte(AllBuf.Spi1, page, area_ind * QDJAreaNum, QDJAreaNum); //;������������
    EW.RFlashNByte = 0;
    
    *(Td_c + 0) = *(AllBuf.Spi1 + 0);
    *(Td_c + 1) = *(AllBuf.Spi1 + 1);
    *(Td_c + 2) = *(AllBuf.Spi1 + 2);
    *(Td_c + 3) = *(AllBuf.Spi1 + 3);
    *(Td_c + 4) = *(AllBuf.Spi1 + 4);
    
    for(j=0; j<len; j++) //;д���������� 
    {  
        *(Buf_Out + Buf_Ind++) = *(AllBuf.Spi1 + stindex + j);
    }
    Buf_Add += len;
    
    return Buf_Add;
}
*/

u8 QDJ_Read(char *Buf_Out, u16 Buf_Ind, u8 Pn, u8 Fn, u8 *Td_c)
{
	u8 j;
	u16 Buf_Add = 0;
	u8 area_ind,  len;
	u16 page, PnDJHome, DJIndex, stindex;
	u32 *adr32;
	s16 index;
	
	DJIndex = QDJ_FTP(Pn, Td_c);
//	if(DJIndex == 0xFFFF)   //;������ 
//	{ 
//		return 0xFF;
//	}
		
	adr32 = (u32*)(PN0DZU + Fn); 
	len  = *((u8*)adr32 + 2);
	stindex = *(u16*)(adr32);

//	*(Td_c + 0) = *(AllBuf.Spi1 + 0);
//	*(Td_c + 1) = *(AllBuf.Spi1 + 1);
//	*(Td_c + 2) = *(AllBuf.Spi1 + 2);
//	*(Td_c + 3) = *(AllBuf.Spi1 + 3);
//	*(Td_c + 4) = *(AllBuf.Spi1 + 4);
	if(DJIndex == 0xFFFF)   //;qiu_20130516
	{
		for(j=0; j<len; j++) //;д���������� 
		{  
			*(Buf_Out + Buf_Ind++) = 0xee;
		}
	}
	else
	{
		index = ZD_Pn_Find(Pn);
	  if( index < 0)  //;--20141225
    	return 0xFF;  //;�����㲻����	
		adr32 = (u32*)(*(PnDTAddr + index));
		PnDJHome = (u16)(*(adr32 + _PnQDJHome));
		area_ind = (DJIndex % QDJPointNum) % QDJPagePoint;
		page = (DJIndex % QDJPointNum) / QDJPagePoint;
		page += PnDJHome;    
		EW.RFlashNByte = 'R';
		SPI_FLASH_ReadNByte(AllBuf.Spi1, page, area_ind * QDJAreaNum, QDJAreaNum); //;������������
		EW.RFlashNByte = 0;
		for(j=0; j<len; j++) //;д���������� 
		{  
			*(Buf_Out + Buf_Ind++) = *(AllBuf.Spi1 + stindex + j);
		}
	}
	Buf_Add += len;
	
	return Buf_Add;
}


/* 
********************************************************************************
* �� �� ����HDJ_Read
* ��    �ܣ�Сʱ�������ݴ�Flash�ж�����
* ��    ������
* ��    �أ���
* ˵    �����ϵ�ʱ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��2011-09-29 16:37:06 ����ǰһ�����ڼ���,�޸Ķ������ݴ���
********************************************************************************
*/
void HDJ_Read(void)
{
    u8 ch = false;
    u8 i, year=0, month=0, day=0, hour=0;
    u16 j, PnNHDJHome,PnOHDJHome;
    u32 *Addr32;
    struct PointDataStr *PnD;

    for(i=0; i<PNMAXNUM; i++)
    {
        Addr32 = (u32*)(*(PnDTAddr + i));
        PnD = (struct PointDataStr*)(*(Addr32 + _PnD));
        PnNHDJHome = (u16)(*(Addr32 + _PnNHDJHome));
        PnOHDJHome = (u16)(*(Addr32 + _PnOHDJHome));
        
        SPI_FLASH_PageRead(AllBuf.Spi1, PnNHDJHome);      //;������ǰСʱ��������
        if((*AllBuf.Spi1=='H') && (*(AllBuf.Spi1+1)=='D') && 
           (*(AllBuf.Spi1+2)=='J'))
        {
            j = 7;
            EW.MoveByte = 'M';
            MovNByte(56, PnD->NDJ_B630.Td_h,  AllBuf.Spi1 + j);    //;��,A,B,C�й�����
            j += 56;
            MovNByte(56, PnD->NDJ_B640.Td_h,  AllBuf.Spi1 + j);    //;��,A,B,C�޹�����
            j += 56;
            MovNByte(30, PnD->NDJ_B611.Td_h,  AllBuf.Spi1 + j);    //;A,B,C���ѹ
            j += 30;
/*            MovNByte(40, PnD->NDJ_B621.Td_h,  AllBuf.Spi1 + j);    //;A,B,C������Լ����ߵ���  
            j += 40;  */ 
//   by zrt 20130719    ����Ϊ3���ֽڣ������ܹ���Ϊ56���ֽ�
            MovNByte(56, PnD->NDJ_B621.Td_h,  AllBuf.Spi1 + j);    //;A,B,C������Լ����ߵ���
            j += 56;
            MovNByte(72, PnD->NDJ_9010.Td_h,  AllBuf.Spi1 + j);    //;Сʱ�����յ�����
            j += 72;
            MovNByte(72, PnD->NDJ_9010_1.Td_h,  AllBuf.Spi1 + j);  //;Сʱ�����ܵ���ʾֵ
            j += 72;
            MovNByte(40, PnD->NDJ_B650.Td_h,  AllBuf.Spi1 + j);    //;��,A,B,C�๦������

            EW.MoveByte = 0;
        }
        
        SPI_FLASH_PageRead(AllBuf.Spi1, PnOHDJHome);     //;�����������һСʱ��������  
        if((*AllBuf.Spi1=='H') && (*(AllBuf.Spi1+1)=='D') && 
           (*(AllBuf.Spi1+2)=='J'))
        {
            j = 3;
            year = *(AllBuf.Spi1 + j++);
            month = *(AllBuf.Spi1 + j++);
            day = *(AllBuf.Spi1 + j++);
            hour = *(AllBuf.Spi1 + j++);
            EW.MoveByte = 'M';
            MovNByte(56, PnD->ODJ_B630.Td_h,  AllBuf.Spi1 + j);    //;��,A,B,C�й�����
            j += 56;
            MovNByte(56, PnD->ODJ_B640.Td_h,  AllBuf.Spi1 + j);    //;��,A,B,C�޹�����
            j += 56;
            MovNByte(30, PnD->ODJ_B611.Td_h,  AllBuf.Spi1 + j);    //;A,B,C���ѹ
            j += 30;
/*            MovNByte(40, PnD->ODJ_B621.Td_h,  AllBuf.Spi1 + j);    //;A,B,C������Լ����ߵ���
            j += 40;			
*/         
//by zrt 20130719 
            MovNByte(56, PnD->ODJ_B621.Td_h,  AllBuf.Spi1 + j);    //;A,B,C������Լ����ߵ���
            j += 56;
            MovNByte(72, PnD->ODJ_9010.Td_h,  AllBuf.Spi1 + j);    //;Сʱ�����յ�����
            j += 72;
            MovNByte(72, PnD->ODJ_9010_1.Td_h,  AllBuf.Spi1 + j);  //;Сʱ�����ܵ���ʾֵ
            j += 72;
            MovNByte(40, PnD->ODJ_B650.Td_h,  AllBuf.Spi1 + j);    //;��,A,B,C�๦������
            EW.MoveByte = 0;
        }
       
        if((year != THEX.Year) || (month != THEX.Month) || 
           (day != THEX.Day) || (hour != THEX.Hour)) //;�������Ĳ��ǵ�ǰСʱ������
        {
            //;������һ�յ�������
            if(hour >= 23)  //;�췭ת
	  	  	  {
	  	  	  	  hour = 0;	  	  	 	  
	  	  	  	  if(day >= DaysOfMonth[month])  //;�ж��Ƿ����·�ת
	  	  	  	  {
	  	  	  	  	  if(month == 2)  //;���·ݵ��������
	  	  	  	  	  {
	  	  	  	  	  	  if((year % 4) == 0x00)  //;���� --@2
	  	  	  	  	  	  {
	  	  	  	  	  	  	  if(day >= 29 )
	  	  	  	  	  	  	  {
	  	  	  	  	  	  	  	  day = 1;
	  	  	  	  	  	  	  }
	  	  	  	  	  	  	  else   
	  	  	  	  	  	  	  {
	  	  	  	  	  	  	  	  day++;
	  	  	  	  	  	  	  }
	  	  	  	  	  	  }
	  	  	  	  	  	  else
	  	  	  	  	  	  {
	  	  	  	  	  	  	  day = 1;
	  	  	  	  	  	  }
	  	  	  	  	  }
	  	  	  	  	  else
	  	  	  	  	  {
	  	  	  	  	  	  day = 1;
	  	  	  	  	  }
	  	  	  	  	  
	  	  	  	  	  if(day == 1) //;�����·�ת
	  	  	  	  	  {	  	  	  	  	  	      	  	  	  	  	  	      
	  	  	  	  	      if(month >= 12) //;�����귭ת
	  	  	  	  	      {	  	  	  	  	  	          
	  	  	  	  	          month = 1;
	  	  	  	  	          year++;
	  	  	  	  	      }
	  	  	  	  	      else
	  	  	  	  	      {
	  	  	  	  	          month++;
	  	  	  	  	      }
	  	  	  	  	  }
	  	  	  	  }
	  	  	  	  else
	  	  	  	  {
	  	  	  	  	  day++;
	  	  	  	  }
	  	  	  }
            else
            {
                hour++;
            }
            
            if((hour == THEX.Hour) && (year == THEX.Year) && 
                (month == THEX.Month) && (day == THEX.Day)) //;���Ϊ��һ��Сʱ������
            {
                ch = true;
            }           
            
            if(ch == true)  //;������������Ϊ��һ��Сʱ������
            {
                EW.MoveByte = 'M';
                MovNByte(56, PnD->ODJ_B630.Td_h,   PnD->NDJ_B630.Td_h);    //;��,A,B,C�й�����
                MovNByte(56, PnD->ODJ_B640.Td_h,   PnD->NDJ_B640.Td_h);    //;��,A,B,C�޹�����
                MovNByte(30, PnD->ODJ_B611.Td_h,   PnD->NDJ_B611.Td_h);    //;A,B,C���ѹ
//              MovNByte(40, PnD->ODJ_B621.Td_h,   PnD->NDJ_B621.Td_h);    //;A,B,C������Լ����ߵ���  by zrt 20130719
                MovNByte(56, PnD->ODJ_B621.Td_h,   PnD->NDJ_B621.Td_h);    //;A,B,C������Լ����ߵ���                
                MovNByte(72, PnD->ODJ_9010.Td_h,   PnD->NDJ_9010.Td_h);    //;Сʱ�����յ�����
                MovNByte(72, PnD->ODJ_9010_1.Td_h, PnD->NDJ_9010_1.Td_h);  //;Сʱ�����ܵ���ʾֵ
                MovNByte(40, PnD->ODJ_B650.Td_h,   PnD->NDJ_B650.Td_h);    //;��,A,B,C�๦������
                EW.MoveByte = 0;
                HDJ_Reset(i);  //;���¿�ʼһ��Сʱ��Сʱ����ǰ��Сʱ�������ݸ�λΪû�и�����
            }
            else //;������һСʱ���ݣ�Ҳ���ǵ�ǰСʱ����
            {
                ByteN_Fill(366, PnD->ODJ_B630.Td_h, 0x00);               
            }
        }
        HDJ_Pre(i); //;����Сʱ����Ĳ����ѵ�ǰ���������е�Td_h׼����
    }
}


/* 
********************************************************************************
* �� �� ����HDJ_Save
* ��    �ܣ���Сʱ�������ݱ��浽FLASH
* ��    ����Pn-�������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void HDJ_Save(u8 Pn)
{
    u16 j, PnNHDJHome, PnOHDJHome;
    u32 *Addr32;
    struct PointDataStr *PnD;

    Addr32 = (u32*)(*(PnDTAddr + Pn));
    PnD = (struct PointDataStr*)(*(Addr32 + _PnD));
    PnNHDJHome = (u16)(*(Addr32 + _PnNHDJHome));
    PnOHDJHome = (u16)(*(Addr32 + _PnOHDJHome));  
    
    EW.MoveByte = 'M'; //;���浱ǰСʱ����
    j = 0;
    *(AllBuf.Spi1 + j++) = 'H';
    *(AllBuf.Spi1 + j++) = 'D';
    *(AllBuf.Spi1 + j++) = 'J';
    *(AllBuf.Spi1 + j++) = THEX.Year;
    *(AllBuf.Spi1 + j++) = THEX.Month;
    *(AllBuf.Spi1 + j++) = THEX.Day;
    *(AllBuf.Spi1 + j++) = THEX.Hour;

    MovNByte(56,  AllBuf.Spi1 + j, PnD->NDJ_B630.Td_h);    //;��,A,B,C�й�����
    j += 56;
    MovNByte(56,  AllBuf.Spi1 + j, PnD->NDJ_B640.Td_h);    //;��,A,B,C�޹�����
    j += 56;
    MovNByte(30,  AllBuf.Spi1 + j, PnD->NDJ_B611.Td_h);    //;A,B,C���ѹ
    j += 30;
/*	
    MovNByte(40,  AllBuf.Spi1 + j, PnD->NDJ_B621.Td_h);    //;A,B,C������Լ����ߵ���
    j += 40;  
*/
//   by zrt 20130719    ����Ϊ3���ֽڣ������ܹ�40�ֽ�Ӧ��Ϊ56���ֽ�
    MovNByte(56,  AllBuf.Spi1 + j, PnD->NDJ_B621.Td_h);    //;A,B,C������Լ����ߵ���
    j += 56;  
    MovNByte(72,  AllBuf.Spi1 + j, PnD->NDJ_9010.Td_h);    //;Сʱ�����յ�����
    j += 72;
    MovNByte(72,  AllBuf.Spi1 + j, PnD->NDJ_9010_1.Td_h);  //;Сʱ�����ܵ���ʾֵ
    j += 72;
    MovNByte(40,  AllBuf.Spi1 + j, PnD->NDJ_B650.Td_h);    //;��,A,B,C�๦������
    j += 40;

    for(; j < 527; j++)
    {
        *(AllBuf.Spi1 + j) = 0x00; //;���õĵ�ַ���Ϊ0
    }
    
    SPI_FLASH_PageWrite_E(AllBuf.Spi1,PnNHDJHome);

    j = 0; //;������һСʱ����
    *(AllBuf.Spi1 + j++) = 'H';
    *(AllBuf.Spi1 + j++) = 'D';
    *(AllBuf.Spi1 + j++) = 'J';
    *(AllBuf.Spi1 + j++) = THEX.Year;
    *(AllBuf.Spi1 + j++) = THEX.Month;
    *(AllBuf.Spi1 + j++) = THEX.Day;
    *(AllBuf.Spi1 + j++) = THEX.Hour;

    MovNByte(56,  AllBuf.Spi1 + j, PnD->ODJ_B630.Td_h);    //;��һСʱ������,A,B,C�й�����
    j += 56;
    MovNByte(56,  AllBuf.Spi1 + j, PnD->ODJ_B640.Td_h);    //;��һСʱ������,A,B,C�޹�����
    j += 56;
    MovNByte(30,  AllBuf.Spi1 + j, PnD->ODJ_B611.Td_h);    //;��һСʱ����A,B,C���ѹ
    j += 30;
/*	
    MovNByte(40,  AllBuf.Spi1 + j, PnD->ODJ_B621.Td_h);    //;��һСʱ����A,B,C������Լ����ߵ���
    j += 40;
*/
//20130719 by zrt 
    MovNByte(56,  AllBuf.Spi1 + j, PnD->ODJ_B621.Td_h);    //;��һСʱ����A,B,C������Լ����ߵ���
    j += 56;
    MovNByte(72,  AllBuf.Spi1 + j, PnD->ODJ_9010.Td_h);    //;��һСʱ�����յ�����
    j += 72;
    MovNByte(72,  AllBuf.Spi1 + j, PnD->ODJ_9010_1.Td_h);  //;��һСʱ�����ܵ���ʾֵ
    j += 72;
    MovNByte(40,  AllBuf.Spi1 + j, PnD->ODJ_B650.Td_h);    //;��һСʱ������,A,B,C�๦������
    j += 40;

    EW.MoveByte = 0;
    
    for (; j < 527; j++)
    {
        *(AllBuf.Spi1 + j) = 0x00;
    }
    
    SPI_FLASH_PageWrite_E(AllBuf.Spi1, PnOHDJHome);
}

/* 
********************************************************************************
* �� �� ����StatData_Read
* ��    �ܣ��������յ��µ�ͳ������
* ��    ������
* ��    �أ���
* ˵    ����443B+3B+3B
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void MVT_Read(void)
{
    u8 i, Year, Month, Day;
    u16 j, PnMVTHome;
    u32 *Addr32;
    struct PointDataStr *PnD;

    for(i=0; i<PNMAXNUM; i++)
    {
        Addr32 = (u32*)(*(PnDTAddr + i));
        PnD = (struct PointDataStr*)(*(Addr32 + _PnD));
        PnMVTHome = (u16)(*(Addr32 + _PnMVTHome));

        SPI_FLASH_PageRead(AllBuf.Spi1, PnMVTHome);
        if ((*AllBuf.Spi1=='M') && (*(AllBuf.Spi1+1)=='T') && 
            (*(AllBuf.Spi1+2)=='V'))
        {
            j = 3;
            Day  = *(AllBuf.Spi1 + j++);
            Month = *(AllBuf.Spi1 + j++);
            Year  = *(AllBuf.Spi1 + j++);
            if((Year == THEX.Year) && (Month == THEX.Month))
            {
                /*����ǵ���ͳ������*/
                EW.MoveByte = 'M';
                MovNByte(32, PnD->DF33.ZPMax,  AllBuf.Spi1 + j);   //;�����ܼ���������ʼ�����ʱ��  F33
                j += 32;
                MovNByte(24, PnD->DF34.ZPMN, AllBuf.Spi1 + j);    //;�������÷����������������ʱ��  F34
                j += 24;
                MovNByte(66, PnD->DF35.AVUUMin, AllBuf.Spi1 + j);  //;���µ�ѹͳ������  F35
                j += 66;
                MovNByte(16, PnD->DF36.IImbMin, AllBuf.Spi1 + j);//;���²�ƽ��ȳ����ۼ�ʱ�� F36
                j += 16;
                MovNByte(38, PnD->DF37.AIUUMin, AllBuf.Spi1 + j);  //;���µ���ͳ������ F37
                j += 38;
                MovNByte(4,  PnD->DF38.SUUMin, AllBuf.Spi1 + j);   //;�������ڹ���Խ���ۼ�ʱ�� F38
                j += 4;
                MovNByte(12, PnD->DF39.Vmax, AllBuf.Spi1 + j);   //;�¸�����ͳ�� 
                j += 12;
                MovNByte(4,  PnD->DF51.PowerT, AllBuf.Spi1 + j);   //;�¶��ṩ��ʱ���¸�λ�ۼ�
                j += 4;
                MovNByte(4,  PnD->DF52.MonthCutNum, AllBuf.Spi1 + j);   //;�¶������ͳ��
                j += 4;
                MovNByte(4,  (u8*)&ZDS.F10.Flux_Month, AllBuf.Spi1 + j);   //;���� F54
                j += 4;
                MovNByte(14,  PnD->DF213.ProgramNum, AllBuf.Spi1 + j);   //;F213
                j += 14;
                MovNByte(14,  PnD->DF214.CLKChangeNum, AllBuf.Spi1 + j);   //;F214
                j += 14;
                if(Day == THEX.Day)
                {
                    MovNByte(32, PnD->DF25.ZPMax,  AllBuf.Spi1 + j);  
                    j += 32;
                    MovNByte(24, PnD->DF26.ZPMN, AllBuf.Spi1 + j);    
                    j += 24;
                    MovNByte(66, PnD->DF27.AVUUMin, AllBuf.Spi1 + j); 
                    j += 66;
                    MovNByte(14, PnD->DF28.IImbMin, AllBuf.Spi1 + j);
                    j += 14;
                    MovNByte(38, PnD->DF29.AIUUMin, AllBuf.Spi1 + j); 
                    j += 38;
                    MovNByte(4,  PnD->DF30.SUUMin, AllBuf.Spi1 + j); 
                    j += 4;
                    MovNByte(10, PnD->DF31.Vmax, AllBuf.Spi1 + j);   
                    j += 10;
                    MovNByte(4,  PnD->DF49.PowerT, AllBuf.Spi1 + j);   
                    j += 4;
                    MovNByte(4,  PnD->DF50.MonthCutNum, AllBuf.Spi1 + j);   
                    j += 4;
                    MovNByte(4,  (u8*)&ZDS.F10.Flux_Day, AllBuf.Spi1 + j);  
                    j += 4;                    
                    MovNByte(11,  PnD->DF209.MeterPowerStat, AllBuf.Spi1 + j);                
                }
                EW.MoveByte = 0;
            }
        }
    }
}


/* 
********************************************************************************
* �� �� ����MVT_Save
* ��    �ܣ����籣��,ÿСʱ����һ�ε��յ��µ�, Most Value and Time
* ��    ����Pn-��Ϣ��
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void MVT_Save(u8 Pn)
{
    u16 j, PnMVTHome;
    u32 *Addr32;
    struct PointDataStr *PnD;
  
    Addr32 = (u32*)(*(PnDTAddr + Pn));
    PnD = (struct PointDataStr*)(*(Addr32 + _PnD));
    PnMVTHome = (u16)(*(Addr32 + _PnMVTHome));

    j = 0;
    *(AllBuf.Spi1 + j++) = 'M';
    *(AllBuf.Spi1 + j++) = 'T';
    *(AllBuf.Spi1 + j++) = 'V';
    *(AllBuf.Spi1 + j++) = THEX.Day;
    *(AllBuf.Spi1 + j++) = THEX.Month;
    *(AllBuf.Spi1 + j++) = THEX.Year;

    EW.MoveByte = 'M';
    MovNByte(32, AllBuf.Spi1 + j, PnD->DF33.ZPMax);//;�����ܼ���������ʼ�����ʱ��  F33
    j += 32;
    MovNByte(24, AllBuf.Spi1 + j, PnD->DF34.ZPMN);//;�����ܼ������������������ʱ��  F34
    j += 24;
    MovNByte(66, AllBuf.Spi1 + j, PnD->DF35.AVUUMin); //;���µ�ѹͳ������  F35
    j += 66;
    MovNByte(16, AllBuf.Spi1 + j, PnD->DF36.IImbMin);//;���²�ƽ��ȳ����ۼ�ʱ�� F36
    j += 16;
    MovNByte(38, AllBuf.Spi1 + j, PnD->DF37.AIUUMin); //;���µ���ͳ������ F37
    j += 38;
    MovNByte(4, AllBuf.Spi1 + j, PnD->DF38.SUUMin);//;�������ڹ���Խ���ۼ�ʱ�� F38
    j += 4;
    MovNByte(12,AllBuf.Spi1 + j, PnD->DF39.Vmax);   //;�¸�����ͳ�� 
    j += 12;
    MovNByte(4, AllBuf.Spi1 + j,  PnD->DF51.PowerT);   //;�¶��ṩ��ʱ���¸�λ�ۼ�
    j += 4;
    MovNByte(4, AllBuf.Spi1 + j,  PnD->DF52.MonthCutNum);   //;�¶������ͳ��
    j += 4;
    MovNByte(4, AllBuf.Spi1 + j,  (u8*)&ZDS.F10.Flux_Month);   //;���� F54
    j += 4;
    MovNByte(14, AllBuf.Spi1 + j,  PnD->DF213.ProgramNum);   //;F213
    j += 14;
    MovNByte(14, AllBuf.Spi1 + j,  PnD->DF214.CLKChangeNum);   //;F214
    j += 14;
    
    //;��ͳ������
    MovNByte(32,  AllBuf.Spi1 + j, PnD->DF25.ZPMax);  
    j += 32;
    MovNByte(24, AllBuf.Spi1 + j, PnD->DF26.ZPMN);    
    j += 24;
    MovNByte(66, AllBuf.Spi1 + j, PnD->DF27.AVUUMin); 
    j += 66;
    MovNByte(14, AllBuf.Spi1 + j, PnD->DF28.IImbMin);
    j += 14;
    MovNByte(38, AllBuf.Spi1 + j, PnD->DF29.AIUUMin); 
    j += 38;
    MovNByte(4, AllBuf.Spi1 + j,  PnD->DF30.SUUMin); 
    j += 4;
    MovNByte(10, AllBuf.Spi1 + j, PnD->DF31.Vmax);   
    j += 10;
    MovNByte(4, AllBuf.Spi1 + j,  PnD->DF49.PowerT);   
    j += 4;
    MovNByte(4, AllBuf.Spi1 + j,  PnD->DF50.MonthCutNum);   
    j += 4;
    MovNByte(4, AllBuf.Spi1 + j,  (u8*)&ZDS.F10.Flux_Day);  
    j += 4;                    
    MovNByte(11, AllBuf.Spi1 + j,  PnD->DF209.MeterPowerStat); 
    j += 11;
    EW.MoveByte = 0;

    for(; j < 527; j++) //;���õĵ�ַ���Ϊ0
    {
        *(AllBuf.Spi1 + j) = 0x00;
    }
    
    SPI_FLASH_PageWrite_E(AllBuf.Spi1, PnMVTHome);
}



/* 
********************************************************************************
* �� �� ����PEZD_Save
* ��    �ܣ����浱�յ������������ڼ����յ���
* ��    ����Pn-��Ϣ�㣻Date-��������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void PEZD_Save(u8 Pn, u8 *Date)
{
    u16 j, PnPEZHome;
    u32 *Addr32;
    struct PointDataStr *PnD;
    
    Addr32 = (u32*)(*(PnDTAddr + Pn));
    PnD = (struct PointDataStr*)(*(Addr32 + _PnD));
    PnPEZHome = (u16)(*(Addr32 + _PnPEZHome));
    
    SPI_FLASH_PageRead(AllBuf.Spi1, PnPEZHome);  
    
    EW.MoveByte = 'M';
    j = 0;    
    *(AllBuf.Spi1 + j++) = 'P';  //;������һ����ĩ������ʾֵ
    *(AllBuf.Spi1 + j++) = 'E';
    *(AllBuf.Spi1 + j++) = 'Z';
    *(AllBuf.Spi1 + j++) = 'D';
    *(AllBuf.Spi1 + j++) = *(Date + 0);
    *(AllBuf.Spi1 + j++) = *(Date + 1);
    *(AllBuf.Spi1 + j++) = *(Date + 2);
    MovNByte(90, AllBuf.Spi1 + j, PnD->Z9010_D);
    j += 90; 
    SPI_FLASH_PageWrite_E(AllBuf.Spi1, PnPEZHome);
}


/* 
********************************************************************************
* �� �� ����PEZM_Save
* ��    �ܣ����浱�µ������������ڼ����µ���
* ��    ����Pn-��Ϣ�㣻Date-��������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*
void PEZM_Save(u8 Pn, u8 *Date)
{
    u16 j, PnPEZHome;
    u32 *Addr32;
    struct PointDataStr *PnD;

    Addr32 = (u32*)(*(PnDTAddr + Pn));
    PnD = (struct PointDataStr*)(*(Addr32 + _PnD));
    PnPEZHome = (u16)(*(Addr32 + _PnPEZHome));

    SPI_FLASH_PageRead(AllBuf.Spi1, PnPEZHome);
    
    EW.MoveByte = 'M';    
    j = 97;
    *(AllBuf.Spi1 + j++) = 'P';
    *(AllBuf.Spi1 + j++) = 'E';
    *(AllBuf.Spi1 + j++) = 'Z';
    *(AllBuf.Spi1 + j++) = 'M';
    *(AllBuf.Spi1 + j++) = *(Date + 0);
    *(AllBuf.Spi1 + j++) = *(Date + 1);   
    MovNByte(90, AllBuf.Spi1 + j, PnD->Z9010_M);
    j += 90;
    EW.MoveByte = 0;
    for (; j < 527; j++) //;���õĵ�ַ���Ϊ0
    {
        *(AllBuf.Spi1 + j) = 0x00;
    }  
    SPI_FLASH_PageWrite_E(AllBuf.Spi1, PnPEZHome);
}
*/
/* 
********************************************************************************
* �� �� ����PEZM_Save
* ��    �ܣ����浱�µ������������ڼ����µ�������������¼(zrt20130815)
* ��    ����Pn-��Ϣ�㣻Date-��������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void PEZM_Save(u8 Pn, u8 *Date)
{
    u16 j, PnPEZHome;
    u32 *Addr32;
    struct PointDataStr *PnD;

    Addr32 = (u32*)(*(PnDTAddr + Pn));
    PnD = (struct PointDataStr*)(*(Addr32 + _PnD));
    PnPEZHome = (u16)(*(Addr32 + _PnPEZHome));

    SPI_FLASH_PageRead(AllBuf.Spi1, PnPEZHome);
    
    EW.MoveByte = 'M';    
    j = 97;
    *(AllBuf.Spi1 + j++) = 'P';
    *(AllBuf.Spi1 + j++) = 'E';
    *(AllBuf.Spi1 + j++) = 'Z';
    *(AllBuf.Spi1 + j++) = 'M';
    *(AllBuf.Spi1 + j++) = *(Date + 0);
    *(AllBuf.Spi1 + j++) = *(Date + 1);   
    MovNByte(90+28, AllBuf.Spi1 + j, PnD->Z9010_M);
    j += (90+28);
    EW.MoveByte = 0;
    for (; j < 527; j++) //;���õĵ�ַ���Ϊ0
    {
        *(AllBuf.Spi1 + j) = 0x00;
    }  
    SPI_FLASH_PageWrite_E(AllBuf.Spi1, PnPEZHome);
}


/* 
********************************************************************************
* �� �� ����PEZ_Read
* ��    �ܣ�����������,���û�ж������ͽ���ǰֵ��Ϊ�����ܣ���������¼(zrt20130815)
* ��    ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*
void PEZ_Read(void)
{
    u8 i, PEZBZ[4], Year, Month, Day;
    u16 j, PnPEZHome;
    u32 *Addr32;
    struct PointDataStr *PnD;

    for(i = 0; i < PNMAXNUM; i++)
    {
        Addr32 = (u32*)(*(PnDTAddr + i));
        PnD = (struct PointDataStr*)(*(Addr32 + _PnD));
        PnPEZHome = (u16)(*(Addr32 + _PnPEZHome));
        SPI_FLASH_PageRead(AllBuf.Spi1, PnPEZHome); 
        j = 0;
        PEZBZ[0] = *(AllBuf.Spi1  + j++);     
        PEZBZ[1] = *(AllBuf.Spi1  + j++);
        PEZBZ[2] = *(AllBuf.Spi1  + j++);
        PEZBZ[3] = *(AllBuf.Spi1  + j++);        
        if((PEZBZ[0] == 'P') && (PEZBZ[1] == 'E') && (PEZBZ[2] == 'Z') && (PEZBZ[3] == 'D'))
        {
            Day   = *(AllBuf.Spi1 + j++);
            Month = *(AllBuf.Spi1 + j++);
            Year  = *(AllBuf.Spi1 + j++);               
            if ((Day == TBCD.Day) && (Month == TBCD.Month) && (Year == TBCD.Year))
            {
                EW.MoveByte = 'M';
                MovNByte(90, PnD->Z9010_D,  AllBuf.Spi1 + j);
                EW.MoveByte = 0;
            }
        }
        
        j = 97;
        PEZBZ[0] = *(AllBuf.Spi1  + j++);     
        PEZBZ[1] = *(AllBuf.Spi1  + j++);
        PEZBZ[2] = *(AllBuf.Spi1  + j++);
        PEZBZ[3] = *(AllBuf.Spi1  + j++);        
        if((PEZBZ[0] == 'P') && (PEZBZ[1] == 'E') && (PEZBZ[2] == 'Z') && (PEZBZ[3] == 'M'))
        {
            Month = *(AllBuf.Spi1 + j++);
            Year  = *(AllBuf.Spi1 + j++);                   
            if((Month == TBCD.Month) && (Year == TBCD.Year))
            {
                EW.MoveByte = 'M';
                MovNByte(90, PnD->Z9010_M,  AllBuf.Spi1 + j);
                EW.MoveByte = 0;
            }
        }  
    }
}
*/

void PEZ_Read(void)
{
    u8 i, PEZBZ[4], Year, Month, Day;
    u16 j, PnPEZHome;
    u32 *Addr32;
    struct PointDataStr *PnD;

    for(i = 0; i < PNMAXNUM; i++)
    {
        Addr32 = (u32*)(*(PnDTAddr + i));
        PnD = (struct PointDataStr*)(*(Addr32 + _PnD));
        PnPEZHome = (u16)(*(Addr32 + _PnPEZHome));
        SPI_FLASH_PageRead(AllBuf.Spi1, PnPEZHome); 
        j = 0;
        PEZBZ[0] = *(AllBuf.Spi1  + j++);     
        PEZBZ[1] = *(AllBuf.Spi1  + j++);
        PEZBZ[2] = *(AllBuf.Spi1  + j++);
        PEZBZ[3] = *(AllBuf.Spi1  + j++);        
        if((PEZBZ[0] == 'P') && (PEZBZ[1] == 'E') && (PEZBZ[2] == 'Z') && (PEZBZ[3] == 'D'))
        {
            Day   = *(AllBuf.Spi1 + j++);
            Month = *(AllBuf.Spi1 + j++);
            Year  = *(AllBuf.Spi1 + j++);               
            if ((Day == TBCD.Day) && (Month == TBCD.Month) && (Year == TBCD.Year))
            {
                EW.MoveByte = 'M';
                MovNByte(90, PnD->Z9010_D,  AllBuf.Spi1 + j);
                EW.MoveByte = 0;
            }
        }
        
        j = 97;
        PEZBZ[0] = *(AllBuf.Spi1  + j++);     
        PEZBZ[1] = *(AllBuf.Spi1  + j++);
        PEZBZ[2] = *(AllBuf.Spi1  + j++);
        PEZBZ[3] = *(AllBuf.Spi1  + j++);        
        if((PEZBZ[0] == 'P') && (PEZBZ[1] == 'E') && (PEZBZ[2] == 'Z') && (PEZBZ[3] == 'M'))
        {
            Month = *(AllBuf.Spi1 + j++);
            Year  = *(AllBuf.Spi1 + j++);                   
            if((Month == TBCD.Month) && (Year == TBCD.Year))
            {
                EW.MoveByte = 'M';
                MovNByte(90+28, PnD->Z9010_M,  AllBuf.Spi1 + j);
                EW.MoveByte = 0;
            }
        }  
    }
}

/* 
********************************************************************************
* �� �� ����FindIndex_DJ
* ��    �ܣ��ҳ���ǰ��������Ҫ��ŵ�˳���
* ��    ����Type:==0,�ն��᣻==1,�����ն���
* ��    �أ��������ݴ�ŵ����
* ˵    ����ʱ����ǰͻ���ʱ���˳���Ҳ��ǰ�ı�
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
u8 FindIndex_DJ(u8 Pn, u8 Type, u8 *Date)
{
    u8 Ind_Low, Ind_High, Ind_Mid, compare;
    u8 T_Adr[3], DJPointNum, PointPage ;
    u16 temp, page, PnDJHome;
    u32 *Addr32;
    struct DJ_Control *PnDJ;
    
    Addr32 =(u32*)(*(PnDTAddr + Pn));
    if(Type == 0) //;�ն���
    {
        PnDJ = (struct DJ_Control*)(*(Addr32 + _PnDDJ));        
        PnDJHome = (u16)(*(Addr32 + _PnDDJHome));
        DJPointNum = DDJPointNum; //;�ܹ���ĵ���
        PointPage = DDJPointPage; //;ÿ���ҳ��
    }
    else if(Type == 1) //;�����ն���
    {
        PnDJ = (struct DJ_Control*)(*(Addr32 + _PnBDJ));        
        PnDJHome = (u16)(*(Addr32 + _PnBDJHome));
        DJPointNum = BDJPointNum;
        PointPage = BDJPointPage; 
    }
   
    if(PnDJ->Index == PnDJ->StInd)
    {
        return PnDJ->Index;//;�����뿪ʼ����ֻͬ���ڶ�Ϊ0������³���
    }
    
    temp = (PnDJ->Index - 1) % DJPointNum;//;��PointNum�еĵڼ���
    page = temp * PointPage; //;�ڲ����㱣�����ڵ�ҳ��
    page += PnDJHome; //;����Flash�е�ҳ��
    EW.RFlashNByte = 'R'; 
    SPI_FLASH_ReadNByte(T_Adr, page, 0, 3);//;�����������е����һ��ʱ��
    EW.RFlashNByte = 0;
    
    compare = TimeCompare_3Byte(Date, T_Adr);
    if(compare == back)
    {
        return PnDJ->Index;
    }
    else if(compare == same)
    {
        return PnDJ->Index - 1;
    }
    
    temp = PnDJ->StInd % DJPointNum;
    page = temp * PointPage; 
    page += PnDJHome;
    EW.RFlashNByte = 'R';
    SPI_FLASH_ReadNByte(T_Adr, page, 0, 3);
    EW.RFlashNByte = 0;
    
    compare = TimeCompare_3Byte(Date, T_Adr);
    if(compare != back)
    {
        return PnDJ->StInd;
    }
    
    Ind_Low = PnDJ->StInd;
    Ind_High = PnDJ->Index - 1;
    while(Ind_High >= Ind_Low + 2)
    {
        Ind_Mid = (Ind_Low + Ind_High) / 2;
        temp = Ind_Mid % DJPointNum; 
        page = temp * PointPage; 
        page += PnDJHome; 
        EW.RFlashNByte = 'R';
        SPI_FLASH_ReadNByte(T_Adr, page, 0, 3);
        EW.RFlashNByte = 0;
        
        compare = TimeCompare_3Byte(Date, T_Adr);
        if(compare == ahead)
        {
            Ind_High = Ind_Mid;
        } 
        else if(compare == back)
        {
            Ind_Low = Ind_Mid;
        } 
        else if(compare == same)
        {
            return Ind_Mid; 
        }
    }
    return Ind_High;
}



/* 
********************************************************************************
* �� �� ����FindIndex_MDJ
* ��    �ܣ��ҳ���ǰ��������Ҫ��ŵ�˳���
* ��    ����Pn-��Ϣ�㣻Date-��������
* ��    �أ��������ݴ�ŵ���� 
* ˵    ����ʱ����ǰͻ���ʱ���˳���Ҳ��ǰ�ı�
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
u8 FindIndex_MDJ(u8 Pn, u8 * Date)
{
    u8 Ind_Low, Ind_High, Ind_Mid, compare;
    u8 T_Adr[2], DJPointNum, PointPage ;
    u16 temp, page, PnDJHome;
    u32 *Addr32;
    struct DJ_Control *PnDJ;
    
    Addr32 =(u32*)(*(PnDTAddr + Pn));
    
    PnDJ = (struct DJ_Control*)(*(Addr32 + _PnMDJ));        
    PnDJHome = (u16)(*(Addr32 + _PnMDJHome));
    DJPointNum = MDJPointNum;
    PointPage = MDJPointPage; 

    if(PnDJ->Index == PnDJ->StInd)
    {
        return PnDJ->Index;//;�����뿪ʼ����ֻͬ���ڶ�Ϊ0������³���
    }
    
    temp = (PnDJ->Index - 1) % DJPointNum;//;��PointNum�еĵڼ���
    page = temp * PointPage; //;�ڲ����㱣�����ڵ�ҳ��
    page += PnDJHome; //;����Flash�е�ҳ��
    EW.RFlashNByte = 'R'; 
    SPI_FLASH_ReadNByte(T_Adr, page, 0, 2);//;�����������е����һ��ʱ��
    EW.RFlashNByte = 0;
    compare = TimeCompare_2Byte(Date, T_Adr);
    if(compare == back)
    {
        return PnDJ->Index;
    }
    else if(compare == same)
    {
        return (PnDJ->Index - 1);
    }
    
    temp = PnDJ->StInd % DJPointNum;
    page = temp * PointPage; 
    page += PnDJHome;
    EW.RFlashNByte = 'R';
    SPI_FLASH_ReadNByte(T_Adr, page, 0, 2);
    EW.RFlashNByte = 0;
    compare = TimeCompare_2Byte(Date, T_Adr);
    if(compare != back)
    {
        return PnDJ->StInd;
    }
    
    Ind_Low = PnDJ->StInd;
    Ind_High = PnDJ->Index - 1;
    while (Ind_High >= Ind_Low + 2)
    {
        Ind_Mid = (Ind_Low + Ind_High) / 2;
        temp = Ind_Mid % DJPointNum; 
        page = temp * PointPage; 
        page += PnDJHome; 
        
        EW.RFlashNByte = 'R';
        SPI_FLASH_ReadNByte(T_Adr, page, 0, 2);
        EW.RFlashNByte = 0;
        compare = TimeCompare_2Byte(Date, T_Adr);
        if(compare == ahead)
        {
            Ind_High = Ind_Mid;
        } 
        else if(compare == back)
        {
            Ind_Low = Ind_Mid;
        } 
        else if(compare == same)
        {
            return Ind_Mid;
        }
    }
    return Ind_High;
}



/* 
********************************************************************************
* �� �� ����FindIndex_CDJ
* ��    �ܣ��ҳ���ǰ��������Ҫ��ŵ�˳���
* ��    ����Pn-��Ϣ��
* ��    �أ����ݴ�����
* ˵    ����ʱ����ǰͻ���ʱ���˳���Ҳ��ǰ�ı�
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
u16 FindIndex_CDJ(u8 Pn)
{
    u8 area_ind, PagePoint, AreaNum, compare;
    u8 T_Adr[5] ;
    u16 Ind_Low, Ind_High, Ind_Mid;
    u16 temp, page, PnDJHome, DJPointNum;
    u32 *Addr32;
    struct QX_Control *PnDJ;
    
    Addr32 =(u32*)(*(PnDTAddr + Pn));
    PnDJ = (struct QX_Control*)(*(Addr32 + _PnQDJ));  
    PnDJHome = (u16)(*(Addr32 + _PnQDJHome));
    DJPointNum = QDJPointNum;   //;���߶����ܹ��ĵ�
    PagePoint = QDJPagePoint; //;ÿҳ��ĵ�
    AreaNum = QDJAreaNum;     //;ÿ�����ֽ�
    
    if(PnDJ->Index == PnDJ->StInd)
    {
        return PnDJ->Index;
    }
    
    temp = (PnDJ->Index - 1) % DJPointNum; 
    page = temp / PagePoint;
    page += PnDJHome;
    area_ind = temp % PagePoint; //;�ڸ�ҳ�ڵĵڼ�����
    EW.RFlashNByte = 'R'; //;���ڱ���
    SPI_FLASH_ReadNByte(T_Adr, page, area_ind * AreaNum, 5); //;�����������е����һ��ʱ��
    EW.RFlashNByte = 0;
    compare = TimeCompare_5Byte(&TBCD.Min, T_Adr);
    if(compare == back)
    {
        return PnDJ->Index;
    }
    else if(compare == same)
    {
        return (PnDJ->Index - 1);
    }
    
    temp = PnDJ->StInd % DJPointNum;
    page = temp / PagePoint;
    page += PnDJHome;
    area_ind = temp % PagePoint;
    EW.RFlashNByte = 'R'; 
    SPI_FLASH_ReadNByte(T_Adr, page, area_ind * AreaNum, 5);
    EW.RFlashNByte = 0;
    
    compare = TimeCompare_5Byte(&TBCD.Min, T_Adr);
    if(compare != back)
    {
        return PnDJ->StInd;
    }
    
    Ind_Low = PnDJ->StInd;
    Ind_High = PnDJ->Index - 1;
    while(Ind_High >= Ind_Low + 2)
    {
        
        Ind_Mid = (Ind_Low + Ind_High) / 2;
        temp = Ind_Mid % DJPointNum; 
        page = temp / PagePoint; 
        page += PnDJHome;
        area_ind = temp % PagePoint; 
        EW.RFlashNByte = 'R'; 
        SPI_FLASH_ReadNByte(T_Adr, page, area_ind * AreaNum, 5);
        EW.RFlashNByte = 0;
        compare = TimeCompare_5Byte(&TBCD.Min, T_Adr);
        if(compare == ahead)
        {
            Ind_High = Ind_Mid;
        }
        else if(compare == back)
        {
            Ind_Low = Ind_Mid;
        }
        else if(compare == same)
        {
            return Ind_Mid;
        }
    }
    return Ind_High;
}


/* 
********************************************************************************
* �� �� ����DDJ_Save
* ��    �ܣ����ն������ݱ��浽Flash��
* ��    ����Pn-��Ϣ�㣻Date-��������
* ��    �أ���
* ˵    �����ն�������һ������100��,ÿ��1���㣬����2ҳ������200ҳ
*           Flash�еĵ�ַ�� P1DDJ.Index ���������ݱ������Ժ�Ӱ�����ֵ
*           ���Ұ����ֵ���浽FM24C04��ȥ
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void DDJ_Save(u8 Pn, u8 *Date)
{
    u8  Index, StInd, MaxPo, Points, flag ;
    u16 page, j, PnDJHome, E2R_PnDJ;
    u32 *Addr32;
    struct PointDataStr *PnD;
    struct DJ_Control   *PnDJ;

    Addr32 = (u32*)(*(PnDTAddr + Pn));
    PnD = (struct PointDataStr*)(*(Addr32 + _PnD));
    PnDJ = (struct DJ_Control*)(*(Addr32 + _PnDDJ));
    PnDJHome = (u16)(*(Addr32 + _PnDDJHome));
    E2R_PnDJ =(u16)(*(Addr32 + _E2R_PnDDJ));
    
    PnDJ->Index = FindIndex_DJ(Pn, 0, Date);//;���ն���

    j = 0;
    *(AllBuf.Spi1 + j++) = *(Date + 0);
    *(AllBuf.Spi1 + j++) = *(Date + 1);
    *(AllBuf.Spi1 + j++) = *(Date + 2);
    EW.MoveByte = 'M';
    MovNByte(5, AllBuf.Spi1 + j, PnD->CB_Time); //;���泭��ʱ��
    j += 5;
    *(AllBuf.Spi1 + j++) = 4;

    *(AllBuf.Spi1 + j++) = DDLDJPn.Status;	
//	j++;	//;����һ���ֽڵĿռ� //;20130926
    //;F1
    MovNByte(25, AllBuf.Spi1 + j, PnD->R9010);
    j += 25;
    MovNByte(20, AllBuf.Spi1 + j, PnD->R9110);
    j += 20;
    MovNByte(20, AllBuf.Spi1 + j, PnD->R9130);
    j += 20;
    MovNByte(20, AllBuf.Spi1 + j, PnD->R9140);
    j += 20;
    //;F2
    MovNByte(25, AllBuf.Spi1 + j, PnD->R9020);
    j += 25;
    MovNByte(20, AllBuf.Spi1 + j, PnD->R9120);
    j += 20;
    MovNByte(20, AllBuf.Spi1 + j, PnD->R9150);
    j += 20;
    MovNByte(20, AllBuf.Spi1 + j, PnD->R9160);
    j += 20;
    //;F3
    MovNByte(15, AllBuf.Spi1 + j, PnD->RA010);
    j += 15;
    MovNByte(20, AllBuf.Spi1 + j, PnD->RB010);
    j += 20;
    MovNByte(15, AllBuf.Spi1 + j, PnD->RA110);
    j += 15;
    MovNByte(20, AllBuf.Spi1 + j, PnD->RB110);
    j += 20;
    //;F4
    MovNByte(15, AllBuf.Spi1 + j, PnD->RA020);
    j += 15;
    MovNByte(20, AllBuf.Spi1 + j, PnD->RB020);
    j += 20;
    MovNByte(15, AllBuf.Spi1 + j, PnD->RA120);
    j += 15;
    MovNByte(20, AllBuf.Spi1 + j, PnD->RB120);
    j += 20;
    //;F5
    MovNByte(20, AllBuf.Spi1 + j, PnD->R9010_D);
    j += 20;
    //;F6
    MovNByte(20, AllBuf.Spi1 + j, PnD->R9110_D);
    j += 20;
    //;F7
    MovNByte(20, AllBuf.Spi1 + j, PnD->R9020_D);
    j += 20;
    //;F8
    MovNByte(20, AllBuf.Spi1 + j, PnD->R9120_D);
    j += 20;
    //;F25
    MovNByte(32, AllBuf.Spi1 + j, PnD->DF25.ZPMax);
    j += 32;
    //;F26
    MovNByte(24, AllBuf.Spi1 + j, PnD->DF26.ZPMN);
    j += 24;
    //;F27
    MovNByte(66, AllBuf.Spi1 + j, PnD->DF27.AVUUMin);
    j += 66;    
    
    for (; j < 528; j++)  //;���õĵ�ַ���Ϊ0
    {
        *(AllBuf.Spi1 + j) = 0x00;
    }

    page = PnDJ->Index % DDJPointNum; //;pageΪʵ�ʵ����
    page *= DDJPointPage;
    page += PnDJHome; //;pageΪʵ�ʶ�Ӧ��ҳ���еĵ�һҳ
    SPI_FLASH_PageWrite_E(AllBuf.Spi1, page);

    j = 0;
    *(AllBuf.Spi1 + j++) = *(Date + 0);
    *(AllBuf.Spi1 + j++) = *(Date + 1);
    *(AllBuf.Spi1 + j++) = *(Date + 2);
    MovNByte(5, AllBuf.Spi1 + j, PnD->CB_Time); //;���泭��ʱ��
    j += 5;

    *(AllBuf.Spi1 + j++) = 4; //;������
    
    MovNByte(14, AllBuf.Spi1 + j, PnD->DF28.IImbMin);//;F28
    j += 14; 
    MovNByte(38, AllBuf.Spi1 + j, PnD->DF29.AIUUMin);//;F29
    j += 38;  
    MovNByte(4, AllBuf.Spi1 + j,  PnD->DF30.SUUMin);//;F30
    j += 4;
    MovNByte(10, AllBuf.Spi1 + j, PnD->DF31.Vmax); //;F31
    j += 10;   
    MovNByte(52, AllBuf.Spi1 + j, PnD->RB310); //;F32
    j += 52;
    MovNByte(4, AllBuf.Spi1 + j, PnD->DF49.PowerT); //;F49
    j += 4;   
    MovNByte(4, AllBuf.Spi1 + j, PnD->DF50.MonthCutNum); //;F50
    j += 4;   
    MovNByte(4, AllBuf.Spi1 + j, (u8*)&ZDS.F10.Flux_Day); //;F53
    j += 4;  
    //;F153
    MovNByte(5, AllBuf.Spi1 + j, PnD->R30A1); 
    j += 5; 
    MovNByte(5, AllBuf.Spi1 + j, PnD->R30B1); 
    j += 5;  
    MovNByte(5, AllBuf.Spi1 + j, PnD->R30C1); 
    j += 5;
     //;F154
    MovNByte(4, AllBuf.Spi1 + j, PnD->R30A3); 
    j += 4; 
    MovNByte(4, AllBuf.Spi1 + j, PnD->R30B3); 
    j += 4;  
    MovNByte(4, AllBuf.Spi1 + j, PnD->R30C3); 
    j += 4;
     //;F155
    MovNByte(5, AllBuf.Spi1 + j, PnD->R30A2); 
    j += 5; 
    MovNByte(5, AllBuf.Spi1 + j, PnD->R30B2); 
    j += 5;  
    MovNByte(5, AllBuf.Spi1 + j, PnD->R30C2); 
    j += 5;
     //;F156
    MovNByte(4, AllBuf.Spi1 + j, PnD->R30A4); 
    j += 4; 
    MovNByte(4, AllBuf.Spi1 + j, PnD->R30B4); 
    j += 4;  
    MovNByte(4, AllBuf.Spi1 + j, PnD->R30C4); 
    j += 4;
    
    MovNByte(11, AllBuf.Spi1 + j, PnD->DF209.MeterPowerStat); //;F209
    j += 11;   
    for (; j < 528; j++)
    //;���õĵ�ַ���Ϊ0
    {
        *(AllBuf.Spi1 + j) = 0x00;
    }

    page = PnDJ->Index % DDJPointNum; //;pageΪʵ�ʵ���� --@6
    page *= DDJPointPage;
    page += PnDJHome;

    page++; //;pageΪʵ�ʶ�Ӧ��ҳ���еĵڶ�ҳ
    
    SPI_FLASH_PageWrite_E(AllBuf.Spi1, page);

    /*���㷨��������*/
    Index = PnDJ->Index;//;��β
    StInd = PnDJ->StInd;//;��ͷ
    MaxPo = DDJPointNum;//;������
    Points = 1;         //;����
    Index++;
    if((Index - StInd) >= MaxPo)
    {
        StInd = Index - MaxPo + Points;
        flag = (u8)(StInd % Points);
        StInd -= flag;
    }
    if(StInd >= MaxPo)
    {
        StInd -= MaxPo;
        Index -= MaxPo;
    }
    PnDJ->Index = Index;//;��β
    PnDJ->StInd = StInd;//;��ͷ
    SaveData_FM24(2, E2R_PnDJ, E2R_PnDJ + E2R_ByteNum, &PnDJ->Index);
    EW.MoveByte = 0;
}


/* 
********************************************************************************
* �� �� ����BDJ_Save
* ��    �ܣ��ѳ����ն������ݱ��浽Flash��
* ��    ����Pn-��Ϣ�㣻Date-��������
* ��    �أ���
* ˵    �����ն�������һ������100��,ÿ��1���㣬����2ҳ������200ҳ
*           Flash�еĵ�ַ�� P1DDJ.Index ���������ݱ������Ժ�Ӱ�����ֵ
*           ���Ұ����ֵ���浽FM24C04��ȥ
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void BDJ_Save(u8 Pn, u8 *Date)
{
    u8  Index, StInd, MaxPo, Points, flag;
    u16 page, j, PnDJHome, E2R_PnDJ;
    u32 *Addr32;
    struct PointDataStr *PnD;
    struct DJ_Control   *PnDJ;
    
    Addr32 = (u32*)(*(PnDTAddr + Pn));
    PnD = (struct PointDataStr*)(*(Addr32 + _PnD));
    PnDJ = (struct DJ_Control*)(*(Addr32 + _PnBDJ));
    PnDJHome = (u16)(*(Addr32 + _PnBDJHome));
    E2R_PnDJ =(u16)(*(Addr32 + _E2R_PnBDJ));
    
    PnDJ->Index = FindIndex_DJ(Pn, 1, Date);//;�ҳ����ն���

    j = 0;
    *(AllBuf.Spi1 + j++) = *(Date + 0);
    *(AllBuf.Spi1 + j++) = *(Date + 1);
    *(AllBuf.Spi1 + j++) = *(Date + 2);
    EW.MoveByte = 'M';
    MovNByte(5, AllBuf.Spi1 + j, PnD->CB_Time); //;���泭��ʱ��
    j += 5;
    *(AllBuf.Spi1 + j++) = 4;
    //;*(AllBuf.Spi1 + j++) = PnD->RC313[0];       //;������
    //;F9
    MovNByte(25, AllBuf.Spi1 + j, PnD->R9010);
    j += 25;
    MovNByte(20, AllBuf.Spi1 + j, PnD->R9110);
    j += 20;
    MovNByte(20, AllBuf.Spi1 + j, PnD->R9130);
    j += 20;
    MovNByte(20, AllBuf.Spi1 + j, PnD->R9140);
    j += 20;
    //;F10
    MovNByte(25, AllBuf.Spi1 + j, PnD->R9020);
    j += 25;
    MovNByte(20, AllBuf.Spi1 + j, PnD->R9120);
    j += 20;
    MovNByte(20, AllBuf.Spi1 + j, PnD->R9150);
    j += 20;
    MovNByte(20, AllBuf.Spi1 + j, PnD->R9160);
    j += 20;
    //;F11
    MovNByte(15, AllBuf.Spi1 + j, PnD->RA010);
    j += 15;
    MovNByte(20, AllBuf.Spi1 + j, PnD->RB010);
    j += 20;
    MovNByte(15, AllBuf.Spi1 + j, PnD->RA110);
    j += 15;
    MovNByte(20, AllBuf.Spi1 + j, PnD->RB110);
    j += 20;
    //;F12
    MovNByte(15, AllBuf.Spi1 + j, PnD->RA020);
    j += 15;
    MovNByte(20, AllBuf.Spi1 + j, PnD->RB020);
    j += 20;
    MovNByte(15, AllBuf.Spi1 + j, PnD->RA120);
    j += 15;
    MovNByte(20, AllBuf.Spi1 + j, PnD->RB120);
    j += 20;
    for (; j < 528; j++)
    {
        *(AllBuf.Spi1 + j) = 0x00;
    }
    page = PnDJ->Index % BDJPointNum;
    page *= BDJPointPage;
    page += PnDJHome;               //;pageΪʵ�ʶ�Ӧ��ҳ���еĵ�һҳ   
    SPI_FLASH_PageWrite_E(AllBuf.Spi1, page);
    /*---------------���㷨��������------------*/
    Index  =  PnDJ->Index; //;��β
    StInd  =  PnDJ->StInd; //;��ͷ
    MaxPo  =  DDJPointNum; //;������
    Points =  1;           //;����

    Index++;
    if((Index - StInd) >= MaxPo)
    {
        StInd = Index - MaxPo + Points;
        flag = (u8)(StInd % Points);
        StInd -= flag;
    }
    if(StInd >= MaxPo)
    {
        StInd -= MaxPo;
        Index -= MaxPo;
    }
    PnDJ->Index = Index;   //;��β
    PnDJ->StInd = StInd;   //;��ͷ
    /*-----------------------------------------*/
    SaveData_FM24(2, E2R_PnDJ, E2R_PnDJ + E2R_ByteNum, &PnDJ->Index);
    EW.MoveByte = 0;
}


/* 
********************************************************************************
* �� �� ����BDJ_Save
* ��    �ܣ����¶������ݱ��浽Flash��
* ��    ����Pn-��Ϣ�㣻Date-��������
* ��    �أ��� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void MDJ_Save(u8 Pn, u8 *Date)
{
    u8  Index, StInd, MaxPo, Points, flag ;
    u16 page, j, PnDJHome, E2R_PnDJ;
    u32 *Addr32;
    struct PointDataStr *PnD;
    struct DJ_Control   *PnDJ;
    
    Addr32 = (u32*)(*(PnDTAddr + Pn));
    PnD = (struct PointDataStr*)(*(Addr32 + _PnD));
    PnDJ = (struct DJ_Control*)(*(Addr32 + _PnMDJ));
    PnDJHome = (u16)(*(Addr32 + _PnMDJHome));
    E2R_PnDJ =(u16)(*(Addr32 + _E2R_PnMDJ));
    
    PnDJ->Index = FindIndex_MDJ(Pn, Date);

    j = 0;
    *(AllBuf.Spi1 + j++) = *(Date + 0);
    *(AllBuf.Spi1 + j++) = *(Date + 1);
    EW.MoveByte = 'M';
    MovNByte(5, AllBuf.Spi1 + j, PnD->CB_Time); //;���泭��ʱ��
    j += 5;
    *(AllBuf.Spi1 + j++) = 4;

    *(AllBuf.Spi1 + j++) = MDLDJPn.Status;	
//	j++;	//;����һ���ֽڵĿռ� //;20130926
	
    //;*(AllBuf.Spi1 + j++) = PnD->RC313[0]; //;������
    //;F17
    MovNByte(25, AllBuf.Spi1 + j, PnD->R9010);
    j += 25;
    MovNByte(20, AllBuf.Spi1 + j, PnD->R9110);
    j += 20;
    MovNByte(20, AllBuf.Spi1 + j, PnD->R9130);
    j += 20;
    MovNByte(20, AllBuf.Spi1 + j, PnD->R9140);
    j += 20;
    //;F18
    MovNByte(25, AllBuf.Spi1 + j, PnD->R9020);
    j += 25;
    MovNByte(20, AllBuf.Spi1 + j, PnD->R9120);
    j += 20;
    MovNByte(20, AllBuf.Spi1 + j, PnD->R9150);
    j += 20;
    MovNByte(20, AllBuf.Spi1 + j, PnD->R9160);
    j += 20;
    //;F19
    MovNByte(15, AllBuf.Spi1 + j, PnD->RA010);
    j += 15;
    MovNByte(20, AllBuf.Spi1 + j, PnD->RB010);
    j += 20;
    MovNByte(15, AllBuf.Spi1 + j, PnD->RA110);
    j += 15;
    MovNByte(20, AllBuf.Spi1 + j, PnD->RB110);
    j += 20;
    //;F20
    MovNByte(15, AllBuf.Spi1 + j, PnD->RA020);
    j += 15;
    MovNByte(20, AllBuf.Spi1 + j, PnD->RB020);
    j += 20;
    MovNByte(15, AllBuf.Spi1 + j, PnD->RA120);
    j += 15;
    MovNByte(20, AllBuf.Spi1 + j, PnD->RB120);
    j += 20;
    //;F21
    MovNByte(20, AllBuf.Spi1 + j, PnD->R9010_M);
    j += 20;
    //;F22
    MovNByte(20, AllBuf.Spi1 + j, PnD->R9110_M);
    j += 20;
    //;F23
    MovNByte(20, AllBuf.Spi1 + j, PnD->R9020_M);
    j += 20;
    //;F24
    MovNByte(20, AllBuf.Spi1 + j, PnD->R9120_M);
    j += 20;
    //;F33
    MovNByte(32, AllBuf.Spi1 + j, PnD->DF33.ZPMax);
    j += 32;
    //;F34
    MovNByte(24, AllBuf.Spi1 + j, PnD->DF34.ZPMN);
    j += 24;
    //;F35
    MovNByte(66, AllBuf.Spi1 + j, PnD->DF35.AVUUMin);
    j += 66;
    
    for (; j < 528; j++)//;���õĵ�ַ���Ϊ0
    {
        *(AllBuf.Spi1 + j) = 0x00;
    }

    page = PnDJ->Index % MDJPointNum; //;pageΪʵ�ʵ����
    page *= MDJPointPage;
    page += PnDJHome; //;pageΪʵ�ʶ�Ӧ��ҳ���еĵ�һҳ
    SPI_FLASH_PageWrite_E(AllBuf.Spi1, page);

    j = 0;
    *(AllBuf.Spi1 + j++) = *(Date + 0);
    *(AllBuf.Spi1 + j++) = *(Date + 1);
    MovNByte(5, AllBuf.Spi1 + j, PnD->CB_Time); //;���泭��ʱ��
    j += 5;

    *(AllBuf.Spi1 + j++) = 4; //;������

    MovNByte(16, AllBuf.Spi1 + j, PnD->DF36.IImbMin);//;F36
    j += 16; 
    MovNByte(38, AllBuf.Spi1 + j, PnD->DF37.AIUUMin);//;F37
    j += 38;  
    MovNByte(4, AllBuf.Spi1 + j,  PnD->DF38.SUUMin);//;F38
    j += 4;
    MovNByte(12, AllBuf.Spi1 + j, PnD->DF39.Vmax); //;F39
    j += 12;     
    MovNByte(4, AllBuf.Spi1 + j, PnD->DF51.PowerT); //;F51
    j += 4;   
    MovNByte(4, AllBuf.Spi1 + j, PnD->DF52.MonthCutNum); //;F52
    j += 4;   
    MovNByte(4, AllBuf.Spi1 + j, (u8*)&ZDS.F10.Flux_Month); //;F54
    j += 4;  
    //;F157
    MovNByte(5, AllBuf.Spi1 + j, PnD->R30A1); 
    j += 5; 
    MovNByte(5, AllBuf.Spi1 + j, PnD->R30B1); 
    j += 5;  
    MovNByte(5, AllBuf.Spi1 + j, PnD->R30C1); 
    j += 5;
     //;F158
    MovNByte(4, AllBuf.Spi1 + j, PnD->R30A3); 
    j += 4; 
    MovNByte(4, AllBuf.Spi1 + j, PnD->R30B3); 
    j += 4;  
    MovNByte(4, AllBuf.Spi1 + j, PnD->R30C3); 
    j += 4;
     //;F159
    MovNByte(5, AllBuf.Spi1 + j, PnD->R30A2); 
    j += 5; 
    MovNByte(5, AllBuf.Spi1 + j, PnD->R30B2); 
    j += 5;  
    MovNByte(5, AllBuf.Spi1 + j, PnD->R30C2); 
    j += 5;
     //;F160
    MovNByte(4, AllBuf.Spi1 + j, PnD->R30A4); 
    j += 4; 
    MovNByte(4, AllBuf.Spi1 + j, PnD->R30B4); 
    j += 4;  
    MovNByte(4, AllBuf.Spi1 + j, PnD->R30C4); 
    j += 4;
    //;F201~F208
    MovNByte(25, AllBuf.Spi1 + j, PnD->R5010); 
    j += 25;
    MovNByte(25, AllBuf.Spi1 + j, PnD->R5020); 
    j += 25;
    MovNByte(25, AllBuf.Spi1 + j, PnD->R5030); 
    j += 25;
    MovNByte(25, AllBuf.Spi1 + j, PnD->R5040); 
    j += 25;
    MovNByte(25, AllBuf.Spi1 + j, PnD->R5050); 
    j += 25;
    MovNByte(25, AllBuf.Spi1 + j, PnD->R5060); 
    j += 25;
    MovNByte(25, AllBuf.Spi1 + j, PnD->R5070); 
    j += 25;
    MovNByte(25, AllBuf.Spi1 + j, PnD->R5080); 
    j += 25;
   
    MovNByte(14, AllBuf.Spi1 + j, PnD->DF213.ProgramNum); //;F213
    j += 14;  
    MovNByte(14, AllBuf.Spi1 + j, PnD->DF214.CLKChangeNum); //;F214
    j += 14; 
    for (; j < 528; j++)
    //;���õĵ�ַ���Ϊ0
    {
        *(AllBuf.Spi1 + j) = 0x00;
    }

    page = PnDJ->Index % MDJPointNum; //;pageΪʵ�ʵ���� --@6
    page *= MDJPointPage;
    page += PnDJHome; //;pageΪʵ�ʶ�Ӧ��ҳ���еĵ�һҳ
    page++;   
    SPI_FLASH_PageWrite_E(AllBuf.Spi1, page);


    /*---------------------------���㷨��������-----*/
    Index  =  PnDJ->Index; //;��β
    StInd  =  PnDJ->StInd; //;��ͷ
    MaxPo  =  MDJPointNum; //;������
    Points =  1;           //;����

    Index++;
    if((Index - StInd) >= MaxPo)
    {
      StInd = Index - MaxPo + Points;
      flag = (u8)(StInd % Points);
      StInd -= flag;
    }
    if(StInd >= MaxPo)
    {
      StInd -= MaxPo;
      Index -= MaxPo;
    }

    PnDJ->Index = Index;     //;��β
    PnDJ->StInd = StInd;     //;��ͷ
    /*----------------------------------------------*/
    SaveData_FM24(2, E2R_PnDJ, E2R_PnDJ + E2R_ByteNum, &PnDJ->Index);
    EW.MoveByte = 0;
}



/* 
********************************************************************************
* �� �� ����CDJ_Save_Pre
* ��    �ܣ����������������д��������(�ն�Ϊ��1��������)
* ��    ����Pn-��Ϣ�㣻Date-�������ڣ�Buf-��������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void CDJ_Save_Pre(u8 *Buf, u8 Pn, u8 *Date)
{
    u8 j = 0;
    u32 *Addr32;
    struct PointDataStr *PnD;

    Addr32 =(u32*)(*(PnDTAddr + Pn));
    PnD = (struct PointDataStr*)(*(Addr32 + _PnD));

    EW.MoveByte = 'M';
    *(Buf + j++) = *(Date + 0);
    *(Buf + j++) = *(Date + 1);
    *(Buf + j++) = *(Date + 2);
    *(Buf + j++) = *(Date + 3);
    *(Buf + j++) = *(Date + 4);
    MovNByte(12, Buf + j, PnD->RB630);   //;F81-F84  �й�����
    j += 12;
    MovNByte(12, Buf + j, PnD->RB640);   //;F85-F88  �޹�����
    j += 12;
    MovNByte(6,  Buf + j, PnD->RB611); //;F89-F91  ��ѹ
    j += 6;
    MovNByte(12,  Buf + j, PnD->RB621);   //;F92-F95  �����������
    j += 12;
    MovNByte(4, Buf + j, PnD->R9010_D);  //;F97
    j += 4;
    MovNByte(4, Buf + j, PnD->R9110_D);  //;F98
    j += 4;
    MovNByte(4, Buf + j, PnD->R9020_D);  //;F99
    j += 4;
    MovNByte(4, Buf + j, PnD->R9120_D);  //;F100
    j += 4;
    MovNByte(4, Buf + j, PnD->R9010 + 1);    //;F101
    j += 4;
    MovNByte(4, Buf + j, PnD->R9110);    //;102
    j += 4;
    MovNByte(4, Buf+  j, PnD->R9020 + 1);    //;103
    j += 4;
    MovNByte(4, Buf + j, PnD->R9120);    //;104
    j += 4;
    MovNByte(8, Buf + j, PnD->RB650);    //;F105-F108
    j += 8;
    MovNByte(12, Buf + j, PnD->RB671);    //;F109-F110
    j += 12;
    MovNByte(4, Buf + j, PnD->R9130);    //;F145
    j += 4;
    MovNByte(4, Buf + j, PnD->R9140);    //;F146
    j += 4;
    MovNByte(4, Buf + j, PnD->R9150);    //;F147
    j += 4;
    MovNByte(4, Buf + j, PnD->R9160);    //;F148
    j += 4;
    
    *(Buf + j++) = 0;
    *(Buf + j++) = 0;
    *(Buf + j++) = 0;
    *(Buf + j++) = 0;
    *(Buf + j++) = 0;
    EW.MoveByte = 0;
}

/* 
********************************************************************************
* �� �� ����CDJ_Save
* ��    �ܣ������߶������ݱ��浽Flash��
* ��    ����Pn-��Ϣ�㣻Date-��������
* ��    �أ���
* ˵    ����Flash�еĵ�ַ�� P1QX.Index ���� �����ݱ������Ժ�Ӱ�����ֵ
*           ���Ұ����ֵ���浽FM24C04��ȥ
*           88�ֽ�һ���㣬ÿҳ����6���㣬������9600���㣬Ҫ1600ҳ
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void CDJ_Save(u8 Pn, u8 *Date)
{
    u8 Points, flag, PagePoint, AreaNum ;
    u16 page, j, temp, ZIndex, DJPointNum;
    u16 Index, StInd, MaxPo, PnDJHome, E2R_PnDJ;
    u32 *Addr32;
//;    struct PointDataStr *PnD;
    struct QX_Control *PnDJ;
    
    Addr32 =(u32*)(*(PnDTAddr + Pn));
//;    PnD = (struct PointDataStr*)(*(Addr32 + _PnD));

//;    /*��ǰ���ݴ���ʱ���ܶ���*/
//;    if ((PnD->CB_Time[0] != 0) || (PnD->CB_Time[1] != 0) || (PnD->CB_Time[2] != 0) ||
//;        (PnD->CB_Time[3] != 0) || (PnD->CB_Time[4] != 0))
//;    {
        PnDJ = (struct QX_Control*)(*(Addr32 + _PnQDJ));
        PnDJHome = (u16)(*(Addr32 + _PnQDJHome));
        E2R_PnDJ =(u16)(*(Addr32 + _E2R_PnQDJ));
        DJPointNum = QDJPointNum; //;���߶����ܹ��ĵ�
        PagePoint = QDJPagePoint; //;ÿҳ��ĵ�
        AreaNum = QDJAreaNum;     //;ÿ�����ֽ�

        PnDJ->Index = FindIndex_CDJ(Pn);

        temp = PnDJ->Index % DJPointNum;
        page = temp / PagePoint;
        page += PnDJHome;
        
        ZIndex = temp % PagePoint;
        SPI_FLASH_PageRead(AllBuf.Spi1, page);
        for(j = ZIndex * AreaNum; j < (ZIndex + 1) * AreaNum; j++)
        {
            if(*(AllBuf.Spi1 + j) != 0xff)
            {
                break;
            }
        }
        CDJ_Save_Pre(AllBuf.Spi1 + ZIndex * AreaNum, Pn, Date);
        if (j >= (ZIndex + 1) * AreaNum)  //;�Ѿ���ʽ��
        {
            SPI_FLASH_PageWrite_NE(AllBuf.Spi1, page); //;�����ݱ��浽flash��
        }
        else //;��û�и�ʽ��
        {
            for (j = (ZIndex + 1) * AreaNum; j < PAGESIZE; j++)
            {
                *(AllBuf.Spi1 + j) = 0xff;
            }
            SPI_FLASH_PageWrite_E(AllBuf.Spi1, page); //;�����ݱ��浽flash��
        }

        /*---------------------------���㷨��������-----*/
        Index  =  PnDJ->Index; //;��β
        StInd  =  PnDJ->StInd; //;��ͷ
        MaxPo  =  DJPointNum;  //;������
        Points =  PagePoint;   //;����

        Index++;
        if((Index - StInd) >= MaxPo)
        {
            StInd = Index - MaxPo + Points;
            flag = (u8)(StInd % Points);
            StInd -= flag;
        }
        if(StInd >= MaxPo)
        {
            StInd -= MaxPo;
            Index -= MaxPo;
        }

        PnDJ->Index = Index;     //;��β
        PnDJ->StInd = StInd;     //;��ͷ
        /*----------------------------------------------*/
        SaveData_FM24(4, E2R_PnDJ, E2R_PnDJ + E2R_ByteNum, (u8*)(&PnDJ->Index));
//;    }
}


/* 
********************************************************************************
* �� �� ����ERC_Save
* ��    �ܣ�������¼����� 264�� ÿҳ8�� ����33ҳ
* ��    ����Len-�¼����ݳ��ȣ�Data_Add-���ݵ���ʼ��ַ
*             ERC-��¼�¼�����  P1ERC1 Ϊ��Ҫ�¼�   P1ERC2 Ϊһ���¼�
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void ERC_Save(u8 Len, char *Data_Add, u8 Type)
{
    u8  Points, flag, AreaNum,PagePoint;
    u16 i, j, Ind_S, ERCHome, ERCIndex;
    u16 page, temp, area_ind;
    u16 Index, StInd, MaxPo;

    PagePoint = ERCPagePoint;
    AreaNum = ERCAreaNum;

    if(Type == 1)
    {
        ERCHome = ERC1Home;
        ERCIndex = ERC1.Index;
    }
    else if(Type == 0)
    {
        ERCHome = ERC2Home;
        ERCIndex = ERC2.Index;
    }
    temp = ERCIndex % ERCPointNum;
    page = ERCHome + temp / PagePoint;
    
    area_ind = temp % PagePoint;
    SPI_FLASH_PageRead( AllBuf.Spi1, page);
    for(i = area_ind * AreaNum; i < (area_ind + 1) *AreaNum; i++)
    {
        if (*(AllBuf.Spi1 + i) != 0xff)
        {
            break;
        }
    }
    Ind_S = area_ind * AreaNum;
    for(j = 0; j < Len; j++)
    {
        *(AllBuf.Spi1 + Ind_S + j) = *(Data_Add + j);
    }
    if(i >= (area_ind + 1) * AreaNum)
    //;�Ѿ���ʽ��
    {
        SPI_FLASH_PageWrite_NE(AllBuf.Spi1, page);
       
    }
    else
    //;��û�и�ʽ��
    {
        for(i = (area_ind + 1) * AreaNum; i < PAGESIZE; i++)
        {
            *(AllBuf.Spi1 + i) = 0xff;
        }        
        SPI_FLASH_PageWrite_E(AllBuf.Spi1, page);
    }

    if(Type == 1)
    {
        Index  =  ERC1.Index;   //;��β
        StInd  =  ERC1.StInd;   //;��ͷ
    }
    else if(Type == 0)
    {
        Index  =  ERC2.Index;   //;��β
        StInd  =  ERC2.StInd;   //;��ͷ
    }

    MaxPo  =  ERCPointNum;   //;������
    Points =  ERCPagePoint;  //;����

    /*---------------------------���㷨��������-----*/
    Index++;
    if((Index - StInd) >= MaxPo)
    {
        StInd = Index - MaxPo + Points;
        flag = (u8)(StInd % Points);
        StInd -= flag;
    }
    if(StInd >= MaxPo)
    {
        StInd -= MaxPo;
        Index -= MaxPo;
    }
    /*----------------------------------------------*/
    if(Type == 1)
    {
        ERC1.Index = Index;     //;��β
        ERC1.StInd = StInd;     //;��ͷ
        SaveData_FM24(4, E2R_ERC1, E2R_ERC1 + E2R_ByteNum, (u8*)(&ERC1.Index));
    }
    else if(Type == 0)
    {
        ERC2.Index = Index;     //;��β
        ERC2.StInd = StInd;     //;��ͷ
        SaveData_FM24(4, E2R_ERC2, E2R_ERC2 + E2R_ByteNum, (u8*)(&ERC2.Index));
    }
}

