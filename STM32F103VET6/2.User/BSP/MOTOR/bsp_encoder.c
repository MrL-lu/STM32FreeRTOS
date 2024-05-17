/* 引用头文件 ------------------------------------------------------------------*/  
#include "./MOTOR/bsp_encoder.h"  //引用编码器配置头文件

/* 定义全局变量 ------------------------------------------------------------------*/
volatile uint16_t BSP_ENCODER_Overflow_Count = 0;   //定时器溢出次数


/* 程序开始 ----------------------------------------------------------------------*/

/**
  **********************************************************************
  * @brief  编码器 中断优先级配置
  * @author  Lu
  * @version V1.0
  * @date    2024-04-16
  **********************************************************************
  */ 
//static void GENERAL_TIM_NVIC_Config(void)
//{
//    NVIC_InitTypeDef NVIC_InitStructure; 
//    // 设置中断组为0
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
//	// 设置中断来源
//    NVIC_InitStructure.NVIC_IRQChannel = BSP_ENCODER_TIM_IRQ ;	
//	// 设置主优先级为 0
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
//	// 设置抢占优先级为3
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
//}


/**
  **********************************************************************
  * @brief  编码器相关配置函数(输入捕获通道设置)
  * @author  Lu
  * @version V1.0
  * @date    2024-xx-xx
  **********************************************************************
  */ 
static void BSP_ENCODER_GPIO_Config(void)
{
    /* 定义一个GPIO_InitTypeDef类型的结构体 */
	GPIO_InitTypeDef BSP_ENCODER_GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(BSP_ENCODER_A_GPIO_CLK,ENABLE);//配置时钟
    BSP_ENCODER_GPIO_InitStructure.GPIO_Mode    =   GPIO_Mode_IN_FLOATING;  //浮空输入模式
    BSP_ENCODER_GPIO_InitStructure.GPIO_Pin     =   BSP_ENCODER_A_GPIO_PIN;
    BSP_ENCODER_GPIO_InitStructure.GPIO_Speed   =   GPIO_Speed_50MHz;
    /*调用库函数，初始化GPIO*/
	GPIO_Init(BSP_ENCODER_A_GPIO_PORT, &BSP_ENCODER_GPIO_InitStructure);
    
    RCC_APB2PeriphClockCmd(BSP_ENCODER_B_GPIO_CLK,ENABLE);//配置时钟
    BSP_ENCODER_GPIO_InitStructure.GPIO_Mode    =   GPIO_Mode_IN_FLOATING;  //浮空输入模式
    BSP_ENCODER_GPIO_InitStructure.GPIO_Pin     =   BSP_ENCODER_B_GPIO_PIN;
    BSP_ENCODER_GPIO_InitStructure.GPIO_Speed   =   GPIO_Speed_50MHz;
    /*调用库函数，初始化GPIO*/
	GPIO_Init(BSP_ENCODER_B_GPIO_PORT, &BSP_ENCODER_GPIO_InitStructure);
}

/**
  **********************************************************************
  * @brief  编码器 定时器配置函数
  * @author  Lu
  * @version V1.0
  * @date    2024-xx-xx
  **********************************************************************
  */ 
static void BSP_ENCODER_TIM_Config(void)
{
    RCC_APB1PeriphClockCmd(BSP_ENCODER_TIM_CLK,ENABLE); // 开启定时器时钟,即内部时钟CK_INT=72M
    /*--------------------时基结构体初始化-------------------------*/
	
    /* 定义一个 TIM_TimeBaseInitTypeDef 类型的结构体 */
    TIM_TimeBaseInitTypeDef		BSP_ENCODER_TIM_TimeBaseStructure;
	// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
	BSP_ENCODER_TIM_TimeBaseStructure.TIM_Period=BSP_ENCODER_TIM_PERIOD;	
	// 驱动CNT计数器的时钟 = Fck_int/(psc+1)
	BSP_ENCODER_TIM_TimeBaseStructure.TIM_Prescaler= BSP_ENCODER_TIM_PSC;	
	// 时钟分频因子 ，配置死区时间时需要用到
	BSP_ENCODER_TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// 计数器计数模式，设置为向上计数
	BSP_ENCODER_TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// 重复计数器的值，没用到不用管
	BSP_ENCODER_TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// 初始化定时器
	TIM_TimeBaseInit(BSP_ENCODER_TIM, &BSP_ENCODER_TIM_TimeBaseStructure);

    /*--------------------输入捕获结构体初始化-------------------*/	
	TIM_ICInitTypeDef BSP_ENCODER_TIM_ICInitStructure;
    
    TIM_ICStructInit(&BSP_ENCODER_TIM_ICInitStructure); //把TIM_ICInitStruct 中的每一个参数按缺省值填入
    
    /*-------------------- IC1 -------------------*/	
	// 配置输入捕获的通道，需要根据具体的GPIO来配置
	BSP_ENCODER_TIM_ICInitStructure.TIM_Channel = BSP_ENCODER_TIM_CHANNEL_A;
    // 输入的需要被捕获的信号的滤波系数
	BSP_ENCODER_TIM_ICInitStructure.TIM_ICFilter = 15;
    // 定时器输入捕获初始化
	TIM_ICInit(BSP_ENCODER_TIM, &BSP_ENCODER_TIM_ICInitStructure);
    /*-------------------- IC2 -------------------*/	
    // 配置输入捕获的通道，需要根据具体的GPIO来配置
	BSP_ENCODER_TIM_ICInitStructure.TIM_Channel = BSP_ENCODER_TIM_CHANNEL_B;
    // 输入的需要被捕获的信号的滤波系数
	BSP_ENCODER_TIM_ICInitStructure.TIM_ICFilter = 15;
    // 定时器输入捕获初始化
	TIM_ICInit(BSP_ENCODER_TIM, &BSP_ENCODER_TIM_ICInitStructure);
    
    /* 编码器配置 ：定时器 模式3 上升沿和下降沿都触发 */
    TIM_EncoderInterfaceConfig(BSP_ENCODER_TIM,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising); //使用编码器模式3
    
//    TIM_EncoderInterfaceConfig(BSP_ENCODER_TIM,TIM_EncoderMode_TI12,TIM_ICPolarity_BothEdge,TIM_ICPolarity_BothEdge); //使用编码器模式3
    
    
//	// 输入捕获信号的极性配置
//	BSP_ENCODER_TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge; //双边沿触发捕获
//	// 输入通道和捕获通道的映射关系，有直连和非直连两种
//	BSP_ENCODER_TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
//	// 输入的需要被捕获的信号的分频系数
//	BSP_ENCODER_TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
//	// 输入的需要被捕获的信号的滤波系数
//	BSP_ENCODER_TIM_ICInitStructure.TIM_ICFilter = 10;
//	// 定时器输入捕获初始化
//	TIM_ICInit(BSP_ENCODER_TIM, &BSP_ENCODER_TIM_ICInitStructure);

    //重置定时器计数
    TIM_SetCounter(BSP_ENCODER_TIM,BSP_ENCODER_Num);
	// 清除更新和捕获中断标志位
    TIM_ClearFlag(BSP_ENCODER_TIM, TIM_FLAG_Update|BSP_ENCODER_TIM_IT_CCx);	
    // 开启更新和捕获中断  
	TIM_ITConfig (BSP_ENCODER_TIM, TIM_IT_Update | BSP_ENCODER_TIM_IT_CCx, ENABLE );
    
//    TIM_SetCounter(BSP_ENCODER_TIM,0);
	TIM_Cmd(BSP_ENCODER_TIM, ENABLE);   // 使能计数器
}

/**
  **********************************************************************
  * @brief  编码器 初始化函数
  * @author  Lu
  * @version V1.0
  * @date    2024-04-16
  **********************************************************************
  */ 
void BSP_ENCODER_Init(void)
{
    BSP_ENCODER_GPIO_Config();
//    GENERAL_TIM_NVIC_Config();
    BSP_ENCODER_TIM_Config();
}

/**
  **********************************************************************
  * @brief  编码器 读取计数函数
  * @param	BSP_ENCODER_TIMX:定时器，取值参考为 TIM3 TIM4
  * @retval CNT:编码器数值(可以换算为速度)
  * @author  Lu
  * @version V1.0
  * @date    2024-xx-xx
  **********************************************************************
  */ 
uint16_t BSP_ENCODER_Read_CNT(TIM_TypeDef* BSP_ENCODER_TIMX)
{
    uint16_t CNT =0;
    CNT = (short)TIM_GetCounter(BSP_ENCODER_TIM); //获取计数值
    TIM_SetCounter(BSP_ENCODER_TIM,BSP_ENCODER_Num);//清零
    return CNT;
}

/**
  **********************************************************************
  * @brief   编码器中断服务函数
  * @author  Lu
  * @version V1.0
  * @date    2024-04-16
  **********************************************************************
  */ 
void BSP_ENCODER_TIM_INT_FUN()
{
    if(BSP_ENCODER_TIM->SR & 0x0001)//Overflow interrupt //溢出中断
    {
    }
    BSP_ENCODER_TIM->SR&=~(1<<0);//Clear the interrupt flag bit //清除中断标志位 
}


/**
  **********************************************************************
  * @brief  编码器 通过计数值换算速度函数
  * @author  Lu
  * @version V1.0
  * @date    2024-04-16
  **********************************************************************
  */ 
float BSP_ENCODER_Read_CNT_Speed(TIM_TypeDef* BSP_ENCODER_TIMX)
{
    float Speed;
    Speed = BSP_ENCODER_Read_CNT(BSP_ENCODER_TIM) * BSP_ENCODER_FREQUENCY* BSP_ENCODER_Tire_Length * 3.1416 / BSP_ENCODER_Reduction_Ratio; //速度 = 编码器计数值 * 轮胎周长 * 3.1416 * 编码器获取频率 / 编码器线数（330）
    return Speed;
}

/* END OF FILE ------------------------------------------------------------------*/

