/*
 * task_freeze.c
 * likejshy@126.com
 * 2017-01-08
 */

#include <string.h>
#include <stdio.h>
#include <ucos_ii.h>
#include "BSP_CFG.h"
#include "Serial.h"
#include "GDW376_CFG.h"
#include "printf.h"


#define CFG_DEBUG

#ifdef CFG_DEBUG
#define print(fmt,args...) kprintf(fmt, ##args)
#else
#define print(fmt,args...)
#endif


   u8 Sec;
    u8 Min;
    u8 Hour;
    u8 Day;
    u8 Month;
    u8 Year;

/* ��ǰϵͳʱ��ķֻ�ȡ */
static int freeze_get_min_time(unsigned char *minute)
{	
	if ((THEX.Year == 0) || (THEX.Month == 0)|| (THEX.Day == 0))
		return 0;
	
	*minute = THEX.Min;
	
	return 1;
}


/* ��ȡ��Ҫ����Ĳ��������� */
static int freeze_get_pn_num(unsigned char *pn_num)
{
	*pn_num = PNMAXNUM;
	return 0;
}



/* �ж�ϵͳ���߶����ʱ���Ƿ��� */
static int freeze_curve_time_up(unsigned char minute)
{
	static unsigned old_minute = 0xFF;

	/* ÿ����ִ��һ�� */
	if (minute != old_minute) {
		old_minute = minute;

		/* ��ǰ����Ϊ15�ı���ʱΪ���߶���ʱ�䵽��
		 * ����ÿСʱ��:0��, 15�֣�30�֣�45�ֽ���
		 * ���߶���
		 */
		if (minute % 15 == 0)
			return 1;
	}

	return 0;
}



/* ���߶������������ */
static int freeze_curve_data_save(unsigned char pn)
{
	CDJ_Save(pn, &TBCD.Min); //;���߶���
	HDJ_Program(pn); //;Сʱ����
	HDJ_Save(pn); //;Сʱ���ᱣ��
	MVT_Save(pn); //;����һ��ͳ��ʱ��
	PEZD_Save(pn, &TBCD.Day);
	return 0;
}



static int freeze_curve_data(void)
{
	unsigned char minute;
	unsigned char num, i;
	int ret;

	/* ��ǰϵͳʱ��ķֻ�ȡ */
	ret = freeze_get_min_time(&minute);
	if (ret == 0)
		return 0;

	/* ��ȡ��Ҫ����Ĳ��������� */
	ret = freeze_get_pn_num(&num);
	if (ret < 0)
		return -1;

	/* �ж�ϵͳ���߶����ʱ���Ƿ��� */
	ret = freeze_curve_time_up(minute);
	if (ret == 1) {

		/* �����еĲ�����������߶��� */
		for (i = 0; i < num; i++) {

			/* ���߶������������ */
			print("freeze_curve_data_save pn = %d\r\n", i + 1);
			ret = freeze_curve_data_save(i);
			if (ret < 0) {
				print("freeze_curve_data_save error!\r\n");
				return -1;
			}
		}
	}

	return 0;
}


/* ��ǰϵͳʱ��ķ֣�ʱ��ȡ */
static int freeze_day_get_time(unsigned char *minute, unsigned char *hour)
{
	if ((THEX.Year == 0) || (THEX.Month == 0)|| (THEX.Day == 0))
		return 0;	
	
	*minute = THEX.Min;
	*hour = THEX.Hour;
	
	return 1;
}


/* �ж��ն���ʱ���Ƿ��� */
static int freeze_day_time_up(unsigned char minute, unsigned char hour)
{
	static unsigned old_minute = 0xFF;

	/* ÿ�����ж�һ�� */
	if (minute != old_minute) {
		old_minute = minute;

		/* ÿ�����ʱΪ�ն���ʱ�䵽�� */
		if ((minute == 0) && (hour == 0))
			return 1;
	}

	return 0;
}


/* �ն������������ */
static int freeze_day_data_save(unsigned char pn)
{
	u8 Date[3];

	Day_Last(&TBCD.Day, Date);//;�������һ�յ�����
	DDJ_Save(pn,  Date);	  //;�ն���
	MVTD_Res(pn);		  //;��ͳ������;
	return 0;
}


/* �ն������� */
static int freeze_day_data(void)
{
	unsigned char minute, hour;
	unsigned char num, i;
	int ret;

	/* ��ǰϵͳʱ��ķ֣�ʱ��ȡ */
	ret = freeze_day_get_time(&minute, &hour);
	if (ret == 0)
		return 0;

	/* ��ȡ��Ҫ����Ĳ��������� */
	ret = freeze_get_pn_num(&num);
	if (ret < 0)
		return -1;

	/* �ж��ն���ʱ���Ƿ��� */
	ret = freeze_day_time_up(minute, hour);
	if (ret == 1) {

		/* �����еĲ���������ն��� */
		for (i = 0; i < num; i++) {

			/* �ն������������ */
			print("freeze_day_data_save pn = %d\r\n", i + 1);
			ret = freeze_day_data_save(i);
			if (ret < 0) {
				print("freeze_day_data_save pn = %d error!\r\n", i + 1);
				return -1;
			}
		}
	}

	return 0;
}


/* ��ǰϵͳʱ��ķ֣�ʱ���ջ�ȡ */
static int freeze_month_get_time(unsigned char *minute, unsigned char *hour,
			  unsigned char *day)
{
	if ((THEX.Year == 0) || (THEX.Month == 0)|| (THEX.Day == 0))
		return 0;
	
	*minute = THEX.Min;
	*hour = THEX.Hour;
	*day = THEX.Day;
	
	return 1;
}


/* �ж��¶���ʱ���Ƿ��� */
static int freeze_month_time_up(unsigned char minute, unsigned char hour,
			 unsigned char day)
{
	static unsigned old_minute = 0xFF;

	/* ÿ�����ж�һ�� */
	if (minute != old_minute) {
		old_minute = minute;

		/* ÿ���³����������ݶ��� */
		if (day == 1 && hour == 0 && minute == 0)
			return 1;
	}

	return 0;
}


/* �¶������� */
static int freeze_month_data_save(unsigned char pn)
{
	u8 Date[2];

	Month_Last(&TBCD.Month, Date);//;�������һ�µ��·�
	MDJ_Save(pn, Date); //;�¶���
	MVTM_Res(pn);  //;��ͳ������
	return 0;
}


/* �¶������ݴ��� */
static int freeze_month_data(void)
{
	unsigned char minute, hour, month;
	unsigned char num, i;
	int ret;

	/* ��ȡ��ǰϵͳʱ��ķ֣�ʱ���� */
	ret = freeze_month_get_time(&minute, &hour, &month);
	if (ret == 0)
		return 0;

	/* ��ȡ��Ҫ����Ĳ��������� */
	ret = freeze_get_pn_num(&num);
	if (ret < 0)
		return -1;

	/* ��ǰ�¶����ʱ���Ƿ����� */
	ret = freeze_month_time_up(minute, hour, month);
	if (ret == 1) {

		/* �����еĲ��������ݽ��ж��� */
		for (i = 0; i < num; i++) {

			/* ���������� */
			print("freeze_month_data_save pn = %d\r\n", i + 1);
			ret = freeze_month_data_save(i);
			if (ret < 0) {
				print("freeze_month_data_save pn = %d error\r\n", i + 1);
				return -1;
			}
		}
	}

	return 0;
}


/* ��ȡϵͳ��ǰʱ���BCD�룬���ڳ����յ��ж� */
static int freeze_coller_day_get_time(unsigned char *minute, unsigned char *hour,
			       unsigned char *day)
{
	if ((THEX.Year == 0) || (THEX.Month == 0)|| (THEX.Day == 0))
		return 0;
	
	*minute = TBCD.Min;
	*hour = TBCD.Hour;
	*day = TBCD.Day;
	
	return 1;
}


/* �жϳ������Ƿ��� */
static int freeze_coller_day_time_up(unsigned char minute, unsigned char hour,
			      unsigned char day)
{
	static unsigned old_minute = 0xFF;
	u32 ch1 = 0, ch2 = 1;
	u8 i, Hour, Min;
        //Day, 

	if (minute != old_minute) {
		old_minute = minute;

		Min  = ZDP.F33.PortP[0].RTime[0];
		Hour = ZDP.F33.PortP[0].RTime[1];

		/* ��ȡ��ǰϵͳ������*/
		//Day = TBCD.Day;

		/* ����������� */
		ch1 |= ZDP.F33.PortP[0].RDay[3];
		ch1 <<= 8;
		ch1 |= ZDP.F33.PortP[0].RDay[2];
		ch1 <<= 8;
		ch1 |= ZDP.F33.PortP[0].RDay[1];
		ch1 <<= 8;
		ch1 |= ZDP.F33.PortP[0].RDay[0];

		for(i = 1; i < 32; i++) {
			if(ch1 & ch2) {
				if (day == i
				    && hour == Hour
				    && minute == Min) {
					return 1;
				}
			}

			ch2 <<= 1;
		}
	}

	return 0;
}


/* ������������� */
static int freeze_coller_day_data_save(unsigned char pn)
{
	u8 Date[2];

	Month_Last(&TBCD.Month, Date);//;�������һ�µ��·�
	MDJ_Save(pn, Date);	    //;�¶���
	MVTM_Res(pn);		    //;��ͳ������
	return 0;
}



/* ���᳭�������� */
static int freeze_coller_day_data(void)
{
	unsigned char minute, hour, day;
	unsigned char num, i;
	int ret;
	
	/* ��ȡϵͳ��ǰʱ��ķ֣�ʱ���� */
	ret = freeze_coller_day_get_time(&minute, &hour, &day);
	if (ret == 0)
		return 0;

	/* ��ȡ���������� */
	ret = freeze_get_pn_num(&num);
	if (ret < 0)
		return -1;

	/* �������Ƿ��� */
	ret = freeze_coller_day_time_up(minute, hour, day);
	if (ret == 1) {

		/* �����еĲ��������ݽ��ж��� */
		for (i = 0; i < num; i++) {
			print("freeze_coller_day_data_save pn = %d\r\n", i + 1);
			ret = freeze_coller_day_data_save(i);
			if (ret < 0) {
				print("freeze_coller_day_data_save pn = %d error!\r\n", i + 1);
				return -1;
			}
		}
	}

	return 0;
}


/* �������������� */
void App_Task_Freeze (void *p_arg)
{
	int ret;

	while (1) {
		/* ������������ */
		ret = freeze_curve_data();
		if (ret < 0) {
			print("freeze_curve_data error!\r\n");
		}

		/* ���������� */
		ret = freeze_day_data();
		if (ret < 0) {
			print("freeze_day_data error!\r\n");
		}

		/* ���������� */
		ret = freeze_month_data();
		if (ret < 0) {
			print("freeze_month_data error!\r\n");
		}

		/* ���᳭�������� */
		ret = freeze_coller_day_data();
		if (ret < 0) {
			print("freeze_coller_meter_day_data error!\r\n");
		}

		OSTimeDlyHMSM(0, 0, 1, 0);
	}
}


