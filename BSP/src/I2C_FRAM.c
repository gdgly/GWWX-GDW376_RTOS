/*
********************************************************************************
*  �� �� ���� I2C_FRAM.c
*
*  �ļ������� MB85RC04V����������
*
*  ����оƬ�� MB85RC04V
*
*  �� �� �ˣ� simon
*
*  �� �� �ţ� 1.00
*
*  �޸ļ�¼�� 1. ������2014-12-10 10:52:17
********************************************************************************
*/

/* ��Ҫ��ͷ�ļ� */
#include "stm32f10x.h"
#include "I2C_FRAM.h"

/* оƬ��غ궨�� */
#define		FRAM_ADDR		0xA0
#define		FRAM_READ		0x01
#define		FRAM_WRITE		0x00
#define		FRAM_UPPER_ADDR		0x02
#define		FRAM_LOWER_ADDR		0x00

//;SCL����
#define		I2C_SCL_H		GPIO_SetBits(GPIOB, GPIO_Pin_6)
#define		I2C_SCL_L		GPIO_ResetBits(GPIOB, GPIO_Pin_6)
//;SDA����
#define		I2C_SDA_H		GPIO_SetBits(GPIOB, GPIO_Pin_7)
#define		I2C_SDA_L		GPIO_ResetBits(GPIOB, GPIO_Pin_7)
#define		I2C_GET_SDA		GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)
//;WP����
#ifdef I2C_USE_WP
	#define		I2C_WP_H		GPIO_SetBits(GPIOB, GPIO_Pin_5)
	#define		I2C_WP_L		GPIO_ResetBits(GPIOB, GPIO_Pin_5)
#else
	#define		I2C_WP_H
	#define		I2C_WP_L
#endif


/* ���� */
#ifdef I2C_STM32_SYSCLK
	static u32 SYSCLK_Frequency = 0;	//;ϵͳʱ��
#endif

/* 
********************************************************************************
* �� �� ���� I2C_SDA_DIR_OUT
* ��    �ܣ� ����SDA����Ϊ���
* ��    ���� ��
* ��    �أ� �� 
* ˵    ���� 
* �׳��쳣��
* ��    �ߣ� simon
* �޸ļ�¼�� 
********************************************************************************
*/
static void I2C_SDA_DIR_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //;SDA
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //;�������
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/* 
********************************************************************************
* �� �� ���� I2C_SDA_DIR_IN
* ��    �ܣ� ����SDA����Ϊ����
* ��    ���� ��
* ��    �أ� ��
* ˵    ����
* �׳��쳣��
* ��    �ߣ�simon
* �޸ļ�¼��2014-12-10
********************************************************************************
*/
static void I2C_SDA_DIR_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //;SDA
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //;��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}


/* 
********************************************************************************
* �� �� ���� I2C_TimingDelay
* ��    �ܣ� IIC����ʱ����ʱ����
* ��    ���� time: ��Ҫ��ʱ��΢��(us)�� ��ȥ 1 
* ��    �أ� ��
* ˵    ���� �ɸ��ݲ�ͬ����Ƶ�ʵ�����ʱʱ��(����STM32�����±�д�ú���)
* �׳��쳣��
* ��    �ߣ� simon
* �޸ļ�¼�� ������2014-12-10 13:37:37
********************************************************************************
*/
void I2C_TimingDelay(u32 time)
{
	#ifdef I2C_STM32_SYSCLK		//;stm32��Ƭ����ʱ����
		u32 cnt;
		cnt = SYSCLK_Frequency * time;
		while (cnt--);
	#else				//;������Ƭ����ʱ����
		time *= 10;
		while (time--);
	#endif
}

/* 
********************************************************************************
* �� �� ���� I2C_StartTimingSeq
* ��    �ܣ� IIC����ʱ��
* ��    ���� ��
* ��    �أ� ��
* ˵    ���� 
* �׳��쳣��
* ��    �ߣ� simon
* �޸ļ�¼�� ������2014-12-10
********************************************************************************
*/
static void I2C_StartTimingSeq(void)
{
	I2C_SDA_DIR_OUT();
	I2C_SCL_H;
	I2C_SDA_H;
	I2C_TimingDelay(5);
	I2C_SDA_L;
	I2C_TimingDelay(5);
	I2C_SCL_L;
}

/* 
********************************************************************************
* �� �� ���� I2C_StopTimingSeq
* ��    �ܣ� IIC����ʱ��
* ��    ���� ��
* ��    �أ� ��
* ˵    ���� 
* �׳��쳣��
* ��    �ߣ� simon
* �޸ļ�¼�� 2014-12-10
********************************************************************************
*/
static void I2C_StopTimingSeq(void)
{
	I2C_SDA_DIR_OUT();
	I2C_SCL_L;
	I2C_SDA_L;
	I2C_TimingDelay(5);
	I2C_SCL_H;
	I2C_SDA_H;
	I2C_TimingDelay(10);
}

/* 
********************************************************************************
* �� �� ���� I2C_InitFRAM
* ��    �ܣ� ����IIC������ų�ʼ״̬
* ��    ���� ��
* ��    �أ� ��
* ˵    ����
* �׳��쳣��
* ��    �ߣ�simon
* �޸ļ�¼��2014-12-10
********************************************************************************
*/
void I2C_InitFRAM(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE); //;����GPIOʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7; //;SCL , SDA
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //;�������
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	I2C_SCL_H;
	I2C_SDA_H;
	I2C_WP_L;
	
	#ifdef I2C_STM32_SYSCLK		//;ʹ��STM32оƬ�������
	{
		RCC_ClocksTypeDef RCC_Clocks;
		RCC_GetClocksFreq(&RCC_Clocks); //;��ȡ����ʱ��
		SYSCLK_Frequency = RCC_Clocks.SYSCLK_Frequency;
		if (0 == SYSCLK_Frequency)
		{
			SYSCLK_Frequency = 36000000;	//;Ĭ�Ϲ���Ƶ��36M
		}
		SYSCLK_Frequency /= 6000000;		//;����6M
	}
	#endif
        I2C_StopTimingSeq();
        I2C_TimingDelay(100);
}


/* 
********************************************************************************
* �� �� ���� I2C_SendByte
* ��    �ܣ� ��IIC����д��һ���ֽ�
* ��    ���� data����Ҫд�������
* ��    �أ� ��
* ˵    ���� 
* �׳��쳣��
* ��    �ߣ� simon
* �޸ļ�¼�� 2014-12-10
********************************************************************************
*/
static void I2C_SendByte(u8 data)
{
	u8 i;
	
	I2C_SDA_DIR_OUT();
	I2C_SCL_L;
	for (i = 0; i < 8; i++)
	{
		if (data & 0x80) //;�����λ��ʼ����
		{
			I2C_SDA_H;
		}
		else
		{
			I2C_SDA_L;
		}
		data <<= 1;
		I2C_TimingDelay(2);
		I2C_SCL_H; 
		I2C_TimingDelay(3);
		I2C_SCL_L;
		I2C_TimingDelay(2);
	}
}

/* 
********************************************************************************
* �� �� ���� I2C_GetAck
* ��    �ܣ� ��ȡӦ���ź�
* ��    ���� ��
* ��    �أ� Ӧ���ƽ
* ˵    ���� 
* �׳��쳣��
* ��    �ߣ� simon
* �޸ļ�¼�� 2014-12-10
********************************************************************************
*/
static u8 I2C_GetAck(void)
{
	u8 ack = I2C_SLAVE_NOACK, trynum = 0;
	
	I2C_SDA_DIR_OUT();
	I2C_SCL_L;
	I2C_SDA_H;
	I2C_SDA_DIR_IN();
	I2C_TimingDelay(0);
	while ((I2C_SLAVE_NOACK == ack) && (trynum < 200))
	{
		ack = I2C_GET_SDA;
		trynum++;
	}
	if (trynum < 200)
	{
		ack = I2C_SLAVE_ACK;
	}
	I2C_SCL_H; 
	I2C_TimingDelay(2);
	I2C_SCL_L;
	I2C_TimingDelay(2);
	return ack;
}

/* 
********************************************************************************
* �� �� ���� I2C_ReceiveByte
* ��    �ܣ� ��IIC���߶�ȡһ���ֽ�
* ��    ���� ��
* ��    �أ� ��ȡ��������
* ˵    ���� 
* �׳��쳣��
* ��    �ߣ� simon
* �޸ļ�¼�� 2014-12-10
********************************************************************************
*/
static u8 I2C_ReceiveByte(void)
{
	u8 data = 0;
	u8 i;
	
	I2C_SCL_L;
	I2C_TimingDelay(0);
	I2C_SDA_DIR_IN();
	for (i=0; i<8; i++)
	{
		I2C_SCL_H;
		data <<= 1;
		if (I2C_GET_SDA)
		{
			data++;
		}
		I2C_TimingDelay(4);
		I2C_SCL_L;
		I2C_TimingDelay(2);
	}
	I2C_SDA_DIR_OUT();
	return data;
}
/* 
********************************************************************************
* �� �� ���� I2C_GiveAck
* ��    �ܣ� ����Ӧ���ź�
* ��    ���� ��
* ��    �أ� ��
* ˵    ���� 
* �׳��쳣��
* ��    �ߣ� simon
* �޸ļ�¼�� 2014-12-10
********************************************************************************
*/
static void I2C_GiveAck(void)
{
	I2C_SDA_DIR_OUT();
	I2C_SCL_L;
	I2C_TimingDelay(1);
	I2C_SDA_L;
	I2C_TimingDelay(1);
	I2C_SCL_H;
	I2C_TimingDelay(3);
	I2C_SCL_L;
	I2C_TimingDelay(1);
}

/* 
********************************************************************************
* �� �� ���� I2C_GiveNoAck
* ��    �ܣ� ������Ӧ���ź�
* ��    ���� ��
* ��    �أ� ��
* ˵    ���� 
* �׳��쳣��
* ��    �ߣ� simon
* �޸ļ�¼�� 2014-12-10
********************************************************************************
*/
static void I2C_GiveNoAck(void)
{
	I2C_SDA_DIR_OUT();
	I2C_SCL_L;
	I2C_TimingDelay(1);
	I2C_SDA_H;
	I2C_TimingDelay(1);
	I2C_SCL_H;
	I2C_TimingDelay(3);
	I2C_SCL_L;
	I2C_TimingDelay(1);
}

/* 
********************************************************************************
* �� �� ���� I2C_SaveDataToFRAM
* ��    �ܣ� ����ָ���������ݵ�FRAM��ָ����ַ
* ��    ���� 
*	dataNum������������ֽ���
*	saveAddr������λ��
*	dataToSave����Ҫ���������
*	checkWord: ����Ϊ�궨�塰I2C_WRITE_PROTECT_WORD������ֹ�����ܷɣ������󱣴�
* ��    �أ� result:�������
* ˵    ���� 
* �׳��쳣��
* ��    �ߣ� simon
* �޸ļ�¼�� 2014-12-10
********************************************************************************
*/
u8 I2C_SaveDataToFRAM(u16 dataNum, u16 saveAddr, u8* dataToSave,u32 checkWord)
{
	u8 result = I2C_OPERATION_FAILED; //;�����������ֵ
	
	if((dataNum > 0) && (I2C_WRITE_PROTECT_WORD == checkWord))
	{
		u8 upperFlag = saveAddr / 256;	//;��λ��ַ���address[8]
		u8 offset = saveAddr % 256;	//;ƫ�Ƶ�ַ
		u8 ack;		//;Ӧ���ź�
		u8 trynum;	//;���Դ���
		u16 i;		//;������
		
		for (trynum = 0; trynum < 5; trynum++)
		{
			I2C_WP_L;		//;����д����
			I2C_StartTimingSeq();	//;����IIC���߲���
			if (upperFlag)
			{
				I2C_SendByte(FRAM_ADDR | FRAM_UPPER_ADDR | FRAM_WRITE); //;�ߵ�ַд���ݲ���
			}
			else
			{
				I2C_SendByte(FRAM_ADDR | FRAM_LOWER_ADDR | FRAM_WRITE); //;�͵�ַд���ݲ���
			}
			ack = I2C_GetAck();
			if (I2C_SLAVE_NOACK == ack)
			{
				continue;
			}
			I2C_SendByte(offset); //;address[7:0]
			ack = I2C_GetAck();
			if (I2C_SLAVE_NOACK == ack)
			{
				continue;
			}
			for (i = 0; i < dataNum; i++)
			{
				I2C_SendByte(dataToSave[i]);
				ack = I2C_GetAck();
				if (I2C_SLAVE_NOACK == ack)
				{
					break;
				}
			}
			if (i != dataNum)
			{
				continue;
			}
			else
			{
				break;
			}
		}
		I2C_StopTimingSeq();
		I2C_TimingDelay(100);
		I2C_WP_H;
		if (trynum <5)
		{
			result = I2C_OPERATION_SUCCEED;
		}
	}
	return result;
}

/* 
********************************************************************************
* �� �� ���� I2C_GetDataFromFRAM
* ��    �ܣ� ��FRAM��ָ����ַ��ȡָ�����ȵ�����
* ��    ���� 
*	dataNum����Ҫ��ȡ�������ֽ���
*	getAddr����ȡλ��
*	dataBuf����ȡ�������ݱ����λ��
* ��    �أ� result:�������
* ˵    ���� 
* �׳��쳣��
* ��    �ߣ� simon
* �޸ļ�¼�� 2014-12-10
********************************************************************************
*/
u8 I2C_GetDataFromFRAM(u16 dataNum, u16 getAddr, u8* dataBuf)
{
	u8 result = I2C_OPERATION_FAILED; //;�����������ֵ
	
	if(dataNum > 0)
	{
		u8 upperFlag = getAddr / 256;	//;��λ��ַ���address[8]
		u8 offset = getAddr % 256;	//;ƫ�Ƶ�ַ
		u8 ack;		//;Ӧ���ź�
		u8 trynum;	//;���Դ���
		u16 i;		//;������
		
		for (trynum = 0; trynum < 5; trynum++)
		{
			I2C_WP_L;		//;����д����
			I2C_StartTimingSeq();	//;����IIC���߲���
			if (upperFlag)
			{
				I2C_SendByte(FRAM_ADDR | FRAM_UPPER_ADDR | FRAM_WRITE); //;�ߵ�ַд���ݲ���
			}
			else
			{
				I2C_SendByte(FRAM_ADDR | FRAM_LOWER_ADDR | FRAM_WRITE); //;�͵�ַд���ݲ���
			}
			ack = I2C_GetAck();
			if (I2C_SLAVE_NOACK == ack)
			{
				continue;
			}
			I2C_SendByte(offset); //;address[7:0]
			ack = I2C_GetAck();
			if (I2C_SLAVE_NOACK == ack)
			{
				continue;
			}
			I2C_StartTimingSeq();	//;����IIC���߲���
			if (upperFlag)
			{
				I2C_SendByte(FRAM_ADDR | FRAM_UPPER_ADDR | FRAM_READ); //;�ߵ�ַд���ݲ���
			}
			else
			{
				I2C_SendByte(FRAM_ADDR | FRAM_LOWER_ADDR | FRAM_READ); //;�͵�ַд���ݲ���
			}
			ack = I2C_GetAck();
			if (I2C_SLAVE_NOACK == ack)
			{
				continue;
			}
			for(i = 0; i < (dataNum - 1); i++)
			{
				dataBuf[i] = I2C_ReceiveByte();
				I2C_GiveAck();
			}
			dataBuf[dataNum - 1] = I2C_ReceiveByte();
			I2C_GiveNoAck();
			I2C_StopTimingSeq();
			I2C_TimingDelay(100);
			result = I2C_OPERATION_SUCCEED;
			break;
		}
	}
	return result;
}

/* 
********************************************************************************
* �� �� ���� I2C_FormatFRAM
* ��    �ܣ� ��ʽ��FRAM
* ��    ���� ��
* ��    �أ� ��
* ˵    ���� 
* �׳��쳣��
* ��    �ߣ� simon
* �޸ļ�¼�� 2014-12-10
********************************************************************************
*/
void I2C_FormatFRAM(void)
{
	u8 trynum, ack;
	u16 i;
	
	for (trynum = 0; trynum < 5; trynum++)
	{
		I2C_WP_L;		//;����д����
		I2C_StartTimingSeq();	//;����IIC���߲���
		I2C_SendByte(FRAM_ADDR | FRAM_LOWER_ADDR | FRAM_WRITE); //;�͵�ַд���ݲ���
		ack = I2C_GetAck();
		if (I2C_SLAVE_NOACK == ack)
		{
			continue;
		}
		I2C_SendByte(0); 	//;address[7:0],�ӵ�ַ0��ʼ
		ack = I2C_GetAck();
		if (I2C_SLAVE_NOACK == ack)
		{
			continue;
		}
		for (i = 0; i < 512; i++)
		{
			I2C_SendByte(0);
			ack = I2C_GetAck();
			if (I2C_SLAVE_NOACK == ack)
			{
				break;
			}
		}
		if (i != 512)
		{
			continue;
		}
		else
		{
			break;
		}
	}
	I2C_StopTimingSeq();
	I2C_TimingDelay(100);
	I2C_WP_H;
        I2C_TimingDelay(5000);
}

/*----------------------------------����Ϊ��ȱ�ר�ú�������ֲ��ɾ��--------------------------------------*/

//;==================================================
//;�����ݺ�����У���뱣�浽�洢��������λ��
//; ���� n--      ���ݳ��ȣ�������У����
//;      EEAdr1 EEAdr2  �洢��������λ��
//;      adr_ram        Ҫ�������ݵ���ʼ��ַ
//;==================================================
void SaveData_FM24(u8 n, u16 EEAdr1, u16 EEAdr2, u8 *adr_ram)
{
	u8 i;
	u8 buf[256];
	
	buf[n] = 0x55;
	for (i = 0; i < n; i++)
	{
		buf[i] = *(adr_ram + i);
		buf[n] += *(adr_ram + i);
	}
	I2C_SaveDataToFRAM(n+1, EEAdr1, buf, I2C_WRITE_PROTECT_WORD);
	I2C_SaveDataToFRAM(n+1, EEAdr2, buf, I2C_WRITE_PROTECT_WORD);
}

//;==================================================
//;�����ݴӴ洢��������λ�ö������ҳ���ȷ������
//; ���� n    --        ���ݳ��ȣ�������У����
//;      EEAdr1 EEAdr2  �洢��������λ��
//;      adr_ram        Ҫ�������ݵ���ʼ��ַ
//;==================================================
void ReadData_FM24(u8 n, u16 EEAdr1, u16 EEAdr2, u8 *adr_ram)
{
	u8 i, ch;
	u8 buf[256];
	
	I2C_GetDataFromFRAM(n + 1, EEAdr1, buf);
	ch = 0x55;
	for (i = 0; i < n; i++)
	{
		ch += buf[i];
	}
	if (ch == buf[n])
	{
		for (i = 0; i < n; i++)
		{
			*(adr_ram + i) = buf[i];
		}
		return ;
	}
	else
	{
		I2C_GetDataFromFRAM(n + 1, EEAdr2, buf);
		ch = 0x55;
		for (i = 0; i < n; i++)
		{
			ch += buf[i];
		}
		if (ch == buf[n])
		{
			for (i = 0; i < n; i++)
			{
				*(adr_ram + i) = buf[i];
			}
			return ;
		}
		else
		{
			for (i = 0; i < n; i++)
			{
				*(adr_ram + i) = 0;
			}
		}
	}
}
/*------------------------------------------------END---------------------------------------------------*/

