/*
********************************************************************************
*  �� �� ����SPI_Flash.C
*
*  �ļ�������
*
*  ����оƬ��
*
*  �� �� �ˣ�
*
*  �� �� �ţ�
*
*  �޸ļ�¼��2011-07-07 14:40:06
********************************************************************************
*/

/* Includes ------------------------------------------------------------------*/

#include "BSP_CFG.h"

/* Private typedef -----------------------------------------------------------*/
#ifndef PAGECOUNT
    #define PAGECOUNT 4096
#endif


#define WIP_Flag   0x01  /* Write In Progress (WIP) flag */

#define Dummy_Byte 0xA5


//;======================AT45DB161D��������===========================
#define BUFFER_1_WRITE 0x84					 //; д���һ������
#define BUFFER_2_WRITE 0x87					 //; д��ڶ�������
#define BUFFER_1_READ 0xD4					 //; ��ȡ��һ������
#define BUFFER_2_READ 0xD6					 //; ��ȡ�ڶ�������
#define B1_TO_MM_PAGE_PROG_WITH_ERASE 0x83	 //; ����һ������������д�����洢��������ģʽ��
#define B2_TO_MM_PAGE_PROG_WITH_ERASE 0x86	 //; ���ڶ�������������д�����洢��������ģʽ��
#define MM_PAGE_TO_B1_XFER 0x53				 //; �����洢����ָ��ҳ���ݼ��ص���һ������
#define MM_PAGE_TO_B2_XFER 0x55				 //; �����洢����ָ��ҳ���ݼ��ص��ڶ�������
#define PAGE_ERASE 0x81						 //; ҳɾ����ÿҳ512/528�ֽڣ�
#define SECTOR_ERASE 0x7C					 //; ����������ÿ����128K�ֽڣ�
#define READ_STATE_REGISTER 0xD7			 //; ��ȡ״̬�Ĵ���

//;-----------Flash2����
#define SPI1_CS1_Low    GPIO_ResetBits(GPIOC, GPIO_Pin_4)
#define SPI1_CS1_High   GPIO_SetBits(GPIOC, GPIO_Pin_4)
#define SPI1_WP1_Low    GPIO_ResetBits(GPIOC, GPIO_Pin_5)
#define SPI1_WP1_High   GPIO_SetBits(GPIOC, GPIO_Pin_5)

//;//;-----------Flash2����
//;#define SPI1_CS2_Low    GPIO_ResetBits(GPIOC, GPIO_Pin_4)
//;#define SPI1_CS2_High   GPIO_SetBits(GPIOC, GPIO_Pin_4)
//;#define SPI1_WP2_Low    GPIO_ResetBits(GPIOC, GPIO_Pin_5)
//;#define SPI1_WP2_High   GPIO_SetBits(GPIOC, GPIO_Pin_5)
//;
//;//;-----------Flash3����
//;#define SPI2_CS1_Low    GPIO_ResetBits(GPIOD, GPIO_Pin_10)
//;#define SPI2_CS1_High   GPIO_SetBits(GPIOD, GPIO_Pin_10)
//;#define SPI2_WP1_Low    GPIO_ResetBits(GPIOD, GPIO_Pin_13)
//;#define SPI2_WP1_High   GPIO_SetBits(GPIOD, GPIO_Pin_13)
//;
//;//;-----------Flash4����
//;#define SPI2_CS2_Low    GPIO_ResetBits(GPIOE, GPIO_Pin_4)
//;#define SPI2_CS2_High   GPIO_SetBits(GPIOE, GPIO_Pin_4)
//;#define SPI2_WP2_Low    GPIO_ResetBits(GPIOE, GPIO_Pin_3)
//;#define SPI2_WP2_High   GPIO_SetBits(GPIOE, GPIO_Pin_3)


 /*���������ҳ����ѡ��ڼ���Flash����������*/
void SPI_CS_Low(u8 M)
{
    if(M == 0)
    {
        SPI1_CS1_Low;
    }
//;    else if(M == 1)
//;    {
//;        SPI1_CS2_Low;
//;    }
//;    else if(M == 2)
//;    {
//;        SPI2_CS1_Low;
//;    }
//;    else if(M == 3)
//;    {
//;        SPI2_CS2_Low;
//;    }
}
 /*���������ҳ����ѡ��ڼ���Flash����������*/
void SPI_CS_High(u8 M)
{
    if(M == 0)
    {
        SPI1_CS1_High;
    }
//;    else if(M == 1)
//;    {
//;        SPI1_CS2_High;
//;    }
//;    else if(M == 2)
//;    {
//;        SPI2_CS1_High;
//;    }
//;    else if(M == 3)
//;    {
//;        SPI2_CS2_High;
//;    }
}

/*���������ҳ����ѡ��ڼ���Flash����������*/
void SPI_WP_Low(u8 M)
{
    if(M == 0)
    {
        SPI1_WP1_Low;
    }
//;    else if(M == 1)
//;    {
//;        SPI1_WP2_Low;
//;    }
//;    else if(M == 2)
//;    {
//;        SPI2_WP1_Low;
//;    }
//;    else if(M == 3)
//;    {
//;        SPI2_WP2_Low;
//;    }
}

/*���������ҳ����ѡ��ڼ���Flash����������*/
void SPI_WP_High(u8 M)
{
    if(M == 0)
    {
        SPI1_WP1_High;
    }
//;    else if(M == 1)
//;    {
//;        SPI1_WP2_High;
//;    }
//;    else if(M == 2)
//;    {
//;        SPI2_WP1_High;
//;    }
//;    else if(M == 3)
//;    {
//;        SPI2_WP2_High;
//;    }
}

/* 
********************************************************************************
* �� �� ����SPI_FLASH_SendByte
* ��    �ܣ�����һ���ֽڲ��ҷ��ؽ��յ���һ���ֽ�
* ��    ����n==0:SPI1,n==1:SPI2 ;byte : ��Ҫ���͵��ֽ�
* ��    �أ����յ����ֽ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��2011-07-07 15:09:29
********************************************************************************
*/
u8 SPI_FLASH_SendByte(u8 n, u8 byte)
{
    if(n == 0)
    {
        while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
        SPI_I2S_SendData(SPI1, byte);
        while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
        return SPI_I2S_ReceiveData(SPI1);
    }
//;    else if(n == 1)
//;    {
//;        while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
//;        SPI_I2S_SendData(SPI2, byte);
//;        while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
//;        return SPI_I2S_ReceiveData(SPI2);
//;    }  
    return 0;
}


/*******************************************************************************
* ��������   : SPI_FLASH_ReadByte
* ��    ��   : �� SPI Flash �ж�������
*              ���������Ҫ��ִ�� Start_Read_Sequence ����
* ��    ��   ��None
* ��    ��   : None
* ��    ��   : Byte Read from the SPI Flash.
*******************************************************************************/
u8 SPI_FLASH_ReadByte(u8 n)
{
  return (SPI_FLASH_SendByte(n, Dummy_Byte));
}


/*******************************************************************************
* ��������  : SPI_FLASH_WriteEnable
* ��    ��  : ������FlashоƬд����(ȡ���������)
* ��    ��  : None
* ��    ��  : None
* ��    ��  : None
*******************************************************************************/
void SPI_FLASH_WriteEnable(u8 n, u8 M)
{
  SPI_CS_Low(M);                     //;ѡ��������Ч
  SPI_FLASH_SendByte(n, 0x3d);        //;д���ݣ�д�����ָ��
  SPI_FLASH_SendByte(n, 0x2a);        //;д���ݣ�д�����ָ��
  SPI_FLASH_SendByte(n, 0x7f);        //;д���ݣ�д�����ָ��
  SPI_FLASH_SendByte(n, 0x9a);        //;д���ݣ�д�����ָ��
  SPI_CS_High(M);                    //;ѡ��������Ч
}

/*******************************************************************************
* ��������  : SPI_FLASH_WriteEnable
* ��    ��  : ��ֹ��FlashоƬд���� (�������)
* ��    ��  : None
* ��    ��  : None
* ��    ��  : None
*******************************************************************************/
void SPI_FLASH_WriteDisable(u8 n, u8 M)
{
  SPI_CS_Low(M);                     //;ѡ��������Ч
  SPI_FLASH_SendByte(n, 0x3d);        //;д���ݣ�д�����ָ��
  SPI_FLASH_SendByte(n, 0x2a);        //;д���ݣ�д�����ָ��
  SPI_FLASH_SendByte(n, 0x7f);        //;д���ݣ�д�����ָ��
  SPI_FLASH_SendByte(n, 0xA9);        //;д���ݣ�д�����ָ��
  SPI_CS_High(M);                    //;ѡ��������Ч
}
/*******************************************************************************
*��������  :SPI_FLASH_ReadStatus
*��    ��  :��ȡFlash��״̬�Ĵ���  
*��    ��  :���϶�ȡFLash��״̬һֱ�� (RDY/BUSY)λΪ1
*��    ��  :PROTECT λ��״̬ 
*��    ��  :None
*******************************************************************************/
u8 SPI_FLASH_ReadStatus(u8 n, u8 M)
{
  u8 FLASH_Status = 0;
  SPI_CS_Low(M);     //;ѡ��������Ч
  SPI_FLASH_SendByte(n, READ_STATE_REGISTER);  //;���Ͷ�״̬�Ĵ���������
  //;ѭ����ȡд�Ĵ�����״̬��һֱ��Flash��æ���Ƴ�
  do
  {
    FLASH_Status = SPI_FLASH_SendByte(n, 0x00);
  } while(!(FLASH_Status & 0x80));
  SPI_CS_High(M);    //;CS2�ø�
  return(FLASH_Status&0x02);
}


/*******************************************************************************
* ��������  : SPI_FLASH_SectorErase
* ��    ��  : ���� FLASHָ��ҳ������
* ��    ��  : SectorAddr: ָ�������ĵ�ַ
* ��    ��  : ��
* ��    ��  : ��
*******************************************************************************/
void SPI_FLASH_SectorErase(u32 page)
{
  u8 FlashX, spiN;
  
  FlashX = page / PAGECOUNT; //;�ڼ�ƬFlash
  spiN   = FlashX / 2;
  page = page % PAGECOUNT; //;�ڸ�Flash�еĵڼ�ҳ
  //;SPI_FLASH_WriteEnable();   //;��FLASHоƬ����д�����ָ��
  SPI_CS_Low(FlashX);               //;ѡ��оƬ cs �õ�
  SPI_FLASH_SendByte(spiN, SECTOR_ERASE);    //;�������� �� ��������������
  SPI_FLASH_SendByte(spiN, (unsigned char)(page >> 6)); //;�������� ����ַ2�ֽ�
  SPI_FLASH_SendByte(spiN, (unsigned char)(page << 2));  //;�������� ����ַ1�ֽ�
  SPI_FLASH_SendByte(spiN, 0x00);   //;�������� �� ��ַ����ֽ�
  SPI_CS_High(FlashX);               //; cs �ø�
  SPI_FLASH_ReadStatus(spiN, FlashX);  //;�ȴ���д���
}


/*******************************************************************************
* ��������  : SPI_FLASH_PageErase
* ��    ��  : ���� FLASHָ��ҳ������
* ��    ��  : SectorAddr: ָ�������ĵ�ַ
* ��    ��  : ��
* ��    ��  : ��
*******************************************************************************/
void SPI_FLASH_PageErase(u32 page)
{
  u8 FlashX, spiN;
  
  FlashX = page / PAGECOUNT; //;�ڼ�ƬFlash
  spiN   = FlashX / 2;
  page = page % PAGECOUNT; //;�ڸ�Flash�еĵڼ�ҳ
  //;SPI_FLASH_WriteEnable();   //;��FLASHоƬ����д�����ָ��
  SPI_CS_Low(FlashX);               //;ѡ��оƬ cs �õ�
  SPI_FLASH_SendByte(spiN, PAGE_ERASE);    //;�������� �� ��������������
  SPI_FLASH_SendByte(spiN, (unsigned char)(page >> 6)); //;�������� ����ַ2�ֽ�
  SPI_FLASH_SendByte(spiN, (unsigned char)(page << 2));  //;�������� ����ַ1�ֽ�
  SPI_FLASH_SendByte(spiN, 0x00);   //;�������� �� ��ַ����ֽ�
  SPI_CS_High(FlashX);               //; cs �ø�
  SPI_FLASH_ReadStatus(spiN, FlashX);  //;�ȴ���д���
}

/*******************************************************************************
*��������  : SPI_FLASH_PageWrite_E
*��    ��  : flash��ˢ��д
*��    ��  : - pBuffer : д��Flash�����ݵ���ʼ��ַ   page ��д�뵽��ҳ��
*******************************************************************************/
void SPI_FLASH_PageWrite_E(u8* pBuffer, u16 page)
{ 
  u8 FlashX, spiN;
  u16 NumByteToWrite=528;
  
  FlashX = page / PAGECOUNT; //;�ڼ�ƬFlash
  spiN   = FlashX / 2;
  page = page % PAGECOUNT; //;�ڸ�Flash�еĵڼ�ҳ
  
  SPI_FLASH_Init();
  SPI_FLASH_WriteEnable(spiN, FlashX);
  
  SPI_WP_High(FlashX);
  //;SPI_FLASH_WriteEnable(); //;����дFlashоƬ
  SPI_CS_Low(FlashX);               //;ѡ��оƬ cs �õ�
  SPI_FLASH_SendByte(spiN, BUFFER_2_WRITE);   //;дbuffer2����
  SPI_FLASH_SendByte(spiN, 0x00);
  SPI_FLASH_SendByte(spiN, 0x00);  
  SPI_FLASH_SendByte(spiN, 0x00);
  while(NumByteToWrite--) 
  {
    SPI_FLASH_SendByte(spiN, *pBuffer);
    pBuffer++; 
  }
  SPI_CS_High(FlashX);               //;ѡ��оƬ cs �ø�
  
  if(page<4096)
  { 
  	SPI_CS_Low(FlashX);                //;ѡ��оƬ cs �õ�
    SPI_FLASH_SendByte(spiN, 0x86);   //;ͨ��buffer2��ˢ��д
    SPI_FLASH_SendByte(spiN, (u8)(page>>6));
    SPI_FLASH_SendByte(spiN, (u8)(page<<2));  
    SPI_FLASH_SendByte(spiN, 0x00);
  	
    SPI_CS_High(FlashX);               //;ѡ��оƬ cs �ø�
    SPI_FLASH_ReadStatus(spiN, FlashX);
  }
  SPI_WP_Low(FlashX);
  
  Delay_nms(40);
  SPI_FLASH_WriteDisable(spiN, FlashX);
}


/*******************************************************************************
*��������  : SPI_FLASH_PageWrite_NE
*��    ��  : flash����ˢ��ֱ��д
*��    ��  : - pBuffer : д��Flash�����ݵ���ʼ��ַ   page ��д�뵽��ҳ��
*******************************************************************************/
void SPI_FLASH_PageWrite_NE(u8* pBuffer, u16 page )
{ 
  u8  FlashX, spiN;
  u16 NumByteToWrite=528;
  
  FlashX = page / PAGECOUNT; //;�ڼ�ƬFlash
  spiN   = FlashX / 2;
  page = page % PAGECOUNT; //;�ڸ�Flash�еĵڼ�ҳ
  
  SPI_FLASH_Init();
  SPI_FLASH_WriteEnable(spiN, FlashX);
  
  SPI_WP_High(FlashX);
  //;SPI_FLASH_WriteEnable(); //;����дFlashоƬ
  SPI_CS_Low(FlashX);               //;ѡ��оƬ cs �õ�
  SPI_FLASH_SendByte(spiN, BUFFER_2_WRITE);   //;дbuffer2����
  SPI_FLASH_SendByte(spiN, 0x00);
  SPI_FLASH_SendByte(spiN, 0x00);  
  SPI_FLASH_SendByte(spiN, 0x00);
  while(NumByteToWrite--) 
  {
    SPI_FLASH_SendByte(spiN, *pBuffer);
    pBuffer++; 
  }
  SPI_CS_High(FlashX);               //;ѡ��оƬ cs �ø�
  
  if(page<4096)
  { 
  	SPI_CS_Low(FlashX);               //;ѡ��оƬ cs �õ�
    SPI_FLASH_SendByte(spiN, 0x89);   //;ͨ��buffer2ֱ��д
    SPI_FLASH_SendByte(spiN, (u8)(page>>6));
    SPI_FLASH_SendByte(spiN, (u8)(page<<2));  
    SPI_FLASH_SendByte(spiN, 0x00);
  	
    SPI_CS_High(FlashX);               //;ѡ��оƬ cs �ø�
    SPI_FLASH_ReadStatus(spiN, FlashX);
  }
  SPI_WP_Low(FlashX);
  
  Delay_nms(40);
  SPI_FLASH_WriteDisable(spiN, FlashX);
}

/*******************************************************************************
*��������  : SPI_FLASH_PageRead
*��    ��  : flashҳд
*��    ��  : - pBuffer : д��Flash�����ݵ���ʼ��ַ   page ��д�뵽��ҳ��
*��    ��  :
*��    ��  :
*******************************************************************************/
void SPI_FLASH_PageRead(u8* pBuffer, u16 page )
{ 
  u8  FlashX, spiN;
  u16 NumByteToWrite=528;
  
  FlashX = page / PAGECOUNT; //;�ڼ�ƬFlash
  spiN   = FlashX / 2;
  page = page % PAGECOUNT; //;�ڸ�Flash�еĵڼ�ҳ
  SPI_CS_Low(FlashX);               //;ѡ��оƬ cs �õ�
  SPI_FLASH_SendByte(spiN, MM_PAGE_TO_B1_XFER);   //;�����洢��������д�뵽 buffer1
  SPI_FLASH_SendByte(spiN, (u8)(page>>6));
  SPI_FLASH_SendByte(spiN, (u8)(page<<2));  
  SPI_FLASH_SendByte(spiN, 0x00);
  SPI_CS_High(FlashX);              //;ѡ��оƬ cs �ø�
  SPI_FLASH_ReadStatus(spiN, FlashX);
  
  SPI_CS_Low(FlashX);               //;ѡ��оƬ cs �õ�
  SPI_FLASH_SendByte(spiN, BUFFER_1_READ );   //;��buffer1����
  SPI_FLASH_SendByte(spiN, 0x00);
  SPI_FLASH_SendByte(spiN, 0x00);  
  SPI_FLASH_SendByte(spiN, 0x00);
  SPI_FLASH_SendByte(spiN, 0x00);
  while(NumByteToWrite--) 
  {
  	if((pBuffer< AllBuf.St_Spi) || (pBuffer> AllBuf.En_Spi))  //;���������ܳ���������
  	{ 
  	    SPI_CS_High(FlashX); return; 
  	}
    *pBuffer=SPI_FLASH_ReadByte(spiN);
    pBuffer++; 
  }        
  SPI_CS_High(FlashX);             //;ѡ��оƬ cs �ø�  
}



//;/*******************************************************************************
//;*��������  : SPI_FLASH_ReadNByte
//;*��    ��  : ��FLASH��������ֱ�Ӷ���n���ֽڵ�������
//;*��    ��  : - pBuffer : �Ѵ�Flash���������ݱ�����������ʼ��ַ
//;               page ��ҳ   adr_p ��ҳ�ڵĵ�ַ
//;*��    ��  :
//;*��    ��  :
//;*******************************************************************************/
void SPI_FLASH_ReadNByte(u8* pBuffer, u16 page,u16 adr_p,u16 n )
{ //;u16 NumByteToWrite=528;
	u8  FlashX, spiN;
	if(n>=528) n=528;
  FlashX = page / PAGECOUNT; //;�ڼ�ƬFlash
  spiN   = FlashX / 2;
  page = page % PAGECOUNT; //;�ڸ�Flash�еĵڼ�ҳ
  SPI_CS_Low(FlashX);               //;ѡ��оƬ cs �õ�
  SPI_FLASH_SendByte(spiN, MM_PAGE_TO_B1_XFER);   //;�����洢��������д�뵽 buffer1
  SPI_FLASH_SendByte(spiN, (u8)(page>>6));
  SPI_FLASH_SendByte(spiN, (u8)(page<<2));  
  SPI_FLASH_SendByte(spiN, 0x00);
  SPI_CS_High(FlashX);              //;ѡ��оƬ cs �ø�
  SPI_FLASH_ReadStatus(spiN, FlashX);
  
  SPI_CS_Low(FlashX);               //;ѡ��оƬ cs �õ�
  SPI_FLASH_SendByte(spiN,  BUFFER_1_READ );   //;��buffer1����
  SPI_FLASH_SendByte(spiN, 0x00);
  SPI_FLASH_SendByte(spiN, (u8)(adr_p>>8));
  SPI_FLASH_SendByte(spiN, (u8)(adr_p));
  SPI_FLASH_SendByte(spiN, 0x00);
  while(n--) 
  {
  	if(EW.RFlashNByte!='R') return;
    *pBuffer=SPI_FLASH_ReadByte(spiN);
    pBuffer++; 
  }        
  SPI_CS_High(FlashX);             //;ѡ��оƬ cs �ø�  
  
}

/*******************************************************************************
*��������  :SPI_FLASH_CheckProtReg
*��    ��  :���Protection�Ĵ��������Ƿ�ȫΪ0xff
*��    ��  :��
*��    ��  :��
*��    ��  :None
*******************************************************************************/
u8 SPI_FLASH_CheckProtReg(u8 n, u8 M)
{   
    u8 NumByteToWrite=16;
	u8 ch1,ch2=0xff;
	
	SPI_CS_Low(M);
	SPI_FLASH_SendByte(n, 0x32); //;
	SPI_FLASH_SendByte(n, 0x00);                        
	SPI_FLASH_SendByte(n, 0x00);   
	SPI_FLASH_SendByte(n, 0x00);  
    while(NumByteToWrite--) 
    {
      ch1=SPI_FLASH_ReadByte(n);
      if(ch1!=0XFF) ch2=0x00; 
    }        
    SPI_CS_High(M);      
    SPI_FLASH_ReadStatus(n, M);   
    return(ch2);       	
} 


/*******************************************************************************
*��������  :SPI_FLASH_EraseProtReg
*��    ��  :��ʽ��FLashоƬ�ı����Ĵ����������ǽ�Flash�ı����Ĵ���ȫ��дΪ 0xff
*��    ��  :��
*��    ��  :��
*��    ��  :��
*******************************************************************************/
void SPI_FLASH_EraseProtReg(u8 M)
{ 
    u8 spiN;
    
    spiN = M / 2;
	  if(SPI_FLASH_CheckProtReg(spiN, M) == 0x00)
	  {
	      SPI_CS_Low(M);
	      SPI_FLASH_SendByte(spiN, 0x3D); //;
	      SPI_FLASH_SendByte(spiN, 0x2A);                        
	      SPI_FLASH_SendByte(spiN, 0x7F);   
	      SPI_FLASH_SendByte(spiN, 0xCF);  
        SPI_CS_High(M);       
        SPI_FLASH_ReadStatus(spiN, M);      	
	  }
} 


