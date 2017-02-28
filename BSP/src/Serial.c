/*
********************************************************************************
*  �� �� ����Serial.c
*
*  �ļ�����������������غ���
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
#include <string.h>
#include <stdio.h>
#include <ucos_ii.h>

#include "BSP_CFG.h"
#include "Serial.h"

struct uart2_rec_ctl uart2;
struct uart1_rec_ctl uart1;
struct uart5_rec_ctl uart5;

#ifdef Local_USART1_ON
/*----------------------------------------------------------------------------*
 |  ����1��ʼ��������
 *----------------------------------------------------------------------------*/
void SendFirstByte_USART1(void)
{
    U1DS.TBusy = 5;
    USART_SendData(USART1, (u16)*(U1DS.BufAdr + 0));
    U1DS.TIndex = 1;
    USART_ITConfig(USART1, USART_IT_TXE, ENABLE); 
}

/*----------------------------------------------------------------------------*
 |  ����1�жϷ�������
 *----------------------------------------------------------------------------*/
void SendByte_USART1(void)
{
    if((USART1->SR & USART_FLAG_TXE))
    {
        U1DS.TBusy = 30;
        if(U1DS.TIndex >= U1DS.TLong)
        {
            U1DS.TLong = 0;
            U1DS.TBusy = 0;
            U1DS.SedOrd = 0; //;���������֡���
            U1DS.BufAdr = 0;
            USART_ITConfig(USART1, USART_IT_TXE, DISABLE);           
            return ;
        }
        USART_SendData(USART1, (u16)*(U1DS.BufAdr + U1DS.TIndex));
        U1DS.TIndex++;
    }
}


/*----------------------------------------------------------------------------*
 |  ����1�жϽ�������
 *----------------------------------------------------------------------------*/
//void ReceiveByte_USART1(void)
//{
//    int temp;
//    u8 ch;
//
//    if(USART1->SR & USART_FLAG_RXNE)
//    {
//        U1DS.RecRun = true;
//        U1DS.RBet2Byte = 0x00;
//        temp = USART_ReceiveData(USART1);
//        ch = (u8)temp;
//        AllBuf.Rx1_1[U1DS.RIndex] = ch;
//        U1DS.RIndex++;
//        if(U1DS.RIndex >= U1BUFSIZE)
//        {
//            U1DS.RIndex = 0;
//        }
//    }
//}


/*----------------------------------------------------------------------------*
 |  ����1��������
 *----------------------------------------------------------------------------*/
void USART1_Rece(void)
{
    if(U1DS.RBet2Byte >= U1FRAMEDIST)          																					//;����2�������һ���ֽ��Ѿ�50ms,�Խ��յ������ݽ���Ӧ��
    {
        U1DS.RecRun = false;
        U1DS.RBet2Byte = 0x00;

        if(U1DS.RIndex > U1BUFSIZE)
        {
            U1DS.RIndex = 0;
            return;
        }
        
        U1DS.RecLen = U1DS.RIndex;
        U1DS.RIndex = 0;
        U1DS.RecOrd = true; //;�����յ�����
        U1DS.BufAdr = AllBuf.Rx1_1; //;ָ��������ĵ�ַ
    }
}


/*----------------------------------------------------------------------------*
 |  ����1��������
 *----------------------------------------------------------------------------*/
void USART1_Send(void)
{
    if(U1DS.TLong > U1BUFSIZE)
    {
        return;
    }    
    
    if(U1DS.TBusy == 0)
    {
        if(U1DS.SedOrd == 'E') //;��������
        {
            U1DS.SedOrd = 'S'; //;���ڷ���
            SendFirstByte_USART1();
        }
    }
}
#endif

/*----------------------------------------------------------------------------*
 |  ����2��ʼ��������
 *----------------------------------------------------------------------------*/
void SendFirstByte_USART2(void)
{
    U2DS.TBusy = 5;
    USART_SendData(USART2, (u16)*(U2DS.BufAdr + 0));
    U2DS.TIndex = 1;  
    USART_ITConfig(USART2, USART_IT_TXE, ENABLE);   
}


/*----------------------------------------------------------------------------*
 |  ����2�жϷ�������
 *----------------------------------------------------------------------------*/
void SendByte_USART2(void)
{    
    if ((USART2->SR & USART_FLAG_TXE))
    {
        //U2DS.TBusy = 5;
        if(U2DS.TIndex >= U2DS.TLong)
        {
            U2DS.TLong = 0;
            U2DS.TBusy = 0;
            U2DS.SedOrd = 0; //;���������֡���
            U2DS.BufAdr = 0;
            USART_ITConfig(USART2, USART_IT_TXE, DISABLE);                                      
            return ;
        }
        USART_SendData(USART2, (u16)*(U2DS.BufAdr + U2DS.TIndex));
        U2DS.TIndex++;
    }
}






/*----------------------------------------------------------------------------*
 | ����2�������� 
 *----------------------------------------------------------------------------*/
void USART2_Rece(void)
{  
    if(U2DS.RBet2Byte >= U2FRAMEDIST)          																					//;����2�������һ���ֽ��Ѿ�50ms,�Խ��յ������ݽ���Ӧ��
    {
        U2DS.RecRun = false;
        U2DS.RBet2Byte = 0x00;

        if(U2DS.RIndex > U2RXBUFSIZE)
        {
            U2DS.RIndex = 0;
            return;
        }
        
        U2DS.RecLen = U2DS.RIndex;
        U2DS.RIndex = 0;
        U2DS.RecOrd = true; //;�����յ�����
        U2DS.BufAdr = AllBuf.Rx2_1; //;ָ��������ĵ�ַ
    }
}


/*----------------------------------------------------------------------------*
 |  ����2��������
 *----------------------------------------------------------------------------*/
void USART2_Send(void)
{
    if(U2DS.TLong > U2TXBUFSIZE)
    {
        return;
    }    
    
    if(U2DS.TBusy == 0)
    {
        if(U2DS.SedOrd == 'E') //;��������
        {    
            U2DS.SedOrd = 'S'; //;���ڷ���
            SendFirstByte_USART2();
        }
    }
}

/*----------------------------------------------------------------------------*
 |  ����3��ʼ��������
 *----------------------------------------------------------------------------*/
void SendFirstByte_USART3(void)
{
    U3DS.TBusy = 5;
    USART_SendData(USART3, (u16)*(U3DS.BufAdr + 0));
    U3DS.TIndex = 1;
    USART_ITConfig(USART3, USART_IT_TXE, ENABLE); 
}

/*----------------------------------------------------------------------------*
 |  ����3�жϷ�������
 *----------------------------------------------------------------------------*/
void SendByte_USART3(void)
{
    if((USART3->SR & USART_FLAG_TXE))
    {
        U3DS.TBusy = 20;
        if(U3DS.TIndex >= U3DS.TLong)
        {
            U3DS.TLong = 0;
            U3DS.TBusy = 0;
            U3DS.SedOrd = 0; //;���������֡���
            U3DS.BufAdr = 0;
            USART_ITConfig(USART3, USART_IT_TXE, DISABLE); 
            USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
            return ;
        }
        USART_SendData(USART3, (u16)*(U3DS.BufAdr + U3DS.TIndex));
        U3DS.TIndex++;
    }
}


/*----------------------------------------------------------------------------*
 |  ����3�жϽ�������
 *----------------------------------------------------------------------------*/
void ReceiveByte_USART3(void)
{
    int temp;
    u8 ch;

    if(USART3->SR & USART_FLAG_RXNE)
    {
        U3DS.RecRun = true;
        U3DS.RBet2Byte = 0x00;
        temp = USART_ReceiveData(USART3);
        ch = (u8)temp;
        AllBuf.Rx3_1[U3DS.RIndex] = ch;
        U3DS.RIndex++;
        if(U3DS.RIndex >= U3BUFSIZE)
        {
            U3DS.RIndex = 0;
        }
    }
}


/*----------------------------------------------------------------------------*
 |  ����3��������
 *----------------------------------------------------------------------------*/
void USART3_Rece(void)
{
    if(U3DS.RBet2Byte >= U3FRAMEDIST)          																					//;����2�������һ���ֽ��Ѿ�50ms,�Խ��յ������ݽ���Ӧ��
    {
        U3DS.RecRun = false;
        U3DS.RBet2Byte = 0x00;

        if(U3DS.RIndex > U3BUFSIZE)
        {
            U3DS.RIndex = 0;
            return;
        }
       
        if(U3DS.RIndex < 10)
        {
             U3DS.RIndex = 0;
             return;
        }
        
        U3DS.RecLen = U3DS.RIndex;
        U3DS.RIndex = 0;
        U3DS.RecOrd = true; //;�����յ�����
        U3DS.BufAdr = AllBuf.Rx3_1; //;ָ��������ĵ�ַ
    }
}


/*----------------------------------------------------------------------------*
 |  ����3��������
 *----------------------------------------------------------------------------*/
void USART3_Send(void)
{
    if(U3DS.TLong > U3BUFSIZE)
    {
        return;
    }    
    
    if(U3DS.TBusy == 0)
    {
        if(U3DS.SedOrd == 'E') //;��������
        {
            USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
            U3DS.SedOrd = 'S'; //;���ڷ���
            SendFirstByte_USART3();
        }
    }
}

#ifdef PORT2USART
/*----------------------------------------------------------------------------*
 |  ����4��ʼ��������
 *----------------------------------------------------------------------------*/
void SendFirstByte_UART4(void)
{
    U4DS.TBusy = 5; 
    USART_SendData(UART4, (u16)*(U4DS.BufAdr + 0));
    U4DS.TIndex = 1;
    USART_ITConfig(UART4, USART_IT_TXE, ENABLE);  
}

/*----------------------------------------------------------------------------*
 |  ����4�жϷ�������
 *----------------------------------------------------------------------------*/
void SendByte_UART4(void)
{
    if((UART4->SR & USART_FLAG_TXE))
    {
        U4DS.TBusy = 5;
        if(U4DS.TIndex >= U4DS.TLong)
        {
            U4DS.TLong = 0;
            U4DS.TBusy = 0;
            U4DS.SedOrd = 0; //;���������֡���
            U4DS.BufAdr = 0;
            USART_ITConfig(UART4, USART_IT_TXE, DISABLE);
            return ;
        }
        USART_SendData(UART4, (u16)*(U4DS.BufAdr + U4DS.TIndex));
        U4DS.TIndex++;
    }
}


/*----------------------------------------------------------------------------*
 | ����4�жϽ������� 
 *----------------------------------------------------------------------------*/
void ReceiveByte_UART4(void)
{
    int temp;
    u8 ch;
    
    if(UART4->SR & USART_FLAG_RXNE)
    {
        U4DS.RecRun = true;
        U4DS.RBet2Byte = 0x00;
        temp = USART_ReceiveData(UART4);
        ch = (u8)temp;
        AllBuf.Rx4_1[U4DS.RIndex] = ch;
        U4DS.RIndex++;
        if(U4DS.RIndex >= U4BUFSIZE)
        {
            U4DS.RIndex = 0;
        }
    }
}


/*----------------------------------------------------------------------------*
 |  ����4��������
 *----------------------------------------------------------------------------*/
void UART4_Send(void)
{
    if(U4DS.TLong > U4BUFSIZE)
    {
        return;
    }    
    
    if(U4DS.TBusy == 0)
    {
        if(U4DS.SedOrd == 'E') //;��������
        {
            U4DS.SedOrd = 'S'; //;���ڷ���
            SendFirstByte_UART4();
        }
    }
}


/*----------------------------------------------------------------------------*
 |  ����4��������
 *----------------------------------------------------------------------------*/
void UART4_Rece(void)
{
    if(U4DS.RBet2Byte >= U4FRAMEDIST)  //;�յ�����
    {
        U4DS.RecRun = false;
        U4DS.RBet2Byte = 0x00;

        if(U4DS.RIndex > U4BUFSIZE)
        {
            U4DS.RIndex = 0;
            return;
        }
        
        U4DS.RecLen = U4DS.RIndex;
        U4DS.RIndex = 0;
        U4DS.RecOrd = true;
        U4DS.BufAdr = AllBuf.Rx4_1; //;ָ��������ĵ�ַ
    }
}
#endif

/*----------------------------------------------------------------------------*
 |  ����5��ʼ��������
 *----------------------------------------------------------------------------*/
void SendFirstByte_UART5(void)
{
    U5DS.TBusy = 5;
    USART_SendData(UART5, (u16)*(U5DS.BufAdr + 0));
    U5DS.TIndex = 1;
    USART_ITConfig(UART5, USART_IT_TXE, ENABLE); 
    RS485CTRLSED();
}


/*----------------------------------------------------------------------------*
 |  ����5�жϷ�������
 *----------------------------------------------------------------------------*/
void SendByte_UART5(void)
{
    if ((UART5->SR &USART_FLAG_TXE))
    {
        U5DS.TBusy = 5;
        if (U5DS.TIndex >= U5DS.TLong)
        {
            U5DS.TLong = 0;
            U5DS.TBusy = 0;
            U5DS.SedOrd = 0; //;���������֡���
            U5DS.BufAdr = 0;
//;            Delay_01s();
            USART_ITConfig(UART5, USART_IT_TXE, DISABLE);
            RS485CTRLRCV();
            return ;
        }
        USART_SendData(UART5, (u16)*(U5DS.BufAdr + U5DS.TIndex));
        U5DS.TIndex++;
    }
}


/*----------------------------------------------------------------------------*
 |  ����5�жϽ�������
 *----------------------------------------------------------------------------*/
//void ReceiveByte_UART5(void)
//{
//    int temp;
//    u8 ch;
//    
//    if(UART5->SR & USART_FLAG_RXNE)
//    {
//        U5DS.RecRun = true;
//        U5DS.RBet2Byte = 0x00;
//        temp = USART_ReceiveData(UART5);
//        ch = (u8)temp;
//        AllBuf.Rx5_1[U5DS.RIndex] = ch;
//        U5DS.RIndex++;
//        if(U5DS.RIndex >= U5BUFSIZE)
//        {
//            U5DS.RIndex = 0; 
//        }
//    }
//}

/*----------------------------------------------------------------------------*
 |  ����5��������
 *----------------------------------------------------------------------------*/
void UART5_Rece(void)
{
    if(U5DS.RBet2Byte >= U5FRAMEDIST)        //;����5�������һ���ֽ��Ѿ�50ms,�Խ��յ������ݽ���Ӧ��
    {
        U5DS.RecRun = false;
        U5DS.RBet2Byte = 0x00;

        if(U5DS.RIndex > U5BUFSIZE)
        {
            U5DS.RIndex = 0;
            return;
        }
        
        U5DS.RecLen = U5DS.RIndex;
        U5DS.RIndex = 0;
        U5DS.RecOrd = true; //;�����յ�����
        U5DS.BufAdr = AllBuf.Rx5_1; //;ָ��������ĵ�ַ               
        U5DS.Mark = 'G';
    }
}


/*----------------------------------------------------------------------------*
 |  ����5��������
 *----------------------------------------------------------------------------*/
void UART5_Send(void)
{
    if(U5DS.TLong > U5BUFSIZE)
    {
        return;
    }    
    
    if(U5DS.TBusy == 0)
    {
        if(U5DS.SedOrd == 'E') //;��������
        {
            U5DS.SedOrd = 'S'; //;���ڷ���
            SendFirstByte_UART5();
        }
    }
}



/*----------------------------------------------------------------------------*
 |  ���ں���
 *----------------------------------------------------------------------------*/
void USART_Com(void)
{
    //UART5_Rece();      //;ģ������ͨѶ�˿�1 @like
    //UART5_Send();
    
#ifdef Local_USART1_ON
    //USART1_Rece();    //;mini USBģ����Դ���
    //USART1_Send();   @like
#endif    
  
#ifdef PORT2USART    
    //UART4_Rece();      //;ģ������ͨѶ�˿�2 (�Ѿ��ر�20141111)
    //UART4_Send();
#endif 
   
    //USART2_Rece();    //;ģ��GPRSͨѶ @like
    //USART2_Send();
    
    //USART3_Rece();    //;ģ�����ͨ��
    //USART3_Send();
}



int uart1_write(char *txbuf, int size)
{
	int i;
	char ove_ms;
	
	for (i = 0; i < size; i++) {
		USART_SendData(USART1, (u16)txbuf[i]);		
		ove_ms = 50;
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) {
			ove_ms--;
			if (ove_ms == 0)
				return -1;			
			OSTimeDly(1);
		}			
	}
	
	return 0;
}



int uart2_send_byte(char *txbuf, int size)
{
	int i;
	char ove_ms;
	
	for (i = 0; i < size; i++) {
		USART_SendData(USART2, (u16)txbuf[i]);		
		ove_ms = 50;
		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET) {
			ove_ms--;
			if (ove_ms == 0)
				return -1;			
			OSTimeDly(1);
		}			
	}
	
	memset((char *)&uart2, 0, sizeof(struct uart2_rec_ctl));
	
	return 0;
}



int uart5_send_byte(char *txbuf, int size)
{
	int i;
	char ove_ms;
	
	for (i = 0; i < size; i++) {
		USART_SendData(UART5, (u16)txbuf[i]);		
		ove_ms = 50;
		while (USART_GetFlagStatus(UART5, USART_FLAG_TXE) == RESET) {
			ove_ms--;
			if (ove_ms == 0)
				return -1;			
			OSTimeDly(1);
		}			
	}
	
	memset((char *)&uart5, 0, sizeof(struct uart5_rec_ctl));
	
	return 0;
}




void ReceiveByte_USART1(void)
{
	int temp;
    	u8 ch;
    
    	if (USART1->SR & USART_FLAG_RXNE) {
		if (uart1.rxlen < UART1_RX_BUFF_SIZE) {
			temp = USART_ReceiveData(USART1);
       			ch = (u8)temp;
       			uart1.rxbuf[uart1.rxlen++] = ch;
			uart1.interval_ms = 0;
    		}
    	}
}




void ReceiveByte_USART2(void)
{
	int temp;
    	u8 ch;
    
    	if (USART2->SR & USART_FLAG_RXNE) {
		if (uart2.rxlen < UART2_RX_BUFF_SIZE) {
			temp = USART_ReceiveData(USART2);
       			ch = (u8)temp;
       			uart2.rxbuf[uart2.rxlen++] = ch;
			uart2.interval_ms = 0;
    		}
    	}
}


void ReceiveByte_UART5(void)
{
	int temp;
    	u8 ch;
    
    	if (UART5->SR & USART_FLAG_RXNE) {
		if (uart5.rxlen < UART5_RX_BUFF_SIZE) {
			temp = USART_ReceiveData(UART5);
       			ch = (u8)temp;
       			uart5.rxbuf[uart5.rxlen++] = ch;
			uart5.interval_ms = 0;
    		}
    	}
}



int uart1_receive_packet(char *rxbuf, int size, unsigned short interval_ms)
{	
	char packet_receive_flag = 0;
	int len = 0;
	
	/* two data packet time is interval_ms */
	if (uart1.rxlen > 0) {
	     	if (uart1.interval_ms++ > interval_ms) {		
			uart1.interval_ms = 0;
			packet_receive_flag = 1;
		}
	}
	
	/* cat data from uart2 rx buff */
	if (packet_receive_flag == 1) {				
		
		USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
		
		if (uart1.rxlen <= size) {																
			len = uart1.rxlen;
			memcpy(rxbuf, uart1.rxbuf, len);
			
			memset(uart1.rxbuf, 0, UART1_RX_BUFF_SIZE);
			uart1.rxlen = 0;
		} else {
			len = size;
			memcpy(rxbuf, uart1.rxbuf, len);
			uart1.rxlen -= len;
			memcpy(uart1.rxbuf, uart1.rxbuf + len, uart1.rxlen);		
		}
	
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	}
	
	return len;
}



int uart1_read(char *rxbuf, int size, int ovt_ms)
{
	int len = 0;
	
	while (ovt_ms--) {
		len = uart1_receive_packet(rxbuf, size, 1000);
		if (len > 0) {
			break;
		}
		
		OSTimeDly(1);
	}
	
	return len;
}




int uart2_receive_packet(char *rxbuf, int size, unsigned short interval_ms)
{	
	char packet_receive_flag = 0;
	int len = 0;
	
	/* two data packet time is interval_ms */
	if (uart2.rxlen > 0) {
	     	if (uart2.interval_ms++ > interval_ms) {		
			uart2.interval_ms = 0;
			packet_receive_flag = 1;
		}
	}
	
	/* cat data from uart2 rx buff */
	if (packet_receive_flag == 1) {				
		
		USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
		
		if (uart2.rxlen <= size) {																
			len = uart2.rxlen;
			memcpy(rxbuf, uart2.rxbuf, len);
			
			memset(uart2.rxbuf, 0, UART2_RX_BUFF_SIZE);
			uart2.rxlen = 0;
		} else {
			len = size;
			memcpy(rxbuf, uart2.rxbuf, len);
			uart2.rxlen -= len;
			memcpy(uart2.rxbuf, uart2.rxbuf + len, uart2.rxlen);		
		}
	
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	}
	
	return len;
}



int uart5_receive_packet(char *rxbuf, int size, unsigned short interval_ms)
{	
	char packet_receive_flag = 0;
	int len = 0;
	
	/* two data packet time is interval_ms */
	if (uart5.rxlen > 0) {
	     	if (uart5.interval_ms++ > interval_ms) {		
			uart5.interval_ms = 0;
			packet_receive_flag = 1;
		}
	}
	
	/* cat data from uart5 rx buff */
	if (packet_receive_flag == 1) {				
		
		USART_ITConfig(UART5, USART_IT_RXNE, DISABLE);
		
		if (uart5.rxlen <= size) {																
			len = uart5.rxlen;
			memcpy(rxbuf, uart5.rxbuf, len);
			
			memset(uart5.rxbuf, 0, UART5_RX_BUFF_SIZE);
			uart5.rxlen = 0;
		} else {
			len = size;
			memcpy(rxbuf, uart5.rxbuf, len);
			uart5.rxlen -= len;
			memcpy(uart5.rxbuf, uart5.rxbuf + len, uart5.rxlen);		
		}
	
		USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
	}
	
	return len;
}


