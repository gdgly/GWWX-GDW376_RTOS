/******************************************************************************
    ��Ȩ���У�
    �� �� ��: 1.0
    �� �� ��: store.c
    ��������: 
    ��    �ߣ����
    ����˵�������ݴ洢
    ����˵����
    �޸ļ�¼��
*******************************************************************************/
#include <stdio.h>
#include <string.h>

//#include "w25q64.h"


int storage_write(unsigned long addr, char *data, int size)
{   
    //SPI_Flash_Write((unsigned char*)data, addr,(u16)size);    
    return 0;
}


int storage_read(unsigned long addr, char *data, int size)
{
    //SPI_Flash_Read((unsigned char*)data, addr, (u16)size);   
    return size;
}