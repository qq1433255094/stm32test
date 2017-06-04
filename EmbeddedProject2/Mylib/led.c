#include "main.h"

/*----LED_GREEN----PA6-----'0' is on,'1' is off */
/*----LED_RED------PA7-----'0' is on,'1' is off */

void Led_Configuration(void)
{
	//����һ��GPIO_InitTypeDef���͵Ľṹ��
  GPIO_InitTypeDef gpio;
  
	//����GPIOA����ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	//ѡ��Ҫ���Ƶ�����
	gpio.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	
	//��������Ϊ���ģʽ
	gpio.GPIO_Mode = GPIO_Mode_OUT;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&gpio);
    
  LED_GREEN_OFF();
  LED_RED_OFF();
}

//��ʾϵͳ����״̬����������Ϊ�̵������������쳣�̵�������
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
