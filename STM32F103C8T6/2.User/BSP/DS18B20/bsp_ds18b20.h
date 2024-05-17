/**
  ******************************************************************************
  * @file     （程序名）	  	BSP_DS18B20.h
  * @author （程序作者）		Lu
  * @version（程序版本）		V1.0.0.0
  * @date   （修改日期）		2024-03-12
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

#ifndef __BSP_DS18B20_H
#define __BSP_DS18B20_H
/* 用户头文件 ------------------------------------------------------------------*/
#include "stm32f10x.h"   //引用stm32头文件

/* FreeRTOS头文件 */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* 宏定义 ----------------------------------------------------------------------*/
#define     BSP_DS18B20_DQ_SCK_APBxClock_FUN        RCC_APB2PeriphClockCmd          /* GPIO端口对应的APB */
#define     BSP_DS18B20_GPIO_PORT				    GPIOE			                /* GPIO端口 */
#define     BSP_DS18B20_GPIO_CLK				    RCC_APB2Periph_GPIOE		    /* GPIO端口时钟 */
#define     BSP_DS18B20_GPIO_PIN				    GPIO_Pin_6			            /* 连接到SCL时钟线的GPIO */

#define     BSP_DS18B20_Low                         GPIO_ResetBits(BSP_DS18B20_GPIO_PORT,BSP_DS18B20_GPIO_PIN)
#define     BSP_DS18B20_High                        GPIO_SetBits(BSP_DS18B20_GPIO_PORT,BSP_DS18B20_GPIO_PIN)
#define     BSP_DS18B20_IN()                        GPIO_ReadInputDataBit(BSP_DS18B20_GPIO_PORT, BSP_DS18B20_GPIO_PIN)                          

/* 函数声明 --------------------------------------------------------------------*/
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

