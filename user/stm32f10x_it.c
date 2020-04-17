/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "include.h"
#include "usb_istr.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

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
  * @brief  This function handles PendSVC exception.
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
	SysTick_Increment();
    SysTick_Decrement(); 
}

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

void USART1_IRQHandler(void)	/* 中断处理函数名要与中断源对应 */
{
    u8 value = 0;

    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)	/* 判断读寄存器是否非空	*/
    {
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);     /* 清除中断标志	*/

        value = USART_ReceiveData(USART1);	/* 读取接收到的数据 */

        QueuePutchar(&uart1RxQ, value, 0);	/* 将接收到的数据存入队列 */
    }

    if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)    /* 这段是为了避免STM32 USART 第一个字节发不出去的BUG */
    {
        USART_ITConfig(USART1, USART_IT_TXE, DISABLE);		/* 禁止发缓冲器空中断 */
    }
}

/******************************************************************************/
void USART2_IRQHandler(void)
{
    u8 value = 0;
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)	/* 判断读寄存器是否非空	*/
    {
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);     /* 清除中断标志	*/

        value = USART_ReceiveData(USART2);	/* 读取接收到的数据 */
        QueuePutchar(&uart2RxQ, value, 0);	/* 将接收到的数据存入队列 */
    }

    if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)    /* 这段是为了避免STM32 USART 第一个字节发不出去的BUG */
    {
        USART_ITConfig(USART2, USART_IT_TXE, DISABLE);		/* 禁止发缓冲器空中断 */
    }
}

/******************************************************************************/
void USART3_IRQHandler(void)
{
    u8 value = 0;

    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)	/* 判断读寄存器是否非空	*/
    {
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);     /* 清除中断标志	*/

        value = USART_ReceiveData(USART3);	/* 读取接收到的数据 */
        QueuePutchar(&uart3RxQ, value, 0);	/* 将接收到的数据存入队列 */
    }

    if(USART_GetITStatus(USART3, USART_IT_TXE) != RESET)    /* 这段是为了避免STM32 USART 第一个字节发不出去的BUG */
    {
        USART_ITConfig(USART3, USART_IT_TXE, DISABLE);		/* 禁止发缓冲器空中断 */
    }
}

/******************************************************************************/
void CAN2_RX0_IRQHandler(void)
{
  
  CAN_Receive(CAN2, CAN_FIFO0, &RxMessage);
        
	//if((RxMessage.ExtId == 0x12)
        //   && (RxMessage.IDE == CAN_ID_EXT))
  if(RxMessage.IDE == CAN_ID_EXT)
  {
    CAN_RX_Flag = 1;
  }
  else
  {
    CAN_RX_Flag = 0;
  }
  CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);
}



/******************************************************************************/
void TIM3_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
  {
//		timer_Interrupt();
		__TIM_COUNT++;
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);         //Clear Interrupt
	}
}


/*******************************************************************************
* Function Name  : OTG_FS_IRQHandler
* Description    : This function handles USB-On-The-Go FS global interrupt request.
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#ifdef STM32F10X_CL
void OTG_FS_IRQHandler(void)
{
  STM32_PCD_OTG_ISR_Handler(); 
}

#else
/*----------USB中断配置----------*/
void USB_LP_CAN1_RX0_IRQHandler(void)
{
  USB_Istr();
}
#endif

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
