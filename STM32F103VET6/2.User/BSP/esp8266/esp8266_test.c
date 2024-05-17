/* 用户头文件 ------------------------------------------------------------------*/
#include "./esp8266/esp8266_test.h"	//引用 esp8266_test头文件

volatile uint8_t ucTcpClosedFlag = 0;// ESP8266 掉线标志位

/**
 * @brief  ：ESP8266 （Sta Tcp Client）透传
 * @param  ：无
 * @arg    ：无
 * @note   ：设置ESP8266为 Sta Tcp Client 模式
 * @retval ：无
 * @doc    ：无
 */
void ESP8266_StaTcpClient_UnvarnishTest ( void )
{
	/*	使能ESP8266	*/
	ESP8266_CHIP_EN_ENABLE();
	
	/*	发送AT 测试AT启动	*/
	ESP8266_AT_Test(  );//对WF-ESP8266模块进行AT测试启动
	
	/*	选择工作模式	*/
	ESP8266_Work_Mode_Choose( STA );//选择ESP8266模块的工作模式为客户端模式
	
	/*	等待ESP8266连接成功	*/
	while(! ESP8266_Join_WIFI(User_ESP8266_ApSsid, User_ESP8266_ApPwd )); //WF-ESP8266模块连接外部WiFi
	
	/*	设置单连接	*/
	ESP8266_Enable_MultipleId ( DISABLE );//WF-ESP8266模块设置为单连接模式
	
	/*	连接网络助手建立的 TCP 服务器	*/
	while ( !ESP8266_Link_Server (enumTCP,User_ESP8266_TcpServer_IP,User_ESP8266_TcpServer_Port,Single_ID_0 ));//WF-ESP8266模块连接外部服务器
	
	/*	设置透传模式	*/
	while (! ESP8266_UnvarnishSend ( ));//配置WF-ESP8266模块进入透传发送
	
	/*	到此 ESP8266 （Sta Tcp Client）透传 设置完成	*/
}


/**
 * @brief  ：ESP8266 （Sta Tcp Client）透传掉线处理函数
 * @param  ：无
 * @arg    ：无
 * @retval ：无
 * @doc    ：需放在死循环当中
 */
void ESP8266_StaTcpClient_UnvarnishTest_OnlineTest( void )
{
	uint8_t ucStatus = 0;
	
	if ( ucTcpClosedFlag )	//判断是否掉线
	{
		/*	退出透传模式	*/
		ESP8266_ExitUnvarnishSend ();
		
		/*	退出透传模式	*/
		do
		{
			ucStatus = ESP8266_Get_LinkStatus (  );//获取 WF-ESP8266 的连接状态，较适合单端口时使用 
		}
		while(! ucStatus);// end of do while
		
		if( ucStatus == 4 ) //确认失去连接后重连
		{
			
			while(! ESP8266_Join_WIFI(User_ESP8266_ApSsid, User_ESP8266_ApPwd )); //WF-ESP8266模块连接外部WiFi
			while ( !ESP8266_Link_Server (enumTCP,User_ESP8266_TcpServer_IP,User_ESP8266_TcpServer_Port,Single_ID_0 ));//WF-ESP8266模块连接外部服务器
		
		}
		
		while ( ! ESP8266_UnvarnishSend (  ));//配置WF-ESP8266模块进入透传发送
		
	}
}

/**
 * @brief  ：ESP8266设置为AP模式-作为局域网服务器
 * @param  ：参数说明
 * @arg    ：参数里面可选择参量列举
 * @note   ：注释
 * @retval ：返回值说明
 * @doc    ：无
 */
void ESP8266_AP_Config( void )
{
	/*	使能ESP8266	*/
	ESP8266_CHIP_EN_ENABLE();
	
	/*	发送AT 测试AT启动	*/
	ESP8266_AT_Test(  );//对WF-ESP8266模块进行AT测试启动
	
	/*	选择工作模式	*/
	ESP8266_Work_Mode_Choose( AP );//选择ESP8266模块的工作模式为客户端模式
	
	/*	创建wifi	*/
	while ( !ESP8266_Build_WIFI( User_ESP8266_BulitApSsid,User_ESP8266_BulitApPwd,User_ESP8266_BulitApEcn));
	
	/*	创建多连接	*/
	ESP8266_Enable_MultipleId ( ENABLE );
	
	/*	设置模块服务器端口	*/
	while ( !	ESP8266_StartOrShutServer ( ENABLE, User_ESP8266_TcpServer_Port, User_ESP8266_TcpServer_OverTime ) );
	
	/*	ESP8266-AP模式设置成功	*/
}




/* END OF FILE ------------------------------------------------------------------*/
