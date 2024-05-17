/* ����ͷ�ļ� ------------------------------------------------------------------*/  
#include "./MOTOR/bsp_motor.h"  //���õ������ͷ�ļ�

/* ����ȫ�ֱ��� ------------------------------------------------------------------*/


/* ����ʼ ----------------------------------------------------------------------*/

/**
  * @brief    BSP_MOTOR_GPIO_Config(void);	//ͨ�ö�ʱ��3����Ƚ�ͨ�� GPIO�������ú���
  * @param  ������˵��
  * @arg    �����������ѡ������о�
  * @note   ��ע��
  * @retval ������ֵ˵��
  * @doc    ����
  */
void BSP_MOTOR_GPIO_Config(void)
{
	/*	���� BSP_MOTOR_GPIO_InitStructure �ṹ��	*/
	GPIO_InitTypeDef BSP_MOTOR_GPIO_InitStructure;

    // ����Ƚ�ͨ��1 GPIO ��ʼ��  PA6
    RCC_APB2PeriphClockCmd(BSP_MOTOR_TIM_CH1_GPIO_CLK, ENABLE);
    BSP_MOTOR_GPIO_InitStructure.GPIO_Pin =  BSP_MOTOR_TIM_CH1_PIN;
    BSP_MOTOR_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    BSP_MOTOR_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BSP_MOTOR_TIM_CH1_PORT, &BSP_MOTOR_GPIO_InitStructure);

	// ����Ƚ�ͨ��2 GPIO ��ʼ��	PA7
    RCC_APB2PeriphClockCmd(BSP_MOTOR_TIM_CH2_GPIO_CLK, ENABLE);
    BSP_MOTOR_GPIO_InitStructure.GPIO_Pin =  BSP_MOTOR_TIM_CH2_PIN;
    BSP_MOTOR_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    BSP_MOTOR_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BSP_MOTOR_TIM_CH2_PORT, &BSP_MOTOR_GPIO_InitStructure);
	
	// ����Ƚ�ͨ��3 GPIO ��ʼ��	PB0
    RCC_APB2PeriphClockCmd(BSP_MOTOR_TIM_CH3_GPIO_CLK, ENABLE);
    BSP_MOTOR_GPIO_InitStructure.GPIO_Pin =  BSP_MOTOR_TIM_CH3_PIN;
    BSP_MOTOR_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    BSP_MOTOR_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BSP_MOTOR_TIM_CH3_PORT, &BSP_MOTOR_GPIO_InitStructure);

	// ����Ƚ�ͨ��4 GPIO ��ʼ��	PB1
    RCC_APB2PeriphClockCmd(BSP_MOTOR_TIM_CH4_GPIO_CLK, ENABLE);
    BSP_MOTOR_GPIO_InitStructure.GPIO_Pin =  BSP_MOTOR_TIM_CH4_PIN;
    BSP_MOTOR_GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    BSP_MOTOR_GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BSP_MOTOR_TIM_CH4_PORT, &BSP_MOTOR_GPIO_InitStructure);
}


///*
// * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
// * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
// * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
// *-----------------------------------------------------------------------------
// *typedef struct
// *{ TIM_Prescaler            ����
// *	TIM_CounterMode			     TIMx,x[6,7]û�У���������
// *  TIM_Period               ����
// *  TIM_ClockDivision        TIMx,x[6,7]û�У���������
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]����
// *}TIM_TimeBaseInitTypeDef; 
// *-----------------------------------------------------------------------------
// */

/* ----------------   PWM�ź� ���ں�ռ�ձȵļ���--------------- */
// ARR ���Զ���װ�ؼĴ�����ֵ
// CLK_cnt����������ʱ�ӣ����� Fck_int / (psc+1) = 72M/(psc+1)
// PWM �źŵ����� T = ARR * (1/CLK_cnt) = ARR*(PSC+1) / 72M
// ռ�ձ�P=CCR/(ARR+1)

 /**
  * @brief  �����ʹ��ͨ�ö�ʱ������PWM ������PWM
  * @param  ��timer	������ʱ�� ȡֵ��TIMx  x={1��2��3��4}
  * @param  ��tim_channel����ͨ��		ȡֵ��tim_channel={1��2��3��4}
  * @param  ��frequency����PWMƵ��(KHz)  ���磺freq = 10 KHz
  * @param  ��Duty_cycle����PWMռ�ձ�(%)	���磺duty = 20 %
  * @note   ��ע��
  * @doc    ����
  */
void BSP_MOTOR_PWM_SET(TIM_TypeDef* timer,uint8_t tim_channel,uint16_t frequency,uint16_t Duty_cycle)
{
	uint16_t	ARR	=	(SystemCoreClock/1000)/frequency-1;	//�趨�Զ���װ�ؼĴ������ڵ���ֵ��freq��λΪKHz��һ��ֱ��ʹ��72000
	// ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
	BSP_MOTOR_TIM_APBxClock_FUN(BSP_MOTOR_TIM_CLK,ENABLE);	//ʾ���򿪵���ͨ�ö�ʱ��3��ʹ��ʱ��
	TIM_TimeBaseInitTypeDef GeneralTim_PWM;	//����ṹ��
	// ����CNT��������ʱ�� = Fck_int/(psc+1)
	GeneralTim_PWM.TIM_Prescaler			=	0;	//����Ԥ��Ƶ
	// ����������ģʽ������Ϊ���ϼ���
	GeneralTim_PWM.TIM_CounterMode			= TIM_CounterMode_Up;	//���ϼ���ģʽ
    
	// �Զ���װ�ؼĴ�����ֵ���ۼ�ARR+1��Ƶ�ʺ����һ�����»����ж�
	GeneralTim_PWM.TIM_Period				=	ARR;	//ָ��Ҫ���ص���е��ڼ�ֵ

	// ʱ�ӷ�Ƶ���� ����������ʱ��ʱ��Ҫ�õ�
	GeneralTim_PWM.TIM_ClockDivision		=	TIM_CKD_DIV1;	//����ʱ�ӷ�Ƶϵ��������Ƶ(�����ò���)

	// �ظ���������ֵ��û�õ����ù�
	GeneralTim_PWM.TIM_RepetitionCounter	=	0;
	
	// ��ʼ����ʱ��
	TIM_TimeBaseInit(timer, &GeneralTim_PWM);
	
	/*	���� TIM_OCInitStructure �ṹ��	*/
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	/*	����ΪPWMģʽ1	*/
	TIM_OCInitStructure.TIM_OCMode			=		TIM_OCMode_PWM1;
	/*	���ʹ��	*/
	TIM_OCInitStructure.TIM_OutputState		=		TIM_OutputState_Enable;
	/*	����PWM����ռ�ձ�	*/
	TIM_OCInitStructure.TIM_Pulse			=		(ARR+1)*Duty_cycle/100-1;
	/*	���ͨ����ƽ��������	*/
	TIM_OCInitStructure.TIM_OCPolarity		=		TIM_OCPolarity_Low;	//�͵�ƽ
	
	switch(tim_channel)
	{
		case 1:
				TIM_OC1Init(timer, &TIM_OCInitStructure);	 									//ʹ��ͨ��3
				TIM_OC1PreloadConfig(timer, TIM_OCPreload_Enable);
			break;
		case 2:
				TIM_OC2Init(timer, &TIM_OCInitStructure);	 									//ʹ��ͨ��3
				TIM_OC2PreloadConfig(timer, TIM_OCPreload_Enable);
			break;
		case 3:
				TIM_OC3Init(timer, &TIM_OCInitStructure);	 									//ʹ��ͨ��3
				TIM_OC3PreloadConfig(timer, TIM_OCPreload_Enable);
			break;
		case 4:
				TIM_OC4Init(timer, &TIM_OCInitStructure);	 									//ʹ��ͨ��3
				TIM_OC4PreloadConfig(timer, TIM_OCPreload_Enable);
			break;
	}
    TIM_ARRPreloadConfig(timer, ENABLE);//ʹ�ܶ�ʱ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���	ʹ��TIM3���ؼĴ���ARR
    /* ʹ�ܶ�ʱ��TIMx	*/
    TIM_Cmd(timer, ENABLE);//ʹ�ܶ�ʱ��TIMx
}


/**
  **********************************************************************
  * @brief  ���õ��ת���ٶȣ�����TIMͨ����ռ�ձ�
  * @param	channel - ѡ��ͨ��
  * @param	Duty_cycle - ѡ��ռ�ձȣ�����ռ�ձ�Ϊ����
  * @author  Lu
  * @version V1.0
  * @date    2024-04-11
  **********************************************************************
  */ 
void BSP_MOTOR_Set_PWM_Pulse(uint8_t channel,uint16_t Duty_cycle)
{
    /*	���� TIM_OCInitStructure �ṹ��	*/
	TIM_OCInitTypeDef  TIM_OCInitStructure;
    
    TIM_OCInitStructure.TIM_Pulse	= Duty_cycle;
    switch(channel)
	{
		case 1:
				TIM_OC1Init(BSP_MOTOR_TIM, &TIM_OCInitStructure);	 									//ʹ��ͨ��3
				TIM_OC1PreloadConfig(BSP_MOTOR_TIM, TIM_OCPreload_Enable);
			break;
		case 2:
				TIM_OC2Init(BSP_MOTOR_TIM, &TIM_OCInitStructure);	 									//ʹ��ͨ��3
				TIM_OC2PreloadConfig(BSP_MOTOR_TIM, TIM_OCPreload_Enable);
			break;
		case 3:
				TIM_OC3Init(BSP_MOTOR_TIM, &TIM_OCInitStructure);	 									//ʹ��ͨ��3
				TIM_OC3PreloadConfig(BSP_MOTOR_TIM, TIM_OCPreload_Enable);
			break;
		case 4:
				TIM_OC4Init(BSP_MOTOR_TIM, &TIM_OCInitStructure);	 									//ʹ��ͨ��3
				TIM_OC4PreloadConfig(BSP_MOTOR_TIM, TIM_OCPreload_Enable);
			break;
	}
}



/**
  **********************************************************************
  * @brief  ���ͣת���������ͨ����ֹͣ
  * @author  Lu
  * @param	timer - ѡ��ʱ��
  * @version V1.0
  * @date    2024-04-11
  **********************************************************************
  */ 
void BSP_MOTOR_STOP(TIM_TypeDef* timer)
{
    /* ʧ��TIM */
    TIM_Cmd(timer, ENABLE);//ʹ�ܶ�ʱ��TIMx
    /* ʹIN1 IN2 IN3 IN4 ������� */
    GPIO_ResetBits(BSP_MOTOR_TIM_CH1_PORT,BSP_MOTOR_TIM_CH1_PIN);
    GPIO_ResetBits(BSP_MOTOR_TIM_CH2_PORT,BSP_MOTOR_TIM_CH2_PIN);
    GPIO_ResetBits(BSP_MOTOR_TIM_CH3_PORT,BSP_MOTOR_TIM_CH3_PIN);
    GPIO_ResetBits(BSP_MOTOR_TIM_CH4_PORT,BSP_MOTOR_TIM_CH4_PIN);
}


///**
//  **********************************************************************
//  * @brief  ���õ������
//  * @author  Lu
//  * @version V1.0
//  * @date    2024-04-11
//  **********************************************************************
//  */ 
//void BSP_MOTOR_Set_Direction(uint8_t motor_channel,bsp_motor_dir_t motor_dir)
//{
//    /* */
//    if(motor_dir == BSP_MOTOR_FWD) //��ת
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

