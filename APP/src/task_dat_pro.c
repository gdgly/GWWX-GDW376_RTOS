/*
 * task_dat_pro.c
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


int data_pro_250ms_come_up(unsigned short time_ms)
{
	if (time_ms % 250 == 0)
		return 1;
	return 0;
}


int data_pro_sec_come_up(unsigned short time_ms)
{
	if (time_ms % 1000 == 0)
		return 1;
	return 0;
}


int data_pro_min_come_up(unsigned short time_ms)
{
	if (time_ms % 60000 == 0)
		return 1;
	return 0;
}



void App_Task_DatPro (void *p_arg)
{
	unsigned short time_ms = 0;
	int ret;
	
	while (1) {				
		
		/* ����ն˲������� */
		ZD_CSSAVE_CHK();
		
		/* ÿ250���봦��һ�ε����� */
		ret = data_pro_250ms_come_up(time_ms);
		if (ret == 1) {
		
			/* ��ʱ�Ӵ��� */
			ClockTimer();     	
		}

		/* ÿ�봦��һ�ε����� */
		ret = data_pro_sec_come_up(time_ms); 	
		if (ret == 1) {
		  	Watchdog();   	
		}

		/* ÿ���Ӵ���һ�ε����� */
		ret = data_pro_min_come_up(time_ms); 
		if (ret == 1) {   
			ZDPN_MXT();   //;���յ�����ֵ�뷢��ʱ�����
			ZDPN_YXT();   //;���յ���Խ��ʱ�����
		}
		
		if (++time_ms > 60000)
			time_ms = 0;

		OSTimeDlyHMSM(0, 0, 0, 1);
	}
}
