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
/* FreeRTOSͷ�ļ� */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* ������Ӳ��bspͷ�ļ� */
#include "./usart/bsp_usart.h"  //����bsp_usart.h ͷ�ļ�
#include "./LED/bsp_led.h"  //����bsp_led����ͷ�ļ�

#include "./MOTOR/bsp_motor.h"  //���õ������ͷ�ļ�
#include "./MOTOR/bsp_encoder.h"  //���ñ���������ͷ�ļ�

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
///* ENCODER_Task������ */
//static TaskHandle_t ENCODER_Task_Handle = NULL;

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
SemaphoreHandle_t BinarySem_Handle =NULL;

/******************************* ȫ�ֱ������� ************************************/
/*
 * ��������дӦ�ó����ʱ�򣬿�����Ҫ�õ�һЩȫ�ֱ�����
 */
extern char Usart_Rx_Buf[USART_RBUFF_SIZE];

extern volatile uint16_t BSP_ENCODER_Overflow_Count;   //��ʱ���������
uint8_t DIR=0;


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
//static void ENCODER_Task(void* pvParameters);   /* ENCODER_Task ����ʵ�� */

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
    
    /* ���� BinarySem */
    BinarySem_Handle = xSemaphoreCreateBinary();
    
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
    
	/* ����LED_Task���� */                                                     
    xReturn = xTaskCreate((TaskFunction_t )LED_Task,                    //������
                          (const char*    )"LED_Task",                  //��������
                          (uint16_t       )128,                         //�����ջ��С
                          (void*          )NULL,                        //���ݸ��������Ĳ���
                          (UBaseType_t    )2,                           //�������ȼ�
                          (TaskHandle_t*  )&LED_Task_Handle);           //������ƿ�
                          
	if(pdPASS == xReturn)/* �����ɹ� */
    {
        /*  ʲôҲ���� */
        printf("����LED_Task����ɹ�!\n");
    }
    
    /* ����LED_Task���� */                                                     
    xReturn = xTaskCreate((TaskFunction_t )MOTOR_Task,                    //������
                          (const char*    )"MOTOR_Task",                  //��������
                          (uint16_t       )128,                         //�����ջ��С
                          (void*          )NULL,                        //���ݸ��������Ĳ���
                          (UBaseType_t    )2,                           //�������ȼ�
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
    uint16_t speed =0;
    
    /* ������ʱʱ�䣬��ʱ��תΪ������ */
    const portTickType TimeIncrement = pdMS_TO_TICKS(300);
    
    /* ��ȡ��ǰϵͳʱ�� */
    PreviousWakeTime = xTaskGetTickCount();
    
    while (1)
    {
        speed = BSP_ENCODER_Overflow_Count*BSP_ENCODER_TIM_PERIOD + BSP_ENCODER_Read_CNT(BSP_ENCODER_TIM);
        if(DIR == 1)
        {
            printf("��ת����������%d\r\n",speed);
        }
        else
        {
            printf("��ת����������%d\r\n",speed);
        }
        
//        printf("����������%d\r\n",speed);
        vTaskDelayUntil( &PreviousWakeTime,TimeIncrement );
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
    while (1)
    {
        LED_Red_TOGGLE;
        vTaskDelay(1000);
        LED_Red_TOGGLE;
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
    uint8_t PWM = 50;
    while (1)
    {
        if(PWM < 20)
        {
            PWM = 80;
        }
        PWM--;
        DIR = 1;
        BSP_MOTOR_PWM_SET(BSP_MOTOR_TIM,3,1,PWM);
        BSP_MOTOR_PWM_SET(BSP_MOTOR_TIM,4,1,50);
        vTaskDelay(500);
        
        DIR = 0;
        BSP_MOTOR_PWM_SET(BSP_MOTOR_TIM,4,1,PWM);
        BSP_MOTOR_PWM_SET(BSP_MOTOR_TIM,3,1,50);
        vTaskDelay(500);
    }
}


///**********************************************************************
//  * @ ������  �� LED_Task
//  * @ ����˵���� LED_Task��������
//  * @ ����    ��   
//  * @ ����ֵ  �� ��
//  ********************************************************************/
//static void ENCODER_Task(void* parameter)
//{	
//    while (1)
//    {
//        vTaskDelay(200);
//        printf("��������!\r\n");
//    }
//}


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
    
    BSP_MOTOR_GPIO_Config();
    BSP_ENCODER_Init();

}
 
/********************************END OF FILE****************************/
