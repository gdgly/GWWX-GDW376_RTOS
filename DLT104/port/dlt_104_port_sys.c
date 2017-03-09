/******************************************************************************
    ��Ȩ���У�
    �� �� ��: 1.0
    �� �� ��: dlt_104_port_sys.c
    ��������: 
    ��	 �ߣ����
    ����˵����ʵ�֡�����Զ���ϵͳӦ��DL/T634.5104-2009ʵʩϸ����ϵͳ���ݲ�ӿ�
    ����˵����
    �޸ļ�¼��
*******************************************************************************/
#include <stdio.h>
#include <string.h>
#include "GDW376_CFG.h"
#include "store.h"

//#include "file_dir.h"
#define CFG_DEBUG

#ifdef CFG_DEBUG
    #define Print(fmt,args...) printf(fmt, ##args)
#else
    #define Print(fmt,args...)
#endif


unsigned char week;

/******************************************************************************
    ����˵����
    ���������
    ���������
    �� �� ֵ��
*******************************************************************************/
char dlt_104_get_dir_file_num(char *dir_name)
{
	//char num;

	//num = file_dir_num_get(dir_name, 0);

	return 0;
}



/******************************************************************************
    ����˵����
    ���������
    ���������
    �� �� ֵ��
*******************************************************************************/
int dlt_104_get_dir_info(char *dir_name,
			 char file_index,
			 char *file_name_len,
			 char *file_name,
			 unsigned long *file_size,
			 char *file_time)
{
//	struct file_info info = {0};
//	int ret;
//
//	/* ��ȡĿ¼�µ������ļ���Ϣ */
//	ret = file_dir_info(dir_name, file_index, &info);
//	if (ret == 1) {
//		*file_name_len = info.name_len;
//		memcpy(file_name, info.name, info.name_len);
//		*file_size = info.size;
//		memcpy(file_time, info.time, 7);
//		return 1;
//	}

	return 0;
}


/******************************************************************************
    ����˵����
    ���������
    ���������
    �� �� ֵ��
*******************************************************************************/
int dlt_104_get_file_info(char *file_name,
			  char file_name_len,
			  unsigned long *file_size,
			  char *id,
			  char *file_time)
{
//	struct file_info info = {0};
//	int ret;
//
//	ret = file_dir_file_info(file_name, &info);
//	if (ret == 0) {
//		*file_size = info.size;
//		memcpy(id, info.id, 4);
//		memcpy(file_time, info.time, 7);
//		return 1;
//	}

	return 0;
}



/******************************************************************************
    ����˵����
    ���������
    ���������
    �� �� ֵ��
*******************************************************************************/
int dlt_104_read_file_segment(char *file_name,
			      unsigned long file_len,
			      unsigned long segment_index,
			      char *segment,
			      char *segment_len,
			      char *flag)
{
//	int len;
//
//	len = file_dir_file_segment_read(file_name, segment_index, segment, flag);
//	if (len > 0) {
//		*segment_len = len;
//		return 1;
//	}

	return 0;
}




/******************************************************************************
    ����˵����
    ���������
    ���������
    �� �� ֵ��
*******************************************************************************/
int dlt_104_save_file_segment(char *file_name,
			      unsigned long file_len,
			      unsigned long segment_index,
			      char *segment,
			      char segment_len)
{
//	int ret;
//
//	ret = file_dir_file_segment_write(file_name, segment_index,
//					  segment, segment_len);
//	if (ret == 0) {
//		return 0;
//	}

	return -1;
}



static unsigned char get_point_param(char *buff, unsigned long point_addr, 
				unsigned char SN)
{
	//unsigned char len = 0;
	
	switch (point_addr) {
	case 0x8001:
	       break;
	default:
		break;
	}

	return 0;
}


static unsigned char dlt_104_get_some_param(char *txbuf, 
			    unsigned char *index,
			    const unsigned char asdu_index,
			    const char *asdu_data, 
			    const unsigned char adsu_len,
			    const unsigned char SN)
{
	unsigned char i, len = 0, txlen = 0;
	unsigned long point_addr;	
	
	for (i = asdu_index; i < adsu_len; i += 3) {
		
		/* ����SN��2��byte */
		if (i == 0)
			i = 2; 

		/* ��ֹ���ͻ���Խ�� */
		if (txlen > 200) 		       
			break;	

		/* ȡ����Ϣ���ַ */
		point_addr = asdu_data[i + 2] * 0x10000
			+ asdu_data[i + 1] * 0x100
			+ asdu_data[i];	
		
		/* ��ȡ��Ϣ���ַ��Ӧ�Ĳ��� */
		len = get_point_param(txbuf + txlen, point_addr, SN);
		if (len > 0)
			txlen += len;
	}	

	*index = i;
	return txlen;
}



static unsigned char dlt_104_get_all_param(char *txbuf, 
			    unsigned char *index,
			    const unsigned char asdu_index,
			    const unsigned char SN)
{
	unsigned char i, len = 0, txlen = 0;
	unsigned long point_addr;
	unsigned char asdu_data[150]; /* ��Ҫָ���������ӳ��� */
	unsigned char PARAM_MAX_NUM = 50;

	for (i = asdu_index; i < PARAM_MAX_NUM; i++) {
		
		/* ��ֹ���ͻ���Խ�� */
		if (txlen > 200) {		       
			break;	
		}

		/* ȡ����Ϣ���ַ */
		point_addr = asdu_data[i + 2] * 0x10000
			+ asdu_data[i + 1] * 0x100
			+ asdu_data[i];	
		
		/* ��ȡ��Ϣ���ַ��Ӧ�Ĳ��� */
		len = get_point_param(txbuf + txlen, point_addr, SN);
		if (len > 0)
			txlen += len;
	}	

	*index = i;
	return txlen;
}



unsigned char dlt_104_get_param(char *txbuf, 
			unsigned char *index,   
			const unsigned char adsu_index,
			const char *asdu_data, 
			const unsigned char asdu_len)
{
	unsigned short  SN;	
	unsigned char len;

	SN = asdu_data[1] * 0x100 + asdu_data[0]; /* ��ֵ����SN */

	/* ��ȡ��վ�·��Ķ������ */
	if (asdu_len > 2) {		
		
		/* ��ǰ����asdu���ݵ�λ��С��asdu���� */
		if (adsu_index < asdu_len) {
			
			len = dlt_104_get_some_param(txbuf, 
							index, 
							adsu_index,
						     	asdu_data,
						     	asdu_len, 
						     	SN);
			return len;
		} 		

	} 
	
	/* ��ȡȫ������ */
	else if (asdu_len == 2) {				
		len = dlt_104_get_all_param(txbuf + 12, index, adsu_index, SN);
		return len;	
	}	
	
	return 0;
}



int set_point_param(char *asdu_data, unsigned char asdu_len, 
		unsigned long point_addr, unsigned char SN)
{
	int ret;

	switch (point_addr) {
	case 0x8020:  /* �������� */
		ret = store_param_save("0x8020", asdu_data, asdu_len);
		break;

	case 0x8021:  /* ������ѹ���� */
		ret = store_param_save("0x8021", asdu_data, asdu_len);
		break;

	default:
		break;
	}

	return ret;		
}



int dlt_104_set_param(char *asdu_data, unsigned char asdu_len)
{
	unsigned short SN;		
	unsigned long point_addr;
	unsigned char index, len;
	int ret;

	SN = asdu_data[1] * 0x100 + asdu_data[0]; /* ��ֵ����SN */
	index = 2;

	while (index < asdu_len) {
		
		/* ȡ����Ϣ���ַ */
		point_addr = asdu_data[index + 2] * 0x10000
				+ asdu_data[index + 1] * 0x100
				+ asdu_data[index];
		
		len = asdu_data[index + 4];
		ret = set_point_param(asdu_data, len + 5, point_addr, SN);
		if (ret < 0) {
			Print("set_point_param %X error!\r\b", point_addr);
			return -1;
		}
		
		index += (len + 5); /* ָ����һ������ */
	}
		
	return 0;
}



/* A14: XX XX XX .XX XX */
static void A14_to_float(float *data, unsigned char *A14)
{
	unsigned char BCD_sww, BCD_ww;
	unsigned char BCD_qw, BCD_bw;
	unsigned char BCD_sw, BCD_gw;
	unsigned char BCD_sfw, BCD_bfw;
	unsigned char BCD_qfw, BCD_wfw;

	BCD_sww = (A14[4] >> 4) & 0x0F;
	BCD_ww = A14[4] & 0x0F;

	BCD_qw = (A14[3] >> 4) & 0x0F;
	BCD_bw = A14[3] & 0x0F;

	BCD_sw = (A14[2] >> 4) & 0x0F;
	BCD_gw = A14[2] & 0x0F;

	BCD_sfw = (A14[1] >> 4) & 0x0F;
	BCD_bfw = A14[1] & 0x0F;

	BCD_qfw = (A14[0] >> 4) & 0x0F;
	BCD_wfw = A14[0] & 0x0F;	

	*data = BCD_sww * 100000.0 + BCD_ww * 10000.0 
		+ BCD_qw * 1000.0 + BCD_bw * 100.0
		+ BCD_sw * 10.0 + BCD_gw * 1.0
		+ BCD_sfw * 0.1 + BCD_bfw * 0.01
		+ BCD_qfw * 0.001 + BCD_wfw * 0.0001;
}




/* A11: XX XX XX. XX */
static void A11_to_float(float *data, unsigned char *A11)
{
	unsigned char BCD_sww, BCD_ww;
	unsigned char BCD_qw, BCD_bw;
	unsigned char BCD_sw, BCD_gw;
	unsigned char BCD_sfw, BCD_bfw;
	
	BCD_sww = (A11[3] >> 4) & 0x0F;
	BCD_ww = A11[3] & 0x0F;

	BCD_qw = (A11[2] >> 4) & 0x0F;
	BCD_bw = A11[2] & 0x0F;

	BCD_sw = (A11[1] >> 4) & 0x0F;
	BCD_gw = A11[1] & 0x0F;

	BCD_sfw = (A11[0] >> 4) & 0x0F;
	BCD_bfw = A11[0] & 0x0F;

	*data = BCD_sww * 100000.0 + BCD_ww * 10000.0 
		+ BCD_qw * 1000.0 + BCD_bw * 100.0
		+ BCD_sw * 10.0 + BCD_gw * 1.0
		+ BCD_sfw * 0.1 + BCD_bfw * 0.01;
}


static int get_elect_pn_YG(unsigned char pn, unsigned char ABC, char *buff)
{
	float elect = 0;
	char *data;

	if (pn == 1) {
		if (ABC == 'Z')
			A14_to_float(&elect, P1D.R9010);
		if (ABC == 'F')
			A14_to_float(&elect, P1D.R9020);	
	}

	data = (char *)&elect;
	buff[0] = data[0];
	buff[1] = data[1];
	buff[2] = data[2];
	buff[3] = data[3];
	buff[4] = 0; /* Ʒ��������QDS */
	
	return 5;
}


static int get_elect_pn_WG(unsigned char pn, unsigned char ABC, char *buff)
{
	float elect = 0;
	char *data;

	if (pn == 1) {
		if (ABC == 'Z')
			A11_to_float(&elect, P1D.R9110);
		if (ABC == 'F')
			A11_to_float(&elect, P1D.R9120);	
		if (ABC == '1')
			A11_to_float(&elect, P1D.R9130);	
		if (ABC == '2')
			A11_to_float(&elect, P1D.R9150);	
		if (ABC == '3')
			A11_to_float(&elect, P1D.R9160);	
		if (ABC == '4')
			A11_to_float(&elect, P1D.R9140);	
	}

	data = (char *)&elect;
	buff[0] = data[0];
	buff[1] = data[1];
	buff[2] = data[2];
	buff[3] = data[3];
	buff[4] = 0; /* Ʒ��������QDS */
	return 5;
}


static int get_elect(unsigned long point_addr, char *buff)
{
	unsigned char len = 0;

	switch (point_addr) {
	case 0x006401: /* 1��· ��ǰ�����й�����ʾֵ */
		len = get_elect_pn_YG(1, 'Z', buff); 
		break;

	case 0x006402: /* 1��· ��ǰ�����޹�����ʾֵ */
		len = get_elect_pn_WG(1, 'Z', buff);
		break;

	case 0x006403: /* 1��· ��ǰһ�����޹�����ʾֵ */
		len = get_elect_pn_WG(1, '1', buff);
		break;

	case 0x006404: /* 1��· ��ǰ�������޹�����ʾֵ */
		len = get_elect_pn_WG(1, '4', buff);
		break;

	case 0x006405: /* 1��· ��ǰ�����й�����ʾֵ */
		len = get_elect_pn_YG(1, 'F', buff);
		break;

	case 0x006406: /* 1��· ��ǰ�����޹�����ʾֵ */
		len = get_elect_pn_WG(1, 'F', buff);
		break;
		
	case 0x006407: /* 1��· ��ǰ�������޹�����ʾֵ */
		len = get_elect_pn_WG(1, '2', buff);
		break;

	case 0x006408: /* 1��· ��ǰ�������޹�����ʾֵ */
		len = get_elect_pn_WG(1, '3', buff);
		break;

	default:
		break;	
	}
	
	return len;
}




int dlt_104_get_elect(char *txbuf, unsigned char SQ, unsigned char *num)
{
	unsigned char index = 0, len = 0;
	
	/* ��������Ϣ0x6401-0x6500 */
	txbuf[index++] = 0x01;
	txbuf[index++] = 0x64;
	txbuf[index++] = 0x00;
	
	/* 1��· ��ǰ�����й�����ʾֵ */
	len = get_elect(0x006401, txbuf + index);
	index += len;
	/* 1��· ��ǰ�����޹�����ʾֵ */
	len = get_elect(0x006402, txbuf + index);
	index += len;
	/* 1��· ��ǰһ�����޹�����ʾֵ */
	len = get_elect(0x006403, txbuf + index);
	index += len;
	/* 1��· ��ǰ�������޹�����ʾֵ */
	len = get_elect(0x006404, txbuf + index);
	index += len;
	/* 1��· ��ǰ�����й�����ʾֵ */
	len = get_elect(0x006405, txbuf + index);
	index += len;
	/* 1��· ��ǰ�����޹�����ʾֵ */
	len = get_elect(0x006406, txbuf + index);
	index += len;
	/* 1��· ��ǰ�������޹�����ʾֵ */
	len = get_elect(0x006407, txbuf + index);
	index += len;
	/* 1��· ��ǰ�������޹�����ʾֵ */
	len = get_elect(0x006408, txbuf + index);
	index += len;

	*num = 8;

	return index;
}


/* 
 * ��ȡPn����ң�� 
 * ��ʱ��F28���������״̬�ּ����λ��־
 * ����ң������
 */
static int get_yx_pn_one_point(unsigned char pn, char const *yx_name, char *buff)
{
	unsigned char len;

	/* ң��: �������㷽ʽ��0����/1���䣩*/
	if (strstr(yx_name, "F28_FLAG1_bit1") != 0) {
		buff[0] = (P1D.RC027[0] >> 1) & 0x01; 
		len = 1;	
	}

	/* ң��: ����ʱ�ӵ�أ�0����/1Ƿѹ��*/
	if (strstr(yx_name, "F28_FLAG1_bit2") != 0) {
		buff[0] = (P1D.RC027[0] >> 2) & 0x01;  
		len = 1;	
	}

	/* ң��: ͣ�糭���أ�0����/1Ƿѹ�� */
	if (strstr(yx_name, "F28_FLAG1_bit3") != 0) {
		buff[0] = (P1D.RC027[0] >> 3) & 0x01;  
		len = 1;	
	}

	/* ң��: �й����ʷ���0����/1����*/
	if (strstr(yx_name, "F28_FLAG1_bit4") != 0) {
		buff[0] = (P1D.RC027[0] >> 4) & 0x01;  
		len = 1;	
	}

	/* ң��: �޹����ʷ���0����/1����*/
	if (strstr(yx_name, "F28_FLAG1_bit5") != 0) {
		buff[0] = (P1D.RC027[0] >> 5) & 0x01;  
		len = 1;	
	}


	return len;
}



static int get_yx(unsigned long point_addr, char *buff)
{
	unsigned char len = 0;

	switch (point_addr) {
	case 0x000001:
		len = get_yx_pn_one_point(1, "F28_FLAG1_bit1", buff);
		break;

	case 0x000002:
		len = get_yx_pn_one_point(1, "F28_FLAG1_bit2", buff);
		break;

	case 0x000003:
		len = get_yx_pn_one_point(1, "F28_FLAG1_bit3", buff);
		break;

	case 0x000004:
		len = get_yx_pn_one_point(1, "F28_FLAG1_bit4", buff);
		break;

	case 0x000005:
		len = get_yx_pn_one_point(1, "F28_FLAG1_bit5", buff);
		break;
		
	default:
		break;	
	}
	
	return len;
}



/* ������Ϣ, ��������  */
int user_data_read_YX(char TI, char SQ, char *txbuf, unsigned char *num)
{
	unsigned char index = 0, len = 0;
	
	/* ң����Ϣ����1��ַ */
	txbuf[index++] = 0x01;
	txbuf[index++] = 0x40;
	txbuf[index++] = 0x00;

	len = get_yx(0x000001, txbuf + index);
	index += len;

	len = get_yx(0x000002, txbuf + index);
	index += len;

	len = get_yx(0x000003, txbuf + index);
	index += len;

	len = get_yx(0x000004, txbuf + index);
	index += len;

	len = get_yx(0x000005, txbuf + index);
	index += len;

	*num = 5;

	return index;
}


void A7_to_float(float *data, unsigned char *A7)
{
	unsigned char BCD_bw, BCD_sw;
	unsigned char BCD_gw, BCD_sfw;
	
	BCD_bw = (A7[1] >> 4) & 0x0F;
	BCD_sw = A7[1] & 0x0F;

	BCD_gw = (A7[0] >> 4) & 0x0F;
	BCD_sfw = A7[0] & 0x0F;

	*data = BCD_bw * 100 + BCD_sw * 10 + BCD_gw + BCD_sfw * 0.1;
}



int get_yc_pn_voltage(unsigned char pn, unsigned char ABC, char *buff)
{
	float voltage = 0;
	char *data;

	if (pn == 1) {
		if (ABC == 'A')
			A7_to_float(&voltage, P1D.RB611);
		if (ABC == 'B')
			A7_to_float(&voltage, P1D.RB612);	
		if (ABC == 'C')
			A7_to_float(&voltage, P1D.RB613);	
	}

	data = (char *)&voltage;
	buff[0] = data[0];
	buff[1] = data[1];
	buff[2] = data[2];
	buff[3] = data[3];
	buff[4] = 0; /* Ʒ��������QDS */
	return 5;
}


/* ��GDW376.1��Լ�е�A9���͡�XX.XXXX
 * ����ת����IEEE STD745(float 4λ) 
 * */
void A9_to_float(float *data, unsigned char *A9)
{
	unsigned char BCD_sw, BCD_gw; 
	unsigned char BCD_sf, BCD_bfw;
	unsigned char BCD_qfw, BCD_wfw;
	unsigned char S;
	
	S = (A9[2] >> 7) & 0x01; /* ����λ */

	BCD_sw = (A9[2] >> 4) & 0x07;	
	BCD_gw = A9[2] & 0x0F;

	BCD_sf = (A9[1] >> 4) & 0x0F;
	BCD_bfw = A9[1] & 0x0F;

	BCD_qfw = (A9[0] >> 4) & 0x0F;
	BCD_wfw = A9[0] & 0x0F;

	*data = BCD_sw * 10.0 + BCD_gw * 1.0
		+ BCD_sf * 0.1 + BCD_bfw * 0.01 
		+ BCD_qfw * 0.001 + BCD_wfw * 0.0001;

	if (S == 1)
		*data = -*data;
}



int get_yc_pn_power(unsigned char pn, unsigned char ABC, char *buff)
{
	float power = 0;
	char *data;

	if (pn == 1) {
		if (ABC == 'Z')
			A9_to_float(&power, P1D.RB630);		
		if (ABC == 'A')
			A9_to_float(&power, P1D.RB631);
		if (ABC == 'B')
			A9_to_float(&power, P1D.RB632);	
		if (ABC == 'C')
			A9_to_float(&power, P1D.RB633);	
	}
	
	data = (char*)&power;

	buff[0] = data[0];
	buff[1] = data[1];
	buff[2] = data[2];
	buff[3] = data[3];
	buff[4] = 0; /* Ʒ��������QDS */

	return 5;
}



/* ��GDW376.1��Լ�е�A25���͡�XXX.XXX
 * ����ת����IEEE STD745(float 4λ) 
 * */
void A25_to_float(float *data, unsigned char *A25)
{
	unsigned char BCD_bw, BCD_sw; 
	unsigned char BCD_gw, BCD_sfw;
	unsigned char BCD_bfw, BCD_qfw;
	unsigned char S;
	
	S = (A25[2] >> 7) & 0x01; /* ����λ */

	BCD_bw = (A25[2] >> 4) & 0x07;	
	BCD_sw = A25[2] & 0x0F;

	BCD_gw = (A25[1] >> 4) & 0x0F;
	BCD_sfw = A25[1] & 0x0F;

	BCD_bfw = (A25[0] >> 4) & 0x0F;
	BCD_qfw = A25[0] & 0x0F;

	*data = BCD_bw * 100 + BCD_sw * 10 
		+ BCD_gw + BCD_sfw * 0.1 
		+ BCD_bfw * 0.01 + BCD_qfw * 0.001;

	if (S == 1)
		*data = -*data;
}



int get_yc_pn_current(unsigned char pn, unsigned char ABC, char *buff)
{
	float current = 0;
	char *data;

	if (pn == 1) {
		if (ABC == 'A')
			A25_to_float(&current, P1D.RB621);
		if (ABC == 'B')
			A25_to_float(&current, P1D.RB622);	
		if (ABC == 'C')
			A25_to_float(&current, P1D.RB623);	
		if (ABC == 'O')
			A25_to_float(&current, P1D.RB6E5);	
	}
	
	data = (char*)&current;

	buff[0] = data[0];
	buff[1] = data[1];
	buff[2] = data[2];
	buff[3] = data[3];
	buff[4] = 0; /* Ʒ��������QDS */
	return 5;
}



static int get_yc(unsigned long point_addr, char *buff)
{
	unsigned char len = 0;

	switch (point_addr) {
	case 0x004001:
		len = get_yc_pn_power(1, 'Z', buff);
		break;

	case 0x004002:
		len = get_yc_pn_power(1, 'A', buff);
		break;

	case 0x004003:
		len = get_yc_pn_power(1, 'B', buff);
		break;

	case 0x004004:
		len = get_yc_pn_power(1, 'C', buff);
		break;

	case 0x004005:
		len = get_yc_pn_voltage(1, 'A', buff);
		break;

	case 0x004006:
		len = get_yc_pn_voltage(1, 'B', buff);
		break;
		
	case 0x004007:
		len = get_yc_pn_voltage(1, 'C', buff);
		break;

	case 0x004008:
		len = get_yc_pn_current(1, 'A', buff);
		break;

	case 0x004009:
		len = get_yc_pn_current(1, 'C', buff);
		break;

	case 0x00400A:
		len = get_yc_pn_current(1, 'C', buff);
		break;

	default:
		break;	
	}
	
	return len;
}



/*  ����ֵ IEEE STD745�̸����� ��������  */
int user_data_read_YC(char TI, char SQ, char *txbuf, unsigned char *num)
{
	unsigned char index = 0, len = 0;
	
	/* ң����Ϣ����1��ַ */
	txbuf[index++] = 0x01;
	txbuf[index++] = 0x40;
	txbuf[index++] = 0x00;

	len = get_yc(0x004001, txbuf + index);
	index += len;

	len = get_yc(0x004002, txbuf + index);
	index += len;

	len = get_yc(0x004003, txbuf + index);
	index += len;

	len = get_yc(0x004004, txbuf + index);
	index += len;

	len = get_yc(0x004005, txbuf + index);
	index += len;

	len = get_yc(0x004006, txbuf + index);
	index += len;

	len = get_yc(0x004007, txbuf + index);
	index += len;

	len = get_yc(0x004008, txbuf + index);
	index += len;

	len = get_yc(0x004009, txbuf + index);
	index += len;

	len = get_yc(0x00400A, txbuf + index);
	index += len;

	*num = 3;
	
	return index;
}



void time_hex_to_cp56time2a(char *cp56time2a, struct Time_Type *hex_time)
{
	unsigned char years, month, day, hours,  minutes;
        unsigned short ms;

	years = cp56time2a[6] & 0x7F;
	month = cp56time2a[5] & 0x0F;
	day = cp56time2a[4] & 0x1F;
	week = (cp56time2a[4] >> 5) & 0x07;
	hours = cp56time2a[3] & 0x1F; 
	minutes = cp56time2a[2] & 0x3F;
	ms = cp56time2a[1] * 0x100 + cp56time2a[0];

	years = hex_time->Year;
	month = hex_time->Month;
	day = hex_time->Day;
	hours = hex_time->Hour;
	minutes = hex_time->Min;
	ms = hex_time->Sec * 1000;

	cp56time2a[6] = years;
	cp56time2a[5] = month;
	cp56time2a[4] = (week << 5) | day;
	cp56time2a[3] = hours;
	cp56time2a[2] = minutes;
	cp56time2a[1] = ms / 1000;
	cp56time2a[0] = ms % 1000;
}




int dlt_104_cp56time2a_read(char *cp56time2a)
{
	time_hex_to_cp56time2a(cp56time2a, &THEX);
	return 0;
}



void cp56time2a_to_time_hex(struct Time_Type *hex_time,  char *cp56time2a)
{
	unsigned char years, month, day, hours,  minutes;
        unsigned short ms;

	years = cp56time2a[6] & 0x7F;
	month = cp56time2a[5] & 0x0F;
	day = cp56time2a[4] & 0x1F;
	week = (cp56time2a[4] >> 5) & 0x07;
	hours = cp56time2a[3] & 0x1F; 
	minutes = cp56time2a[2] & 0x3F;
	ms = cp56time2a[1] * 0x100 + cp56time2a[0];

	hex_time->Year = years;
	hex_time->Month = month;
	hex_time->Day = day;
	hex_time->Hour = hours;
	hex_time->Min = minutes;
	hex_time->Sec = ms / 1000;
}






/* ��һ���ֽڵ�hex��ת����һ���ֽڵ�BCD, hex����С��100 */
void __hex_to_bcd(unsigned char *bcd, unsigned char hex)
{
	unsigned char hex_sw, hex_gw;

	hex_sw = hex / 10;
	hex_gw = hex % 10;

	*bcd = hex_sw * 0x10 + hex_gw;
}


void hex_to_bcd(unsigned char *bcd, unsigned char *hex, int len)
{
	int i;

	for (i = 0; i < len; i++)
		__hex_to_bcd(bcd + i, hex[i]);
}



int dlt_104_cp56time2a_write(char *cp56time2a)
{
	struct Time_Type time_hex, time_bcd;

	cp56time2a_to_time_hex(&time_hex, cp56time2a);

	hex_to_bcd((unsigned char *)&time_bcd, (unsigned char *)&time_hex, 
			sizeof(struct Time_Type));
	
	THEX = time_hex;
	TBCD = time_bcd;
	
	return 0;
}



struct soe_yx
{
	unsigned short point_add;
	unsigned char yx_data;
};


int user_data_YX_change(unsigned char *num, char *buff, 
		unsigned char TI, unsigned char SQ)
{
		


	return 0;
}




int user_data_YX_clear(void)
{
	return 0;
}


int user_data_YC_change(unsigned char *num, char *buff, unsigned char TI, unsigned char SQ)
{
	return 0;
}


