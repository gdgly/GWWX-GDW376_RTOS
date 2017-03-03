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

#include "store.h"
//#include "lib.h"
#include "GDW376_MMU.h"
#include "printf.h"


#define CFG_DEBUG

#ifdef CFG_DEBUG
#define print(fmt,args...) kprintf(fmt, ##args)
#else
#define print(fmt,args...)
#endif



/* ���������ַ */
#define STA_ADDR_STORE_PARAM   ((DLT104_PARAM_START) * PAGESIZE) //0x0FF5D0
#define END_ADDR_STORE_PARAM   ((DLT104_PARAM_END) * PAGESIZE)   //0x100A70


#define PARAM_MAX_NUM          100
#define PARAM_MAX_SIZE         64


/* ������Ϣ */
struct stroe_param_map
{
    char *name;
    unsigned long addr;
    char size;
};

#pragma pack(1)
struct stroe_param
{
    char len;
    unsigned short cs;
    char data[PARAM_MAX_SIZE];    
};
#pragma pack()


/* �����洢ӳ��� */
const struct stroe_param_map param_map[PARAM_MAX_NUM] = 
{
    /* �ն˹��в�����Ϣ���ַ����� */
    {"0x8001",  0x0FF5D0L,  16}, /* �ն����� */
    {"0x8002",  0x0FF5E0L,  16}, /* �ն˲���ϵͳ */
    {"0x8003",  0x0FF5F0L,  16}, /* �ն������� */
    {"0x8004",  0x0FF600L,  16}, /* �ն�Ӳ���汾 */
    {"0x8005",  0x0FF610L,  16}, /* �ն�����汾 */
    {"0x8006",  0x0FF620L,  16}, /* �ն�����汾У���� */   
    {"0x8007",  0x0FF630L,  16}, /* �ն�ͨ�Ź�Լ���� */
    {"0x8008",  0x0FF640L,  16}, /* �ն˳����ͺ� */    
    {"0x8009",  0x0FF650L,  16}, /* �ն�ID�� */  
    {"0x800A",  0x0FF660L,  16}, /* �ն�����MAC��ַ */  
    
     /* �ն����в�����Ϣ���ַ */
    {"0x8026",  0x0FF670L,  16}, /* PTһ�ζ */
    {"0x8027",  0x0FF680L,  16}, /* PT���ζ */  

     /* �ն˶�����ֵ��Ϣ���ַ */
    {"0x8824",  0x0FF690L,  16}, /* Xʱ�䶨ֵ */
    {"0x8825",  0x0FF6A0L,  16}, /* Yʱ�䶨ֵ */
    {"0x8826",  0x0FF6B0L,  16}, /* Cʱ�䶨ֵ */
    {"0x8827",  0x0FF6C0L,  16}, /* Sʱ�䶨ֵ */
};


/* 104��Լ���� */
struct dlt_param
{
	unsigned char tag;
	unsigned char len;
        unsigned char val[64];  
};



/* ������Ϣ */
struct param_default_tab
{
    char *name;
    unsigned char len;
    struct dlt_param data;
};



/* �����ĳ���Ĭ��ֵ */
const struct param_default_tab param_default[PARAM_MAX_NUM] = 
{
	{"0x8001",   7,  {4,  5,   "GYDLB"} },  	/* �ն����� */
	{"0x8002",   4,  {4,  7,   "uCOS-II"} },  	/* �ն˲���ϵͳ */
};


extern u16 Cal_CRC16(const u8* data, u32 size);



static int __store_param_save(char *name, char *data, char len)
{
    struct stroe_param param = {0}; 
    unsigned long addr;
    char size;
    int ret; 
    int i;
    
    if (len > PARAM_MAX_SIZE)
    {
        return -1;
    }
    
    for (i = 0; i < PARAM_MAX_NUM; i++)
    {
        if (strcmp(name, param_map[i].name) == 0)
        {
            size = param_map[i].size;
            addr = param_map[i].addr;           
            if (len > size)
            {
                return -1;
            } 
            
            param.len = len;
            memcpy(param.data, data, len);
            param.cs = Cal_CRC16((unsigned char *)param.data, len);
            
            len += 3; 
            ret = storage_write(addr, (char *)&param, len);
            if (ret < 0)
            {
                return -1;
            }
            
            return 0;
        }
    }
    
    return -1;
}


static int __store_param_read(char *name, char *data)
{
    struct stroe_param param = {0}; 
    unsigned long addr;
    unsigned short cs;
    char size;
    int len = 0; 
    int i;
    
    for (i = 0; i < PARAM_MAX_NUM; i++)
    {
        if (strcmp(name, param_map[i].name) == 0)
        {
            size = param_map[i].size;
            addr = param_map[i].addr;
            
            len = storage_read(addr, (char *)&param, size);
            if ((len > 0) && (len <= size))
            {
                cs = Cal_CRC16((unsigned char *)param.data, param.len); 
                if ((cs == param.cs) && (cs != 0))               
                {           
                    len = param.len;
                    memcpy(data, param.data,len); 
                    return len;
                }
            }
            
            break;
        }
    }
    
    return 0;
}



int store_param_save(char *name, char *data, unsigned char data_len)
{	
	int ret;
	char temp[64] = {0};
	unsigned char len;

	ret = __store_param_save(name, data, data_len);
	if  (ret < 0)
		return -1;

	len = __store_param_read(name, temp);
	if (len == data_len) {
		if (memcmp(data, temp, data_len) == 0)
			return 0;	
	}
	
	return -1;
}




static int store_param_default(char *name)
{
	unsigned char len;
	char *pdata;
   	int ret; 
   	int i;
        
    	for (i = 0; i < PARAM_MAX_NUM; i++) {
        	if (strcmp(name, param_default[i].name) == 0) {
			
			len = param_default[i].len;
			pdata = (char *)&param_default[i].data;
			
			ret = __store_param_save(name, pdata, len);
			if (ret == 0)
				return 0;
		}
        }
    
    	return -1;
}



///* ��ȡ�ն˵����ò��� */
//int store_param_config(char *name, char *data)
//{
//	unsigned char len;
//	char *pdata;
//   	int i;
//        
//    	for (i = 0; i < PARAM_MAX_NUM; i++) {
//        	if (strcmp(name, param_config[i].name) == 0) {
//			
//			len = param_config[i].len;
//			pdata = (char *)param_config[i].data;
//			
//			memcpy(data, pdata, len);
//			return len;
//		}
//        }
//    
//    	return 0;
//}



int store_param_read(char *name, char *data)
{
	int len = 0;
	int ret;
	
	len = __store_param_read(name, data);
	if (len == 0) {
		ret = store_param_default(name);
		if (ret == 0)
			len = __store_param_read(name, data);	
	}

	return len;
}



//int test(void)
//{
//	char data[20] = {0};
//	
//	print("STA_ADDR_STORE_PARAM = %X\r\n",STA_ADDR_STORE_PARAM);
//	print("STA_ADDR_STORE_PARAM = %X\r\n",END_ADDR_STORE_PARAM);
//	
//	store_param_save("server_ip", "192.168.1.1", 13);
//	store_param_read("server_ip", data);
//	
//}

