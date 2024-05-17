/**
  ******************************************************************************
  * @file     ����������      esp8266.h
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
  * �˿ڣ�
  * 
  *
  *
  ******************************************************************************
  */ 
/*------------------------------------------------------------------------------*/ 
#ifndef __ESP8266_H
#define __ESP8266_H

/* �û�ͷ�ļ� ------------------------------------------------------------------*/
#include "stm32f10x.h"//����stm32f10xͷ�ļ�
#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h> 

#include "./SysTick/SysTick.h"  //���� ϵͳ��ʱ�� ͷ�ļ�

/* �궨�� ----------------------------------------------------------------------*/


#if defined ( __CC_ARM   )
#pragma anon_unions
#endif

#define RX_BUF_MAX_LEN     1024                                     //�����ջ����ֽ���

extern struct  STRUCT_USARTx_Fram                                  //��������֡�Ĵ���ṹ��
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



//����ģʽѡ��ṹ��
/*
 * 1-SoftAPģʽ����·��ģʽ��
 *		�ڴ�ģʽ�£�ģ�鱾���൱��һ��·�����������豸�����ӵ���ģ�鷢�͵��źţ���ҪӦ���������豸����������
 * 2-stationģʽ���ͻ���ģʽ��
 *		�ڴ�ģʽ�£�ģ���൱��һ���ͻ��ˣ��������ӵ�����·����������WIFI�źš���ҪӦ��������ͨ����
 * 3-SoftAP+station����ģʽ�����ģʽ��
 *		�ڴ�ģʽ�£�ģ������������豸���ӵ�ͬʱ�䵱·�����������������ģʽ���ۺ�Ӧ�ã�һ���Ӧ������Ҫ����ͨ���������ӹ�ϵ�е�����		
 */
typedef enum{
	STA,					//�ͻ���ģʽ
  AP,						//��·��ģʽ
  STA_AP  			//���ģʽ
} ESP8266_Work_ModeTypeDef;


//��������Э��ṹ��
typedef enum{
	 enumTCP,
	 enumUDP,
} ENUM_NetPro_TypeDef;

//����ģ�����ӷ�����ID�ṹ��
typedef enum{
	Multiple_ID_0 = 0,
	Multiple_ID_1 = 1,
	Multiple_ID_2 = 2,
	Multiple_ID_3 = 3,
	Multiple_ID_4 = 4,
	Single_ID_0 = 5,
} ENUM_ID_NO_TypeDef;
	
//APģʽ���ܷ�ʽ
typedef enum{
	OPEN = 0,
	WEP = 1,
	WPA_PSK = 2,
	WPA2_PSK = 3,
	WPA_WPA2_PSK = 4,
} ENUM_AP_PsdMode_TypeDef;


/******************************** ESP8266 �������Ŷ��� ***********************************/
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

/*********************************************** ESP8266 �����궨�� *******************************************/
#define     ESP8266_Usart( fmt, ... )           ESP8266_USART_Printf ( ESP8266_USARTx, fmt, ##__VA_ARGS__ ) 
#define     PC_Usart( fmt, ... )                printf ( fmt, ##__VA_ARGS__ )
//#define     macPC_Usart( fmt, ... )                

//CHIP_EN �ߵ�ƽ��Ч ESP8266��������
#define     ESP8266_CHIP_EN_ENABLE()                 GPIO_SetBits ( ESP8266_CHIP_EN_PORT, ESP8266_CHIP_EN_PIN )
#define     ESP8266_CHIP_EN_DISABLE()                GPIO_ResetBits ( ESP8266_CHIP_EN_PORT, ESP8266_CHIP_EN_PIN )

//EX_RSTB  �ⲿ reset �ź�(�͵�ƽ��Ч)
#define     ESP8266_RST_HIGH_LEVEL()            GPIO_SetBits ( ESP8266_RST_PORT, ESP8266_RST_PIN )
#define     ESP8266_RST_LOW_LEVEL()             GPIO_ResetBits ( ESP8266_RST_PORT, ESP8266_RST_PIN )


/* �������� --------------------------------------------------------------------*/
void ESP8266_Init( void );//ESP8266��ʼ������
void ESP8266_RST( void );//����WF-ESP8266ģ��
void ESP8266_USART_Printf( USART_TypeDef * USARTx, char * Data, ... );//��ʽ�����
bool ESP8266_AT_Cmd( char * cmd, char * reply1, char * reply2, u32 waittime );//��WF-ESP8266ģ�鷢��ATָ��
void ESP8266_AT_Test( void );//��WF-ESP8266ģ�����AT��������
bool ESP8266_Work_Mode_Choose(ESP8266_Work_ModeTypeDef  ESP8266_WorkMode );//ѡ��WF-ESP8266ģ��Ĺ���ģʽ
bool ESP8266_Join_WIFI(char * pSSID, char * pPassWord );//WF-ESP8266ģ�������ⲿWiFi
bool ESP8266_Build_WIFI( char * pSSID, char * pPassWord, ENUM_AP_PsdMode_TypeDef ESP8266_ENUM_AP_PsdMode );//WF-ESP8266ģ�鴴��WiFi�ȵ�	
bool ESP8266_Enable_MultipleId ( FunctionalState enumEnUnvarnishTx );//WF-ESP8266ģ������������
bool ESP8266_Link_Server ( ENUM_NetPro_TypeDef enumE, char * ip, char * ComNum, ENUM_ID_NO_TypeDef id);//WF-ESP8266ģ�������ⲿ������
bool ESP8266_StartOrShutServer ( FunctionalState enumMode, char * pPortNum, char * pTimeOver );//WF-ESP8266ģ�鿪����رշ�����ģʽ
uint8_t ESP8266_Get_LinkStatus ( void );//��ȡ WF-ESP8266 ������״̬�����ʺϵ��˿�ʱʹ��
uint8_t ESP8266_Get_IdLinkStatus ( void );//��ȡ WF-ESP8266 �Ķ˿ڣ�Id������״̬�����ʺ϶�˿�ʱʹ��
uint8_t ESP8266_Inquire_ApIp ( char * pApIp, uint8_t ucArrayLength );//��ȡ F-ESP8266 �� AP IP
bool ESP8266_UnvarnishSend ( void );//����WF-ESP8266ģ�����͸������
void ESP8266_ExitUnvarnishSend ( void );//����WF-ESP8266ģ���˳�͸��ģʽ
bool ESP8266_SendString ( FunctionalState enumEnUnvarnishTx, char * pStr, u32 ulStrLength, ENUM_ID_NO_TypeDef ucId );//WF-ESP8266ģ�鷢���ַ���
char * ESP8266_ReceiveString ( FunctionalState enumEnUnvarnishTx );//WF-ESP8266ģ������ַ���
uint8_t ESP8266_CWLIF ( char * pStaIp );//��ѯ�ѽ����豸��IP
uint8_t ESP8266_CIPAP ( char * pApIp );//����ģ��� AP IP


#endif /* __ESP8266_H */
/* END OF FILE ------------------------------------------------------------------*/

