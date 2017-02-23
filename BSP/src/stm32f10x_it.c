/**
  ******************************************************************************
  * @file    USART/Interrupt/stm32f10x_it.c
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    04/16/2010
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "BSP_CFG.h"


/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup USART_Interrupt
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern uint8_t TxBuffer1[];
extern uint8_t TxBuffer2[];
extern uint8_t RxBuffer1[];
extern uint8_t RxBuffer2[];
extern __IO uint8_t TxCounter1;
extern __IO uint8_t TxCounter2;
extern __IO uint8_t RxCounter1;
extern __IO uint8_t RxCounter2;
extern uint8_t NbrOfDataToTransfer1;
extern uint8_t NbrOfDataToTransfer2;
extern uint8_t NbrOfDataToRead1;
extern uint8_t NbrOfDataToRead2;

extern u16 DelayNms;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
    static u8 EventN250ms = 0x00;
    
    
    
    if(DelayNms)
    {
        DelayNms--;
    }
    
    if(U5DS.RecRun == true)   //;����1���ڽ����ֽ�
    { 
        U5DS.RBet2Byte++;      //;��¼����1���������ֽ�֮���ʱ�����
    }
#ifdef Local_USART1_ON    
    if(U1DS.RecRun == true)   //;����1���ڽ����ֽ�
    { 
        U1DS.RBet2Byte++;      //;��¼����1���������ֽ�֮���ʱ�����
    }
#endif 

#ifdef PORT2USART  
    if(U4DS.RecRun == true)   //;����4���ڽ����ֽ�
    { 
        U4DS.RBet2Byte++;      //;��¼����3���������ֽ�֮���ʱ�����
    }
#endif  
  
    if(U2DS.RecRun == true)   //;����2���ڽ����ֽ�
    { 
        U2DS.RBet2Byte++;      //;��¼����2���������ֽ�֮���ʱ�����
    }
    
    if(U3DS.RecRun == true)   //;����3���ڽ����ֽ�
    { 
        U3DS.RBet2Byte++;      //;��¼����3���������ֽ�֮���ʱ�����
    }
    
    if(U5DS.TBusy)             //;����5����1���ֽ��Ժ����ʱʱ��
    { 
        U5DS.TBusy--; 
    }
    
#ifdef Local_USART1_ON    
    if(U1DS.TBusy)  				  //;����1����1���ֽ��Ժ����ʱʱ��
    { 
        U1DS.TBusy--; 
    }
#endif    

#ifdef PORT2USART    
    if(U4DS.TBusy)  				  //;����4����1���ֽ��Ժ����ʱʱ��
    { 
        U4DS.TBusy--; 
    }
#endif
    
    if(U2DS.TBusy)  				  //;����3����1���ֽ��Ժ����ʱʱ��
    { 
        U2DS.TBusy--; 
    }
    
    if(U3DS.TBusy)  				  //;����3����1���ֽ��Ժ����ʱʱ��
    { 
        U3DS.TBusy--; 	
    }
    
    if(GprsC.DataLightMs)
    {
        GprsC.DataLightMs--;
        if(GprsC.DataLightMs == 0x00)
        {
            TCPDATALIGHTOFF(); //;���ݵ���
        }
    }
    
    if(PORT1USARTCTR.DataLightMs)
    {
        PORT1USARTCTR.DataLightMs--;
        if(PORT1USARTCTR.DataLightMs == 0x00)
        {
            METERDATALIGHTOFF(); //;���ݵ���
        }
    }
    
    if(!HWareC.EventFlag)
    {
        EventN250ms <<= 1;
        if(EVENTCHECK() == 0x01)
        {
            EventN250ms++;
            if(EventN250ms == 0xFF)
            {
                HWareC.EventFlag = true; //;�и澯�¼���Ҫ��ȡ
            }
        }
    }
//;    HWareC.GIn2<<=1;  //;��GIn2��״̬
//;	  if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11) == 0x01)
//;	  {
//;	      HWareC.GIn2++;
//;	  }	   
//;	  if(HWareC.GIn2 == 0xFF)
//;    {
//;        HWareC.GIn2_Del++;
//;        if(HWareC.GIn2_Del >= 20)
//;        {
//;            HWareC.GIn2_Del = 0x00;
//;            Power_Modem = _48V;
//;        }
//;    }
//;    else if(HWareC.GIn2 == 0x00)
//;    {
//;        Power_Modem = _220V;
//;    }
//;
//;	  HWareC.RI<<=1;  //;��RI��״̬
//;	  if( GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)==0x01 )
//;	  HWareC.RI++;
    
}

/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/
void RTC_IRQHandler(void)
{
    if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
    {
        /* Clear the RTC Second interrupt */
        RTC_ClearITPendingBit(RTC_IT_SEC);
        RTC_FlagMs = true; 
//;        Flag_025s = true;
        Num025s_WT++;  
    }
    else if (RTC_GetITStatus(RTC_IT_OW) != RESET)
    {
        RTC_ClearITPendingBit(RTC_IT_OW);
    }
    else if (RTC_GetITStatus(RTC_IT_ALR) != RESET)
    {
        RTC_ClearITPendingBit(RTC_IT_ALR);
    }
}


void UART5_IRQHandler(void)
{
    if (USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
    {
        ReceiveByte_UART5();
        USART_ClearITPendingBit(UART5, USART_IT_RXNE);
    }
    if (USART_GetITStatus(UART5, USART_IT_TXE) != RESET)
    {
        SendByte_UART5();
        USART_ClearITPendingBit(UART5, USART_IT_TXE);
    }
   
}

#ifdef Local_USART1_ON
/*USART1_IRQHandler*/
void USART1_IRQHandler(void)
{
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        ReceiveByte_USART1();
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);   //;����յ��жϱ�־
    }
    if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
    {
        SendByte_USART1();
        USART_ClearITPendingBit(USART1, USART_IT_TXE);   //;�巢�͵��жϱ�־
    }
}
#endif

/*USART2_IRQHandler*/
void USART2_IRQHandler(void)
{
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        ReceiveByte_USART2();
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);   //;����յ��жϱ�־
    }
    if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)
    {
        SendByte_USART2();
        USART_ClearITPendingBit(USART2, USART_IT_TXE);   //;�巢�͵��жϱ�־
    }
}

/*USART3_IRQHandler*/
void USART3_IRQHandler(void)
{
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        ReceiveByte_USART3();
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);   //;����յ��жϱ�־
    }
    if(USART_GetITStatus(USART3, USART_IT_TXE) != RESET)
    {
        SendByte_USART3();
        USART_ClearITPendingBit(USART3, USART_IT_TXE);   //;�巢�͵��жϱ�־
    }
}

#ifdef PORT2USART
/*USART4_IRQHandler*/
void UART4_IRQHandler(void)
{
    if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
    {
        ReceiveByte_UART4();
        USART_ClearITPendingBit(UART4, USART_IT_RXNE);   //;����յ��жϱ�־
    }
    if(USART_GetITStatus(UART4, USART_IT_TXE) != RESET)
    {       
        SendByte_UART4();
        USART_ClearITPendingBit(UART4, USART_IT_TXE);   //;�巢�͵��жϱ�־
    }
}
#endif
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
