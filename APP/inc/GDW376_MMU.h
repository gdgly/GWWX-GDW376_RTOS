/*
********************************************************************************
*  �� �� ����GDW376_MMU.h
*
*  �ļ����������ݱ������Flash�ռ䣬FM24c04�ռ�
*
*  ����оƬ��FLASHΪAT45DB161D��2M = 4096ҳ * 528�ֽڣ�
*             EEROMΪFM24C04��(4*1024)/8 = 512byte��
*
*  �� �� �ˣ�
*
*  �� �� �ţ�
*
*  �޸ļ�¼��
********************************************************************************
*/
#ifndef __GDW130_MMU_h
#define __GDW130_MMU_h

#include "stm32f10x.h"
#include "Function_CFG.h"

#ifndef PAGECOUNT
    #define PAGECOUNT 4096
#endif
#define PAGESIZE         528  //;ÿҳ������ֽ�
//;#define PAGECOUNT        4095
#define PAGESOFSECTOR0A  8
#define PAGESOFSECTOR0B  248
#define PAGESOFSECTOR    264
/*------------------------------------------------------------------------------
|| Bin�ļ����������ΪPROG0(�������³���)��PROG1(���ԭ����)��
|| ��ʼҳΪ0��������0AΪ4K��0BΪ124K����ʼҳ�ı����޸���������
------------------------------------------------------------------------------*/
#if defined   (STM32F10XXB)
    #define   PROG_FLASH_SIZE     (0x20000)    /* 128 Kbyte */
#elif defined (STM32F10XXC)
    #define   PROG_FLASH_SIZE     (0x40000)    /* 256 Kbyte */
#elif defined (STM32F10XXD)
    #define   PROG_FLASH_SIZE     (0x60000)    /* 384 Kbyte */
#elif defined (STM32F10XXE)
    #define   PROG_FLASH_SIZE     (0x80000)    /* 512 Kbyte */
#else 
 #error "Not select the STM32 device type(Function_CFG.h)"    
#endif

#define PROG_PAGE_COUNT  (PROG_FLASH_SIZE / 512)

#define PROG_SECTOR_COUNT (PROG_PAGE_COUNT / 8 / 32)

#define FLASH_PAGE_START  0
#define PROG0_PAGE_START   FLASH_PAGE_START
#define PROG1_PAGE_START   PROG0_PAGE_START + PROG_PAGE_COUNT
#define PROG_PAGE_END      PROG1_PAGE_START + PROG_PAGE_COUNT

#define PAGE_SIGN_SIZE   4
#define PAGE_LENG_SIZE   4
#define PAGE_INDEX_SIZE  (2 + 2)
#define PAGE_HEAD_SIZE   PAGE_SIGN_SIZE + PAGE_LENG_SIZE + PAGE_INDEX_SIZE

#define PAGE_DATA_SIZE   512

//;
#define UPDATASIG0 'X'
#define UPDATASIG1 'Y'
#define UPDATASIG2 'G'
#define UPDATASIG3 'X'

#define OLDDATASIG0 'S'
#define OLDDATASIG1 'I'
#define OLDDATASIG2 'G'
#define OLDDATASIG3 'N'

//;�����������������������
#if PROG0_PAGE_START == 0
    #define PROG0_SECTOR_COUNT PROG_SECTOR_COUNT + 1
#else
    #define PROG0_SECTOR_COUNT PROG_SECTOR_COUNT
#endif

#define PROG1_SECTOR_COUNT PROG_SECTOR_COUNT

#if (PROG0_PAGE_START % 256) || (PROG0_PAGE_START % 256)
    #error "PROG isn't the sector's start!!"
#endif
 



/*------------------------------------------------------------------------------
|| �����㶳������,�����ն���,�����ն���,�¶���,���߶�����Flash�еĿռ����
------------------------------------------------------------------------------*/
#define CSBiaoZhi1  'J'
#define CSBiaoZhi2  'S'
#define CSBiaoZhi3  'J'

//#define CSBiaoZhi1  'z'
//#define CSBiaoZhi2  'z'
//#define CSBiaoZhi3  'S'

#define TestBiaoZhi1  'T'           //;zrt 20130904   2014012217
#define TestBiaoZhi2  'E'
#define TestBiaoZhi3  'S' 
#define TestBiaoZhi4  'T'


/*�¼���ض���*/

#define ERCReadNum   256  //;Ҫ��ERC_Num�ж�ȡ�ĵ�����
#define ERCPointNum  264  //;������¼������� ÿҳ8�� ����33ҳ
#define ERCAreaNum   66   //;ÿ�㱣����ֽ�
#define ERCPagePoint (PAGESIZE / ERCAreaNum)    //;ÿҳ����ĵ�
#define ERCPageNum   (ERCPointNum / ERCPagePoint)  //;������ҳ��

/*�ն�����ض���*/
#define DDJPointNum  32   //;�������
#define DDJPointPage 2    //;ÿ�㱣��2ҳ
#define DDJPageNum   (DDJPointNum * DDJPointPage)  //;������ҳ��

/*�����ն�����ض���*/
#define BDJPointNum  32
#define BDJPointPage 1
#define BDJPageNum   (BDJPointNum * BDJPointPage)

/*�¶�����ض���*/
#define MDJPointNum  13
#define MDJPointPage 2
#define MDJPageNum   (MDJPointNum * MDJPointPage)

/*���߶�����ض���*/
#define QDJPointNum  2976//;������� = ��*24*4
#define QDJAreaNum   111  //;ÿ�㱣����ֽ�
#define QDJPagePoint (PAGESIZE / QDJAreaNum)   //;ÿҳ����ĵ� 4
#define QDJPageNum   (QDJPointNum / QDJPagePoint) //;������ҳ�� 744 

/*Сʱ������ض���*/
//;����ʱ���档���浱ǰСʱ����һСʱ���ݲ����ݡ�
#define HDJPointPage 1   
#define HDJPageNum   2   //;ÿ����������4ҳ

/*������ֵͳ��������ض���*/
#define  MVTPointPage  1
#define  MVTPageNum    1 //;ÿ����������1ҳ

/*����������������ض���*/
#define  PEZPointPage 1
#define  PEZPageNum   1  //;ÿ����������1ҳ

//;Ϊÿ������������ҳ��.
#define PnZPage (MDJPageNum + DDJPageNum + BDJPageNum + QDJPageNum\
                 +HDJPageNum + MVTPageNum + PEZPageNum)


/* ���������� */
#define CSBock1     PROG_PAGE_END 
#define CSBock2     (CSBock1       +  1)
#define CSBock3     (CSBock2       +  1)


#define P1CSBock4     (CSBock3   + 1)
#define CSBock5       (P1CSBock4 + 1)
#define CSBock6       (CSBock5       +  1)
#define CSBock7       (CSBock6       +  1)
#define CSBock8       (CSBock7       +  1)
#define CSBock9       (CSBock8       +  1)
#define CSBock10      (CSBock9       +  1)

#define CSBock1Bak    (CSBock10       +  1) //;����������
#define CSBock2Bak    (CSBock1Bak    +  1)
#define CSBock3Bak    (CSBock2Bak    +  1)
#define P1CSBock4Bak  (CSBock3Bak   + 1)
#define CSBock5Bak    (P1CSBock4Bak  +  1)
#define CSBock6Bak    (CSBock5Bak    +  1)
#define CSBock7Bak    (CSBock6Bak    +  1)
#define CSBock8Bak    (CSBock7Bak    +  1)
#define CSBock9Bak    (CSBock8Bak    +  1)
#define CSBock10Bak   (CSBock9Bak    +  1)
#define CSEND         (CSBock10Bak    +  1)

/*�¼�����66ҳ*/
#define ERC1Home   (CSEND)
#define ERC2Home   (ERC1Home + ERCPageNum)
#define ERCEND     (ERC2Home + ERCPageNum)

/*����������PnZPage * PnMaxNum = 517* 10 = 5170ҳ*/
/*������1��*/
#define P1DDJHome  (ERCEND)
#define P1BDJHome  (P1DDJHome  + DDJPageNum)
#define P1MDJHome  (P1BDJHome  + BDJPageNum)
#define P1QDJHome  (P1MDJHome  + MDJPageNum)
#define P1NHDJHome (P1QDJHome  + QDJPageNum)
#define P1OHDJHome (P1NHDJHome + HDJPointPage)
#define P1MVTHome  (P1OHDJHome + HDJPointPage)
#define P1PEZHome  (P1MVTHome  + MVTPointPage)
#define P1DJEND    (P1PEZHome  + PEZPointPage)
#define PnDJEND    (P1DJEND)

#define FLASH_TEST PnDJEND+1            //;by zrt 20130904  ����flash����


//����10ҳ���ڱ���DLT104��Լ����@like
#define DLT104_PARAM_START  FLASH_TEST
#define DLT104_PARAM_END  (DLT104_PARAM_START + 10)


#define FLASHENDPAGE  DLT104_PARAM_END

#if FLASHENDPAGE > PAGECOUNT
    #error "More than total FLASHPAGE "
#endif


/*------------------------------------------------------------------------------
|| FM24C04���ݷ��䡣
------------------------------------------------------------------------------*/
#define FM24BYTESIZE  512

//#define _FM24_BZ1    'C'
//#define _FM24_BZ2    'J'
//#define _FM24_BZ3    'Q'
//#define _FM24_BZ4    'K'

#define _FM24_BZ1    'J'
#define _FM24_BZ2    'S'
#define _FM24_BZ3    'J'
#define _FM24_BZ4    'M'

/*X�ֽ�+1��У���*/
#define  BZByteNum     4        //;����
#define  VerByteNum   (4  +  1)  //;����汾��
#define  XDJByteNum   (2  +  1)  //;�գ��£������ն��ᱣ������
#define  QDJByteNum   (4  +  1)  //;���߱�������
#define  P0ERCByteNum (4  +  1)  //;�ն��¼���������
#define  ERCByteNum   (1  +  1)  //;�ն��¼�����
#define  ERCUpByteNum (1  +  1)  //;�������
#define  FluxByteNum  (11 +  1)  //;����

#define  E2R_BiaoZhi  0

/*ǰ200���ֽ�,���ڱ����ն����ò���*/
#define  Version      (E2R_BiaoZhi + BZByteNum)
#define  E2R_ERC1     (Version    + VerByteNum)
#define  E2R_ERC2     (E2R_ERC1   + P0ERCByteNum)
#define  E2R_EC1      (E2R_ERC2   + P0ERCByteNum)
#define  E2R_EC2      (E2R_EC1    + ERCByteNum)
#define  E2R_ERCUP    (E2R_EC2    + ERCByteNum)
//;#define  E2R_Flux     (E2R_ERCUP  + ERCUpByteNum)
#define  E2R_PowD     (E2R_ERCUP   + ERCUpByteNum)

/*���ڱ�����������ò��� 14 * 10 = 140byte*/
/*������1*/
#define  E2R_P1DDJ      200
#define  E2R_P1BDJ      (E2R_P1DDJ + XDJByteNum)
#define  E2R_P1MDJ      (E2R_P1BDJ + XDJByteNum)
#define  E2R_P1QDJ      (E2R_P1MDJ + XDJByteNum)
#define  E2R_PNEND      E2R_P1QDJ

#define  E2R_ByteNum    (E2R_PNEND + QDJByteNum)  //; ��E2R_XXXX + E2R_ByteNum���浽��Ӧ�ı�����

#define  _FM24_TEST     (E2R_ByteNum + 1)         //;FM24 ����by zrt 20130904

#if (E2R_ByteNum * 2 - 200) > FM24BYTESIZE
    #error "more than FM24SIZE"
#endif

#endif /*__GDW130_MMU_h */