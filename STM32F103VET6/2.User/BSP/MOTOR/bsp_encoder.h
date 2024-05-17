/**
  ******************************************************************************
  * @file     （程序名）	  	bsp_encoder.h 编码器驱动程序
  * @author （程序作者）		Lu
  * @version（程序版本）		V1.0.0.0
  * @date   （修改日期）		2024-04-12
  * @brief  （功能简介）		STM32F103VET6固件库模板
  * @parameter  （参数）		无
  * @return   （返回值）		无
  ******************************************************************************
  * @attention （注意事项）
  * 芯片型号：STM32F103VET6
  * 使用引脚： PA6 ->A端   PA7 ->B端
  * 修改日期：
  * 实现功能：
  * V=S/T  S：路程-根据轮子的周长 T：设定定时器的时间
  *	
  ******************************************************************************
  */
/*------------------------------------------------------------------------------*/

#ifndef __BSP_ENCODER_H
#define __BSP_ENCODER_H
/* 用户头文件 ------------------------------------------------------------------*/
#include "stm32f10x.h"   //引用stm32头文件
#include "main.h"

/* FreeRTOS头文件 */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* 宏定义 ----------------------------------------------------------------------*/
/*
编码器电机线序
红：电机电源+（和白色对调实现正反转）
黑：编码器电源-（3.3V/5V）
黄：信号线 编码器输出A相
绿：信号线 编码器输出B相
蓝：编码器电源+（3.3V/5V）
白：电机电源-（和红色对调实现正反转）
*/

/* 电机方向控制枚举 */
typedef struct
{
    uint8_t BSP_MOTOR_DIR;      //方向
    float BSP_MOTOR_Speed;    //速度
}BSP_MOTOR_InitTypeDef; //电机属性结构体

// 当使用不同的定时器的时候，对应的GPIO是不一样的，这点要注意
// 我们这里默认使用TIM3
#define		BSP_ENCODER_TIM					        TIM3						    /* 编码器 对应的定时器 */
#define     BSP_ENCODER_TIM_CLK                     RCC_APB1Periph_TIM3
#define     BSP_ENCODER_TIM_APBxClock_FUN           RCC_APB1PeriphClockCmd          /* 编码器 对应的定时器 */
#define     BSP_ENCODER_TIM_PSC                     (1-1)                           /* 编码器 对应的定时器分频  不分频*/
#define     BSP_ENCODER_TIM_PERIOD                  65535                           /* 编码器 对应的定时器最大计数数值65535 */
//#define     BSP_ENCODER_TIM_PERIOD                  (50000 - 1)                       /* 编码器 对应的定时器周期数 50ms */


//编码器参数和电机参数宏定义
#define     BSP_ENCODER_Num                         0                               /* 编码器数值 */
#define     BSP_ENCODER_FREQUENCY                   50                              /* 编码器读取频率 1s周期内读取次数 */
#define     BSP_ENCODER_Reduction_Ratio             330                             /* 电机编码器减速比 */
#define     BSP_ENCODER_Tire_Length                 65                              /* 轮胎长度 周长 */
// 测量的起始边沿
#define     BSP_ENCODER_TIM_STRAT_ICPolarity        TIM_ICPolarity_Rising
// 测量的结束边沿
#define     BSP_ENCODER_TIM_END_ICPolarity          TIM_ICPolarity_Falling


//A端
#define     BSP_ENCODER_A_GPIO_PORT				    GPIOA			                /* GPIO端口 */
#define     BSP_ENCODER_A_GPIO_CLK				    RCC_APB2Periph_GPIOA		    /* GPIO端口时钟 */
#define     BSP_ENCODER_A_GPIO_PIN				    GPIO_Pin_6			            /* 连接到SCL时钟线的GPIO */
#define     BSP_ENCODER_TIM_CHANNEL_A               TIM_Channel_1


//B端
#define     BSP_ENCODER_B_GPIO_PORT				    GPIOA			                /* GPIO端口 */
#define     BSP_ENCODER_B_GPIO_CLK				    RCC_APB2Periph_GPIOA		    /* GPIO端口时钟 */
#define     BSP_ENCODER_B_GPIO_PIN				    GPIO_Pin_7			            /* 连接到SCL时钟线的GPIO */
#define     BSP_ENCODER_TIM_CHANNEL_B               TIM_Channel_2


// 中断相关宏定义
#define     BSP_ENCODER_TIM_IT_CCx                  TIM_IT_CC1
#define     BSP_ENCODER_TIM_IRQ                     TIM3_IRQn
#define     BSP_ENCODER_TIM_INT_FUN                 TIM3_IRQHandler

/* 函数声明 --------------------------------------------------------------------*/
static void BSP_ENCODER_GPIO_Config(void);
static void GENERAL_TIM_NVIC_Config(void);
static void BSP_ENCODER_TIM_Config(void);
void BSP_ENCODER_Init(void);
uint16_t BSP_ENCODER_Read_CNT(TIM_TypeDef* BSP_ENCODER_TIMX);
float BSP_ENCODER_Read_CNT_Speed(TIM_TypeDef* BSP_ENCODER_TIMX);

#endif  /*  __BSP_ENCODER_H */

/* END OF FILE ------------------------------------------------------------------*/

