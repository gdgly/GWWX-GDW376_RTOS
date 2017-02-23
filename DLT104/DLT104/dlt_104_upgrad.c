/******************************************************************************
    ��Ȩ���У�
    �� �� ��: 1.0
    �� �� ��: dlt_104_upgrad.c
    ��������: 
    ��	 �ߣ����
    ����˵����ʵ�֡�����Զ���ϵͳӦ��DL/T634.5104-2009ʵʩϸ�򡷵��������
    ����˵��������104��Լ����վ�ķ�ʽ�������վ�����������
    �޸ļ�¼��
*******************************************************************************/
#include <stdio.h>
#include <string.h>

#include "dlt_104_lib.h"
#include "dlt_104_port_sys.h"
#include "dlt_104_port_com.h"


struct upgrad_str {
	char FCB;   /* ��¼���յ�������վ���ĵ�֡����������Ӧ������վ */
	char FCV;

	char M_FCB; /* ��Ϊ����վ������*/
	char M_FCV;

	struct dtl_ovt ovt;
};


static struct upgrad_str process;




/******************************************************************************
    ����˵���������������� -->
    ���������
    ���������
    �� �� ֵ��
*******************************************************************************/
static int upgrad_start_cmd(char *rxbuf, int len)
{
	char TYP, COT;
	char C, DIR, PRM, FCB, FCV, FC;
	char SE;
	int index;

	index = 4;
	C = rxbuf[index++];

	DIR = C >> 7;  /* 0: ���б���; 1���б��� */
	if (DIR != 0) {
		return -1;
	}

	PRM = C >> 6;
	if (PRM != 1) { /* 0: �Ӷ�վ; 1:����վ */
		return -1;
	}

	FCB = C >> 5;
	FCV = C >> 4;
	process.FCB = FCB; /* ��¼��������λ */
	process.FCV = FCV;

	FC = C & 0x0F;	   /* ������ */
	if (FC != 3) {
		return -1;
	}

	index += 2; /* A */

	TYP = rxbuf[index++];
	if (TYP != 211) {
		return -1;
	}

	/* �ɱ䳤���޶��� */
	index += 1;

	/*����ԭ�� */
	COT = rxbuf[index++];
	if (COT != 6) {
		return -1;
	}

	index += 2; /* ASDU���õ�ַ */

	index += 2; /* ��Ϣ���ַ */

	SE = rxbuf[index++] >> 7; /* S/E  */
	if (SE != 1) {
		return -1;
	}

	return 0;
}


/******************************************************************************
    ����˵������������������Ӧ <--
    ���������
    ���������
    �� �� ֵ��
*******************************************************************************/
static int upgrad_start_cmd_res(char *txbuf)
{
	char C, DIR, PRM, FCB, FCV, FC;

	DIR = 1;  /* 0: ���б���; 1: ���б��� */
	PRM = 0;  /* 0: �Ӷ�վ; 1: ����վ */

	FCB = process.FCB;
	FCV = process.FCV;
	FC = 0;

	C = (DIR << 7) | (PRM << 6) | (FCB << 5)| (FCV << 4) | FC;
	txbuf[4] = C;

	return 0;
}



/******************************************************************************
    ����˵������������ȷ�� <--
    ���������
    ���������
    �� �� ֵ��
*******************************************************************************/
static int upgrad_start_ack(char *txbuf)
{
	char C, DIR, PRM, FCB, FCV, FC;
	char index;

	DIR = 1; /* 0: ���б���; 1: ���б��� */
	PRM = 1; /* 0: �Ӷ�վ; 1: ����վ */

	process.M_FCB &= ~0x01;
	FCB = process.M_FCB;

	FCV = 1;
	FC = 3;

	C = (DIR << 7) | (PRM << 6) | (FCB << 5)| (FCV << 4) | FC;

	index = 4;
	txbuf[index++] = C;  /* C */

	index += 2; /* A */

	txbuf[index++] = 211; /* TI */

	txbuf[index++] = 1; /* VSQ */
	txbuf[index++] = 7; /* COT */

	index += 2; /* ASDU */

	txbuf[index++] = 0; /* ��Ϣ���ַ */
	txbuf[index++] = 0;

	txbuf[index++] = 1 << 7; /* S/E = 1 */

	return index;
}



/******************************************************************************
    ����˵������������ȷ����Ӧ -->
    ���������
    ���������
    �� �� ֵ��
*******************************************************************************/
static int upgrad_start_ack_res(char *rxbuf, int len)
{
	char C, DIR, PRM, FCB, FCV, FC;

	C = rxbuf[1];

	DIR = C >> 7;  /* 0: ���б���; 1���б��� */
	if (DIR != 0) {
		return -1;
	}

	PRM = C >> 6;
	if (PRM != 0) { /* 0: �Ӷ�վ; 1:����վ */
		return -1;
	}

	FCB = C >> 5;
	FCV = C >> 4;
	if ((FCB != process.M_FCB) && (FCV != 1)) { /* ��¼��������λ */
		return -1;
	}

	FC = C & 0x0F; /* ������ */
	if (FC != 0) {
		return -1;
	}

	return 0;
}


/******************************************************************************
    ����˵����
    ���������
    ���������
    �� �� ֵ��
*******************************************************************************/
int process_upgrad_start(void)
{
	static char step;
	int ret;

	switch(step) {
	case 0: /* ������������ --> */
		ret = com_rx(upgrad_start_cmd, &process.ovt);
		if (ret == 1) {
			step++;
		}
		break;

	case 1: /* ��������������Ӧ <-- */
		ret = com_tx(upgrad_start_cmd_res);
		if (ret == 1) {
			step++;
		}
		break;

	case 2: /* ��������ȷ�� <-- */
		ret = com_tx(upgrad_start_ack);
		if (ret == 1) {
			step++;
		}
		break;

	case 3: /* ��������ȷ����Ӧ --> */
		ret = com_rx(upgrad_start_ack_res, &process.ovt);
		if (ret == 1) {
			step = 0;
		}
		break;

	default:
		step = 0;
		return -1;
	}

	return 0;
}



/******************************************************************************
    ����˵����������ֹ���� -->
    ���������
    ���������
    �� �� ֵ��
*******************************************************************************/
static int upgrad_cancel_cmd(char *rxbuf, int len)
{
	char TYP, COT;
	char C, DIR, PRM, FCB, FCV, FC;
	char SE;
	int index;

	index = 4;
	C = rxbuf[index++];

	DIR = C >> 7;  /* 0: ���б���; 1���б��� */
	if (DIR != 0) {
		return -1;
	}

	PRM = C >> 6;
	if (PRM != 1) { /* 0: �Ӷ�վ; 1:����վ */
		return -1;
	}

	FCB = C >> 5;
	FCV = C >> 4;
	process.FCB = FCB; /* ��¼��������λ */
	process.FCV = FCV;

	FC = C & 0x0F;	   /* ������ */
	if (FC != 3) {
		return -1;
	}

	index += 2; /* A */

	TYP = rxbuf[index++];
	if (TYP != 211) {
		return -1;
	}

	/* �ɱ䳤���޶��� */
	index += 1;

	/*����ԭ�� */
	COT = rxbuf[index++];
	if (COT != 8) {
		return -1;
	}

	index += 2; /* ASDU���õ�ַ */

	index += 2; /* ��Ϣ���ַ */

	SE = rxbuf[index++] >> 7; /* S/E  */
	if (SE != 0) {
		return -1;
	}

	return 0;
}


/******************************************************************************
    ����˵����������ֹ������Ӧ <--
    ���������
    ���������
    �� �� ֵ��
*******************************************************************************/
static int upgrad_cancel_cmd_res(char *txbuf)
{
	char C, DIR, PRM, FCB, FCV, FC;

	DIR = 1;  /* 0: ���б���; 1: ���б��� */
	PRM = 0;  /* 0: �Ӷ�վ; 1: ����վ */

	FCB = process.FCB;
	FCV = process.FCV;
	FC = 0;

	C = (DIR << 7) | (PRM << 6) | (FCB << 5)| (FCV << 4) | FC;
	txbuf[4] = C;

	return 0;
}



/******************************************************************************
    ����˵����������ֹȷ�� <--
    ���������
    ���������
    �� �� ֵ��
*******************************************************************************/
static int upgrad_cancel_ack(char *txbuf)
{
	char C, DIR, PRM, FCB, FCV, FC;
	char index;

	DIR = 1; /* 0: ���б���; 1: ���б��� */
	PRM = 1; /* 0: �Ӷ�վ; 1: ����վ */

	process.M_FCB &= ~0x01;
	FCB = process.M_FCB;

	FCV = 1;
	FC = 3;

	C = (DIR << 7) | (PRM << 6) | (FCB << 5)| (FCV << 4) | FC;

	index = 4;
	txbuf[index++] = C;  /* C */

	index += 2; /* A */

	txbuf[index++] = 211; /* TI */

	txbuf[index++] = 1; /* VSQ */
	txbuf[index++] = 9; /* COT */

	index += 2; /* ASDU */

	txbuf[index++] = 0; /* ��Ϣ���ַ */
	txbuf[index++] = 0;

	txbuf[index++] = 0 << 7; /* S/E = 1 */

	return index;
}



/******************************************************************************
    ����˵����������ֹȷ����Ӧ -->
    ���������
    ���������
    �� �� ֵ��
*******************************************************************************/
static int upgrad_cancel_ack_res(char *rxbuf, int len)
{
	char C, DIR, PRM, FCB, FCV, FC;

	C = rxbuf[1];

	DIR = C >> 7;  /* 0: ���б���; 1���б��� */
	if (DIR != 0) {
		return -1;
	}

	PRM = C >> 6;
	if (PRM != 0) { /* 0: �Ӷ�վ; 1:����վ */
		return -1;
	}

	FCB = C >> 5;
	FCV = C >> 4;
	if ((FCB != process.M_FCB) && (FCV != 1)) { /* ��¼��������λ */
		return -1;
	}

	FC = C & 0x0F; /* ������ */
	if (FC != 0) {
		return -1;
	}

	return 0;
}



/******************************************************************************
    ����˵����
    ���������
    ���������
    �� �� ֵ��
*******************************************************************************/
int process_upgrad_cancel(void)
{
	static char step;
	int ret;

	switch(step) {
	case 0: /* ������ֹ���� --> */
		ret = com_rx(upgrad_cancel_cmd, &process.ovt);
		if (ret == 1) {
			step++;
		}
		break;

	case 1: /* ������ֹ������Ӧ <-- */
		ret = com_tx(upgrad_cancel_cmd_res);
		if (ret == 1) {
			step++;
		}
		break;

	case 2: /* ������ֹȷ�� <-- */
		ret = com_tx(upgrad_cancel_ack);
		if (ret == 1) {
			step++;
		}
		break;

	case 3: /* ������ֹȷ����Ӧ --> */
		ret = com_rx(upgrad_cancel_ack_res, &process.ovt);
		if (ret == 1) {
			step = 0;
		}
		break;

	default:
		step = 0;
		return -1;
	}

	return step;
}



/******************************************************************************
    ����˵��������ִ������ -->
    ���������
    ���������
    �� �� ֵ��
*******************************************************************************/
static int upgrad_execute_cmd(char *rxbuf, int len)
{
	char TYP, COT;
	char C, DIR, PRM, FCB, FCV, FC;
	char SE;
	int index;

	index = 4;
	C = rxbuf[index++];

	DIR = C >> 7;  /* 0: ���б���; 1���б��� */
	if (DIR != 0) {
		return -1;
	}

	PRM = C >> 6;
	if (PRM != 1) { /* 0: �Ӷ�վ; 1:����վ */
		return -1;
	}

	FCB = C >> 5;
	FCV = C >> 4;
	process.FCB = FCB; /* ��¼��������λ */
	process.FCV = FCV;

	FC = C & 0x0F;	   /* ������ */
	if (FC != 3) {
		return -1;
	}

	index += 2; /* A */

	TYP = rxbuf[index++];
	if (TYP != 211) {
		return -1;
	}

	/* �ɱ䳤���޶��� */
	index += 1;

	/*����ԭ�� */
	COT = rxbuf[index++];
	if (COT != 6) {
		return -1;
	}

	index += 2; /* ASDU���õ�ַ */

	index += 2; /* ��Ϣ���ַ */

	SE = rxbuf[index++] >> 7; /* S/E  */
	if (SE != 0) {
		return -1;
	}

	return 0;
}


/******************************************************************************
    ����˵��������ִ��������Ӧ <--
    ���������
    ���������
    �� �� ֵ��
*******************************************************************************/
static int upgrad_execute_cmd_res(char *txbuf)
{
	char C, DIR, PRM, FCB, FCV, FC;

	DIR = 1;  /* 0: ���б���; 1: ���б��� */
	PRM = 0;  /* 0: �Ӷ�վ; 1: ����վ */

	FCB = process.FCB;
	FCV = process.FCV;
	FC = 0;

	C = (DIR << 7) | (PRM << 6) | (FCB << 5)| (FCV << 4) | FC;
	txbuf[4] = C;

	return 0;
}



/******************************************************************************
    ����˵��������ִ��ȷ�� <--
    ���������
    ���������
    �� �� ֵ��
*******************************************************************************/
static int upgrad_execute_ack(char *txbuf)
{
	char C, DIR, PRM, FCB, FCV, FC;
	char index;

	DIR = 1; /* 0: ���б���; 1: ���б��� */
	PRM = 1; /* 0: �Ӷ�վ; 1: ����վ */

	process.M_FCB &= ~0x01;
	FCB = process.M_FCB;

	FCV = 1;
	FC = 3;

	C = (DIR << 7) | (PRM << 6) | (FCB << 5)| (FCV << 4) | FC;

	index = 4;
	txbuf[index++] = C;  /* C */

	index += 2; /* A */

	txbuf[index++] = 211; /* TI */

	txbuf[index++] = 1; /* VSQ */
	txbuf[index++] = 7; /* COT */

	index += 2; /* ASDU */

	txbuf[index++] = 0; /* ��Ϣ���ַ */
	txbuf[index++] = 0;

	txbuf[index++] = 0 << 7; /* S/E = 1 */

	return index;
}



/******************************************************************************
    ����˵��������ִ��ȷ����Ӧ -->
    ���������
    ���������
    �� �� ֵ��
*******************************************************************************/
static int upgrad_execute_ack_res(char *rxbuf, int len)
{
	char C, DIR, PRM, FCB, FCV, FC;

	C = rxbuf[1];

	DIR = C >> 7;  /* 0: ���б���; 1���б��� */
	if (DIR != 0) {
		return -1;
	}

	PRM = C >> 6;
	if (PRM != 0) { /* 0: �Ӷ�վ; 1:����վ */
		return -1;
	}

	FCB = C >> 5;
	FCV = C >> 4;
	if ((FCB != process.M_FCB) && (FCV != 1)) { /* ��¼��������λ */
		return -1;
	}

	FC = C & 0x0F; /* ������ */
	if (FC != 0) {
		return -1;
	}

	return 0;
}



/******************************************************************************
    ����˵����
    ���������
    ���������
    �� �� ֵ��
*******************************************************************************/
int process_upgrad_execute(void)
{
	static char step;
	int ret;

	switch(step) {
	case 0: /* ����ִ������ --> */
		ret = com_rx(upgrad_execute_cmd, &process.ovt);
		if (ret == 1) {
			step++;
		}
		break;

	case 1: /* ����ִ��������Ӧ <-- */
		ret = com_tx(upgrad_execute_cmd_res);
		if (ret == 1) {
			step++;
		}
		break;

	case 2: /* ����ִ��ȷ�� <-- */
		ret = com_tx(upgrad_execute_ack);
		if (ret == 1) {
			step++;
		}
		break;

	case 3: /* ����ִ��ȷ����Ӧ --> */
		ret = com_rx(upgrad_execute_ack_res, &process.ovt);
		if (ret == 1) {
			step = 0;
		}
		break;

	default:
		step = 0;
		return -1;
	}

	return step;
}


/******************************************************************************
    ����˵����
    ���������
    ���������
    �� �� ֵ��
*******************************************************************************/
int dlt_104_process_upgrad(void)
{
	int ret;

	/* ��������������� */
	ret = process_upgrad_start();
	if (ret < 0) {
		return -1;
	}

	/* ��������������� */
	ret = process_upgrad_cancel();
	if (ret < 0) {
		return -1;
	}

	/* ִ�У�����/������������� */
	ret = process_upgrad_execute();
	if (ret < 0) {
		return -1;
	}

	return 0;
}


/******************************************************************************
    ����˵����
    ���������
    ���������
    �� �� ֵ��
*******************************************************************************/
int dlt_104_process_upgrad_init(void)
{
	memset((char *)&process, 0, sizeof(process));
	return 0;
}