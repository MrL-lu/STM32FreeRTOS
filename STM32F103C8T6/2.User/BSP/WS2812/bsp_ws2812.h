/**
  ******************************************************************************
  * @file     ����������	  	bsp_ws2812.h
  * @author ���������ߣ�		Lu
  * @version������汾��		V1.0.0.0
  * @date   ���޸����ڣ�		2024-03-14
  * @brief  �����ܼ�飩		STM32F103VET6�̼���ģ��
  * @parameter  ��������		��
  * @return   ������ֵ��		��
  ******************************************************************************
  * @attention ��ע�����
  * оƬ�ͺţ�STM32F103VET6
  * ʹ�����ţ�
  * �޸����ڣ�
  * ʵ�ֹ��ܣ�
  *	
  ******************************************************************************
  */
/*------------------------------------------------------------------------------*/

#ifndef __BSP_WS2812_H
#define __BSP_WS2812_H
/* �û�ͷ�ļ� ------------------------------------------------------------------*/
#include "stm32f10x.h"   //����stm32ͷ�ļ�

//typedef struct
//{
//    uint8_t R;      //WS2812��Redֵ��ȡֵΪ��0~255
//    uint8_t G;      //WS2812��Greenֵ��ȡֵΪ��0~255
//    uint8_t B;      //WS2812��Blueֵ��ȡֵΪ��0~255
//    float h;        //WS2812��ɫ��ֵ��ȡֵΪ��0~360��
//    float s;        //WS2812�ı��Ͷ�ֵ��ȡֵΪ��0~1.0
//    float v;        //WS2812������ֵ��ȡֵΪ��0~1.0
//} WS2812_Data_TypeDef;

/* �궨�� ----------------------------------------------------------------------*/
#define			WS2812_LED_Number			8							/* WS2812 ��������� */
#define			WS2812_Data_Bit				24
#define			WS2812_T0H					30							/* 0����ߵ�ƽʱ��ռ1/3 */
#define			WS2812_T1H					60							/* 1����ߵ�ƽʱ��ռ2/3 */
#define			WS2812_Sendspeed			800							/* WS2812���ͽ��������ٶ�800Kbps */

#define			WS2812_GPIO_PORT			GPIOB						/* GPIO�˿� */
#define			WS2812_GPIO_CLK				RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define			WS2812_GPIO_PIN				GPIO_Pin_6					/* ���ӵ�SCLʱ���ߵ�GPIO */
#define         WS2812_TIM_CLK              RCC_APB1Periph_TIM4
#define			WS2812_TIM					TIM4						/* WS2812 ��Ӧ�Ķ�ʱ�� */
#define			WS2812_TIM_DMA				TIM_DMA_CC1					/* WS2812 ��Ӧ�Ķ�ʱ����Ӧ��DMAͨ�� */
#define			WS2812_DMA_Addr				(WS2812_TIM->CCR1)			/* WS2812 DMA�����Ŀ�ĵ�ַ */

//#define			WS2812_TIM_DMA1				TIM_DMA_CC1					/* WS2812 ��Ӧ�Ķ�ʱ����Ӧ��DMAͨ�� */
//#define			WS2812_TIM_DMA2			  	TIM_DMA_CC2					/* WS2812 ��Ӧ�Ķ�ʱ����Ӧ��DMAͨ�� */
//#define			WS2812_TIM_DMA3				TIM_DMA_CC3					/* WS2812 ��Ӧ�Ķ�ʱ����Ӧ��DMAͨ�� */
//#define			WS2812_TIM_DMA4				TIM_DMA_CC4					/* WS2812 ��Ӧ�Ķ�ʱ����Ӧ��DMAͨ�� */
//#define			WS2812_DMA1_Addr		    (WS2812_TIM->CCR1)			/* WS2812 DMA�����Ŀ�ĵ�ַ */
//#define			WS2812_DMA2_Addr			(WS2812_TIM->CCR2)			/* WS2812 DMA�����Ŀ�ĵ�ַ */
//#define			WS2812_DMA3_Addr			(WS2812_TIM->CCR3)			/* WS2812 DMA�����Ŀ�ĵ�ַ */
//#define			WS2812_DMA4_Addr			(WS2812_TIM->CCR4)			/* WS2812 DMA�����Ŀ�ĵ�ַ */

#define			WS2812_DMA_Channel			DMA1_Channel1				/* WS2812 ��Ӧ��DMAͨ�� */
#define			WS2812_DMA_Flag				DMA1_FLAG_TC1				/* WS2812 ��Ӧ��DMAͨ��������ɱ�־λ */


//#define			WS2812_GPIO_PORT			GPIOB						/* GPIO�˿� */
//#define			WS2812_GPIO_CLK				RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
//#define			WS2812_GPIO_PIN				GPIO_Pin_0					/* ���ӵ�SCLʱ���ߵ�GPIO */

//#define			WS2812_TIM					TIM3						/* WS2812 ��Ӧ�Ķ�ʱ�� */
//#define			WS2812_TIM_DMA				TIM_DMA_CC3					/* WS2812 ��Ӧ�Ķ�ʱ����Ӧ��DMAͨ�� */
//#define			WS2812_DMA_Addr				(TIM3->CCR3)				/* WS2812 DMA�����Ŀ�ĵ�ַ */
//#define			WS2812_DMA_Channel			DMA1_Channel2				/* WS2812 ��Ӧ��DMAͨ�� */
//#define			WS2812_DMA_Flag				DMA1_FLAG_TC2				/* WS2812 ��Ӧ��DMAͨ��������ɱ�־λ */

#define         WS2812Breath_Flag           1                           /* WS2812 �������ߺ궨�� */
#define         WS2812color_allochroism     0                           /* WS2812 �߲ʽ���궨�� */

/* �������� --------------------------------------------------------------------*/
void WS2812_Config(uint16_t WS2812_Byte);
static void  WS2812_Reset(void);
static void  WS2812_Display(uint8_t LED_Num);
void WS2812_Display_Color(uint8_t LED_Num,uint32_t RGB_Data);
void WS2812_Set_LED_Color(uint8_t LED_Num,uint32_t RGB_Data);
void WS2812_BreathingLight(uint16_t LED_Num,uint32_t RGB_Data);
#endif  /*  __BSP_WS2812_H */

/* END OF FILE ------------------------------------------------------------------*/