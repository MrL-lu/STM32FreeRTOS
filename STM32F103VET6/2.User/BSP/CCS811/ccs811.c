/* 用户头文件 ------------------------------------------------------------------*/
#include "./CCS811/ccs811.h"	//引用空气传感器CCS811

//来自-SysTick.h 的全局变量
extern  uint8_t  Timer_flag ;//定义全局变量 Timer_flag 作为是否进行计时的标志位
extern  uint16_t Timer_us ;//定义全局变量 Timer_us 作为1us计时
extern  uint16_t Timer_ms ;//定义全局变量 Timer_us 作为1ms计时
extern  uint16_t Timer_s  ;//定义全局变量 Timer_us 作为1s计时

//来自-ccs811.h 的全局变量
unsigned char CSS811_USART_Data_Buff[100];//定义CCS811 串口传输数据组
unsigned char CCS811_USART_Data_End=0;//定义CCS811 串口传输完成标志位
static unsigned char Cnt=0;//中断函数移位变量

 /**
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* 嵌套向量中断控制器组选择 */
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* 配置USART为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = CCS811_USART_IRQ;
  /* 抢断优先级*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* 子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
  /* 使能中断 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* 初始化配置NVIC */
  NVIC_Init(&NVIC_InitStructure);
}


/**
 * @brief  ：CCS811 空气传感器 串口GPIO配置函数
 * @param  ：参数说明
 * @arg    ：参数里面可选择参量列举
 * @note   ：注释
 * @retval ：返回值说明
 * @doc    ：无
 */
void CCS811_USART_GPIO_Config( void )//CCS811 空气传感器 串口GPIO配置函数
{
	/*  定义结构体   */
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
    
    /*  打开串口GPIO的时钟  */
	CCS811_USART1_GPIO_APBxClkCmd(CCS811_USART1_GPIO_CLK, ENABLE);
	 
    /*  打开串口外设的时钟  */
	CCS811_USART_APBxClkCmd_1(CCS811_USART_CLK_1, ENABLE);

    /*  将USART Tx的GPIO配置为推挽复用模式  */
	GPIO_InitStructure.GPIO_Pin = CCS811_USART1_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CCS811_USART1_TX_GPIO_PORT, &GPIO_InitStructure);

    /*  将USART Rx的GPIO配置为浮空输入模式  */
	GPIO_InitStructure.GPIO_Pin = CCS811_USART1_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(CCS811_USART1_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// 配置串口的工作参数
    /*  配置波特率   */
	USART_InitStructure.USART_BaudRate = CCS811_USART_BAUDRATE;
    /*  配置 针数据字长   */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    /*  配置停止位   */
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
    /*  配置校验位   */
	USART_InitStructure.USART_Parity = USART_Parity_No ;
    /*  配置硬件流控制   */
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    /*  配置工作模式，收发一起   */
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    /*  完成串口的初始化配置   */
	USART_Init(CCS811_USART, &USART_InitStructure);	
	
    /*  串口中断优先级配置   */
	NVIC_Configuration();
    
    // 使能串口接收中断
	USART_ITConfig(CCS811_USART, USART_IT_RXNE, ENABLE);
    
    /*  使能串口   */
	USART_Cmd(CCS811_USART, ENABLE);
}

/**
 * @brief  ：CCS811 空气传感器 串口发送函数
 * @param  ：参数说明
 * @arg    ：参数里面可选择参量列举
 * @note   ：注释
 * @retval ：返回值说明
 * @doc    ：无
 */
void CSS811_USART_Send(uint8_t *data,uint8_t len)
{
	uint8_t i;//定义一个局部变量
	USART_ClearFlag(CCS811_USART,USART_FLAG_TC);//清除 CCS811 所用串口传输完成标志位
	for(i=0;i<len;i++)
	{
		USART_SendData(CCS811_USART,*(data+i));
		while(USART_GetFlagStatus(CCS811_USART,USART_FLAG_TC) == RESET);//等待传输完成
	}
}


/**
 * @brief  ：CCS811 空气传感器 串口中断函数
 * @param  ：参数说明
 * @arg    ：参数里面可选择参量列举
 * @note   ：注释
 * @retval ：返回值说明
 * @doc    ：无
 */
void CCS811_USART_IRQHandler( void )
{
	uint8_t CCS811_usartdata=0;	//定义局部变量
	if (	USART_GetFlagStatus(CCS811_USART, USART_IT_RXNE) != RESET)	//判断是否为误判
	{
		/*	中断寄存器存在数据	*/
		CCS811_usartdata = USART_ReceiveData (CCS811_USART); //读取寄存器的数值
		CSS811_USART_Data_Buff[Cnt++] = CCS811_usartdata;//将数据打包进数组		
		if(CCS811_usartdata == 0x0A)//接收到了结束符
		{
			CCS811_USART_Data_End =1;//CCS811 串口传输完成标志位
			Cnt =0;//清零，方便下次接受数据
		}
	}
	USART_ClearITPendingBit (CCS811_USART,USART_IT_RXNE);//软件清除标志位，方便下一次接受数据

}


/**
 * @brief  ：CCS811 数据解析函数
 * @param  ：参数说明
 * @arg    ：参数里面可选择参量列举
 * @note   ：注释
 * @retval ：返回值说明
 * @doc    ：无
 */
void CCS811_DATA_Analysis(unsigned char* flag)
{
	unsigned char *p;
	int co2=0,tvos=0;
	
	if(*flag==1)
	{
		p=(unsigned char*)strstr((char*)CSS811_USART_Data_Buff,"CO2 = ")+6;
		while(*p!='p')
		{
			if(*p>='0'&&*p<='9')
				co2=co2*10+(*p-'0');
			p++;		
		}

		p=(unsigned char*)strstr((char*)CSS811_USART_Data_Buff,"TVOS = ")+7;
		while(*p!='p')
		{
			if(*p>='0'&&*p<='9')
				tvos=tvos*10+(*p-'0');
			p++;		
		}
		if(strstr((char*)CSS811_USART_Data_Buff,"CO2 = ")!=NULL && strstr((char*)CSS811_USART_Data_Buff,"TVOS = ")!=NULL)
		printf("CO2浓度:%d ppm  TVOS浓度:%d ppb\r\n",co2,tvos);
		
		*flag=0;
 }
	
}

/**
 * @brief  ：CCS811 传感器检测函数
 * @param  ：参数说明
 * @arg    ：参数里面可选择参量列举
 * @note   ：注释
 * @retval ：返回值说明
 * @doc    ：无
 */
void CCS811_Check(unsigned char* flag)
{
	Timer_flag =1;Timer_us=0;Timer_ms=0;//启动计时
	
	CSS811_USART_Send("AT\r\n",4);//发送 AT 指令
  while((Timer_ms<100)&&(*flag==0));
	if(*flag==0) 
	{
		printf("Connect Error\r\n");
		Timer_flag =0;//停止计时
	}	
	else          
	{
		printf("Connect Success\r\n");
		Timer_flag =0;//停止计时
	}
	*flag=0;
}

/**
 * @brief  ：CCS811 传感器发送AT指令初始化设置函数
 * @param  ：参数说明
 * @arg    ：参数里面可选择参量列举
 * @note   ：注释
 * @retval ：返回值说明
 * @doc    ：无
 */
void CCS811_AT_Send_Init()
{
	int flag=0;
	//传感器检测
	Delay_ms(1000); Timer_flag =1;Timer_us=0;Timer_ms=0;//启动计时
	CSS811_USART_Send("AT\r\n",strlen("AT\r\n"));	//发送 AT 指令
	while(Timer_ms<100 && flag==0)
	{
		if(CCS811_USART_Data_End==1 && strstr((char*)CSS811_USART_Data_Buff,"OK\r\n")!=NULL)
		{CCS811_USART_Data_End=0; Timer_flag =0;break; }
	}
	if(Timer_ms<90)
		
	  printf("Senser Connect Success\r\n");
	
	//传感器初始化
	flag=0; Delay_ms(100); Timer_ms=0;
	CSS811_USART_Send("AT+INIT\r\n",strlen("AT+INIT\r\n"));
	while(Timer_ms<500 && flag==0)
	{
		if(CCS811_USART_Data_End==1 && strstr((char*)CSS811_USART_Data_Buff,"SUCCESS")!=NULL)
		{CCS811_USART_Data_End=0; Timer_flag =0;break; }
	}
	if(Timer_ms<490)
	  printf("Senser Init Success\r\n");
	
	//设置传感器回传速度 1000MS
	flag=0; Delay_ms(100); Timer_ms=0;
	CSS811_USART_Send("AT+PRATE=1000\r\n",strlen("AT+PRATE=1000\r\n"));
	while(Timer_ms<500 && flag==0)
	{
		if(CCS811_USART_Data_End==1 && strstr((char*)CSS811_USART_Data_Buff,"OK")!=NULL)
		{CCS811_USART_Data_End=0;  Timer_flag =0;break; }		
	}
	if(Timer_ms<490)
	{
	  printf("Senser Set Success\r\n");//传感器初始化成功
		Timer_flag =0;
//		*connect=1;
	}
}








/* END OF FILE ------------------------------------------------------------------*/

