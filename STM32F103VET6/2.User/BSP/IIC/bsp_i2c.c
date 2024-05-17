/* ����ͷ�ļ� ------------------------------------------------------------------*/  
#include "./IIC/bsp_i2c.h"  //����IIC����ͷ�ļ�

/*
1.��ʼ��IIC���GPIO
2.����IIC���蹤��ģʽ
3.��дIICд����
4.��дIIC��ȡ����
*/

/* ����ȫ�ֱ��� ------------------------------------------------------------------*/
static __IO uint32_t  BSP_I2CTimeout = BSP_I2CT_LONG_TIMEOUT; //��ʱ����

/* ����ʼ ----------------------------------------------------------------------*/

/**
  **********************************************************************
  * @brief      ����LED���GPIO
  * @author     Lu
  * @version    V1.0
  * @date       2024-03-22
  **********************************************************************
  */   
static void BSP_IIC_Config(void)	//IIC��ʼ������
{
    /*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef BSP_LED_GPIO_InitStructure;
	/*����IIC��ص�����ʱ��*/
	BSP_I2C_APBxClock_FUN( BSP_I2C_CLK , ENABLE);
    BSP_I2C_GPIO_APBxClock_FUN( BSP_I2C_GPIO_CLK , ENABLE);
    
	/*����SCL����ģʽΪ���ÿ�©���*/
	BSP_LED_GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF_OD;
	/*������������Ϊ50MHz */   
	BSP_LED_GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	/*ѡ��Ҫ���Ƶ�GPIO����*/
	BSP_LED_GPIO_InitStructure.GPIO_Pin     = BSP_I2C_SCL_PIN;	
	/*���ÿ⺯������ʼ��GPIO*/
	GPIO_Init(BSP_I2C_SCL_PORT, &BSP_LED_GPIO_InitStructure);
    
    /*����SDA����ģʽΪ���ÿ�©���*/
	BSP_LED_GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF_OD;
	/*������������Ϊ50MHz */   
	BSP_LED_GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	/*ѡ��Ҫ���Ƶ�GPIO����*/
	BSP_LED_GPIO_InitStructure.GPIO_Pin     = BSP_I2C_SDA_PIN;	
	/*���ÿ⺯������ʼ��GPIO*/
	GPIO_Init(BSP_I2C_SDA_PORT, &BSP_LED_GPIO_InitStructure);
	
}


/**
  **********************************************************************
  * @brief      I2C ����ģʽ����
  * @author     Lu
  * @version    V1.0
  * @date       2024-03-22
  **********************************************************************
  */ 
static void BSP_IIC_Mode_Config(void)
{
    I2C_InitTypeDef  I2C_InitStructure; 

    /* I2C ���� */
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;

    /* �ߵ�ƽ�����ȶ����͵�ƽ���ݱ仯 SCL ʱ���ߵ�ռ�ձ� */
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    
    I2C_InitStructure.I2C_OwnAddress1 =BSP_I2C_OWN_ADDRESS7; //�����豸��ַ
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;    //IICӦ��ʹ��

    /* I2C��Ѱַģʽ */
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; //7λ����10λ��ַ

    /* ͨ������ */
    I2C_InitStructure.I2C_ClockSpeed = BSP_I2C_Speed;

    /* I2C ��ʼ�� */
    I2C_Init(BSP_I2Cx, &I2C_InitStructure);

    /* ʹ�� I2C */
    I2C_Cmd(BSP_I2Cx, ENABLE);
}

/**
  **********************************************************************
  * @brief      I2C ��ʼ��
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
  * @brief      I2C ��ʱ����
  * @param      WriteAddr   д��ַ
  * @param      pBuffer     ������ָ��
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
  * @brief      I2C дһ���ֽ�
  * @param      WriteAddr   д��ַ
  * @param      pBuffer     ������ָ��
  * @author     Lu
  * @version    V1.0
  * @date       2024-04-10
  **********************************************************************
  */ 
uint8_t BSP_IIC_WritByte(uint8_t Slave_ADDRESS,uint8_t WriteAddr,uint8_t pBuffer)
{
    /* 1.������ʼ�ź� */
    I2C_GenerateSTART(BSP_I2Cx, ENABLE);
    
    BSP_I2CTimeout = BSP_I2CT_FLAG_TIMEOUT;
    /* 2.���EV5 ��ʼ�źŷ��ͳɹ� */
    while(!I2C_CheckEvent(BSP_I2Cx, I2C_EVENT_MASTER_MODE_SELECT))  
    {
        if((BSP_I2CTimeout--) == 0)
        {
            I2C_TIMEOUT_UserCallback(); //��ʱ
            return 0;   
//            return BSP_I2C_TIMEOUT_UserCallback(0);
        }      
    } 
    BSP_I2CTimeout = BSP_I2CT_FLAG_TIMEOUT;
    
    /* 3.���ʹӻ��ĵ�ַ */
    I2C_Send7bitAddress(BSP_I2Cx, Slave_ADDRESS, I2C_Direction_Transmitter);
    
    /* 4. ����¼�EV6*/
    while(!I2C_CheckEvent(BSP_I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        if((BSP_I2CTimeout--) == 0)
        {
            I2C_TIMEOUT_UserCallback(); //��ʱ
            return 1;   // EV6 ��ʱ
//            return BSP_I2C_TIMEOUT_UserCallback(0);
        }      
    } 
    BSP_I2CTimeout = BSP_I2CT_FLAG_TIMEOUT; 
    
    /* 5.����һ���ֽ� */
    I2C_SendData(BSP_I2Cx, WriteAddr);
    
    BSP_I2CTimeout = BSP_I2CT_FLAG_TIMEOUT;
    /* 6. ��� EV8 �¼� */
    while(!I2C_CheckEvent(BSP_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
        if((BSP_I2CTimeout--) == 0)
        {
            I2C_TIMEOUT_UserCallback(); //��ʱ
            return 2;   //EV8 ��ʱ
//            return BSP_I2C_TIMEOUT_UserCallback(0);
        }      
    }

    /* 7. ����ֹͣ�ź� */
    I2C_GenerateSTOP(BSP_I2Cx, ENABLE);
    
    return 255; //���ͳɹ�
}

/**
  **********************************************************************
  * @brief      I2C ��һ���ֽ�
  * @param      WriteAddr   ���豸�ĵ�ַ�����ӻ���ַ��
  * @param      pBuffer     ������ָ��
  * @param      NumByteToRead     ��ȡ���ֽ���
  * @author     Lu
  * @version    V1.0
  * @date       2024-04-10
  **********************************************************************
  */ 
uint8_t BSP_IIC_ReadByte(uint8_t ReadAddr,uint8_t* pBuffer, uint16_t NumByteToRead)
{
    BSP_I2CTimeout = BSP_I2CT_FLAG_TIMEOUT;
    //*((u8 *)0x4001080c) |=0x80; 
    while(I2C_GetFlagStatus(BSP_I2Cx, I2C_FLAG_BUSY)) //�ȴ�IIC���æµ״̬
    {
        if((BSP_I2CTimeout--) == 0)
        {
            I2C_TIMEOUT_UserCallback(); //��ʱ
//            return I2C_TIMEOUT_UserCallback(9);
            return 0; // ��ȡ��ʱ
        }             
    }

    /* 1.������ʼ�ź� */
    I2C_GenerateSTART(BSP_I2Cx, ENABLE);
    
    BSP_I2CTimeout = BSP_I2CT_FLAG_TIMEOUT;
    /* 2.���EV5 ��ʼ�źŷ��ͳɹ� */
    while(!I2C_CheckEvent(BSP_I2Cx, I2C_EVENT_MASTER_MODE_SELECT))  
    {
        if((BSP_I2CTimeout--) == 0)
        {
            I2C_TIMEOUT_UserCallback(); //��ʱ
            return 0;   //��ʼ�źų�ʱ
//            return BSP_I2C_TIMEOUT_UserCallback(0);
        }      
    } 

    /* 3.���ʹӻ��ĵ�ַ */
    I2C_Send7bitAddress(BSP_I2Cx, ReadAddr, I2C_Direction_Transmitter);
    
    /* 4. ����¼�EV6*/
    BSP_I2CTimeout = BSP_I2CT_FLAG_TIMEOUT;
    while(!I2C_CheckEvent(BSP_I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        if((BSP_I2CTimeout--) == 0)
        {
            I2C_TIMEOUT_UserCallback(); //��ʱ
            return 1;   // EV6 ��ʱ
//            return BSP_I2C_TIMEOUT_UserCallback(0);
        }      
    } 
    
    /* Clear EV6 by setting again the PE bit */
    I2C_Cmd(BSP_I2Cx, ENABLE);
    
    /* 5. Send the slave's internal address to write to */
    I2C_SendData(BSP_I2Cx, ReadAddr); 
    
    /* 6. ����¼� EV8*/
    BSP_I2CTimeout = BSP_I2CT_FLAG_TIMEOUT;
    while(!I2C_CheckEvent(BSP_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
        if((BSP_I2CTimeout--) == 0)
        {
            I2C_TIMEOUT_UserCallback(); //��ʱ
            return 1;   // EV6 ��ʱ
//            return BSP_I2C_TIMEOUT_UserCallback(0);
        }      
    } 
    /* Send STRAT condition a second time */  
    I2C_GenerateSTART(BSP_I2Cx, ENABLE);
    
    /* 6. ����¼� EV5*/
    BSP_I2CTimeout = BSP_I2CT_FLAG_TIMEOUT;
    while(!I2C_CheckEvent(BSP_I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if((BSP_I2CTimeout--) == 0)
        {
            I2C_TIMEOUT_UserCallback(); //��ʱ
            return 1;   // EV6 ��ʱ
//            return BSP_I2C_TIMEOUT_UserCallback(0);
        }      
    } 
    
    /* Send slave address for read */
    I2C_Send7bitAddress(BSP_I2Cx, ReadAddr, I2C_Direction_Receiver);
    
    /* 6. ����¼� EV6*/
    BSP_I2CTimeout = BSP_I2CT_FLAG_TIMEOUT;
    while(!I2C_CheckEvent(BSP_I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
    {
        if((BSP_I2CTimeout--) == 0)
        {
            I2C_TIMEOUT_UserCallback(); //��ʱ
            return 1;   // EV6 ��ʱ
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

	return 1; //����������1
}

/**
  * @brief  Wait for slave Standby state 
  * @param  ��
  * @retval ��
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

