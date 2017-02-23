/*
********************************************************************************
*  �� �� ����Setup.C
*
*  �ļ�������Ӳ����ʼ����غ���  
*
*  ����оƬ��
*
*  �� �� �ˣ�
*
*  �� �� �ţ�
*
*  �޸ļ�¼��1.�޸���ʱ��������Ӧ��ͬʱ�ӡ� 
********************************************************************************
*/
#include "BSP_CFG.h"

#define I2C1_SLAVE_ADDRESS7    0xa0   //;FM24C16
#define I2C2_SLAVE_ADDRESS7    0x65   //;EPSON8025
#define I2C_Clock              100000

u16 DelayNms;


/* 
********************************************************************************
* �� �� ����RCC_Configuration
* ��    �ܣ�ͳ��Χ����ʱ������
* ��    ������
* ��    �أ���
* ˵    ������
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void RCC_Configuration(void)
{
    /* ADCʱ����ΪAPB2��ʱ��2��Ƶ */
    RCC_ADCCLKConfig(RCC_PCLK2_Div2);

    /* ����GPIO����ʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
                           RCC_APB2Periph_GPIOC , ENABLE);

    /* ����I2C1,I2C2ʱ�� */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    /* ��������2,����4,����5ʱ�� */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 
                           | RCC_APB1Periph_USART3
                           | RCC_APB1Periph_UART5 
                           | RCC_APB1Periph_UART4, ENABLE);

    /* TIM3 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    /* ��������1,SPI1ʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_SPI1, ENABLE);
    
    /* ����SPI2ʱ�� */
//;    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

    /* Enable DMA1 clock */
//;    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    /* Enable ADC1 */
//;    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
}


/* 
********************************************************************************
* �� �� ����NVIC_Configuration
* ��    �ܣ�ϵͳ�ж��������ȼ�����
* ��    ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Configure the NVIC Preemption Priority Bits */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    /* Enable the RTC Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


/* 
********************************************************************************
* �� �� ����GPIO_Configuration
* ��    �ܣ���ʼ��IO��
* ��    ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    //;�����͹ر�GPRSģ�� PA8,���ƻ���485ͨ��PA4
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //;�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    //;���� PA0 ΪM_EVENT ���� ,���ڼ�����Ƿ����¼����� @20
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    //;���� PA4 ΪM_SET���� ,���ڼ�����Ƿ����������� @20 --20141210
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    
    //;���� PB14 Ϊ ���� ,���ڼ��GPRSģ���Ƿ����� @20
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //; PB0, PB1, PB8,PB9 LEDָʾ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //;�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);


    //; ���� PC6Ϊ���룬
    //;P6���ڼ��ģ���Ƿ����磬�����õ�
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOC, &GPIO_InitStructure);


    //;���� PC9Ϊ���
    //;PC9��������M590��EMERGOFF�ţ����ڽ����ػ��������õ�
    //;PC7 ���������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_7 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //;�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    //;���� PC14 ΪM_STA��©��� , @20 --20141210
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD; //;��©���
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    //;���� PC15 ΪM_RST ���� ,���ڼ�����Ƿ��и�λ���� @20 --20141210
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    //;���� PB12���
    //;PB12��Ϊ�ߵ�ƽʱ��GPRSģ���ϵ�
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //;�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

}


/* 
********************************************************************************
* �� �� ����SysTickStart
* ��    �ܣ�����ϵͳ�δ�ʱ��
* ��    ������
* ��    �أ���
* ˵    ����1ms
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void SysTickStart(void)
{
    if (SysTick_Config(SystemCoreClock / 1000)) 
    {
      /* Capture error */
      while (1);
    }
}


/* 
********************************************************************************
* �� �� ����RTC_Configuration
* ��    �ܣ�ʵʱʱ��250ms�ж�һ��
* ��    ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void RTC_Configuration(void)
{
      /* Enable PWR and BKP clocks */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

    /* Allow access to BKP Domain */
    PWR_BackupAccessCmd(ENABLE);

    /* Reset Backup Domain */
    BKP_DeInit();
    
    //;����RTC��ʱ��ΪHSE��128��Ƶ
    RCC_RTCCLKConfig(RCC_RTCCLKSource_HSE_Div128);  

    RCC_RTCCLKCmd(ENABLE);  //;����RTC��ʱ�ӹ���

    /* Wait for RTC registers synchronization */
    RTC_WaitForSynchro();

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();

    /* Enable the RTC Second */
    RTC_ITConfig(RTC_IT_SEC, ENABLE);

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();

    //;8M/128=62500HZ
    RTC_SetPrescaler(15624); //; 0.25���Ӳ���һ���ж�

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
}



/* 
********************************************************************************
* �� �� ����TIM_Configuration
* ��    �ܣ�
* ��    ����
* ��    �أ� 
* ˵    ����TIM2 Channel2(PA1) ���� 38KHZ ��������
*           ϵͳʱ�ӣ�56MHZ
*           TIM3_ARR = (56000 / 38) - 1 = 1472;
*           TIM3_CCR1 = 736;
*           ռ�ձ� = (TIM3_CCR1/ TIM3_ARR)* 100 = 50%
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void TIM_Configuration(void)
{
    u16 period;
    
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);

    /* Time base configuration  */
    period = (SystemCoreClock / 38000) - 1;
     TIM_TimeBaseStructure.TIM_Period = period;
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    /* PWM1 Mode configuration: Channel3 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = period / 2;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);

    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM2, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
}


/* 
********************************************************************************
* �� �� ����I2C_Configuration
* ��    �ܣ���ʼ��I2C����
* ��    ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
//void I2C_Configuration (void)
//{
//    GPIO_InitTypeDef GPIO_InitStructure;
//    I2C_InitTypeDef  I2C_InitStructure;
//
//    /*I2C-GPIO*/
//    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD; //;��©���
//    GPIO_Init(GPIOB, &GPIO_InitStructure);
//    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//;�������
//    GPIO_Init(GPIOB, &GPIO_InitStructure);
//    
//    /*I2C1 //ȡ��ʹ��оƬ�Դ�I2C --20141210*/
//    /*
//    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C; //;I2Cģʽ
//    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2; //;����ģʽTlow/Thigh=2
//    I2C_InitStructure.I2C_OwnAddress1 = I2C1_SLAVE_ADDRESS7; //;���õ�һ���豸��ַ
//    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable; //;ʹ��Ӧ��
//    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; //;Ӧ��7λ��ַ
//    I2C_InitStructure.I2C_ClockSpeed = I2C_Clock; //;����ʱ��Ƶ��
//    I2C_Cmd(I2C1, ENABLE);
//    I2C_Init(I2C1, &I2C_InitStructure);
//    */
//}


/* 
********************************************************************************
* �� �� ����USART_Configuration
* ��    �ܣ���ʼ��USART
* ��    ������
* ��    �أ���
* ˵    ����485A :  USART5 -- PC12(TX), PD2(RX)
*           485B :  USART4 -- PC10(TX), PC11(RX)
*           485C :  USART1 -- PA9(TX),  PA10(RX)
*           GPRS :  USART2 -- PA2(TX),  PA3(RX)
*           ����ڣ�USART3 -- PD8(TX),  PD9(RX),
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void USART_Configuration(void)
{
    GPIO_InitTypeDef        GPIO_InitStructure;
    USART_InitTypeDef       USART_InitStructure;
    USART_ClockInitTypeDef  USART_ClockInitStructure;

    //;ʱ������
    USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
    USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
    USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
    USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;

#ifdef Local_USART1_ON 
    /*USART1*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //;��������ģʽ
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_ClockInit(USART1, &USART_ClockInitStructure); //;��USART1��ͬ
   USART_InitStructure.USART_BaudRate = 9600;
//    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);
#else
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif   
    /*USART2*/
//;    GPIO_PinRemapConfig(GPIO_FullRemap_USART2, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_ClockInit(USART2, &USART_ClockInitStructure); //;��USART1��ͬ
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART2, ENABLE);
    
     /*USART3*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    USART_ClockInit(USART3, &USART_ClockInitStructure); //;��USART1��ͬ
    USART_InitStructure.USART_BaudRate = 1200;
    USART_InitStructure.USART_WordLength = USART_WordLength_9b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
//;    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_Parity = USART_Parity_Even;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART3, ENABLE);

#ifdef PORT2USART    
    /*USART4*/
//;    GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    USART_ClockInit(UART4, &USART_ClockInitStructure); //;��USART1��ͬ
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_9b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_Even;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(UART4, &USART_InitStructure);
    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
    USART_Cmd(UART4, ENABLE);
#else
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
#endif    
    
    /*USART5*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //;��������ģʽ
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //;���ù����������ģʽ
    GPIO_Init(GPIOC, &GPIO_InitStructure);

   
    USART_ClockInit(UART5, &USART_ClockInitStructure);

    //USART_InitStructure.USART_BaudRate = 9600; //;������
    USART_InitStructure.USART_BaudRate = 2400; //;������
    USART_InitStructure.USART_WordLength = USART_WordLength_9b; //;��У��λΪ9������λ
    USART_InitStructure.USART_StopBits = USART_StopBits_1; //;ֹͣλΪ1λ
    USART_InitStructure.USART_Parity = USART_Parity_Even; //;żУ��
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //;��Ӳ������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //;ͬʱ����TX��RX
    USART_Init(UART5, &USART_InitStructure);
    /*����USART5�жϽ���*/
    RS485CTRLRCV();
    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
    
    /*����USART5����*/
    USART_Cmd(UART5, ENABLE);

}


/* 
********************************************************************************
* �� �� ����SPI_FLASH_Init
* ��    �ܣ���ʼ��Flash���õ�SPI1���ߵ�����Ӳ��
* ��    ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void SPI_FLASH_Init(void)
{
    u16 i;

    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    //;��ʼ�� SPI1���� SCK, MISO and MOSI
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;;//;���ù����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //;PC4����spi1-Flash2Ƭѡ,PC3���ڸ�λ,PC5���ڱ���
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4|GPIO_Pin_3|GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //;�������
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    //;�����λ�źŵ�flash2оƬ
    GPIO_ResetBits(GPIOC, GPIO_Pin_3);
    for (i = 0;i < 100;i++);//;��ʱ
    {
        GPIO_SetBits(GPIOC, GPIO_Pin_4);
    }
    GPIO_SetBits(GPIOC, GPIO_Pin_3);

//;    //;�����λ�źŵ�flash1оƬ
//;    GPIO_ResetBits(GPIOC, GPIO_Pin_0);
//;    for (i = 0;i < 100; i++); //;��ʱ
//;    {
//;        GPIO_SetBits(GPIOC, GPIO_Pin_1);
//;    }
//;    GPIO_SetBits(GPIOC, GPIO_Pin_0);

    /* SPI1 configuration */
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //;ȫ˫��ͨѶ
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;  //;������
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;  //;���ݸ�ʽΪ8λ
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;   //;ʱ��Ϊ������
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;  //;��2��ʱ�Ӳſ�ʼ��Ч
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;  //;ͨѶ������Ϊ72M/4=18M
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  //;�ȷ������λ
    SPI_InitStructure.SPI_CRCPolynomial = 7;   //;ѭ��������У����ù�ʽ 7
    SPI_Init(SPI1, &SPI_InitStructure);
    /* Enable SPI1  */
    SPI_Cmd(SPI1, ENABLE);
}


/* 
********************************************************************************
* �� �� ����InitHardware
* ��    �ܣ���ʼ��ϵͳӲ��
* ��    ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void InitHardware(void)
{ 
   s8 i;
#ifdef IAP
   // NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x3000);
#endif
    RCC_Configuration();
#ifdef IWDGON
    IWDG_Configuration(); //;�򿪶������Ź�
 #endif
    NVIC_Configuration();
    GPIO_Configuration();
    
    SPI_FLASH_Init();
    SPI_FLASH_EraseProtReg(0); //;�����е������ı����Ĵ���ȫ��дΪ0xff
    I2C_InitFRAM();
    //I2C_Configuration();
    SysTickStart();    //;ϵͳʱ��1ms�ж�һ��
    RTC_Configuration();
    USART_Configuration();
    TIM_Configuration();
    LOGONLIGHTOFF;
    COMLIGHTOFF();
    TCPDATALIGHTOFF();
    METERDATALIGHTOFF();
    Modem_PowOFF();
    //;�����粻�ȶ�����ʱ2S
    for(i=5; i>0; i--)
    {
        Delay_nms(200);
        Watchdog(); 
        Delay_nms(200); 
        Watchdog();
    }
//    Delay_nms(200);
//    Watchdog(); 
//    Delay_nms(200); 
//    Watchdog(); 
//    Delay_nms(200);
}


/* 
********************************************************************************
* �� �� ����IWDG_Configuration
* ��    �ܣ����ö������Ź�
* ��    ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void IWDG_Configuration(void)
{
    /* Enable write access to IWDG_PR and IWDG_RLR registers */
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

    /* IWDG counter clock: 40KHz(LSI) / 64 = 0.625 KHz =1.6ms*/
    IWDG_SetPrescaler(IWDG_Prescaler_64);

    /* Set counter reload value to 2000 * 1.6ms = 3.2s */
    IWDG_SetReload(2000);

    /* Reload IWDG counter */
    IWDG_ReloadCounter();

    /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
    IWDG_Enable();
}


/* 
********************************************************************************
* �� �� ����Watchdog
* ��    �ܣ����Ź���λ
* ��    ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void Watchdog(void)
{
#ifdef IWDGON
    IWDG_ReloadCounter();
#endif
}

/* 
********************************************************************************
* �� �� ����Delay_nms
* ��    �ܣ���ʱ������������100ms����ι����
* ��    ������ʱ������
* ��    �أ� 
* ˵    ������ʱ���ܳ���1�롣
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��2012-05-18 15:16:12
********************************************************************************
*/
void Delay_nms(u16 n)
{
    DelayNms = n;
    if(n > 35)
    {
        Watchdog();
    }
    while(DelayNms)
    {
        ;
    }
}


/* 
********************************************************************************
* �� �� ����USART1_BaudRateSet
* ��    �ܣ�USART2 ���貨����
* ��    ������
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void USART2_BaudRateSet(u32 BaudRate)
{
  USART_InitTypeDef USART_InitStructure;
  USART_InitStructure.USART_BaudRate = BaudRate;  //;������
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //;��У��λΪ8������λ
  USART_InitStructure.USART_StopBits = USART_StopBits_1;  //;ֹͣλΪ1λ
  USART_InitStructure.USART_Parity = USART_Parity_No;  //;��У��
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //;��Ӳ������
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  //;ͬʱ����TX��RX
  USART_Init(USART2, &USART_InitStructure);  //;��ʼ�� USART2
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);  //;����USART2�ж�
  USART_Cmd(USART2, ENABLE);  //;����USART2����
  
}

/* 
********************************************************************************
* �� �� ����UARTx_BaudRate_Set
* ��    �ܣ����ô���4,5�Ĳ�����
* ��    ����BPS-�����ʣ�UAERx-���ں�
* ��    �أ���
* ˵    ����
* �׳��쳣��
* ��    �ߣ�023
* �޸ļ�¼��
********************************************************************************
*/
void UARTx_BaudRate_Set(u8 BPS, u8 UARTx)
{
    u32 BaudRate;
    USART_InitTypeDef USART_InitStructure;
    USART_ClockInitTypeDef USART_ClockInitStructure;

    switch(BPS)
    {
        case 0x01:
        {
            BaudRate = 600;
            break;
        }
        case 0x02:
        {
            BaudRate = 1200;
            break;
        }
        case 0x03:
        {
            BaudRate = 2400;
            break;
        }
        case 0x04:
        {
            BaudRate = 4800;
            break;
        }
        case 0x05:
        {
            BaudRate = 7200;
            break;
        }
        case 0x06:
        {
            BaudRate = 9600;
            break;
        }
        case 0x07:
        {
            BaudRate = 19200;
            break;
        }
        case 0x08:
        {
            BaudRate = 38400;
            break;
        }
        default:
        {
            BaudRate = 2400;
            break;
        }
    }

    USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
    USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
    USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
    USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;
    USART_InitStructure.USART_BaudRate = BaudRate;
    USART_InitStructure.USART_WordLength = USART_WordLength_9b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_Even;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    
    if(UARTx == 5)
    {
        USART_ClockInit(UART5, &USART_ClockInitStructure);
        USART_Init(UART5, &USART_InitStructure);
        USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
        USART_Cmd(UART5, ENABLE);
    }
#ifdef PORT2USART    
    else if(UARTx == 4)
    {
        USART_ClockInit(UART4, &USART_ClockInitStructure);
        USART_Init(UART4, &USART_InitStructure);
        USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
        USART_Cmd(UART4, ENABLE);
    }
#endif    
}
