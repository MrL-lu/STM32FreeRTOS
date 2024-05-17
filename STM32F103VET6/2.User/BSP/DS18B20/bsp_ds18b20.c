/* ����ͷ�ļ� ------------------------------------------------------------------*/  
#include "./DS18B20/bsp_ds18b20.h"  //����DS18B20����ͷ�ļ�

/* ����ȫ�ֱ��� ------------------------------------------------------------------*/

/* ����ʼ ----------------------------------------------------------------------*/
#ifdef      BSP_DS18B20_FreeRTOS

/**********************************************************************
  * @ ������  �� DS18B20_Task
  * @ ����˵���� DS18B20_Task��������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  ********************************************************************/
void DS18B20_Task(void* parameter)
{	
//    BSP_DS18B20_Init(); //��ʼ��
    while( BSP_DS18B20_Init() )	
		printf("\r\n no ds18b20 exit \r\n");
    
    while (1)
    {
//        vTaskSuspendAll(); //��ֹOS���ȣ���ֹ���us��ʱ
        printf ( "\r\n�����¶ȣ� %.1f\r\n", BSP_DS18B20_GetTemp_SkipRom () );		// ��ӡͨ�� DS18B20 ���кŻ�ȡ���¶�ֵ	
//        xTaskResumeAll();    //�ָ�OS����
        vTaskDelay(1000);
    }
}

#endif  //BSP_DS18B20_FreeRTOS

/**
  **********************************************************************
  * @brief  DS18B20 ��ʱ����
  * @author  Lu
  * @version V1.0
  * @date    2024-03-13
  **********************************************************************
  */ 
//static void BSP_DS18B20_Delay(uint16_t time)
//{
//	uint16_t i;

//	/*��
//	 	�����ʱ����ͨ���߼������ǲ��Եõ��ġ�
//        ����������CPU��Ƶ72MHz ��MDK���뻷����1���Ż�
//		ѭ������Ϊ10ʱ��Ƶ�� = 205KHz ��4.878us
//		ѭ������Ϊ7ʱ��Ƶ�� = 347KHz�� 2.879us
//	 	ѭ������Ϊ5ʱ��Ƶ�� = 421KHz�� 2.374us
//	*/
//	for (i = 0; i < time; i++);
//}

static void BSP_DS18B20_Delay(uint32_t nTime)
{
    uint32_t ticks,told,tnow,tcnt=0;
    uint32_t reload = SysTick->LOAD; //��ȡLOAD��ֵ
    ticks = nTime*(SystemCoreClock/1000000);//��Ҫ�Ľ�����
    tcnt = 0;
    vTaskSuspendAll(); //��ֹOS���ȣ���ֹ���us��ʱ
    told = SysTick->VAL;//�ս���ʱ�ļ�����ֵ
    while(1)
    {
        tnow = SysTick->VAL;
        if(tnow != told)
        {
            if(tnow<told)tcnt+=told-tnow;
            else tcnt+=reload-tnow+told;
            told = tnow;
            if(tcnt>=ticks)break;//ʱ�䳬�����˳���ʱ
        }
    }
   xTaskResumeAll();    //�ָ�OS����
}
/**
  **********************************************************************
  * @brief  DS18B20 ��ʼ������
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
  * @brief  ����DS18B20�õ���I/O��
  * @author  Lu
  * @version V1.0
  * @date    2024-03-12
  **********************************************************************
  */ 
static void BSP_DS18B20_GPIO_Config(void)
{
    /*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef BSP_DS18B20_GPIO_InitStructure;
	/*����DS18B20��ص�GPIO����ʱ��*/
	BSP_DS18B20_DQ_SCK_APBxClock_FUN( BSP_DS18B20_GPIO_CLK, ENABLE);
	/*��������ģʽΪͨ���������*/
	BSP_DS18B20_GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_Out_PP;
	/*������������Ϊ50MHz */   
	BSP_DS18B20_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	/*ѡ��Ҫ���Ƶ�GPIO����*/
	BSP_DS18B20_GPIO_InitStructure.GPIO_Pin = BSP_DS18B20_GPIO_PIN;	
	/*���ÿ⺯������ʼ��GPIO*/
	GPIO_Init(BSP_DS18B20_GPIO_PORT, &BSP_DS18B20_GPIO_InitStructure);
}

/**
  **********************************************************************
  * @brief  �������ӻ����͸�λ����
  * @author  Lu
  * @version V1.0
  * @date    2024-03-12
  **********************************************************************
  */ 
static void BSP_DS18B20_Reset(void)
{
    BSP_DS18B20_Mode_Out_PP();
    
    BSP_DS18B20_Low;
    /* �������ٲ���480us�ĵ͵�ƽ��λ�ź� */
    BSP_DS18B20_Delay(500);
    BSP_DS18B20_High;
    /* �ӻ����յ������ĸ�λ�źź󣬻���15~60us���������һ���������� */
    BSP_DS18B20_Delay(15);
}



/**
  **********************************************************************
  * @brief  ʹDS18B20-DATA���ű�Ϊ���ģʽ
  * @author  Lu
  * @version V1.0
  * @date    2024-03-12
  **********************************************************************
  */ 
static void BSP_DS18B20_Mode_Out_PP(void)
{
    /*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef BSP_DS18B20_GPIO_InitStructure;
	/*����DS18B20��ص�GPIO����ʱ��*/
	BSP_DS18B20_DQ_SCK_APBxClock_FUN( BSP_DS18B20_GPIO_CLK, ENABLE);
	/*��������ģʽΪͨ���������*/
	BSP_DS18B20_GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_Out_PP;
	/*������������Ϊ50MHz */   
	BSP_DS18B20_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	/*ѡ��Ҫ���Ƶ�GPIO����*/
	BSP_DS18B20_GPIO_InitStructure.GPIO_Pin = BSP_DS18B20_GPIO_PIN;	
	/*���ÿ⺯������ʼ��GPIO*/
	GPIO_Init(BSP_DS18B20_GPIO_PORT, &BSP_DS18B20_GPIO_InitStructure);
}


/**
  **********************************************************************
  * @brief  ʹDS18B20-DATA���ű�Ϊ����ģʽ
  * @author  Lu
  * @version V1.0
  * @date    2024-03-12
  **********************************************************************
  */ 
static void BSP_DS18B20_Mode_IPU(void)
{
    /*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef BSP_DS18B20_GPIO_InitStructure;
	/*����DS18B20��ص�GPIO����ʱ��*/
	BSP_DS18B20_DQ_SCK_APBxClock_FUN( BSP_DS18B20_GPIO_CLK, ENABLE);
	/*��������ģʽΪͨ���������*/
	BSP_DS18B20_GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_IPU;
	/*ѡ��Ҫ���Ƶ�GPIO����*/
	BSP_DS18B20_GPIO_InitStructure.GPIO_Pin = BSP_DS18B20_GPIO_PIN;	
	/*���ÿ⺯������ʼ��GPIO*/
	GPIO_Init(BSP_DS18B20_GPIO_PORT, &BSP_DS18B20_GPIO_InitStructure);
}




/**
  **********************************************************************
  * @brief   ���ӻ����������صĴ�������
  * @retval	��0���ɹ� 1��ʧ��
  * @author  Lu
  * @version V1.0
  * @date    2024-03-12
  **********************************************************************
  */ 
static uint8_t BSP_DS18B20_Presence(void)
{
    uint8_t pulse_time = 0;
    /* ��������Ϊ�������� */
    BSP_DS18B20_Mode_IPU();
    /* �ȴ���������ĵ�������������Ϊһ��60~240us�ĵ͵�ƽ�ź� 
	 * �����������û����������ʱ�����ӻ����յ������ĸ�λ�źź󣬻���15~60us���������һ����������
	 */
    while( BSP_DS18B20_IN() && pulse_time<100 )
	{
		pulse_time++;
        BSP_DS18B20_Delay(1);
	}	
	/* ����100us�󣬴������嶼��û�е���*/
	if( pulse_time >=100 )
		return 1;
	else
		pulse_time = 0;
	
	/* �������嵽�����Ҵ��ڵ�ʱ�䲻�ܳ���240us */
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
  * @brief   ��DS18B20��ȡһ��bit
  * @author  Lu
  * @version V1.0
  * @date    2024-03-12
  **********************************************************************
  */ 
static uint8_t BSP_DS18B20_ReadBit(void)
{
	uint8_t dat;
	
	/* ��0�Ͷ�1��ʱ������Ҫ����60us */	
	BSP_DS18B20_Mode_Out_PP();
	/* ��ʱ�����ʼ���������������� >1us <15us �ĵ͵�ƽ�ź� */
	BSP_DS18B20_Low;
    BSP_DS18B20_Delay(10); //10us
	
	/* ���ó����룬�ͷ����ߣ����ⲿ�������轫�������� */
	BSP_DS18B20_Mode_IPU();
	
	if( BSP_DS18B20_IN() == SET )
		dat = 1;
	else
		dat = 0;
	
	/* �����ʱ������ο�ʱ��ͼ 45us */
    BSP_DS18B20_Delay(45); //45us
	
	return dat;
}

/**
  **********************************************************************
  * @brief   ��DS18B20��һ���ֽڣ���λ����
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
  * @brief   дһ���ֽڵ�DS18B20����λ����
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
		/* д0��д1��ʱ������Ҫ����60us */
		if (testb)
		{			
			BSP_DS18B20_Low;
			/* 1us < �����ʱ < 15us */
            BSP_DS18B20_Delay(10); //10us
            
			BSP_DS18B20_High;
            /* �����ʱ 58us */
			BSP_DS18B20_Delay(58); //58us
		}		
		else
		{			
			BSP_DS18B20_Low;
			/* 60us < Tx 0 < 120us */
            BSP_DS18B20_Delay(70); //70us
			BSP_DS18B20_High;			
			/* 1us < Trec(�ָ�ʱ��) < �����*/
            BSP_DS18B20_Delay(2); //2us
		}
	}
}


/**
  **********************************************************************
  * @brief   ����ƥ�� DS18B20 ROM
  * @author  Lu
  * @version V1.0
  * @date    2024-03-13
  **********************************************************************
  */ 
static void BSP_DS18B20_SkipRom ( void )
{
	BSP_DS18B20_Reset();	   	
	BSP_DS18B20_Presence();	 
	BSP_DS18B20_WriteByte(0XCC);		/* ���� ROM */
}

/**
  **********************************************************************
  * @brief   ִ��ƥ�� DS18B20 ROM
  * @author  Lu
  * @version V1.0
  * @date    2024-03-13
  **********************************************************************
  */ 
static void BSP_DS18B20_MatchRom ( void )
{
	BSP_DS18B20_Reset();	   
	BSP_DS18B20_Presence();	 
	BSP_DS18B20_WriteByte(0X55);		/* ƥ�� ROM */
}


/*
 * �洢���¶���16 λ�Ĵ�������չ�Ķ����Ʋ�����ʽ
 * ��������12λ�ֱ���ʱ������5������λ��7������λ��4��С��λ
 *
 *         |---------����----------|-----С�� �ֱ��� 1/(2^4)=0.0625----|
 * ���ֽ�  | 2^3 | 2^2 | 2^1 | 2^0 | 2^(-1) | 2^(-2) | 2^(-3) | 2^(-4) |
 *
 *
 *         |-----����λ��0->��  1->��-------|-----------����-----------|
 * ���ֽ�  |  s  |  s  |  s  |  s  |    s   |   2^6  |   2^5  |   2^4  |
 *
 * 
 * �¶� = ����λ + ���� + С��*0.0625
 */
/**
  **********************************************************************
  * @brief   ������ƥ�� ROM ����»�ȡ DS18B20 �¶�ֵ 
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
	BSP_DS18B20_WriteByte(0X44);				/* ��ʼת�� */
	
	
	BSP_DS18B20_SkipRom ();
    BSP_DS18B20_WriteByte(0XBE);				/* ���¶�ֵ */
	
	tplsb = BSP_DS18B20_ReadByte();		 
	tpmsb = BSP_DS18B20_ReadByte(); 
	
	
	s_tem = tpmsb<<8;
	s_tem = s_tem | tplsb;
	
	if( s_tem < 0 )		/* ���¶� */
		f_tem = (~s_tem+1) * 0.0625;	
	else
		f_tem = s_tem * 0.0625;
	
	return f_tem; 	

}

/**
  **********************************************************************
  * @brief   ��ƥ�� ROM ����»�ȡ DS18B20 �¶�ֵ
  * @param  ds18b20_id�����ڴ�� DS18B20 ���кŵ�������׵�ַ
  * @author  Lu
  * @version V1.0
  * @date    2024-03-13
  **********************************************************************
  */ 
void BSP_DS18B20_ReadId ( uint8_t * ds18b20_id )
{
	uint8_t uc;
	BSP_DS18B20_WriteByte(0x33);       //��ȡ���к�
	
	for ( uc = 0; uc < 8; uc ++ )
	  ds18b20_id [ uc ] = BSP_DS18B20_ReadByte();
}


/**
  **********************************************************************
  * @brief   ��ƥ�� ROM ����»�ȡ DS18B20 �¶�ֵ
  * @param  ds18b20_id�����ڴ�� DS18B20 ���кŵ�������׵�ַ
  * @retval �¶�ֵ
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
	
	BSP_DS18B20_MatchRom ();            //ƥ��ROM
	
  for(i=0;i<8;i++)
		BSP_DS18B20_WriteByte ( ds18b20_id [ i ] );	
	
	BSP_DS18B20_WriteByte(0X44);				/* ��ʼת�� */

	BSP_DS18B20_MatchRom ();            //ƥ��ROM
	
	for(i=0;i<8;i++)
		BSP_DS18B20_WriteByte ( ds18b20_id [ i ] );	
	
	BSP_DS18B20_WriteByte(0XBE);				/* ���¶�ֵ */
	
	tplsb = BSP_DS18B20_ReadByte();		 
	tpmsb = BSP_DS18B20_ReadByte(); 

	s_tem = tpmsb<<8;
	s_tem = s_tem | tplsb;
	
	if( s_tem < 0 )		/* ���¶� */
		f_tem = (~s_tem+1) * 0.0625;	
	else
		f_tem = s_tem * 0.0625;
	
	return f_tem; 	

}

/* END OF FILE ------------------------------------------------------------------*/


