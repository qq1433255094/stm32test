#include "main.h"

/*----LED_GREEN----PA6-----'0' is on,'1' is off */
/*----LED_RED------PA7-----'0' is on,'1' is off */

void Led_Configuration(void)
{
	//定义一个GPIO_InitTypeDef类型的结构体
  GPIO_InitTypeDef gpio;
  
	//开启GPIOA外设时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	//选择要控制的引脚
	gpio.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	
	//引脚设置为输出模式
	gpio.GPIO_Mode = GPIO_Mode_OUT;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&gpio);
    
  LED_GREEN_OFF();
  LED_RED_OFF();
}

//显示系统运行状态，运行正常为绿灯亮红灭，运行异常绿灯灭红灯亮
void SystemState(char state)
{
	if(state==NORMOL)
	{
		  LED_GREEN_ON();
			LED_RED_OFF();
	}
	if(state==UNNORMOL)
	{
			LED_GREEN_OFF();
			LED_RED_ON();
	}
}
