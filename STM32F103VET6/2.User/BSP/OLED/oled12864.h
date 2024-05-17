/**
  ******************************************************************************
  * @file     （程序名）      oled12864.h
  * @author （程序作者）      Lu
  * @version（程序版本）      V1.0
  * @date   （修改日期）      2022-2-15
  * @brief  （功能简介）      STM32F103VET6固件库模板
  * @parameter  （参数）	    无
  * @return   （返回值）	    无
  ******************************************************************************
  * @attention （注意事项）
  * 芯片型号 : STM32F103VET6
  * 引脚端口 :	SCL->PB6  SDA ->PB7
  * 修改日期 : 
  * 实现功能 : 
	* 延时显示 :	
  *	
	* OLED的显存
  * 存放格式如下.
  * [0]0 1 2 3 ... 127	
  * [1]0 1 2 3 ... 127	
  * [2]0 1 2 3 ... 127	
  * [3]0 1 2 3 ... 127	
  * [4]0 1 2 3 ... 127	
  * [5]0 1 2 3 ... 127	
  * [6]0 1 2 3 ... 127	
  * [7]0 1 2 3 ... 127 
  *
  * OLED模快地址：0x78 	
  * 命令：0x00
  * 数据：0x40	
  ******************************************************************************
  */
/*------------------------------------------------------------------------------*/

#ifndef __OLED12864_H
#define __OLED12864_H
/* 用户头文件 ------------------------------------------------------------------*/
#include	<stm32f10x_conf.h>
#include	"main.h"
#include "stdlib.h"	

/* 宏定义 ----------------------------------------------------------------------*/
#define		OLED_Address							0x78 //通过调整0R电阻,屏可以0x78和0x7A两个地址 -- 默认0x78

//时钟线
#define			OLED_SCL_GPIO_RCC_APB2						RCC_APB2Periph_GPIOB	/* GPIO端口时钟 */
#define			OLED_SCL_GPIOX										GPIOB									/* GPIO端口 */
#define			OLED_SCL_GPIO_Pin									GPIO_Pin_6						/* GPIO引脚 */

//数据线
#define			OLED_SDA_GPIO_RCC_APB2						RCC_APB2Periph_GPIOB	/* GPIO端口时钟 */
#define			OLED_SDA_GPIOX										GPIOB									/* GPIO端口 */
#define			OLED_SDA_GPIO_Pin									GPIO_Pin_7						/* GPIO引脚 */


#define			OLED_SCL_Low											GPIO_ResetBits(OLED_SCL_GPIOX,OLED_SCL_GPIO_Pin)
#define			OLED_SCL_High											GPIO_SetBits(OLED_SCL_GPIOX,OLED_SCL_GPIO_Pin)

#define			OLED_SDA_Low											GPIO_ResetBits(OLED_SDA_GPIOX,OLED_SDA_GPIO_Pin)
#define			OLED_SDA_High											GPIO_SetBits(OLED_SDA_GPIOX,OLED_SDA_GPIO_Pin)

/* 函数声明 --------------------------------------------------------------------*/
void  OLED_IIC_delay(void);
void	OLED_IIC_Start(void);
void	OLED_IIC_Stop(void);
void	OLED_IIC_WaitACK(void);
void	OLED_IIC_WriteByte(uint8_t data);
void	OLED_Write_CMD(uint8_t data);
void	OLED_Write_Data(uint8_t data);
void  OLED_IIC_Display_ON(void);
void  OLED_IIC_Display_OFF(void);
void	OLED_IIC_Refresh(void);
void	OLED_Clear(void);
void	OLED_IIC_Init(void);
void 	OLED_ColorTurn(uint8_t i);
void  OLED_DisplayTurn(uint8_t i);



void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1,u8 mode);
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1,u8 mode);
u32 OLED_Pow(u8 m,u8 n);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1,u8 mode);
void OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1,u8 mode);
void OLED_ScrollDisplay(u8 num,u8 space,u8 mode);
void OLED_ShowPicture(u8 x,u8 y,u8 sizex,u8 sizey,u8 BMP[],u8 mode);

#endif  /*  __OLED12864_H */

/* END OF FILE ------------------------------------------------------------------*/
