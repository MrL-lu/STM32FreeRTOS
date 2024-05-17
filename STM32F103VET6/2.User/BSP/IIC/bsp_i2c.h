/**
  ******************************************************************************
  * @file     ����������	  	bsp_i2c.h
  * @author ���������ߣ�		Lu
  * @version������汾��		V1.0.0.0
  * @date   ���޸����ڣ�		2024-03-22
  * @brief  �����ܼ�飩		STM32F103VET6�̼���ģ��
  * @parameter  ��������		��
  * @return   ������ֵ��		��
  ******************************************************************************
  * @attention ��ע�����
  * оƬ�ͺţ�STM32F103VET6
  * ʹ�����ţ�SCL ->PB6      SDA->PB7
  * �޸����ڣ�
  * ʵ�ֹ��ܣ�
  *	
  ******************************************************************************
  */
/*------------------------------------------------------------------------------*/

#ifndef __BSP_I2C_H
#define __BSP_I2C_H
/* �û�ͷ�ļ� ------------------------------------------------------------------*/
#ifdef      FreeRTOS_OS
/* FreeRTOSͷ�ļ� */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "main.h"

#else

#include "stm32f10x.h"   //����stm32ͷ�ļ�
#include "main.h"

#endif  //FreeRTOS_OS

/* �궨�� ----------------------------------------------------------------------*/
#define             BSP_I2Cx                                I2C1                        //IIC�豸
#define             BSP_I2C_APBxClock_FUN                   RCC_APB1PeriphClockCmd      //IICʱ��
#define             BSP_I2C_CLK                             RCC_APB1Periph_I2C1         //IICʱ��
#define             BSP_I2C_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd      //��ӦGPIOʱ��
#define             BSP_I2C_GPIO_CLK                        RCC_APB2Periph_GPIOB        //��ӦGPIOʱ�Ӷ˿�
#define             BSP_I2C_SCL_PORT                        GPIOB                       //��ӦSCL��GPIO�˿�
#define             BSP_I2C_SCL_PIN                         GPIO_Pin_6                  //��ӦSCL��GPIO����
#define             BSP_I2C_SDA_PORT                        GPIOB                       //��ӦSDA��GPIO�˿�
#define             BSP_I2C_SDA_PIN                         GPIO_Pin_7                  //��ӦSDA��GPIO����

#define             BSP_I2C_Speed                           400000                      //400KHz
#define             BSP_I2C_OWN_ADDRESS7                    0X0A                        //MCU������ַ�������ַֻҪ��STM32��ҵ�I2C������ַ��һ������

/*�ȴ���ʱʱ��*/
#define             BSP_I2CT_FLAG_TIMEOUT                   ((uint32_t)0x1000)
#define             BSP_I2CT_LONG_TIMEOUT                   ((uint32_t)(10 * BSP_I2CT_FLAG_TIMEOUT))

/*��Ϣ���*/
//#define             IIC_DEBUG_ON                            1

//#define             IIC_INFO(fmt,arg...)                    printf("<<-IIC-INFO->> "fmt"\n",##arg)
//#define             IIC_ERROR(fmt,arg...)                   printf("<<-IIC-ERROR->> "fmt"\n",##arg)
//#define             IIC_DEBUG(fmt,arg...)                   do{\
//                                                                if(MPU_DEBUG_ON)\
//                                                                    printf("<<-MPU-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
//                                                                }while(0)

/* �������� --------------------------------------------------------------------*/
static void BSP_IIC_Config(void);	//IIC��ʼ������
static void BSP_IIC_Mode_Config(void);  //I2C ����ģʽ����
void BSP_IIC_Init(void);    //I2C ��ʼ��
static  uint8_t I2C_TIMEOUT_UserCallback(void); //��ʱ����
uint8_t BSP_IIC_WritByte(uint8_t Slave_ADDRESS,uint8_t WriteAddr,uint8_t pBuffer); //I2C дһ���ֽ�
uint8_t BSP_IIC_ReadByte(u8 ReadAddr,u8* pBuffer, u16 NumByteToRead);   //I2C ��һ���ֽ�
void I2C_WaitStandbyState(uint8_t SlaveAddress);    //�ȴ���������״̬

#endif  /*  __BSP_I2C_H */

/* END OF FILE ------------------------------------------------------------------*/

