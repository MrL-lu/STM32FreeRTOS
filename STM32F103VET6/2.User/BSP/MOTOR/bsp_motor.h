/**
  ******************************************************************************
  * @file     （程序名）	  	bsp_motor.h
  * @author （程序作者）		Lu
  * @version（程序版本）		V1.0.0.0
  * @date   （修改日期）		2024-04-11
  * @brief  （功能简介）		STM32F103VET6固件库模板
  * @parameter  （参数）		无
  * @return   （返回值）		无
  ******************************************************************************
  * @attention （注意事项）
  * 芯片型号：STM32F103VET6
  * 使用引脚：PB6-> IN1 PB7-> IN2   PB8-> IN3 PB9-> IN4【一组电机】
  * 修改日期：
  * 实现功能：
  *	
  ******************************************************************************
  */
/*------------------------------------------------------------------------------*/

#ifndef __BSP_MOTOR_H
#define __BSP_MOTOR_H

/* 用户头文件 ------------------------------------------------------------------*/
#include "stm32f10x.h"   //引用stm32头文件
//#include "main.h"

/* FreeRTOS头文件 */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/*
//连接L298N电机驱动芯片
IN1 & IN2 对应A端
IN3 & IN4 对应B端

    | IN1 | IN2 | IN3 | IN4 | M1 | M2 |
    |  1  |  0  |  /  |  /  | 正 |  / |
    |  0  |  1  |  /  |  /  | 反 |  / |
    |  0  |  1  |  /  |  /  | 停 |  / |
    |  /  |  /  |  1  |  0  | / |  正 |
    |  /  |  /  |  0  |  1  | / |  反 |
    |  /  |  /  |  0  |  0  | / |  停 |

*/
/* 宏定义 ----------------------------------------------------------------------*/
#define            BSP_MOTOR_Freq                       1  //1KHz
#define            BSP_MOTOR_Duty                       0  //正占空比

/************通用定时器TIM参数定义，只限TIM2、3、4、5************/
// 当使用不同的定时器的时候，对应的GPIO是不一样的，这点要注意
// 我们这里默认使用TIM3
#define            BSP_MOTOR_TIM                        TIM4
#define            BSP_MOTOR_TIM_APBxClock_FUN          RCC_APB1PeriphClockCmd
#define            BSP_MOTOR_TIM_CLK                    RCC_APB1Periph_TIM4
//#define            BSP_MOTOR_TIM_Period               9
#define            BSP_MOTOR_TIM_Prescaler              71
// TIM3 输出比较通道1 --IN1
#define            BSP_MOTOR_TIM_CH1_GPIO_CLK           RCC_APB2Periph_GPIOB
#define            BSP_MOTOR_TIM_CH1_PORT               GPIOB
#define            BSP_MOTOR_TIM_CH1_PIN                GPIO_Pin_6
// TIM3 输出比较通道2 --IN2
#define            BSP_MOTOR_TIM_CH2_GPIO_CLK           RCC_APB2Periph_GPIOB
#define            BSP_MOTOR_TIM_CH2_PORT               GPIOB
#define            BSP_MOTOR_TIM_CH2_PIN                GPIO_Pin_7
// TIM3 输出比较通道3 --IN3
#define            BSP_MOTOR_TIM_CH3_GPIO_CLK           RCC_APB2Periph_GPIOB
#define            BSP_MOTOR_TIM_CH3_PORT               GPIOB
#define            BSP_MOTOR_TIM_CH3_PIN                GPIO_Pin_8
// TIM3 输出比较通道4 --IN4
#define            BSP_MOTOR_TIM_CH4_GPIO_CLK           RCC_APB2Periph_GPIOB
#define            BSP_MOTOR_TIM_CH4_PORT               GPIOB
#define            BSP_MOTOR_TIM_CH4_PIN                GPIO_Pin_9

/* 函数声明 --------------------------------------------------------------------*/
void BSP_MOTOR_GPIO_Config(void);
void BSP_MOTOR_PWM_SET(TIM_TypeDef* timer,uint8_t tim_channel,uint16_t frequency,uint16_t Duty_cycle);
void BSP_MOTOR_STOP(TIM_TypeDef* timer);
void BSP_MOTOR_Control(TIM_TypeDef* timer,uint8_t Number,uint8_t Motor_Dir,uint16_t Speed);
#endif  /*  __BSP_MOTOR_H */

/* END OF FILE ------------------------------------------------------------------*/
