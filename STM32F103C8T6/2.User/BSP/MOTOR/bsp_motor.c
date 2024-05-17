/* 引用头文件 ------------------------------------------------------------------*/  
#include "./MOTOR/bsp_motor.h"  //引用电机配置头文件

/* 定义全局变量 ------------------------------------------------------------------*/


/* 程序开始 ----------------------------------------------------------------------*/

/**
  * @brief    BSP_MOTOR_GPIO_Config(void);	//通用定时器3输出比较通道 GPIO引脚配置函数
  * @param  ：参数说明
  * @arg    ：参数里面可选择参量列举
  * @note   ：注释
  * @retval ：返回值说明
  * @doc    ：无
  */
void BSP_MOTOR_GPIO_Config(void)
{
	/*	定义 BSP_MOTOR_GPIO_InitStructure 结构体	*/
	GPIO_InitTypeDef BSP_MOTOR_GPIO_InitStructure;

    // 输出比较通道1 GPIO 初始化  PA6
    RCC_APB2PeriphClockCmd(BSP_MOTOR_TIM_CH1_GPIO_CLK, ENABLE);
    BSP_MOTOR_GPIO_InitStructure.GPIO_Pin =  BSP_MOTOR_TIM_CH1_PIN;
    BSP_MOTOR_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    BSP_MOTOR_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BSP_MOTOR_TIM_CH1_PORT, &BSP_MOTOR_GPIO_InitStructure);

	// 输出比较通道2 GPIO 初始化	PA7
    RCC_APB2PeriphClockCmd(BSP_MOTOR_TIM_CH2_GPIO_CLK, ENABLE);
    BSP_MOTOR_GPIO_InitStructure.GPIO_Pin =  BSP_MOTOR_TIM_CH2_PIN;
    BSP_MOTOR_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    BSP_MOTOR_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BSP_MOTOR_TIM_CH2_PORT, &BSP_MOTOR_GPIO_InitStructure);
	
	// 输出比较通道3 GPIO 初始化	PB0
    RCC_APB2PeriphClockCmd(BSP_MOTOR_TIM_CH3_GPIO_CLK, ENABLE);
    BSP_MOTOR_GPIO_InitStructure.GPIO_Pin =  BSP_MOTOR_TIM_CH3_PIN;
    BSP_MOTOR_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    BSP_MOTOR_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BSP_MOTOR_TIM_CH3_PORT, &BSP_MOTOR_GPIO_InitStructure);

	// 输出比较通道4 GPIO 初始化	PB1
    RCC_APB2PeriphClockCmd(BSP_MOTOR_TIM_CH4_GPIO_CLK, ENABLE);
    BSP_MOTOR_GPIO_InitStructure.GPIO_Pin =  BSP_MOTOR_TIM_CH4_PIN;
    BSP_MOTOR_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    BSP_MOTOR_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BSP_MOTOR_TIM_CH4_PORT, &BSP_MOTOR_GPIO_InitStructure);
}


///*
// * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
// * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
// * 另外三个成员是通用定时器和高级定时器才有.
// *-----------------------------------------------------------------------------
// *typedef struct
// *{ TIM_Prescaler            都有
// *	TIM_CounterMode			     TIMx,x[6,7]没有，其他都有
// *  TIM_Period               都有
// *  TIM_ClockDivision        TIMx,x[6,7]没有，其他都有
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]才有
// *}TIM_TimeBaseInitTypeDef; 
// *-----------------------------------------------------------------------------
// */

/* ----------------   PWM信号 周期和占空比的计算--------------- */
// ARR ：自动重装载寄存器的值
// CLK_cnt：计数器的时钟，等于 Fck_int / (psc+1) = 72M/(psc+1)
// PWM 信号的周期 T = ARR * (1/CLK_cnt) = ARR*(PSC+1) / 72M
// 占空比P=CCR/(ARR+1)

 /**
  * @brief  ：电机使用通用定时器设置PWM 并启动PWM
  * @param  ：timer	——定时器 取值：TIMx  x={1，2，3，4}
  * @param  ：tim_channel——通道		取值：tim_channel={1，2，3，4}
  * @param  ：frequency——PWM频率(KHz)  例如：freq = 10 KHz
  * @param  ：Duty_cycle——PWM占空比(%)	例如：duty = 20 %
  * @note   ：注释
  * @doc    ：无
  */
void BSP_MOTOR_PWM_SET(TIM_TypeDef* timer,uint8_t tim_channel,uint16_t frequency,uint16_t Duty_cycle)
{
	uint16_t	ARR	=	(SystemCoreClock/1000)/frequency-1;	//设定自动重装载寄存器周期的数值，freq单位为KHz，一般直接使用72000
	// 开启定时器时钟,即内部时钟CK_INT=72M
	BSP_MOTOR_TIM_APBxClock_FUN(BSP_MOTOR_TIM_CLK,ENABLE);	//示例打开的是通用定时器3的使能时钟
	TIM_TimeBaseInitTypeDef GeneralTim_PWM;	//定义结构体
	// 驱动CNT计数器的时钟 = Fck_int/(psc+1)
	GeneralTim_PWM.TIM_Prescaler			=	0;	//设置预分频
	// 计数器计数模式，设置为向上计数
	GeneralTim_PWM.TIM_CounterMode			= TIM_CounterMode_Up;	//向上计数模式
    
	// 自动重装载寄存器的值，累计ARR+1个频率后产生一个更新或者中断
	GeneralTim_PWM.TIM_Period				=	ARR;	//指定要加载到活动中的期间值

	// 时钟分频因子 ，配置死区时间时需要用到
	GeneralTim_PWM.TIM_ClockDivision		=	TIM_CKD_DIV1;	//设置时钟分频系数：不分频(这里用不到)

	// 重复计数器的值，没用到不用管
	GeneralTim_PWM.TIM_RepetitionCounter	=	0;
	
	// 初始化定时器
	TIM_TimeBaseInit(timer, &GeneralTim_PWM);
	
	/*	定义 TIM_OCInitStructure 结构体	*/
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	/*	配置为PWM模式1	*/
	TIM_OCInitStructure.TIM_OCMode			=		TIM_OCMode_PWM1;
	/*	输出使能	*/
	TIM_OCInitStructure.TIM_OutputState		=		TIM_OutputState_Enable;
	/*	设置PWM脉冲占空比	*/
	TIM_OCInitStructure.TIM_Pulse			=		(ARR+1)*Duty_cycle/100-1;
	/*	输出通道电平极性配置	*/
	TIM_OCInitStructure.TIM_OCPolarity		=		TIM_OCPolarity_Low;	//低电平
	
	switch(tim_channel)
	{
		case 1:
				TIM_OC1Init(timer, &TIM_OCInitStructure);	 									//使能通道3
				TIM_OC1PreloadConfig(timer, TIM_OCPreload_Enable);
			break;
		case 2:
				TIM_OC2Init(timer, &TIM_OCInitStructure);	 									//使能通道3
				TIM_OC2PreloadConfig(timer, TIM_OCPreload_Enable);
			break;
		case 3:
				TIM_OC3Init(timer, &TIM_OCInitStructure);	 									//使能通道3
				TIM_OC3PreloadConfig(timer, TIM_OCPreload_Enable);
			break;
		case 4:
				TIM_OC4Init(timer, &TIM_OCInitStructure);	 									//使能通道3
				TIM_OC4PreloadConfig(timer, TIM_OCPreload_Enable);
			break;
	}
    TIM_ARRPreloadConfig(timer, ENABLE);//使能定时器TIMx在ARR上的预装载寄存器	使能TIM3重载寄存器ARR
    /* 使能定时器TIMx	*/
    TIM_Cmd(timer, ENABLE);//使能定时器TIMx
}


/**
  **********************************************************************
  * @brief  设置电机转动速度，设置TIM通道的占空比
  * @param	channel - 选择通道
  * @param	Duty_cycle - 选择占空比（以正占空比为例）
  * @author  Lu
  * @version V1.0
  * @date    2024-04-11
  **********************************************************************
  */ 
void BSP_MOTOR_Set_PWM_Pulse(uint8_t channel,uint16_t Duty_cycle)
{
    /*	定义 TIM_OCInitStructure 结构体	*/
	TIM_OCInitTypeDef  TIM_OCInitStructure;
    
    TIM_OCInitStructure.TIM_Pulse	= Duty_cycle;
    switch(channel)
	{
		case 1:
				TIM_OC1Init(BSP_MOTOR_TIM, &TIM_OCInitStructure);	 									//使能通道3
				TIM_OC1PreloadConfig(BSP_MOTOR_TIM, TIM_OCPreload_Enable);
			break;
		case 2:
				TIM_OC2Init(BSP_MOTOR_TIM, &TIM_OCInitStructure);	 									//使能通道3
				TIM_OC2PreloadConfig(BSP_MOTOR_TIM, TIM_OCPreload_Enable);
			break;
		case 3:
				TIM_OC3Init(BSP_MOTOR_TIM, &TIM_OCInitStructure);	 									//使能通道3
				TIM_OC3PreloadConfig(BSP_MOTOR_TIM, TIM_OCPreload_Enable);
			break;
		case 4:
				TIM_OC4Init(BSP_MOTOR_TIM, &TIM_OCInitStructure);	 									//使能通道3
				TIM_OC4PreloadConfig(BSP_MOTOR_TIM, TIM_OCPreload_Enable);
			break;
	}
}



/**
  **********************************************************************
  * @brief  电机停转，所有输出通道都停止
  * @author  Lu
  * @param	timer - 选择定时器
  * @version V1.0
  * @date    2024-04-11
  **********************************************************************
  */ 
void BSP_MOTOR_STOP(TIM_TypeDef* timer)
{
    /* 失能TIM */
    TIM_Cmd(timer, ENABLE);//使能定时器TIMx
    /* 使IN1 IN2 IN3 IN4 都输出低 */
    GPIO_ResetBits(BSP_MOTOR_TIM_CH1_PORT,BSP_MOTOR_TIM_CH1_PIN);
    GPIO_ResetBits(BSP_MOTOR_TIM_CH2_PORT,BSP_MOTOR_TIM_CH2_PIN);
    GPIO_ResetBits(BSP_MOTOR_TIM_CH3_PORT,BSP_MOTOR_TIM_CH3_PIN);
    GPIO_ResetBits(BSP_MOTOR_TIM_CH4_PORT,BSP_MOTOR_TIM_CH4_PIN);
}


///**
//  **********************************************************************
//  * @brief  设置电机方向
//  * @author  Lu
//  * @version V1.0
//  * @date    2024-04-11
//  **********************************************************************
//  */ 
//void BSP_MOTOR_Set_Direction(uint8_t motor_channel,bsp_motor_dir_t motor_dir)
//{
//    /* */
//    if(motor_dir == BSP_MOTOR_FWD) //正转
//    {
//        switch(motor_channel)
//        {
//            case 1:
//                BSP_MOTOR_PWM_SET(BSP_MOTOR_TIM,1,BSP_MOTOR_Freq,BSP_MOTOR_Duty);
//                BSP_MOTOR_PWM_SET(BSP_MOTOR_TIM,1,BSP_MOTOR_Freq,BSP_MOTOR_Duty);
//                break;
//            case 2:
//                BSP_MOTOR_PWM_SET(BSP_MOTOR_TIM,1,BSP_MOTOR_Freq,BSP_MOTOR_Duty);
//                BSP_MOTOR_PWM_SET(BSP_MOTOR_TIM,1,BSP_MOTOR_Freq,BSP_MOTOR_Duty);
//                break;
//        }
//    }
//}

/* END OF FILE ------------------------------------------------------------------*/

