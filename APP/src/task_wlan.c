
/*
 * task_wlan.c
 * likejshy@126.com
 * 2017-01-08
 */

#include <string.h>
#include <stdio.h>
#include <ucos_ii.h>

#include "noway_m590.h"
#include "GDW376_CFG.h"
#include "task_wlan.h"
#include "task_ptl.h"


/* WLAN��������С */
#define WLAN_BUFF_SIZE 512

OS_EVENT *WlanMboxFd;

struct wlan_info
{
	struct m590info cfg;
	struct m590tcp  tcp;
	char link_status;
} ;


static struct wlan_info wlan;


int wlan_info_link_status(void)
{
	return 	wlan.link_status;
}




static int wlan_init(void)
{
	WlanMboxFd = OSMboxCreate (NULL);
	if (WlanMboxFd != 0)
		return 0;	
	return -1;
}


static int wlan_tcp_data_post(char *buff, int len)
{
	struct WlanMsgStr WlanMsg;
	struct PtlMsgStr *PtlMsg = 0;
	INT8U ret, perr;
	unsigned short ovt_ms;
	
	/* post message to ptl task */
	WlanMsg.buff = buff;
	WlanMsg.len = len;		
	ovt_ms = 1000;
	do {
		ret = OSMboxPost (WlanMboxFd, &WlanMsg);
		ovt_ms--;
		if (ovt_ms == 0)
			return -1;		
		OSTimeDly(1);
	} while (ret != OS_ERR_NONE);
	
	/* wait until the message is unpacked  by ptl task */
	PtlMsg = OSMboxPend (PtlMboxFd, 1000, &perr);
	if ((perr == OS_ERR_NONE) && (PtlMsg != 0))
		return 0;
	
	return -1;
}


static int wlan_tcp_data_pend(char *buff, int size, int ovt)
{
	struct WlanMsgStr WlanMsg;
	struct PtlMsgStr *PtlMsg;	
	int len;
	INT8U ret, perr;
	
	PtlMsg = OSMboxPend (PtlMboxFd, ovt, &perr);
	if (perr != OS_ERR_NONE) 
		return -1;
	
	if ((PtlMsg != 0) && (PtlMsg->len > 0)) {			
		len = PtlMsg->len;
		if (len > size)
			len = size;
		memcpy(buff, PtlMsg->buff, len);	
		WlanMsg.buff = 0;
		WlanMsg.len = 0;		

		ret = OSMboxPost (WlanMboxFd, &WlanMsg);
		if (ret == OS_ERR_NONE)
			return len;
	} 
	
	return 0;	
}



static int wlan_tcp_client(struct wlan_info *info)
{
        char buff[WLAN_BUFF_SIZE] = {0};
	int len = 0;
        int ret;
	int time = 0;	
	 	
	/* ģ���ʼ�� */
        ret = m590_init();
        if (ret < 0) {
                return -1;
        }

        ret = m590_config(&info->cfg);
        if (ret < 0) {
                return -1;
        }
      
	sprintf(info->tcp.severip, "%d.%d.%d.%d", ZDP.F3.IP1[0], ZDP.F3.IP1[1],
		ZDP.F3.IP1[2],ZDP.F3.IP1[3]);
	info->tcp.severport = ZDP.F3.Port1[1] * 0x100 + ZDP.F3.Port1[1];
	memcpy(info->tcp.apn, ZDP.F3.APN, 16);
	
	strcpy(info->tcp.severip, "122.228.19.57\0");
	info->tcp.severport = 12627;
	strcpy(info->tcp.apn, "CMNET\0");
	
	/* ����TCP���� */
        ret = m590_tcp_link(&info->tcp);
        if (ret < 0) {
                return -1;
        }
	
	info->link_status = 1;
	
        while (1) {                		
		/* �ӷ���������TCP���� */
                len = m590_read(buff, WLAN_BUFF_SIZE, 100);
       		if (len > 0) {			
			/* ���յ����������ݺ󣬳�ʱ���� */
                        time = 0;									
			/* �����յ������ݷ��͹�Լ����������ݽ��� */
			wlan_tcp_data_post(buff, len);			
			memset(buff, 0, WLAN_BUFF_SIZE);
		} else if (len == 0) {
            		/*
                	 * TCPû���յ����������ݺ󣬳�ʱ��ʱ�ۼ�
                	 * ��ʱ���ڶ�ֵ����ΪTCP��·�Ͽ��ˣ���Ҫ
                	 * ���·�������
                	 */
                	time++;
                	if (time > 1000) {
                	        time = 0;
                	        info->link_status = 0;
				return -1;
                	}		
		}  else {
			info->link_status = 0;
			return -1;
		}	
		
		/* ���ӹ�Լ������յ������ݲ���tcp���� */
		len = wlan_tcp_data_pend(buff, WLAN_BUFF_SIZE, 100);
		if (len > 0) {
			/* ����TCP���� */
               		ret = m590_write(buff, len);
			if (ret < 0) {
				info->link_status = 0;
               			return -1;
			}
               	}
        }
}


void App_Task_Wlan(void *p_arg)
{
        char mode = 0;
	
	wlan_init();
	
	for ( ;; ) {
                /* diff app mode */
		switch (mode) {
                case 0:
                        wlan_tcp_client(&wlan);
                        break;
                case 1:
                        //wlan_tcp_server(&wlan);
                        break;
                case 2:
                        //wlan_masege(&wlan);
                        break;			
                default:
                        break;
                }
		
		OSTimeDly(500);
        }
}



