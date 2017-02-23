

/* 
 * noway m590 
 * likejshy@126.com
 * 2017-01-01
 */


#ifndef  _HUAWEI_M590_H
#define  _HUAWEI_M590_H



/* ���� */
enum M590CMD {
    M590_COMINT,
    M590_POWERON,
    M590_POWEROFF,
    M590_TURNON,
    M590_TURNOFF,
    M590_RESET,
    M590_INIT,
    M590_ATTEST,
    M590_GETCSQ,
    M590_TCPLINK,
    M590_TCPTX,
    M590_TCPRX,
    M590_TCPCHK,
    M590_TCPCOM,
    M590_TCPCLOSD,
    M590_UARTTX,
    M590_UARTRX,
};


struct m590com {       /* M590ͨ�Žṹ */
    char *dmarxbuf;     /* ģ��AT����DMA���ջ��� */
    int dmarxbufsize;   /* ģ��AT����DMA���ջ����С */
};


struct m590info {      /* M590ģ���ʼ����Ϣ */
    char bandrate[7];   /* ͨѶ������ */
    char software_v[20];/* ģ������汾��*/
    char imei[16];      /* IMEI�� */
    char imsi[16];      /* SIM����IMSI��*/
    char simcpin[15];   /* SIM��PIN��Ȩ */
    char netreg[2];     /* ����ע��״�� */
    char opt[20];       /* ��Ӫ�� */
    char csq[1];        /* �ź�ֵ */
};


struct m590tcp {       /* M590ģ��TCP���Ӳ��� */
    char csq[1];        /* �ź�ֵ */
    char gprsstat[2];   /* ����GPRS����״̬ */
    char apn[17];       /* APN����� */
    char linkstat[2];   /* ģ��internet����״̬ */
    char localip[16];   /* ����IP */
    char severip[16];   /* �����IP */
    unsigned short severport;  /* �����port */
    char tcpstat[2];    /* ģ��TCP����״̬ */
    char linkflag;      /* ���ӱ�־*/
};


struct m590csq { 
    char csqval;        /* �ź�ֵ */
};


struct m590tcptx {     /* TCP�������ݽṹ */
    char *txbuf;        /* TCP���ݷ������ݻ��� */
    int txlen;          /* TCP���ݷ������ݳ��� */
};


struct m590tcprx {     /* TCP�������ݽṹ */
    char *rxbuf;        /* TCP�������ݻ��� */
    int rxsize;         /* ��Ҫ���ճ��� */
    int rxlen;          /* ʵ�ʽ��ճ��� */
};


struct m590tcpchk {    /* TCP��·��� */
    char linkflag;      /* ���ӱ�־*/
};


struct m590uartrx {    /* ���ڽ������ݽṹ */
    char *rxbuf;        /* ���ڽ������ݻ��� */
    int rxsize;         /* ���ڽ��ջ��������� */
    int rxlen;          /* ����ʵ�ʽ��ճ��� */
};


struct m590uarttx {    /* ���ڷ������ݽṹ */
    char *txbuf;        /* �������ݷ������ݻ��� */
    int txsize;         /* ���ڷ��ͻ��������� */
    int txlen;          /* �������ݷ������ݳ��� */
};


struct m590str
{
    char *dmarxbuf;             /* ����dam���ջ���������Ӧ�������� */
    int dmarxbufsize;           /* ����dam���ջ�������С����Ӧ�������� */
    struct m590tcpchk tcpchk;
    struct m590tcptx tcptx;
    struct m590tcprx tcprx;
    unsigned char mutex;    /* ģ�����Ȩ�ź��� */
    unsigned char prio;     /* ��ǰ���ڲ���ģ������� */
};


extern struct dev_ops m590_ops;
int m590_init(void);
int m590_config(struct m590info *info);
int m590_tcp_link(struct m590tcp *tcp);
int m590_read(char *rxbuf, int size, int ovt);
int m590_write(char *wbuf, int wlen);

#endif /* _HUAWEI_M590_H */



