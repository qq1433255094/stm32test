#include "main.h"

/*-----USART3_TX-----PB10-----*/
/*-----USART3_RX-----PB11-----*/

void USART3_Configuration(void)
{
    USART_InitTypeDef usart3;
		GPIO_InitTypeDef  gpio;
    NVIC_InitTypeDef  nvic;
	
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); 
	
		gpio.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
		gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOB,&gpio);

		usart3.USART_BaudRate = 115200;								//波特率设置
		usart3.USART_WordLength = USART_WordLength_8b;//配置串口传输的字长：8bite
		usart3.USART_StopBits = USART_StopBits_1;			//配置停止位
		usart3.USART_Parity = USART_Parity_No;				//配置奇偶校验位，不设置奇偶校验
		usart3.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;//配置串口的模式：为了配置双线全双工通信
		usart3.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//死否采用硬件流控制
		USART_Init(USART3,&usart3);

    USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
		USART_Cmd(USART3,ENABLE);
    
    nvic.NVIC_IRQChannel = USART3_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 3;
    nvic.NVIC_IRQChannelSubPriority = 3;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
}

void USART3_SendChar(unsigned char b)
{
    while (USART_GetFlagStatus(USART3,USART_FLAG_TC) == RESET);
		USART_SendData(USART3,b);
}

int fputc(int ch, FILE *f)				//重定向C库函数printf到USART3
{
		USART_SendData(USART3, (uint8_t)ch);
    while (USART_GetFlagStatus(USART3,USART_FLAG_TC) == RESET);
    USART_SendData(USART3, (uint8_t)ch);
    return ch;
}

char Rx_data[8],num=7,temp[8],chage;
extern short int gyr_z;
void USART3_IRQHandler(void)
{
	
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART3,USART_IT_RXNE);
				for(num=0;num<7;num++)
					temp[num]=temp[num+1];
					
				temp[7]  = USART3->DR;

					LED_GREEN_ON();
					LED_RED_OFF();
				if(temp[0]=='S' && temp[1]=='T' && temp[6]=='E' && temp[7]=='D')
				{
					LED_GREEN_ON();
					LED_RED_OFF();
					for(chage=0;chage<8;chage++)
					Rx_data[chage]=temp[chage];
					gyr_z = ((Rx_data[2]<<8)+Rx_data[3]);
					if(gyr_z>32758)
						gyr_z=-65536+gyr_z;					
					gyr_z/=8.2;
			 // 	printf("angle %d\n",gyr_z);
				}

    }
}
