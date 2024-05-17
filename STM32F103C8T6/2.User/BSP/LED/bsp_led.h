/**
  ******************************************************************************
  * @file     （程序名）	  	main.h
  * @author （程序作者）		Lu
  * @version（程序版本）		V1.0.0.0
  * @date   （修改日期）		2024-02-19
  * @brief  （功能简介）		STM32F103VET6固件库模板
  * @parameter  （参数）		无
  * @return   （返回值）		无
  ******************************************************************************
  * @attention （注意事项）
  * 芯片型号：  STM32F103VET6
  * 使用引脚：	LED_R -> PB5	LED_G -> PB0	LED_B -> PB1
  * 修改日期： 
  * 实现功能：
  *	
  ******************************************************************************
  */
/*------------------------------------------------------------------------------*/

#ifndef __BSP_LED_H
#define __BSP_LED_H
/* 用户头文件 ------------------------------------------------------------------*/
#include "stm32f10x.h"   //引用stm32头文件

/* 宏定义 ----------------------------------------------------------------------*/
//红色LED
#define     LED_Red_GPIO_PORT				GPIOC			                /* GPIO端口 */
#define     LED_Red_GPIO_CLK				RCC_APB2Periph_GPIOC		    /* GPIO端口时钟 */
#define     LED_Red_GPIO_PIN				GPIO_Pin_13			            /* 连接到SCL时钟线的GPIO */

//绿色LED
#define     LED_Green_GPIO_PORT				GPIOB			                /* GPIO端口 */
#define     LED_Green_GPIO_CLK				RCC_APB2Periph_GPIOB		    /* GPIO端口时钟 */
#define     LED_Green_GPIO_PIN				GPIO_Pin_0			            /* 连接到SCL时钟线的GPIO */

//蓝色LED
#define     LED_Blue_GPIO_PORT				GPIOB			                /* GPIO端口 */
#define     LED_Blue_GPIO_CLK				RCC_APB2Periph_GPIOB		    /* GPIO端口时钟 */
#define     LED_Blue_GPIO_PIN				GPIO_Pin_1			            /* 连接到SCL时钟线的GPIO */

/* 直接操作寄存器的方法控制IO */
#define	    digitalHi(p,i)		    {p->BSRR=i;}	 //输出为高电平		
#define     digitalLo(p,i)		    {p->BRR=i;}	 //输出低电平
#define     digitalToggle(p,i)      {p->ODR ^=i;} //输出反转状态

/* 定义控制IO的宏 */
#define     LED_Red_TOGGLE		 	digitalToggle(LED_Red_GPIO_PORT,LED_Red_GPIO_PIN)
#define     LED_Red_OFF		   		digitalHi(LED_Red_GPIO_PORT,LED_Red_GPIO_PIN)
#define     LED_Red_ON			   	digitalLo(LED_Red_GPIO_PORT,LED_Red_GPIO_PIN)

#define     LED_Green_TOGGLE		digitalToggle(LED_Green_GPIO_PORT,LED_Green_GPIO_PIN)
#define     LED_Green_OFF		   	digitalHi(LED_Green_GPIO_PORT,LED_Green_GPIO_PIN)
#define     LED_Green_ON			digitalLo(LED_Green_GPIO_PORT,LED_Green_GPIO_PIN)

#define     LED_Blue_TOGGLE		 	digitalToggle(LED_Blue_GPIO_PORT,LED_Blue_GPIO_PIN)
#define     LED_Blue_OFF		   	digitalHi(LED_Blue_GPIO_PORT,LED_Blue_GPIO_PIN)
#define     LED_Blue_ON			   	digitalLo(LED_Blue_GPIO_PORT,LED_Blue_GPIO_PIN)

/* 函数声明 --------------------------------------------------------------------*/
void BSP_LED_Config(void);	//LED初始化函数
void BSP_LED_TOGGLE(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);//LED翻转
#endif  /*  __BSP_LED_H */

/* END OF FILE ------------------------------------------------------------------*/

