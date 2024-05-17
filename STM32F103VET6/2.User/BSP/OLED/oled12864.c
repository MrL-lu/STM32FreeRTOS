/* 引用头文件 ------------------------------------------------------------------*/  
#include "./OLED/oled12864.h"  //引用oled12864驱动头文件
#include	"./OLED/oled12864font.h"	//字体文件

/* 定义全局变量 ------------------------------------------------------------------*/
uint8_t	OLED_GRAM[144][8];

//extern const unsigned char asc2_0806[][6];
//extern const unsigned char asc2_1206[95][12];
//extern const unsigned char asc2_1608[][16];
//extern const unsigned char asc2_2412[][36];

/**
 * @brief  ：OLED12864-IIC协议延时函数
 * @param  ：无
 * @retval ：无
 * @doc    ：无
 */
void  OLED_IIC_delay(void)
{
	uint8_t t=3;
	while(t--);
}

/**
 * @brief  ：OLED12864-IIC协议起始信号
 * @param  ：当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号
 * @retval ：无
 * @doc    ：无
 */
void	OLED_IIC_Start(void)
{
		OLED_SDA_High;
		OLED_SCL_High;
		OLED_IIC_delay();	//延时
		OLED_SDA_Low;
		OLED_IIC_delay();	//延时
		OLED_SCL_Low;
		OLED_IIC_delay();	//延时
}

/**
 * @brief  ：OLED12864-IIC协议停止信号
 * @param  ：当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号
 * @retval ：无
 * @doc    ：无
 */
void	OLED_IIC_Stop(void)
{
		OLED_SDA_Low;
		OLED_SCL_High;
		OLED_IIC_delay();	//延时
		OLED_SDA_High;
}

/**
 * @brief  ：OLED12864-IIC协议读取应答信号
 * @param  ：当SCL高电平时，SDA保持至少4us的低电平
 * @retval ：1 - 器件异常  0 - 器件正常
 * @doc    ：无
 */
void	OLED_IIC_WaitACK(void)
{
		OLED_SDA_High;
		OLED_IIC_delay();	//延时
		OLED_SCL_High;
		OLED_IIC_delay();	//延时
		OLED_SCL_Low;
		OLED_IIC_delay();	//延时
}

/**
 * @brief  ：OLED12864-IIC协议写入一个字节
 * @param  ：CPU向I2C总线设备发送8bit数据
 * @arg    ：data : 要发送的字节
 * @doc    ：从高位到低位开始
 */
void	OLED_IIC_WriteByte(uint8_t data)
{
		uint8_t	i;
		for(i=0;i<8;i++)
			{
				if(data & 0x80 )
				{
					OLED_SDA_High;
				}
				else
				{
					OLED_SDA_Low;
				}
				OLED_IIC_delay();	//延时
				OLED_SCL_High;
				OLED_IIC_delay();	//延时
				OLED_SCL_Low;
				data <<=1; //左移一位
			}
}

/**
 * @brief  ：OLED12864-IIC协议写入一个命令
 * @param  ：
 * @retval ：data : 发送的数据/命令
 * @doc    ：从高位到低位开始
 */
void	OLED_Write_CMD(uint8_t data)
{
		OLED_IIC_Start();	//发送起始信号
		OLED_IIC_WriteByte(OLED_Address);	//写入OLED的地址
		OLED_IIC_WaitACK();	//应答
		
		OLED_IIC_WriteByte(0x00);	//写命令
		OLED_IIC_WaitACK();	//应答
	
		OLED_IIC_WriteByte(data);	//
		OLED_IIC_WaitACK();	//应答
		OLED_IIC_Stop();	//停止信号
}

/**
 * @brief  ：OLED12864-IIC协议写入一个数据
 * @param  ：
 * @retval ：data : 发送的数据/命令
 * @doc    ：从高位到低位开始
 */
void	OLED_Write_Data( uint8_t data )
{
		OLED_IIC_Start();	//发送起始信号
		OLED_IIC_WriteByte(OLED_Address);	//写入OLED的地址
		OLED_IIC_WaitACK();	//应答
		
		OLED_IIC_WriteByte(0x40);	//写数据
		OLED_IIC_WaitACK();	//应答
	
		OLED_IIC_WriteByte(data);	//
		OLED_IIC_WaitACK();	//应答
		OLED_IIC_Stop();	//停止信号
}

/**
 * @brief  ：OLED12864-IIC协议 开启OLED显示
 * @param  ：无
 * @retval ：无
 * @doc    ：无
 */
void  OLED_IIC_Display_ON(void)
{
		OLED_Write_CMD(0x8D);	// 电荷泵使能
		OLED_Write_CMD(0x14);	// 开启电荷泵
		OLED_Write_CMD(0xAF);	// 点亮屏幕
}

/**
 * @brief  ：OLED12864-IIC协议 关闭OLED显示
 * @param  ：无
 * @retval ：无
 * @doc    ：无
 */
void OLED_IIC_Display_OFF(void)
{
		OLED_Write_CMD(0x8D);	// 电荷泵使能
		OLED_Write_CMD(0x10);	// 关闭电荷泵
		OLED_Write_CMD(0xAE);	// 关闭屏幕
}


/**
 * @brief  ：OLED12864-IIC协议 更新显存到OLED
 * @param  ：无
 * @retval ：无
 * @doc    ：无
 */
void	OLED_IIC_Refresh(void)
{
		uint8_t	i,n;
		for(i=0;i<8;i++)
			{
				OLED_Write_CMD(0xb0+i);
				OLED_Write_CMD(0x00);
				OLED_Write_CMD(0x10);
				OLED_IIC_Start();
				OLED_IIC_WriteByte(OLED_Address);
				OLED_IIC_WaitACK();
				OLED_IIC_WriteByte(0x40);
				OLED_IIC_WaitACK();
				for(n=0;n<128;n++)
					{
						OLED_IIC_WriteByte(OLED_GRAM[n][i]);
						OLED_IIC_WaitACK();
					}
				OLED_IIC_Stop();
			}
}

/**
 * @brief  ：OLED12864-IIC协议 清屏函数
 * @param  ：无
 * @retval ：无
 * @doc    ：无
 */
void	OLED_Clear(void)
{
		uint8_t	i,n;
		for(i=0;i<8;i++)
			{
				for(n=0;n<128;n++)
					{
						OLED_GRAM[n][i]=0;//清除所有数据
					}
			}
		OLED_IIC_Refresh();//更新显示
}



/**
 * @brief  ：OLED12864-IIC协议 初始化函数
 * @param  ：无
 * @retval ：无
 * @doc    ：无
 */
void	OLED_IIC_Init(void)
{
		GPIO_InitTypeDef  GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(OLED_SCL_GPIO_RCC_APB2, ENABLE);	 //使能端口时钟
		GPIO_InitStructure.GPIO_Pin = OLED_SCL_GPIO_Pin;	 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		 //开漏输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
		GPIO_Init(OLED_SCL_GPIOX, &GPIO_InitStructure);	  //初始化

		RCC_APB2PeriphClockCmd(OLED_SDA_GPIO_RCC_APB2, ENABLE);	 //使能端口时钟
		GPIO_InitStructure.GPIO_Pin = OLED_SDA_GPIO_Pin;	 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		 //开漏输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
		GPIO_Init(OLED_SDA_GPIOX, &GPIO_InitStructure);	  //初始化
	
		SysTick_Delay_ms(200);	//这里的延时很重要,上电后延时，没有错误的冗余设计,至少200ms
		OLED_Write_CMD(0xAE);//--turn off oled panel
		OLED_Write_CMD(0x00);//---set low column address
		OLED_Write_CMD(0x10);//---set high column address
		OLED_Write_CMD(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
		OLED_Write_CMD(0x81);//--set contrast control register
		OLED_Write_CMD(0xCF);// Set SEG Output Current Brightness
		OLED_Write_CMD(0xA1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
		OLED_Write_CMD(0xC8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
		OLED_Write_CMD(0xA6);//--set normal display
		OLED_Write_CMD(0xA8);//--set multiplex ratio(1 to 64)
		OLED_Write_CMD(0x3f);//--1/64 duty
		OLED_Write_CMD(0xD3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
		OLED_Write_CMD(0x00);//-not offset
		OLED_Write_CMD(0xd5);//--set display clock divide ratio/oscillator frequency
		OLED_Write_CMD(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
		OLED_Write_CMD(0xD9);//--set pre-charge period
		OLED_Write_CMD(0xF1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
		OLED_Write_CMD(0xDA);//--set com pins hardware configuration
		OLED_Write_CMD(0x12);
		OLED_Write_CMD(0xDB);//--set vcomh
		OLED_Write_CMD(0x40);//Set VCOM Deselect Level
		OLED_Write_CMD(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
		OLED_Write_CMD(0x02);//
		OLED_Write_CMD(0x8D);//--set Charge Pump enable/disable
		OLED_Write_CMD(0x14);//--set(0x10) disable
		OLED_Write_CMD(0xA4);// Disable Entire Display On (0xa4/0xa5)
		OLED_Write_CMD(0xA6);// Disable Inverse Display On (0xa6/a7) 
		OLED_Clear();
		OLED_Write_CMD(0xAF);
		
}


/**
 * @brief  ：OLED12864-IIC协议 反显函数
 * @param  ：无
 * @retval ：无
 * @doc    ：无
 */
void 	OLED_ColorTurn(uint8_t i)
{
	if(i==0)
		{
			OLED_Write_CMD(0xA6);//正常显示
		}
	if(i==1)
		{
			OLED_Write_CMD(0xA7);//反色显示
		}
}

/**
 * @brief  ：OLED12864-IIC协议 屏幕旋转180度
 * @param  ：无
 * @retval ：无
 * @doc    ：无
 */
void  OLED_DisplayTurn(uint8_t i)
{
	if(i==0)
		{
			OLED_Write_CMD(0xC8);//正常显示
			OLED_Write_CMD(0xA1);
		}
	if(i==1)
		{
			OLED_Write_CMD(0xC0);//反转显示
			OLED_Write_CMD(0xA0);
		}
}






/**
 * @brief  ：OLED12864-IIC协议 点亮屏幕一个点
 * @param  ：x 横坐标 y 纵坐标 t是否填充
 * @arg    ：x :0-127 y:0-63 t:0(填充)/1(清空)
 * @retval ：无
 * @doc    ：无
 */
//画点 
//x:0~127
//y:0~63
//t:1 填充 0,清空	
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 i,m,n;
	i=y/8;
	m=y%8;
	n=1<<m;
	if(t){OLED_GRAM[x][i]|=n;}
	else
	{
		OLED_GRAM[x][i]=~OLED_GRAM[x][i];
		OLED_GRAM[x][i]|=n;
		OLED_GRAM[x][i]=~OLED_GRAM[x][i];
	}
}

/**
 * @brief  ：OLED12864-IIC协议 在指定位置显示一个字符,包括部分字符
 * @param  ：x 横坐标 y 纵坐标 size1:选择字体 mode:显示模式
 * @arg    ：x :0-127 y:0-63  size1: 6x8/6x12/8x16/12x24 mode:0,反色显示;1,正常显示
 * @retval ：无
 * @doc    ：无
 */
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//size1:选择字体 6x8/6x12/8x16/12x24
//mode:0,反色显示;1,正常显示
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1,u8 mode)
{
	u8 i,m,temp,size2,chr1;
	u8 x0=x,y0=y;
	if(size1==8)size2=6;
	else size2=(size1/8+((size1%8)?1:0))*(size1/2);  //得到字体一个字符对应点阵集所占的字节数
	chr1=chr-' ';  //计算偏移后的值
	for(i=0;i<size2;i++)
	{
		if(size1==8)
			  {temp=asc2_0806[chr1][i];} //调用0806字体
		else if(size1==12)
        {temp=asc2_1206[chr1][i];} //调用1206字体
		else if(size1==16)
        {temp=asc2_1608[chr1][i];} //调用1608字体
		else if(size1==24)
        {temp=asc2_2412[chr1][i];} //调用2412字体
		else return;
		for(m=0;m<8;m++)
		{
			if(temp&0x01)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp>>=1;
			y++;
		}
		x++;
		if((size1!=8)&&((x-x0)==size1/2))
		{x=x0;y0=y0+8;}
		y=y0;
  }
}




/**
 * @brief  ：OLED12864-IIC协议 显示字符串
 * @param  ：x 横坐标 y 纵坐标 size1:选择字体大小 *chr:字符串起始地址 mode:0,反色显示;1,正常显示
 * @arg    ：x :0-127 y:0-63  size1: 6x8/6x12/8x16/12x24 mode:0,反色显示;1,正常显示
 * @retval ：无
 * @doc    ：无
 */
//显示字符串
//x,y:起点坐标  
//size1:字体大小 
//*chr:字符串起始地址 
//mode:0,反色显示;1,正常显示
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1,u8 mode)
{
	while((*chr>=' ')&&(*chr<='~'))//判断是不是非法字符!
	{
		OLED_ShowChar(x,y,*chr,size1,mode);
		if(size1==8)x+=6;
		else x+=size1/2;
		chr++;
  }
}



//m^n
u32 OLED_Pow(u8 m,u8 n)
{
	u32 result=1;
	while(n--)
	{
	  result*=m;
	}
	return result;
}

//显示数字
//x,y :起点坐标
//num :要显示的数字
//len :数字的位数
//size:字体大小
//mode:0,反色显示;1,正常显示
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1,u8 mode)
{
	u8 t,temp,m=0;
	if(size1==8)m=2;
	for(t=0;t<len;t++)
	{
		temp=(num/OLED_Pow(10,len-t-1))%10;
			if(temp==0)
			{
				OLED_ShowChar(x+(size1/2+m)*t,y,'0',size1,mode);
      }
			else 
			{
			  OLED_ShowChar(x+(size1/2+m)*t,y,temp+'0',size1,mode);
			}
  }
}



//显示汉字
//x,y:起点坐标
//num:汉字对应的序号
//mode:0,反色显示;1,正常显示
void OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1,u8 mode)
{
	u8 m,temp;
	u8 x0=x,y0=y;
	u16 i,size3=(size1/8+((size1%8)?1:0))*size1;  //得到字体一个字符对应点阵集所占的字节数
	for(i=0;i<size3;i++)
	{
		if(size1==16)
				{temp=Hzk1[num][i];}//调用16*16字体
		else if(size1==24)
				{temp=Hzk2[num][i];}//调用24*24字体
		else if(size1==32)       
				{temp=Hzk3[num][i];}//调用32*32字体
		else if(size1==64)
				{temp=Hzk4[num][i];}//调用64*64字体
		else return;
		for(m=0;m<8;m++)
		{
			if(temp&0x01)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp>>=1;
			y++;
		}
		x++;
		if((x-x0)==size1)
		{x=x0;y0=y0+8;}
		y=y0;
	}
}



//num 显示汉字的个数
//space 每一遍显示的间隔
//mode:0,反色显示;1,正常显示
void OLED_ScrollDisplay(u8 num,u8 space,u8 mode)
{
	u8 i,n,t=0,m=0,r;
	while(1)
	{
		if(m==0)
		{
	    OLED_ShowChinese(128,24,t,16,mode); //写入一个汉字保存在OLED_GRAM[][]数组中
			t++;
		}
		if(t==num)
			{
				for(r=0;r<16*space;r++)      //显示间隔
				 {
					for(i=1;i<144;i++)
						{
							for(n=0;n<8;n++)
							{
								OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
							}
						}
           OLED_IIC_Refresh();
				 }
        t=0;
      }
		m++;
		if(m==16){m=0;}
		for(i=1;i<144;i++)   //实现左移
		{
			for(n=0;n<8;n++)
			{
				OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
			}
		}
		OLED_IIC_Refresh();
	}
}


//x,y：起点坐标
//sizex,sizey,图片长宽
//BMP[]：要写入的图片数组
//mode:0,反色显示;1,正常显示
void OLED_ShowPicture(u8 x,u8 y,u8 sizex,u8 sizey,u8 BMP[],u8 mode)
{
	u16 j=0;
	u8 i,n,temp,m;
	u8 x0=x,y0=y;
	sizey=sizey/8+((sizey%8)?1:0);
	for(n=0;n<sizey;n++)
	{
		 for(i=0;i<sizex;i++)
		 {
				temp=BMP[j];
				j++;
				for(m=0;m<8;m++)
				{
					if(temp&0x01)OLED_DrawPoint(x,y,mode);
					else OLED_DrawPoint(x,y,!mode);
					temp>>=1;
					y++;
				}
				x++;
				if((x-x0)==sizex)
				{
					x=x0;
					y0=y0+8;
				}
				y=y0;
     }
	 }
}
/* END OF FILE ------------------------------------------------------------------*/



