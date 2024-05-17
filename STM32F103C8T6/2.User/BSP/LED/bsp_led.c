/* 引用头文件 ------------------------------------------------------------------*/  
#include "./LED/bsp_led.h"  //引用bsp_led配置头文件

/* 定义全局变量 ------------------------------------------------------------------*/

/* 程序开始 ----------------------------------------------------------------------*/

/**
  **********************************************************************
  * @brief      配置LED相关GPIO
  * @author     Lu
  * @version    V1.0
  * @date       2024-2-27
  **********************************************************************
  */   
void BSP_LED_Config()	//LED初始化函数
{
    /*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef BSP_LED_GPIO_InitStructure;
	/*开启LED相关的GPIO外设时钟*/
	RCC_APB2PeriphClockCmd( LED_Red_GPIO_CLK , ENABLE);
	/*设置引脚模式为通用推挽输出*/
	BSP_LED_GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_Out_PP;
	/*设置引脚速率为50MHz */   
	BSP_LED_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	/*选择要控制的GPIO引脚*/
	BSP_LED_GPIO_InitStructure.GPIO_Pin = LED_Red_GPIO_PIN;	
	/*调用库函数，初始化GPIO*/
	GPIO_Init(LED_Red_GPIO_PORT, &BSP_LED_GPIO_InitStructure);
	
	/*设置引脚模式为通用推挽输出*/
	BSP_LED_GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_Out_PP;
	/*设置引脚速率为50MHz */   
	BSP_LED_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	/*选择要控制的GPIO引脚*/
	BSP_LED_GPIO_InitStructure.GPIO_Pin = LED_Green_GPIO_PIN;	
	/*调用库函数，初始化GPIO*/
	GPIO_Init(LED_Green_GPIO_PORT, &BSP_LED_GPIO_InitStructure);
	
	/*设置引脚模式为通用推挽输出*/
	BSP_LED_GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_Out_PP;
	/*设置引脚速率为50MHz */   
	BSP_LED_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	/*选择要控制的GPIO引脚*/
	BSP_LED_GPIO_InitStructure.GPIO_Pin = LED_Blue_GPIO_PIN;	
	/*调用库函数，初始化GPIO*/
	GPIO_Init(LED_Blue_GPIO_PORT, &BSP_LED_GPIO_InitStructure);
	/* 关闭所有led灯	*/
	GPIO_SetBits(LED_Red_GPIO_PORT, LED_Red_GPIO_PIN);
	GPIO_SetBits(LED_Green_GPIO_PORT, LED_Green_GPIO_PIN);
	GPIO_SetBits(LED_Blue_GPIO_PORT, LED_Blue_GPIO_PIN);
}


/**
  **********************************************************************
  * @brief      使得LED翻转
  *			    GPIOx	  :	GPIO端口
  *			    GPIO_Pin  :	连接到SCL时钟线的GPIO
  * @author     Lu
  * @version    V1.0
  * @note	    注释，配合brief一起使用可以达到很好的注释效果；
  * @date       2024-02-27
  **********************************************************************
  */ 
void BSP_LED_TOGGLE(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    GPIOx->ODR ^= GPIO_Pin;
}



/* END OF FILE ------------------------------------------------------------------*/
