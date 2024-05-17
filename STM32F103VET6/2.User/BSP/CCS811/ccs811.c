/* �û�ͷ�ļ� ------------------------------------------------------------------*/
#include "./CCS811/ccs811.h"	//���ÿ���������CCS811

//����-SysTick.h ��ȫ�ֱ���
extern  uint8_t  Timer_flag ;//����ȫ�ֱ��� Timer_flag ��Ϊ�Ƿ���м�ʱ�ı�־λ
extern  uint16_t Timer_us ;//����ȫ�ֱ��� Timer_us ��Ϊ1us��ʱ
extern  uint16_t Timer_ms ;//����ȫ�ֱ��� Timer_us ��Ϊ1ms��ʱ
extern  uint16_t Timer_s  ;//����ȫ�ֱ��� Timer_us ��Ϊ1s��ʱ

//����-ccs811.h ��ȫ�ֱ���
unsigned char CSS811_USART_Data_Buff[100];//����CCS811 ���ڴ���������
unsigned char CCS811_USART_Data_End=0;//����CCS811 ���ڴ�����ɱ�־λ
static unsigned char Cnt=0;//�жϺ�����λ����

 /**
  * @brief  ����Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Ƕ�������жϿ�������ѡ�� */
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* ����USARTΪ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = CCS811_USART_IRQ;
  /* �������ȼ�*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* �����ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
  /* ʹ���ж� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* ��ʼ������NVIC */
  NVIC_Init(&NVIC_InitStructure);
}


/**
 * @brief  ��CCS811 ���������� ����GPIO���ú���
 * @param  ������˵��
 * @arg    �����������ѡ������о�
 * @note   ��ע��
 * @retval ������ֵ˵��
 * @doc    ����
 */
void CCS811_USART_GPIO_Config( void )//CCS811 ���������� ����GPIO���ú���
{
	/*  ����ṹ��   */
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
    
    /*  �򿪴���GPIO��ʱ��  */
	CCS811_USART1_GPIO_APBxClkCmd(CCS811_USART1_GPIO_CLK, ENABLE);
	 
    /*  �򿪴��������ʱ��  */
	CCS811_USART_APBxClkCmd_1(CCS811_USART_CLK_1, ENABLE);

    /*  ��USART Tx��GPIO����Ϊ���츴��ģʽ  */
	GPIO_InitStructure.GPIO_Pin = CCS811_USART1_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CCS811_USART1_TX_GPIO_PORT, &GPIO_InitStructure);

    /*  ��USART Rx��GPIO����Ϊ��������ģʽ  */
	GPIO_InitStructure.GPIO_Pin = CCS811_USART1_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(CCS811_USART1_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// ���ô��ڵĹ�������
    /*  ���ò�����   */
	USART_InitStructure.USART_BaudRate = CCS811_USART_BAUDRATE;
    /*  ���� �������ֳ�   */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    /*  ����ֹͣλ   */
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
    /*  ����У��λ   */
	USART_InitStructure.USART_Parity = USART_Parity_No ;
    /*  ����Ӳ��������   */
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    /*  ���ù���ģʽ���շ�һ��   */
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    /*  ��ɴ��ڵĳ�ʼ������   */
	USART_Init(CCS811_USART, &USART_InitStructure);	
	
    /*  �����ж����ȼ�����   */
	NVIC_Configuration();
    
    // ʹ�ܴ��ڽ����ж�
	USART_ITConfig(CCS811_USART, USART_IT_RXNE, ENABLE);
    
    /*  ʹ�ܴ���   */
	USART_Cmd(CCS811_USART, ENABLE);
}

/**
 * @brief  ��CCS811 ���������� ���ڷ��ͺ���
 * @param  ������˵��
 * @arg    �����������ѡ������о�
 * @note   ��ע��
 * @retval ������ֵ˵��
 * @doc    ����
 */
void CSS811_USART_Send(uint8_t *data,uint8_t len)
{
	uint8_t i;//����һ���ֲ�����
	USART_ClearFlag(CCS811_USART,USART_FLAG_TC);//��� CCS811 ���ô��ڴ�����ɱ�־λ
	for(i=0;i<len;i++)
	{
		USART_SendData(CCS811_USART,*(data+i));
		while(USART_GetFlagStatus(CCS811_USART,USART_FLAG_TC) == RESET);//�ȴ��������
	}
}


/**
 * @brief  ��CCS811 ���������� �����жϺ���
 * @param  ������˵��
 * @arg    �����������ѡ������о�
 * @note   ��ע��
 * @retval ������ֵ˵��
 * @doc    ����
 */
void CCS811_USART_IRQHandler( void )
{
	uint8_t CCS811_usartdata=0;	//����ֲ�����
	if (	USART_GetFlagStatus(CCS811_USART, USART_IT_RXNE) != RESET)	//�ж��Ƿ�Ϊ����
	{
		/*	�жϼĴ�����������	*/
		CCS811_usartdata = USART_ReceiveData (CCS811_USART); //��ȡ�Ĵ�������ֵ
		CSS811_USART_Data_Buff[Cnt++] = CCS811_usartdata;//�����ݴ��������		
		if(CCS811_usartdata == 0x0A)//���յ��˽�����
		{
			CCS811_USART_Data_End =1;//CCS811 ���ڴ�����ɱ�־λ
			Cnt =0;//���㣬�����´ν�������
		}
	}
	USART_ClearITPendingBit (CCS811_USART,USART_IT_RXNE);//��������־λ��������һ�ν�������

}


/**
 * @brief  ��CCS811 ���ݽ�������
 * @param  ������˵��
 * @arg    �����������ѡ������о�
 * @note   ��ע��
 * @retval ������ֵ˵��
 * @doc    ����
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
		printf("CO2Ũ��:%d ppm  TVOSŨ��:%d ppb\r\n",co2,tvos);
		
		*flag=0;
 }
	
}

/**
 * @brief  ��CCS811 ��������⺯��
 * @param  ������˵��
 * @arg    �����������ѡ������о�
 * @note   ��ע��
 * @retval ������ֵ˵��
 * @doc    ����
 */
void CCS811_Check(unsigned char* flag)
{
	Timer_flag =1;Timer_us=0;Timer_ms=0;//������ʱ
	
	CSS811_USART_Send("AT\r\n",4);//���� AT ָ��
  while((Timer_ms<100)&&(*flag==0));
	if(*flag==0) 
	{
		printf("Connect Error\r\n");
		Timer_flag =0;//ֹͣ��ʱ
	}	
	else          
	{
		printf("Connect Success\r\n");
		Timer_flag =0;//ֹͣ��ʱ
	}
	*flag=0;
}

/**
 * @brief  ��CCS811 ����������ATָ���ʼ�����ú���
 * @param  ������˵��
 * @arg    �����������ѡ������о�
 * @note   ��ע��
 * @retval ������ֵ˵��
 * @doc    ����
 */
void CCS811_AT_Send_Init()
{
	int flag=0;
	//���������
	Delay_ms(1000); Timer_flag =1;Timer_us=0;Timer_ms=0;//������ʱ
	CSS811_USART_Send("AT\r\n",strlen("AT\r\n"));	//���� AT ָ��
	while(Timer_ms<100 && flag==0)
	{
		if(CCS811_USART_Data_End==1 && strstr((char*)CSS811_USART_Data_Buff,"OK\r\n")!=NULL)
		{CCS811_USART_Data_End=0; Timer_flag =0;break; }
	}
	if(Timer_ms<90)
		
	  printf("Senser Connect Success\r\n");
	
	//��������ʼ��
	flag=0; Delay_ms(100); Timer_ms=0;
	CSS811_USART_Send("AT+INIT\r\n",strlen("AT+INIT\r\n"));
	while(Timer_ms<500 && flag==0)
	{
		if(CCS811_USART_Data_End==1 && strstr((char*)CSS811_USART_Data_Buff,"SUCCESS")!=NULL)
		{CCS811_USART_Data_End=0; Timer_flag =0;break; }
	}
	if(Timer_ms<490)
	  printf("Senser Init Success\r\n");
	
	//���ô������ش��ٶ� 1000MS
	flag=0; Delay_ms(100); Timer_ms=0;
	CSS811_USART_Send("AT+PRATE=1000\r\n",strlen("AT+PRATE=1000\r\n"));
	while(Timer_ms<500 && flag==0)
	{
		if(CCS811_USART_Data_End==1 && strstr((char*)CSS811_USART_Data_Buff,"OK")!=NULL)
		{CCS811_USART_Data_End=0;  Timer_flag =0;break; }		
	}
	if(Timer_ms<490)
	{
	  printf("Senser Set Success\r\n");//��������ʼ���ɹ�
		Timer_flag =0;
//		*connect=1;
	}
}








/* END OF FILE ------------------------------------------------------------------*/

