/*
********************************************************************************
*  �� �� ����I2C_FM24.c
*
*  �ļ�������FM24CL04�����ӿ�
*
*  ����оƬ��FM24CL04
*
*  �� �� �ˣ�023
*
*  �� �� �ţ�
*
*  �޸ļ�¼��2013-04-09 10:57:03
********************************************************************************
*/

#include "BSP_CFG.h"

#define I2C_WP_LOW()   GPIO_ResetBits(GPIOB, GPIO_Pin_5)
#define I2C_WP_HIGH()  GPIO_SetBits(GPIOB, GPIO_Pin_5)

/* 
********************************************************************************
* �� �� ����Chk_StAns
* ��    �ܣ�
* ��    ����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
u8 Chk_StAns(void)
{
    u16 err = 0;
    
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
    {
        err++;
        if(err >= 2000)
        {
            return false;
        }
    }
    return true;
}

/* 
********************************************************************************
* �� �� ����Chk_DevAns
* ��    �ܣ�
* ��    ����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
u8 Chk_DevAns(void)
{
    u16 err = 0;
    
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        err++;
        if(err >= 2000)
        {
            return false;
        }
    }
    return true;
}

/* 
********************************************************************************
* �� �� ����Chk_DataAns
* ��    �ܣ�
* ��    ����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
u8 Chk_DataAns(void)
{
    u16 err = 0;
    
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
        err++;
        if(err >= 2000)
        {
            return false;
        }
    }
    return true;
}

/* 
********************************************************************************
* �� �� ����I2C_EE_WaitEepromStandbyState
* ��    �ܣ�
* ��    ����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void I2C_EE_WaitEepromStandbyState(void) 
{
    vu16 SR1_Tmp = 0;
    u16 err = 0; 
    
    do
    {
        //;���� I2Cx���� START����
        I2C_GenerateSTART(I2C1, ENABLE); 

        //;��ȡָ���� I2C�Ĵ��� I2C_SR1 ��������ֵ
        SR1_Tmp = I2C_ReadRegister(I2C1, I2C_Register_SR1); 
        
        //;��ָ���Ĵ� I2C�豸���͵�ַ�� ,ѡ���ͷ���
        I2C_Send7bitAddress(I2C1, 0xA0, I2C_Direction_Transmitter); 
        SR1_Tmp = I2C_ReadRegister(I2C1, I2C_Register_SR1);
        err++;
        if(err >= 2000)
        {
            return ;
        }
    }
    while(!(I2C_ReadRegister(I2C1, I2C_Register_SR1) &0x0002)); //;��ַ���ͽ���

    //;��� I2Cx��Ӧ������־λ
    I2C_ClearFlag(I2C1, I2C_FLAG_AF); 
}


/* 
********************************************************************************
* �� �� ����WriteM16_I2C1
* ��    �ܣ�ͨ��I2C1��AT24C04д�����16���ֽ�
* ��    ����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
u8 WriteM16_I2C1(u8 n, u16 adr, u8 *adr_ram)
{
    u16 devies;
    u8 i, num;

    if(n > 16)
    {
        n = 16;
    }
    
//    if(adr &0x100)
//    {
//        devies = 0xa2;
//    }
//    else
//    {
//        devies = 0xa0;
//    }
    
    devies = (adr & 0x100)?(0xa2):(0xa0);
    for(num = 0; num < 5; num++)
    {
        I2C_WP_LOW();
        I2C_EE_WaitEepromStandbyState();
        I2C_GenerateSTART(I2C1, ENABLE);
        if(EW.W16 != 'W')
        {
            return false; //;���������д���ݣ����˳�
        }
        if(!Chk_StAns())
        {
            continue;
        }
        I2C_Send7bitAddress(I2C1, devies, I2C_Direction_Transmitter);
        if(!Chk_DevAns())
        {
            continue;
        }
        I2C_SendData(I2C1, (u8)(adr));
//;        I2C_SendData(I2C1, (u8)((adr >> 8) << 1));
        if(Chk_DataAns() == false)
        {
            continue;
        }
        for(i = 0; i < n; i++)
        {
            I2C_SendData(I2C1, *(adr_ram + i));
            if(Chk_DataAns() == false)
            {
                break;
            }
        }
        if(i != n)
        {
            continue;
        }
        I2C_GenerateSTOP(I2C1, ENABLE);
        I2C_WP_HIGH();;
        EW.W16 = 0; //;������д�洢��

        return true;
    }
    I2C_GenerateSTOP(I2C1, ENABLE);
    I2C_WP_HIGH();;
    EW.W16 = 0; //;������д�洢��
    return false;
}


/* 
********************************************************************************
* �� �� ����
* ��    �ܣ�ͨ��I2C1��FM24CL04д��N���ֽ�
* ��    ����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
u8 WriteNByte_I2C1(u8 n, u16 adr, u8 *adr_ram)
{
    u8 i, j = 0, k, l;

    i = n;
    
    if(i == 0)
    {
        return true;
    }
    while(i > 16)
    {
        k = 5;
        while(--k)
        {
            for(l = 0; l < 16; l++)
            {
                *(AllBuf.I2C_1 + l) = *(adr_ram + j + l);
            }
            
            if(EW.WFM != 'W')
            {
                return false;
            }
            
            EW.W16 = 'W'; //;����EEPROMд�����16���ֽ�
            if(WriteM16_I2C1(16, adr + j, AllBuf.I2C_1) == false)
            {
                EW.WFM = 0;
                return false;
            }
            
            EW.R16 = 'R'; //;����EEPROM�������16���ֽ�
            if(ReadM16_I2C1(16, adr + j, AllBuf.I2C_2) == false)
            {
                EW.WFM = 0;
                return false;
            }
            
            if(Compare_NByte(16, AllBuf.I2C_1, AllBuf.I2C_2) == true)
            {
                i -= 16;
                j += 16;
                break;
            }
        }
        if(k == 0)
        {
            EW.WFM = 0;
            return false;
        }
    }

    k = 5;
    while(--k)
    {
        for(l = 0; l < i; l++)
        {
            *(AllBuf.I2C_1 + l) = *(adr_ram + j + l);
        }
        
        if (EW.WFM != 'W')
        {
            return false;
        }
        EW.W16 = 'W'; //;����EEPROMд�����16���ֽ�
        if(WriteM16_I2C1(i, adr + j, AllBuf.I2C_1) == false)
        {
            EW.WFM = 0;
            return false;
        }
        EW.R16 = 'R';

        if(ReadM16_I2C1(i, adr + j, AllBuf.I2C_2) == false)
        {
            EW.WFM = 0;
            return false;
        }

        if(Compare_NByte(i, AllBuf.I2C_1, AllBuf.I2C_2) == true)
        {
            break;
        }
    }
    if(k == 0)
    {
        EW.WFM = 0;
        return false;
    }
    else
    {
        return true;
    }
}


/* 
********************************************************************************
* �� �� ����ReadM16_I2C1
* ��    �ܣ�ͨ��I2C1��AT24C04����N���ֽ�
* ��    ����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
u8 ReadM16_I2C1(u8 n, u16 adr, u8 *adr_ram)
{
    u16 devies;
    u8 i = 0;
    u16 err;
    u8 num;
    
    if(n < 1)
    {
        return true;
    }
    //;��������ֽ���Ϊ0��������
    if (n > 16)
    {
        n = 16;
    }

    devies = (adr & 0x100)?(0xa2):(0xa0);
    for(num = 0; num < 5; num++)
    {
        I2C_EE_WaitEepromStandbyState();

        I2C_GenerateSTART(I2C1, ENABLE); //;��ʼλ
        if(Chk_StAns() == false)
        {
            continue;
        }
        if(n == 1)
        {
            I2C_AcknowledgeConfig(I2C1, DISABLE);
        } //;���ֻ��һ���ֽ�
        I2C_Send7bitAddress(I2C1, devies, I2C_Direction_Transmitter);
        if(Chk_DevAns() == false)
        {
            continue;
        }
        I2C_Cmd(I2C1, ENABLE);
        I2C_SendData(I2C1, (u8)(adr));
        if(Chk_DataAns() == false)
        {
            continue;
        }
        I2C_GenerateSTART(I2C1, ENABLE);
        if(Chk_StAns() == false)
        {
            continue;
        }
        I2C_Send7bitAddress(I2C1, devies, I2C_Direction_Receiver);
        err = 0;
        while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) //;д�������ĵ�ַ
        {
            err++;
            if (err > 2000)
            {
                break;
            }
        }
        err = 0;
        while(n)
        {
            if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))
            {
                *(adr_ram + i) = I2C_ReceiveData(I2C1);
                if (n == 2)
                {
                    I2C_AcknowledgeConfig(I2C1, DISABLE);
                }
                if(n == 1)
                {
                    I2C_GenerateSTOP(I2C1, ENABLE);
                }
                i++;
                n--;
                err = 0;
            }
            else
            {
                err++;
                if(err > 2000)
                {
                    break;
                }
            }
        }
        if(n != 0)
        {
            continue;
        }
        I2C_AcknowledgeConfig(I2C1, ENABLE);
        return true;
    }
    return false;
}


/* 
********************************************************************************
* �� �� ����ReadNByte_I2C1
* ��    �ܣ�ͨ��I2C1��FM24C04д��N���ֽ�
* ��    ����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
u8 ReadNByte_I2C1(u8 n, u16 adr, u8 *adr_ram)
{
    u8 i, j = 0, k, l;

    i = n;
    
    if(i == 0)
    {
        return true;
    }
    
    while(i > 16)
    {
        k = 5;
        while(--k)
        {
            if(EW.RFM != 'R')
            {
                return false;
            }
            EW.R16 = 'R'; //;����EEPROM�������16���ֽ�

            if(ReadM16_I2C1(16, adr + j, AllBuf.I2C_1) == false)
            {
                EW.RFM = 0;
                return false;
            }

            EW.R16 = 'R'; //;����EEPROM�������16���ֽ�
            if(ReadM16_I2C1(16, adr + j, AllBuf.I2C_2) == false)
            {
                EW.RFM = 0;
                return false;
            }
            if(Compare_NByte(16, AllBuf.I2C_1, AllBuf.I2C_2) == true)
            {
                for(l = 0; l < 16; l++)
                {
                    *(adr_ram + j + l) = AllBuf.I2C_1[l];
                }
                i -= 16;
                j += 16;
                break;
            }
        }
        if(k == 0)
        {
            EW.RFM = 0;
            return (false);
        }
    }
    k = 5;
    while(--k)
    {
        if (EW.RFM != 'R')
        {
            return false;
        }
        EW.R16 = 'R'; //;����EEPROM�������16���ֽ�
        if (ReadM16_I2C1(i, adr + j, AllBuf.I2C_1) == false)
        {
            EW.RFM = 0;
            return (false);
        }

        EW.R16 = 'R'; //;����EEPROM�������16���ֽ�
        if (ReadM16_I2C1(i, adr + j, AllBuf.I2C_2) == false)
        {
            EW.RFM = 0;
            return (false);
        }
        if (Compare_NByte(i, AllBuf.I2C_1, AllBuf.I2C_2) == true)
        {
            for (l = 0; l < i; l++)
            {
                *(adr_ram + j + l) = AllBuf.I2C_1[l];
            }
            break;
        }
    }
    if(k == 0)
    {
        EW.RFM = 0;
        return false;
    }
    else
    {
        return false;
    }

}

/* 
********************************************************************************
* �� �� ����SaveData_FM24
* ��    �ܣ����ݺ�����У���뱣�浽�洢��������λ��
* ��    ����n--      ���ݳ��ȣ�������У����
*           EEAdr1 EEAdr2  �洢��������λ��
*           adr_ram        Ҫ�������ݵ���ʼ��ַ
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void SaveData_FM24(u8 n, u16 EEAdr1, u16 EEAdr2, u8 *adr_ram)
{
    u8 i;

    AllBuf.Spi1[n] = 0x55;
    for(i = 0; i < n; i++)
    {
        AllBuf.Spi1[i] = *(adr_ram + i);
        AllBuf.Spi1[n] += *(adr_ram + i);
    }

    EW.WFM = 'W';
    WriteNByte_I2C1(n + 1, EEAdr1, AllBuf.Spi1);
    WriteNByte_I2C1(n + 1, EEAdr2, AllBuf.Spi1);
    EW.WFM = 0;

}

/* 
********************************************************************************
* �� �� ����ReadData_FM24
* ��    �ܣ������ݴӴ洢��������λ�ö������ҳ���ȷ������
* ��    ����n    --        ���ݳ��ȣ�������У����
*           EEAdr1 EEAdr2  �洢��������λ��
*           adr_ram        Ҫ�������ݵ���ʼ��ַ
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void ReadData_FM24(u8 n, u16 EEAdr1, u16 EEAdr2, u8 *adr_ram)
{
    u8 i, ch;

    EW.RFM = 'R';
    ReadNByte_I2C1(n + 1, EEAdr1, AllBuf.Spi1);
    ch = 0x55;
    for(i = 0; i < n; i++)
    {
        ch += AllBuf.Spi1[i];
    }
    if(ch == AllBuf.Spi1[n])
    {
        for(i = 0; i < n; i++)
        {
            *(adr_ram + i) = AllBuf.Spi1[i];
        }
        return ;
    }
    else
    {
        ReadNByte_I2C1(n + 1, EEAdr2, AllBuf.Spi1);
        ch = 0x55;
        for(i = 0; i < n; i++)
        {
            ch += AllBuf.Spi1[i];
        }
        if(ch == AllBuf.Spi1[n])
        {
            for(i = 0; i < n; i++)
            {
                *(adr_ram + i) = AllBuf.Spi1[i];
            }
            return ;
        }
        else
        {
            for(i = 0; i < n; i++)
            {
                *(adr_ram + i) = 0;
            }
        }
    }
    EW.RFM = 0;
}
