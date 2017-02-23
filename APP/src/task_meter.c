/*
 * task_meter.c
 * likejshy@126.com
 * 2017-01-08
 */

#include <string.h>
#include <stdio.h>
#include <ucos_ii.h>
#include "BSP_CFG.h"
#include "Serial.h"
#include "GDW376_CFG.h"

//#define CFG_DEBUG

#ifdef CFG_DEBUG
#define Print(fmt,args...) printf(fmt, ##args)
#else
#define Print(fmt,args...)
#endif


struct meter_ctl {
	unsigned char collect_time[5];
	unsigned short index;
	unsigned short pn;
	unsigned char port;
	unsigned char bps;
	unsigned char ptl_type;
	unsigned char send_index;
	unsigned long DI1DI0;
	unsigned char meter_addr[6];
};


static int collect_meter_led(void )
{	
	static unsigned short status;
	
	if (status == 0) {
		status = 1;
		METERDATALIGHTON(); //���ݵ���
		return 0;
	}
	
	if (status == 1) {
		status = 0;
		METERDATALIGHTOFF(); //���ݵ���
		return 0;
	}
	
	return 0;
}

static int collect_meter_cycle(unsigned char *cycle_minute)
{
	*cycle_minute = ZDP.F33.PortP[0].RTimeDis;
	return 0;
}



static int collect_meter_time_read(unsigned char *time_minute,
				   unsigned char *collect_time_bcd)
{
	collect_time_bcd[0] = TBCD.Min;
	collect_time_bcd[1] = TBCD.Hour;
	collect_time_bcd[2] = TBCD.Day;
	collect_time_bcd[3] = TBCD.Month;
	collect_time_bcd[4] = TBCD.Year;

	*time_minute = THEX.Min;
	return 0;
}



static int collect_meter_time_up(unsigned char *collect_time)
{
	unsigned char cycle_minute = 0;
	unsigned char time_minute;
	int ret;
	static unsigned char old_minute = 0xFF;

	/* ��ȡ��������: ���� */
	ret = collect_meter_cycle(&cycle_minute);
	if (ret < 0)
		return 0;
	
	/* ��ȡϵͳ��ǰʱ��ķ��� */
	ret = collect_meter_time_read(&time_minute, collect_time);
	if (ret < 0)
		return 0;
	
	/* ÿ����һ�Σ������ǰ���ӷ������� */
	if (old_minute != time_minute) {
		old_minute = time_minute;	
		/* �����ǰʱ��ķ����ǳ���ʱ���������ʱ����Ϊ����ʱ�䵽�� */
		if ((time_minute % cycle_minute) == 0)
			return 1;
	}

	return 0;
}



static int collect_meter_num_get(unsigned short *meter_num)
{
	unsigned short num;

	num = ZDP.F10. MeterNum[1] * 0x100 + ZDP.F10. MeterNum[0];
	if (num == 0)
		num = 1;

	if (num > METERNUM)
		num = METERNUM;

	*meter_num = num;

	return 0;
}



static int meter_pre_ready(struct meter_ctl *ctl, unsigned char meter_index)
{
	unsigned char i = meter_index;

	ctl->index = ZDP.F10.Meter[i].Index[1] * 0x100 + ZDP.F10.Meter[i].Index[0];
	if (ctl->index == 0)
		return 0;

	ctl->pn = ZDP.F10.Meter[i].Point[1] * 0x100 + ZDP.F10.Meter[i].Point[0];
	if (ctl->pn == 0)
		return 0;

	ctl->port = ZDP.F10.Meter[i].BPS_Port & 0x1F;
	ctl->bps = (ZDP.F10.Meter[i].BPS_Port >> 5) & 0x07;
	UARTx_BaudRate_Set(0x03, 5);//;�����ͨ��ֻ֧��2400bps --20141229

	ctl->ptl_type = ZDP.F10.Meter[i].Type & 0x1F;
	if (ctl->ptl_type == 1 || ctl->ptl_type == 30) {
		memcpy(ctl->meter_addr, ZDP.F10.Meter[i].Addr, 6);
		return 1;
	}

	return 0;
}



static int collect_meter_frame_pack(unsigned char *txbuf, unsigned long DI1DI0,
				    unsigned char ptl)
{
	unsigned char index = 0;
	unsigned char i, sum = 0;

	txbuf[index++] = 0xFE;
	txbuf[index++] = 0xFE;
	txbuf[index++] = 0xFE;
	txbuf[index++] = 0xFE;
	txbuf[index++] = 0x68;
	txbuf[index++] = 0xAA;
	txbuf[index++] = 0xAA;
	txbuf[index++] = 0xAA;
	txbuf[index++] = 0xAA;
	txbuf[index++] = 0xAA;
	txbuf[index++] = 0xAA;
	txbuf[index++] = 0x68;

	if (ptl == GYType2007) {
		txbuf[index++] = 0x11; //��׼��Լ������
		txbuf[index++] = 0x04;
		txbuf[index++] = (unsigned char)DI1DI0 + 0x33;
		txbuf[index++] = (unsigned char)(DI1DI0 >> 8) + 0x33;
		txbuf[index++] = (unsigned char)(DI1DI0 >> 16) + 0x33;
		txbuf[index++] = (unsigned char)(DI1DI0 >> 24) + 0x33;
	} else if (ptl == GYType1997) {
		txbuf[index++] = 0x01; //��׼��Լ������
		txbuf[index++] = 0x02;
		txbuf[index++] = (unsigned char)DI1DI0 + 0x33;
		txbuf[index++] = (unsigned char)(DI1DI0 >> 8) + 0x33;
	} else {
		return 0;
	}

	for(i = 4; i < index; i++)
		sum += txbuf[i];

	txbuf[index++] = sum;
	txbuf[index++] = 0x16;

	return index;
}



static int meter_frame_send(unsigned char *txbuf, int len)
{
	int ret;


	
	ret = uart5_send_byte(txbuf, len);
	if (ret < 0)
		return -1;

	return 0;
}


static int meter_frame_receive(unsigned char *rxbuf, int size, int ovt_s)
{
	int len = 0;
	int ovt_ms = ovt_s * 1000;

	while (ovt_ms --) {
		len = uart5_receive_packet(rxbuf, size, 100);
		if (len > 0) {
			collect_meter_led();
			break;
		}

		OSTimeDly(1);
	}

	return len;
}


/* ���������� */
static int collect_meter_frame_unpack(struct meter_ctl *ctl, unsigned long DI1DI0,
				      unsigned char *rxbuf, int len)
{
	unsigned char Len;
	struct PortCtrl *Port = &Port1;

	/* ����ǲ���645���� */
	Len = DL645_CHK(rxbuf, len);
	if (Len != 0xFF) {
		/* ����Ƿ�һ�� */
		if (MeterNumChk(ZDP.F10.Meter[0].Addr, rxbuf + Len + 1)) {
			MovNByte(6, ZDP.F10.Meter[0].Addr, (u8*)(rxbuf + Len + 1));
			EW.CSB1 = 0xC1;
			if (!f_is_sole_data ((u8*)(rxbuf + Len + 1), 6)) { /*���Ϊȫ0,���޸�������*/
				u8 buf[4];

				buf[0] = 0x25;
				buf[1] = ZDP.F10.Meter[0].Addr[3];
				buf[2] = ZDP.F10.Meter[0].Addr[2];
				buf[3] = ZDP.F10.Meter[0].Addr[1];
				EW.MoveByte = 'M';
				MovNByte(4,  ZDPKZ.F89.QuXian,(u8*)&buf[0]);
				EW.MoveByte = 0;
				EW.CSB8 = 0xC8;
				//GprsC.ReConnect = true; //;�ն���������ģ���Ҫ��������
			}
		}

		if (Port == &Port1) {
			//METERDATALIGHTON(); //;���ݵ���
			//PORT1USARTCTR.DataLightMs = 100;
			//if (comstat == false)
			{
				//comstat = true;
				//COMLIGHTON();  //;ͨ������ָʾ
			}
		}

		Port->GYType = ctl->ptl_type;
		Port->IndSend = ctl->send_index;
		Port->DI1DI0 = DI1DI0;
		ZDCB_MovDataDI(rxbuf + Len, &PMD1, Port); //;���Ӷ˿ڽ��յ�������д�뵽PMD��
		return 1;
	}
	
	return 0;
}



static int colleter_meter(struct meter_ctl *ctl, unsigned char index)
{
	unsigned char di_num = 0, i;
	unsigned long ComTab_Head, DI1DI0;
	unsigned char buff[256] = {0};
	char no_ack_num = 0;
	int len, ret, size;
        
	/* ����ͨ��Э�飬��ȡDI���ĵ�ַͷ */
	if (ctl->ptl_type == GYType2007) {
		di_num = DITab_2007_Num;
		ComTab_Head = (u32)DITab_2007;
	} else if (ctl->ptl_type == DITab_1997_Num) {
		di_num = DITab_2007_Num;
		ComTab_Head = (u32)DITab_1997;
	} else {
		return -1;
	}
	
	COMLIGHTOFF();//;ͨ��ʧ��ָʾ
	
	for (i = 0; i < di_num; i++) {

		/* ȡ��DI�����ʶ*/
		DI1DI0 = *((u32*)(ComTab_Head + i * DITab_Num));	
		ctl->send_index = i;
		
		/* ��װ������ */
		len = collect_meter_frame_pack(buff, DI1DI0, ctl->ptl_type);
		if (len > 0) {
			/* ��˿ڷ��ͱ��� */
			ret = meter_frame_send(buff, len);
			if (ret < 0)
				return -1;
			
			/* �ȴ����ն˿ڱ��� */
			len = meter_frame_receive(buff, 256, 5);
			if (len < 0)
				return -1;

			if (len > 0) {							
				/* �Խ��յ��ı��Ľ��н��� */
				ret = collect_meter_frame_unpack(ctl, DI1DI0, buff, len);
				if (ret == 1)
					continue;
			}

			if (len == 0) {								
				no_ack_num++;
				if (no_ack_num > 5)
					return -1;
			}
		}
	}

	return 0;
}



/* �����γ��������д���Ӧ�Ĳ����� */
static int copy_meter_data_to_point(struct meter_ctl *ctl)
{
	ZDCB_MovDataPn(ctl->pn - 1, ctl->ptl_type, &PMD1);
}



/* ������̴��� */
int collect_meter_process(unsigned char *collect_time, unsigned char meter_index)
{
	struct meter_ctl ctl = {0};
	int ret;

	memcpy(ctl.collect_time, collect_time, 5);
	ret = meter_pre_ready(&ctl, meter_index);
	if (ret < 0)
		return -1;

	ret = colleter_meter(&ctl, meter_index);
	if (ret < 0)
		return -1;

	ret = copy_meter_data_to_point(&ctl);
	if (ret < 0)
		return -1;
}



/* ��ȡ����DI������ */
unsigned char config_meter_di_get(unsigned long *di, unsigned char meter_index)
{
	unsigned char num;

	num = CtrlOrder.IndSave;
	if (num == 0)
		return 0;

	memcpy((char *)di, (char *)&CtrlOrder.DI, num * 4);

	memset((char *)&CtrlOrder.DI, 0, num * 4);
	CtrlOrder.IndSave = 0;

	return num;
}


/* ��װ����� */
static int config_meter_frame_pack(unsigned char *txbuf, unsigned char DI1DI0,
				   unsigned char ptl)
{
	u8 i, j = 0, sum = 0, n;
	u32 DI;
	struct PortCtrl *Port = &Port1;
	extern	u8 GprsSign;

	Port->Buffer = txbuf;
	Port->GYType = ptl;
	DI = DI1DI0;

	*(Port->Buffer + j++ ) = 0xFE;
	*(Port->Buffer + j++ ) = 0xFE;
	*(Port->Buffer + j++ ) = 0xFE;
	*(Port->Buffer + j++ ) = 0xFE;
	*(Port->Buffer + j++ ) = 0x68;

	*(Port->Buffer + j++ ) = 0xAA;	//;���
	*(Port->Buffer + j++ ) = 0xAA;
	*(Port->Buffer + j++ ) = 0xAA;
	*(Port->Buffer + j++ ) = 0xAA;
	*(Port->Buffer + j++ ) = 0xAA;
	*(Port->Buffer + j++) = 0xAA;
	*(Port->Buffer + j++) = 0x68;

	if (Port->GYType == GYType2007) {
		*(Port->Buffer + j++) = 0x14; //;��׼��Լ�����
		n = j;
		*(Port->Buffer + j++) = 12; //;��׼��Լ�����
		*(Port->Buffer + j++) = (u8)DI + 0x33;
		*(Port->Buffer + j++) = (u8)(DI >> 8) + 0x33;
		*(Port->Buffer + j++) = (u8)(DI >> 16) + 0x33;
		*(Port->Buffer + j++) = (u8)(DI >> 24) + 0x33;
		*(Port->Buffer + j++) = 0x33;	 //;������Ȩ��
		*(Port->Buffer + j++) = 0x33;
		*(Port->Buffer + j++) = 0x33;
		*(Port->Buffer + j++) = 0x33;
		*(Port->Buffer + j++) = 0x33;	 //;�����ߴ���
		*(Port->Buffer + j++) = 0x33;
		*(Port->Buffer + j++) = 0x33;
		*(Port->Buffer + j++) = 0x33;

		switch(DI) {
		case 0x04001301: { //;����״̬
			*(Port->Buffer + j++) = GprsSign + 0x33;    //; �źŵĴ�С
			*(Port->Buffer + n) += 1; //;���ݸ���
		}
		break;
		case 0x04000101: { //;����
			*(Port->Buffer + j++) = (TBCD.Month >> 5) + 0x33;
			*(Port->Buffer + j++) = TBCD.Day + 0x33;
			*(Port->Buffer + j++) = (TBCD.Month & 0x1F)  + 0x33;
			*(Port->Buffer + j++) = TBCD.Year + 0x33;
			*(Port->Buffer + n) += 4; //;���ݸ���
		}
		break;
		case 0x04000102: { //;����
			*(Port->Buffer + j++) = TBCD.Sec + 0x33 ; //;��
			*(Port->Buffer + j++) = TBCD.Min  + 0x33; //;��
			*(Port->Buffer + j++) = TBCD.Hour + 0x33; //;ʱ
			*(Port->Buffer + n) += 3; //;���ݸ���
		}
		break;

		default:
			break;
		}
	} else if(Port->GYType == GYType1997) {
		*(Port->Buffer + j++) = 0x04; //;��׼��Լ�����
		n = j;
		*(Port->Buffer + j++) = 6; //;��׼��Լ�����
		*(Port->Buffer + j++) = (u8)DI + 0x33;
		*(Port->Buffer + j++) = (u8)(DI >> 8) + 0x33;
		*(Port->Buffer + j++) = 0x33;	 //;������Ȩ��
		*(Port->Buffer + j++) = 0x33;
		*(Port->Buffer + j++) = 0x33;
		*(Port->Buffer + j++) = 0x33;
		switch((u16)DI) {
		case 0xCA00: {
			*(Port->Buffer + j++) = GprsSign + 0x33;    //; �źŵĴ�С
			*(Port->Buffer + n) += 1;
		}
		break;
		case 0xC900: { //;IP��ַ1��
			*(Port->Buffer + n) += 4;
			*(Port->Buffer + j++) = ZDP.F3.IP1[0] + 0x33;
			*(Port->Buffer + j++) = ZDP.F3.IP1[1] + 0x33;
			*(Port->Buffer + j++) = ZDP.F3.IP1[2] + 0x33;
			*(Port->Buffer + j++) = ZDP.F3.IP1[3] + 0x33;
		}
		break;
		case 0xC901: { //;�˿ڵ�ַ1
			*(Port->Buffer + n) += 2;
			*(Port->Buffer + j++) = ZDP.F3.Port1[0] + 0x33;
			*(Port->Buffer + j++) = ZDP.F3.Port1[1] + 0x33;
//;		      *(Port->Buffer + j++) = ZDP.F3.Port1[1] + 0x33;
//;		      *(Port->Buffer + j++) = ZDP.F3.Port1[0] + 0x33;
		}
		break;
//;	       case 0xF313: //;������ C010
//;		  {
//;		      j = 13;
//;		      *(Port->Buffer + j++) = 0x0A;
//;		      *(Port->Buffer + j++) = 0x10 + 0x33;
//;		      *(Port->Buffer + j++) = 0xC0 + 0x33;
//;		      j = 20;
//;		      *(Port->Buffer + j++) = 0x00 + 0x33;
//;		      *(Port->Buffer + j++) = TBCD_bak.Day + 0x33;
//;		      *(Port->Buffer + j++) = TBCD_bak.Month & 0x0F + (TBCD_bak.Month & 0x10) * 10  + 0x33;
//;		      *(Port->Buffer + j++) = TBCD_bak.Year + 0x33;
//;
//;		  }
//;		  break;
//;	       case 0xF314: //;��ʱ�� C011
//;		  {
//;		       j = 13;
//;		       *(Port->Buffer + j++) = 0x09;
//;		       *(Port->Buffer + j++)  = 0x11 + 0x33;
//;		       *(Port->Buffer + j++)  = 0xC0 + 0x33;
//;		       j = 20;
//;		       *(Port->Buffer + j++) = TBCD_bak.Sec + 0x33 ; //;��
//;		       *(Port->Buffer + j++) = TBCD_bak.Min  + 0x33; //;��
//;		       *(Port->Buffer + j++) = TBCD_bak.Hour + 0x33; //;ʱ
//;		  }
//;		  break;
		case 0xC902: { //;IP��ַ2��
			*(Port->Buffer + n) += 4;
			*(Port->Buffer + j++) = ZDP.F3.IP2[0] + 0x33;
			*(Port->Buffer + j++) = ZDP.F3.IP2[1] + 0x33;
			*(Port->Buffer + j++) = ZDP.F3.IP2[2] + 0x33;
			*(Port->Buffer + j++) = ZDP.F3.IP2[3] + 0x33;
		}
		break;
		case 0xC903: { //;�˿ڵ�ַ2
			*(Port->Buffer + n) += 2;

			*(Port->Buffer + j++) = ZDP.F3.Port2[0] + 0x33;
			*(Port->Buffer + j++) = ZDP.F3.Port2[1] + 0x33;
		}
		break;
		case 0xC904: { //; APN
			*(Port->Buffer + n) += 16;

			for (i = 0; i < 16; i++) {
				*(Port->Buffer + j++) = ZDP.F3.APN[i] + 0x33;
			}
		}
		break;
		case 0xC905: { //;��վ�ֻ��绰����
			*(Port->Buffer + n) += 8;

			for (i = 0; i < 8; i++) {
				*(Port->Buffer + j++) = ZDP.F4.Tel_Code[7-i] + 0x33;
			}
		}
		break;
		case 0xC906: { //;��վ�������ĺ���
			*(Port->Buffer + n) += 8;

			for(i = 0; i < 8; i++) {
				*(Port->Buffer + j++) = ZDP.F4.SMS_Code[7-i] + 0x33;
			}
		}
		break;
//;	       case 0xC040: //;��ѹ����������
//;		  {
//;		      *(Port->Buffer + j - 7) = 10 ;
//;
//;		      *(Port->Buffer + j++) = 0 + 0x33;
//;		      *(Port->Buffer + j++) = P1P.F25.UMul[0] + 0x33; //;PT���
//;		      *(Port->Buffer + j++) = P1P.F25.UMul[1] + 0x33;
//;		      *(Port->Buffer + j++) = 0 + 0x33;
//;		  }
//;		  break;
//;	       case 0xC041:  //;��������������
//;		  {
//;		      *(Port->Buffer + j - 7) = 10 ;
//;
//;		      *(Port->Buffer + j++) = 0 + 0x33;
//;		      *(Port->Buffer + j++) = P1P.F25.IMul[0] + 0x33; //;CT���
//;		      *(Port->Buffer + j++) = P1P.F25.IMul[1] + 0x33;
//;		      *(Port->Buffer + j++) = 0 + 0x33;
//;		  }
//;		  break;
//;	       case 0xC141:  //;���ѹ
//;		  {
//;		      *(Port->Buffer + j - 7) = 9 ;
//;
//;		      *(Port->Buffer + j++) = 0 + 0x33;
//;		      *(Port->Buffer + j++) = P1P.F25.Un[0] + 0x33;
//;		      *(Port->Buffer + j++) = P1P.F25.Un[1] + 0x33;
//;		      break;
//;		  }
		case 0xC907: { //;�ն�����������
			*(Port->Buffer + n) += 2;

			*(Port->Buffer + j++) = ZDPKZ.F89.QuXian[0] + 0x33;
			*(Port->Buffer + j++) = ZDPKZ.F89.QuXian[1] + 0x33;
			break;
		}
		case 0xC908: { //;�ն˵�ַ
			u8 DiZhi[2];

			*(Port->Buffer + n) += 2;

			U16To2BCD(*(u16*)ZDPKZ.F89.DiZhi, DiZhi);
			*(Port->Buffer + j++) = DiZhi[0] + 0x33;
			*(Port->Buffer + j++) = DiZhi[1] + 0x33;
			break;
		}
		case 0xC912: {
			*(Port->Buffer + n) += 1;
			*(Port->Buffer + j++) = (ZDP.F1.HBeat / 10) * 16 + (ZDP.F1.HBeat % 10)+ 0x33;
			break;
		}
		case 0xC910: {
			*(Port->Buffer + n) += 1;
			*(Port->Buffer + j++) = (ZDP.F8.WorkMode & 0x03) + 0x33;
			break;
		}
		case 0xC911: {
			u16 t;
			*(Port->Buffer + n) += 2;
			t = ZDP.F8.ActRedailSec[0] + (ZDP.F8.ActRedailSec[1] << 8);
			t = t / 60;
			U16To2BCD(t, (u8*)Port->Buffer + j);
			*(Port->Buffer + j++) += 0x33;
			*(Port->Buffer + j++) += 0x33;
			break;
		}
		case 0x6601: {
			*(Port->Buffer + n) += 1;
			*(Port->Buffer + j++) = CtrlOrder.Order[CtrlOrder.IndSend % SBNUM] + 0x33;
		}
		default:
			break;
		}
	}


	for(i = 4; i < j; i++) {
		sum += *(Port->Buffer + i);
	}

	*(Port->Buffer + j++) = sum;
	*(Port->Buffer + j++) = 0x16;
	Port->Len = j;
	
	return Port->Len;
}


/* ����Ľ��� */
static int config_meter_frame_unpack(struct meter_ctl *ctl, unsigned long DI1DI0,
				     unsigned char *rxbuf, int len)
{
	unsigned char Len;
	
	/* 645֡��·����ȷ�Լ�� */
	Len = DL645_CHK(rxbuf, len);
	if (Len != 0xFF) {
             /* ���ɹ� */
	     if ((*(rxbuf + 8 + Len) == 0x84)||(*(rxbuf + 8 + Len) == 0x94))
                   return 1;
	 }

	return 0;
}


/* ����� */
int config_meter(unsigned long *di, unsigned char di_num,
		 struct meter_ctl *ctl, unsigned char meter_index)
{
	unsigned char  i;
	unsigned char buff[256] = {0}; /* �����շ����� */
	char no_ack_num = 0;
	int len, ret, size;

	for (i = 0; i < di_num; i++) {

		/* ����DI��װ���645����֡ */
		len = collect_meter_frame_pack(buff, di[i], ctl->ptl_type);
		if (len > 0) {
			
			/* ���ͱ��� */
			ret = meter_frame_send(buff, len);
			if (ret < 0)
				return -1;

			/* �ȴ����յ��Ӧ���ģ������5�� */
			len = meter_frame_receive(buff, size, 5);
			if (len < 0)
				return -1;
			
			/* �յ����Ӧ������ */
			if (len > 0) {

				/* �Ե��Ӧ������ݽ��н��� */
				ret = config_meter_frame_unpack(ctl, di[i], buff, len);
				if (ret == 1) {
					no_ack_num = 0;
					continue;
				}			
			}
		
			/* 
			 * ���û���յ����Ӧ�𣬻�Ӧ���������ͼ�����
			 * �������ﵽ5�ξ���Ϊ���γ���ʧ�ܣ��˳�������һ������
			 */
			no_ack_num++;
			if (no_ack_num > 5)
				return -1;
		}
	}

	return 0;
}



int config_meter_process(unsigned char meter_index)
{
	struct meter_ctl ctl = {0};
	unsigned long cfg_di[30];
	unsigned char cfg_di_num;
	int ret;
	
	/* ��ȡ��Ҫ����DI���ͺ�DI���� */
	cfg_di_num = config_meter_di_get(cfg_di, meter_index);
	if (cfg_di_num == 0)
		return 0;
	
	/* ���ͨѶǰ��׼������������ͨѶ�˿ڵ�*/
	ret = meter_pre_ready(&ctl,  meter_index);
	if (ret < 0)
		return -1;
	
	/* �����·����DI */
	ret = config_meter(cfg_di, cfg_di_num, &ctl, meter_index);
	if (ret < 0)
		return -1;

	return 0;
}



void App_Task_Meter (void *p_arg)
{
	int ret;
	unsigned short num, i;
	unsigned char collect_time[5]; /* ����ʱ�� */
	unsigned char collect_en = 0;  /* ����ʹ�ܱ�־ */
	
	while (1) {
		
		Print("collect meter start !\r\n");
		
		/* ��ѯ����ʱ���Ƿ��� */
		collect_en = collect_meter_time_up(collect_time);
		
		/* ��ȡ�ܹ���Ҫ��������� */
		collect_meter_num_get(&num);
		
		/* �����еĵ������г������� */
		for (i = 0; i < num; i++) {

			/* ����� */
			ret = config_meter_process(i);
			if (ret < 0) {
				Print("config_meter_process error![%d]\r\n", ret);
			}

			if (collect_en) {
				/* ������ */
				ret = collect_meter_process(collect_time, i);
				if (ret < 0) {
					Print("collect_meter_process error![%d]\r\n", ret);
				}
			}

		}

		OSTimeDlyHMSM(0, 0, 1, 0);
	}
}
