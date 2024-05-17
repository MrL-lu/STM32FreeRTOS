/**
  ******************************************************************************
  * @file     （程序名）	  	main.h
  * @author （程序作者）		Lu
  * @version（程序版本）		V1.0.0.0
  * @date   （修改日期）		2024-02-20
  * @brief  （功能简介）		STM32F103VET6固件库模板
  * @parameter  （参数）		无
  * @return   （返回值）		无
  ******************************************************************************
  * @attention （注意事项）
  * 芯片型号：STM32F103VET6
  * 使用引脚：
  * 修改日期：
  * 实现功能：
  *	
  ******************************************************************************
  */
/*------------------------------------------------------------------------------*/

#ifndef __BSP_USART_H
#define __BSP_USART_H
/* 用户头文件 ------------------------------------------------------------------*/
#include "stm32f10x.h"   //引用stm32头文件
#include <stdio.h>

/* 宏定义 ----------------------------------------------------------------------*/
// 串口对应的DMA请求通道
#define  USART_RX_DMA_CHANNEL           DMA1_Channel5
// 外设寄存器地址
#define  USART_DR_ADDRESS               (&DEBUG_USARTx->DR)
// 一次发送的数据量
#define  USART_RBUFF_SIZE               1000 

// 串口1-USART1
#define  DEBUG_USARTx                   USART1
#define  DEBUG_USART_CLK                RCC_APB2Periph_USART1
#define  DEBUG_USART_APBxClkCmd         RCC_APB2PeriphClockCmd
#define  DEBUG_USART_BAUDRATE           115200

// USART GPIO 引脚宏定义
#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  DEBUG_USART_TX_GPIO_PORT       GPIOA   
#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_9
#define  DEBUG_USART_RX_GPIO_PORT       GPIOA
#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_10

#define  DEBUG_USART_IRQ                USART1_IRQn
#define  DEBUG_USART_IRQHandler         USART1_IRQHandler

/* 函数声明 --------------------------------------------------------------------*/
void USART_Config(void);
void USARTx_DMA_Config(void);
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);
void Uart_DMA_Rx_Data(void);

#endif  /*  __BSP_USART_H */

/* END OF FILE ------------------------------------------------------------------*/
