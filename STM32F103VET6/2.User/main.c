/**
  *****************************************************************************
  * @file     ����������	  	main.c
  * @author ���������ߣ�		Lu
  * @version������汾��		V1.0.0.0
  * @date   ���޸����ڣ�		2024-02-19
  * @brief  �����ܼ�飩		STM32F103VET6�̼���ģ��
  * @parameter  ��������		��
  * @return   ������ֵ��		��
  *****************************************************************************
  * @attention ��ע�����
  * оƬ�ͺţ�STM32F103VET6
  * ʹ�����ţ�
  * �޸����ڣ� 
  * ʵ�ֹ��ܣ�
  *	
  *****************************************************************************
  */

/*
*************************************************************************
*                             ������ͷ�ļ�
*************************************************************************
*/

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




/**************************** ������ ********************************/
/* 
 * ��������һ��ָ�룬����ָ��һ�����񣬵����񴴽���֮�����;�����һ��������
 * �Ժ�����Ҫ��������������Ҫͨ�������������������������������Լ�����ô
 * ����������ΪNULL��
 */
 /* ���������� */
static TaskHandle_t AppTaskCreate_Handle = NULL;
/* TEST_Task������ */
static TaskHandle_t TEST_Task_Handle = NULL;		
/* LED_Task������ */
static TaskHandle_t LED_Task_Handle = NULL;	

/* MOTOR_Task������ */
static TaskHandle_t MOTOR_Task_Handle = NULL;	

/********************************** �ں˶����� *********************************/
/*
 * �ź�������Ϣ���У��¼���־�飬�����ʱ����Щ�������ں˵Ķ���Ҫ��ʹ����Щ�ں�
 * ���󣬱����ȴ����������ɹ�֮��᷵��һ����Ӧ�ľ����ʵ���Ͼ���һ��ָ�룬������
 * �ǾͿ���ͨ��������������Щ�ں˶���
 *
 * �ں˶���˵���˾���һ��ȫ�ֵ����ݽṹ��ͨ����Щ���ݽṹ���ǿ���ʵ��������ͨ�ţ�
 * �������¼�ͬ���ȸ��ֹ��ܡ�������Щ���ܵ�ʵ��������ͨ��������Щ�ں˶���ĺ���
 * ����ɵ�
 * 
 */
SemaphoreHandle_t BinarySem_Handle =NULL; //��ֵ�ź���
QueueHandle_t Test_Queue =NULL; //��Ϣ����

/******************************* ȫ�ֱ������� ************************************/
/*
 * ��������дӦ�ó����ʱ�򣬿�����Ҫ�õ�һЩȫ�ֱ�����
 */
extern char Usart_Rx_Buf[USART_RBUFF_SIZE];

#define  QUEUE_LEN    4   /* ���еĳ��ȣ����ɰ������ٸ���Ϣ */
#define  QUEUE_SIZE   4   /* ������ÿ����Ϣ��С���ֽڣ� */

extern uint32_t ChipUniqueID[3];
//extern uint16_t	WS2812_LED_Buffer[WS2812_Data_Bit*WS2812_LED_Number+1];

extern volatile uint16_t BSP_ENCODER_Number;
uint8_t BSP_MOTOR_Dir =0;   //ֹͣ״̬ 1 ��ת 2 ��ת

/*
*************************************************************************
*                             ��������
*************************************************************************
*/
static void BSP_Init(void);/* ���ڳ�ʼ�����������Դ */

static void AppTaskCreate(void);/* ���ڴ������� */

static void TEST_Task(void* pvParameters);/* TEST_Task����ʵ�� */
static void LED_Task(void* pvParameters);/* LED_Task����ʵ�� */

static void MOTOR_Task(void* pvParameters);     /* MOTOR_Task ����ʵ�� */


/**
  * ʹ���˾�̬�����ڴ棬�������������������û�ʵ�֣�������task.c�ļ���������
  *	���ҽ��� configSUPPORT_STATIC_ALLOCATION ����궨��Ϊ 1 ��ʱ�����Ч
  */
  
/*****************************************************************
  * @brief  ������
  * @param  ��
  * @retval ��
  * @note   ��һ����������Ӳ����ʼ�� 
            �ڶ���������APPӦ������
            ������������FreeRTOS����ʼ���������
  ****************************************************************/
int main(void)
{	
    /* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
    BaseType_t xReturn = pdPASS;
    /* ������Ӳ����ʼ�� */
    BSP_Init();
    
    printf("���ڷ������ݴ����ж�,����������!\n");
   /* ���� AppTaskCreate ���� */ 
    xReturn = xTaskCreate(  (TaskFunction_t )AppTaskCreate,             //������
                            (const char*    )"AppTaskCreate",           //��������
                            (uint16_t       )512,                       //�����ջ��С
                            (void*          )NULL,                      //���ݸ��������Ĳ���
                            (UBaseType_t    )1,                         //�������ȼ�
                            (TaskHandle_t*  )&AppTaskCreate_Handle);    //������ƿ� 
															
	if(pdPASS == xReturn)/* �����ɹ� */
        vTaskStartScheduler();   /* �������񣬿������� */
    else
        return -1;
  
    while(1);   /* ��������ִ�е����� */    
}


/***********************************************************************
  * @ ������  �� AppTaskCreate
  * @ ����˵���� Ϊ�˷���������е����񴴽����������������������
  * @ ����    �� ��  
  * @ ����ֵ  �� ��
  **********************************************************************/
static void AppTaskCreate(void)
{
    /* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
    BaseType_t xReturn = pdPASS;
    
    taskENTER_CRITICAL();           //�����ٽ���

    /* ����TEST_Task���� */                                                     
    xReturn = xTaskCreate((TaskFunction_t )TEST_Task,                   //������
                          (const char*    )"TEST_Task",                 //��������
                          (uint16_t       )128,                         //�����ջ��С
                          (void*          )NULL,                        //���ݸ��������Ĳ���
                          (UBaseType_t    )1,                           //�������ȼ�
                          (TaskHandle_t*  )&TEST_Task_Handle);          //������ƿ�
                          
    if(pdPASS == xReturn)/* �����ɹ� */
    {
        /*  ʲôҲ���� */
        printf("����TEST_Task����ɹ�!\n");
    }
    
//	/* ����LED_Task���� */                                                     
//    xReturn = xTaskCreate((TaskFunction_t )LED_Task,                    //������
//                          (const char*    )"LED_Task",                  //��������
//                          (uint16_t       )32,                          //�����ջ��С
//                          (void*          )NULL,                        //���ݸ��������Ĳ���
//                          (UBaseType_t    )2,                           //�������ȼ�
//                          (TaskHandle_t*  )&LED_Task_Handle);           //������ƿ�
//                          
//	if(pdPASS == xReturn)/* �����ɹ� */
//    {
//        /*  ʲôҲ���� */
//        printf("����LED_Task����ɹ�!\n");
//    }

    /* ����MOTOR_Task���� */                                                     
    xReturn = xTaskCreate((TaskFunction_t )MOTOR_Task,                    //������
                          (const char*    )"MOTOR_Task",                  //��������
                          (uint16_t       )128,                           //�����ջ��С
                          (void*          )NULL,                          //���ݸ��������Ĳ���
                          (UBaseType_t    )2,                             //�������ȼ�
                          (TaskHandle_t*  )&MOTOR_Task_Handle);           //������ƿ�
                          
	if(pdPASS == xReturn)/* �����ɹ� */
    {
        /*  ʲôҲ���� */
        printf("����MOTOR_Task����ɹ�!\n");
    }
    
    
    vTaskDelete(AppTaskCreate_Handle); //ɾ��AppTaskCreate����
  
    taskEXIT_CRITICAL();            //�˳��ٽ���
}

/**********************************************************************
  * @ ������  �� TEST_Task
  * @ ����˵���� TEST_Task��������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  ********************************************************************/
static void TEST_Task(void* parameter)
{	
    /* ���ڱ����ϴ�ʱ�䡣���ú�ϵͳ�Զ����� */
    static portTickType PreviousWakeTime;
//    uint16_t Speed =0;
    uint16_t Speed = 0;
    
    /* ������ʱʱ�䣬��ʱ��תΪ������ */
    const portTickType TimeIncrement = pdMS_TO_TICKS(50); //50msִ��һ������
    
    /* ��ȡ��ǰϵͳʱ�� */
    PreviousWakeTime = xTaskGetTickCount();
    
    while (1)
    {
        printf("��������!\r\n");
        vTaskDelayUntil( &PreviousWakeTime,TimeIncrement );
//        BSP_ENCODER_Number = BSP_ENCODER_Read_CNT(BSP_ENCODER_TIM);
//        if(BSP_MOTOR_Dir == 0)
//        {
//            printf("���ֹͣ\r\n");
//        }
//        if(BSP_MOTOR_Dir == 1)
//        {
//            printf("�����ת\r\n");
//        }
//        if(BSP_MOTOR_Dir == 2)
//        {
//            printf("�����ת\r\n");
//        }
//        if(BSP_ENCODER_Number > BSP_ENCODER_Num)
//        {
//            BSP_ENCODER_Number = BSP_ENCODER_Number - BSP_ENCODER_Num;
//            
//        }
//        else if(BSP_ENCODER_Number < BSP_ENCODER_Num)
//        {
//            BSP_ENCODER_Number = BSP_ENCODER_Num - BSP_ENCODER_Number;
//            
//        }

//        Speed = (BSP_ENCODER_Number * 3.1416 * 65 / 330);
//        printf("����������%d\r\n",BSP_ENCODER_Number);
//        printf("����ٶ�Ϊ��%d\r\n",Speed);
    }
}


/**********************************************************************
  * @ ������  �� LED_Task
  * @ ����˵���� LED_Task��������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  ********************************************************************/
static void LED_Task(void* parameter)
{	
//    static TickType_t pxPreviousWakeTime;
//    const TickType_t xTimeIncrement = pdMS_TO_TICKS(1000);
//    
//    pxPreviousWakeTime = xTaskGetTickCount(); //��ȡ��ǰʱ��
    while (1)
    {
//        vTaskDelayUntil(&pxPreviousWakeTime,xTimeIncrement);
//        printf("start time = %d \n",xTaskGetTickCount());
//        LED_Red_TOGGLE;
//        vTaskDelayUntil(&pxPreviousWakeTime,xTimeIncrement);
//        printf("stop time = %d \n",xTaskGetTickCount());
//        LED_Red_TOGGLE;
        
//        LED_Red_TOGGLE;
//        vTaskDelay(1000);
//        LED_Red_TOGGLE;
//        vTaskDelay(1000);
//        
//        LED_Green_TOGGLE;
//        vTaskDelay(1000);
//        LED_Green_TOGGLE;
//        vTaskDelay(1000);
//        
//        LED_Blue_TOGGLE;
//        vTaskDelay(1000);
//        LED_Blue_TOGGLE;
        vTaskDelay(1000);
    }
}

/**********************************************************************
  * @ ������  �� LED_Task
  * @ ����˵���� LED_Task��������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  ********************************************************************/
static void MOTOR_Task(void* parameter)
{	
    uint8_t pwm = 70;
    while (1)
    {
        
        if(pwm <=30)
        {
            pwm = 80;
        }
        BSP_MOTOR_Dir = 1;
        BSP_MOTOR_PWM_SET(BSP_MOTOR_TIM,3,1,pwm);
        BSP_MOTOR_PWM_SET(BSP_MOTOR_TIM,4,1,0);
        vTaskDelay(1000);
        
        BSP_MOTOR_Dir=0;
        BSP_MOTOR_PWM_SET(BSP_MOTOR_TIM,4,1,0);
        BSP_MOTOR_PWM_SET(BSP_MOTOR_TIM,3,1,0);
        vTaskDelay(500);
        
        BSP_MOTOR_Dir = 2;
        BSP_MOTOR_PWM_SET(BSP_MOTOR_TIM,4,1,pwm);
        BSP_MOTOR_PWM_SET(BSP_MOTOR_TIM,3,1,0);
        vTaskDelay(1000);

        BSP_MOTOR_Dir=0;
        BSP_MOTOR_PWM_SET(BSP_MOTOR_TIM,4,1,0);
        BSP_MOTOR_PWM_SET(BSP_MOTOR_TIM,3,1,0);
        vTaskDelay(500);
        
        
//        BSP_MOTOR_Control(BSP_MOTOR_TIM,2,BSP_MOTOR_FWD,pwm);
//        printf("�����ת\r\n");
//        vTaskDelay(1000);
//        BSP_MOTOR_Control(BSP_MOTOR_TIM,2,BSP_MOTOR_FWD,0);
//        printf("���ֹͣ\r\n");
//        vTaskDelay(1000);
//        
//        BSP_MOTOR_Control(BSP_MOTOR_TIM,2,BSP_MOTOR_REV,pwm);
//        printf("�����ת\r\n");
//        vTaskDelay(1000);
//        BSP_MOTOR_Control(BSP_MOTOR_TIM,2,BSP_MOTOR_REV,0);
//        printf("���ֹͣ\r\n");
//        vTaskDelay(1000);
        
        pwm--;
    }
}

/***********************************************************************
  * @ ������  �� BSP_Init
  * @ ����˵���� �弶�����ʼ�������а����ϵĳ�ʼ�����ɷ��������������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  *********************************************************************/
static void BSP_Init(void)
{
	/*
	 * STM32�ж����ȼ�����Ϊ4����4bit��������ʾ��ռ���ȼ�����ΧΪ��0~15
	 * ���ȼ�����ֻ��Ҫ����һ�μ��ɣ��Ժ������������������Ҫ�õ��жϣ�
	 * ��ͳһ��������ȼ����飬ǧ��Ҫ�ٷ��飬�мɡ�
	 */
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
    
    /* DMA��ʼ��	*/
	USARTx_DMA_Config();
	/* ���ڳ�ʼ��	*/
	USART_Config();  
    BSP_LED_Config(); 
    
    Get_ChipID();
    printf("\r\n ds18b20 exit \r\n");
    
    printf("\r\nоƬ��ΨһIDΪ: 0x%08X-%08X-%08X\r\n",
	        ChipUniqueID[0],ChipUniqueID[1],ChipUniqueID[2]);
    
    printf("\r\nоƬflash������Ϊ: %dK \r\n", *(__IO u16 *)(0X1FFFF7E0));
    
    
    BSP_MOTOR_GPIO_Config();
    BSP_ENCODER_Init();
}


/********************************END OF FILE****************************/
