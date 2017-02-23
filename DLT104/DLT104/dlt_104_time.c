/******************************************************************************
    ��Ȩ���У�
    �� �� ��: 1.0
    �� �� ��: dlt_104_time.c
    ��������: 
    ��	 �ߣ� ���
    ����˵����ʵ�֡�����Զ���ϵͳӦ��DL/T634.5104-2009ʵʩϸ��
    ����˵����ʱ��ͬ������
    �޸ļ�¼��
*******************************************************************************/
#include <stdio.h>
#include <string.h>

#include "dlt_104_lib.h"
#include "dlt_104_port_sys.h"
#include "dlt_104_port_com.h"
#include "dlt_104_cfg.h"


#define PRIO 2


//#define CFG_DEBUG

#ifdef CFG_DEBUG
    #define Print(fmt,args...) printf(fmt, ##args)
#else
    #define Print(fmt,args...)
#endif



struct param_process {
	char FCB;   /* ��¼���յ�������վ���ĵ�֡����������Ӧ������վ */
	char FCV;

	char M_FCB; /* ��Ϊ����վ������*/
	char M_FCV;

	char TYP;
	char SQ;
	char N;
	char SE;
	
	char no;

	char syn_step;
	char read_step;
	

};


static struct param_process arg[MAX_PORT];


static int dlt_104_time_syn(unsigned char port, char *rxbuf, int len)
{
	char TYP, VSQ, ASDU, COT;
	struct dlt_lib  *arg = &dlt_lib_arg[port];
	unsigned int point_addr = 0;
	
	Print("unpack time syn cmd\r\n");	

	TYP = rxbuf[6]; /* TI */
	if (TYP != 103) {
		Print("unpack time syn TI error\r\n");		
		return 0;
	}

	/* �ɱ䳤���޶��� */
	VSQ = rxbuf[7];
	if (VSQ != 1) {
		Print("unpack time syn VSQ error\r\n");	
		return 0;
	}

	/*����ԭ�� */
	COT = rxbuf[9] * 0x100 + rxbuf[8];
	if (COT != 6) {
		Print("unpack time syn COT error\r\n");	
		return 0;
	}
	
	/* ASDU ������ַ */
	ASDU = rxbuf[11] * 0x100 + rxbuf[10];
	if (ASDU != arg->asdu) { 
		Print("unpack time syn asdu error\r\n");	
		return 0;
	}

	/* ��Ϣ�����ַ  */
	point_addr = rxbuf[14] * 0x10000 + rxbuf[13] * 0x100 + rxbuf[12];
	if (point_addr != 0) { 
		Print("unpack time syn point addr error\r\n");	
		return 0;
	}

	dlt_104_cp56time2a_write(rxbuf + 15);
			
	return 1;
}



static int dlt_104_time_syn_ack(unsigned char port, char *txbuf)
{
	struct dlt_lib  *arg = &dlt_lib_arg[port];
	
	txbuf[0] = 0x68;
	txbuf[1] = 20;
			
	txbuf[2] = 0; /*������C */
	txbuf[3] = 0;
	txbuf[4] = 0;
	txbuf[5] = 0;

	txbuf[6] = 103; /* TI */
	txbuf[7] = 1;  /* VSQ */	
	
	txbuf[8] = 7;  /* ����ԭ��COT ����ȷ�� */
	txbuf[9] = 0;
 	
	txbuf[10] = (unsigned char)arg->asdu; /* ASDU������ַ */
	txbuf[11] = (unsigned char)(arg->asdu >> 8);
	
	txbuf[12] = 0; /* ��Ϣ�����ַ */
	txbuf[13] = 0;
	txbuf[14] = 0;

	dlt_104_cp56time2a_read(txbuf + 15); /* cp56time2a  7 bytes */

	return 22;
}



int time_syn_process(unsigned char port)
{
	char step = arg[port].syn_step;
	int ret;

	switch(step) {
	case 0: /* time syn --> */
		Print("time syn\r\n");
		ret = com_rx(dlt_104_time_syn, port, PRIO);
		if (ret == 1)
			step++;			
		break;

	case 1: /* time syn ack <-- */
		ret = com_tx(dlt_104_time_syn_ack, port, PRIO);
		if (ret == 1)
			step = 0;
		break;
		
	default:
		step = 0;
		break;
	}

	arg[port].syn_step = step;
	return step;
}


static int dlt_104_time_read(unsigned char port, char *rxbuf, int len)
{
	char TYP, VSQ, ASDU, COT;
	struct dlt_lib  *arg = &dlt_lib_arg[port];
	unsigned int point_addr = 0;
	
	Print("unpack time read\r\n");	

	TYP = rxbuf[6]; /* TI */
	if (TYP != 103) {
		Print("unpack time read TI error\r\n");		
		return 0;
	}

	/* �ɱ䳤���޶��� */
	VSQ = rxbuf[7];
	if (VSQ != 1) {
		Print("unpack time read VSQ error\r\n");	
		return 0;
	}

	/*����ԭ�� */
	COT = rxbuf[9] * 0x100 + rxbuf[8];
	if (COT != 5) {
		Print("unpack time read COT error\r\n");	
		return 0;
	}
	
	/* ASDU ������ַ */
	ASDU = rxbuf[11] * 0x100 + rxbuf[10];
	if (ASDU != arg->asdu) { 
		Print("unpack time read asdu error\r\n");	
		return 0;
	}

	/* ��Ϣ�����ַ  */
	point_addr = rxbuf[14] * 0x10000 + rxbuf[13] * 0x100 + rxbuf[12];
	if (point_addr != 0) { 
		Print("unpack time read point addr error\r\n");	
		return 0;
	}
			
	return 1;
}


static int dlt_104_time_read_ack(unsigned char port, char *txbuf)
{
	struct dlt_lib  *arg = &dlt_lib_arg[port];
	
	txbuf[0] = 0x68;
	txbuf[1] = 20;
			
	txbuf[2] = 0; /*������C */
	txbuf[3] = 0;
	txbuf[4] = 0;
	txbuf[5] = 0;

	txbuf[6] = 103; /* TI */
	txbuf[7] = 1;  /* VSQ */	
	
	txbuf[8] = 5;  /* ����ԭ��COT ����ȷ�� */
	txbuf[9] = 0;
 	
	txbuf[10] = (unsigned char)arg->asdu; /* ASDU������ַ */
	txbuf[11] = (unsigned char)(arg->asdu >> 8);
	
	txbuf[12] = 0; /* ��Ϣ�����ַ */
	txbuf[13] = 0;
	txbuf[14] = 0;

	dlt_104_cp56time2a_read(txbuf + 15); /* cp56time2a  7 bytes */

	return 22;
}



int time_read_process(unsigned char port)
{
	char step = arg[port].read_step;
	int ret;

	switch(step) {
	case 0: /* time read --> */
		ret = com_rx(dlt_104_time_read, port, PRIO);
		if (ret == 1)
			step++;			
		break;

	case 1: /* time read ack <-- */
		ret = com_tx(dlt_104_time_read_ack, port, PRIO);
		if (ret == 1)
			step = 0;
		break;
		
	default:
		step = 0;
		break;
	}

	arg[port].read_step = step;
	return step;
}


int time_process_init(unsigned char port)
{
	memset((char *)&arg[port], 0, sizeof(arg[port]));
	return 0;
}


