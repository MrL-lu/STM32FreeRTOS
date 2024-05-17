/* ����ͷ�ļ� ------------------------------------------------------------------*/  
#include "./Timbase/bsp_TiMbase.h"  //���û�����ʱ������ͷ�ļ�

/* ����ȫ�ֱ��� ------------------------------------------------------------------*/

/* ����ʼ ----------------------------------------------------------------------*/
static void BASIC_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // �����ж���Ϊ0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);		
		// �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = BASIC_TIM_IRQ ;	
		// ���������ȼ�Ϊ 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;	 
	  // ������ռ���ȼ�Ϊ3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

static void BASIC_TIM_Mode_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	
    // ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
    BASIC_TIM_APBxClock_FUN(BASIC_TIM_CLK, ENABLE);
    // �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
    TIM_TimeBaseStructure.TIM_Period = BASIC_TIM_Period;	
    // ʱ��Ԥ��Ƶ��Ϊ
    TIM_TimeBaseStructure.TIM_Prescaler= BASIC_TIM_Prescaler;
//    // ʱ�ӷ�Ƶ���� ��������ʱ��û�У����ù�
//    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;

//    //����������ģʽ��������ʱ��ֻ�����ϼ�����û�м���ģʽ������
//    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 

//    // �ظ���������ֵ��������ʱ��û�У����ù�
//    TIM_TimeBaseStructure.TIM_RepetitionCounter=0;

    // ��ʼ����ʱ��
    TIM_TimeBaseInit(BASIC_TIM, &TIM_TimeBaseStructure);

    // ����������жϱ�־λ
    TIM_ClearFlag(BASIC_TIM, TIM_FLAG_Update);

    // �����������ж�
    TIM_ITConfig(BASIC_TIM,TIM_IT_Update,ENABLE);

    // ʹ�ܼ�����
    TIM_Cmd(BASIC_TIM, DISABLE);	
}

void BASIC_TIM_Init(void)
{
	BASIC_TIM_NVIC_Config();
	BASIC_TIM_Mode_Config();
}


/**
  **********************************************************************
  * @brief  ��ȡ��������������ջ��������ƿ��ڴ�
  * @param  ��frequency����PWMƵ��(KHz)  ���磺freq = 1 KHz = 1ms �ж�һ��
  * @param  ��frequency����PWMƵ��(KHz)  ���磺freq = 1 KHz = 1ms �ж�һ��
  * @author  Lu
  * @version V1.0
  * @date    2024-xx-xx
  **********************************************************************
  */
// ARR ���Զ���װ�ؼĴ�����ֵ
// CLK_cnt����������ʱ�ӣ����� Fck_int / (psc+1) = 72M/(psc+1)
// PWM �źŵ����� T = ARR * (1/CLK_cnt) = ARR*(PSC+1) / 72M
uint32_t BasicTime = 0;

void BASIC_TIM_Delay(uint16_t frequency,uint32_t delay_time)
{
    //�趨�Զ���װ�ؼĴ������ڵ���ֵ��freq��λΪKHz��ͨ��SystemCoreClock��ȡϵͳ��ʱ��Ƶ��
    uint16_t ARR = (SystemCoreClock/(frequency*1000))-1;

    BasicTime = delay_time; //������ʱʱ��
    
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	
    // ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
    BASIC_TIM_APBxClock_FUN(BASIC_TIM_CLK, ENABLE);
    // �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
    TIM_TimeBaseStructure.TIM_Period = ARR;	
    // ʱ��Ԥ��Ƶ��Ϊ
    TIM_TimeBaseStructure.TIM_Prescaler= BASIC_TIM_Prescaler;
//    // ʱ�ӷ�Ƶ���� ��������ʱ��û�У����ù�
//    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;

    // ��ʼ����ʱ��
    TIM_TimeBaseInit(BASIC_TIM, &TIM_TimeBaseStructure);

    // ����������жϱ�־λ
    TIM_ClearFlag(BASIC_TIM, TIM_FLAG_Update);

    // �����������ж�
    TIM_ITConfig(BASIC_TIM,TIM_IT_Update,ENABLE);

    // ʹ�ܼ�����
    TIM_Cmd(BASIC_TIM, ENABLE);
    
}

/* END OF FILE ------------------------------------------------------------------*/
