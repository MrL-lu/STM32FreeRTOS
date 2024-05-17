/**
  ******************************************************************************
  * @file     ����������      ccs811.h
  * @author ���������ߣ�      Lu
  * @version������汾��      V1.0
  * @date   ���޸����ڣ�      2020-10-08
  * @brief  �����ܼ�飩      STM32F103VET6 ��ģ��
  * @parameter  ��������	  	��
  * @return   ������ֵ��	  	��
  ******************************************************************************
  * @attention ��ע�����
  * оƬ : STM32F103VET6
  * CCS811 �������������� �����CO2Ũ�ȡ��ܻӷ����л��ﺬ����
  * ͨѶЭ�飺���ڣ�ģ�飩 IIC
  *
  *
  ******************************************************************************
  */ 
/*------------------------------------------------------------------------------*/ 
#ifndef __CCS811_H
#define __CCS811_H

/* �û�ͷ�ļ� ------------------------------------------------------------------*/
#include "stm32f10x.h"//����stm32f10xͷ�ļ�
#include "stdio.h"	
#include "string.h"

#include "./SysTick/SysTick.h"  //���� ϵͳ��ʱ�� ͷ�ļ�
#include "./USART/usart.h"  //���� ����ͨѶ ͷ�ļ�

/* �궨�� ----------------------------------------------------------------------*/
//���ڲ�����
#define 	CCS811_USART_BAUDRATE             115200

// ����1-USART1
#define  	CCS811_USART      	              USART2
#define  	CCS811_USART_CLK_1            	  RCC_APB2Periph_USART1
#define  	CCS811_USART_APBxClkCmd_1   	    RCC_APB2PeriphClockCmd


// USART GPIO ���ź궨��
#define  	CCS811_USART1_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  	CCS811_USART1_GPIO_APBxClkCmd  	  RCC_APB2PeriphClockCmd
//TXD    
#define  	CCS811_USART1_TX_GPIO_PORT      	GPIOA   
#define  	CCS811_USART1_TX_GPIO_PIN        	GPIO_Pin_2
//RXD
#define  	CCS811_USART1_RX_GPIO_PORT    	  GPIOA
#define  	CCS811_USART1_RX_GPIO_PIN      	  GPIO_Pin_3
// USART �жϺ궨��
#define  	CCS811_USART_IRQ               	  USART2_IRQn
#define  	CCS811_USART_IRQHandler						USART2_IRQHandler


/* �������� --------------------------------------------------------------------*/
void CCS811_USART_GPIO_Config( void );//CCS811 ���������� ����GPIO���ú���
void CSS811_USART_Send(uint8_t *data,uint8_t len);//CCS811 ���������� ���ڷ��ͺ���
void CCS811_DATA_Analysis(unsigned char flag);//CCS811 ���ݽ�������
void CCS811_Check(unsigned char* flag);//CCS811 ��������⺯��
void CCS811_AT_Send_Init( void );//CCS811 ����������ATָ���ʼ�����ú���
void CCS811_USART_IRQHandler( void );//CCS811 ���������� �����жϺ���
#endif /* __CCS811_H */
/* END OF FILE ------------------------------------------------------------------*/

