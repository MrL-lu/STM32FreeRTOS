/**
  ******************************************************************************
  * @file     ����������      oled12864.h
  * @author ���������ߣ�      Lu
  * @version������汾��      V1.0
  * @date   ���޸����ڣ�      2022-2-15
  * @brief  �����ܼ�飩      STM32F103VET6�̼���ģ��
  * @parameter  ��������	    ��
  * @return   ������ֵ��	    ��
  ******************************************************************************
  * @attention ��ע�����
  * оƬ�ͺ� : STM32F103VET6
  * ���Ŷ˿� :	SCL->PB6  SDA ->PB7
  * �޸����� : 
  * ʵ�ֹ��� : 
	* ��ʱ��ʾ :	
  *	
	* OLED���Դ�
  * ��Ÿ�ʽ����.
  * [0]0 1 2 3 ... 127	
  * [1]0 1 2 3 ... 127	
  * [2]0 1 2 3 ... 127	
  * [3]0 1 2 3 ... 127	
  * [4]0 1 2 3 ... 127	
  * [5]0 1 2 3 ... 127	
  * [6]0 1 2 3 ... 127	
  * [7]0 1 2 3 ... 127 
  *
  * OLEDģ���ַ��0x78 	
  * ���0x00
  * ���ݣ�0x40	
  ******************************************************************************
  */
/*------------------------------------------------------------------------------*/

#ifndef __OLED12864_H
#define __OLED12864_H
/* �û�ͷ�ļ� ------------------------------------------------------------------*/
#include	<stm32f10x_conf.h>
#include	"main.h"
#include "stdlib.h"	

/* �궨�� ----------------------------------------------------------------------*/
#define		OLED_Address							0x78 //ͨ������0R����,������0x78��0x7A������ַ -- Ĭ��0x78

//ʱ����
#define			OLED_SCL_GPIO_RCC_APB2						RCC_APB2Periph_GPIOB	/* GPIO�˿�ʱ�� */
#define			OLED_SCL_GPIOX										GPIOB									/* GPIO�˿� */
#define			OLED_SCL_GPIO_Pin									GPIO_Pin_6						/* GPIO���� */

//������
#define			OLED_SDA_GPIO_RCC_APB2						RCC_APB2Periph_GPIOB	/* GPIO�˿�ʱ�� */
#define			OLED_SDA_GPIOX										GPIOB									/* GPIO�˿� */
#define			OLED_SDA_GPIO_Pin									GPIO_Pin_7						/* GPIO���� */


#define			OLED_SCL_Low											GPIO_ResetBits(OLED_SCL_GPIOX,OLED_SCL_GPIO_Pin)
#define			OLED_SCL_High											GPIO_SetBits(OLED_SCL_GPIOX,OLED_SCL_GPIO_Pin)

#define			OLED_SDA_Low											GPIO_ResetBits(OLED_SDA_GPIOX,OLED_SDA_GPIO_Pin)
#define			OLED_SDA_High											GPIO_SetBits(OLED_SDA_GPIOX,OLED_SDA_GPIO_Pin)

/* �������� --------------------------------------------------------------------*/
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
