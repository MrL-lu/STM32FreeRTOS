/**
  ******************************************************************************
  * @file     （程序名）	  	bsp_ws2812.h
  * @author （程序作者）		Lu
  * @version（程序版本）		V1.0.0.0
  * @date   （修改日期）		2024-03-14
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

#ifndef __BSP_WS2812_H
#define __BSP_WS2812_H
/* 用户头文件 ------------------------------------------------------------------*/
#include "stm32f10x.h"   //引用stm32头文件

//typedef struct
//{
//    uint8_t R;      //WS2812的Red值，取值为：0~255
//    uint8_t G;      //WS2812的Green值，取值为：0~255
//    uint8_t B;      //WS2812的Blue值，取值为：0~255
//    float h;        //WS2812的色调值，取值为：0~360°
//    float s;        //WS2812的饱和度值，取值为：0~1.0
//    float v;        //WS2812的亮度值，取值为：0~1.0
//} WS2812_Data_TypeDef;

/* 宏定义 ----------------------------------------------------------------------*/
#define			WS2812_LED_Number			8							/* WS2812 灯珠的数量 */
#define			WS2812_Data_Bit				24
#define			WS2812_T0H					30							/* 0编码高电平时间占1/3 */
#define			WS2812_T1H					60							/* 1编码高电平时间占2/3 */
#define			WS2812_Sendspeed			800							/* WS2812发送接收数据速度800Kbps */

#define			WS2812_GPIO_PORT			GPIOB						/* GPIO端口 */
#define			WS2812_GPIO_CLK				RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define			WS2812_GPIO_PIN				GPIO_Pin_6					/* 连接到SCL时钟线的GPIO */
#define         WS2812_TIM_CLK              RCC_APB1Periph_TIM4
#define			WS2812_TIM					TIM4						/* WS2812 对应的定时器 */
#define			WS2812_TIM_DMA				TIM_DMA_CC1					/* WS2812 对应的定时器对应的DMA通道 */
#define			WS2812_DMA_Addr				(WS2812_TIM->CCR1)			/* WS2812 DMA传输的目的地址 */

//#define			WS2812_TIM_DMA1				TIM_DMA_CC1					/* WS2812 对应的定时器对应的DMA通道 */
//#define			WS2812_TIM_DMA2			  	TIM_DMA_CC2					/* WS2812 对应的定时器对应的DMA通道 */
//#define			WS2812_TIM_DMA3				TIM_DMA_CC3					/* WS2812 对应的定时器对应的DMA通道 */
//#define			WS2812_TIM_DMA4				TIM_DMA_CC4					/* WS2812 对应的定时器对应的DMA通道 */
//#define			WS2812_DMA1_Addr		    (WS2812_TIM->CCR1)			/* WS2812 DMA传输的目的地址 */
//#define			WS2812_DMA2_Addr			(WS2812_TIM->CCR2)			/* WS2812 DMA传输的目的地址 */
//#define			WS2812_DMA3_Addr			(WS2812_TIM->CCR3)			/* WS2812 DMA传输的目的地址 */
//#define			WS2812_DMA4_Addr			(WS2812_TIM->CCR4)			/* WS2812 DMA传输的目的地址 */

#define			WS2812_DMA_Channel			DMA1_Channel1				/* WS2812 对应的DMA通道 */
#define			WS2812_DMA_Flag				DMA1_FLAG_TC1				/* WS2812 对应的DMA通道传输完成标志位 */


//#define			WS2812_GPIO_PORT			GPIOB						/* GPIO端口 */
//#define			WS2812_GPIO_CLK				RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
//#define			WS2812_GPIO_PIN				GPIO_Pin_0					/* 连接到SCL时钟线的GPIO */

//#define			WS2812_TIM					TIM3						/* WS2812 对应的定时器 */
//#define			WS2812_TIM_DMA				TIM_DMA_CC3					/* WS2812 对应的定时器对应的DMA通道 */
//#define			WS2812_DMA_Addr				(TIM3->CCR3)				/* WS2812 DMA传输的目的地址 */
//#define			WS2812_DMA_Channel			DMA1_Channel2				/* WS2812 对应的DMA通道 */
//#define			WS2812_DMA_Flag				DMA1_FLAG_TC2				/* WS2812 对应的DMA通道传输完成标志位 */

#define         WS2812Breath_Flag           1                           /* WS2812 呼吸曲线宏定义 */
#define         WS2812color_allochroism     0                           /* WS2812 七彩渐变宏定义 */

/* 函数声明 --------------------------------------------------------------------*/
void WS2812_Config(uint16_t WS2812_Byte);
static void  WS2812_Reset(void);
static void  WS2812_Display(uint8_t LED_Num);
void WS2812_Display_Color(uint8_t LED_Num,uint32_t RGB_Data);
void WS2812_Set_LED_Color(uint8_t LED_Num,uint32_t RGB_Data);
void WS2812_BreathingLight(uint16_t LED_Num,uint32_t RGB_Data);
#endif  /*  __BSP_WS2812_H */

/* END OF FILE ------------------------------------------------------------------*/
