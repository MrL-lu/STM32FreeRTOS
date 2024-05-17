/* 引用头文件 ------------------------------------------------------------------*/  
#include "./IIC/bsp_i2c.h"  //引用IIC配置头文件

/*
1.初始化IIC相关GPIO
2.配置IIC外设工作模式
3.编写IIC写函数
4.编写IIC读取函数
*/

/* 定义全局变量 ------------------------------------------------------------------*/
static __IO uint32_t  BSP_I2CTimeout = BSP_I2CT_LONG_TIMEOUT; //超时变量

/* 程序开始 ----------------------------------------------------------------------*/

/**
  **********************************************************************
  * @brief      配置LED相关GPIO
  * @author     Lu
  * @version    V1.0
  * @date       2024-03-22
  **********************************************************************
  */   
static void BSP_IIC_Config(void)	//IIC初始化函数
{
    /*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef BSP_LED_GPIO_InitStructure;
	/*开启IIC相关的外设时钟*/
	BSP_I2C_APBxClock_FUN( BSP_I2C_CLK , ENABLE);
    BSP_I2C_GPIO_APBxClock_FUN( BSP_I2C_GPIO_CLK , ENABLE);
    
	/*设置SCL引脚模式为复用开漏输出*/
	BSP_LED_GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF_OD;
	/*设置引脚速率为50MHz */   
	BSP_LED_GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	/*选择要控制的GPIO引脚*/
	BSP_LED_GPIO_InitStructure.GPIO_Pin     = BSP_I2C_SCL_PIN;	
	/*调用库函数，初始化GPIO*/
	GPIO_Init(BSP_I2C_SCL_PORT, &BSP_LED_GPIO_InitStructure);
    
    /*设置SDA引脚模式为复用开漏输出*/
	BSP_LED_GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF_OD;
	/*设置引脚速率为50MHz */   
	BSP_LED_GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	/*选择要控制的GPIO引脚*/
	BSP_LED_GPIO_InitStructure.GPIO_Pin     = BSP_I2C_SDA_PIN;	
	/*调用库函数，初始化GPIO*/
	GPIO_Init(BSP_I2C_SDA_PORT, &BSP_LED_GPIO_InitStructure);
	
}


/**
  **********************************************************************
  * @brief      I2C 工作模式配置
  * @author     Lu
  * @version    V1.0
  * @date       2024-03-22
  **********************************************************************
  */ 
static void BSP_IIC_Mode_Config(void)
{
    I2C_InitTypeDef  I2C_InitStructure; 

    /* I2C 配置 */
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;

    /* 高电平数据稳定，低电平数据变化 SCL 时钟线的占空比 */
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    
    I2C_InitStructure.I2C_OwnAddress1 =BSP_I2C_OWN_ADDRESS7; //主机设备地址
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;    //IIC应答使能

    /* I2C的寻址模式 */
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; //7位或者10位地址

    /* 通信速率 */
    I2C_InitStructure.I2C_ClockSpeed = BSP_I2C_Speed;

    /* I2C 初始化 */
    I2C_Init(BSP_I2Cx, &I2C_InitStructure);

    /* 使能 I2C */
    I2C_Cmd(BSP_I2Cx, ENABLE);
}

/**
  **********************************************************************
  * @brief      I2C 初始化
  * @author     Lu
  * @version    V1.0
  * @date       2024-04-10
  **********************************************************************
  */ 
void BSP_IIC_Init(void)
{
    BSP_IIC_Config();
    BSP_IIC_Mode_Config();
}

/**
  **********************************************************************
  * @brief      I2C 超时错误
  * @param      WriteAddr   写地址
  * @param      pBuffer     缓冲区指针
  * @author     Lu
  * @version    V1.0
  * @date       2024-04-10
  **********************************************************************
  */
static  uint8_t I2C_TIMEOUT_UserCallback(void)
{
  /* Block communication and all processes */
  printf("I2C Timeout error!"); 
  return 0;
}

/**
  **********************************************************************
  * @brief      I2C 写一个字节
  * @param      WriteAddr   写地址
  * @param      pBuffer     缓冲区指针
  * @author     Lu
  * @version    V1.0
  * @date       2024-04-10
  **********************************************************************
  */ 
uint8_t BSP_IIC_WritByte(uint8_t Slave_ADDRESS,uint8_t WriteAddr,uint8_t pBuffer)
{
    /* 1.产生起始信号 */
    I2C_GenerateSTART(BSP_I2Cx, ENABLE);
    
    BSP_I2CTimeout = BSP_I2CT_FLAG_TIMEOUT;
    /* 2.检测EV5 起始信号发送成功 */
    while(!I2C_CheckEvent(BSP_I2Cx, I2C_EVENT_MASTER_MODE_SELECT))  
    {
        if((BSP_I2CTimeout--) == 0)
        {
            I2C_TIMEOUT_UserCallback(); //超时
            return 0;   
//            return BSP_I2C_TIMEOUT_UserCallback(0);
        }      
    } 
    BSP_I2CTimeout = BSP_I2CT_FLAG_TIMEOUT;
    
    /* 3.发送从机的地址 */
    I2C_Send7bitAddress(BSP_I2Cx, Slave_ADDRESS, I2C_Direction_Transmitter);
    
    /* 4. 检测事件EV6*/
    while(!I2C_CheckEvent(BSP_I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        if((BSP_I2CTimeout--) == 0)
        {
            I2C_TIMEOUT_UserCallback(); //超时
            return 1;   // EV6 超时
//            return BSP_I2C_TIMEOUT_UserCallback(0);
        }      
    } 
    BSP_I2CTimeout = BSP_I2CT_FLAG_TIMEOUT; 
    
    /* 5.发送一个字节 */
    I2C_SendData(BSP_I2Cx, WriteAddr);
    
    BSP_I2CTimeout = BSP_I2CT_FLAG_TIMEOUT;
    /* 6. 检测 EV8 事件 */
    while(!I2C_CheckEvent(BSP_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
        if((BSP_I2CTimeout--) == 0)
        {
            I2C_TIMEOUT_UserCallback(); //超时
            return 2;   //EV8 超时
//            return BSP_I2C_TIMEOUT_UserCallback(0);
        }      
    }

    /* 7. 发送停止信号 */
    I2C_GenerateSTOP(BSP_I2Cx, ENABLE);
    
    return 255; //发送成功
}

/**
  **********************************************************************
  * @brief      I2C 读一个字节
  * @param      WriteAddr   读设备的地址（即从机地址）
  * @param      pBuffer     缓冲区指针
  * @param      NumByteToRead     读取的字节数
  * @author     Lu
  * @version    V1.0
  * @date       2024-04-10
  **********************************************************************
  */ 
uint8_t BSP_IIC_ReadByte(uint8_t ReadAddr,uint8_t* pBuffer, uint16_t NumByteToRead)
{
    BSP_I2CTimeout = BSP_I2CT_FLAG_TIMEOUT;
    //*((u8 *)0x4001080c) |=0x80; 
    while(I2C_GetFlagStatus(BSP_I2Cx, I2C_FLAG_BUSY)) //等待IIC解除忙碌状态
    {
        if((BSP_I2CTimeout--) == 0)
        {
            I2C_TIMEOUT_UserCallback(); //超时
//            return I2C_TIMEOUT_UserCallback(9);
            return 0; // 读取超时
        }             
    }

    /* 1.产生起始信号 */
    I2C_GenerateSTART(BSP_I2Cx, ENABLE);
    
    BSP_I2CTimeout = BSP_I2CT_FLAG_TIMEOUT;
    /* 2.检测EV5 起始信号发送成功 */
    while(!I2C_CheckEvent(BSP_I2Cx, I2C_EVENT_MASTER_MODE_SELECT))  
    {
        if((BSP_I2CTimeout--) == 0)
        {
            I2C_TIMEOUT_UserCallback(); //超时
            return 0;   //起始信号超时
//            return BSP_I2C_TIMEOUT_UserCallback(0);
        }      
    } 

    /* 3.发送从机的地址 */
    I2C_Send7bitAddress(BSP_I2Cx, ReadAddr, I2C_Direction_Transmitter);
    
    /* 4. 检测事件EV6*/
    BSP_I2CTimeout = BSP_I2CT_FLAG_TIMEOUT;
    while(!I2C_CheckEvent(BSP_I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        if((BSP_I2CTimeout--) == 0)
        {
            I2C_TIMEOUT_UserCallback(); //超时
            return 1;   // EV6 超时
//            return BSP_I2C_TIMEOUT_UserCallback(0);
        }      
    } 
    
    /* Clear EV6 by setting again the PE bit */
    I2C_Cmd(BSP_I2Cx, ENABLE);
    
    /* 5. Send the slave's internal address to write to */
    I2C_SendData(BSP_I2Cx, ReadAddr); 
    
    /* 6. 检测事件 EV8*/
    BSP_I2CTimeout = BSP_I2CT_FLAG_TIMEOUT;
    while(!I2C_CheckEvent(BSP_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
        if((BSP_I2CTimeout--) == 0)
        {
            I2C_TIMEOUT_UserCallback(); //超时
            return 1;   // EV6 超时
//            return BSP_I2C_TIMEOUT_UserCallback(0);
        }      
    } 
    /* Send STRAT condition a second time */  
    I2C_GenerateSTART(BSP_I2Cx, ENABLE);
    
    /* 6. 检测事件 EV5*/
    BSP_I2CTimeout = BSP_I2CT_FLAG_TIMEOUT;
    while(!I2C_CheckEvent(BSP_I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if((BSP_I2CTimeout--) == 0)
        {
            I2C_TIMEOUT_UserCallback(); //超时
            return 1;   // EV6 超时
//            return BSP_I2C_TIMEOUT_UserCallback(0);
        }      
    } 
    
    /* Send slave address for read */
    I2C_Send7bitAddress(BSP_I2Cx, ReadAddr, I2C_Direction_Receiver);
    
    /* 6. 检测事件 EV6*/
    BSP_I2CTimeout = BSP_I2CT_FLAG_TIMEOUT;
    while(!I2C_CheckEvent(BSP_I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
    {
        if((BSP_I2CTimeout--) == 0)
        {
            I2C_TIMEOUT_UserCallback(); //超时
            return 1;   // EV6 超时
//            return BSP_I2C_TIMEOUT_UserCallback(0);
        }      
    }
    /* While there is data to be read */
    while(NumByteToRead)  
    {
        if(NumByteToRead == 1)
        {
            /* Disable Acknowledgement */
            I2C_AcknowledgeConfig(BSP_I2Cx, DISABLE);
            /* Send STOP Condition */
            I2C_GenerateSTOP(BSP_I2Cx, ENABLE);
        }

        /* Test on EV7 and clear it */
        if(I2C_CheckEvent(BSP_I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED))  
        {      
            /* Read a byte from the slave */
            *pBuffer = I2C_ReceiveData(BSP_I2Cx);

            /* Point to the next location where the byte read will be saved */
            pBuffer++; 
      
            /* Decrement the read bytes counter */
            NumByteToRead--;        
        }   
    }
    /* Enable Acknowledgement to be ready for another reception */
    I2C_AcknowledgeConfig(BSP_I2Cx, ENABLE);

	return 1; //正常，返回1
}

/**
  * @brief  Wait for slave Standby state 
  * @param  无
  * @retval 无
  */
void I2C_WaitStandbyState(uint8_t SlaveAddress)      
{
    vu16 SR1_Tmp = 0;
    do
    {
        /* Send START condition */
        I2C_GenerateSTART(BSP_I2Cx, ENABLE);
        /* Read I2C1 SR1 register */
        SR1_Tmp = I2C_ReadRegister(BSP_I2Cx, I2C_Register_SR1);
        /* Send slave address for write */
        I2C_Send7bitAddress(BSP_I2Cx, SlaveAddress, I2C_Direction_Transmitter);
    }while(!(I2C_ReadRegister(BSP_I2Cx, I2C_Register_SR1) & 0x0002));
  
    /* Clear AF flag */
    I2C_ClearFlag(BSP_I2Cx, I2C_FLAG_AF);
    /* STOP condition */    
    I2C_GenerateSTOP(BSP_I2Cx, ENABLE); 
}



/* END OF FILE ------------------------------------------------------------------*/

