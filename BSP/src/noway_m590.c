
/*
 * noway m590
 * likejshy@126.com
 * 2017-01-01
 */

#include <string.h>
#include <stdio.h>
#include <ucos_ii.h>

#include "stm32f10x.h"

#include "Serial.h"
#include "noway_m590.h"


#define DELAYMS(x)		OSTimeDly(x)
#define M590_POWER_ON		GPIO_SetBits(GPIOB, GPIO_Pin_12)
#define M590_POWER_OFF		GPIO_ResetBits(GPIOB, GPIO_Pin_12)

#define M590_TERN_ON_HIGH  	GPIO_ResetBits(GPIOA, GPIO_Pin_8)
#define M590_TERN_ON_LOW    	GPIO_SetBits(GPIOA, GPIO_Pin_8)

#define TXMAXLEN    1024
#define RXMAXLEN    1300


void com_led_flash(void)
{	
	GPIO_ResetBits(GPIOB, GPIO_Pin_8) ; //;�������ݵƿ�
	OSTimeDly(50);
	GPIO_SetBits(GPIOB, GPIO_Pin_8) ;	
	OSTimeDly(50);
}


void  tcp_link_led_on(void)
{	
	GPIO_ResetBits(GPIOB, GPIO_Pin_9) ; //;�������ݵƿ�
}

  

void  tcp_link_led_off(void)
{	
	GPIO_SetBits(GPIOB, GPIO_Pin_9) ;	
}


char* memstr(char* full_data, int full_data_len, char* substr)
{
    if (full_data == NULL || full_data_len <= 0 || substr == NULL) {
        return NULL;
    }

    if (*substr == '0') {
        return NULL;
    }

    int sublen = strlen(substr);

    int i;
    char* cur = full_data;
    int last_possible = full_data_len - sublen + 1;
    for (i = 0; i < last_possible; i++) {
        if (*cur == *substr) {
            //assert(full_data_len - i >= sublen);
            if (memcmp(cur, substr, sublen) == 0) {
                //found
                return cur;
            }
        }
        cur++;
    }

    return NULL;
}








/******************************************************************************
    ����˵��������һ��AT���
    ���������athead������AT����ͷ�ؼ���
	      attail������AT����β�ؼ���
	      size�����յ���󳤶�
	      ovt����ʱʱ��, ��λ:ms
    ���������rxbuf��AT�����ַ
    �� �� ֵ��0�� ʧ�ܣ�>0���ɹ�
*******************************************************************************/
static int rxatcmd(char *rxbuf, char *athead, char *attail, 
		   int size, int ovt_ms)
{
	int len = 0;
	char *key;

	memset(rxbuf, 0, size);
	
	while (ovt_ms--) {
		len = uart2_receive_packet(rxbuf, size, 250);
		if (len > 0) {
			rxbuf[len] = '\0';
			com_led_flash();
			break;
		}
		
		OSTimeDly(1);
	}
	
	if (len > 0) {
	
		/* want to find the head key word */
		if (athead != 0) {				
			key = memstr(rxbuf, len, athead);
			if (key != NULL) {
				if (key != rxbuf) {
					len -= (key - rxbuf);
					memcpy(rxbuf, key, len);				
				}
				
				/* not want find the tail key word, just return */
				if (attail == NULL)				
					return len;
				
			} else {
				return 0;
			}
		}
	
		/* wnat to find the tail key word */
		if (attail != NULL) {			
			key = memstr(rxbuf + 1, len - 1, attail);
			if (key != NULL)
				return len;
			else
				return 0;
		}
		
		return len;
	}

        return 0;
}


/******************************************************************************
    ����˵��������һ��AT�����֤��������AT����֮����Ϊ400ms
    ���������atcmd��ATT����
    �����������
    �� �� ֵ��-1�� ʧ�ܣ�0���ɹ�
*******************************************************************************/
static int txatcmd(char *atcmd)
{
	uart2_send_byte(atcmd, strlen(atcmd));
	com_led_flash();
	return 0;
}


/******************************************************************************
    ����˵����AT����: ���û���
    ���������value��0 �����Թرգ�1�����Կ���
    �����������
    �� �� ֵ��-1�� ʧ�ܣ�0���ɹ�
*******************************************************************************/
static int at_at_exec(void)
{
	char cmdbuf[64] = {0};

	if (txatcmd("AT\r\n") < 0)
		return -1;
	
	if (rxatcmd(cmdbuf, "OK", 0, 64, 500) <= 0)
		return -1;
	
	return 0;
}


/******************************************************************************
    ����˵����AT����ػ�
    �����������
    �����������
    �� �� ֵ��-1�� ʧ�ܣ�0���ɹ�
*******************************************************************************/
static int at_cpwroff_exec(void)
{
	char cmdbuf[64] = {0};

	if (txatcmd("AT+CPWROFF\r\n") < 0)
		return -1;
	
	if (rxatcmd(cmdbuf, "OK", 0, 64, 5000) <= 0)
		return -1;
	
	DELAYMS(5000);
	return 0;
}


/******************************************************************************
    ����˵����ATE  ���û�������
    ���������value��0 �����Թرգ�1�����Կ���
    �����������
    �� �� ֵ��-1�� ʧ�ܣ�0���ɹ�
*******************************************************************************/
static int at_ate_exec(int value)
{
	char cmdbuf[64] = {0};

	sprintf(cmdbuf, "%s%d\r\n", "ATE", value);
	if (txatcmd(cmdbuf) < 0)
		return -1;
	
	if (rxatcmd(cmdbuf, "OK", 0, 64, 500) <= 0)
		return -1;
	
	return 0;
}


/******************************************************************************
    ����˵������ѯ��Ʒ��Ϣ����
    �����������
    �����������
    �� �� ֵ��-1�� ʧ�ܣ�0���ɹ�
*******************************************************************************/
static int at_cgsn_exec(char *imei)
{
	char cmdbuf[64] = {0};

	if (txatcmd("AT+CGSN\r\n") < 0)
		return -1;
	
	if (rxatcmd(cmdbuf, "+CGSN:", "OK", 64, 500) <= 0)
		return -1;
	
	sscanf(cmdbuf, "+CGSN:%sOK", imei);
	
	return 0;
}


/******************************************************************************
    ����˵�������ò���������
    ���������bandrate��0(����Ӧ������)��600 ��1200 ��2400 ��4800��9600��14400��
			19200��28800��38400��57600��115200(Ĭ��ֵ) ��230400
    �����������
    �� �� ֵ��-1�� ʧ�ܣ�0���ɹ�
*******************************************************************************/
int at_ipr_write(char *bandrate)
{
	char cmdbuf[64] = {0};

	sprintf(cmdbuf, "AT+IPR=%s\r\n", bandrate);
	if (txatcmd(cmdbuf) < 0)
		return -1;
	
	if (rxatcmd(cmdbuf, "OK", 0, 64, 500) <= 0)
		return -1;
	return 0;
}


/******************************************************************************
    ����˵��������Ƿ���ڹ��� PIN ������
    �����������
    ���������code: SIM��PIN��Ȩ
    �� �� ֵ��-1�� ʧ�ܣ�0���ɹ�
*******************************************************************************/
static int at_cpin_read(char *pin)
{
	char cmdbuf[64] = {0};

	if (txatcmd("AT+CPIN?\r\n") < 0)
		return -1;
	
	if (rxatcmd(cmdbuf, "+CPIN:", "OK\r\n", 64, 500) <= 0)
		return -1;
	
	sscanf(cmdbuf, "+CPIN: %s\r\n", pin);
	
	return 0;
}


/******************************************************************************
    ����˵������ѯSIM����IMSI
    �����������
    ���������code: SIM��PIN��Ȩ
    �� �� ֵ��-1�� ʧ�ܣ�0���ɹ�
*******************************************************************************/
static int at_ccid_read(char *ccid)
{
	char cmdbuf[64] = {0};

	if (txatcmd("AT+CCID\r\n") < 0)
		return -1;
	
	if (rxatcmd(cmdbuf, "\r\n+CCID:", "\r\n\r\nOK\r\n", 64, 500) <= 0)
		return -1;
	
	sscanf(cmdbuf, "+CCID:%s\r\n\r\nOK\r\n", ccid);
	
	return 0;
}



/******************************************************************************
    ����˵����������汾
    �����������
    ���������vers: ����汾
    �� �� ֵ��-1�� ʧ�ܣ�0���ɹ�
*******************************************************************************/
static int at_getvers_exec(char *vers)
{
	char cmdbuf[64] = {0};

	if (txatcmd("AT+GETVERS\r\n") < 0)
		return -1;
	
	if (rxatcmd(cmdbuf, "\r\n", "OK\r\n", 64, 500) <= 0)
		return -1;
	
	sscanf(cmdbuf, "\r\n%s\r\n\r\nOK\r\n", vers);
	
	return 0;
}



/******************************************************************************
    ����˵����AT+CSQ ��ѯ�ź���������
    �����������
    ���������csq: �ź�ֵ
    �� �� ֵ��-1�� ʧ�ܣ�0���ɹ�
*******************************************************************************/
static int at_csq_exec(char *csq)
{
	char cmdbuf[64] = {0};
	int val = 0;

	if (txatcmd("AT+CSQ\r\n") < 0)
		return -1;
	if (rxatcmd(cmdbuf, "+CSQ:", "OK\r\n", 64, 500) <= 0)
		return -1;
	sscanf(cmdbuf, "+CSQ: %d,", &val);
	*csq = (char)val;
	return 0;
}


/******************************************************************************
    ����˵����AT+CREG ��ѯ����ע��״̬
    �����������
    ���������stat����ǰ����ע��״̬
		    0 ��û��ע�ᣬME���ڲ�û������ѰҪע����µ���Ӫ��
		    1 ��ע���˱�������
		    2 ��û��ע�ᣬ�� MS������ѰҪע����µ���Ӫ��
		    3 ��ע�ᱻ�ܾ�
		    4 ��δ֪ԭ��
		    5 ��ע������������
    �� �� ֵ��-1��ʧ�ܣ�0���ɹ�
*******************************************************************************/
static int at_creg_read(char *stat)
{
	char cmdbuf[64] = {0};

	if (txatcmd("AT+CREG?\r\n") < 0)
		return -1;
	if (rxatcmd(cmdbuf, "+CREG:", "OK\r\n", 64, 500) <=0)
		return -1;
	sscanf(cmdbuf, "+CREG: 0,%s\r\n", stat);
	return 0;
}


/******************************************************************************
    ����˵������ѯ��ǰ�� GPRS ����״̬
    �����������
    �����������
    �� �� ֵ��-1�� ʧ�ܣ�0���ɹ�
*******************************************************************************/
int at_cgatt_read(char *stat)
{
	char cmdbuf[64] = {0};

	if (txatcmd("AT+CGATT?\r\n") < 0)
		return -1;
	if (rxatcmd(cmdbuf, "+CGATT: ", "OK\r\n", 64, 500) <= 0)
		return -1;
	*stat = cmdbuf[8];
	return 0;
}


/******************************************************************************
    ����˵������ȡ��ǰ�Ѿ�ע����������Ӫ����Ϣ
    �����������
    ���������opt:��Ӫ����Ϣ
    �� �� ֵ��-1�� ʧ�ܣ�0���ɹ�
*******************************************************************************/
int at_cops_read(char *opt)
{
	char cmdbuf[64] = {0};
	int i;

	if (txatcmd("AT+COPS?\r\n") < 0)
		return -1;
	if (rxatcmd(cmdbuf, "+COPS:", "OK\r\n", 64, 500) <= 0)
		return -1;
	for (i = 0; i < 16; i++) {
		if (cmdbuf[12 + i] == '"') {
			opt[i] = '\0';
			break;
		}
		opt[i] = cmdbuf[12 + i];
	}
	return 0;
}



/******************************************************************************
    ����˵����дInternet ������������, ÿ����������Ϊ1024���ֽ�
    ���������id��Profile�ı�ʶ
	      txbuf�����������׵�ַ
	      txlen���������ݳ���
    �����������
    �� �� ֵ��-1�� ʧ�ܣ�0���ɹ�
*******************************************************************************/
static int at_tcp_send(const char *id, char *txbuf, int txlen)
{
	char cmdbuf[64] = {0};
	int ret;
	
	if ((txlen > TXMAXLEN) || (txbuf == NULL))
		return -1;
	
	sprintf(cmdbuf, "AT+TCPSEND=%s,%d\r\n", id, txlen);
	if (txatcmd(cmdbuf) < 0)
		return -1;
	
	if (rxatcmd(cmdbuf, "\r\n>", 0, 64, 500) <= 0)
		return -1;

	txbuf[txlen++] = 0x0d;
	
	ret = uart2_send_byte(txbuf, txlen);
	if (ret < 0)
		return -1;
	
	if (rxatcmd(cmdbuf, "OK", 0, 64, 500) <= 0)
		return -1;
	
	return 0;
}


/******************************************************************************
    ����˵����m590ģ����ػ�
    �����������
    �����������
    �� �� ֵ��-1�� ʧ�ܣ�0���ɹ�
*******************************************************************************/
static int m590_shutdown(void)
{
	tcp_link_led_off();
	return at_cpwroff_exec();
}


/******************************************************************************
    ����˵����m590ģ��ص�Դ
    �����������
    �����������
    �� �� ֵ����
*******************************************************************************/
static int m590_poweroff(void)
{
	M590_POWER_OFF;
	DELAYMS(1000);
	OSTimeDlyHMSM(0, 0, 0, 0);
	return 0;
}


/******************************************************************************
    ����˵����m590ģ�鿪��Դ
    �����������
    �����������
    �� �� ֵ����
*******************************************************************************/
static int m590_poweron(void)
{
	/* �ڴ�ģ���Դǰ���������ű�����Ϊ�ߵ�ƽ */
	M590_TERN_ON_HIGH;
	M590_POWER_ON;
	DELAYMS(1000);
	return 0;
}


/******************************************************************************
    ����˵����m590ģ������
    ���������com���˿ڲ���
    �����������
    �� �� ֵ��-1�� ʧ�ܣ�0���ɹ�
*******************************************************************************/
int m590_init(void)
{	
	/* ��ػ� */
	m590_shutdown();

	/* �ص�Դ */
	m590_poweroff();

	/* ����Դ */
	m590_poweron();

	/* ģ�鿪�� */
	M590_TERN_ON_HIGH;
	DELAYMS(750);
	M590_TERN_ON_LOW;	
	
	DELAYMS(5000);
	
	/* AT������Կ����Ƿ�ɹ� */
	if (at_at_exec() < 0)
		return -1;

	return 0;
}


/******************************************************************************
    ����˵����m590ģ���������
    �����������
    ���������info: ģ���ʼ����Ϣ
    �� �� ֵ��-n�� ʧ�ܣ�0���ɹ�
*******************************************************************************/
int m590_config(struct m590info *info)
{
	int i;

	/* �ػ��� */
	if (at_ate_exec(0))
		return -1;

	/* �鿴����汾����Ϣ */
	if (at_getvers_exec(info->software_v))
		return -2;	
	
	/* read imei */
	if (at_cgsn_exec(info->imei))
		return -3;
	
	/* ��ȡSIM��IMSI����, ���SIM���Ƿ��λ */
	for (i = 0; i < 10; i++) {
		if (at_ccid_read(info->imsi) == 0)
			break;
		if (i >= 9)
			return -4;
		DELAYMS(1000);
	}

	/* ���SIM��PIN���Ƿ��Ȩ�ɹ� */
	for (i = 0; i < 10; i++)  {
		memset(info->simcpin, 0, 15);
		if (at_cpin_read(info->simcpin))
			return -4;
		if (strstr(info->simcpin, "READY") != NULL)
			break;
		if (i >= 9)
			return -5;
		DELAYMS(1000);
	}

	/* ����ź��Ƿ����� */
	for (i = 0; i < 20; i++)  {
		if (at_csq_exec(info->csq) < 0)
			return -6;
		if ((info->csq[0] > 0) && (info->csq[0] <= 31))
			break;
		if (i > 19)
			return -7;
		DELAYMS(2000);
	}

	/* �����Ƿ�ע������ */
	for (i = 0; i < 20; i++) {
		if (at_creg_read(info->netreg) < 0)
			return -8;
		if ((info->netreg[0] == '1') || (info->netreg[0] == '5'))
			break;
		if (i >= 19)
			return -9;
		DELAYMS(1000);
	}

	return 0;
}



static int at_xisp_write(void)
{
	char cmdbuf[64] = {0};

	if (txatcmd("AT+XISP=0\r\n") < 0)
		return -1;
	
	if (rxatcmd(cmdbuf, "OK", 0, 64, 1000) <= 0)
		return -1;
	
	return 0;
}


static int at_cgdcont(const char *apn)
{
	char cmdbuf[64] = {0};

	sprintf(cmdbuf, "AT+CGDCONT=1,\"IP\",\"%s\"\r\n", apn);
	if (txatcmd(cmdbuf) < 0)
		return -1;
	
	if (rxatcmd(cmdbuf, "OK", 0, 64, 500) <= 0)
		return -1;
	
	return 0;
}


static int at_xiic_write(void)
{
	char cmdbuf[64] = {0};

	if (txatcmd("AT+XIIC=1\r\n") < 0)
		return -1;
	
	if (rxatcmd(cmdbuf, "OK", 0, 64, 500) <= 0)
		return -1;
	
	return 0;
}


static int at_xiic_read(char *stat, char *localip)
{
	char cmdbuf[64] = {0};

	if (txatcmd("AT+XIIC?\r\n") < 0)
		return -1;
	
	if (rxatcmd(cmdbuf, "+XIIC:", 0, 64, 500) <= 0)
		return -1;
	
	*stat = cmdbuf[10];
	
	return 0;
}


static int at_tcpsetup_write(char *ip, unsigned short port)
{
	char cmdbuf[64] = {0};
	char ovt_ms = 60;
	int len;

	sprintf(cmdbuf, "AT+TCPSETUP=0,%s,%d\r\n", ip, port);
	if (txatcmd(cmdbuf) < 0)
		return -1;
	
	while (ovt_ms--) {
		len = rxatcmd(cmdbuf, "\r\n", "\r\n", 64, 1000);
		if (len > 0) {
			if (strstr(cmdbuf, "+TCPSETUP:0,OK") != NULL) {
				tcp_link_led_on();
				return 0;
			}
			if (strstr(cmdbuf, "OK") != NULL)
				continue;
			if (strstr(cmdbuf, "+TCPCLOSE") != NULL)
				return -1;
		}	
	}	
		
	return -1;
}




/******************************************************************************
    ����˵����m590ģ����ÿͻ���ģ�������������TCP����
    �����������
    ���������tcp: ģ��TCP������Ϣ
    �� �� ֵ��-1�� ����0���ɹ�
*******************************************************************************/
int m590_tcp_link(struct m590tcp *tcp)
{
	int i;
	int ret;

	/* use self tcp ptl */
	if (at_xisp_write() < 0)
		return -1;
	
	if (at_cgdcont(tcp->apn) < 0)
		return -1;

	if (at_xiic_write() < 0)
		return -1;
	
	/* ppp link */
	for (i = 0; i < 30; i++) {
		if (at_xiic_read(tcp->gprsstat, tcp->localip) < 0)
			return -1;
		if (tcp->gprsstat[0] == '1')
			break;
		if (i >= 29)
			return -1;
		DELAYMS(2000);
	}

	ret = at_tcpsetup_write(tcp->severip, tcp->severport);
	if (ret < 0)
		return -1;
	
	
	return 0;
}


/******************************************************************************
    ����˵����m590ģ���ȡ����
    �����������
    �����������
    �� �� ֵ����
*******************************************************************************/
int m590_read(char *rxbuf, int size, int ovt)
{
	int len = 0;
	char *index;

	if ((rxbuf == NULL) || (size <= 0))
		return -1;
		
	len = rxatcmd(rxbuf, "\r\n", "\r\n", size, ovt);
	if (len > 0) {			
		if (strstr(rxbuf, "TCPCLOSE") != NULL)
			return -1;
		
		index = strstr(rxbuf, "+TCPRECV:");
		if (index != NULL) {	 
			sscanf(index, "+TCPRECV:0,%d,", &len);
			if (len > size)
				len = size;
		
			index = strstr(index + 11, ",");
			if (index != NULL) {	
				memcpy(rxbuf , index + 1, len);
				return len;
			}
		}
	}
			 
	return 0;
}


/******************************************************************************
    ����˵����m590ģ��д������
    �����������
    �����������
    �� �� ֵ����
*******************************************************************************/
int m590_write(char *wbuf, int wlen)
{
	if ((wbuf == NULL) || (wlen <= 0))
		return -1;

	if (at_tcp_send("0", wbuf, wlen) < 0)
		return -1;
	
	return 0;
}
