/* �û�ͷ�ļ� ------------------------------------------------------------------*/
#include "./ccs811/iic_ccs811.h"	//���� CCS811 ���������� IICͨѶͷ�ļ�


void CCS811_IIC_ReadID( void )
{
		
		IIC_Start(  );//CPU����I2C���������ź�
		
		IIC_SendByte( 0xB4 );//CPU��I2C�����豸����8bit����
		if( IIC_WriteAck(  ))//CPU����һ��ʱ�ӣ�����ȡ������ACKӦ���ź�
			{
				//û��Ӧ���ź�
				printf("\r\n��ȡID��Ӧ���źŲ���ʧ��\r\n");
			}
		IIC_SendByte( 0x20 );//CPU��I2C�����豸����8bit����
		if( IIC_WriteAck(  ))//CPU����һ��ʱ�ӣ�����ȡ������ACKӦ���ź�
			{
				//û��Ӧ���ź�
				printf("\r\n��ȡID��Ӧ���ź�2����ʧ��\r\n");
			}
		IIC_Stop(  );//CPU����I2C����ֹͣ�ź�
		
		IIC_Start(  );//CPU����I2C���������ź�
		IIC_SendByte( 0xB5 );//CPU��I2C�����豸����8bit����
		if( IIC_WriteAck(  ))//CPU����һ��ʱ�ӣ�����ȡ������ACKӦ���ź�
		{
			//û��Ӧ���ź�
			printf("\r\n��ȡID��Ӧ���ź�3����ʧ��\r\n");
		}
		printf("\r\nCCS811��ID�ǣ�%x",IIC_ReadByte(  ));
		IIC_NOACK(  );//CPU����1��NACK�ź�
		IIC_Stop(  );//CPU����I2C����ֹͣ�ź�
}


/* END OF FILE ------------------------------------------------------------------*/

