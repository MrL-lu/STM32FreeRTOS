/* 引用头文件 ------------------------------------------------------------------*/  
#include "./PID/bsp_pid.h"  //引用PID算法头文件

/* 定义全局变量 ------------------------------------------------------------------*/
bsp_pid pid;

/* 程序开始 ----------------------------------------------------------------------*/

/**
  **********************************************************************
  * @brief   PID参数初始化
  * @author  Lu
  * @version V1.0
  * @date    2024-xx-xx
  **********************************************************************
  */ 
void BSP_PID_init()
{
		/* 初始化参数 */
//    printf("PID_init begin \n");
    pid.target_val=200.0;				
    pid.actual_val=0.0;
    pid.err=0.0;
    pid.err_last=0.0;
    pid.integral=0.0;
//		pid.Kp = 0.31;
//		pid.Ki = 0.070;
//		pid.Kd = 0.3;
//		pid.Kp = 0.21;
//		pid.Ki = 0.070;
//		pid.Kd = 0.3;
		pid.Kp = 0.01;//24
		pid.Ki = 0.80;
		pid.Kd = 0.04;

}


/**
  **********************************************************************
  * @brief  设置目标值
  *			temp_val	:		目标值
  * @author  Lu
  * @version V1.0
  * @date    2024-xx-xx
  **********************************************************************
  */ 
void BSP_Set_PID_Target(float temp_val)
{
    pid.target_val = temp_val;    // 设置当前的目标值
}

/**
  **********************************************************************
  * @brief   获取目标值
  * @author  Lu
  * @version V1.0
  * @date    2024-xx-xx
  **********************************************************************
  */ 
float get_pid_target(void)
{
  return pid.target_val;    // 设置当前的目标值
}

/**
  **********************************************************************
  * @brief  设置比例、积分、微分系数
  * @param  p：比例系数 P
  * @param  i：积分系数 i
  * @param  d：微分系数 d
  * @author  Lu
  * @version V1.0
  * @date    2024-xx-xx
  **********************************************************************
  */ 
void set_p_i_d(float p, float i, float d)
{
  	pid.Kp = p;    // 设置比例系数 P
	pid.Ki = i;    // 设置积分系数 I
	pid.Kd = d;    // 设置微分系数 D
}

/**
  **********************************************************************
  * @brief  PID算法实现
  * @param  temp_val：实际值
  * @retval 通过PID计算后的输出
  * @author  Lu
  * @version V1.0
  * @date    2024-xx-xx
  **********************************************************************
  */ 
float PID_realize(float temp_val)
{
	/*计算目标值与实际值的误差*/
    pid.err=pid.target_val-temp_val;
	/*误差累积*/
    pid.integral+=pid.err;
	/*PID算法实现*/
    pid.actual_val=pid.Kp*pid.err+pid.Ki*pid.integral+pid.Kd*(pid.err-pid.err_last);
	/*误差传递*/
    pid.err_last=pid.err;
	/*返回当前实际值*/
    return pid.actual_val;
}

/**
  **********************************************************************
  * @brief   定时器周期调用函数
  * @author  Lu
  * @version V1.0
  * @date    2024-xx-xx
  **********************************************************************
  */ 
void time_period_fun()
{
	static int flag=0;
	static int num=0;
	static int run_i=0;
		
	if(!flag)
	{
		float val=PID_realize(pid.actual_val);
		printf("val,%f;act,%f\n",pid.target_val,val);	
		run_i++;
		if(abs(val-pid.target_val)<=1)
		{
			num++;
		}
		else//必须满足连续次数
		{
			num=0;
		}
		if(num>20)//稳定次数
		{
			printf("PID算法运行%d 次后稳定\r\n",run_i);
			flag=1;
		}
	}
}

/* END OF FILE ------------------------------------------------------------------*/

