/******************************************************************************
    ��Ȩ���У�
    �� �� ��: 1.0
    �� �� ��: dlt_104_init.c
    ��������: 
    ��	 �ߣ����
    ����˵����ʵ�֡�����Զ���ϵͳӦ��DL/T634.5104-2009ʵʩϸ�򡷵ĳ�ʼ������
    ����˵����
    �޸ļ�¼��
*******************************************************************************/
#include <stdio.h>
#include <string.h>

#include "dlt_104_lib.h"
#include "dlt_104_port_sys.h"
#include "dlt_104_cfg.h"
#include "dlt_104_port_com.h"

#define PRIO 0
//#define CFG_DEBUG


struct param_process {
	char step;
	char FCB;   /* ��¼���յ�������վ���ĵ�֡����������Ӧ������վ */
	char FCV;

	char M_FCB; /* ��Ϊ����վ������*/
	char M_FCV;

	char TYP;
	char SQ;
	char N;
	char SE;
};


static struct param_process arg[MAX_PORT];




/* RX[006]U:68 04 07 00 00 00  */
static int dlt_104_setup_link(unsigned char port, char *rxbuf, int len)
{
	char C1; 
	char STARTDT_cmd;
	
	C1 = rxbuf[2];
	
	if (((rxbuf[2] & 0x03) == 0x03) 
	    && ((rxbuf[4] & 0x01) == 0)) {
		
 		STARTDT_cmd = (C1 >> 2) & 0x03;
	
		/* U frame */	
		if (STARTDT_cmd == 1) 			
			return 1;
	}

	return 0;
}


static int dlt_104_setup_link_ack(unsigned char port, char *txbuf)
{
	char C1 = 0, C2 = 0, C3 = 0, C4 = 0; 
	char STARTDT_cmd = 2, TESTFR_cmd = 0, STOPDT_cmd = 0;

	txbuf[0] = 0x68;
	txbuf[1] = 4;
	C1 = 0x03 | STARTDT_cmd << 2 | TESTFR_cmd << 6 | STOPDT_cmd << 4 | 0x03;
	txbuf[2] = C1;
	txbuf[3] = C2;
	txbuf[4] = C3;
	txbuf[5] = C4;

	return 6;
}



static int dlt_104_init_end(unsigned char port, char *txbuf)
{
	struct dlt_lib  *arg = &dlt_lib_arg[port];

	txbuf[0] = 0x68;
	txbuf[1] = 10;
			
	txbuf[2] = 0; /*������C */
	txbuf[3] = 0;
	txbuf[4] = 0;
	txbuf[5] = 0;

	txbuf[6] = 70; /* TI */
	txbuf[7] = 0;  /* VSQ */	
	
	txbuf[8] = 4;  /* ����ԭ��COT */
	txbuf[9] = 0;
 	
	txbuf[10] = (unsigned char)arg->asdu; /* ASDU������ַ */
	txbuf[11] = (unsigned char)(arg->asdu >> 8);

	
	txbuf[12] = 0; /* ��Ϣ�����ַ */
	txbuf[13] = 0;
	txbuf[14] = 0;

	txbuf[15] = 4; /* ��ʼ��ԭ��COI */

	return 16;
}



int init_process_init(unsigned char port)
{
	memset((char *)&arg[port], 0, sizeof(arg[port]));
	return 0;
}

	

int init_process(unsigned char port)
{
	char step = arg[port].step;
	int ret;

	switch(step) {
	case 0: /* setup link --> */
		ret = com_rx(dlt_104_setup_link, port, PRIO);
		if (ret == 1) {
			step++;
		}
		break;

	case 1: /* setup link ack <-- */
		ret = com_tx(dlt_104_setup_link_ack, port, PRIO);
		if (ret == 1) {
			step++;
		}
		break;

	case 2: /* init end <-- */
		ret = com_tx(dlt_104_init_end, port, PRIO);
		if (ret == 1) {
			step = 0;
		}
		break;

	default:
		init_process_init(port);
		return -1;
	}

	arg[port].step = step;
	return 0;
}








