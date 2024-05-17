/**
  ******************************************************************************
  * @file     （程序名）      ccs811.h
  * @author （程序作者）      Lu
  * @version（程序版本）      V1.0
  * @date   （修改日期）      2020-10-08
  * @brief  （功能简介）      STM32F103VET6 库模板
  * @parameter  （参数）	  	无
  * @return   （返回值）	  	无
  ******************************************************************************
  * @attention （注意事项）
  * 芯片 : STM32F103VET6
  * CCS811 空气质量传感器 （检测CO2浓度、总挥发性有机物含量）
  * 通讯协议：串口（模块） IIC
  *
  *
  ******************************************************************************
  */ 
/*------------------------------------------------------------------------------*/ 
#ifndef __CCS811_H
#define __CCS811_H

/* 用户头文件 ------------------------------------------------------------------*/
#include "stm32f10x.h"//引用stm32f10x头文件
#include "stdio.h"	
#include "string.h"

#include "./SysTick/SysTick.h"  //引用 系统定时器 头文件
#include "./USART/usart.h"  //引用 串口通讯 头文件

/* 宏定义 ----------------------------------------------------------------------*/
//串口波特率
#define 	CCS811_USART_BAUDRATE             115200

// 串口1-USART1
#define  	CCS811_USART      	              USART2
#define  	CCS811_USART_CLK_1            	  RCC_APB2Periph_USART1
#define  	CCS811_USART_APBxClkCmd_1   	    RCC_APB2PeriphClockCmd


// USART GPIO 引脚宏定义
#define  	CCS811_USART1_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  	CCS811_USART1_GPIO_APBxClkCmd  	  RCC_APB2PeriphClockCmd
//TXD    
#define  	CCS811_USART1_TX_GPIO_PORT      	GPIOA   
#define  	CCS811_USART1_TX_GPIO_PIN        	GPIO_Pin_2
//RXD
#define  	CCS811_USART1_RX_GPIO_PORT    	  GPIOA
#define  	CCS811_USART1_RX_GPIO_PIN      	  GPIO_Pin_3
// USART 中断宏定义
#define  	CCS811_USART_IRQ               	  USART2_IRQn
#define  	CCS811_USART_IRQHandler						USART2_IRQHandler


/* 函数声明 --------------------------------------------------------------------*/
void CCS811_USART_GPIO_Config( void );//CCS811 空气传感器 串口GPIO配置函数
void CSS811_USART_Send(uint8_t *data,uint8_t len);//CCS811 空气传感器 串口发送函数
void CCS811_DATA_Analysis(unsigned char flag);//CCS811 数据解析函数
void CCS811_Check(unsigned char* flag);//CCS811 传感器检测函数
void CCS811_AT_Send_Init( void );//CCS811 传感器发送AT指令初始化设置函数
void CCS811_USART_IRQHandler( void );//CCS811 空气传感器 串口中断函数
#endif /* __CCS811_H */
/* END OF FILE ------------------------------------------------------------------*/

