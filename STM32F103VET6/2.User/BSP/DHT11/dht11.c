/* ����ͷ�ļ� ------------------------------------------------------------------*/  
#include "./DHT11/dht11.h"  //����dht11����ͷ�ļ�

/* ����ȫ�ֱ��� ------------------------------------------------------------------*/

/**
 * @brief  ��DHT11���ŵ��������Ϊ�����������
 * @param  ����
 * @retval ����
 * @doc    ����
 */
void DHT11_Output( void )
{
		GPIO_InitTypeDef  DHT11_GPIO_InitStructure;
		RCC_APB2PeriphClockCmd( DHT11_GPIO_RCC_APB2, ENABLE);	//ʹ��LED��GPIO�˿�ʱ��
		
		DHT11_GPIO_InitStructure.GPIO_Mode		=			GPIO_Mode_Out_PP;	//�������ŵ��������Ϊ�������
		DHT11_GPIO_InitStructure.GPIO_Pin			=			DHT11_GPIO_Pin;
		DHT11_GPIO_InitStructure.GPIO_Speed		=			GPIO_Speed_50MHz;
	
		GPIO_Init(DHT11_GPIOX,&DHT11_GPIO_InitStructure);
}


/**
 * @brief  ��DHT11���ŵ��������Ϊ�������뺯��
 * @param  ����
 * @retval ����
 * @doc    ����
 */
void DHT11_Input( void )
{
		GPIO_InitTypeDef  DHT11_GPIO_InitStructure;
		RCC_APB2PeriphClockCmd( DHT11_GPIO_RCC_APB2, ENABLE);	//ʹ��LED��GPIO�˿�ʱ��
		
		DHT11_GPIO_InitStructure.GPIO_Mode		=			GPIO_Mode_IPU;	//�������ŵ��������Ϊ��������
		DHT11_GPIO_InitStructure.GPIO_Pin			=			DHT11_GPIO_Pin;
	
		GPIO_Init(DHT11_GPIOX,&DHT11_GPIO_InitStructure);	
}

/**
 * @brief  ��DHT11��ȡһ���ֽ� ��λ����
 * @param  ����
 * @retval ��dat �����ض�ȡ��һ���ֽ�
 * @doc    ����
 */
static uint8_t DHT11_ReadByte( void )
{
	uint8_t i,dat;
	
	for(i=0;i<8;i++)
	{
		/*ÿbit��50us�͵�ƽ���ÿ�ʼ����ѯֱ���ӻ����� ��50us �͵�ƽ ����*/
		while(GPIO_ReadInputDataBit (DHT11_GPIOX,DHT11_GPIO_Pin) == 0);
		
		/*DHT11 ��26~28us�ĸߵ�ƽ��ʾ��0������70us�ߵ�ƽ��ʾ��1����
		 *ͨ����� x us��ĵ�ƽ��������������״ ��x ���������ʱ 
		 */
		SysTick_Delay_us(30);	//��ʱ30us
		if(GPIO_ReadInputDataBit (DHT11_GPIOX,DHT11_GPIO_Pin) == 1 ) //�ߵ�ƽ
		{
			while(GPIO_ReadInputDataBit (DHT11_GPIOX,DHT11_GPIO_Pin));	//�ȴ��ߵ�ƽ����
			dat |= (uint8_t)(0x01<<(7-i));	//�ѵ�7-iλ��1��MSB����
		}
		else
		{
			dat &= (uint8_t)~(0x01<<(7-i));	//�ѵ�7-iλ��1��MSB����
		}
	}
	return dat;	//������ֵ
}

/**
 * @brief  ��DHT11��ȡһ�����ݣ�40bit�� ��λ����
 * @param  ��8bit ʪ������ + 8bit ʪ��С�� + 8bit �¶����� + 8bit �¶�С�� + 8bit У��� 
 * @retval ��dat �����ض�ȡ��һ���ֽ�
 * @doc    ����
 */
uint8_t DHT11_Read_TempAndHumidity(DHT11_Data_TypeDef *DHT11_Data)
{
	DHT11_Output();	//���ģʽ
	
	/*����������ʼ�ź�*/
	GPIO_ResetBits(DHT11_GPIOX,DHT11_GPIO_Pin);	//�������Ͳ�С��18ms�ĵ�ƽ
	SysTick_Delay_ms(18);
	
	/*�������ߵ�ƽ�ͷ�����*/
	GPIO_SetBits(DHT11_GPIOX,DHT11_GPIO_Pin);	//������������
	SysTick_Delay_us(30);	//��ʱ30us
	
	DHT11_Input();	//DHT11���ŵ��������Ϊ�������뺯��
	
	if(GPIO_ReadInputDataBit (DHT11_GPIOX,DHT11_GPIO_Pin) == 0)
	{
		/*�ӻ���Ӧ�ź�*/
		while(GPIO_ReadInputDataBit (DHT11_GPIOX,DHT11_GPIO_Pin) == 0);	//�ӻ����͵�ƽ83us
		while(GPIO_ReadInputDataBit (DHT11_GPIOX,DHT11_GPIO_Pin) == 1);	//�ӻ����ߵ�ƽ87us
		
		/*��ʼ��������*/
		DHT11_Data->humi_int 	= DHT11_ReadByte();	//8bit ʪ������
		DHT11_Data->humi_deci = DHT11_ReadByte();	//8bit ʪ��С��
		DHT11_Data->temp_int 	= DHT11_ReadByte();	//8bit �¶�����
		DHT11_Data->temp_deci = DHT11_ReadByte();	//8bit �¶�С��
		DHT11_Data->check_sum = DHT11_ReadByte();	//8bit У���
		
		/*��ȡ���������Ÿ�Ϊ���ģʽ*/
		DHT11_Output();	//DHT11���ŵ��������Ϊ�����������
		GPIO_SetBits(DHT11_GPIOX,DHT11_GPIO_Pin);	//������������
		
		if(DHT11_Data->check_sum == DHT11_Data->humi_int + DHT11_Data->humi_deci + DHT11_Data->temp_int+ DHT11_Data->temp_deci)
		{
//			DHT11_Data->humi_int 	= (DHT11_Data->humi_int /10)*16 + (DHT11_Data->humi_int %10); //ʪ������ת��Ϊ10����
//			DHT11_Data->humi_deci = (DHT11_Data->humi_deci /10)*16 + (DHT11_Data->humi_deci %10); //ʪ��С��ת��Ϊ10����
//			DHT11_Data->temp_int 	= (DHT11_Data->temp_int /10)*16 + (DHT11_Data->temp_int %10); //�¶�����ת��Ϊ10����
//			DHT11_Data->temp_deci = (DHT11_Data->temp_deci /10)*16 + (DHT11_Data->temp_deci %10); //�¶�С��ת��Ϊ10����
			return 1;	//����У��ɹ�
		}
		else
			return 0;	//����У��ʧ��
	}
	else
		return 0;	//����У��ʧ��
}

/* END OF FILE ------------------------------------------------------------------*/

