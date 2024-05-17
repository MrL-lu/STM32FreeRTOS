/**
  ******************************************************************************
  * @file     ����������	  	BSP_DS18B20.h
  * @author ���������ߣ�		Lu
  * @version������汾��		V1.0.0.0
  * @date   ���޸����ڣ�		2024-03-12
  * @brief  �����ܼ�飩		STM32F103VET6�̼���ģ��
  * @parameter  ��������		��
  * @return   ������ֵ��		��
  ******************************************************************************
  * @attention ��ע�����
  * оƬ�ͺţ�STM32F103VET6
  * ʹ�����ţ�
  * �޸����ڣ�
  * ʵ�ֹ��ܣ�
  *	
  ******************************************************************************
  */
/*------------------------------------------------------------------------------*/

#ifndef __BSP_DS18B20_H
#define __BSP_DS18B20_H
/* �û�ͷ�ļ� ------------------------------------------------------------------*/
#include "stm32f10x.h"   //����stm32ͷ�ļ�

/* FreeRTOSͷ�ļ� */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* �궨�� ----------------------------------------------------------------------*/
#define     BSP_DS18B20_DQ_SCK_APBxClock_FUN        RCC_APB2PeriphClockCmd          /* GPIO�˿ڶ�Ӧ��APB */
#define     BSP_DS18B20_GPIO_PORT				    GPIOE			                /* GPIO�˿� */
#define     BSP_DS18B20_GPIO_CLK				    RCC_APB2Periph_GPIOE		    /* GPIO�˿�ʱ�� */
#define     BSP_DS18B20_GPIO_PIN				    GPIO_Pin_6			            /* ���ӵ�SCLʱ���ߵ�GPIO */

#define     BSP_DS18B20_Low                         GPIO_ResetBits(BSP_DS18B20_GPIO_PORT,BSP_DS18B20_GPIO_PIN)
#define     BSP_DS18B20_High                        GPIO_SetBits(BSP_DS18B20_GPIO_PORT,BSP_DS18B20_GPIO_PIN)
#define     BSP_DS18B20_IN()                        GPIO_ReadInputDataBit(BSP_DS18B20_GPIO_PORT, BSP_DS18B20_GPIO_PIN)                          

/* �������� --------------------------------------------------------------------*/
uint8_t BSP_DS18B20_Init(void);
static void BSP_DS18B20_GPIO_Config(void);
static void BSP_DS18B20_Reset(void);
static void BSP_DS18B20_Mode_Out_PP(void);
static void BSP_DS18B20_Mode_IPU(void);
static uint8_t BSP_DS18B20_Presence(void);
static uint8_t BSP_DS18B20_ReadBit(void);
static uint8_t BSP_DS18B20_ReadByte(void);
static void BSP_DS18B20_WriteByte(uint8_t dat);
static void BSP_DS18B20_Delay(uint16_t time);
static void BSP_DS18B20_SkipRom ( void );
static void BSP_DS18B20_MatchRom ( void );
float BSP_DS18B20_GetTemp_SkipRom ( void );
void BSP_DS18B20_ReadId ( uint8_t * ds18b20_id );
float BSP_DS18B20_GetTemp_MatchRom ( uint8_t * ds18b20_id );

#endif  /*  __BSP_DS18B20_H */

/* END OF FILE ------------------------------------------------------------------*/

