/**
  ******************************************************************************
  * @file     ����������      esp8266_test.h
  * @author ���������ߣ�      Lu
  * @version������汾��      V1.0
  * @date   ���޸����ڣ�      2020-10-08
  * @brief  �����ܼ�飩      STM32F103VET6 ��ģ��
  * @parameter  ��������	  	��
  * @return   ������ֵ��	  	��
  ******************************************************************************
  * @attention ��ע�����
  * оƬ : STM32F103VET6
  *
  ******************************************************************************
  */ 
/*------------------------------------------------------------------------------*/ 
#ifndef __ESP8266_TEST_H
#define __ESP8266_TEST_H

/* �û�ͷ�ļ� ------------------------------------------------------------------*/
#include "stm32f10x.h"//����stm32f10xͷ�ļ�
#include <stdio.h>  
#include <string.h>  
#include <stdbool.h>

#include "./SysTick/SysTick.h"  //���� ϵͳ��ʱ�� ͷ�ļ�
#include "./DHT11/dht11.h"  //����  DHT11������ ͷ�ļ�
#include "./ESP8266/ESP8266.h"	//����ESP8266 ͷ�ļ�
/********************************** �û���Ҫ���õĲ���**********************************/

#define      User_ESP8266_BulitApSsid         "Liu"      //Ҫ�������ȵ������
#define   	 User_ESP8266_BulitApEcn           OPEN               //Ҫ�������ȵ�ļ��ܷ�ʽ
#define      User_ESP8266_BulitApPwd           "liulu74123."         //Ҫ�������ȵ����Կ
#define      User_ESP8266_TcpServer_OverTime   "1800"             //��������ʱʱ�䣨��λ���룩

#define      User_ESP8266_ApSsid                       "Mercy"                //Ҫ���ӵ��ȵ������
#define      User_ESP8266_ApPwd                        "liulu74123."           //Ҫ���ӵ��ȵ����Կ
#define      User_ESP8266_TcpServer_IP                 "110.65.97.60"      //Ҫ���ӵķ������� IP
#define      User_ESP8266_TcpServer_Port               "8080"               //Ҫ���ӵķ������Ķ˿�



/* �궨�� ----------------------------------------------------------------------*/

//�ⲿȫ�ֱ���
extern volatile uint8_t ucTcpClosedFlag;

/* �������� --------------------------------------------------------------------*/
void ESP8266_StaTcpClient_UnvarnishTest ( void );//ESP8266 ��Sta Tcp Client��͸��
void ESP8266_StaTcpClient_UnvarnishTest_OnlineTest( void );//ESP8266 ��Sta Tcp Client��͸�����ߴ�����

void ESP8266_AP_Config( void );//ESP8266����ΪAPģʽ-��Ϊ������������

#endif /* __ESP8266_TEST_H */
/* END OF FILE ------------------------------------------------------------------*/

