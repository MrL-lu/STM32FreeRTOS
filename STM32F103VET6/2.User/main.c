/**
  *****************************************************************************
  * @file     （程序名）	  	main.c
  * @author （程序作者）		Lu
  * @version（程序版本）		V1.0.0.0
  * @date   （修改日期）		2024-02-19
  * @brief  （功能简介）		STM32F103VET6固件库模板
  * @parameter  （参数）		无
  * @return   （返回值）		无
  *****************************************************************************
  * @attention （注意事项）
  * 芯片型号：STM32F103VET6
  * 使用引脚：
  * 修改日期： 
  * 实现功能：
  *	
  *****************************************************************************
  */

/*
*************************************************************************
*                             包含的头文件
*************************************************************************
*/

#ifdef      FreeRTOS_OS
/* FreeRTOS头文件 */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "main.h"

#else

#include "stm32f10x.h"   //引用stm32头文件
#include "main.h"

#endif  //FreeRTOS_OS




/**************************** 任务句柄 ********************************/
/* 
 * 任务句柄是一个指针，用于指向一个任务，当任务创建好之后，它就具有了一个任务句柄
 * 以后我们要想操作这个任务都需要通过这个任务句柄，如果是自身的任务操作自己，那么
 * 这个句柄可以为NULL。
 */
 /* 创建任务句柄 */
static TaskHandle_t AppTaskCreate_Handle = NULL;
/* TEST_Task任务句柄 */
static TaskHandle_t TEST_Task_Handle = NULL;		
/* LED_Task任务句柄 */
static TaskHandle_t LED_Task_Handle = NULL;	

/* MOTOR_Task任务句柄 */
static TaskHandle_t MOTOR_Task_Handle = NULL;	

/********************************** 内核对象句柄 *********************************/
/*
 * 信号量，消息队列，事件标志组，软件定时器这些都属于内核的对象，要想使用这些内核
 * 对象，必须先创建，创建成功之后会返回一个相应的句柄。实际上就是一个指针，后续我
 * 们就可以通过这个句柄操作这些内核对象。
 *
 * 内核对象说白了就是一种全局的数据结构，通过这些数据结构我们可以实现任务间的通信，
 * 任务间的事件同步等各种功能。至于这些功能的实现我们是通过调用这些内核对象的函数
 * 来完成的
 * 
 */
SemaphoreHandle_t BinarySem_Handle =NULL; //二值信号量
QueueHandle_t Test_Queue =NULL; //消息队列

/******************************* 全局变量声明 ************************************/
/*
 * 当我们在写应用程序的时候，可能需要用到一些全局变量。
 */
extern char Usart_Rx_Buf[USART_RBUFF_SIZE];

#define  QUEUE_LEN    4   /* 队列的长度，最大可包含多少个消息 */
#define  QUEUE_SIZE   4   /* 队列中每个消息大小（字节） */

extern uint32_t ChipUniqueID[3];
//extern uint16_t	WS2812_LED_Buffer[WS2812_Data_Bit*WS2812_LED_Number+1];

extern volatile uint16_t BSP_ENCODER_Number;
uint8_t BSP_MOTOR_Dir =0;   //停止状态 1 正转 2 反转

/*
*************************************************************************
*                             函数声明
*************************************************************************
*/
static void BSP_Init(void);/* 用于初始化板载相关资源 */

static void AppTaskCreate(void);/* 用于创建任务 */

static void TEST_Task(void* pvParameters);/* TEST_Task任务实现 */
static void LED_Task(void* pvParameters);/* LED_Task任务实现 */

static void MOTOR_Task(void* pvParameters);     /* MOTOR_Task 任务实现 */


/**
  * 使用了静态分配内存，以下这两个函数是由用户实现，函数在task.c文件中有引用
  *	当且仅当 configSUPPORT_STATIC_ALLOCATION 这个宏定义为 1 的时候才有效
  */
  
/*****************************************************************
  * @brief  主函数
  * @param  无
  * @retval 无
  * @note   第一步：开发板硬件初始化 
            第二步：创建APP应用任务
            第三步：启动FreeRTOS，开始多任务调度
  ****************************************************************/
int main(void)
{	
    /* 定义一个创建信息返回值，默认为pdPASS */
    BaseType_t xReturn = pdPASS;
    /* 开发板硬件初始化 */
    BSP_Init();
    
    printf("串口发送数据触发中断,任务处理数据!\n");
   /* 创建 AppTaskCreate 任务 */ 
    xReturn = xTaskCreate(  (TaskFunction_t )AppTaskCreate,             //任务函数
                            (const char*    )"AppTaskCreate",           //任务名称
                            (uint16_t       )512,                       //任务堆栈大小
                            (void*          )NULL,                      //传递给任务函数的参数
                            (UBaseType_t    )1,                         //任务优先级
                            (TaskHandle_t*  )&AppTaskCreate_Handle);    //任务控制块 
															
	if(pdPASS == xReturn)/* 创建成功 */
        vTaskStartScheduler();   /* 启动任务，开启调度 */
    else
        return -1;
  
    while(1);   /* 正常不会执行到这里 */    
}


/***********************************************************************
  * @ 函数名  ： AppTaskCreate
  * @ 功能说明： 为了方便管理，所有的任务创建函数都放在这个函数里面
  * @ 参数    ： 无  
  * @ 返回值  ： 无
  **********************************************************************/
static void AppTaskCreate(void)
{
    /* 定义一个创建信息返回值，默认为pdPASS */
    BaseType_t xReturn = pdPASS;
    
    taskENTER_CRITICAL();           //进入临界区

    /* 创建TEST_Task任务 */                                                     
    xReturn = xTaskCreate((TaskFunction_t )TEST_Task,                   //任务函数
                          (const char*    )"TEST_Task",                 //任务名称
                          (uint16_t       )128,                         //任务堆栈大小
                          (void*          )NULL,                        //传递给任务函数的参数
                          (UBaseType_t    )1,                           //任务优先级
                          (TaskHandle_t*  )&TEST_Task_Handle);          //任务控制块
                          
    if(pdPASS == xReturn)/* 创建成功 */
    {
        /*  什么也不干 */
        printf("创建TEST_Task任务成功!\n");
    }
    
//	/* 创建LED_Task任务 */                                                     
//    xReturn = xTaskCreate((TaskFunction_t )LED_Task,                    //任务函数
//                          (const char*    )"LED_Task",                  //任务名称
//                          (uint16_t       )32,                          //任务堆栈大小
//                          (void*          )NULL,                        //传递给任务函数的参数
//                          (UBaseType_t    )2,                           //任务优先级
//                          (TaskHandle_t*  )&LED_Task_Handle);           //任务控制块
//                          
//	if(pdPASS == xReturn)/* 创建成功 */
//    {
//        /*  什么也不干 */
//        printf("创建LED_Task任务成功!\n");
//    }

    /* 创建MOTOR_Task任务 */                                                     
    xReturn = xTaskCreate((TaskFunction_t )MOTOR_Task,                    //任务函数
                          (const char*    )"MOTOR_Task",                  //任务名称
                          (uint16_t       )128,                           //任务堆栈大小
                          (void*          )NULL,                          //传递给任务函数的参数
                          (UBaseType_t    )2,                             //任务优先级
                          (TaskHandle_t*  )&MOTOR_Task_Handle);           //任务控制块
                          
	if(pdPASS == xReturn)/* 创建成功 */
    {
        /*  什么也不干 */
        printf("创建MOTOR_Task任务成功!\n");
    }
    
    
    vTaskDelete(AppTaskCreate_Handle); //删除AppTaskCreate任务
  
    taskEXIT_CRITICAL();            //退出临界区
}

/**********************************************************************
  * @ 函数名  ： TEST_Task
  * @ 功能说明： TEST_Task任务主体
  * @ 参数    ：   
  * @ 返回值  ： 无
  ********************************************************************/
static void TEST_Task(void* parameter)
{	
    /* 用于保存上次时间。调用后系统自动更新 */
    static portTickType PreviousWakeTime;
//    uint16_t Speed =0;
    uint16_t Speed = 0;
    
    /* 设置延时时间，将时间转为节拍数 */
    const portTickType TimeIncrement = pdMS_TO_TICKS(50); //50ms执行一次任务
    
    /* 获取当前系统时间 */
    PreviousWakeTime = xTaskGetTickCount();
    
    while (1)
    {
        printf("正常运行!\r\n");
        vTaskDelayUntil( &PreviousWakeTime,TimeIncrement );
//        BSP_ENCODER_Number = BSP_ENCODER_Read_CNT(BSP_ENCODER_TIM);
//        if(BSP_MOTOR_Dir == 0)
//        {
//            printf("电机停止\r\n");
//        }
//        if(BSP_MOTOR_Dir == 1)
//        {
//            printf("电机正转\r\n");
//        }
//        if(BSP_MOTOR_Dir == 2)
//        {
//            printf("电机反转\r\n");
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
//        printf("编码器计数%d\r\n",BSP_ENCODER_Number);
//        printf("电机速度为：%d\r\n",Speed);
    }
}


/**********************************************************************
  * @ 函数名  ： LED_Task
  * @ 功能说明： LED_Task任务主体
  * @ 参数    ：   
  * @ 返回值  ： 无
  ********************************************************************/
static void LED_Task(void* parameter)
{	
//    static TickType_t pxPreviousWakeTime;
//    const TickType_t xTimeIncrement = pdMS_TO_TICKS(1000);
//    
//    pxPreviousWakeTime = xTaskGetTickCount(); //获取当前时间
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
  * @ 函数名  ： LED_Task
  * @ 功能说明： LED_Task任务主体
  * @ 参数    ：   
  * @ 返回值  ： 无
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
//        printf("电机正转\r\n");
//        vTaskDelay(1000);
//        BSP_MOTOR_Control(BSP_MOTOR_TIM,2,BSP_MOTOR_FWD,0);
//        printf("电机停止\r\n");
//        vTaskDelay(1000);
//        
//        BSP_MOTOR_Control(BSP_MOTOR_TIM,2,BSP_MOTOR_REV,pwm);
//        printf("电机反转\r\n");
//        vTaskDelay(1000);
//        BSP_MOTOR_Control(BSP_MOTOR_TIM,2,BSP_MOTOR_REV,0);
//        printf("电机停止\r\n");
//        vTaskDelay(1000);
        
        pwm--;
    }
}

/***********************************************************************
  * @ 函数名  ： BSP_Init
  * @ 功能说明： 板级外设初始化，所有板子上的初始化均可放在这个函数里面
  * @ 参数    ：   
  * @ 返回值  ： 无
  *********************************************************************/
static void BSP_Init(void)
{
	/*
	 * STM32中断优先级分组为4，即4bit都用来表示抢占优先级，范围为：0~15
	 * 优先级分组只需要分组一次即可，以后如果有其他的任务需要用到中断，
	 * 都统一用这个优先级分组，千万不要再分组，切忌。
	 */
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
    
    /* DMA初始化	*/
	USARTx_DMA_Config();
	/* 串口初始化	*/
	USART_Config();  
    BSP_LED_Config(); 
    
    Get_ChipID();
    printf("\r\n ds18b20 exit \r\n");
    
    printf("\r\n芯片的唯一ID为: 0x%08X-%08X-%08X\r\n",
	        ChipUniqueID[0],ChipUniqueID[1],ChipUniqueID[2]);
    
    printf("\r\n芯片flash的容量为: %dK \r\n", *(__IO u16 *)(0X1FFFF7E0));
    
    
    BSP_MOTOR_GPIO_Config();
    BSP_ENCODER_Init();
}


/********************************END OF FILE****************************/
