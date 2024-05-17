/* 引用头文件 ------------------------------------------------------------------*/  
#include "./DHT11/dht11.h"  //引用dht11驱动头文件

/* 定义全局变量 ------------------------------------------------------------------*/

/**
 * @brief  ：DHT11引脚的输出类型为推挽输出函数
 * @param  ：无
 * @retval ：无
 * @doc    ：无
 */
void DHT11_Output( void )
{
		GPIO_InitTypeDef  DHT11_GPIO_InitStructure;
		RCC_APB2PeriphClockCmd( DHT11_GPIO_RCC_APB2, ENABLE);	//使能LED的GPIO端口时钟
		
		DHT11_GPIO_InitStructure.GPIO_Mode		=			GPIO_Mode_Out_PP;	//设置引脚的输出类型为推挽输出
		DHT11_GPIO_InitStructure.GPIO_Pin			=			DHT11_GPIO_Pin;
		DHT11_GPIO_InitStructure.GPIO_Speed		=			GPIO_Speed_50MHz;
	
		GPIO_Init(DHT11_GPIOX,&DHT11_GPIO_InitStructure);
}


/**
 * @brief  ：DHT11引脚的输出类型为浮空输入函数
 * @param  ：无
 * @retval ：无
 * @doc    ：无
 */
void DHT11_Input( void )
{
		GPIO_InitTypeDef  DHT11_GPIO_InitStructure;
		RCC_APB2PeriphClockCmd( DHT11_GPIO_RCC_APB2, ENABLE);	//使能LED的GPIO端口时钟
		
		DHT11_GPIO_InitStructure.GPIO_Mode		=			GPIO_Mode_IPU;	//设置引脚的输出类型为浮空输入
		DHT11_GPIO_InitStructure.GPIO_Pin			=			DHT11_GPIO_Pin;
	
		GPIO_Init(DHT11_GPIOX,&DHT11_GPIO_InitStructure);	
}

/**
 * @brief  ：DHT11读取一个字节 高位先行
 * @param  ：无
 * @retval ：dat ：返回读取的一个字节
 * @doc    ：无
 */
static uint8_t DHT11_ReadByte( void )
{
	uint8_t i,dat;
	
	for(i=0;i<8;i++)
	{
		/*每bit以50us低电平标置开始，轮询直到从机发出 的50us 低电平 结束*/
		while(GPIO_ReadInputDataBit (DHT11_GPIOX,DHT11_GPIO_Pin) == 0);
		
		/*DHT11 以26~28us的高电平表示“0”，以70us高电平表示“1”，
		 *通过检测 x us后的电平即可区别这两个状 ，x 即下面的延时 
		 */
		SysTick_Delay_us(30);	//延时30us
		if(GPIO_ReadInputDataBit (DHT11_GPIOX,DHT11_GPIO_Pin) == 1 ) //高电平
		{
			while(GPIO_ReadInputDataBit (DHT11_GPIOX,DHT11_GPIO_Pin));	//等待高电平结束
			dat |= (uint8_t)(0x01<<(7-i));	//把第7-i位置1，MSB先行
		}
		else
		{
			dat &= (uint8_t)~(0x01<<(7-i));	//把第7-i位置1，MSB先行
		}
	}
	return dat;	//返回数值
}

/**
 * @brief  ：DHT11读取一次数据（40bit） 高位先行
 * @param  ：8bit 湿度整数 + 8bit 湿度小数 + 8bit 温度整数 + 8bit 温度小数 + 8bit 校验和 
 * @retval ：dat ：返回读取的一个字节
 * @doc    ：无
 */
uint8_t DHT11_Read_TempAndHumidity(DHT11_Data_TypeDef *DHT11_Data)
{
	DHT11_Output();	//输出模式
	
	/*主机发送起始信号*/
	GPIO_ResetBits(DHT11_GPIOX,DHT11_GPIO_Pin);	//主机拉低不小于18ms的电平
	SysTick_Delay_ms(18);
	
	/*主机拉高电平释放总线*/
	GPIO_SetBits(DHT11_GPIOX,DHT11_GPIO_Pin);	//主机拉高总线
	SysTick_Delay_us(30);	//延时30us
	
	DHT11_Input();	//DHT11引脚的输出类型为浮空输入函数
	
	if(GPIO_ReadInputDataBit (DHT11_GPIOX,DHT11_GPIO_Pin) == 0)
	{
		/*从机响应信号*/
		while(GPIO_ReadInputDataBit (DHT11_GPIOX,DHT11_GPIO_Pin) == 0);	//从机拉低电平83us
		while(GPIO_ReadInputDataBit (DHT11_GPIOX,DHT11_GPIO_Pin) == 1);	//从机拉高电平87us
		
		/*开始接收数据*/
		DHT11_Data->humi_int 	= DHT11_ReadByte();	//8bit 湿度整数
		DHT11_Data->humi_deci = DHT11_ReadByte();	//8bit 湿度小数
		DHT11_Data->temp_int 	= DHT11_ReadByte();	//8bit 温度整数
		DHT11_Data->temp_deci = DHT11_ReadByte();	//8bit 温度小数
		DHT11_Data->check_sum = DHT11_ReadByte();	//8bit 校验和
		
		/*读取结束，引脚改为输出模式*/
		DHT11_Output();	//DHT11引脚的输出类型为推挽输出函数
		GPIO_SetBits(DHT11_GPIOX,DHT11_GPIO_Pin);	//主机拉高总线
		
		if(DHT11_Data->check_sum == DHT11_Data->humi_int + DHT11_Data->humi_deci + DHT11_Data->temp_int+ DHT11_Data->temp_deci)
		{
//			DHT11_Data->humi_int 	= (DHT11_Data->humi_int /10)*16 + (DHT11_Data->humi_int %10); //湿度整数转换为10进制
//			DHT11_Data->humi_deci = (DHT11_Data->humi_deci /10)*16 + (DHT11_Data->humi_deci %10); //湿度小数转换为10进制
//			DHT11_Data->temp_int 	= (DHT11_Data->temp_int /10)*16 + (DHT11_Data->temp_int %10); //温度整数转换为10进制
//			DHT11_Data->temp_deci = (DHT11_Data->temp_deci /10)*16 + (DHT11_Data->temp_deci %10); //温度小数转换为10进制
			return 1;	//数据校验成功
		}
		else
			return 0;	//数据校验失败
	}
	else
		return 0;	//数据校验失败
}

/* END OF FILE ------------------------------------------------------------------*/

