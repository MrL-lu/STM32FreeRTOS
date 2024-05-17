/* ����ͷ�ļ� ------------------------------------------------------------------*/  
#include "./LED/bsp_led.h"  //����bsp_led����ͷ�ļ�

/* ����ȫ�ֱ��� ------------------------------------------------------------------*/

/* ����ʼ ----------------------------------------------------------------------*/

/**
  **********************************************************************
  * @brief      ����LED���GPIO
  * @author     Lu
  * @version    V1.0
  * @date       2024-2-27
  **********************************************************************
  */   
void BSP_LED_Config()	//LED��ʼ������
{
    /*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef BSP_LED_GPIO_InitStructure;
	/*����LED��ص�GPIO����ʱ��*/
	RCC_APB2PeriphClockCmd( LED_Red_GPIO_CLK , ENABLE);
	/*��������ģʽΪͨ���������*/
	BSP_LED_GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_Out_PP;
	/*������������Ϊ50MHz */   
	BSP_LED_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	/*ѡ��Ҫ���Ƶ�GPIO����*/
	BSP_LED_GPIO_InitStructure.GPIO_Pin = LED_Red_GPIO_PIN;	
	/*���ÿ⺯������ʼ��GPIO*/
	GPIO_Init(LED_Red_GPIO_PORT, &BSP_LED_GPIO_InitStructure);
	
	/*��������ģʽΪͨ���������*/
	BSP_LED_GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_Out_PP;
	/*������������Ϊ50MHz */   
	BSP_LED_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	/*ѡ��Ҫ���Ƶ�GPIO����*/
	BSP_LED_GPIO_InitStructure.GPIO_Pin = LED_Green_GPIO_PIN;	
	/*���ÿ⺯������ʼ��GPIO*/
	GPIO_Init(LED_Green_GPIO_PORT, &BSP_LED_GPIO_InitStructure);
	
	/*��������ģʽΪͨ���������*/
	BSP_LED_GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_Out_PP;
	/*������������Ϊ50MHz */   
	BSP_LED_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	/*ѡ��Ҫ���Ƶ�GPIO����*/
	BSP_LED_GPIO_InitStructure.GPIO_Pin = LED_Blue_GPIO_PIN;	
	/*���ÿ⺯������ʼ��GPIO*/
	GPIO_Init(LED_Blue_GPIO_PORT, &BSP_LED_GPIO_InitStructure);
	/* �ر�����led��	*/
	GPIO_SetBits(LED_Red_GPIO_PORT, LED_Red_GPIO_PIN);
	GPIO_SetBits(LED_Green_GPIO_PORT, LED_Green_GPIO_PIN);
	GPIO_SetBits(LED_Blue_GPIO_PORT, LED_Blue_GPIO_PIN);
}


/**
  **********************************************************************
  * @brief      ʹ��LED��ת
  *			    GPIOx	  :	GPIO�˿�
  *			    GPIO_Pin  :	���ӵ�SCLʱ���ߵ�GPIO
  * @author     Lu
  * @version    V1.0
  * @note	    ע�ͣ����briefһ��ʹ�ÿ��Դﵽ�ܺõ�ע��Ч����
  * @date       2024-02-27
  **********************************************************************
  */ 
void BSP_LED_TOGGLE(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    GPIOx->ODR ^= GPIO_Pin;
}



/* END OF FILE ------------------------------------------------------------------*/
