/**
  ******************************************************************************
  * @file     （程序名）      esp8266_test.h
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
  ******************************************************************************
  */ 
/*------------------------------------------------------------------------------*/ 
#ifndef __ESP8266_TEST_H
#define __ESP8266_TEST_H

/* 用户头文件 ------------------------------------------------------------------*/
#include "stm32f10x.h"//引用stm32f10x头文件
#include <stdio.h>  
#include <string.h>  
#include <stdbool.h>

#include "./SysTick/SysTick.h"  //引用 系统定时器 头文件
#include "./DHT11/dht11.h"  //引用  DHT11传感器 头文件
#include "./ESP8266/ESP8266.h"	//引用ESP8266 头文件
/********************************** 用户需要设置的参数**********************************/

#define      User_ESP8266_BulitApSsid         "Liu"      //要建立的热点的名称
#define   	 User_ESP8266_BulitApEcn           OPEN               //要建立的热点的加密方式
#define      User_ESP8266_BulitApPwd           "liulu74123."         //要建立的热点的密钥
#define      User_ESP8266_TcpServer_OverTime   "1800"             //服务器超时时间（单位：秒）

#define      User_ESP8266_ApSsid                       "Mercy"                //要连接的热点的名称
#define      User_ESP8266_ApPwd                        "liulu74123."           //要连接的热点的密钥
#define      User_ESP8266_TcpServer_IP                 "110.65.97.60"      //要连接的服务器的 IP
#define      User_ESP8266_TcpServer_Port               "8080"               //要连接的服务器的端口



/* 宏定义 ----------------------------------------------------------------------*/

//外部全局变量
extern volatile uint8_t ucTcpClosedFlag;

/* 函数声明 --------------------------------------------------------------------*/
void ESP8266_StaTcpClient_UnvarnishTest ( void );//ESP8266 （Sta Tcp Client）透传
void ESP8266_StaTcpClient_UnvarnishTest_OnlineTest( void );//ESP8266 （Sta Tcp Client）透传掉线处理函数

void ESP8266_AP_Config( void );//ESP8266设置为AP模式-作为局域网服务器

#endif /* __ESP8266_TEST_H */
/* END OF FILE ------------------------------------------------------------------*/

