/* 引用头文件 ------------------------------------------------------------------*/  
#include "./SysTick/systick.h"  //引用系统时钟头文件

/**
 * @brief  ：主系统滴答时钟初始化函数
 * @param  ：无
 * @retval ：无
 * @doc    ：无
 */
void SysTick_Init(void) //系统滴答时钟初始化函数
{
    /* SystemCoreClock / 1000    1ms中断一次
	   * SystemCoreClock / 100000	 10us中断一次
	   * SystemCoreClock / 1000000 1us中断一次
	   */
//	if (SysTick_Config(SystemFrequency / 100000))	// ST3.0.0库版本
	if ( SysTick_Config( SystemCoreClock / 1000000 ) )	// ST3.5.0库版本
	{ 
		/* Capture error */ 
		while (1);
	}
	// 关闭滴答定时器  
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}

/**
 * @brief  ：延时微秒函数
 * @param  ：无
 * @retval ：无
 * @doc    ：无
 */
void SysTick_Delay_us( uint32_t us )	//延时微秒函数
{
	uint32_t i;	//定义局部变量
	SysTick_Config(72); //写入reload 寄存器
	for(i=0;i<us;i++)
    {
      while(!((SysTick->CTRL)&(1<<16)));//判断系统定时器标志位
    }
    // 关闭滴答定时器
		SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}


/**
 * @brief  ：延时毫秒函数
 * @param  ：无
 * @retval ：无
 * @doc    ：无
 */
void SysTick_Delay_ms( uint32_t ms )	//延延时毫秒函数
{
	uint32_t i;	//定义局部变量
	SysTick_Config(72000); //写入reload 寄存器
	for(i=0;i<ms;i++)
   {
     while(!((SysTick->CTRL)&(1<<16)));//判断系统定时器标志位
   }
    // 关闭滴答定时器
		SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}




/* END OF FILE ------------------------------------------------------------------*/


