#include "main.h"

/*-PWM1---(PA9---TIM1_CH2)--*/
/*-PWM2---(PA8---TIM1_CH1)--*/

void PWM_Configuration(void)
{
    GPIO_InitTypeDef          gpio;
    TIM_TimeBaseInitTypeDef   tim;
    TIM_OCInitTypeDef         oc;
        
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);		//ʹ��GPIOAʱ���ź�
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);		//ʹ�ܶ�ʱ��TIM1ʱ��

    gpio.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    gpio.GPIO_Mode = GPIO_Mode_AF;							//��Ϊ���ö˿����
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOA,&gpio);
    
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource8, GPIO_AF_TIM1);		//�������ΪTIM��
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource9, GPIO_AF_TIM1);    
    
    tim.TIM_Prescaler = 168-1;								//����Ԥ��Ƶ������Ϊ168��Ƶ����Ϊ1Mhz����1us��������1
    tim.TIM_CounterMode = TIM_CounterMode_Up;	//���ü���ģʽ������Ϊ���ϼ���ģʽ�����������¼�
    tim.TIM_Period = 2000;   //2.5ms//The deafault value is 25000	//��ʱ�����ڣ�����ʱ����0������2499����Ϊ2500�Σ�Ϊһ����ʱ����
    tim.TIM_ClockDivision = TIM_CKD_DIV1;			//���÷�Ƶϵ��:����Ƶ
    TIM_TimeBaseInit(TIM1,&tim);
    
		//PWM1 mode configuration��channal2
//    oc.TIM_OCMode = TIM_OCMode_PWM2;					//����ΪPWMģʽ��
//    oc.TIM_OutputState = TIM_OutputState_Enable;//ʹ�����ģʽ
//    oc.TIM_OutputNState = TIM_OutputState_Disable;//������Ϊ׼�������
//    oc.TIM_Pulse =1300; 												//��������ֵ�������������������ֵʱ����ƽ��������
//    oc.TIM_OCPolarity = TIM_OCPolarity_High;			//������Ч��ƽ�ļ��ԣ���������Ϊ��Ч��ƽΪ�͵�ƽ
//    oc.TIM_OCNPolarity = TIM_OCPolarity_High;			//���û�������
//    oc.TIM_OCIdleState = TIM_OCIdleState_Reset;
//    oc.TIM_OCNIdleState = TIM_OCIdleState_Set;

			oc.TIM_OCMode = TIM_OCMode_PWM2;				  //PWM2ģʽ���
			oc.TIM_OutputState = TIM_OutputState_Enable;	  //�Ƚ����ʹ��
			oc.TIM_OutputNState = TIM_OutputNState_Disable;	  //----
			oc.TIM_Pulse = 0;
			oc.TIM_OCPolarity = TIM_OCPolarity_Low;			  //������Ե�
			oc.TIM_OCNPolarity = TIM_OCPolarity_High;		  //----
			oc.TIM_OCIdleState = TIM_OCIdleState_Reset;
			oc.TIM_OCNIdleState = TIM_OCIdleState_Reset;	  //----

    TIM_OC1Init(TIM1,&oc);
    TIM_OC2Init(TIM1,&oc);
    
    TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);//ʹ��TIM1��CRR1��Ԥװ�ؼĴ���
    TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);//ʹ��TIM1��CRR1��Ԥװ�ؼĴ���
           
    TIM_ARRPreloadConfig(TIM1,ENABLE);//ʹ��TIM1��ARR��Ԥװ�ؼĴ���
    
    TIM_CtrlPWMOutputs(TIM1,ENABLE);//ʹ��TIM1�����
    
    TIM_Cmd(TIM1,ENABLE);//ʹ�ܶ�ʱ��1
}

/////////////////////////////////
void SpeedControl(int CRR_Val)
{
//		TIM_OCInitTypeDef         oc;
//		oc.TIM_OCMode = TIM_OCMode_PWM2;					//����ΪPWMģʽ��
//    oc.TIM_OutputState = TIM_OutputState_Enable;//ʹ�����ģʽ
//    oc.TIM_OutputNState = TIM_OutputState_Disable;//������Ϊ׼�������
//    oc.TIM_Pulse =CRR_Val; 												//��������ֵ�������������������ֵʱ����ƽ��������
//    oc.TIM_OCPolarity = TIM_OCPolarity_High;			//������Ч��ƽ�ļ��ԣ���������Ϊ��Ч��ƽΪ�͵�ƽ
//    oc.TIM_OCNPolarity = TIM_OCPolarity_High;			//���û�������
//    oc.TIM_OCIdleState = TIM_OCIdleState_Reset;
//    oc.TIM_OCNIdleState = TIM_OCIdleState_Set;
//    TIM_OC1Init(TIM1,&oc);
//    TIM_OC2Init(TIM1,&oc);
//		printf("%d\n",CRR_Val);
	TIM_SetCompare1(TIM1,CRR_Val);
}
/////////////////////////////////
