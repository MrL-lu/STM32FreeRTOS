/* 引用头文件 ------------------------------------------------------------------*/  
#include "./MPU6050/bsp_mpu6050.h"  //引用MPU6050配置头文件

/* 定义全局变量 ------------------------------------------------------------------*/


/* 程序开始 ----------------------------------------------------------------------*/
/**
  * @brief   写数据到MPU6050寄存器
  * @param   
  * @retval  
  */
void MPU6050_WriteReg(uint8_t reg_add,uint8_t pBuffer)
{
	BSP_IIC_WritByte(MPU6050_SLAVE_ADDRESS,reg_add,pBuffer); 
}

/**
  * @brief   从MPU6050寄存器读取数据
  * @param   
  * @retval  
  */
void MPU6050_ReadData(uint8_t reg_add,unsigned char* Read,uint8_t num)
{
	BSP_IIC_ReadByte(reg_add,Read,num);
}


/**
  * @brief   初始化MPU6050芯片
  * @param   
  * @retval  
  */
void MPU6050_Init(void)
{
  int i=0,j=0;
  //在初始化之前要延时一段时间，若没有延时，则断电后再上电数据可能会出错
  for(i=0;i<1000;i++)
  {
    for(j=0;j<1000;j++)
    {
      ;
    }
  }
	MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_1, 0x00);	     //解除休眠状态
	MPU6050_WriteReg(MPU6050_RA_SMPLRT_DIV , 0x07);	    //陀螺仪采样率
	MPU6050_WriteReg(MPU6050_RA_CONFIG , 0x06);	
	MPU6050_WriteReg(MPU6050_RA_ACCEL_CONFIG , 0x01);	  //配置加速度传感器工作在4G模式
	MPU6050_WriteReg(MPU6050_RA_GYRO_CONFIG, 0x18);     //陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
}


/**
  * @brief   读取MPU6050的ID
  * @param   
  * @retval  正常返回1，异常返回0
  */
uint8_t MPU6050ReadID(void)
{
	unsigned char Re = 0;
    MPU6050_ReadData(MPU6050_RA_WHO_AM_I,&Re,1);    //读器件地址
	if(Re != 0x68)
	{
		printf("MPU6050 dectected error!\r\n检测不到MPU6050模块，请检查模块与开发板的接线");
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
  * @brief  读取MPU6050的加速度数据
  *					accData	:		加速度原始数据
  * @author  Lu
  * @version V1.0
  * @date    2024-xx-xx
  **********************************************************************
  */ 
float Actual[3]; //加速度->速度
/**
  * @brief   读取MPU6050的加速度数据
  * @param   
  * @retval  
  */
void MPU6050ReadAcc(short *accData)
{
    uint8_t buf[6];
    // 读取加速度原始数据
    MPU6050_ReadData(MPU6050_ACC_OUT, buf, 6);
    // 将原始数据转换为实际加速度值
    accData[0] = (buf[0] << 8) | buf[1];
    accData[1] = (buf[2] << 8) | buf[3];
    accData[2] = (buf[4] << 8) | buf[5];
    
    // 根据MPU6050的配置和量程计算加速度值
    // 这里需要根据MPU6050的手册编写相应的转换公式
    ConvertRawAccelToActual(accData,Actual);
}

/**
  * @brief   根据量程和原始数据计算实际加速度值
  * @param   
  * @retval  
  */
void ConvertRawAccelToActual(short* Accel_Raw, float* Accel)
{
    float accel_range = 2.0; // MPU6050配置的加速度计量程为 ±2g
    
    // 根据量程和原始数据计算实际加速度值
    Accel[0] = (float)Accel_Raw[0] / 16384.0 * accel_range;
    Accel[1] = (float)Accel_Raw[1] / 16384.0 * accel_range;
    Accel[2] = (float)Accel_Raw[2] / 16384.0 * accel_range;
}

float Angle[3]; //角速率->角度
/**
  * @brief   读取MPU6050的角加速度数据
  * @param   
  * @retval  
  */
void MPU6050ReadGyro(short *gyroData)
{
    uint8_t buf[6];
    // 读取角速度原始数据
    MPU6050_ReadData(MPU6050_GYRO_OUT,buf,6);
    // 将原始数据转换为实际角速度值
    gyroData[0] = (buf[0] << 8) | buf[1];
    gyroData[1] = (buf[2] << 8) | buf[3];
    gyroData[2] = (buf[4] << 8) | buf[5];
    
    // 根据MPU6050的配置和量程计算角速度值
    // 这里需要根据MPU6050的手册编写相应的转换公式
    ConvertRawGyroToActual(gyroData,Angle);
    
}

void ConvertRawGyroToActual(short* Gyro_Raw, float* Gyro)
{
    float gyro_range = 250.0; // MPU6050配置的陀螺仪量程为 ±250°/s
    
    // 根据量程和原始数据计算实际角速度值
    Gyro[0] = (float)Gyro_Raw[0] / 32768.0 * gyro_range;
    Gyro[1] = (float)Gyro_Raw[1] / 32768.0 * gyro_range;
    Gyro[2] = (float)Gyro_Raw[2] / 32768.0 * gyro_range;
}


void CalculateAngle(float* Accel, float* Gyro, float* Angle)
{
    float acc_angle[3], gyro_rate[3];
    float dt = 0.01; // 假设采样周期为10ms
    
    // 加速度计角度计算
    acc_angle[0] = atan2f(Accel[1], Accel[2]) * 180 / M_PI; // X轴角度
    acc_angle[1] = atan2f(-Accel[0], sqrtf(Accel[1] * Accel[1] + Accel[2] * Accel[2])) * 180 / M_PI; // Y轴角度
    
    // 陀螺仪角速度计算
    gyro_rate[0] = Gyro[0]; // X轴角速度
    gyro_rate[1] = Gyro[1]; // Y轴角速度
    
    // 互补滤波器
    Angle[0] = 0.98 * (Angle[0] + gyro_rate[0] * dt) + 0.02 * acc_angle[0]; // X轴角度
    Angle[1] = 0.98 * (Angle[1] + gyro_rate[1] * dt) + 0.02 * acc_angle[1]; // Y轴角度
}


/**
  * @brief   读取MPU6050的原始温度数据
  * @param   
  * @retval  
  */
void MPU6050ReadTemp(short *tempData)
{
	uint8_t buf[2];
    MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2);     //读取温度值
    *tempData = (buf[0] << 8) | buf[1];
}

/**
  * @brief   读取MPU6050的温度数据，转化成摄氏度
  * @param   
  * @retval  
  */
void MPU6050_ReturnTemp(float *Temperature)
{
	short temp3;
	uint8_t buf[2];
	
	MPU6050_ReadData(MPU6050_RA_TEMP_OUT_H,buf,2);     //读取温度值
    temp3= (buf[0] << 8) | buf[1];	
	*Temperature=((double) temp3/340.0)+36.53;

}


/* END OF FILE ------------------------------------------------------------------*/
