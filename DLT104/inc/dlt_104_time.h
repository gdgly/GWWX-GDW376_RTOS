/******************************************************************************
    ��Ȩ���У�
    �� �� ��: 1.0
    �� �� ��: dlt_104_time.h
    ��������: 
    ��    �ߣ����
    ����˵����ʵ�֡�����Զ���ϵͳӦ��DL/T634.5104-2009ʵʩϸ��
    ����˵����
    �޸ļ�¼��
*******************************************************************************/
#ifndef _DLT_104_TIME_
#define _DLT_104_TIME_


extern int time_syn_process(unsigned char port);
extern int time_read_process(unsigned char port);
extern int time_process_init(unsigned char port);


#endif /* _DLT_104_TIME_ */
