/* ����ͷ�ļ� ------------------------------------------------------------------*/  
#include "./MOTOR/bsp_encoder.h"  //���ñ���������ͷ�ļ�

/* ����ȫ�ֱ��� ------------------------------------------------------------------*/
volatile uint16_t BSP_ENCODER_Overflow_Count = 0;   //��ʱ���������


/* ����ʼ ----------------------------------------------------------------------*/

/**
  **********************************************************************
  * @brief  ������ �ж����ȼ�����
  * @author  Lu
  * @version V1.0
  * @date    2024-04-16
  **********************************************************************
  */ 
//static void GENERAL_TIM_NVIC_Config(void)
//{
//    NVIC_InitTypeDef NVIC_InitStructure; 
//    // �����ж���Ϊ0
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
//	// �����ж���Դ
//    NVIC_InitStructure.NVIC_IRQChannel = BSP_ENCODER_TIM_IRQ ;	
//	// ���������ȼ�Ϊ 0
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
//	// ������ռ���ȼ�Ϊ3
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
//}


/**
  **********************************************************************
  * @brief  ������������ú���(���벶��ͨ������)
  * @author  Lu
  * @version V1.0
  * @date    2024-xx-xx
  **********************************************************************
  */ 
static void BSP_ENCODER_GPIO_Config(void)
{
    /* ����һ��GPIO_InitTypeDef���͵Ľṹ�� */
	GPIO_InitTypeDef BSP_ENCODER_GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(BSP_ENCODER_A_GPIO_CLK,ENABLE);//����ʱ��
    BSP_ENCODER_GPIO_InitStructure.GPIO_Mode    =   GPIO_Mode_IN_FLOATING;  //��������ģʽ
    BSP_ENCODER_GPIO_InitStructure.GPIO_Pin     =   BSP_ENCODER_A_GPIO_PIN;
    BSP_ENCODER_GPIO_InitStructure.GPIO_Speed   =   GPIO_Speed_50MHz;
    /*���ÿ⺯������ʼ��GPIO*/
	GPIO_Init(BSP_ENCODER_A_GPIO_PORT, &BSP_ENCODER_GPIO_InitStructure);
    
    RCC_APB2PeriphClockCmd(BSP_ENCODER_B_GPIO_CLK,ENABLE);//����ʱ��
    BSP_ENCODER_GPIO_InitStructure.GPIO_Mode    =   GPIO_Mode_IN_FLOATING;  //��������ģʽ
    BSP_ENCODER_GPIO_InitStructure.GPIO_Pin     =   BSP_ENCODER_B_GPIO_PIN;
    BSP_ENCODER_GPIO_InitStructure.GPIO_Speed   =   GPIO_Speed_50MHz;
    /*���ÿ⺯������ʼ��GPIO*/
	GPIO_Init(BSP_ENCODER_B_GPIO_PORT, &BSP_ENCODER_GPIO_InitStructure);
}

/**
  **********************************************************************
  * @brief  ������ ��ʱ�����ú���
  * @author  Lu
  * @version V1.0
  * @date    2024-xx-xx
  **********************************************************************
  */ 
static void BSP_ENCODER_TIM_Config(void)
{
    RCC_APB1PeriphClockCmd(BSP_ENCODER_TIM_CLK,ENABLE); // ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
    /*--------------------ʱ���ṹ���ʼ��-------------------------*/
	
    /* ����һ�� TIM_TimeBaseInitTypeDef ���͵Ľṹ�� */
    TIM_TimeBaseInitTypeDef		BSP_ENCODER_TIM_TimeBaseStructure;
	// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
	BSP_ENCODER_TIM_TimeBaseStructure.TIM_Period=BSP_ENCODER_TIM_PERIOD;	
	// ����CNT��������ʱ�� = Fck_int/(psc+1)
	BSP_ENCODER_TIM_TimeBaseStructure.TIM_Prescaler= BSP_ENCODER_TIM_PSC;	
	// ʱ�ӷ�Ƶ���� ����������ʱ��ʱ��Ҫ�õ�
	BSP_ENCODER_TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// ����������ģʽ������Ϊ���ϼ���
	BSP_ENCODER_TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// �ظ���������ֵ��û�õ����ù�
	BSP_ENCODER_TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// ��ʼ����ʱ��
	TIM_TimeBaseInit(BSP_ENCODER_TIM, &BSP_ENCODER_TIM_TimeBaseStructure);

    /*--------------------���벶��ṹ���ʼ��-------------------*/	
	TIM_ICInitTypeDef BSP_ENCODER_TIM_ICInitStructure;
    
    TIM_ICStructInit(&BSP_ENCODER_TIM_ICInitStructure); //��TIM_ICInitStruct �е�ÿһ��������ȱʡֵ����
    
    /*-------------------- IC1 -------------------*/	
	// �������벶���ͨ������Ҫ���ݾ����GPIO������
	BSP_ENCODER_TIM_ICInitStructure.TIM_Channel = BSP_ENCODER_TIM_CHANNEL_A;
    // �������Ҫ��������źŵ��˲�ϵ��
	BSP_ENCODER_TIM_ICInitStructure.TIM_ICFilter = 15;
    // ��ʱ�����벶���ʼ��
	TIM_ICInit(BSP_ENCODER_TIM, &BSP_ENCODER_TIM_ICInitStructure);
    /*-------------------- IC2 -------------------*/	
    // �������벶���ͨ������Ҫ���ݾ����GPIO������
	BSP_ENCODER_TIM_ICInitStructure.TIM_Channel = BSP_ENCODER_TIM_CHANNEL_B;
    // �������Ҫ��������źŵ��˲�ϵ��
	BSP_ENCODER_TIM_ICInitStructure.TIM_ICFilter = 15;
    // ��ʱ�����벶���ʼ��
	TIM_ICInit(BSP_ENCODER_TIM, &BSP_ENCODER_TIM_ICInitStructure);
    
    /* ���������� ����ʱ�� ģʽ3 �����غ��½��ض����� */
    TIM_EncoderInterfaceConfig(BSP_ENCODER_TIM,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising); //ʹ�ñ�����ģʽ3
    
//    TIM_EncoderInterfaceConfig(BSP_ENCODER_TIM,TIM_EncoderMode_TI12,TIM_ICPolarity_BothEdge,TIM_ICPolarity_BothEdge); //ʹ�ñ�����ģʽ3
    
    
//	// ���벶���źŵļ�������
//	BSP_ENCODER_TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge; //˫���ش�������
//	// ����ͨ���Ͳ���ͨ����ӳ���ϵ����ֱ���ͷ�ֱ������
//	BSP_ENCODER_TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
//	// �������Ҫ��������źŵķ�Ƶϵ��
//	BSP_ENCODER_TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
//	// �������Ҫ��������źŵ��˲�ϵ��
//	BSP_ENCODER_TIM_ICInitStructure.TIM_ICFilter = 10;
//	// ��ʱ�����벶���ʼ��
//	TIM_ICInit(BSP_ENCODER_TIM, &BSP_ENCODER_TIM_ICInitStructure);

    //���ö�ʱ������
    TIM_SetCounter(BSP_ENCODER_TIM,BSP_ENCODER_Num);
	// ������ºͲ����жϱ�־λ
    TIM_ClearFlag(BSP_ENCODER_TIM, TIM_FLAG_Update|BSP_ENCODER_TIM_IT_CCx);	
    // �������ºͲ����ж�  
	TIM_ITConfig (BSP_ENCODER_TIM, TIM_IT_Update | BSP_ENCODER_TIM_IT_CCx, ENABLE );
    
//    TIM_SetCounter(BSP_ENCODER_TIM,0);
	TIM_Cmd(BSP_ENCODER_TIM, ENABLE);   // ʹ�ܼ�����
}

/**
  **********************************************************************
  * @brief  ������ ��ʼ������
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
  * @brief  ������ ��ȡ��������
  * @param	BSP_ENCODER_TIMX:��ʱ����ȡֵ�ο�Ϊ TIM3 TIM4
  * @retval CNT:��������ֵ(���Ի���Ϊ�ٶ�)
  * @author  Lu
  * @version V1.0
  * @date    2024-xx-xx
  **********************************************************************
  */ 
uint16_t BSP_ENCODER_Read_CNT(TIM_TypeDef* BSP_ENCODER_TIMX)
{
    uint16_t CNT =0;
    CNT = (short)TIM_GetCounter(BSP_ENCODER_TIM); //��ȡ����ֵ
    TIM_SetCounter(BSP_ENCODER_TIM,BSP_ENCODER_Num);//����
    return CNT;
}

/**
  **********************************************************************
  * @brief   �������жϷ�����
  * @author  Lu
  * @version V1.0
  * @date    2024-04-16
  **********************************************************************
  */ 
void BSP_ENCODER_TIM_INT_FUN()
{
    if(BSP_ENCODER_TIM->SR & 0x0001)//Overflow interrupt //����ж�
    {
    }
    BSP_ENCODER_TIM->SR&=~(1<<0);//Clear the interrupt flag bit //����жϱ�־λ 
}


/**
  **********************************************************************
  * @brief  ������ ͨ������ֵ�����ٶȺ���
  * @author  Lu
  * @version V1.0
  * @date    2024-04-16
  **********************************************************************
  */ 
float BSP_ENCODER_Read_CNT_Speed(TIM_TypeDef* BSP_ENCODER_TIMX)
{
    float Speed;
    Speed = BSP_ENCODER_Read_CNT(BSP_ENCODER_TIM) * BSP_ENCODER_FREQUENCY* BSP_ENCODER_Tire_Length * 3.1416 / BSP_ENCODER_Reduction_Ratio; //�ٶ� = ����������ֵ * ��̥�ܳ� * 3.1416 * ��������ȡƵ�� / ������������330��
    return Speed;
}

/* END OF FILE ------------------------------------------------------------------*/

