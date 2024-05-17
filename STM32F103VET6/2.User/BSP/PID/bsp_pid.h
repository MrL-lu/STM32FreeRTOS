/**
  ******************************************************************************
  * @file     ����������	  	bsp_pid.h
  * @author ���������ߣ�		Lu
  * @version������汾��		V1.0.0.0
  * @date   ���޸����ڣ�		2024-04-16
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

#ifndef __BSP_PID_H
#define __BSP_PID_H
/* �û�ͷ�ļ� ------------------------------------------------------------------*/
#include "stm32f10x.h"   //����stm32ͷ�ļ�
//#include "main.h"

#ifdef      FreeRTOS_OS
/* FreeRTOSͷ�ļ� */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>

#else

#include "stm32f10x.h"   //����stm32ͷ�ļ�
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#endif  //FreeRTOS_OS

/* �궨�� ----------------------------------------------------------------------*/
typedef struct
{
    float target_val;               //Ŀ��ֵ
    float actual_val;        		//ʵ��ֵ
    float err;             			//����ƫ��ֵ
    float err_last;          		//������һ��ƫ��ֵ
    float Kp,Ki,Kd;          		//������������֡�΢��ϵ��
    float integral;          		//�������ֵ
}bsp_pid;

/* �������� --------------------------------------------------------------------*/
void BSP_PID_init();//PID������ʼ��
void BSP_Set_PID_Target(float temp_val);//
float get_pid_target(void);
void set_p_i_d(float p, float i, float d);
float PID_realize(float temp_val);
void time_period_fun();

#endif  /*  __BSP_PID_H */

/* END OF FILE ------------------------------------------------------------------*/
