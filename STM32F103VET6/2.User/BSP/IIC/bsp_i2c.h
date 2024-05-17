/**
  ******************************************************************************
  * @file     （程序名）	  	bsp_i2c.h
  * @author （程序作者）		Lu
  * @version（程序版本）		V1.0.0.0
  * @date   （修改日期）		2024-03-22
  * @brief  （功能简介）		STM32F103VET6固件库模板
  * @parameter  （参数）		无
  * @return   （返回值）		无
  ******************************************************************************
  * @attention （注意事项）
  * 芯片型号：STM32F103VET6
  * 使用引脚：SCL ->PB6      SDA->PB7
  * 修改日期：
  * 实现功能：
  *	
  ******************************************************************************
  */
/*------------------------------------------------------------------------------*/

#ifndef __BSP_I2C_H
#define __BSP_I2C_H
/* 用户头文件 ------------------------------------------------------------------*/
#ifdef      FreeRTOS_OS
/* FreeRTOS头文件 */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "main.h"

#else

#include "stm32f10x.h"   //引用stm32头文件
#include "main.h"

#endif  //FreeRTOS_OS

/* 宏定义 ----------------------------------------------------------------------*/
#define             BSP_I2Cx                                I2C1                        //IIC设备
#define             BSP_I2C_APBxClock_FUN                   RCC_APB1PeriphClockCmd      //IIC时钟
#define             BSP_I2C_CLK                             RCC_APB1Periph_I2C1         //IIC时钟
#define             BSP_I2C_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd      //对应GPIO时钟
#define             BSP_I2C_GPIO_CLK                        RCC_APB2Periph_GPIOB        //对应GPIO时钟端口
#define             BSP_I2C_SCL_PORT                        GPIOB                       //对应SCL的GPIO端口
#define             BSP_I2C_SCL_PIN                         GPIO_Pin_6                  //对应SCL的GPIO引脚
#define             BSP_I2C_SDA_PORT                        GPIOB                       //对应SDA的GPIO端口
#define             BSP_I2C_SDA_PIN                         GPIO_Pin_7                  //对应SDA的GPIO引脚

#define             BSP_I2C_Speed                           400000                      //400KHz
#define             BSP_I2C_OWN_ADDRESS7                    0X0A                        //MCU器件地址，这个地址只要与STM32外挂的I2C器件地址不一样即可

/*等待超时时间*/
#define             BSP_I2CT_FLAG_TIMEOUT                   ((uint32_t)0x1000)
#define             BSP_I2CT_LONG_TIMEOUT                   ((uint32_t)(10 * BSP_I2CT_FLAG_TIMEOUT))

/*信息输出*/
//#define             IIC_DEBUG_ON                            1

//#define             IIC_INFO(fmt,arg...)                    printf("<<-IIC-INFO->> "fmt"\n",##arg)
//#define             IIC_ERROR(fmt,arg...)                   printf("<<-IIC-ERROR->> "fmt"\n",##arg)
//#define             IIC_DEBUG(fmt,arg...)                   do{\
//                                                                if(MPU_DEBUG_ON)\
//                                                                    printf("<<-MPU-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
//                                                                }while(0)

/* 函数声明 --------------------------------------------------------------------*/
static void BSP_IIC_Config(void);	//IIC初始化函数
static void BSP_IIC_Mode_Config(void);  //I2C 工作模式配置
void BSP_IIC_Init(void);    //I2C 初始化
static  uint8_t I2C_TIMEOUT_UserCallback(void); //超时错误
uint8_t BSP_IIC_WritByte(uint8_t Slave_ADDRESS,uint8_t WriteAddr,uint8_t pBuffer); //I2C 写一个字节
uint8_t BSP_IIC_ReadByte(u8 ReadAddr,u8* pBuffer, u16 NumByteToRead);   //I2C 读一个字节
void I2C_WaitStandbyState(uint8_t SlaveAddress);    //等待从属待机状态

#endif  /*  __BSP_I2C_H */

/* END OF FILE ------------------------------------------------------------------*/

