/**
  ******************************************************************************
  * @file     ����������	  	bsp_encoder.h ��������������
  * @author ���������ߣ�		Lu
  * @version������汾��		V1.0.0.0
  * @date   ���޸����ڣ�		2024-04-12
  * @brief  �����ܼ�飩		STM32F103VET6�̼���ģ��
  * @parameter  ��������		��
  * @return   ������ֵ��		��
  ******************************************************************************
  * @attention ��ע�����
  * оƬ�ͺţ�STM32F103VET6
  * ʹ�����ţ� PA6 ->A��   PA7 ->B��
  * �޸����ڣ�
  * ʵ�ֹ��ܣ�
  * V=S/T  S��·��-�������ӵ��ܳ� T���趨��ʱ����ʱ��
  *	
  ******************************************************************************
  */
/*------------------------------------------------------------------------------*/

#ifndef __BSP_ENCODER_H
#define __BSP_ENCODER_H
/* �û�ͷ�ļ� ------------------------------------------------------------------*/
#include "stm32f10x.h"   //����stm32ͷ�ļ�
#include "main.h"

/* FreeRTOSͷ�ļ� */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* �궨�� ----------------------------------------------------------------------*/
/*
�������������
�죺�����Դ+���Ͱ�ɫ�Ե�ʵ������ת��
�ڣ���������Դ-��3.3V/5V��
�ƣ��ź��� ���������A��
�̣��ź��� ���������B��
������������Դ+��3.3V/5V��
�ף������Դ-���ͺ�ɫ�Ե�ʵ������ת��
*/

/* ����������ö�� */
typedef struct
{
    uint8_t BSP_MOTOR_DIR;      //����
    float BSP_MOTOR_Speed;    //�ٶ�
}BSP_MOTOR_InitTypeDef; //������Խṹ��

// ��ʹ�ò�ͬ�Ķ�ʱ����ʱ�򣬶�Ӧ��GPIO�ǲ�һ���ģ����Ҫע��
// ��������Ĭ��ʹ��TIM3
#define		BSP_ENCODER_TIM					        TIM3						    /* ������ ��Ӧ�Ķ�ʱ�� */
#define     BSP_ENCODER_TIM_CLK                     RCC_APB1Periph_TIM3
#define     BSP_ENCODER_TIM_APBxClock_FUN           RCC_APB1PeriphClockCmd          /* ������ ��Ӧ�Ķ�ʱ�� */
#define     BSP_ENCODER_TIM_PSC                     (1-1)                           /* ������ ��Ӧ�Ķ�ʱ����Ƶ  ����Ƶ*/
#define     BSP_ENCODER_TIM_PERIOD                  65535                           /* ������ ��Ӧ�Ķ�ʱ����������ֵ65535 */
//#define     BSP_ENCODER_TIM_PERIOD                  (50000 - 1)                       /* ������ ��Ӧ�Ķ�ʱ�������� 50ms */


//�����������͵�������궨��
#define     BSP_ENCODER_Num                         0                               /* ��������ֵ */
#define     BSP_ENCODER_FREQUENCY                   50                              /* ��������ȡƵ�� 1s�����ڶ�ȡ���� */
#define     BSP_ENCODER_Reduction_Ratio             330                             /* ������������ٱ� */
#define     BSP_ENCODER_Tire_Length                 65                              /* ��̥���� �ܳ� */
// ��������ʼ����
#define     BSP_ENCODER_TIM_STRAT_ICPolarity        TIM_ICPolarity_Rising
// �����Ľ�������
#define     BSP_ENCODER_TIM_END_ICPolarity          TIM_ICPolarity_Falling


//A��
#define     BSP_ENCODER_A_GPIO_PORT				    GPIOA			                /* GPIO�˿� */
#define     BSP_ENCODER_A_GPIO_CLK				    RCC_APB2Periph_GPIOA		    /* GPIO�˿�ʱ�� */
#define     BSP_ENCODER_A_GPIO_PIN				    GPIO_Pin_6			            /* ���ӵ�SCLʱ���ߵ�GPIO */
#define     BSP_ENCODER_TIM_CHANNEL_A               TIM_Channel_1


//B��
#define     BSP_ENCODER_B_GPIO_PORT				    GPIOA			                /* GPIO�˿� */
#define     BSP_ENCODER_B_GPIO_CLK				    RCC_APB2Periph_GPIOA		    /* GPIO�˿�ʱ�� */
#define     BSP_ENCODER_B_GPIO_PIN				    GPIO_Pin_7			            /* ���ӵ�SCLʱ���ߵ�GPIO */
#define     BSP_ENCODER_TIM_CHANNEL_B               TIM_Channel_2


// �ж���غ궨��
#define     BSP_ENCODER_TIM_IT_CCx                  TIM_IT_CC1
#define     BSP_ENCODER_TIM_IRQ                     TIM3_IRQn
#define     BSP_ENCODER_TIM_INT_FUN                 TIM3_IRQHandler

/* �������� --------------------------------------------------------------------*/
static void BSP_ENCODER_GPIO_Config(void);
static void GENERAL_TIM_NVIC_Config(void);
static void BSP_ENCODER_TIM_Config(void);
void BSP_ENCODER_Init(void);
uint16_t BSP_ENCODER_Read_CNT(TIM_TypeDef* BSP_ENCODER_TIMX);
float BSP_ENCODER_Read_CNT_Speed(TIM_TypeDef* BSP_ENCODER_TIMX);

#endif  /*  __BSP_ENCODER_H */

/* END OF FILE ------------------------------------------------------------------*/

