/**
  ******************************************************************************
  * @file     （程序名）	  	bsp_pid.h
  * @author （程序作者）		Lu
  * @version（程序版本）		V1.0.0.0
  * @date   （修改日期）		2024-04-16
  * @brief  （功能简介）		STM32F103VET6固件库模板
  * @parameter  （参数）		无
  * @return   （返回值）		无
  ******************************************************************************
  * @attention （注意事项）
  * 芯片型号：STM32F103VET6
  * 使用引脚：
  * 修改日期：
  * 实现功能：
  *	
  ******************************************************************************
  */
/*------------------------------------------------------------------------------*/

#ifndef __BSP_PID_H
#define __BSP_PID_H
/* 用户头文件 ------------------------------------------------------------------*/
#include "stm32f10x.h"   //引用stm32头文件
//#include "main.h"

#ifdef      FreeRTOS_OS
/* FreeRTOS头文件 */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>

#else

#include "stm32f10x.h"   //引用stm32头文件
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#endif  //FreeRTOS_OS

/* 宏定义 ----------------------------------------------------------------------*/
typedef struct
{
    float target_val;               //目标值
    float actual_val;        		//实际值
    float err;             			//定义偏差值
    float err_last;          		//定义上一个偏差值
    float Kp,Ki,Kd;          		//定义比例、积分、微分系数
    float integral;          		//定义积分值
}bsp_pid;

/* 函数声明 --------------------------------------------------------------------*/
void BSP_PID_init();//PID参数初始化
void BSP_Set_PID_Target(float temp_val);//
float get_pid_target(void);
void set_p_i_d(float p, float i, float d);
float PID_realize(float temp_val);
void time_period_fun();

#endif  /*  __BSP_PID_H */

/* END OF FILE ------------------------------------------------------------------*/
