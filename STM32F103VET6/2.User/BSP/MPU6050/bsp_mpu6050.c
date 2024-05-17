/* ����ͷ�ļ� ------------------------------------------------------------------*/  
#include "./MPU6050/bsp_mpu6050.h"  //����MPU6050����ͷ�ļ�

/* ����ȫ�ֱ��� ------------------------------------------------------------------*/


/* ����ʼ ----------------------------------------------------------------------*/
/**
  * @brief   д���ݵ�MPU6050�Ĵ���
  * @param   
  * @retval  
  */
void MPU6050_WriteReg(uint8_t reg_add,uint8_t pBuffer)
{
	BSP_IIC_WritByte(MPU6050_SLAVE_ADDRESS,reg_add,pBuffer); 
}

/**
  * @brief   ��MPU6050�Ĵ�����ȡ����
  * @param   
  * @retval  
  */
void MPU6050_ReadData(uint8_t reg_add,unsigned char* Read,uint8_t num)
{
	BSP_IIC_ReadByte(reg_add,Read,num);
}


/**
  * @brief   ��ʼ��MPU6050оƬ
  * @param   
  * @retval  
  */
void MPU6050_Init(void)
{
  int i=0,j=0;
  //�ڳ�ʼ��֮ǰҪ��ʱһ��ʱ�䣬��û����ʱ����ϵ�����ϵ����ݿ��ܻ����
  for(i=0;i<1000;i++)
  {
    for(j=0;j<1000;j++)
    {
      ;
    }
  }
	MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_1, 0x00);	     //�������״̬
	MPU6050_WriteReg(MPU6050_RA_SMPLRT_DIV , 0x07);	    //�����ǲ�����
	MPU6050_WriteReg(MPU6050_RA_CONFIG , 0x06);	
	MPU6050_WriteReg(MPU6050_RA_ACCEL_CONFIG , 0x01);	  //���ü��ٶȴ�����������4Gģʽ
	MPU6050_WriteReg(MPU6050_RA_GYRO_CONFIG, 0x18);     //�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
}


/**
  * @brief   ��ȡMPU6050��ID
  * @param   
  * @retval  ��������1���쳣����0
  */
uint8_t MPU6050ReadID(void)
{
	unsigned char Re = 0;
    MPU6050_ReadData(MPU6050_RA_WHO_AM_I,&Re,1);    //��������ַ
	if(Re != 0x68)
	{
		printf("MPU6050 dectected error!\r\n��ⲻ��MPU6050ģ�飬����ģ���뿪����Ľ���");
		return 0;
	}
	else
	{
		printf("MPU6050 ID = %d\r\n",Re);
		return 1;
	}
}


/**
  **********************************************************************
  * @brief  ��ȡMPU6050�ļ��ٶ�����
  *					accData	:		���ٶ�ԭʼ����
  * @author  Lu
  * @version V1.0
  * @date    2024-xx-xx
  **********************************************************************
  */ 
float Actual[3]; //���ٶ�->�ٶ�
/**
  * @brief   ��ȡMPU6050�ļ��ٶ�����
  * @param   
  * @retval  
  */
void MPU6050ReadAcc(short *accData)
{
    uint8_t buf[6];
    // ��ȡ���ٶ�ԭʼ����
    MPU6050_ReadData(MPU6050_ACC_OUT, buf, 6);
    // ��ԭʼ����ת��Ϊʵ�ʼ��ٶ�ֵ
    accData[0] = (buf[0] << 8) | buf[1];
    accData[1] = (buf[2] << 8) | buf[3];
    accData[2] = (buf[4] << 8) | buf[5];
    
    // ����MPU6050�����ú����̼�����ٶ�ֵ
    // ������Ҫ����MPU6050���ֲ��д��Ӧ��ת����ʽ
    ConvertRawAccelToActual(accData,Actual);
}

/**
  * @brief   �������̺�ԭʼ���ݼ���ʵ�ʼ��ٶ�ֵ
  * @param   
  * @retval  
  */
void ConvertRawAccelToActual(short* Accel_Raw, float* Accel)
{
    float accel_range = 2.0; // MPU6050���õļ��ٶȼ�����Ϊ ��2g
    
    // �������̺�ԭʼ���ݼ���ʵ�ʼ��ٶ�ֵ
    Accel[0] = (float)Accel_Raw[0] / 16384.0 * accel_range;
    Accel[1] = (float)Accel_Raw[1] / 16384.0 * accel_range;
    Accel[2] = (float)Accel_Raw[2] / 16384.0 * accel_range;
}

float Angle[3]; //������->�Ƕ�
/**
  * @brief   ��ȡMPU6050�ĽǼ��ٶ�����
  * @param   
  * @retval  
  */
void MPU6050ReadGyro(short *gyroData)
{
    uint8_t buf[6];
    // ��ȡ���ٶ�ԭʼ����
    MPU6050_ReadData(MPU6050_GYRO_OUT,buf,6);
    // ��ԭʼ����ת��Ϊʵ�ʽ��ٶ�ֵ
    gyroData[0] = (buf[0] << 8) | buf[1];
    gyroData[1] = (buf[2] << 8) | buf[3];
    gyroData[2] = (buf[4] << 8) | buf[5];
    
    // ����MPU6050�����ú����̼�����ٶ�ֵ
    // ������Ҫ����MPU6050���ֲ��д��Ӧ��ת����ʽ
    ConvertRawGyroToActual(gyroData,Angle);
    
}

void ConvertRawGyroToActual(short* Gyro_Raw, float* Gyro)
{
    float gyro_range = 250.0; // MPU6050���õ�����������Ϊ ��250��/s
    
    // �������̺�ԭʼ���ݼ���ʵ�ʽ��ٶ�ֵ
    Gyro[0] = (float)Gyro_Raw[0] / 32768.0 * gyro_range;
    Gyro[1] = (float)Gyro_Raw[1] / 32768.0 * gyro_range;
    Gyro[2] = (float)Gyro_Raw[2] / 32768.0 * gyro_range;
}


void CalculateAngle(float* Accel, float* Gyro, float* Angle)
{
    float acc_angle[3], gyro_rate[3];
    float dt = 0.01; // �����������Ϊ10ms
    
    // ���ٶȼƽǶȼ���
    acc_angle[0] = atan2f(Accel[1], Accel[2]) * 180 / M_PI; // X��Ƕ�
    acc_angle[1] = atan2f(-Accel[0], sqrtf(Accel[1] * Accel[1] + Accel[2] * Accel[2])) * 180 / M_PI; // Y��Ƕ�
    
    // �����ǽ��ٶȼ���
    gyro_rate[0] = Gyro[0]; // X����ٶ�
    gyro_rate[1] = Gyro[1]; // Y����ٶ�
    
    // �����˲���
    Angle[0] = 0.98 * (Angle[0] + gyro_rate[0] * dt) + 0.02 * acc_angle[0]; // X��Ƕ�
    Angle[1] = 0.98 * (Angle[1] + gyro_rate[1] * dt) + 0.02 * acc_angle[1]; // Y��Ƕ�
}


/**
  * @brief   ��ȡMPU6050��ԭʼ�¶�����
  * @param   
  * @retval  
  */
void MPU6050ReadTemp(short *tempData)
{
	uint8_t buf[2];
    MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2);     //��ȡ�¶�ֵ
    *tempData = (buf[0] << 8) | buf[1];
}

/**
  * @brief   ��ȡMPU6050���¶����ݣ�ת�������϶�
  * @param   
  * @retval  
  */
void MPU6050_ReturnTemp(float *Temperature)
{
	short temp3;
	uint8_t buf[2];
	
	MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2);     //��ȡ�¶�ֵ
    temp3= (buf[0] << 8) | buf[1];	
	*Temperature=((double) temp3/340.0)+36.53;

}


/* END OF FILE ------------------------------------------------------------------*/
