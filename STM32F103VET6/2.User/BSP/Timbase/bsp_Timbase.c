/* 引用头文件 ------------------------------------------------------------------*/  
#include "./Timbase/bsp_TiMbase.h"  //引用基本定时器配置头文件

/* 定义全局变量 ------------------------------------------------------------------*/

/* 程序开始 ----------------------------------------------------------------------*/
static void BASIC_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);		
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = BASIC_TIM_IRQ ;	
		// 设置主优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;	 
	  // 设置抢占优先级为3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

static void BASIC_TIM_Mode_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	
    // 开启定时器时钟,即内部时钟CK_INT=72M
    BASIC_TIM_APBxClock_FUN(BASIC_TIM_CLK, ENABLE);
    // 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
    TIM_TimeBaseStructure.TIM_Period = BASIC_TIM_Period;	
    // 时钟预分频数为
    TIM_TimeBaseStructure.TIM_Prescaler= BASIC_TIM_Prescaler;
//    // 时钟分频因子 ，基本定时器没有，不用管
//    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;

//    //计数器计数模式，基本定时器只能向上计数，没有计数模式的设置
//    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 

//    // 重复计数器的值，基本定时器没有，不用管
//    TIM_TimeBaseStructure.TIM_RepetitionCounter=0;

    // 初始化定时器
    TIM_TimeBaseInit(BASIC_TIM, &TIM_TimeBaseStructure);

    // 清除计数器中断标志位
    TIM_ClearFlag(BASIC_TIM, TIM_FLAG_Update);

    // 开启计数器中断
    TIM_ITConfig(BASIC_TIM,TIM_IT_Update,ENABLE);

    // 使能计数器
    TIM_Cmd(BASIC_TIM, DISABLE);	
}

void BASIC_TIM_Init(void)
{
	BASIC_TIM_NVIC_Config();
	BASIC_TIM_Mode_Config();
}


/**
  **********************************************************************
  * @brief  获取空闲任务的任务堆栈和任务控制块内存
  * @param  ：frequency——PWM频率(KHz)  例如：freq = 1 KHz = 1ms 中断一次
  * @param  ：frequency——PWM频率(KHz)  例如：freq = 1 KHz = 1ms 中断一次
  * @author  Lu
  * @version V1.0
  * @date    2024-xx-xx
  **********************************************************************
  */
// ARR ：自动重装载寄存器的值
// CLK_cnt：计数器的时钟，等于 Fck_int / (psc+1) = 72M/(psc+1)
// PWM 信号的周期 T = ARR * (1/CLK_cnt) = ARR*(PSC+1) / 72M
uint32_t BasicTime = 0;

void BASIC_TIM_Delay(uint16_t frequency,uint32_t delay_time)
{
    //设定自动重装载寄存器周期的数值，freq单位为KHz，通过SystemCoreClock获取系统的时钟频率
    uint16_t ARR = (SystemCoreClock/(frequency*1000))-1;

    BasicTime = delay_time; //设置延时时间
    
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	
    // 开启定时器时钟,即内部时钟CK_INT=72M
    BASIC_TIM_APBxClock_FUN(BASIC_TIM_CLK, ENABLE);
    // 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
    TIM_TimeBaseStructure.TIM_Period = ARR;	
    // 时钟预分频数为
    TIM_TimeBaseStructure.TIM_Prescaler= BASIC_TIM_Prescaler;
//    // 时钟分频因子 ，基本定时器没有，不用管
//    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;

    // 初始化定时器
    TIM_TimeBaseInit(BASIC_TIM, &TIM_TimeBaseStructure);

    // 清除计数器中断标志位
    TIM_ClearFlag(BASIC_TIM, TIM_FLAG_Update);

    // 开启计数器中断
    TIM_ITConfig(BASIC_TIM,TIM_IT_Update,ENABLE);

    // 使能计数器
    TIM_Cmd(BASIC_TIM, ENABLE);
    
}

/* END OF FILE ------------------------------------------------------------------*/
