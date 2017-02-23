/*
********************************************************************************
*  �� �� ����
*
*  �ļ�������
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
#ifndef __Function_CFG_H
#define __Function_CFG_H

//;���Ź�����
//#define IWDGON

//;������ΪUSB���㴮�����ÿ��� ;�رտ��أ��˿ڶ�Ϊ�������
//#define JTDK_USART1_ON  //;���ڼ����˿�1
#define Local_USART1_ON //;���ش����Ƿ�ر�

//;GPRS�����ݻ������Ӽ�ʱ
#define LINKTIME

//;#define ZS  //;���Լ�¼GPRS������Ϣ����
//;IAP����
//#define IAP

//;#define PORT2USART  //;����Ҫ�˿�2

//;���嵥Ƭ���ͺ�
#define STM32F10XXC


//;�汾��
#define VERSION        '1','.','2','6'
#define VERSION_DATE   0x10, 0x01, 0x15

#define  PNMAXNUM    1     //;���֧�ֲ�������Ŀ
#if PNMAXNUM >10
    #error "too many PNMAXNUM"
#endif

#define GPRSUSARTCTR   U2DS
#define GPRSREVBUF     AllBuf.Rx2_2
#define GPRSSEDBUF     AllBuf.Tx2_1
//;#define U2BUFSIZE      550
#define U2TXBUFSIZE      1024
#define U2RXBUFSIZE      512
#define U2FRAMEDIST    15

#define INFUSARTCTR    U3DS
#define INFREVBUF      AllBuf.Rx3_2
#define INFSEDBUF      AllBuf.Tx3_1
#define U3BUFSIZE      512
#define U3FRAMEDIST    25

/*
#define PORT1USARTCTR  U4DS
#define PORT1REVBUF    AllBuf.Rx4_2
#define PORT1SEDBUF    AllBuf.Tx4_1
#define U4BUFSIZE      256
#define U4FRAMEDIST    15

#define PORT2USARTCTR  U5DS
#define PORT2REVBUF    AllBuf.Rx5_2
#define PORT2SEDBUF    AllBuf.Tx5_1
#define U5BUFSIZE      256
#define U5FRAMEDIST    15
*/

#define PORT1USARTCTR   U5DS            //;�°������ ���ڻ��� ����5Ϊ����˿� 20141111
#define PORT1REVBUF    AllBuf.Rx5_2
#define PORT1SEDBUF    AllBuf.Tx5_1
#define U4BUFSIZE      256
#define U4FRAMEDIST    15

#define PORT2USARTCTR  U4DS
#define PORT2REVBUF    AllBuf.Rx4_2
#define PORT2SEDBUF    AllBuf.Tx4_1
#define U5BUFSIZE      256
#define U5FRAMEDIST    15

//;#define PORT1USARTCTR  U5DS
//;#define PORT1REVBUF    AllBuf.Rx5_2
//;#define PORT1SEDBUF    AllBuf.Tx5_1
//;#define U5BUFSIZE      256
//;#define U5FRAMEDIST    15
//;
//;#define PORT2USARTCTR  U4DS
//;#define PORT2REVBUF    AllBuf.Rx4_2
//;#define PORT2SEDBUF    AllBuf.Tx4_1
//;#define U4BUFSIZE      256
//;#define U4FRAMEDIST    15

#define LOCREVBUF     AllBuf.Rx1_2
#define LOCSEDBUF     AllBuf.Tx1_1
#define LOCUSARTCTR   U1DS
//#define U1BUFSIZE      550
#define U1BUFSIZE      1024
#define U1FRAMEDIST    15

#endif
