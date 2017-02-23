/******************************************************************************
    ��Ȩ���У�
    �� �� ��: 1.0
    �� �� ��: dlt_104_port_comm.h
    ��������: 2016.09.20
    ��    �ߣ����
    ����˵����ʵ�֡�����Զ���ϵͳӦ��DL/T634.5104-2002ʵʩϸ�򡷵�ͨ�Žӿ�
    ����˵����
    �޸ļ�¼��
*******************************************************************************/
#ifndef _PTL_104_PORT_COMM_
#define _PTL_104_PORT_COMM_


extern int com_rx(int (*dlt_104_apdu_unpack)(unsigned char port, char *rxbuf, int len), unsigned char port, unsigned char prio);
extern int com_tx(int (*dlt_104_apdu_pack)(unsigned char port, char *txbuf), unsigned char port, unsigned char prio);
extern void dlt_104_receive(unsigned char port);
extern int dlt_104_com_init(unsigned char port);

#endif /* _PTL_104_PORT_COMM_ */
