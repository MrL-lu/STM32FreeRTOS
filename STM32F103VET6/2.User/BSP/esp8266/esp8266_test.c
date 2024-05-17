/* �û�ͷ�ļ� ------------------------------------------------------------------*/
#include "./esp8266/esp8266_test.h"	//���� esp8266_testͷ�ļ�

volatile uint8_t ucTcpClosedFlag = 0;// ESP8266 ���߱�־λ

/**
 * @brief  ��ESP8266 ��Sta Tcp Client��͸��
 * @param  ����
 * @arg    ����
 * @note   ������ESP8266Ϊ Sta Tcp Client ģʽ
 * @retval ����
 * @doc    ����
 */
void ESP8266_StaTcpClient_UnvarnishTest ( void )
{
	/*	ʹ��ESP8266	*/
	ESP8266_CHIP_EN_ENABLE();
	
	/*	����AT ����AT����	*/
	ESP8266_AT_Test(  );//��WF-ESP8266ģ�����AT��������
	
	/*	ѡ����ģʽ	*/
	ESP8266_Work_Mode_Choose( STA );//ѡ��ESP8266ģ��Ĺ���ģʽΪ�ͻ���ģʽ
	
	/*	�ȴ�ESP8266���ӳɹ�	*/
	while(! ESP8266_Join_WIFI(User_ESP8266_ApSsid, User_ESP8266_ApPwd )); //WF-ESP8266ģ�������ⲿWiFi
	
	/*	���õ�����	*/
	ESP8266_Enable_MultipleId ( DISABLE );//WF-ESP8266ģ������Ϊ������ģʽ
	
	/*	�����������ֽ����� TCP ������	*/
	while ( !ESP8266_Link_Server (enumTCP,User_ESP8266_TcpServer_IP,User_ESP8266_TcpServer_Port,Single_ID_0 ));//WF-ESP8266ģ�������ⲿ������
	
	/*	����͸��ģʽ	*/
	while (! ESP8266_UnvarnishSend ( ));//����WF-ESP8266ģ�����͸������
	
	/*	���� ESP8266 ��Sta Tcp Client��͸�� �������	*/
}


/**
 * @brief  ��ESP8266 ��Sta Tcp Client��͸�����ߴ�����
 * @param  ����
 * @arg    ����
 * @retval ����
 * @doc    ���������ѭ������
 */
void ESP8266_StaTcpClient_UnvarnishTest_OnlineTest( void )
{
	uint8_t ucStatus = 0;
	
	if ( ucTcpClosedFlag )	//�ж��Ƿ����
	{
		/*	�˳�͸��ģʽ	*/
		ESP8266_ExitUnvarnishSend ();
		
		/*	�˳�͸��ģʽ	*/
		do
		{
			ucStatus = ESP8266_Get_LinkStatus (  );//��ȡ WF-ESP8266 ������״̬�����ʺϵ��˿�ʱʹ�� 
		}
		while(! ucStatus);// end of do while
		
		if( ucStatus == 4 ) //ȷ��ʧȥ���Ӻ�����
		{
			
			while(! ESP8266_Join_WIFI(User_ESP8266_ApSsid, User_ESP8266_ApPwd )); //WF-ESP8266ģ�������ⲿWiFi
			while ( !ESP8266_Link_Server (enumTCP,User_ESP8266_TcpServer_IP,User_ESP8266_TcpServer_Port,Single_ID_0 ));//WF-ESP8266ģ�������ⲿ������
		
		}
		
		while ( ! ESP8266_UnvarnishSend (  ));//����WF-ESP8266ģ�����͸������
		
	}
}

/**
 * @brief  ��ESP8266����ΪAPģʽ-��Ϊ������������
 * @param  ������˵��
 * @arg    �����������ѡ������о�
 * @note   ��ע��
 * @retval ������ֵ˵��
 * @doc    ����
 */
void ESP8266_AP_Config( void )
{
	/*	ʹ��ESP8266	*/
	ESP8266_CHIP_EN_ENABLE();
	
	/*	����AT ����AT����	*/
	ESP8266_AT_Test(  );//��WF-ESP8266ģ�����AT��������
	
	/*	ѡ����ģʽ	*/
	ESP8266_Work_Mode_Choose( AP );//ѡ��ESP8266ģ��Ĺ���ģʽΪ�ͻ���ģʽ
	
	/*	����wifi	*/
	while ( !ESP8266_Build_WIFI( User_ESP8266_BulitApSsid,User_ESP8266_BulitApPwd,User_ESP8266_BulitApEcn));
	
	/*	����������	*/
	ESP8266_Enable_MultipleId ( ENABLE );
	
	/*	����ģ��������˿�	*/
	while ( !	ESP8266_StartOrShutServer ( ENABLE, User_ESP8266_TcpServer_Port, User_ESP8266_TcpServer_OverTime ) );
	
	/*	ESP8266-APģʽ���óɹ�	*/
}




/* END OF FILE ------------------------------------------------------------------*/
