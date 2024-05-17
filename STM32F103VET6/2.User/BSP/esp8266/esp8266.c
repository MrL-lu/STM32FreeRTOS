/* �û�ͷ�ļ� ------------------------------------------------------------------*/
#include "./ESP8266/esp8266.h"	//����ESP8266 ͷ�ļ�

//�������
struct  STRUCT_USARTx_Fram strEsp8266_Fram_Record = { 0 };
struct  STRUCT_USARTx_Fram strUSART_Fram_Record = { 0 };


/**
  * @brief  ��ʼ��ESP8266�õ���GPIO����
  * @param  ��
  * @retval ��
  */
static void ESP8266_GPIO_Config( void )
{
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef ESP8266_GPIO_InitStructure;
	
	/* ���� CHIP_EN ����*/
	ESP8266_CHIP_EN_APBxClock_FUN(ESP8266_CHIP_EN_CLK,ENABLE);//����ʱ��
	
	ESP8266_GPIO_InitStructure.GPIO_Mode	=	GPIO_Mode_Out_PP;//�������
	ESP8266_GPIO_InitStructure.GPIO_Pin		=	ESP8266_CHIP_EN_PIN;
	ESP8266_GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	
	/* ��ʼ�� CHIP_EN ����*/
	GPIO_Init ( ESP8266_CHIP_EN_PORT, & ESP8266_GPIO_InitStructure );
	
	/* ���� RST ����*/
	
	ESP8266_RST_APBxClock_FUN(ESP8266_RST_CLK,ENABLE);//����ʱ��
	ESP8266_GPIO_InitStructure.GPIO_Pin		=  ESP8266_RST_PIN;
	
	/* ��ʼ�� RST ����*/
	GPIO_Init ( ESP8266_RST_PORT, & ESP8266_GPIO_InitStructure );
}

/**
  * @brief  ���� ESP8266 USART �� NVIC �ж�
  * @param  ��
  * @retval ��
  */
static void ESP8266_USART_NVIC_Configuration ( void )
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	
	/* Ƕ�������жϿ�������ѡ�� */ 
	NVIC_PriorityGroupConfig ( NVIC_PriorityGroup_2 );

	/* Enable the USART2 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = ESP8266_USART_IRQ;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

 /**
  * @brief  ��ESP8266����GPIO�������ú���
  * @param  ����
  * @retval ����
  * @doc    ����
  */
static void ESP8266_USART_GPIO_Congif( void )
{
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef ESP8266_GPIO_InitStructure;
	/*����һ��USART_InitTypeDef���͵Ľṹ��*/
	USART_InitTypeDef USART_InitStructure;
	
	/*ʹ������˿�ʱ��*/
	ESP8266_USART_APBxClock_FUN(ESP8266_USART_CLK,ENABLE);
	ESP8266_USART_GPIO_APBxClock_FUN(ESP8266_USART_GPIO_CLK,ENABLE);
	
	/*����GPIO��������*/
	
	/*TXD���ø����������*/
	ESP8266_GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF_PP;
	ESP8266_GPIO_InitStructure.GPIO_Pin		= ESP8266_USART_TX_PIN;
	ESP8266_GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	
	/* ��ʼ�� TXD ����*/
	GPIO_Init(ESP8266_USART_TX_PORT,&ESP8266_GPIO_InitStructure);
	
	/*RXD���ø�������*/
	ESP8266_GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;
	ESP8266_GPIO_InitStructure.GPIO_Pin		= ESP8266_USART_RX_PIN;
	
	/* ��ʼ�� RXD ����*/
	GPIO_Init(ESP8266_USART_RX_PORT,&ESP8266_GPIO_InitStructure);
	
	/* ����ģʽ���� */
	USART_InitStructure.USART_BaudRate = ESP8266_USART_BAUD_RATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(ESP8266_USARTx, &USART_InitStructure);
	
	/* �ж����� */
	USART_ITConfig ( ESP8266_USARTx, USART_IT_RXNE, ENABLE ); //ʹ�ܴ��ڽ����ж� 
	USART_ITConfig ( ESP8266_USARTx, USART_IT_IDLE, ENABLE ); //ʹ�ܴ������߿����ж� 	

	ESP8266_USART_NVIC_Configuration ();
	
	
	USART_Cmd(ESP8266_USARTx, ENABLE);
	
	
}

 /**
  * @brief  ��ESP8266��ʼ������
  * @param  ����
  * @retval ����
  * @doc    ����
  */
void ESP8266_Init( void )//ESP8266��ʼ������
{
	ESP8266_GPIO_Config(  );//ESP8266_GPIO�������ú���
	ESP8266_USART_GPIO_Congif(  );//ESP8266����GPIO�������ú���
	
	ESP8266_RST_HIGH_LEVEL();//��λ���Ÿ��ߵ�ƽ
	ESP8266_CHIP_EN_DISABLE();//ʹ��ESP8266
}
	
/*
 * ��������ESP8266_RST
 * ����  ������WF-ESP8266ģ��
 * ����  ����
 * ����  : ��
 * ����  ���� ESP8266_AT_Test ����
 */
void ESP8266_RST( void )
{
	#if 0
	 ESP8266_Cmd ( "AT+RST", "OK", "ready", 2500 );   	
	
	#else
	 ESP8266_RST_LOW_LEVEL();
	 Delay_ms ( 500 ); 
	 ESP8266_RST_HIGH_LEVEL();
	#endif
}



/*
 * ��������itoa
 * ����  ������������ת�����ַ���
 * ����  ��-radix =10 ��ʾ10���ƣ��������Ϊ0
 *         -value Ҫת����������
 *         -buf ת������ַ���
 *         -radix = 10
 * ���  ����
 * ����  ����
 * ����  ����USART2_printf()����
 */
static char * itoa( int value, char *string, int radix )
{
	int     i, d;
	int     flag = 0;
	char    *ptr = string;

	/* This implementation only works for decimal numbers. */
	if (radix != 10)
	{
		*ptr = 0;
		return string;
	}

	if (!value)
	{
		*ptr++ = 0x30;
		*ptr = 0;
		return string;
	}

	/* if this is a negative value insert the minus sign. */
	if (value < 0)
	{
		*ptr++ = '-';

		/* Make the value positive. */
		value *= -1;
		
	}

	for (i = 10000; i > 0; i /= 10)
	{
		d = value / i;

		if (d || flag)
		{
			*ptr++ = (char)(d + 0x30);
			value -= (d * i);
			flag = 1;
		}
	}

	/* Null terminate the string. */
	*ptr = 0;

	return string;

} /* NCL_Itoa */


/*
 * ��������ESP8266_USART_Printf
 * ����  ����ʽ�������������C���е�printf��������û���õ�C��
 * ����  ��-USARTx ����ͨ��������ֻ�õ��˴���2����USART2
 *		     -Data   Ҫ���͵����ڵ����ݵ�ָ��
 *			   -...    ��������
 * ���  ����
 * ����  ���� 
 * ����  ���ⲿ����
 *         ����Ӧ��USART2_printf( USART2, "\r\n this is a demo \r\n" );
 *            		 USART2_printf( USART2, "\r\n %d \r\n", i );
 *            		 USART2_printf( USART2, "\r\n %s \r\n", j );
 */
void ESP8266_USART_Printf( USART_TypeDef * USARTx, char * Data, ... )
{	
	const char *s;//����һ��ָ���ַ�������ָ��
	int d;   
	char buf[16];//����һ������
	
	va_list ap;
	va_start(ap, Data);

	while ( * Data != 0 )     // �ж��Ƿ񵽴��ַ���������
	{				                          
		if ( * Data == 0x5c )  //'\'
		{									  
			switch ( *++Data )
			{
				case 'r':							          //�س���
				USART_SendData(USARTx, 0x0d);
				Data ++;
				break;

				case 'n':							          //���з�
				USART_SendData(USARTx, 0x0a);	
				Data ++;
				break;

				default:
				Data ++;
				break;
			}			 
		}
		
		else if ( * Data == '%')
		{									  //
			switch ( *++Data )
			{				
				case 's':										  //�ַ���
				s = va_arg(ap, const char *);
				
				for ( ; *s; s++) 
				{
					USART_SendData(USARTx,*s);
					while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
				}
				
				Data++;
				
				break;

				case 'd':			
					//ʮ����
				d = va_arg(ap, int);
				
				itoa(d, buf, 10);
				
				for (s = buf; *s; s++) 
				{
					USART_SendData(USARTx,*s);
					while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
				}
				
				Data++;
				
				break;
				
				default:
				Data++;
				
				break;
				
			}		 
		}
		
		else USART_SendData(USARTx, *Data++);
		
		while ( USART_GetFlagStatus ( USARTx, USART_FLAG_TXE ) == RESET );	
	}
	
}

/*
 * ��������ESP8266_AT_Cmd
 * ����  ����WF-ESP8266ģ�鷢��ATָ��
 * ����  ��cmd�������͵�ָ��
 *         reply1��reply2���ڴ�����Ӧ��ΪNULL������Ӧ������Ϊ���߼���ϵ
 *         waittime���ȴ���Ӧ��ʱ��
 * ����  : 1��ָ��ͳɹ�
 *         0��ָ���ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_AT_Cmd( char * cmd, char * reply1, char * reply2, u32 waittime )
{
	strEsp8266_Fram_Record .InfBit .FramLength = 0;               //���¿�ʼ�����µ����ݰ�

	ESP8266_Usart ( "%s\r\n", cmd );	//���ô��ڷ��ͺ�������������

	if ( ( reply1 == 0 ) && ( reply2 == 0 ) )                      //����Ҫ��������
		return true;
	
	Delay_ms ( waittime );                 //��ʱ
	
	strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ]  = '\0';

	PC_Usart ( "%s", strEsp8266_Fram_Record .Data_RX_BUF );
	strEsp8266_Fram_Record .InfBit .FramLength = 0;         //������ձ�־
  strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;
	if ( ( reply1 != 0 ) && ( reply2 != 0 ) )
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply1 ) || 
						 ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply2 ) ); 
 	
	else if ( reply1 != 0 )
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply1 ) );
	
	else
		return ( ( bool ) strstr ( strEsp8266_Fram_Record .Data_RX_BUF, reply2 ) );
	
}

/*
 * ��������ESP8266_AT_Test
 * ����  ����WF-ESP8266ģ�����AT��������
 * ����  ����
 * ����  : ��
 * ����  �����ⲿ����
 */
void ESP8266_AT_Test( void )
{
	char count=0;
	
	ESP8266_RST_HIGH_LEVEL();	
	Delay_ms ( 2000 );
	while ( count < 10 )//����10��
	{
		if( ESP8266_AT_Cmd ( "AT", "OK", NULL, 500 ) ) 
		{
			return;
		}	
		ESP8266_RST();//��λESP8266
		++ count;
	}
}


/*
 * ��������ESP8266_Work_Mode_Choose
 * ����  ��ѡ��WF-ESP8266ģ��Ĺ���ģʽ
 * ����  ��ESP8266_WorkMode������ģʽ
 * ����  : 1��ѡ��ɹ�
 *         0��ѡ��ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_Work_Mode_Choose(ESP8266_Work_ModeTypeDef  ESP8266_WorkMode )
{
	switch ( ESP8266_WorkMode )
	{
		case STA:
			return ESP8266_AT_Cmd( "AT+CWMODE=1", "OK", "no change", 2500 );	//SoftAPģʽ����·��ģʽ��
		
		case AP:
			return ESP8266_AT_Cmd( "AT+CWMODE=1", "OK", "no change", 2500 );	//stationģʽ���ͻ���ģʽ��
		
		case STA_AP:
			return ESP8266_AT_Cmd( "AT+CWMODE=1", "OK", "no change", 2500 );	//SoftAP+station����ģʽ�����ģʽ��
		
		default:
			return false;//���ش���
	}
}

/*
 * ��������ESP8266_Join_WIFI
 * ����  ��WF-ESP8266ģ�������ⲿWiFi
 * ����  ��pSSID��WiFi�����ַ���
 *       ��pPassWord��WiFi�����ַ���
 * ����  : 1�����ӳɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_Join_WIFI(char * pSSID, char * pPassWord )
{
	char lCmd [120]; //����һ������
	
	sprintf ( lCmd, "AT+CWJAP=\"%s\",\"%s\"", pSSID, pPassWord );
	
	return ESP8266_AT_Cmd ( lCmd, "OK", NULL, 5000 );

}

/*
 * ��������ESP8266_Build_WIFI
 * ����  ��WF-ESP8266ģ�鴴��WiFi�ȵ�
 * ����  ��pSSID��WiFi�����ַ���
 *       ��pPassWord��WiFi�����ַ���
 *       ��ESP8266_ENUM_AP_PsdMode��WiFi���ܷ�ʽ�����ַ���
 * ����  : 1�������ɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */	
bool ESP8266_Build_WIFI( char * pSSID, char * pPassWord, ENUM_AP_PsdMode_TypeDef ESP8266_ENUM_AP_PsdMode )	
{
	char lCmd [120]; //����һ������
	/*	sprintf()�������ַ�����ʽ������ */
	sprintf ( lCmd, "AT+CWSAP=\"%s\",\"%s\",1,%d", pSSID, pPassWord, ESP8266_ENUM_AP_PsdMode );//����APģʽ����
	
	return ESP8266_AT_Cmd ( lCmd, "OK", 0, 1000 );
	
}

/*
 * ��������ESP8266_Enable_MultipleId
 * ����  ��WF-ESP8266ģ������������
 * ����  ��enumEnUnvarnishTx�������Ƿ������
 * ����  : 1�����óɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_Enable_MultipleId ( FunctionalState enumEnUnvarnishTx )
{
	char lCmd [20];
	
	sprintf ( lCmd, "AT+CIPMUX=%d", ( enumEnUnvarnishTx ? 1 : 0 ) );
	
	return ESP8266_AT_Cmd ( lCmd, "OK", 0, 500 );
	
}

/*
 * ��������ESP8266_Link_Server
 * ����  ��WF-ESP8266ģ�������ⲿ������
 * ����  ��enumE������Э��
 *       ��ip��������IP�ַ���
 *       ��ComNum���������˿��ַ���
 *       ��id��ģ�����ӷ�������ID
 * ����  : 1�����ӳɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_Link_Server ( ENUM_NetPro_TypeDef enumE, char * ip, char * ComNum, ENUM_ID_NO_TypeDef id)
{
	char cStr [100] = { 0 }, cCmd [120];

  switch (  enumE )
  {
		case enumTCP:
		  sprintf ( cStr, "\"%s\",\"%s\",%s", "TCP", ip, ComNum );//����TCP����
		  break;
		
		case enumUDP:
		  sprintf ( cStr, "\"%s\",\"%s\",%s", "UDP", ip, ComNum );//����UDP����
		  break;
		
		default:
			break;
  }

  if ( id < 5 )
    sprintf ( cCmd, "AT+CIPSTART=%d,%s", id, cStr);

  else
	  sprintf ( cCmd, "AT+CIPSTART=%s", cStr );

	return ESP8266_AT_Cmd ( cCmd, "OK", "ALREAY CONNECT", 4000 );
	
}

/*
 * ��������ESP8266_StartOrShutServer
 * ����  ��WF-ESP8266ģ�鿪����رշ�����ģʽ
 * ����  ��enumMode������/�ر�   ������ENABLE/DISABLE
 *       ��pPortNum���������˿ں��ַ���
 *       ��pTimeOver����������ʱʱ���ַ�������λ����
 * ����  : 1�������ɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_StartOrShutServer ( FunctionalState enumMode, char * pPortNum, char * pTimeOver )
{
	char cCmd1 [120], cCmd2 [120];

	if ( enumMode )
	{
		sprintf ( cCmd1, "AT+CIPSERVER=%d,%s", 1, pPortNum );
		
		sprintf ( cCmd2, "AT+CIPSTO=%s", pTimeOver );

		return ( ESP8266_AT_Cmd ( cCmd1, "OK", 0, 500 ) &&
						 ESP8266_AT_Cmd ( cCmd2, "OK", 0, 500 ) );
	}
	
	else
	{
		sprintf ( cCmd1, "AT+CIPSERVER=%d,%s", 0, pPortNum );

		return ESP8266_AT_Cmd ( cCmd1, "OK", 0, 500 );
	}
	
}

/*
 * ��������ESP8266_Get_LinkStatus
 * ����  ����ȡ WF-ESP8266 ������״̬�����ʺϵ��˿�ʱʹ��
 * ����  ����
 * ����  : 2�����ip
 *         3����������
 *         3��ʧȥ����
 *         0����ȡ״̬ʧ��
 * ����  �����ⲿ����
 */
uint8_t ESP8266_Get_LinkStatus ( void )
{
	if ( ESP8266_AT_Cmd ( "AT+CIPSTATUS", "OK", 0, 500 ) )
	{
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "STATUS:2\r\n" ) )
			return 2;
		
		else if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "STATUS:3\r\n" ) )
			return 3;
		
		else if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "STATUS:4\r\n" ) )
			return 4;		

	}
	
	return 0;
	
}

/*
 * ��������ESP8266_Get_IdLinkStatus
 * ����  ����ȡ WF-ESP8266 �Ķ˿ڣ�Id������״̬�����ʺ϶�˿�ʱʹ��
 * ����  ����
 * ����  : �˿ڣ�Id��������״̬����5λΪ��Чλ���ֱ��ӦId5~0��ĳλ����1���Id���������ӣ�������0���Idδ��������
 * ����  �����ⲿ����
 */
uint8_t ESP8266_Get_IdLinkStatus ( void )
{
	uint8_t ucIdLinkStatus = 0x00;
	
	
	if ( ESP8266_AT_Cmd ( "AT+CIPSTATUS", "OK", 0, 500 ) )
	{
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:0," ) )
			ucIdLinkStatus |= 0x01;
		else 
			ucIdLinkStatus &= ~ 0x01;
		
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:1," ) )
			ucIdLinkStatus |= 0x02;
		else 
			ucIdLinkStatus &= ~ 0x02;
		
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:2," ) )
			ucIdLinkStatus |= 0x04;
		else 
			ucIdLinkStatus &= ~ 0x04;
		
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:3," ) )
			ucIdLinkStatus |= 0x08;
		else 
			ucIdLinkStatus &= ~ 0x08;
		
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+CIPSTATUS:4," ) )
			ucIdLinkStatus |= 0x10;
		else 
			ucIdLinkStatus &= ~ 0x10;	

	}
	
	return ucIdLinkStatus;
	
}

/*
 * ��������ESP8266_Inquire_ApIp
 * ����  ����ȡ F-ESP8266 �� AP IP
 * ����  ��pApIp����� AP IP ��������׵�ַ
 *         ucArrayLength����� AP IP ������ĳ���
 * ����  : 0����ȡʧ��
 *         1����ȡ�ɹ�
 * ����  �����ⲿ����
 */
uint8_t ESP8266_Inquire_ApIp ( char * pApIp, uint8_t ucArrayLength )
{
	char uc;
	
	char * pCh;
	
	
  ESP8266_AT_Cmd ( "AT+CIFSR", "OK", 0, 500 );
	
	pCh = strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "APIP,\"" );
	
	if ( pCh )
		pCh += 6;
	
	else
		return 0;
	
	for ( uc = 0; uc < ucArrayLength; uc ++ )
	{
		pApIp [ uc ] = * ( pCh + uc);
		
		if ( pApIp [ uc ] == '\"' )
		{
			pApIp [ uc ] = '\0';
			break;
		}
		
	}
	
	return 1;
	
}

/*
 * ��������ESP8266_UnvarnishSend
 * ����  ������WF-ESP8266ģ�����͸������
 * ����  ����
 * ����  : 1�����óɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_UnvarnishSend ( void )
{
	if ( ! ESP8266_AT_Cmd ( "AT+CIPMODE=1", "OK", 0, 500 ) )
		return false;
	
	return 
	  ESP8266_AT_Cmd ( "AT+CIPSEND", "OK", ">", 500 );
	
}

/*
 * ��������ESP8266_ExitUnvarnishSend
 * ����  ������WF-ESP8266ģ���˳�͸��ģʽ
 * ����  ����
 * ����  : ��
 * ����  �����ⲿ����
 */
void ESP8266_ExitUnvarnishSend ( void )
{
	Delay_ms ( 1000 );
	
	ESP8266_Usart ( "+++" );
	
	Delay_ms ( 500 ); 
	
}

/*
 * ��������ESP8266_SendString
 * ����  ��WF-ESP8266ģ�鷢���ַ���
 * ����  ��enumEnUnvarnishTx�������Ƿ���ʹ����͸��ģʽ
 *       ��pStr��Ҫ���͵��ַ���
 *       ��ulStrLength��Ҫ���͵��ַ������ֽ���
 *       ��ucId���ĸ�ID���͵��ַ���
 * ����  : 1�����ͳɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
bool ESP8266_SendString ( FunctionalState enumEnUnvarnishTx, char * pStr, u32 ulStrLength, ENUM_ID_NO_TypeDef ucId )
{
	char cStr [20];
	bool bRet = false;
	
		
	if ( enumEnUnvarnishTx )
	{
		ESP8266_Usart ( "%s", pStr );
		
		bRet = true;
		
	}

	else
	{
		if ( ucId < 5 )
			sprintf ( cStr, "AT+CIPSEND=%d,%d", ucId, ulStrLength + 2 );

		else
			sprintf ( cStr, "AT+CIPSEND=%d", ulStrLength + 2 );
		
		ESP8266_AT_Cmd ( cStr, "> ", 0, 1000 );

		bRet = ESP8266_AT_Cmd ( pStr, "SEND OK", 0, 1000 );
  }
	
	return bRet;

}


/*
 * ��������ESP8266_ReceiveString
 * ����  ��WF-ESP8266ģ������ַ���
 * ����  ��enumEnUnvarnishTx�������Ƿ���ʹ����͸��ģʽ
 * ����  : ���յ����ַ����׵�ַ
 * ����  �����ⲿ����
 */
char * ESP8266_ReceiveString ( FunctionalState enumEnUnvarnishTx )
{
	char * pRecStr = 0;
	
	
	strEsp8266_Fram_Record .InfBit .FramLength = 0;
	strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;
	
	while ( ! strEsp8266_Fram_Record .InfBit .FramFinishFlag );
	strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ] = '\0';
	
	if ( enumEnUnvarnishTx )
		pRecStr = strEsp8266_Fram_Record .Data_RX_BUF;
	
	else 
	{
		if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+IPD" ) )
			pRecStr = strEsp8266_Fram_Record .Data_RX_BUF;

	}

	return pRecStr;
	
}



/*
 * ��������ESP8266_CWLIF
 * ����  ����ѯ�ѽ����豸��IP
 * ����  ��pStaIp������ѽ����豸��IP
 * ����  : 1���н����豸
 *         0���޽����豸
 * ����  �����ⲿ����
 */
uint8_t ESP8266_CWLIF ( char * pStaIp )
{
	uint8_t uc, ucLen;
	
	char * pCh, * pCh1;
	
	
  ESP8266_AT_Cmd ( "AT+CWLIF", "OK", 0, 100 );
	
	pCh = strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "," );
	
	if ( pCh )
	{
		pCh1 = strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "AT+CWLIF\r\r\n" ) + 11;
	  ucLen = pCh - pCh1;
	}

	else
		return 0;
	
	for ( uc = 0; uc < ucLen; uc ++ )
		pStaIp [ uc ] = * ( pCh1 + uc);
	
	pStaIp [ ucLen ] = '\0';
	
	return 1;
	
}


/*
 * ��������ESP8266_CIPAP
 * ����  ������ģ��� AP IP
 * ����  ��pApIp��ģ��� AP IP
 * ����  : 1�����óɹ�
 *         0������ʧ��
 * ����  �����ⲿ����
 */
uint8_t ESP8266_CIPAP ( char * pApIp )
{
	char cCmd [ 30 ];
		
	
	sprintf ( cCmd, "AT+CIPAP=\"%s\"", pApIp );
	
  if ( ESP8266_AT_Cmd ( cCmd, "OK", 0, 5000 ) )
		return 1;
 
	else 
		return 0;
	
}



/* END OF FILE ------------------------------------------------------------------*/


