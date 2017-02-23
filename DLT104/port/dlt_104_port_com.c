/******************************************************************************
    ��Ȩ���У�
    �� �� ��: 1.0
    �� �� ��: dlt_104_port_com.c
    ��������: 
    ��	 �ߣ����
    ����˵����ʵ�֡�����Զ���ϵͳӦ��DL/T634.5104-2009ʵʩϸ�򡷵�ͨ�Ų�ӿ�
    ����˵����
    �޸ļ�¼��
*******************************************************************************/
#include <stdio.h>
#include <string.h>
#include "dlt_104_cfg.h"
#include "dlt_104.h"
#include "dlt_104_lib.h"
#include "dlt_104_port_com.h"

#ifdef CFG_SYS_UCOS
  #include <ucos_ii.h>
  #include "Serial.h"
  #include "task_ptl.h"
#else /* CFG_SYS_LINUX */
  #include "eth_client.h"
#endif



#define CFG_DEBUG

#ifdef CFG_DEBUG
    #define Print(fmt,args...) printf(fmt, ##args)
#else
    #define Print(fmt,args...)
#endif



#define CFG_DEBUG
/* Ӧ�ù�Լ���ݵ�ԪAPDU��󳤶� */
#define COM_RXBUF_SIZE  256
#define COM_TXBUF_SIZE  256

#define WLAN_RXBUF_SIZE  256
#define WLAN_TXBUF_SIZE  256


char comtxbuf[COM_TXBUF_SIZE];
char comrxbuf[COM_RXBUF_SIZE];

char wlantxbuf[WLAN_TXBUF_SIZE];
char wlanrxbuf[WLAN_RXBUF_SIZE];


struct dtl_ovt
{
    	unsigned long timer;                        
    	char count;
};


struct port_com 
{
	unsigned char port;
	unsigned short size;
	unsigned short link_addr;
	char *txbuf;
	int txlen;
	char *rxbuf;
	int rxlen;
	char frame[256];
	int frame_len;
	int (*apdu_pack[MAX_PROCESS])(unsigned char port, char *txbuf);
	struct dtl_ovt ovt[MAX_PROCESS];
    	
	char fix_ovt_s;
    	unsigned long tick_ms;
    	char count_fix;
	char type; /* frame type, S, U, I */  
};


struct port_com comm[MAX_PORT];


int dlt_104_port_send(unsigned char port, char *buf, int len)
{
	int ret;

#ifdef CFG_SYS_UCOS	
	if (port == PORT_WLAN) {		
		ret = wlan_data_post(buf, len);
		if (ret < 0)
			return -1;
	}	
		
	if (port == PORT_COM) {		
		ret = uart1_write(buf, len);
		if (ret < 0)
			return -1;
	}

#else /* CFG_SYS_LINUX */
	if (port == PORT_ETH) {
		ret = eth_write(buf, len);
		if (ret < 0)
			return -1;
	}
#endif

	return 0;
}



int dlt_104_port_receive(unsigned char port, char *buff, int size)
{
	int len = 0;

#ifdef CFG_SYS_UCOS	

	if (port == PORT_WLAN) {
		len = wlan_data_pend(buff, size, 200);	
	}
	
	if (port == PORT_COM) {
		len = uart1_read(buff, size, 200);	
	}	

#else /* CFG_SYS_LINUX */
	if (port == PORT_ETH) {
		len = eth_read(buff, size, 200);
		if (len < 0)
			return -1;
	}
#endif


	
	return len;
}


void dlt_104_receive(unsigned char port)
{
	int len = 0;
	struct port_com *com;
	
	com = &comm[port];
		
	/* ��ͨ���������� */
	len = dlt_104_port_receive(port, com->rxbuf + com->rxlen, com->size - com->rxlen);
	if (len > 0) {
		com->rxlen += len;
	}
	
	if (com->rxlen >= 4) {			
		/* �ӻ������ҳ�һ֡ */
		memset(com->frame, 0, 256);
		len = dlt_104_frame_read(port, com->frame, com->rxbuf, &com->rxlen);
		if (len > 0)
			com->frame_len = len;		
	}	
}




int com_rx(int (*dlt_104_apdu_unpack)(unsigned char port, char *rxbuf, int len),
 		unsigned char port, unsigned char prio)
{
	struct dtl_ovt *ovt = &comm->ovt[prio];
	struct port_com *com = &comm[port];
	int ret;

	if (com->frame_len > 0) {	
		
		/* ����֡ */
		ret = dlt_104_apdu_unpack(port, com->frame, com->frame_len);
		if (ret == 1) {
			
			/* ������������ݰ�ɾ�� */
			memset(com->frame, 0, 256);
			com->frame_len = 0;
			
			ovt->timer = 0;
			ovt->count = 0;

			memset(com->txbuf, 0, com->txlen);
			com->txlen = 0;

			return 1; /* Ӧ��ɹ� */
		} 
	}


	/* ��ʱ�ط����� */
	ovt->timer++;
	if (((ovt->timer * com->tick_ms) / 1000) > com->fix_ovt_s) {
		ovt->timer = 0;

		/* ��ʱʱ�䵽û���յ���վ���ݣ����ͻ������ڻ������ݾ��ط� */
		if (com->txlen > 0) {
			/* �ط���֡ */
			ret = dlt_104_port_send(port, com->txbuf, com->txlen);
			if (ret < 0) {
				return -1;
			}
		} else {
			/* ��ʱʱ�䵽û���յ���վ���ݣ����ͻ�������û�����ݾ��˳� */
			ovt->count = 0;
			return 2;
		}

		/* �ط����� */
		ovt->count++;
		if (ovt->count > com->count_fix) {
			ovt->count = 0;
			
			/* ���ͻ�������� */
			memset(com->txbuf, 0, com->txlen);
			com->txlen = 0;
			return 2;
		}
	}

	return 0;
}



int com_tx(int (*dlt_104_apdu_pack)(unsigned char port, char *txbuf), unsigned char port, unsigned char prio)
{
	struct port_com *com = &comm[port];
	int ret;
	int i;
	char type;
	
	/* ��Ҫ���͵�ASDU֡���� */
	if (com->apdu_pack[prio] == 0)
		com->apdu_pack[prio] = dlt_104_apdu_pack;	
	
	/* �ȴ����ȼ��ߵĴ������ */
	for (i = 0; i < prio; i++) {
		if (com->apdu_pack[i] != 0) {
			printf("%d prio is low than %d!\r\n", prio, i);
			return 0;
		}
	}
					
	com->txlen = com->apdu_pack[prio](port, com->txbuf);

	/* ����֡ */
	if (com->txlen > 0) {
		type = dlt_104_frame_type(com->txbuf, com->txlen);
		
		/* set NR, SR */
		if ((type == 'I') || (type == 'S'))
			dlt_104_apci_pack(port, com->txbuf, com->txlen, type);
		
		ret = dlt_104_port_send(port, com->txbuf, com->txlen);
		if (ret == 0) {
#ifdef CFG_DEBUG		
			print_frame(port, com->txbuf, com->txlen, 'T');
#endif	
			com->apdu_pack[prio] = 0;
			memset(com->txbuf, 0, 256);
			com->txlen = 0;
			return 1;
		}
	}
	
	return 0;
}



int dlt_104_com_init(unsigned char port)
{	
	if (port == PORT_COM) {
		memset((char *)&comm[port], 0, sizeof(comm[port]));
		memset(comtxbuf, 0, COM_TXBUF_SIZE);
		memset(comrxbuf, 0, COM_RXBUF_SIZE);
		comm[port].port = PORT_COM;
		comm[port].size = COM_RXBUF_SIZE;
		comm[port].txbuf = comtxbuf;
		comm[port].rxbuf = comrxbuf;
		comm[port].link_addr = 1;
		
		comm[port].fix_ovt_s = 3;
		comm[port].tick_ms = 200;
		comm[port].count_fix = 3;
	}
	
	if (port == PORT_WLAN) {
		memset((char *)&comm[port], 0, sizeof(comm[port]));
		memset(wlantxbuf, 0, WLAN_TXBUF_SIZE);
		memset(wlanrxbuf, 0, WLAN_RXBUF_SIZE);
		comm[port].port = PORT_COM;
		comm[port].size = WLAN_RXBUF_SIZE;
		comm[port].txbuf = wlantxbuf;
		comm[port].rxbuf = wlanrxbuf;
		comm[port].link_addr = 1;

		comm[port].fix_ovt_s = 15;
		comm[port].tick_ms = 200;
		comm[port].count_fix = 3;
	}	



	
	return 0;
}
