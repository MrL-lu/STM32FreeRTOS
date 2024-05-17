/* 用户头文件 ------------------------------------------------------------------*/
#include "./ccs811/iic_ccs811.h"	//引用 CCS811 空气传感器 IIC通讯头文件


void CCS811_IIC_ReadID( void )
{
		
		IIC_Start(  );//CPU发起I2C总线启动信号
		
		IIC_SendByte( 0xB4 );//CPU向I2C总线设备发送8bit数据
		if( IIC_WriteAck(  ))//CPU产生一个时钟，并读取器件的ACK应答信号
			{
				//没有应答信号
				printf("\r\n读取ID，应答信号产生失败\r\n");
			}
		IIC_SendByte( 0x20 );//CPU向I2C总线设备发送8bit数据
		if( IIC_WriteAck(  ))//CPU产生一个时钟，并读取器件的ACK应答信号
			{
				//没有应答信号
				printf("\r\n读取ID，应答信号2产生失败\r\n");
			}
		IIC_Stop(  );//CPU发起I2C总线停止信号
		
		IIC_Start(  );//CPU发起I2C总线启动信号
		IIC_SendByte( 0xB5 );//CPU向I2C总线设备发送8bit数据
		if( IIC_WriteAck(  ))//CPU产生一个时钟，并读取器件的ACK应答信号
		{
			//没有应答信号
			printf("\r\n读取ID，应答信号3产生失败\r\n");
		}
		printf("\r\nCCS811的ID是：%x",IIC_ReadByte(  ));
		IIC_NOACK(  );//CPU产生1个NACK信号
		IIC_Stop(  );//CPU发起I2C总线停止信号
}


/* END OF FILE ------------------------------------------------------------------*/

