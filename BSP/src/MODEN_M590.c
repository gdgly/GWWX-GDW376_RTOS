/*
********************************************************************************
*  �� �� ����MODEN_M590.C
*
*  �ļ��������з�GPRSģ��M590��������  
*
*  ����оƬ��
*
*  �� �� �ˣ�
*
*  �� �� �ţ�
*
*  �޸ļ�¼��1.����ģ����Ž��չ��ܣ��������ѱ�����վ�����ӣ����Ż���������:
*               LINKTOZHGC*IP,�˿ں�*;����:LINKTOZHGC*112.091.151.002,7011*
*            2.�����ڱ�������ģʽ�¶��ż�����վ�����ӡ����ż������
*               *LINKTOMASTER* 2011-09-16 16:49:11
*            3 ������ƬM590 R2   20140704
*              ���Ŀ���ʱ�򣬵͵�ƽ��ʱ2.4S
*              +CPIN ָ��ɾ��ok ֮ǰ��\r\n 
*              +CREG ָ��ɾ��ok ֮ǰ��\r\n   20140704
* 
********************************************************************************
*/
#include "BSP_CFG.h"
#include "GDW376_CFG.h"
//;extern struct ZDStatStr ZDS;      //;�ն�״̬����

/*----------------------------------------------------------------------------*
 |  ��ģ���Դ
 *----------------------------------------------------------------------------*/
void Modem_PowON(void)
{
    _ModemPowerOn;
    _ModemPowerOn;
    _ModemPowerOn;
}


/*----------------------------------------------------------------------------*
 |  �ر�ģ���Դ
 *----------------------------------------------------------------------------*/
void Modem_PowOFF(void)
{
    _ModemPowerOff;
    _ModemPowerOff;
    _ModemPowerOff;
}

/*----------------------------------------------------------------------------*
 |   ��ģ�������
 *----------------------------------------------------------------------------*/
void Modem_GDCDON(void)
{
    LOGONLIGHTON;
    LOGONLIGHTON;
}


/*----------------------------------------------------------------------------*
 |   �ر�ģ�������
 *----------------------------------------------------------------------------*/
void Modem_GDCDOFF(void)
{
    LOGONLIGHTOFF;
    LOGONLIGHTOFF;
}


/*----------------------------------------------------------------------------*
 |  ����ģ��
 *----------------------------------------------------------------------------*/
void Modem_ON(void)
{
    if(_GPRS_Modem_Run == _Modem_OFF)
    {
        /*
        _ModemONOFF_High;                                                                                                       //;��ʹģ��ON/OFF���ű�������200ms�ĸߵ�ƽ
        Delay_nms(220);
        _ModemONOFF_Low;                                                                                                        //;M590Ҫ��������300ms
        Delay_nms(500);
        _ModemONOFF_High;  
        */   
        _ModemONOFF_Low;  //;20140704                                                                                                       //;M590Ҫ��������300ms
        Delay_nms(220);
        Watchdog();
        Delay_nms(220);
        Watchdog();
        Delay_nms(220);
        Watchdog();
        Delay_nms(220);
        Watchdog();
        Delay_nms(220);
        _ModemONOFF_High;                                                                                                  //;��ģ���ON/OFF�ź�Ϊ��
    }
}

/*----------------------------------------------------------------------------*
 |  ���ģ���Ƿ������ɹ�
 *----------------------------------------------------------------------------*/
void Modem_ON_CHK(void)
{
    static u8 On_Off_Sec;
/*    
    if(!Modem_OFF_CHK())     //;���ģ������
    {
        if(GprsLC.LinkEnable == true)
        {
            On_Off_Sec = 0;
            if (!(GprsC.State & _SetUp))
            {
                GprsC.State = _SetUp;                                                                            //;д��ģ��������־
            }
        }
        else
        {
            On_Off_Sec = 0x00;
            if(!((GprsC.SendAT_Step == 20) || (GprsC.SendAT_Step == 99)))
            {                  
                Modem_OFF();//;�ر�ģ���Դ
            }
        }
    }
 */
 /*���ʱ������ģ���Ƿ���Ҫ�ر�20130613  zrt*/
     if(!Modem_OFF_CHK())     //;���ģ������
    {
        if(GprsLC.LinkEnable == true)
        {
            On_Off_Sec = 0;
            if (!(GprsC.State & _SetUp))
            {
                GprsC.State = _SetUp;                                                                            //;д��ģ��������־
            }
        }
        else
        {
            On_Off_Sec = 0x00;
            if (GprsC.State & _HadLogin)  //;�Ѿ���¼
            {
                GprsC.ReConnect = true;
                
            }
            else if((!GprsGyC.Status) &&(!GprsC.ReConnect)) //;�Ѿ��Ͽ����Ӻ������걨��
            {
            	if(!((GprsC.SendAT_Step == 20) || (GprsC.SendAT_Step == 99)))
                {                  
                    Modem_OFF();//;�ر�ģ���Դ
                }
            }
        }
    }
    else
    {
        if(GprsLC.LinkEnable == true)
        {
            On_Off_Sec++;                                                        //;ģ��ػ���ʱ
            if(On_Off_Sec == 2)
            {
                ByteN_Fill(sizeof(struct ModemContrl), (u8*)&GprsC, 0x00);
            }
            else if(On_Off_Sec == 6)
            {
                Modem_PowOFF();               //;10���ر�ģ���Դ
            }
            else if(On_Off_Sec == 12)
            {
                Modem_PowON();               //;20����ģ���Դ
            }
            else if(On_Off_Sec == 18)         //;30���ģ�鿪��
            {
                On_Off_Sec = 0;
                Modem_ON();
            }
        }
    }
}


/* 
********************************************************************************
* �� �� �������ģ������
* ��    �ܣ�
* ��    ����true--�رա�false--����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��2011-06-09 09:51:50
********************************************************************************
*/
u8 Modem_OFF_CHK(void)
{
    if(_GPRS_Modem_Run == _Modem_OFF)  //;���ģ��δ����
    {
        Delay_nms(20);
        if (_GPRS_Modem_Run == _Modem_OFF)
        {
            return true;
        }
    }
    if(_GPRS_Modem_Run == _Modem_ON)
    {
        Delay_nms(20);
        if (_GPRS_Modem_Run == _Modem_ON)
        {
            return false;
        }
    }
    return false;
}


/*----------------------------------------------------------------------------*
 |  ǿ�ƹر�ģ��
 *----------------------------------------------------------------------------*/
void Modem_OFF(void)
{
    if (_GPRS_Modem_Run == _Modem_ON)                                                                                       //;���ģ������
    {
        GprsC.ATStatus = 'E';
        GprsC.SendAT_Wait1s = 2;
        GprsC.SendAT_Step = 20;
        Modem_GDCDOFF(); //;�ر� �����
        GprsC.Status = 0x00;
        GprsC.State = 0x00;
        GprsC.Sign = 0x00;
    }
    else
    {
        Modem_PowOFF();
    }
}


/*----------------------------------------------------------------------------*
 |  ǿ������ģ��
 *----------------------------------------------------------------------------*/
void Modem_OFFON(void)
{
    Modem_OFF();
    Modem_ON();
}


/*----------------------------------------------------------------------------*
 |  ��ģ�鷢��AT����
 *----------------------------------------------------------------------------*/
void Modem_AT_Send(char *str1)
{
    u8 i;

    if (GPRSUSARTCTR.SedOrd == idle) //;��ǰ����û�����ڷ�����
    {
        GprsC.ATStatus = 'S';
        GprsC.ATLen = strlen(str1);
        for (i = 0; i < GprsC.ATLen; i++)
        {
            GprsC.ATStr[i] = *(str1 + i);
        }
    
        for (; i < 50; i++)
        {
            GprsC.ATStr[i] = 0;
        }
    
        GPRSUSARTCTR.SedOrd = 'E';
        GPRSUSARTCTR.BufAdr = GprsC.ATStr; //;ָ��Ҫ���͵����ݻ���
        GPRSUSARTCTR.TLong = GprsC.ATLen;  //;Ҫ���͵ĳ���
    }
}


/*----------------------------------------------------------------------------*
 |  ��������ͨ��TCPЭ�鷢�ͳ�ȥ
 *----------------------------------------------------------------------------*/
u8 Modem_TCP_Send(void)
{
    if(GprsC.ATStatus == idle)
    {
        if(GprsC.LinkState == LINK0)
        {
            if(GprsC.State & _Connect)
            {
                GprsC.ATStatus = 'E';
                GprsC.SendAT_Wait1s = 1;
                GprsC.SendAT_Step = 21;
            }
        }
        else if(GprsC.LinkState == LINK1)
        {
            if(GprsC.State1 & _Connect)
            {
                GprsC.ATStatus = 'E';
                GprsC.SendAT_Wait1s = 1;
                GprsC.SendAT_Step = 21;
            }
        }
    }

    if(GprsC.ATStatus == 'T') //;����������׼������
    {
        if (GPRSUSARTCTR.SedOrd == 0)
        {
            GPRSUSARTCTR.SedOrd = 'E';
            GprsC.ATStatus = 'S';
            GPRSUSARTCTR.TLong = GprsC.TCPLen;//;д��TCPҪ�������ݵĳ���
            GPRSSEDBUF[GPRSUSARTCTR.TLong] = 0x0d; //;Ϊ�������ݺ�ӻس�
            GPRSUSARTCTR.TLong++; //;���ͳ���
            GPRSUSARTCTR.BufAdr = GPRSSEDBUF;  //;���͵Ļ����ַ
        }
    }

    if (GprsC.ATStatus == 'O')
    {
        GprsC.ATStatus = idle;
        if(GprsC.LinkState == LINK1)
        {
            if(GprsC.State1 & _HadLogin)
            {
                GprsC.LinkState = LINK0;
            }
        }
        return 0xFF; //;������� 
    }
    else if(GprsC.TCPFail == true) //;����ʧ��
    {
        GprsC.ATStatus = idle;
        GprsC.TCPFail = false; 
        if(GprsC.LinkState == LINK1)
        {
            if(GprsC.State1 & _HadLogin)
            {
                GprsC.LinkState = LINK0;
            }
        }
        return 0xDF;      
    }
    return 0xEF;
}

/*----------------------------------------------------------------------------*
 |  ��������ͨ�����ŷ��ͳ�ȥ
 *----------------------------------------------------------------------------*/
u8 Modem_SMS_Send(void)
{
    if (GprsC.ATStatus == idle)
    {
        if (GprsC.SMSPer == true)
        {
            GprsC.ATStatus = 'E';
            GprsC.SendAT_Wait1s = 2;
            GprsC.SendAT_Step = 22;
        }
    }

    if(GprsC.ATStatus == 'M')
    {
        if(GPRSUSARTCTR.SedOrd == 0)
        {
            GPRSUSARTCTR.SedOrd = 'E';
            GprsC.ATStatus = 'S'; 
            GPRSUSARTCTR.TLong = GprsC.SMSLen; //;���ͳ���
            GPRSUSARTCTR.BufAdr = GPRSSEDBUF;  //;���͵Ļ����ַ
        }
    }

    if (GprsC.ATStatus == 'O')
    {
        GprsC.ATStatus = idle;
        return true; //;�������
    }

    return false;
}


/*----------------------------------------------------------------------------*
 |  �յ����ŵ�ATָ�n+CMT:
 *----------------------------------------------------------------------------*/
void Modem_SMS_Rece(void)
{  
    char *string;
    char Order[10];
    u16 i;
    
    string = strstr(GPRSUSARTCTR.BufAdr, "L"); //;Ѱ���״γ���L��λ��
    if(string == NULL) return;     
    for (i = 0; i < 10; i++)
    {
        Order[i] = *(string + i); //;�õ�������
    }
    
    if(strcmp(Order, "LINKTOZHGC") == 0)
    {
        GprsLC.Link2Switch = true; //;��������վ������
        GprsLC.IP1_long = 0;
        
        string = strstr(GPRSUSARTCTR.BufAdr, "*"); //;Ѱ���״γ���*��λ��
        if(string == NULL) return;
        for (i = 0; i < 21; i++)   //;*IP,�˿ں�*  ������*���м�����λ��Ϊ21λ
        {
            if (*(string + i + 1) == 0x2A)  //;0x2A���ַ�*��ASCII��
            {
                break; //;ĩβ����*�˳�
            }
    
            GprsLC.IP1[i] = *(string + i + 1);
            GprsLC.IP1_long++;
        }
    }     
    else if(strcmp(Order, "*LINKTOMASTER*") == 0)   //;�����������
    {
        if((ZDP.F8.WorkMode&0x03) == 0x02) //;��������ģʽ
        {
            GprsLC.PassLinkOrd = true;
        }
    } 
}

/*----------------------------------------------------------------------------*
 |  ��������AT���鿴ģ���Ƿ��뵥Ƭ��ͨѶ����
 *----------------------------------------------------------------------------*/
void AT01Comd_AT(void)
{
    if(GprsC.ATStatus == 'E')   
    {
        Modem_AT_Send("AT\r\0"); //;����Send
    }   
    else if(strstr(GprsC.ATStr, "AT\r\r\nOK\r\n") != NULL)                                          //;�յ�ATOK��Ϊ����������
    {
        GprsC.ATStatus = 'E'; 
        GprsC.SendAT_Wait1s = 1;
        GprsC.SendAT_Step = 3;                                              //;��ǰ�����ʿ�������ͨ�Ų�����Ҫ����
    }
    else if(strstr(GprsC.ATStr, "\r\nOK\r\n") != NULL)                                          //;�յ�ATOK��Ϊ����������
    {
        GprsC.ATStatus = 'E'; 
        GprsC.SendAT_Wait1s = 1;
        GprsC.SendAT_Step = 3;                                              //;��ǰ�����ʿ�������ͨ�Ų�����Ҫ����
    } 
}


/*----------------------------------------------------------------------------*
 |  ����ģ��Ĳ�����Ϊ9600
 *----------------------------------------------------------------------------*/
void AT02Comd_IPR(void)
{
    if(GprsC.ATStatus == 'E')   
    {
        Modem_AT_Send("AT+IPR=9600\r\0");
    }      
    else if(strstr(GprsC.ATStr, "AT+IPR=9600\r") != NULL)
    {
        ;                                          //;��������                                                                  //;������������ӵ��ڶ����������óɹ�
    }
    else if(strstr(GprsC.ATStr, "\r\nOK\r\n") != NULL)                    //;�������޸ĳɹ�
    {
        GprsC.ATStatus = 'E'; 
        GprsC.SendAT_Wait1s = 2;
        GprsC.SendAT_Step = 1;
        USART2_BaudRateSet(9600);                                         //;�������ģ��ͨ�ŵĴ��ڲ�����
    }
}


/*----------------------------------------------------------------------------*
 |  ����ģ��Ļ���
 *----------------------------------------------------------------------------*/
void AT03Comd_ATE0(void)
{
    if(GprsC.ATStatus == 'E')   
    {
        Modem_AT_Send("ATE0\r\0");
    }   
    else if(strstr(GprsC.ATStr, "ATE0\r\r\nOK\r\n") != NULL)
    {
        GprsC.ATStatus = 'E';
        GprsC.SendAT_Wait1s = 1;
        GprsC.SendAT_Step = 4;
    }
    else if(strstr(GprsC.ATStr, "\r\nOK\r\n") != NULL)
    {
        GprsC.ATStatus = 'E';
        GprsC.SendAT_Wait1s = 1;
        GprsC.SendAT_Step = 4;
    }
}



/*----------------------------------------------------------------------------*
 |  ���SIM���Ƿ���λ
 *----------------------------------------------------------------------------*/
void AT04Comd_CPIN(void)
{
    if(GprsC.ATStatus == 'E')   
    {
        Modem_AT_Send("AT+CPIN?\r\0");
    }       
    /*else if(strstr(GprsC.ATStr, "\r\n+CPIN: READY\r\n\r\nOK\r\n") != NULL)     //;��sim�����Ҳ�������PIN��*/
    else if(strstr(GprsC.ATStr, "\r\n+CPIN: READY\r\nOK\r\n") != NULL)     //;��sim�����Ҳ�������PIN�� 20140704
    {
        GprsC.ATStatus = 'E';
        GprsC.SendAT_Wait1s = 1;
        GprsC.SendAT_Step = 5;         //;��һ�����ģ���ź�
        GprsC.Status &= (~_NoCard);
        GprsC.CPINErrNum = 0;
    }
    else if(strstr(GprsC.ATStr, "\r\nERROR\r\n") != NULL)                 //;SIM������
    {
        GprsC.ATStatus = 'E';
        GprsC.CPINErrNum++;
        if(GprsC.CPINErrNum < 30)
        {
            GprsC.SendAT_Wait1s = 2;
            GprsC.SendAT_Step = 4;
            GprsC.Status = _NoCard;
            Modem_GDCDOFF();      //;�ص������
            GprsC.State = 0;   
            GprsC.Sign = 0;
        }
        else
        {   
            GprsC.CPINErrNum = 0;
            Modem_OFF();     
        }
    }
}


/* 
********************************************************************************
* �� �� ����AT05Comd_CSQ
* ��    �ܣ����ģ���ź�CSQ
* ��    ����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��2012-01-31 14:46:49 �����ز�״̬�Ĵ���
********************************************************************************
*/
void AT05Comd_CSQ(void)
{
    u8 ch1, ch2;
    
    if(GprsC.ATStatus == 'E')   
    {
        Modem_AT_Send("AT+CSQ\r\0");  //;���źŵ�ǿ��
    }       
    else if(strstr(GprsC.ATStr, "\r\n+CSQ: ") != NULL)
    {  
         //;NEW
        GprsC.SignR = 'R'; //;��ȡ�źųɹ�
        GprsC.ATStatus = 'E';
        ch1 = *(GprsC.ATStr + 8);
        ch2 = ch1 - 0x30;
        ch1 = *(GprsC.ATStr + 9);
        if (ch1 == ',')
        {
            ch1 = ch2;
        }
        else if ((ch1 >= 0x30) && (ch1 <= 0x39))
        {
            ch1 -= 0x30;
            ch1 += (ch2 * 10);
        }
        else
        {
            ch1 = ch2;
        }
        if (ch1 == 99)
        {
            ch1 = 0;
        }

        GprsC.Sign = ch1;

        if(GprsC.State & _Link)          //;��ǰ״̬Ϊ��������(Link)
        {
            //if((GprsC.Sign > 10) && (GprsC.Sign <= 31))                    //;�ź�ǿ���ڴ���10��С��31ʱ�ſ��������շ�����
            if((GprsC.Sign > 1) && (GprsC.Sign <= 35))  //;qiu20140416
            {
                GprsC.SendAT_Wait1s = 1;
                GprsC.SendAT_Step = 6;
                GprsC.CSQErrNum = 0;
            }
            else
            {
                GprsC.CSQErrNum++;
                if (GprsC.CSQErrNum < 30)
                {
                    GprsC.SendAT_Wait1s = 2;
                    GprsC.SendAT_Step = 5;
                }
                else
                {
                    GprsC.SendAT_Wait1s = 2;
                    GprsC.SendAT_Step = 20;                                  //;����ģ��
                    GprsC.CSQErrNum = 0;
                }
            }
        }
        else       //;��ǰ����״̬
        {
            //if((GprsC.Sign > 10) && (GprsC.Sign <= 31))
            if((GprsC.Sign > 1) && (GprsC.Sign <= 35))  //;qiu20140416
            {                 
                /*if(GprsC.State & _Connect)  
                {      
                    GprsC.SendAT_Wait1s = 1;       
                    GprsC.SendAT_Step = 15;       //;�鿴��·1
                }
                else //;�ز�״̬
                {
                    GprsC.ATStatus = 'R';
                }
                */
                GprsC.ATStatus = 'R';
            }
            else
            {
                GprsC.CSQErrNum++;
                if (GprsC.CSQErrNum < 30)
                {
                    GprsC.SendAT_Wait1s = 2;
                    GprsC.SendAT_Step = 5;
                }
                else
                {
                    Modem_GDCDOFF();                       //;�ص������
                    GprsC.State = 0;                       //;�����Ͽ����ӳɹ�                
                    GprsC.Status = 0;
                    GprsC.SendAT_Wait1s = 2;
                    GprsC.SendAT_Step = 20;                //;����ģ��
                    GprsC.CSQErrNum = 0;
                }
            }
        }
    }
}


/*----------------------------------------------------------------------------*
 |  ���ģ���Ƿ�ע��CREG
 *----------------------------------------------------------------------------*/
void AT06Comd_CREG(void)
{
    if(GprsC.ATStatus == 'E')  
    {
        Modem_AT_Send("at+creg?\r\0");
    }   
    /*else if((strstr(GprsC.ATStr, "\r\n+CREG: 0,1\r\n\r\nOK\r\n")!= NULL) ||
             (strstr(GprsC.ATStr,"\r\n+CREG: 0,5\r\n\r\nOK\r\n") != NULL))      //;�Ѿ�ע��Ϊ����״̬�����ε�״̬
    */
    else if((strstr(GprsC.ATStr, "\r\n+CREG: 0,1\r\nOK\r\n")!= NULL) ||
             (strstr(GprsC.ATStr,"\r\n+CREG: 0,5\r\nOK\r\n") != NULL))      //;�Ѿ�ע��Ϊ����״̬�����ε�״̬ 20140704
    {
        GprsC.ATStatus = 'E';
        GprsC.SendAT_Wait1s = 1;
        GprsC.SendAT_Step = 7;
        GprsC.Status |= _FindHost;                                           //;����Ѱ����վ
        GprsC.CREGErrNum = 0;
    }
    /*
    else if((strstr(GprsC.ATStr,"\r\n+CREG: 0,0\r\n\r\nOK\r\n") != NULL) ||
             (strstr(GprsC.ATStr,"\r\n+CREG: 0,2\r\n\r\nOK\r\n") != NULL) ||
             (strstr(GprsC.ATStr,"\r\n+CREG: 0,3\r\n\r\nOK\r\n") != NULL) ||
             (strstr(GprsC.ATStr,"\r\n+CREG: 0,4\r\n\r\nOK\r\n") != NULL)) //;����Ѱ��ע����
    */
    else if((strstr(GprsC.ATStr,"\r\n+CREG: 0,0\r\nOK\r\n") != NULL) ||
             (strstr(GprsC.ATStr,"\r\n+CREG: 0,2\r\nOK\r\n") != NULL) ||
             (strstr(GprsC.ATStr,"\r\n+CREG: 0,3\r\nOK\r\n") != NULL) ||
             (strstr(GprsC.ATStr,"\r\n+CREG: 0,4\r\nOK\r\n") != NULL)) //;����Ѱ��ע���� 20140704
    {

        GprsC.ATStatus = 'E';
        GprsC.CREGErrNum++;
        if (GprsC.CREGErrNum < 30)
        {
            GprsC.SendAT_Wait1s = 2;
            GprsC.SendAT_Step = 5;                   //;���ź�
            GprsC.Status |= _FindHost;              //;����Ѱ����վ
        }
        else                   //;����GPRSʧ��,�źŲ��ã�����Ч��
        {
            GprsC.CREGErrNum = 0;
            Modem_OFF();
        }
    }
}


 /* 
********************************************************************************
* �� �� ����
* ��    �ܣ�
* ��    ����
* ��    �أ� 
* ˵    ����AT+CMGF=1Ϊ�ı�ģʽ =0ΪPDUģʽ
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
void AT07Comd_CMGF(void)
{    
    if(GprsC.ATStatus == 'E')  
    {
        Modem_AT_Send("AT+CMGF=1\r\0");
    }    
    else if(strstr(GprsC.ATStr, "\r\nOK\r\n") != NULL)                          //;��������Ϊ�ı�ģʽ
    {
        GprsC.ATStatus = 'E';
        GprsC.SendAT_Wait1s = 1;
        GprsC.SendAT_Step = 8;
    }
}


/*----------------------------------------------------------------------------*
 |  ����TE�ַ���ΪUCS2
 *----------------------------------------------------------------------------*/
void AT08Comd_UCS2(void)
{
    if(GprsC.ATStatus == 'E')  
    {
           //; Modem_AT_Send("AT+CSCS=\"UCS2\"\r\0");
       Modem_AT_Send("AT+CSCS=\"GSM\"\r\0");
    }  
    else if(strstr(GprsC.ATStr, "\r\nOK\r\n") != NULL)                          //;����TE�ַ���ΪGSM
    {
        GprsC.ATStatus = 'E';
        GprsC.SendAT_Wait1s = 1;
        GprsC.SendAT_Step = 9;
    }
}


/*----------------------------------------------------------------------------*
 |  �¶�������ֱ����ʾ����������SIM��
 *----------------------------------------------------------------------------*/
void AT09Comd_CNMI(void)
{   
    if(GprsC.ATStatus == 'E')  
    {
        Modem_AT_Send("AT+CNMI=2,2,0,0,0\r\0");
    }     
    else if(strstr(GprsC.ATStr, "\r\nOK\r\n") != NULL)
    {
        GprsC.ATStatus = 'E';
        GprsC.SMSPer = true;                                                                                                //;��ɷ��Ͷ��ŵ�����
        GprsC.SendAT_Wait1s = 1;
        GprsC.SendAT_Step = 10;
    }
}


/*----------------------------------------------------------------------------*
 |  ʹ���ڲ�Э��ջ
 *----------------------------------------------------------------------------*/
void AT10Comd_XISP(void)
{  
    if(GprsC.ATStatus == 'E')  
    {
        Modem_AT_Send("AT+XISP=0\r\0");
    }    
    else if(strstr(GprsC.ATStr, "\r\nOK\r\n") != NULL)
    {
        GprsC.ATStatus = 'E';
        GprsC.SendAT_Wait1s = 1;
        GprsC.SendAT_Step = 11;
    }
}


/*----------------------------------------------------------------------------*
 |  ����APN��AT + CGDCONT = 1, "IP", "APN"
 *----------------------------------------------------------------------------*/
void AT11Comd_CGDCONT(char *APN)
{
    if(GprsC.ATStatus == 'E')
    {                         
        strcpy(GprsC.ATStr, "AT+CGDCONT=1,\"IP\",\"\0");
        strcat(GprsC.ATStr, APN);
        strcat(GprsC.ATStr, "\"\r\0");
        Modem_AT_Send(GprsC.ATStr);
    }    
    else if(strstr(GprsC.ATStr, "\r\nOK\r\n") != NULL)
    {
        GprsC.ATStatus = 'E';
        GprsC.SendAT_Wait1s = 1;
        GprsC.SendAT_Step = 12;
    }
}


/*----------------------------------------------------------------------------*
 |  ����PPP����
 *----------------------------------------------------------------------------*/
void AT12Comd_XIIC(void)
{
    if(GprsC.ATStatus == 'E')
    {                         
        Modem_AT_Send("AT+xiic=1\r\0");
    }    
    else if (strstr(GprsC.ATStr, "\r\nOK\r\n") != NULL)
    {
        GprsC.ATStatus = 'E';
        GprsC.SendAT_Wait1s = 1;
        GprsC.SendAT_Step = 13;
    }
}


/*----------------------------------------------------------------------------*
 |  ��ѯPPP��·״̬
 *----------------------------------------------------------------------------*/
void AT13Comd_XIIC(void)
{
    if(GprsC.ATStatus == 'E')
    {                         
        Modem_AT_Send("AT+xiic?\r\0");
    }    
    else if (strstr(GprsC.ATStr, "\r\n+XIIC:    1,") != NULL)                    //;PPP���ӽ����ɹ�
    {
        GprsC.ATStatus = 'E';
        GprsC.SendAT_Wait1s = 1;
        GprsC.SendAT_Step = 14;
        GprsC.XIICErrNum = 0;
    }
    else if (strstr(GprsC.ATStr, "\r\n+XIIC:    0, 0.0.0.0") != NULL)       //;PPP���ӽ���ʧ��,������APN�������Ƿ���ˣ������źŲ���
    {
        GprsC.ATStatus = 'E';
        GprsC.XIICErrNum++;
        if (GprsC.XIICErrNum < 30)
        {
            GprsC.SendAT_Step = 13;
            GprsC.SendAT_Wait1s = 2;
        }
        else
        {
            GprsC.XIICErrNum = 0;
             //;����ģ��
            Modem_OFF();
        }
    }
}

/* 
********************************************************************************
* �� �� ����NumIpToChar
* ��    �ܣ�����ֵIP�Ͷ˿�ת��Ϊ�ַ���������ָ����������
* ��    ����Buf--����ת������ַ���IP--IP��Ż��壻Port--�˿ںš�
* ��    �أ�ת�����ַ�����
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��2012-03-26 10:25:18
********************************************************************************
*/
u8 NumIpToChar(char *Buf, u8 *IP, u16 Port)
{
    u8  j;
    u8  len = 0;
    u8  ch1 = 0;
    u8  ch2 = 0;
    u16 sh;
    
    for(j = 0; j < 4; j++)    //;��վIP
    {
        ch1 = *(IP + j) / 100; //;��λ
        if(ch1 != 0)
        {
            ch2 = 1;
            *(Buf + len++) = ch1 + '0';
        }
        ch1 = (*(IP + j) % 100) / 10; //;ʮλ
        if((ch2 != 0) || (ch1 != 0))
        {
            *(Buf + len++) = ch1 + '0';
        }
        ch1 = *(IP + j) % 10; //;��λ
        *(Buf + len++) = ch1 + '0';
        ch2 = 0;
        *(Buf + len++) = '.';
    }
    len--;
    *(Buf + len++) = ',';
    
    sh = Port; //;�˿ں�
    ch1 = sh / 10000;
    if(ch1 != 0)
    {
        ch2 = 1; //;��λ��Ϊ0
        *(Buf + len++) = ch1 + '0';
    }
    ch1 = (sh % 10000) / 1000;
    if((ch2 != 0) || (ch1 != 0))
    {
        ch2 = 1;
        *(Buf + len++) = ch1 + '0';
    }
    ch1 = (sh % 1000) / 100;
    if((ch2 != 0) || (ch1 != 0))
    {
        ch2 = 1;
        *(Buf + len++) = ch1 + '0';
    }
    ch1 = (sh % 100) / 10;
    if((ch2 != 0) || (ch1 != 0))
    {
        ch2 = 1;
        *(Buf + len++) = ch1 + '0';
    }
    ch1 = sh % 10;
    *(Buf + len++) = ch1 + '0';
    
    return len;
}



/*----------------------------------------------------------------------------*
 |  ����TCP���ӣ�ATָ�AT + TCPSETUP = 0, IP, Port
 *----------------------------------------------------------------------------*/
void AT14Comd_TCPSETUP(u8 Link, u8 *IP, u16 Port)
{
     u8  i = 0, j;
     
    if(GprsC.ATStatus == 'E')
    {                                
        strcpy(GprsC.ATStr, "AT+TCPSETUP=");
        i += 12;
        *(GprsC.ATStr + i++) = Link + 0x30;
        *(GprsC.ATStr + i++) = ',';
        if(GprsC.LinkState == LINK0) 
        {           
            i += NumIpToChar(GprsC.ATStr + i, IP, Port);       
            *(GprsC.ATStr + i++) = '\r';
            *(GprsC.ATStr + i++) = '\0';
            
            if(ZDS.F10.Flux_Month < FluxLimValue)
            {
                Modem_AT_Send(GprsC.ATStr);
                if (GprsC.ATStatus == 'S')
                {
                /*Ϊʲô��500���ֽ�???̩���û�мӣ�����ȥ����by zrt 130705
                    ZDS.F10.Flux_Day += 500;
                    ZDS.F10.Flux_Month += 500;
                */
                }
            }
            else
            {
                #ifdef ZS
                RecordErr(1);
                #endif
                GprsC.ATStatus = idle;
                GprsC.State &= (~_Link); //;�˳�����״̬
                GprsC.State |= _Redial;  //;�����ز�״̬
            }
        }
        else
        {
            for(j = 0; j < GprsLC.IP1_long; j++)
            {
                *(GprsC.ATStr + i++) = GprsLC.IP1[j];
            }
            *(GprsC.ATStr + i++) = '\r';
            *(GprsC.ATStr + i++) = '\0';
            Modem_AT_Send(GprsC.ATStr);
        }
                
    }       
    else if(strstr(GprsC.ATStr, "\r\nOK\r\n") != NULL)    //;���������Ѿ�ʵ������
    {
        ;              //;��������,
    }
    else if(strstr(GprsC.ATStr, "\r\n+TCPSETUP:0,OK\r\n") != NULL)             //;ģ������վ�Ѿ�ʵ������
    {
        GprsC.ATStatus = 'R';
        GprsC.State &= (~_Link);                                 //;�˳�����״̬
        GprsC.State |= _Connect;                                //;��ʾ����ǰ����վ���ӳɹ�
        GprsC.Status &= ~_FindHFalst;
        GprsC.Status &= ~_FindHost;                            //;��ʾѰ����վ����
        GprsC.Status |= _ContHost;                            //;@11 Ѱ����վʧ��
       
    }
    else if(strstr(GprsC.ATStr, "\r\n+TCPSETUP:0,FAIL\r\n")!= NULL)            //;����ʧ�ܣ���������վû�򿪻����粻ͨ
    {
        #ifdef ZS
                RecordErr(2);
        #endif
        GprsC.ATStatus = 'R';
        GprsC.State &= (~_Link);                                                //;�˳�����״̬
        GprsC.State |= _Redial;                                                 //;�����ز�״̬
        GprsC.Status &= ~_FindHost;                                             //;��ʾѰ����վ����
        GprsC.Status |= _FindHFalst;                                            //;@11 Ѱ����վʧ��
    }
    else if(strstr(GprsC.ATStr, "\r\n+TCPSETUP:Error n\r\n") != NULL)
    {
             //;����ģ��
            Modem_OFF();
    }
    else if(strstr(GprsC.ATStr, "\r\n+TCPSETUP:1,OK\r\n") != NULL)             //;ģ������վ�Ѿ�ʵ������
    {
        GprsC.ATStatus = 'R';
        GprsLC.Link2Switch = false; //;��������������
        GprsC.State1 &= (~_Link);                                                //;�˳�����״̬
        GprsC.State1 |= _Connect;                                                //;��ʾ����ǰ����վ���ӳɹ�
    }
    else if (strstr(GprsC.ATStr, "\r\n+TCPSETUP:1,FAIL\r\n")!= NULL)            //;����ʧ�ܣ���������վû�򿪻����粻ͨ
    {
        GprsC.ATStatus = 'R';
        GprsLC.Link2Switch = false; //;��������������
        GprsC.State1 = 0;       
        GprsC.LinkState = LINK0;                                         //;�˳�����״̬
       
    }
}

/*----------------------------------------------------------------------------*
 |  ���ģ��TCP��·�Ƿ�����IPSTATUS
 *----------------------------------------------------------------------------*/
void AT15Comd_IPSTATUS(u8 Link)
{
    if (GprsC.ATStatus == 'E')
    {                         
         u8 i = 0;        
         strcpy(GprsC.ATStr, "AT+IPSTATUS=");
         i += 12;
         *(GprsC.ATStr + i++) = Link + 0x30;
         *(GprsC.ATStr + i++) = '\r';
         *(GprsC.ATStr + i++) = '\0';
         Modem_AT_Send(GprsC.ATStr);
    }   
    else if (strstr(GprsC.ATStr, "\r\n+IPSTATUS:0,CONNECT") != NULL)             //;��·1��������״̬
    {
        GprsC.ATStatus = 'R';
    }
    else if (strstr(GprsC.ATStr, "\r\n+IPSTATUS:0,DISCONNECT") != NULL)     //;��·1���ڶϿ�״̬
    {
        #ifdef ZS
                RecordErr(3);
        #endif
        GprsC.ATStatus = 'R';
        Modem_GDCDOFF();                                                     //;�ص������
        GprsC.State &= ~_Connect;
        GprsC.State &= (~_HadLogin);
        GprsC.State &= (~_Link); //;�˳�����״̬
        GprsC.State |= _Redial;  //;�����ز�״̬
        GprsC.Status &= ~_ContHost;
        GprsC.Status |= _FindHFalst;  
    }
     else if (strstr(GprsC.ATStr, "\r\n+IPSTATUS:1,CONNECT") != NULL)             //;��·1��������״̬
    {
        GprsC.ATStatus = 'E';
        GprsC.SendAT_Wait1s = 2;
        GprsC.SendAT_Step = 17;
    }
    else if (strstr(GprsC.ATStr, "\r\n+IPSTATUS:1,DISCONNECT") != NULL)     //;��·1���ڶϿ�״̬
    {
        GprsC.ATStatus = 'R';                                                    //;�ص������
        GprsC.State1 = 0x00;
        GprsC.LinkState = LINK0;
      
    }
}


/*----------------------------------------------------------------------------*
 |  ǿ�ƶϿ�ģ��TCP��·TCPCLOSE
 *----------------------------------------------------------------------------*/
void AT17Comd_TCPCLOSE(u8 Link)
{
    if (GprsC.ATStatus == 'E')
    {                         
        u8 i = 0;       
        strcpy(GprsC.ATStr,"AT+TCPCLOSE=");
        i += 12;
        *(GprsC.ATStr + i++) = Link + 0x30;
        *(GprsC.ATStr + i++) = '\r';
        *(GprsC.ATStr + i++) = '\0';
        Modem_AT_Send(GprsC.ATStr);
    }    
    else if (strstr(GprsC.ATStr, "\r\n+TCPCLOSE:0,OK\r\n") != NULL)              //;��·1�ɹ��Ͽ�
    {
        #ifdef ZS
                RecordErr(4);
        #endif
        GprsC.ATStatus = 'R';
        Modem_GDCDOFF();                                                     //;�ص������
        GprsC.State &= (~_Connect);                                           //;�����Ͽ����ӳɹ�
        GprsC.State &= (~_HadLogin);
        GprsC.State &= (~_Link); //;�˳�����״̬
        GprsC.State |= _Redial;  //;�����ز�״̬
        GprsC.Status = 0;
    }
    else if(strstr(GprsC.ATStr, "\r\n+TCPCLOSE:1,OK\r\n") != NULL)              //;��·1�ɹ��Ͽ�
    {
        GprsC.ATStatus = 'R';       
        GprsC.State1 = 0x00;
        GprsC.LinkState = LINK0;
    }
}


/*----------------------------------------------------------------------------*
 |  ��ػ�
 *----------------------------------------------------------------------------*/
void AT20Comd_CPWROFF(void)
{   
    if(GprsC.ATStatus == 'E')
    {
        Modem_AT_Send("AT+CPWROFF\r\0");                                     //;��ػ�
    }    
    else if(strstr(GprsC.ATStr, "\r\nOK\r\n") != NULL)
    {
         GprsC.ATStatus = 'R'; 
    }
    else if(strstr(GprsC.ATStr, "\r\nCME ERROR:<error>\r\n") != NULL) //;����
    {
        GprsC.ATStatus = 'R';  
        Modem_PowOFF();
    }
}


/*----------------------------------------------------------------------------*
 |  ATָ�AT + TCPSEND = 0,XXX;
 *----------------------------------------------------------------------------*/
void AT21Comd_TCPSEND(u8 Link, u16 Len)
{
    if (GprsC.ATStatus == 'E')
    {
        u8 i = 0;       
        strcpy(GprsC.ATStr, "AT+TCPSEND=");
        i += 11;
        *(GprsC.ATStr + i++) = Link + 0x30;
        *(GprsC.ATStr + i++) = ',';
        *(GprsC.ATStr + i++) = Len / 100 + 0x30;                                //;д����Ҫͨ��TCPЭ�鷢�͵����ݵĳ���
        *(GprsC.ATStr + i++) = (Len % 100) / 10 + 0x30;
        *(GprsC.ATStr + i++) = (Len % 100) % 10 + 0x30;
        *(GprsC.ATStr + i++) = '\r';
        *(GprsC.ATStr + i++) = '\0';
        Modem_AT_Send(GprsC.ATStr);
    }   
    else if (strstr(GprsC.ATStr, "\r\n>") != NULL)
    {
        GprsC.ATStatus = 'T';                                               //;����ͨ��TCPЭ�鷢������
    }
    else if (strstr(GprsC.ATStr, "\r\n+TCPSEND:0") != NULL)
    {
        GprsC.ATStatus = 'O';                                               //;ͨ����·1TCPЭ�鷢���������
    }
     else if(strstr(GprsC.ATStr, "\r\n+TCPSEND:1") != NULL)
    {
        GprsC.ATStatus = 'O';                                               //;ͨ����·1TCPЭ�鷢���������
    }
    else if(strstr(GprsC.ATStr, "\r\n+TCPSEND:Error"))  //;��·��δ���������ܴ��ڼ�����״̬
    {
        GprsC.TCPFail = true; 
        if(GprsC.LinkState == LINK0)
        {
            if(GprsC.State & _HadLogin)   
            {
                #ifdef ZS
                RecordErr(5);
                #endif
                Modem_GDCDOFF();                                                     //;�ص������
                GprsC.State &= (~_Connect);                                           //;�����Ͽ����ӳɹ�
                GprsC.State &= (~_HadLogin);
                GprsC.State &= (~_Link); //;�˳�����״̬
                GprsC.State |= _Redial;  //;�����ز�״̬
                GprsC.Status = 0x00;      //;���״̬
            }   
        }
        
    }
}


/*----------------------------------------------------------------------------*
 |  ATָ�AT + CMGS = XXX;
 *----------------------------------------------------------------------------*/
void AT22Comd_CMGS(u16 Len)
{
    if (GprsC.ATStatus == 'E')
    {
        u8 i = 0;    
        strcpy(GprsC.ATStr,"AT+CMGS=");
        i += 8;
        *(GprsC.ATStr + i++) = Len / 100 + 0x30;                                //;д����Ҫͨ�����ŷ��͵����ݵĳ���
        *(GprsC.ATStr + i++) = (Len % 100) / 10 + 0x30;
        *(GprsC.ATStr + i++) = (Len % 100) % 10 + 0x30;
        *(GprsC.ATStr + i++) = '\r';
        *(GprsC.ATStr + i++) = '\0';
        Modem_AT_Send(GprsC.ATStr);
        GprsC.CMGSNum = 0;
    }    
    else if (strstr(GprsC.ATStr, "\r\n>") != NULL)
    {
        GprsC.CMGSNum++;
        if (GprsC.CMGSNum == 1)
        {
           GprsC.ATStatus = 'M';                                               //;���Է��Ͷ�������
        }
        else if (GprsC.CMGSNum == 2)
        {
           GprsC.ATStatus = 'R'; 
        }
    }
    else if (strstr(GprsC.ATStr, "+CMGS:") != NULL)
    {
        GprsC.CMGSNum = 0;
        GprsC.ATStatus = 'O';   //;Over                                            //;���Ͷ��ųɹ�
    }
}


/*----------------------------------------------------------------------------*
 |  ��ģ�鷢����ATָ���Ժ󣬵ȴ���ʱ���Ժ�û�н��յ�Ӧ��
 *----------------------------------------------------------------------------*/
void ATComdSetNoRec(void)
{
    static u8 OldStep, firstsign = true;
    
    if(OldStep != GprsC.SendAT_Step) //;����ط���־λ
    {
        OldStep = GprsC.SendAT_Step;
        GprsC.ATAns_Wait1s = 0;
        GprsC.ReSendAT_Num = 0;
    }
    
    if(GprsC.ATStatus == 'E')
    {
        if (GprsC.SendAT_Wait1s > 0)
        {
            GprsC.SendAT_Wait1s--;
        }
    }

    else if(GprsC.ATStatus == 'S')
    {
        GprsC.ATAns_Wait1s++;
        if(GprsC.ATAns_Wait1s > 15)
        {
            GprsC.ATAns_Wait1s = 0;
            GprsC.ReSendAT_Num++;
            if(GprsC.ReSendAT_Num < 2)
            {
                if(GprsC.SendAT_Step == 1)
                {
                    if(firstsign)
                    {
                        firstsign = false;
                        USART2_BaudRateSet(115200); //;��һ����ģ��ͨ��Ҫ����Ƭ���Ĳ�������Ϊ115200;
                        GprsC.ATStatus = 'E';
                        GprsC.SendAT_Wait1s = 3;
                        GprsC.SendAT_Step = 2;    //;ģ��ûAT�ķ��أ���Ҫ��STEP2ȥ��ģ��Ĳ�����
                    }
                    else
                    {
                        Modem_PowOFF();
                    }
                }
                else if(GprsC.SendAT_Step == 2)
                {
                    USART2_BaudRateSet(9600); //;��һ����ģ��ͨ��Ҫ����Ƭ���Ĳ�������Ϊ115200;
                    GprsC.ATStatus = 'E';
                    GprsC.SendAT_Wait1s = 3;
                    GprsC.SendAT_Step = 1;    //;ģ��ûAT�ķ��أ���Ҫ��STEP2ȥ��ģ��Ĳ�����
                }
                else if(GprsC.SendAT_Step == 20) //;��ػ���Ӧ����ϵ�
                {
                    Modem_PowOFF();
                }
                else
                {
                    GprsC.ATStatus = 'E';
                    GprsC.SendAT_Wait1s = 3;   //;�ط�
                }
            }
            else
            {
                if(GprsC.SendAT_Step == 21)
                {
                    GprsC.TCPFail = true;
                }
                if(GprsC.LinkState == LINK1) //;��������վ����
                {
                    GprsLC.Link2Switch = false; //;��������������
                    GprsC.State1 = 0x00; 
                    GprsC.LinkState = LINK0;
                }             
                Modem_OFF();        //;�ر�ģ��                
            }
        }
    }
    
       
    if(GprsC.ATStatus != idle)  //;AT״̬����
    {
        GprsC.ATSOVT++;
        if(GprsC.ATSOVT > 240)
        {
            GprsC.ATStatus = idle; //;״̬��λ
            Modem_PowOFF();
        }
    }
    else
    {
        GprsC.ATSOVT = 0;
    }
}



/*----------------------------------------------------------------------------*
 |  ģ���������͵�AT����󣬸���ģ��ظ���AT������д���
 *----------------------------------------------------------------------------*/
void Modem_AT_Deal(void)
{    
    
    switch (GprsC.SendAT_Step)
    {
        case 1:
        {
            AT01Comd_AT();  //; �鿴ģ���Ƿ������ɹ�
            break;
        }
        case 2:
        {
            AT02Comd_IPR();//;���貨����
            break;
        }
        case 3:
        {
            AT03Comd_ATE0(); //;�رջ���
            break;
        }
        case 4:
        {
            AT04Comd_CPIN(); //;���sim����״̬
            break;
        }
        case 5:
        {
            AT05Comd_CSQ();  //;���ģ���ź�ǿ��
            break;
        }
        case 6:
        {
            AT06Comd_CREG(); //;���GSM����ע�����
            break;
        }
        case 7:
        {
            AT07Comd_CMGF(); //;��������ΪPDUģʽ
            break;
        }
        case 8:
        {
            AT08Comd_UCS2();  //;����TE�ַ���ΪUCS2
            break;
        }
        case 9:
        {
            AT09Comd_CNMI(); //;�¶�������ֱ����ʾ����������SIM��
            break;
        }
        case 10:
        {
            AT10Comd_XISP();  //;ʹ���ڲ�Э��ջ
            break;
        }
        case 11:
        {
            AT11Comd_CGDCONT(GprsC.APN);
            break;
        }
        case 12:
        {
            AT12Comd_XIIC(); //; ����PPP ����
            break;
        }
        case 13:
        {
            AT13Comd_XIIC(); //;��ѯPPP��·״̬
            break;
        }
        case 14:
        {
            AT14Comd_TCPSETUP(GprsC.LinkState, GprsC.IP, GprsC.Port);//;��·1����TCP����
            break;
        }
        case 15:
        {
            AT15Comd_IPSTATUS(GprsC.LinkState);   //;�鿴��·1�����Ƿ�����
            break;
        }
        case 17:
        {
            AT17Comd_TCPCLOSE(GprsC.LinkState); //;������Ҫǿ�ƶϿ���·1
            break;
        }
        case 20:
        {
            AT20Comd_CPWROFF(); //;��ػ�
            break;
        }
        case 21:
        {
            AT21Comd_TCPSEND(GprsC.LinkState, GprsC.TCPLen); //;AT+TCPSEND=0,XXX
            break;
        }
        case 22:
        {
            AT22Comd_CMGS(GprsC.SMSLen); //;AT + CMGS = XXX
            break;
        }      
        default:
        {
            GprsC.ATStatus =0;
            GprsC.ReConnect =true;						
            break;
        }
    }  
}


/*----------------------------------------------------------------------------*
 |  GPRSģ��ͨѶ
 *----------------------------------------------------------------------------*/
void Modem_Com(void)
{
     u16 i;
    
    if((GprsC.ATStatus == 'E')&&(GprsC.SendAT_Wait1s == 0)) //;��������Ҫ����
    {
         Modem_AT_Deal();
    }

    if(GPRSUSARTCTR.RecOrd == true)
    {
        GPRSUSARTCTR.RecOrd = false;
        for (i = 0; i < GPRSUSARTCTR.RecLen; i++)  //;ֻ����ǰ�����50���ֽ�
        {
            if (i == 50)  break;
            GprsC.ATStr[i] =  *(GPRSUSARTCTR.BufAdr + i);
        }
        
        GprsC.ATLen = i;
        
        for (; i < 50; i++)
        {
            GprsC.ATStr[i] = 0;
        }
        
        if(GprsC.ATStatus == 'S')
        {
            Modem_AT_Deal();
        }
        else if(strstr(GprsC.ATStr, "+TCPRECV:0,") != NULL)   //;�յ���վ����������
        {
            TCPDATALIGHTON(); //;���ݵ���
            GprsC.DataLightMs = 200;
            GprsC.NDataMin0 = 0x0000; //;���յ����ݣ�TCP�����ݼ�ʱ����
            GprsC.LinkState = LINK0;
            GprsC.TCPRec = true;  //;�յ�TCP����
            GprsC.TCPBuf = GPRSUSARTCTR.BufAdr;
            GprsC.TCPLen = GPRSUSARTCTR.RecLen - 11;
            GPRSUSARTCTR.RecLen = 0;
        }
        else if(strstr(GprsC.ATStr, "+TCPRECV:1,") != NULL)   //;�յ���վ����������
        {
            GprsC.LinkState = LINK1;
            GprsC.NDataMin1 = 0x00;
            GprsC.TCPRec = true;  //;�յ�TCP����
            GprsC.TCPBuf = GPRSUSARTCTR.BufAdr;
            GprsC.TCPLen = GPRSUSARTCTR.RecLen - 11;
            GPRSUSARTCTR.RecLen = 0;
        }
        else if(strstr(GprsC.ATStr, "\r\n+TCPCLOSE:0,Link Closed\r\n") != NULL)     //;���������Ѿ��Ͽ�
        {
            #ifdef ZS
                RecordErr(6);
        #endif
            GprsC.ATStatus = 'R';
            Modem_GDCDOFF();                                                     //;�ص������
            GprsC.State &= (~_Connect);                                          //;��ʾ���統ǰΪ�Ͽ�״̬
            GprsC.State &= (~_HadLogin);                                         //;��ǰ�����ڵ�½��״̬
            GprsC.State &= (~_Link); //;�˳�����״̬
            GprsC.State |= _Redial;  //;�����ز�״̬
            GprsC.Status = 0;
        }
        else if(strstr(GprsC.ATStr, "\r\n+TCPCLOSE:1,Link Closed\r\n") != NULL)     //;���������Ѿ��Ͽ�
        {
            GprsC.State1 = 0x00;
        }
        else if(strstr(GprsC.ATStr, "\r\n+CMT:") != NULL)
        {
            Modem_SMS_Rece();   //;�յ����ŵĴ���
        }

  
        for(i = 0; i < GprsC.ATLen; i++)
        {
            GprsC.ATStr[i] = 0; //;���AT����׼������
        }
        GprsC.ATLen = 0;
        
        if(GprsC.ATStatus == 'R')  //;������ȷ
        {
            GprsC.ATStatus = idle;
        }
    }
}


/*���绰�����Ƿ����*/
u8 SMS_NumCheck(u8 *NumAddr)
{
    if ((*(NumAddr + 0) == 0) && (*(NumAddr + 1) == 0) &&
        (*(NumAddr + 2) == 0) && (*(NumAddr + 3) == 0) &&
        (*(NumAddr + 4) == 0) && (*(NumAddr + 5) == 0) &&
        (*(NumAddr + 6) == 0) && (*(NumAddr + 7) == 0))
    {
        return false;
    }
    else
    {
        return true;
    }
}


/*----------------------------------------------------------------------------*
 |  ����    : ���úö��ŵ����ĺ�������պ���
 |  out_buf : д�뷢�����ݵĵ�ַ
 |  SMS_Code: �������ĺ���ĵ�ַ
 |  Tel_Code: �����û��ֻ�����
 |  return  : д�뷢�����ݵĳ���
 *----------------------------------------------------------------------------*/
u8 SMS_CodeData(char *Out_Buf, u8 *SMS_Code, u8 *Tel_Code)
{
    u8 j = 0;

    *(Out_Buf + j++) = '0';
    *(Out_Buf + j++) = '8';
    *(Out_Buf + j++) = '9';
    *(Out_Buf + j++) = '1';
    *(Out_Buf + j++) = '6';
    *(Out_Buf + j++) = '8';

    *(Out_Buf + j++) = *(SMS_Code + 3) / 0x10 + 0x30;                           //;�������ĺ���
    *(Out_Buf + j++) = *(SMS_Code + 2) % 0x10 + 0x30;
    *(Out_Buf + j++) = *(SMS_Code + 4) / 0x10 + 0x30;
    *(Out_Buf + j++) = *(SMS_Code + 3) % 0x10 + 0x30;
    *(Out_Buf + j++) = *(SMS_Code + 5) / 0x10 + 0x30;
    *(Out_Buf + j++) = *(SMS_Code + 4) % 0x10 + 0x30;
    *(Out_Buf + j++) = *(SMS_Code + 6) / 0x10 + 0x30;
    *(Out_Buf + j++) = *(SMS_Code + 5) % 0x10 + 0x30;
    *(Out_Buf + j++) = *(SMS_Code + 7) / 0x10 + 0x30;
    *(Out_Buf + j++) = *(SMS_Code + 6) % 0x10 + 0x30;
    *(Out_Buf + j++) = 'F';
    *(Out_Buf + j++) = *(SMS_Code + 7) % 0x10 + 0x30;
    *(Out_Buf + j++) = '0';
    *(Out_Buf + j++) = '1';
    *(Out_Buf + j++) = '0';
    *(Out_Buf + j++) = '0';

    *(Out_Buf + j++) = '0';                                                     //;�Է����볤��תʮ������
    *(Out_Buf + j++) = 'B';
    *(Out_Buf + j++) = '8';
    *(Out_Buf + j++) = '1';

    *(Out_Buf + j++) = *(Tel_Code + 3) / 0x10 + 0x30;                           //;д����Ž����ߵĺ���
    *(Out_Buf + j++) = *(Tel_Code + 2) % 0x10 + 0x30;
    *(Out_Buf + j++) = *(Tel_Code + 4) / 0x10 + 0x30;
    *(Out_Buf + j++) = *(Tel_Code + 3) % 0x10 + 0x30;
    *(Out_Buf + j++) = *(Tel_Code + 5) / 0x10 + 0x30;
    *(Out_Buf + j++) = *(Tel_Code + 4) % 0x10 + 0x30;
    *(Out_Buf + j++) = *(Tel_Code + 6) / 0x10 + 0x30;
    *(Out_Buf + j++) = *(Tel_Code + 5) % 0x10 + 0x30;
    *(Out_Buf + j++) = *(Tel_Code + 7) / 0x10 + 0x30;
    *(Out_Buf + j++) = *(Tel_Code + 6) % 0x10 + 0x30;
    *(Out_Buf + j++) = 'F';
    *(Out_Buf + j++) = *(Tel_Code + 7) % 0x10 + 0x30;
    *(Out_Buf + j++) = '0';
    *(Out_Buf + j++) = '0';
    *(Out_Buf + j++) = '0';
    *(Out_Buf + j++) = '8';

    return j;
}


/*----------------------------------------------------------------------------*
 |  Hex����ת����Ascii����
 *----------------------------------------------------------------------------*/
char SMS_HexToASC(u8 HEX)
{
    if (HEX <= 9)
    {
        HEX += 0x30;
    }
    else if ((HEX >= 0x0A) && (HEX <= 0x0F))
    {
        HEX += 0x37;
    }
    else
    {
        HEX = 0xFF;
    }
    return HEX;
}


/*----------------------------------------------------------------------------*
 |  ���Unicode����Ϊ4��Ascii����
 *----------------------------------------------------------------------------*/
u8 SMS_DataChange(char *ASC, u16 Unicode)
{
    char ASCII[4];

    ASCII[3] = SMS_HexToASC((u8)(Unicode & 0x000F));
    ASCII[2] = SMS_HexToASC((u8)((Unicode & 0x00F0) >> 4));
    ASCII[1] = SMS_HexToASC((u8)((Unicode & 0x0F00) >> 8));
    ASCII[0] = SMS_HexToASC((u8)((Unicode & 0xF000) >> 12));

    if ((ASCII[0] != 0x30) || (ASCII[1] != 0x30) || (ASCII[2] != 0x30) || (ASCII[3] != 0x30))
    {
        *(ASC + 0) = ASCII[0];
        *(ASC + 1) = ASCII[1];
        *(ASC + 2) = ASCII[2];
        *(ASC + 3) = ASCII[3];
        return 4;
    }

    return 0;
}


/*----------------------------------------------------------------------------*
 |  ����    ��ȡ�����ֵı��벢ת����Ascii�ַ�д�뷢�͵�ַ
 |  Len     �����ָ�����
 |  InAddr  �����ֱ����ŵĵ�ַ
 |  OutAddr �����ַ��͵ĵ�ַ
 |  return  ��ʵ��д��OutAddr��ַ��Ascii����ĸ���
 *----------------------------------------------------------------------------*/
u8 SMS_DataMove(u8 Len, char *OutAddr, u16 *InAddr)
{
    u8 i, j = 0, L;

    for (i = 0; i < Len; i++)
    {
        L = SMS_DataChange(OutAddr + j, *(InAddr + i));
        if (L == 0)
        {
            break;
        }
        else
        {
            j += L;
        }
    }
    return j;
}

/*******************************************************************************
 * ��������:   Link2Timing
 * ����    :   ������վ��½��ʼ��ʱ�����10�������޳������ݣ������Ͽ�
 * ����    :
 * ���    :
 * ����    :
 *******************************************************************************/
void Modem_LinkTime(void)
{
    u8 type;
    
    type = ZDP.F8.WorkMode & 0x03;
    
    if((type <= 0x01) || ((type == 0x03) && GprsLC.LinkEnable))
    {
        GprsC.NDataMin0++;
        if(GprsC.NDataMin0 > 180)  //;����Сʱ��TCP����ͨ��
        {
            GprsC.NLogonMin = 0;
            GprsC.NDataMin0 = 0;
            Modem_PowOFF(); //;����������ģ��
        }
        if(!(GprsC.State & _HadLogin))
        {
            GprsC.NLogonMin++;
            if(GprsC.NLogonMin > 180)
            {
                GprsC.NDataMin0 = 0;
                GprsC.NLogonMin = 0;
                Modem_PowOFF(); //;����������ģ��
            }
        }
    }
    else if(type == 0x02)
    {   
        if(GprsLC.PassLinkOrd) 
        {
            if(!(GprsC.State & _HadLogin))//;
            {
                GprsC.NLogonMin++;
                if(GprsC.NLogonMin > 5)
                {
                    GprsC.NDataMin0 = 0;
                    GprsC.NLogonMin = 0;
                    Modem_PowOFF(); //;����������ģ��
                }
            }
            else  //;���ڵ�¼״̬
            {
                GprsC.NDataMin0++;
                if(GprsC.NDataMin0 > ZDP.F8.NoComBreakMin)  //;����Сʱ��TCP����ͨ��
                {
                    GprsC.NLogonMin = 0;
                    GprsC.NDataMin0 = 0;
                    GprsLC.PassLinkOrd = false; //;�����ز�
                    Modem_OFF(); //;�ر�ģ��ģ��
                }
            }
        }
    }
    
    
    if(GprsC.State1 & _HadLogin)
    {
        GprsC.NDataMin1++;
        if(GprsC.NDataMin1 >= ZDP.F8.NoComBreakMin)//;����10�����Զ��Ͽ���·2       
        {
            if(GprsC.ATStatus  == idle)
            {
                GprsC.NDataMin1 = 0;
                GprsC.LinkState = LINK1;
                GprsC.ATStatus = 'E';
                GprsC.SendAT_Wait1s = 2;
                GprsC.State1 = 0x00;  //;���״̬
                GprsC.SendAT_Step = 15; //;�鿴��·2
            }
            else
            {
                GprsC.NDataMin1 = ZDP.F8.NoComBreakMin; //;�ȴ�LinkState����
            }
        }
    }
    else if(GprsC.State1 & _Link) 
    {
        GprsC.NDataMin1++;
        if(GprsC.NDataMin1 >= 5) //;5�������Ӳ��ɹ������״̬��
        {
            GprsC.NDataMin1 = 0x00;
            GprsLC.Link2Switch = false; //;��������������
            GprsC.State1 = 0x00; 
            GprsC.LinkState = LINK0;
        }
    }
}

#ifdef ZS
/* 
********************************************************************************
* �� �� ����RecordErr
* ��    �ܣ�����ʱ����
* ��    ����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
void RecordErr(u8 sign)
{
    u8 *addr1, *addr2;
    u8 len;
    u8 i;
    
    GprsC.RailSign = sign;
    len = sizeof(struct ModemContrl);
    addr1 = (u8*)&GprsC;
    addr2 = (u8*)&GprsCErr;
    for(i = 0; i < len; i++)
    {
        *addr2 = *addr1;
        addr1++;
        addr2++;
    }
}
#endif


/* 
********************************************************************************
* �� �� ����Modem_GDCDLightCtr
* ��    �ܣ�û�е�¼�Ҳ����ز�״̬�������1����˸һ��
* ��    ����
* ��    �أ� 
* ˵    ����
* �׳��쳣��
* ��    �ߣ�
* �޸ļ�¼��
********************************************************************************
*/
void Modem_GDCDLightCtr(void)
{
    static u8 N250ms;   

    if(!(GprsC.State & _HadLogin))
    {
        if(GprsC.Sign > 10)
        {
            N250ms++;        
            if(N250ms == 2)
            {
                Modem_GDCDON();
            }
            else if(N250ms >=4)
            {
                N250ms = 0;
                Modem_GDCDOFF();
            }
        }
        else
        {
            Modem_GDCDOFF();
        }
    }
}


