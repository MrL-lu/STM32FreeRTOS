/* ����ͷ�ļ� ------------------------------------------------------------------*/  
#include "./SysTick/systick.h"  //����ϵͳʱ��ͷ�ļ�

/**
 * @brief  ����ϵͳ�δ�ʱ�ӳ�ʼ������
 * @param  ����
 * @retval ����
 * @doc    ����
 */
void SysTick_Init(void) //ϵͳ�δ�ʱ�ӳ�ʼ������
{
    /* SystemCoreClock / 1000    1ms�ж�һ��
	   * SystemCoreClock / 100000	 10us�ж�һ��
	   * SystemCoreClock / 1000000 1us�ж�һ��
	   */
//	if (SysTick_Config(SystemFrequency / 100000))	// ST3.0.0��汾
	if ( SysTick_Config( SystemCoreClock / 1000000 ) )	// ST3.5.0��汾
	{ 
		/* Capture error */ 
		while (1);
	}
	// �رյδ�ʱ��  
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}

/**
 * @brief  ����ʱ΢�뺯��
 * @param  ����
 * @retval ����
 * @doc    ����
 */
void SysTick_Delay_us( uint32_t us )	//��ʱ΢�뺯��
{
	uint32_t i;	//����ֲ�����
	SysTick_Config(72); //д��reload �Ĵ���
	for(i=0;i<us;i++)
    {
      while(!((SysTick->CTRL)&(1<<16)));//�ж�ϵͳ��ʱ����־λ
    }
    // �رյδ�ʱ��
		SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}


/**
 * @brief  ����ʱ���뺯��
 * @param  ����
 * @retval ����
 * @doc    ����
 */
void SysTick_Delay_ms( uint32_t ms )	//����ʱ���뺯��
{
	uint32_t i;	//����ֲ�����
	SysTick_Config(72000); //д��reload �Ĵ���
	for(i=0;i<ms;i++)
   {
     while(!((SysTick->CTRL)&(1<<16)));//�ж�ϵͳ��ʱ����־λ
   }
    // �رյδ�ʱ��
		SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}




/* END OF FILE ------------------------------------------------------------------*/


