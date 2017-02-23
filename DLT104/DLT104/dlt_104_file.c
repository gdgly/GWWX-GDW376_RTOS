/******************************************************************************
    ��Ȩ���У�
    �� �� ��: 1.0
    �� �� ��: dlt_104_file.c
    ��������: 
    ��	 �ߣ����
    ����˵����ʵ�֡�����Զ���ϵͳӦ��DL/T634.5104-2009ʵʩϸ�򡷵��ļ�����
    ����˵��������104��Լ����վ�ķ�ʽ�������վ�����ļ�����
    �޸ļ�¼��
*******************************************************************************/
#include <stdio.h>
#include <string.h>

#include "dlt_104_lib.h"
#include "dlt_104_port_sys.h"
#include "dlt_104_port_com.h"


#define SEGMENT_SIZE 225      /* �����ݵ���󳤶� */


struct file_str {
	char FCB;   /* ��¼���յ�������վ���ĵ�֡����������Ӧ������վ */
	char FCV;

	char M_FCB; /* ��Ϊ����վ������*/
	char M_FCV;

	char dir_opt;
	char dir_id[4];
	char dir_name_len;
	char dir_name[20];
	char dir_flag;
	char start_time[7];
	char end_time[7];

	unsigned long file_size;
	unsigned long segment_no;
	char finish;

	struct dtl_ovt ovt;
};


static struct file_str process;	   /* �����ļ��ϴ������ع������õ���ȫ�ֱ��� */


/******************************************************************************
    ����˵�����ٻ�Ŀ¼���� -->
    ���������
    ���������
    �� �� ֵ��
*******************************************************************************/
int file_call_dir_cmd(char *rxbuf, int len)
{
	char TYP, COT;
	char C, DIR, PRM, FCB, FCV, FC;
	unsigned long point_addr;
	char poket_type;
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
	if (TYP != 210) {
		return -1;
	}

	/* �ɱ䳤���޶��� */
	index += 1;

	/*����ԭ�� */
	COT = rxbuf[index++];
	if (COT != 5) {
		return -1;
	}

	index += 2; /* ASDU���õ�ַ */

	point_addr = rxbuf[index + 1] * 0x100 + rxbuf[index]; /* ��Ϣ���ַ */
	index += 2;
	if (point_addr != 0) {
		return -1;
	}

	poket_type = rxbuf[index++]; /* �������ݰ����� */
	if (poket_type != 2) {
		return -1;
	}

	process.dir_opt = rxbuf[index++]; /* ������ʶ */

	/* Ŀ¼��ʶ */
	memcpy(process.dir_id,	rxbuf + index, 4);
	index += 4;

	process.dir_name_len = rxbuf[index++]; /* Ŀ¼������ */
	if ((process.dir_name_len < 20) && (process.dir_name_len > 0)) {
		memcpy(process.dir_name, rxbuf + index, process.dir_name_len); /* Ŀ¼�� */
		index += process.dir_name_len;
	}

	process.dir_flag = rxbuf[index++]; /* �ٻ���־ */

	memcpy(process.start_time, rxbuf + index, 7); /* ��ѯ��ʼʱ�� */
	index += 7;

	memcpy(process.end_time, rxbuf + index, 7); /* ��ѯ��ֹʱ�� */
	index += 7;

	return 0;
}


/******************************************************************************
    ����˵�����ٻ�Ŀ¼������Ӧ <--
    ���������
    ���������
    �� �� ֵ��
*******************************************************************************/
int file_call_dir_cmd_res(char *txbuf)
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
    ����˵�����ٻ�Ŀ¼ȷ�� <--
    ���������
    ���������
    �� �� ֵ��
*******************************************************************************/
int file_call_dir_ack(char *txbuf)
{
	char C, DIR, PRM, FCB, FCV, FC;
	char index, i;
	int ret;

	char file_name_len, file_num;
	char file_name[20];
	unsigned long file_size;
	char file_time[7];

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

	txbuf[index++] = 210; /* TI */

	txbuf[index++] = 1; /* VSQ */
	txbuf[index++] = 5; /* COT */

	index += 2; /* ASDU */

	txbuf[index++] = 0; /* ��Ϣ���ַ */
	txbuf[index++] = 0;

	txbuf[index++] = 2; /* �������ݰ����� */

	txbuf[index++] = process.dir_opt; /* ������ʶ */
	txbuf[index++] = 0;  /* ��������� 0���ɹ� 1��ʧ�� */

	memcpy(txbuf + index, process.dir_id, 4);  /* Ŀ¼ID */
	index += 4;

	txbuf[index++] = 0; /* ������־ */

	file_num = dlt_104_get_dir_file_num(process.dir_name);
	txbuf[index++] = file_num; /* �ļ����� */

	for (i = 0; i < file_num; i++) {
		if (i >= 10) {
			break; /* ����ϴ�10���ļ���Ϣ */
		}

		ret = dlt_104_get_dir_info(process.dir_name,
					   i,
					   &file_name_len,
					   file_name,
					   &file_size,
					   file_time);
		if (ret < 0) {
			break;
		}

		txbuf[index++] = file_name_len; /* �ļ����Ƴ��� */
		if ((file_name_len > 0) && (file_name_len < 20)) {
			memcpy(txbuf + index, file_name, file_name_len); /* �ļ����� */
			index += file_name_len;
		}

		txbuf[index++] = 0; /* �ļ����� */

		txbuf[index++] = (char)file_size; /* �ļ���С */
		txbuf[index++] = (char)(file_size >> 8);
		txbuf[index++] = (char)(file_size >> 16);
		txbuf[index++] = (char)(file_size >> 24);

		memcpy(txbuf + index, file_time, 7);
		index += 7; /* �ļ�ʱ��  */
	}

	return index;
}


/******************************************************************************
    ����˵����
    ���������
    ���������
    �� �� ֵ��
*******************************************************************************/
int file_call_dir_ack_res(char *rxbuf, int len)
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
    ����˵�������Ʒ�����ļ����ش���
    ���������
    ���������
    �� �� ֵ��
*******************************************************************************/
int process_file_call_dir(void)
{
	static char step;
	int ret;

	switch(step) {
	case 0: /* �ٻ�Ŀ¼���� --> */
		ret = com_rx(file_call_dir_cmd, &process.ovt);
		if (ret == 1) {
			step++;
		}
		break;

	case 1: /* �ٻ�Ŀ¼������Ӧ <-- */
		ret = com_tx(file_call_dir_cmd_res);
		if (ret == 1) {
			step++;
		}
		break;

	case 2: /* �ٻ�Ŀ¼ȷ�� <-- */
		ret = com_tx(file_call_dir_ack);
		if (ret == 1) {
			step++;
		}
		if (ret == 2) {
			step = 0xFF;
		}
		break;

	case 3: /* �ٻ�Ŀ¼ȷ����Ӧ --> */
		ret = com_rx(file_call_dir_ack_res, &process.ovt);
		if (ret == 1) {
			step = 0;
		}
		if (ret == 2) {
			step = 2;
		}
		break;

	default:
		step = 0;
		return -1;
	}

	return step;
}



/******************************************************************************
    ����˵�������ļ����� -->
    ���������
    ���������
    �� �� ֵ��
*******************************************************************************/
int file_upload_activate(char *rxbuf, int len)
{
	char TYP, COT;
	char C, DIR, PRM, FCB, FCV, FC;
	unsigned long point_addr;
	char poket_type;
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
	if (TYP != 210) {
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

	point_addr = rxbuf[index + 1] * 0x100 + rxbuf[index]; /* ��Ϣ���ַ */
	index += 2;
	if (point_addr != 0) {
		return -1;
	}

	poket_type = rxbuf[index++]; /* �������ݰ����� */
	if (poket_type != 2) {
		return -1;
	}

	process.dir_opt = rxbuf[index++]; /* ������ʶ */
	if (process.dir_opt != 3) {
		return -1;
	}


	process.dir_name_len = rxbuf[index++]; /* �ļ������� */
	if ((process.dir_name_len < 20) && (process.dir_name_len > 0)) {
		memcpy(process.dir_name, rxbuf + index, process.dir_name_len); /* �ļ��� */
		index += process.dir_name_len;
	}

	return 0;
}



/******************************************************************************
    ����˵�������ļ�������Ӧ <--
    ���������
    ���������
    �� �� ֵ��
*******************************************************************************/
int file_upload_activate_res(char *txbuf)
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
    ����˵�������ļ�����ȷ�� <--
    ���������
    ���������
    �� �� ֵ��
*******************************************************************************/
int file_upload_activate_ack(char *txbuf)
{
	char C, DIR, PRM, FCB, FCV, FC;
	char index;
	int ret;

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

	txbuf[index++] = 210; /* TI */

	txbuf[index++] = 1; /* VSQ */
	txbuf[index++] = 7; /* COT */

	index += 2; /* ASDU */

	txbuf[index++] = 0; /* ��Ϣ���ַ */
	txbuf[index++] = 0;

	txbuf[index++] = 2; /* �������ݰ����� */

	txbuf[index++] = 4;  /* ������ʶ */
	txbuf[index++] = 0;  /* ��������� 0���ɹ� 1��ʧ�� */

	txbuf[index++] = process.dir_name_len; /* �ļ����Ƴ��� */
	if ((process.dir_name_len > 0)
	    && (process.dir_name_len < 20)) {
		memcpy(txbuf + index, process.dir_name, process.dir_name_len); /* �ļ����� */
		index += process.dir_name_len;
	}

	ret = dlt_104_get_file_info(process.dir_name,
				    process.dir_name_len,
				    &process.file_size,
				    process.dir_id,
				    NULL);
	if (ret < 0) {
		return -1;
	}

	memcpy(txbuf + index, process.dir_id, 4);  /* �ļ�ID */
	index += 4;

	txbuf[index++] = (char)process.file_size; /* �ļ���С */
	txbuf[index++] = (char)(process.file_size >> 8);
	txbuf[index++] = (char)(process.file_size >> 16);
	txbuf[index++] = (char)(process.file_size >> 24);

	return index;
}



/******************************************************************************
    ����˵�������ļ�����ȷ����Ӧ -->
    ���������
    ���������
    �� �� ֵ��
*******************************************************************************/
int file_upload_activate_ack_res(char *rxbuf, int len)
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
    ����˵�������ļ����ݴ��� <--
    ���������
    ���������
    �� �� ֵ��
*******************************************************************************/
int file_upload_data(char *txbuf)
{
	char C, DIR, PRM, FCB, FCV, FC;
	char index;
	int ret;
	char len;
	char flag = 0;

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

	txbuf[index++] = 210; /* TI */

	txbuf[index++] = 1; /* VSQ */
	txbuf[index++] = 5; /* COT */

	index += 2; /* ASDU */

	txbuf[index++] = 0; /* ��Ϣ���ַ */
	txbuf[index++] = 0;

	txbuf[index++] = 2; /* �������ݰ����� */

	txbuf[index++] = 5;  /* ������ʶ */

	memcpy(txbuf + index, process.dir_id, 4);  /* �ļ�ID */
	index += 4;

	txbuf[index++] = (char)process.segment_no; /* �κ� */
	txbuf[index++] = (char)(process.segment_no >> 8);
	txbuf[index++] = (char)(process.segment_no >> 16);
	txbuf[index++] = (char)(process.segment_no >> 24);


	ret = dlt_104_read_file_segment(process.dir_name,
					process.dir_name_len,
					process.segment_no,
					txbuf + index + 1,
					&len,
					&flag);
	if (ret < 0) {
		return -1;
	}


	if (flag == 1) {
		txbuf[index++] = 1;  /* �к������� */
		process.finish = 0;
	} else {
		txbuf[index++] = 0;  /* �޺������� */
		process.finish = 1;
	}


	process.segment_no++; /* �κ��ۼ� */

	txbuf[index + len] = dlt_104_cs(txbuf + index, len); /* ��У�� */
	index += (len + 1);

	txbuf[index++] = (char)process.file_size; /* �ļ���С */
	txbuf[index++] = (char)(process.file_size >> 8);
	txbuf[index++] = (char)(process.file_size >> 16);
	txbuf[index++] = (char)(process.file_size >> 24);

	return index;
}


/******************************************************************************
    ����˵�������ļ����ݴ�����Ӧ -->
    ���������
    ���������
    �� �� ֵ��
*******************************************************************************/
int file_upload_data_res(char *rxbuf, int len)
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
    ����˵�������ļ����ݴ���ȷ�� -->
    ���������
    ���������
    �� �� ֵ��
*******************************************************************************/
int file_upload_data_ack(char *rxbuf, int len)
{
	char TYP, COT;
	char C, DIR, PRM, FCB, FCV, FC;
	unsigned long point_addr;
	char poket_type;
	int index;
	unsigned long segment_no;
	char dir_id[4];

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
	if (TYP != 210) {
		return -1;
	}

	/* �ɱ䳤���޶��� */
	index += 1;

	/*����ԭ�� */
	COT = rxbuf[index++];
	if (COT != 5) {
		return -1;
	}

	index += 2; /* ASDU���õ�ַ */

	point_addr = rxbuf[index + 1] * 0x100 + rxbuf[index]; /* ��Ϣ���ַ */
	index += 2;
	if (point_addr != 0) {
		return -1;
	}

	poket_type = rxbuf[index++]; /* �������ݰ����� */
	if (poket_type != 2) {
		return -1;
	}

	process.dir_opt = rxbuf[index++]; /* ������ʶ */
	if (process.dir_opt != 6) {
		return -1;
	}

	memcpy(dir_id, rxbuf + index, 4); /* �ļ�id*/
	index += 4;

	if  ((process.dir_id[0] != dir_id[0])
	     || (process.dir_id[1] != dir_id[1])
	     || (process.dir_id[2] != dir_id[2])
	     || (process.dir_id[3] != dir_id[3])) {
		return -1;
	}

	segment_no = rxbuf[index + 3] * 0x1000000
		     + rxbuf[index + 2] * 0x10000
		     + rxbuf[index + 1] * 0x100
		     + rxbuf[index]; /* �κ�*/
	index += 4;

	if (process.segment_no != segment_no) {
		return -1;
	}

	if (rxbuf[index] != 0) { /* �޺������� */
		return -1;
	}

	return 0;
}


/******************************************************************************
    ����˵�������ļ����ݴ���ȷ�� <--
    ���������
    ���������
    �� �� ֵ��
*******************************************************************************/
int file_upload_data_ack_res(char *txbuf)
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
    ����˵�����ļ��ϴ�����
    ���������
    ���������
    �� �� ֵ��
*******************************************************************************/
int process_file_upload_file(void)
{
	static char step;
	int ret;

	switch(step) {
	case 0: /* ���ļ����� --> */
		ret = com_rx(file_upload_activate, &process.ovt);
		if (ret == 1) {
			step++;
		}
		break;

	case 1: /* ���ļ�������Ӧ <-- */
		ret = com_tx(file_upload_activate_res);
		if (ret == 1) {
			step++;
		}
		break;

	case 2: /* ���ļ�����ȷ�� <-- */
		ret = com_tx(file_upload_activate_ack);
		if (ret == 1) {
			step++;
		}
		break;

	case 3: /* ���ļ�����ȷ����Ӧ --> */
		ret = com_rx(file_upload_activate_ack_res, &process.ovt);
		if (ret == 1) {
			step++;
		}
		if (ret == 2) {
			step = 0xFF;
		}
		break;

	case 4: /* ���ļ����ݴ��� <-- */
		ret = com_tx(file_upload_data);
		if (ret == 1) {
			step++;
		}
		break;

	case 5: /* ���ļ����ݴ�����Ӧ --> */
		ret = com_rx(file_upload_data_res, &process.ovt);
		if (ret == 1) {
			if (process.finish == 1) { /* �ļ�������� */
				process.finish = 0;
				step++;
			} else {
				step = 4;  /* ����������ļ����ݴ��䲽���ϴ��ļ����� */
			}
		}
		if (ret == 2) {
			step = 0xFF;
		}
		break;

	case 6: /* ���ļ����ݴ���ȷ�� --> */
		ret = com_rx(file_upload_data_ack, &process.ovt);
		if (ret == 1) {
			step++;
		}
		if (ret == 2) {
			step = 0xFF;
		}
		break;

	case 7: /* ���ļ����ݴ���ȷ����Ӧ <-- */
		ret = com_tx(file_upload_data_ack_res);
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
    ����˵����д�ļ����� -->
    ���������
    ���������
    �� �� ֵ��
*******************************************************************************/
int file_download_activate(char *rxbuf, int len)
{
	char TYP, COT;
	char C, DIR, PRM, FCB, FCV, FC;
	unsigned long point_addr;
	char poket_type;
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
	if (TYP != 210) {
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

	point_addr = rxbuf[index + 1] * 0x100 + rxbuf[index]; /* ��Ϣ���ַ */
	index += 2;
	if (point_addr != 0) {
		return -1;
	}

	poket_type = rxbuf[index++]; /* �������ݰ����� */
	if (poket_type != 2) {
		return -1;
	}

	process.dir_opt = rxbuf[index++]; /* ������ʶ */
	if (process.dir_opt != 7) {
		return -1;
	}

	process.dir_name_len = rxbuf[index++]; /* �ļ������� */
	if ((process.dir_name_len < 20) && (process.dir_name_len > 0)) {
		memcpy(process.dir_name, rxbuf + index, process.dir_name_len); /* �ļ��� */
		index += process.dir_name_len;
	}

	memcpy(process.dir_id, rxbuf + index, 4); /* �ļ�id */
	index += 4;

	process.file_size = rxbuf[index + 3] * 0x1000000
			    + rxbuf[index + 2] * 0x10000
			    + rxbuf[index + 1] * 0x100
			    + rxbuf[index]; /* �ļ���С */

	return 0;
}


/******************************************************************************
    ����˵����д�ļ�������Ӧ <--
    ���������
    ���������
    �� �� ֵ��
*******************************************************************************/
int file_download_activate_res(char *txbuf)
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
    ����˵����д�ļ�����ȷ�� <--
    ���������
    ���������
    �� �� ֵ��
*******************************************************************************/
int file_download_activate_ack(char *txbuf)
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

	txbuf[index++] = 210; /* TI */

	txbuf[index++] = 1; /* VSQ */
	txbuf[index++] = 7; /* COT */

	index += 2; /* ASDU */

	txbuf[index++] = 0; /* ��Ϣ���ַ */
	txbuf[index++] = 0;

	txbuf[index++] = 2; /* �������ݰ����� */

	txbuf[index++] = 8;  /* ������ʶ */
	txbuf[index++] = 0;  /* ��������� 0���ɹ� 1��ʧ�� */

	txbuf[index++] = process.dir_name_len; /* �ļ����Ƴ��� */
	if ((process.dir_name_len > 0)
	    && (process.dir_name_len < 20)) {
		memcpy(txbuf + index, process.dir_name, process.dir_name_len); /* �ļ����� */
		index += process.dir_name_len;
	}

	memcpy(txbuf + index, process.dir_id, 4);  /* �ļ�ID */
	index += 4;

	txbuf[index++] = (char)process.file_size; /* �ļ���С */
	txbuf[index++] = (char)(process.file_size >> 8);
	txbuf[index++] = (char)(process.file_size >> 16);
	txbuf[index++] = (char)(process.file_size >> 24);

	return index;
}


/******************************************************************************
    ����˵����д�ļ�����ȷ����Ӧ -->
    ���������
    ���������
    �� �� ֵ��
*******************************************************************************/
int file_download_activate_ack_res(char *rxbuf, int len)
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
    ����˵����д�ļ����ݴ��� -->
    ���������
    ���������
    �� �� ֵ��
*******************************************************************************/
int file_download_data(char *rxbuf, int len)
{
	char TYP, COT;
	char C, DIR, PRM, FCB, FCV, FC;
	unsigned long point_addr;
	char poket_type;
	int index;
	unsigned long segment_no;
	char dir_id[4];
	char cs;
	int ret;

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
	if (TYP != 210) {
		return -1;
	}

	/* �ɱ䳤���޶��� */
	index += 1;

	/*����ԭ�� */
	COT = rxbuf[index++];
	if (COT != 5) {
		return -1;
	}

	index += 2; /* ASDU���õ�ַ */

	point_addr = rxbuf[index + 1] * 0x100 + rxbuf[index]; /* ��Ϣ���ַ */
	index += 2;
	if (point_addr != 0) {
		return -1;
	}

	poket_type = rxbuf[index++]; /* �������ݰ����� */
	if (poket_type != 2) {
		return -1;
	}

	process.dir_opt = rxbuf[index++]; /* ������ʶ */
	if (process.dir_opt != 9) {
		return -1;
	}

	memcpy(dir_id, rxbuf + index, 4); /* �ļ�id*/
	index += 4;

	if  ((process.dir_id[0] != dir_id[0])
	     || (process.dir_id[1] != dir_id[1])
	     || (process.dir_id[2] != dir_id[2])
	     || (process.dir_id[3] != dir_id[3])) {
		return -1;
	}

	segment_no = rxbuf[index + 3] * 0x1000000
		     + rxbuf[index + 2] * 0x10000
		     + rxbuf[index + 1] * 0x100
		     + rxbuf[index]; /* �κ�*/
	index += 4;

	if (process.segment_no != segment_no) {
		return -1;
	}

	if (rxbuf[index++] == 1) { /* �к������� */
		process.finish = 0;
	} else {
		process.finish = 1;
	}

	len = rxbuf[1] - 23; /* �����ݳ��� */
	cs = dlt_104_cs(rxbuf + index, len);
	if (cs != rxbuf[index + len + 1]) {
		return -1;
	}

	ret = dlt_104_save_file_segment(process.dir_name,
					process.dir_name_len,
					process.segment_no,
					rxbuf + index,
					len);
	if (ret < 0) {
		return -1;
	}

	process.segment_no++;

	return 0;
}



/******************************************************************************
    ����˵�������ļ����ݴ�����Ӧ <--
    ���������
    ���������
    �� �� ֵ��
*******************************************************************************/
int file_download_data_res(char *txbuf)
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
    ����˵����д�ļ����ݴ���ȷ�� <--
    ���������
    ���������
    �� �� ֵ��
*******************************************************************************/
int file_download_data_ack(char *txbuf)
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

	txbuf[index++] = 210; /* TI */

	txbuf[index++] = 1; /* VSQ */
	txbuf[index++] = 5; /* COT */

	index += 2; /* ASDU */

	txbuf[index++] = 0; /* ��Ϣ���ַ */
	txbuf[index++] = 0;

	txbuf[index++] = 2; /* �������ݰ����� */

	txbuf[index++] = 10;  /* ������ʶ */

	memcpy(txbuf + index, process.dir_id, 4);  /* �ļ�ID */
	index += 4;

	txbuf[index++] = (char)process.segment_no; /* ���ݶκ� */
	txbuf[index++] = (char)(process.segment_no >> 8);
	txbuf[index++] = (char)(process.segment_no >> 16);
	txbuf[index++] = (char)(process.segment_no >> 24);

	txbuf[index++] = 0;  /* ��������� 0���ɹ� 1��ʧ�� */

	return index;
}



/******************************************************************************
    ����˵����д�ļ����ݴ���ȷ�� -->
    ���������
    ���������
    �� �� ֵ��
*******************************************************************************/
int file_download_data_ack_res(char *rxbuf, int len)
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
    ����˵�����ļ����ش���
    ���������
    ���������
    �� �� ֵ��
*******************************************************************************/
int process_file_download_file(void)
{
	static char step;
	int ret;

	switch(step) {
	case 0: /* д�ļ����� --> */
		ret = com_rx(file_download_activate, &process.ovt);
		if (ret == 1) {
			step++;
		}
		break;

	case 1: /* д�ļ�������Ӧ <-- */
		ret = com_tx(file_download_activate_res);
		if (ret == 1) {
			step++;
		}
		break;

	case 2: /* д�ļ�����ȷ�� <-- */
		ret = com_tx(file_download_activate_ack);
		if (ret == 1) {
			step++;
		}
		break;

	case 3: /* д�ļ�����ȷ����Ӧ --> */
		ret = com_rx(file_download_activate_ack_res, &process.ovt);
		if (ret == 1) {
			step++;
		}
		if (ret == 2) {
			step = 0xFF;
		}
		break;

	case 4: /* д�ļ����ݴ��� --> */
		ret = com_rx(file_download_data, &process.ovt);
		if (ret == 1) {
			step++;
		}
		if (ret == 2) {
			step = 0xFF;
		}
		break;

	case 5: /* д�ļ����ݴ�����Ӧ <-- */
		ret = com_tx(file_download_data_res);
		if (ret == 1) {
			if (process.finish == 1) { /* �ļ�������� */
				process.finish = 0;
				step++;
			} else {
				step = 4;  /* ����������ļ����ݴ��䲽���ϴ��ļ����� */
			}
		}
		break;

	case 6: /* д�ļ����ݴ���ȷ�� <-- */
		ret = com_tx(file_download_data_ack);
		if (ret == 1) {
			step++;
		}
		break;

	case 7: /* д�ļ����ݴ���ȷ�� --> */
		ret = com_rx(file_download_data_ack_res, &process.ovt);
		if (ret == 1) {
			step = 0;
		}
		if (ret == 2) {
			step = 0xFF;
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
int dlt_104_process_file(void)
{
	int ret;

	/* �ٻ�Ŀ¼���� */
	ret = process_file_call_dir();

	/* �ļ��ϴ����� */
	ret = process_file_upload_file();

	/* �ļ����ع��� */
	ret = process_file_download_file();

	return ret;
}


/******************************************************************************
    ����˵����
    ���������
    ���������
    �� �� ֵ��
*******************************************************************************/
int dlt_104_process_file_init(void)
{
	memset((char *)&process, 0, sizeof(process));
	return 0;
}
