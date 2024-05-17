/**
  ******************************************************************************
  * @file     ����������	  	bsp_motor.h
  * @author ���������ߣ�		Lu
  * @version������汾��		V1.0.0.0
  * @date   ���޸����ڣ�		2024-04-11
  * @brief  �����ܼ�飩		STM32F103VET6�̼���ģ��
  * @parameter  ��������		��
  * @return   ������ֵ��		��
  ******************************************************************************
  * @attention ��ע�����
  * оƬ�ͺţ�STM32F103VET6
  * ʹ�����ţ�PA6-> IN1 PA7-> IN2 PB0-> IN3 PB1-> IN4
  * �޸����ڣ�
  * ʵ�ֹ��ܣ�
  *	
  ******************************************************************************
  */
/*------------------------------------------------------------------------------*/

#ifndef __BSP_MOTOR_H
#define __BSP_MOTOR_H

/* �û�ͷ�ļ� ------------------------------------------------------------------*/
#include "stm32f10x.h"   //����stm32ͷ�ļ�
//#include "main.h"

/* FreeRTOSͷ�ļ� */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/*
//����L298N�������оƬ
IN1 & IN2 ��ӦA��
IN3 & IN4 ��ӦB��

    | IN1 | IN2 | IN3 | IN4 | M1 | M2 |
    |  1  |  0  |  /  |  /  | �� |  / |
    |  0  |  1  |  /  |  /  | �� |  / |
    |  0  |  1  |  /  |  /  | ͣ |  / |
    |  /  |  /  |  1  |  0  | / |  �� |
    |  /  |  /  |  0  |  1  | / |  �� |
    |  /  |  /  |  0  |  0  | / |  ͣ |

*/


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
typedef enum
{
  BSP_MOTOR_FWD = 0,    //��ת
  BSP_MOTOR_REV,        //��ת
}bsp_motor_dir_t;

///* ����ٶȿ���ö�� */
//typedef enum
//{
//  BSP_MOTOR_Freq = 10,  //���PWM���Ƶ��
//  BSP_MOTOR_Duty = 0,   //���PWM��ռ�ձ�
//}bsp_motor_speed;

//typedef struct
//{
//    bsp_motor_dir_t BSP_MOTOR_DIR;      //�������
//    bsp_motor_speed BSP_MOTOR_SPEED;    //����ٶ�
//    
//}BSP_MOTOR_InitTypeDef; //������Խṹ��

/* �궨�� ----------------------------------------------------------------------*/
#define            BSP_MOTOR_Freq                       10  //10KHz
#define            BSP_MOTOR_Duty                       0  //��ռ�ձ�

/************ͨ�ö�ʱ��TIM�������壬ֻ��TIM2��3��4��5************/
// ��ʹ�ò�ͬ�Ķ�ʱ����ʱ�򣬶�Ӧ��GPIO�ǲ�һ���ģ����Ҫע��
// ��������Ĭ��ʹ��TIM3
#define            BSP_MOTOR_TIM                        TIM4
#define            BSP_MOTOR_TIM_APBxClock_FUN          RCC_APB1PeriphClockCmd
#define            BSP_MOTOR_TIM_CLK                    RCC_APB1Periph_TIM4
//#define            BSP_MOTOR_TIM_Period               9
#define            BSP_MOTOR_TIM_Prescaler              71
// TIM3 ����Ƚ�ͨ��1 --IN1
#define            BSP_MOTOR_TIM_CH1_GPIO_CLK           RCC_APB2Periph_GPIOB
#define            BSP_MOTOR_TIM_CH1_PORT               GPIOB
#define            BSP_MOTOR_TIM_CH1_PIN                GPIO_Pin_6
// TIM3 ����Ƚ�ͨ��2 --IN2
#define            BSP_MOTOR_TIM_CH2_GPIO_CLK           RCC_APB2Periph_GPIOB
#define            BSP_MOTOR_TIM_CH2_PORT               GPIOB
#define            BSP_MOTOR_TIM_CH2_PIN                GPIO_Pin_7
// TIM3 ����Ƚ�ͨ��3 --IN3
#define            BSP_MOTOR_TIM_CH3_GPIO_CLK           RCC_APB2Periph_GPIOB
#define            BSP_MOTOR_TIM_CH3_PORT               GPIOB
#define            BSP_MOTOR_TIM_CH3_PIN                GPIO_Pin_8
// TIM3 ����Ƚ�ͨ��4 --IN4
#define            BSP_MOTOR_TIM_CH4_GPIO_CLK           RCC_APB2Periph_GPIOB
#define            BSP_MOTOR_TIM_CH4_PORT               GPIOB
#define            BSP_MOTOR_TIM_CH4_PIN                GPIO_Pin_9

/* �������� --------------------------------------------------------------------*/
void BSP_MOTOR_GPIO_Config(void);
void BSP_MOTOR_PWM_SET(TIM_TypeDef* timer,uint8_t tim_channel,uint16_t frequency,uint16_t Duty_cycle);
void BSP_MOTOR_STOP(TIM_TypeDef* timer);

#endif  /*  __BSP_MOTOR_H */

/* END OF FILE ------------------------------------------------------------------*/
