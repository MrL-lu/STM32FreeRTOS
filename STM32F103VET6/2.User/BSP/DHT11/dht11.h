/**
  ******************************************************************************
  * @file     （程序名）      dht11.h
  * @author （程序作者）      Lu
  * @version（程序版本）      V1.0
  * @date   （修改日期）      2022-2-15
  * @brief  （功能简介）      STM32F103VET6固件库模板
  * @parameter  （参数）	    无
  * @return   （返回值）	    无
  ******************************************************************************
  * @attention （注意事项）
  * 芯片型号 : STM32F103VET6
  * 引脚端口 : PE6
  * 修改日期 : 
  * 实现功能 : 
  *	
  *	数据格式: 8bit 湿度整数数据 + 8bit 湿度小数数据 + 8bit 温度整数数据 + 8bit 温度小数数据 + 8bit 校验位。注：其中湿度小数部分为 0。 
  *	校验位数据定义 “8bit 湿度整数数据 + 8bit 湿度小数数据 + 8bit 温度整数数据 + 8bit 温度小数数据”8bit 校验位等于所得结果的末 8 位。
  ******************************************************************************
  */
/*------------------------------------------------------------------------------*/

#ifndef __DHT11_H
#define __DHT11_H
/* 用户头文件 ------------------------------------------------------------------*/
#include	<stm32f10x_conf.h>
#include	"main.h"


/************************** DHT11 数据类型定义********************************/
typedef struct
{
	uint8_t  humi_int;		//湿度的整数部分
	uint8_t  humi_deci;	 	//湿度的小数部分
	uint8_t  temp_int;	 	//温度的整数部分
	uint8_t  temp_deci;	 	//温度的小数部分
	uint8_t  check_sum;	 	//校验和
		                 
} DHT11_Data_TypeDef;


/* 宏定义 ----------------------------------------------------------------------*/
#define			DHT11_GPIO_RCC_APB2							RCC_APB2Periph_GPIOE	/* GPIO端口时钟 */
#define			DHT11_GPIOX											GPIOE									/* GPIO端口 */
#define			DHT11_GPIO_Pin									GPIO_Pin_6						/* GPIO引脚 */


/* 函数声明 --------------------------------------------------------------------*/
void DHT11_Output( void );	//DHT11引脚的输出类型为推挽输出函数
void DHT11_Input( void );	//DHT11引脚的输出类型为浮空输入函数
static uint8_t DHT11_ReadByte( void );	//DHT11读取一个字节
uint8_t DHT11_Read_TempAndHumidity(DHT11_Data_TypeDef *DHT11_Data);	//DHT11读取一次数据

#endif  /*  __DHT11_H */

/* END OF FILE ------------------------------------------------------------------*/

