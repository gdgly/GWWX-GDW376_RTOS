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

#endif /* _STORE_ */
