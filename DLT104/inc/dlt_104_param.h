/******************************************************************************
    ��Ȩ���У�
    �� �� ��: 1.0
    �� �� ��: dlt_104_param.h
    ��������: 
    ��    �ߣ����
    ����˵����ʵ�֡�����Զ���ϵͳӦ��DL/T634.5104-2009ʵʩϸ�򡷵�װ�ز���
    ����˵��������104��Լ����վ�ķ�ʽ�������վ����װ�ز���
    �޸ļ�¼��
*******************************************************************************/
#ifndef _DLT_104_PARAM_
#define _DLT_104_PARAM_

int param_fixed_area_read_process(unsigned char port);

int param_read_process(unsigned char port);

int param_fixed_area_switch_process(unsigned char port);

extern int param_set_process(unsigned char port);

extern int param_process_init(unsigned char port);

#endif /* _DLT_104_PARAM_ */
