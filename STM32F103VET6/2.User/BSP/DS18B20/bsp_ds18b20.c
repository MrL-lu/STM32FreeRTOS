/* 引用头文件 ------------------------------------------------------------------*/  
#include "./DS18B20/bsp_ds18b20.h"  //引用DS18B20配置头文件

/* 定义全局变量 ------------------------------------------------------------------*/

/* 程序开始 ----------------------------------------------------------------------*/
#ifdef      BSP_DS18B20_FreeRTOS

/**********************************************************************
  * @ 函数名  ： DS18B20_Task
  * @ 功能说明： DS18B20_Task任务主体
  * @ 参数    ：   
  * @ 返回值  ： 无
  ********************************************************************/
void DS18B20_Task(void* parameter)
{	
//    BSP_DS18B20_Init(); //初始化
    while( BSP_DS18B20_Init() )	
		printf("\r\n no ds18b20 exit \r\n");
    
    while (1)
    {
//        vTaskSuspendAll(); //阻止OS调度，防止打断us延时
        printf ( "\r\n现在温度： %.1f\r\n", BSP_DS18B20_GetTemp_SkipRom () );		// 打印通过 DS18B20 序列号获取的温度值	
//        xTaskResumeAll();    //恢复OS调度
        vTaskDelay(1000);
    }
}

#endif  //BSP_DS18B20_FreeRTOS

/**
  **********************************************************************
  * @brief  DS18B20 延时函数
  * @author  Lu
  * @version V1.0
  * @date    2024-03-13
  **********************************************************************
  */ 
//static void BSP_DS18B20_Delay(uint16_t time)
//{
//	uint16_t i;

//	/*　
//	 	下面的时间是通过逻辑分析仪测试得到的。
//        工作条件：CPU主频72MHz ，MDK编译环境，1级优化
//		循环次数为10时，频率 = 205KHz ，4.878us
//		循环次数为7时，频率 = 347KHz， 2.879us
//	 	循环次数为5时，频率 = 421KHz， 2.374us
//	*/
//	for (i = 0; i < time; i++);
//}

static void BSP_DS18B20_Delay(uint32_t nTime)
{
    uint32_t ticks,told,tnow,tcnt=0;
    uint32_t reload = SysTick->LOAD; //获取LOAD的值
    ticks = nTime*(SystemCoreClock/1000000);//需要的节拍数
    tcnt = 0;
    vTaskSuspendAll(); //阻止OS调度，防止打断us延时
    told = SysTick->VAL;//刚进入时的计数器值
    while(1)
    {
        tnow = SysTick->VAL;
        if(tnow != told)
        {
            if(tnow<told)tcnt+=told-tnow;
            else tcnt+=reload-tnow+told;
            told = tnow;
            if(tcnt>=ticks)break;//时间超过，退出延时
        }
    }
   xTaskResumeAll();    //恢复OS调度
}
/**
  **********************************************************************
  * @brief  DS18B20 初始化函数
  * @author  Lu
  * @version V1.0
  * @date    2024-03-12
  **********************************************************************
  */ 
uint8_t BSP_DS18B20_Init(void)
{
    BSP_DS18B20_GPIO_Config();
    BSP_DS18B20_High;
    BSP_DS18B20_Reset();
    return BSP_DS18B20_Presence();
}

/**
  **********************************************************************
  * @brief  配置DS18B20用到的I/O口
  * @author  Lu
  * @version V1.0
  * @date    2024-03-12
  **********************************************************************
  */ 
static void BSP_DS18B20_GPIO_Config(void)
{
    /*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef BSP_DS18B20_GPIO_InitStructure;
	/*开启DS18B20相关的GPIO外设时钟*/
	BSP_DS18B20_DQ_SCK_APBxClock_FUN( BSP_DS18B20_GPIO_CLK, ENABLE);
	/*设置引脚模式为通用推挽输出*/
	BSP_DS18B20_GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_Out_PP;
	/*设置引脚速率为50MHz */   
	BSP_DS18B20_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	/*选择要控制的GPIO引脚*/
	BSP_DS18B20_GPIO_InitStructure.GPIO_Pin = BSP_DS18B20_GPIO_PIN;	
	/*调用库函数，初始化GPIO*/
	GPIO_Init(BSP_DS18B20_GPIO_PORT, &BSP_DS18B20_GPIO_InitStructure);
}

/**
  **********************************************************************
  * @brief  主机给从机发送复位脉冲
  * @author  Lu
  * @version V1.0
  * @date    2024-03-12
  **********************************************************************
  */ 
static void BSP_DS18B20_Reset(void)
{
    BSP_DS18B20_Mode_Out_PP();
    
    BSP_DS18B20_Low;
    /* 主机至少产生480us的低电平复位信号 */
    BSP_DS18B20_Delay(500);
    BSP_DS18B20_High;
    /* 从机接收到主机的复位信号后，会在15~60us后给主机发一个存在脉冲 */
    BSP_DS18B20_Delay(15);
}



/**
  **********************************************************************
  * @brief  使DS18B20-DATA引脚变为输出模式
  * @author  Lu
  * @version V1.0
  * @date    2024-03-12
  **********************************************************************
  */ 
static void BSP_DS18B20_Mode_Out_PP(void)
{
    /*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef BSP_DS18B20_GPIO_InitStructure;
	/*开启DS18B20相关的GPIO外设时钟*/
	BSP_DS18B20_DQ_SCK_APBxClock_FUN( BSP_DS18B20_GPIO_CLK, ENABLE);
	/*设置引脚模式为通用推挽输出*/
	BSP_DS18B20_GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_Out_PP;
	/*设置引脚速率为50MHz */   
	BSP_DS18B20_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	/*选择要控制的GPIO引脚*/
	BSP_DS18B20_GPIO_InitStructure.GPIO_Pin = BSP_DS18B20_GPIO_PIN;	
	/*调用库函数，初始化GPIO*/
	GPIO_Init(BSP_DS18B20_GPIO_PORT, &BSP_DS18B20_GPIO_InitStructure);
}


/**
  **********************************************************************
  * @brief  使DS18B20-DATA引脚变为输入模式
  * @author  Lu
  * @version V1.0
  * @date    2024-03-12
  **********************************************************************
  */ 
static void BSP_DS18B20_Mode_IPU(void)
{
    /*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef BSP_DS18B20_GPIO_InitStructure;
	/*开启DS18B20相关的GPIO外设时钟*/
	BSP_DS18B20_DQ_SCK_APBxClock_FUN( BSP_DS18B20_GPIO_CLK, ENABLE);
	/*设置引脚模式为通用推挽输出*/
	BSP_DS18B20_GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_IPU;
	/*选择要控制的GPIO引脚*/
	BSP_DS18B20_GPIO_InitStructure.GPIO_Pin = BSP_DS18B20_GPIO_PIN;	
	/*调用库函数，初始化GPIO*/
	GPIO_Init(BSP_DS18B20_GPIO_PORT, &BSP_DS18B20_GPIO_InitStructure);
}




/**
  **********************************************************************
  * @brief   检测从机给主机返回的存在脉冲
  * @retval	：0：成功 1：失败
  * @author  Lu
  * @version V1.0
  * @date    2024-03-12
  **********************************************************************
  */ 
static uint8_t BSP_DS18B20_Presence(void)
{
    uint8_t pulse_time = 0;
    /* 主机设置为上拉输入 */
    BSP_DS18B20_Mode_IPU();
    /* 等待存在脉冲的到来，存在脉冲为一个60~240us的低电平信号 
	 * 如果存在脉冲没有来则做超时处理，从机接收到主机的复位信号后，会在15~60us后给主机发一个存在脉冲
	 */
    while( BSP_DS18B20_IN() && pulse_time<100 )
	{
		pulse_time++;
        BSP_DS18B20_Delay(1);
	}	
	/* 经过100us后，存在脉冲都还没有到来*/
	if( pulse_time >=100 )
		return 1;
	else
		pulse_time = 0;
	
	/* 存在脉冲到来，且存在的时间不能超过240us */
	while( !BSP_DS18B20_IN() && pulse_time<240 )
	{
		pulse_time++;
        BSP_DS18B20_Delay(1);
	}	
	if( pulse_time >=240 )
		return 1;
	else
		return 0;
}

/**
  **********************************************************************
  * @brief   从DS18B20读取一个bit
  * @author  Lu
  * @version V1.0
  * @date    2024-03-12
  **********************************************************************
  */ 
static uint8_t BSP_DS18B20_ReadBit(void)
{
	uint8_t dat;
	
	/* 读0和读1的时间至少要大于60us */	
	BSP_DS18B20_Mode_Out_PP();
	/* 读时间的起始：必须由主机产生 >1us <15us 的低电平信号 */
	BSP_DS18B20_Low;
    BSP_DS18B20_Delay(10); //10us
	
	/* 设置成输入，释放总线，由外部上拉电阻将总线拉高 */
	BSP_DS18B20_Mode_IPU();
	
	if( BSP_DS18B20_IN() == SET )
		dat = 1;
	else
		dat = 0;
	
	/* 这个延时参数请参考时序图 45us */
    BSP_DS18B20_Delay(45); //45us
	
	return dat;
}

/**
  **********************************************************************
  * @brief   从DS18B20读一个字节，低位先行
  * @author  Lu
  * @version V1.0
  * @date    2024-03-12
  **********************************************************************
  */ 
static uint8_t BSP_DS18B20_ReadByte(void)
{
	uint8_t i, j, dat = 0;	
	
	for(i=0; i<8; i++) 
	{
		j = BSP_DS18B20_ReadBit();		
		dat = (dat) | (j<<i);
	}
	return dat;
}

/**
  **********************************************************************
  * @brief   写一个字节到DS18B20，低位先行
  * @author  Lu
  * @version V1.0
  * @date    2024-03-12
  **********************************************************************
  */ 
static void BSP_DS18B20_WriteByte(uint8_t dat)
{
	uint8_t i, testb;
	BSP_DS18B20_Mode_Out_PP();
	
	for( i=0; i<8; i++ )
	{
		testb = dat&0x01;
		dat = dat>>1;		
		/* 写0和写1的时间至少要大于60us */
		if (testb)
		{			
			BSP_DS18B20_Low;
			/* 1us < 这个延时 < 15us */
            BSP_DS18B20_Delay(10); //10us
            
			BSP_DS18B20_High;
            /* 这个延时 58us */
			BSP_DS18B20_Delay(58); //58us
		}		
		else
		{			
			BSP_DS18B20_Low;
			/* 60us < Tx 0 < 120us */
            BSP_DS18B20_Delay(70); //70us
			BSP_DS18B20_High;			
			/* 1us < Trec(恢复时间) < 无穷大*/
            BSP_DS18B20_Delay(2); //2us
		}
	}
}


/**
  **********************************************************************
  * @brief   跳过匹配 DS18B20 ROM
  * @author  Lu
  * @version V1.0
  * @date    2024-03-13
  **********************************************************************
  */ 
static void BSP_DS18B20_SkipRom ( void )
{
	BSP_DS18B20_Reset();	   	
	BSP_DS18B20_Presence();	 
	BSP_DS18B20_WriteByte(0XCC);		/* 跳过 ROM */
}

/**
  **********************************************************************
  * @brief   执行匹配 DS18B20 ROM
  * @author  Lu
  * @version V1.0
  * @date    2024-03-13
  **********************************************************************
  */ 
static void BSP_DS18B20_MatchRom ( void )
{
	BSP_DS18B20_Reset();	   
	BSP_DS18B20_Presence();	 
	BSP_DS18B20_WriteByte(0X55);		/* 匹配 ROM */
}


/*
 * 存储的温度是16 位的带符号扩展的二进制补码形式
 * 当工作在12位分辨率时，其中5个符号位，7个整数位，4个小数位
 *
 *         |---------整数----------|-----小数 分辨率 1/(2^4)=0.0625----|
 * 低字节  | 2^3 | 2^2 | 2^1 | 2^0 | 2^(-1) | 2^(-2) | 2^(-3) | 2^(-4) |
 *
 *
 *         |-----符号位：0->正  1->负-------|-----------整数-----------|
 * 高字节  |  s  |  s  |  s  |  s  |    s   |   2^6  |   2^5  |   2^4  |
 *
 * 
 * 温度 = 符号位 + 整数 + 小数*0.0625
 */
/**
  **********************************************************************
  * @brief   在跳过匹配 ROM 情况下获取 DS18B20 温度值 
  * @author  Lu
  * @version V1.0
  * @date    2024-03-13
  **********************************************************************
  */ 
float BSP_DS18B20_GetTemp_SkipRom ( void )
{
	uint8_t tpmsb, tplsb;
	short s_tem;
	float f_tem;
	
	
	BSP_DS18B20_SkipRom ();
	BSP_DS18B20_WriteByte(0X44);				/* 开始转换 */
	
	
	BSP_DS18B20_SkipRom ();
    BSP_DS18B20_WriteByte(0XBE);				/* 读温度值 */
	
	tplsb = BSP_DS18B20_ReadByte();		 
	tpmsb = BSP_DS18B20_ReadByte(); 
	
	
	s_tem = tpmsb<<8;
	s_tem = s_tem | tplsb;
	
	if( s_tem < 0 )		/* 负温度 */
		f_tem = (~s_tem+1) * 0.0625;	
	else
		f_tem = s_tem * 0.0625;
	
	return f_tem; 	

}

/**
  **********************************************************************
  * @brief   在匹配 ROM 情况下获取 DS18B20 温度值
  * @param  ds18b20_id：用于存放 DS18B20 序列号的数组的首地址
  * @author  Lu
  * @version V1.0
  * @date    2024-03-13
  **********************************************************************
  */ 
void BSP_DS18B20_ReadId ( uint8_t * ds18b20_id )
{
	uint8_t uc;
	BSP_DS18B20_WriteByte(0x33);       //读取序列号
	
	for ( uc = 0; uc < 8; uc ++ )
	  ds18b20_id [ uc ] = BSP_DS18B20_ReadByte();
}


/**
  **********************************************************************
  * @brief   在匹配 ROM 情况下获取 DS18B20 温度值
  * @param  ds18b20_id：用于存放 DS18B20 序列号的数组的首地址
  * @retval 温度值
  * @author  Lu
  * @version V1.0
  * @date    2024-03-13
  **********************************************************************
  */ 
float BSP_DS18B20_GetTemp_MatchRom ( uint8_t * ds18b20_id )
{
	uint8_t tpmsb, tplsb, i;
	short s_tem;
	float f_tem;
	
	BSP_DS18B20_MatchRom ();            //匹配ROM
	
  for(i=0;i<8;i++)
		BSP_DS18B20_WriteByte ( ds18b20_id [ i ] );	
	
	BSP_DS18B20_WriteByte(0X44);				/* 开始转换 */

	BSP_DS18B20_MatchRom ();            //匹配ROM
	
	for(i=0;i<8;i++)
		BSP_DS18B20_WriteByte ( ds18b20_id [ i ] );	
	
	BSP_DS18B20_WriteByte(0XBE);				/* 读温度值 */
	
	tplsb = BSP_DS18B20_ReadByte();		 
	tpmsb = BSP_DS18B20_ReadByte(); 

	s_tem = tpmsb<<8;
	s_tem = s_tem | tplsb;
	
	if( s_tem < 0 )		/* 负温度 */
		f_tem = (~s_tem+1) * 0.0625;	
	else
		f_tem = s_tem * 0.0625;
	
	return f_tem; 	

}

/* END OF FILE ------------------------------------------------------------------*/


