/**
  ******************************************************************************
  * @file     （程序名）      esp8266.h
  * @author （程序作者）      Lu
  * @version（程序版本）      V1.0
  * @date   （修改日期）      2020-10-08
  * @brief  （功能简介）      STM32F103VET6 库模板
  * @parameter  （参数）	  	无
  * @return   （返回值）	  	无
  ******************************************************************************
  * @attention （注意事项）
  * 芯片 : STM32F103VET6
  *
  * 端口：
  * 
  *
  *
  ******************************************************************************
  */ 
/*------------------------------------------------------------------------------*/ 
#ifndef __ESP8266_H
#define __ESP8266_H

/* 用户头文件 ------------------------------------------------------------------*/
#include "stm32f10x.h"//引用stm32f10x头文件
#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h> 

#include "./SysTick/SysTick.h"  //引用 系统定时器 头文件

/* 宏定义 ----------------------------------------------------------------------*/


#if defined ( __CC_ARM   )
#pragma anon_unions
#endif

#define RX_BUF_MAX_LEN     1024                                     //最大接收缓存字节数

extern struct  STRUCT_USARTx_Fram                                  //串口数据帧的处理结构体
{
	char  Data_RX_BUF [ RX_BUF_MAX_LEN ];
	
  union {
    __IO u16 InfAll;
    struct {
		  __IO u16 FramLength       :15;                               // 14:0 
		  __IO u16 FramFinishFlag   :1;                                // 15 
	  } InfBit;
  }; 
	
} strEsp8266_Fram_Record;

extern struct STRUCT_USARTx_Fram strUSART_Fram_Record;



//工作模式选择结构体
/*
 * 1-SoftAP模式（软路由模式）
 *		在此模式下，模块本身相当于一个路由器，其他设备可链接到该模块发送的信号，主要应用在主从设备的主机部分
 * 2-station模式（客户端模式）
 *		在此模式下，模块相当于一个客户端，可以链接到其他路由器发出的WIFI信号。主要应用在网络通信中
 * 3-SoftAP+station共存模式（混合模式）
 *		在此模式下，模块可在与其他设备链接的同时充当路由器，结合上面两种模式的综合应用，一般可应用在需要网络通信且在主从关系中的主机		
 */
typedef enum{
	STA,					//客户端模式
  AP,						//软路由模式
  STA_AP  			//混合模式
} ESP8266_Work_ModeTypeDef;


//定义网络协议结构体
typedef enum{
	 enumTCP,
	 enumUDP,
} ENUM_NetPro_TypeDef;

//定义模块连接服务器ID结构体
typedef enum{
	Multiple_ID_0 = 0,
	Multiple_ID_1 = 1,
	Multiple_ID_2 = 2,
	Multiple_ID_3 = 3,
	Multiple_ID_4 = 4,
	Single_ID_0 = 5,
} ENUM_ID_NO_TypeDef;
	
//AP模式加密方式
typedef enum{
	OPEN = 0,
	WEP = 1,
	WPA_PSK = 2,
	WPA2_PSK = 3,
	WPA_WPA2_PSK = 4,
} ENUM_AP_PsdMode_TypeDef;


/******************************** ESP8266 连接引脚定义 ***********************************/
#define      ESP8266_CHIP_EN_APBxClock_FUN                 RCC_APB2PeriphClockCmd
#define      ESP8266_CHIP_EN_CLK                           RCC_APB2Periph_GPIOB  
#define      ESP8266_CHIP_EN_PORT                          GPIOB
#define      ESP8266_CHIP_EN_PIN                           GPIO_Pin_8

#define      ESP8266_RST_APBxClock_FUN                     RCC_APB2PeriphClockCmd
#define      ESP8266_RST_CLK                               RCC_APB2Periph_GPIOB
#define      ESP8266_RST_PORT                              GPIOB
#define      ESP8266_RST_PIN                               GPIO_Pin_9

 

#define      ESP8266_USART_BAUD_RATE                       115200

#define      ESP8266_USARTx                                USART3
#define      ESP8266_USART_APBxClock_FUN                   RCC_APB1PeriphClockCmd
#define      ESP8266_USART_CLK                             RCC_APB1Periph_USART3
#define      ESP8266_USART_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define      ESP8266_USART_GPIO_CLK                        RCC_APB2Periph_GPIOB     
#define      ESP8266_USART_TX_PORT                         GPIOB   
#define      ESP8266_USART_TX_PIN                          GPIO_Pin_10
#define      ESP8266_USART_RX_PORT                         GPIOB
#define      ESP8266_USART_RX_PIN                          GPIO_Pin_11
#define      ESP8266_USART_IRQ                             USART3_IRQn
#define      ESP8266_USART_INT_FUN                         USART3_IRQHandler

/*********************************************** ESP8266 函数宏定义 *******************************************/
#define     ESP8266_Usart( fmt, ... )           ESP8266_USART_Printf ( ESP8266_USARTx, fmt, ##__VA_ARGS__ ) 
#define     PC_Usart( fmt, ... )                printf ( fmt, ##__VA_ARGS__ )
//#define     macPC_Usart( fmt, ... )                

//CHIP_EN 高电平有效 ESP8266正常工作
#define     ESP8266_CHIP_EN_ENABLE()                 GPIO_SetBits ( ESP8266_CHIP_EN_PORT, ESP8266_CHIP_EN_PIN )
#define     ESP8266_CHIP_EN_DISABLE()                GPIO_ResetBits ( ESP8266_CHIP_EN_PORT, ESP8266_CHIP_EN_PIN )

//EX_RSTB  外部 reset 信号(低电平有效)
#define     ESP8266_RST_HIGH_LEVEL()            GPIO_SetBits ( ESP8266_RST_PORT, ESP8266_RST_PIN )
#define     ESP8266_RST_LOW_LEVEL()             GPIO_ResetBits ( ESP8266_RST_PORT, ESP8266_RST_PIN )


/* 函数声明 --------------------------------------------------------------------*/
void ESP8266_Init( void );//ESP8266初始化函数
void ESP8266_RST( void );//重启WF-ESP8266模块
void ESP8266_USART_Printf( USART_TypeDef * USARTx, char * Data, ... );//格式化输出
bool ESP8266_AT_Cmd( char * cmd, char * reply1, char * reply2, u32 waittime );//对WF-ESP8266模块发送AT指令
void ESP8266_AT_Test( void );//对WF-ESP8266模块进行AT测试启动
bool ESP8266_Work_Mode_Choose(ESP8266_Work_ModeTypeDef  ESP8266_WorkMode );//选择WF-ESP8266模块的工作模式
bool ESP8266_Join_WIFI(char * pSSID, char * pPassWord );//WF-ESP8266模块连接外部WiFi
bool ESP8266_Build_WIFI( char * pSSID, char * pPassWord, ENUM_AP_PsdMode_TypeDef ESP8266_ENUM_AP_PsdMode );//WF-ESP8266模块创建WiFi热点	
bool ESP8266_Enable_MultipleId ( FunctionalState enumEnUnvarnishTx );//WF-ESP8266模块启动多连接
bool ESP8266_Link_Server ( ENUM_NetPro_TypeDef enumE, char * ip, char * ComNum, ENUM_ID_NO_TypeDef id);//WF-ESP8266模块连接外部服务器
bool ESP8266_StartOrShutServer ( FunctionalState enumMode, char * pPortNum, char * pTimeOver );//WF-ESP8266模块开启或关闭服务器模式
uint8_t ESP8266_Get_LinkStatus ( void );//获取 WF-ESP8266 的连接状态，较适合单端口时使用
uint8_t ESP8266_Get_IdLinkStatus ( void );//获取 WF-ESP8266 的端口（Id）连接状态，较适合多端口时使用
uint8_t ESP8266_Inquire_ApIp ( char * pApIp, uint8_t ucArrayLength );//获取 F-ESP8266 的 AP IP
bool ESP8266_UnvarnishSend ( void );//配置WF-ESP8266模块进入透传发送
void ESP8266_ExitUnvarnishSend ( void );//配置WF-ESP8266模块退出透传模式
bool ESP8266_SendString ( FunctionalState enumEnUnvarnishTx, char * pStr, u32 ulStrLength, ENUM_ID_NO_TypeDef ucId );//WF-ESP8266模块发送字符串
char * ESP8266_ReceiveString ( FunctionalState enumEnUnvarnishTx );//WF-ESP8266模块接收字符串
uint8_t ESP8266_CWLIF ( char * pStaIp );//查询已接入设备的IP
uint8_t ESP8266_CIPAP ( char * pApIp );//设置模块的 AP IP


#endif /* __ESP8266_H */
/* END OF FILE ------------------------------------------------------------------*/

