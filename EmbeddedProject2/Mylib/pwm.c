#include "main.h"

/*-PWM1---(PA9---TIM1_CH2)--*/
/*-PWM2---(PA8---TIM1_CH1)--*/

void PWM_Configuration(void)
{
    GPIO_InitTypeDef          gpio;
    TIM_TimeBaseInitTypeDef   tim;
    TIM_OCInitTypeDef         oc;
        
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);		//使能GPIOA时钟信号
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);		//使能定时器TIM1时钟

    gpio.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    gpio.GPIO_Mode = GPIO_Mode_AF;							//作为复用端口输出
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOA,&gpio);
    
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource8, GPIO_AF_TIM1);		//复用输出为TIM１
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource9, GPIO_AF_TIM1);    
    
    tim.TIM_Prescaler = 168-1;								//设置预分频：这里为168分频，即为1Mhz，即1us计数器±1
    tim.TIM_CounterMode = TIM_CounterMode_Up;	//设置计数模式：设置为向上计数模式，产生上溢事件
    tim.TIM_Period = 2000;   //2.5ms//The deafault value is 25000	//定时器周期：当定时器从0计数到2499，即为2500次，为一个定时周期
    tim.TIM_ClockDivision = TIM_CKD_DIV1;			//设置分频系数:不分频
    TIM_TimeBaseInit(TIM1,&tim);
    
		//PWM1 mode configuration：channal2
//    oc.TIM_OCMode = TIM_OCMode_PWM2;					//配置为PWM模式２
//    oc.TIM_OutputState = TIM_OutputState_Enable;//使能输出模式
//    oc.TIM_OutputNState = TIM_OutputState_Disable;//不配置为准互补输出
//    oc.TIM_Pulse =1300; 												//设置跳变值，当计数器计数到这个值时，电平发生跳变
//    oc.TIM_OCPolarity = TIM_OCPolarity_High;			//设置有效电平的极性，这里设置为有效电平为低电平
//    oc.TIM_OCNPolarity = TIM_OCPolarity_High;			//设置互补极性
//    oc.TIM_OCIdleState = TIM_OCIdleState_Reset;
//    oc.TIM_OCNIdleState = TIM_OCIdleState_Set;

			oc.TIM_OCMode = TIM_OCMode_PWM2;				  //PWM2模式输出
			oc.TIM_OutputState = TIM_OutputState_Enable;	  //比较输出使能
			oc.TIM_OutputNState = TIM_OutputNState_Disable;	  //----
			oc.TIM_Pulse = 0;
			oc.TIM_OCPolarity = TIM_OCPolarity_Low;			  //输出极性低
			oc.TIM_OCNPolarity = TIM_OCPolarity_High;		  //----
			oc.TIM_OCIdleState = TIM_OCIdleState_Reset;
			oc.TIM_OCNIdleState = TIM_OCIdleState_Reset;	  //----

    TIM_OC1Init(TIM1,&oc);
    TIM_OC2Init(TIM1,&oc);
    
    TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);//使能TIM1在CRR1上预装载寄存器
    TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);//使能TIM1在CRR1上预装载寄存器
           
    TIM_ARRPreloadConfig(TIM1,ENABLE);//使能TIM1在ARR上预装载寄存器
    
    TIM_CtrlPWMOutputs(TIM1,ENABLE);//使能TIM1主输出
    
    TIM_Cmd(TIM1,ENABLE);//使能定时器1
}

/////////////////////////////////
void SpeedControl(int CRR_Val)
{
//		TIM_OCInitTypeDef         oc;
//		oc.TIM_OCMode = TIM_OCMode_PWM2;					//配置为PWM模式２
//    oc.TIM_OutputState = TIM_OutputState_Enable;//使能输出模式
//    oc.TIM_OutputNState = TIM_OutputState_Disable;//不配置为准互补输出
//    oc.TIM_Pulse =CRR_Val; 												//设置跳变值，当计数器计数到这个值时，电平发生跳变
//    oc.TIM_OCPolarity = TIM_OCPolarity_High;			//设置有效电平的极性，这里设置为有效电平为低电平
//    oc.TIM_OCNPolarity = TIM_OCPolarity_High;			//设置互补极性
//    oc.TIM_OCIdleState = TIM_OCIdleState_Reset;
//    oc.TIM_OCNIdleState = TIM_OCIdleState_Set;
//    TIM_OC1Init(TIM1,&oc);
//    TIM_OC2Init(TIM1,&oc);
//		printf("%d\n",CRR_Val);
	TIM_SetCompare1(TIM1,CRR_Val);
}
/////////////////////////////////
