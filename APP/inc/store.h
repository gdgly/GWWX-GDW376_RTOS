/******************************************************************************
    ��Ȩ���У�
    �� �� ��: 1.0
    �� �� ��: store.h
    ��������: 
    ��    �ߣ����
    ����˵�������ݴ洢
    ����˵����
    �޸ļ�¼��
*******************************************************************************/
#ifndef _STORE_
#define _STORE_


extern unsigned short crc16(char *buf, int len);
extern int storage_read(unsigned long add, char *data, int size);
extern int storage_write(unsigned long add, char *data, int size);
extern int store_param_save(char *name, char *data, unsigned char data_len);

#endif /* _STORE_ */
