/*
********************************************************************************
*  �� �� ���� I2C_FRAM.h
*
*  �ļ������� MB85RC04V����������
*
*  ����оƬ�� MB85RC04V
*
*  �� �� �ˣ� simon--20141210
*
*  �� �� �ţ� 1.00
*
*  �޸ļ�¼�� 1. ������2013-10-24 10:52:17
********************************************************************************
*/

#ifndef __I2C_FRAM
#define __I2C_FRAM

/* ���ܺ궨�� */
#define		I2C_USE_WP			//;ʹ��WP����
#define		I2C_STM32_SYSCLK		//;ʹ��stm32оƬ���ں�ʱ�ӽ�����ʱ
						//;����stm32(ȡ���ú궨��)�����޸�I2C_TimingDelay����

/* ����ֵ�궨�� */
#define		I2C_OPERATION_SUCCEED		0x00//;�����ɹ�
#define		I2C_OPERATION_FAILED		0x01//;����ʧ��
#define		I2C_SLAVE_ACK			0x00//;���豸��Ӧ��
#define		I2C_SLAVE_NOACK			0x01//;���豸��Ӧ��

/* ���궨�� */
#define		I2C_WRITE_PROTECT_WORD		0x12345678

/* �������� */
extern void I2C_InitFRAM(void);
extern u8 I2C_GetDataFromFRAM(u16 dataNum, u16 getAddr, u8* dataBuf);
extern u8 I2C_SaveDataToFRAM(u16 dataNum, u16 saveAddr, u8* dataToSave,u32 checkWord);
extern void I2C_FormatFRAM(void);


/*------------------****---����Ϊ���ȱ�ģ�����Ӧ�ã���ֲ��ɾ���������ж���---------****---------*/
static inline u8 ReadNByte_I2C1(u8 n, u16 adr, u8* adr_ram )
{
	I2C_GetDataFromFRAM(n, adr, adr_ram);
	return 1;
}

static inline u8 WriteNByte_I2C1(u8 n, u16 adr, u8 *adr_ram)
{
	I2C_SaveDataToFRAM(n, adr, adr_ram,I2C_WRITE_PROTECT_WORD);
	return 1;
}

extern void SaveData_FM24(u8 n, u16 EEAdr1, u16 EEAdr2, u8 *adr_ram);
extern void ReadData_FM24(u8 n, u16 EEAdr1, u16 EEAdr2, u8 *adr_ram);
/*---------------------------------------------END-------------------------------------------------*/

#endif
