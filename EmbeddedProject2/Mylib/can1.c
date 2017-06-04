#include "main.h"

/*----CAN1_TX-----PA12----*/
/*----CAN1_RX-----PA11----*/

void CAN1_Configuration(void)
{
    CAN_InitTypeDef        can;					//
    CAN_FilterInitTypeDef  can_filter;
    GPIO_InitTypeDef       gpio;
    NVIC_InitTypeDef       nvic;

		//外设时钟设置
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_CAN1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_CAN1);

    gpio.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_11;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    GPIO_Init(GPIOA, &gpio);
    
    nvic.NVIC_IRQChannel = CAN1_RX0_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 2;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
    
    nvic.NVIC_IRQChannel = CAN1_TX_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);    
					
    CAN_DeInit(CAN1);
    CAN_StructInit(&can);
    
    can.CAN_TTCM = DISABLE;		//配置CAN的时间触发通信模式
    can.CAN_ABOM = DISABLE;		//当CAN检测到发送错误或接收错误时一定值时自动进入离线状态
    can.CAN_AWUM = DISABLE;		//是否开启自动唤醒功能
    can.CAN_NART = DISABLE;		//是否禁止报文自动重传
    can.CAN_RFLM = DISABLE;		//配置接收FIFO是否锁定，若使能则FIFO溢出时丢弃下一个接收的报文，否则则是覆盖原报文
    can.CAN_TXFP = ENABLE;		//按照报文的请求顺序来决定报文发送优先级的顺序
    can.CAN_Mode = CAN_Mode_Normal;//正常工作模式
    can.CAN_SJW  = CAN_SJW_1tq;//重新同步跳跃宽度
    can.CAN_BS1 = CAN_BS1_9tq;	//相位缓冲段1
    can.CAN_BS2 = CAN_BS2_4tq;	//相位缓冲段2
    can.CAN_Prescaler = 3;   		//CAN BaudRate 42/(1+9+4)/3=1Mbps//3：3.24
    CAN_Init(CAN1, &can);

		can_filter.CAN_FilterNumber=0;//过滤器组0
		can_filter.CAN_FilterMode=CAN_FilterMode_IdMask;//标识符屏蔽模式
		can_filter.CAN_FilterScale=CAN_FilterScale_32bit;//32位模式
		can_filter.CAN_FilterIdHigh=0x0000;
		can_filter.CAN_FilterIdLow=0x0000;
		can_filter.CAN_FilterMaskIdHigh=0x0000;
		can_filter.CAN_FilterMaskIdLow=0x0000;
		can_filter.CAN_FilterFIFOAssignment=0;//存储位置为0
		can_filter.CAN_FilterActivation=ENABLE;//使能过滤器
		CAN_FilterInit(&can_filter);
    
    CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//FIFO 0 message pending Interrupt
    CAN_ITConfig(CAN1,CAN_IT_TME,ENABLE); //Transmit mailbox empty Interrupt
}

unsigned char CanTxSuccessFlag=0;

void CAN1_TX_IRQHandler(void)
{
		CanTxMsg TxMessage;
	
    if (CAN_GetITStatus(CAN1,CAN_IT_TME)!= RESET) 
		{
				CAN_ClearITPendingBit(CAN1,CAN_IT_TME);
    }
}


void CAN1_RX0_IRQHandler(void)
{
    CanRxMsg RxMessage;
			
    if (CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET)
		{
        CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
        CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
    }
}


u8 Can1SendMsg(u8* msg,u8 len,u8 id)
{ 
	u8 mbox;
	u16 i=0;
	CanTxMsg TxMessage;
	TxMessage.StdId=(id<<3);      // 标准标识符
	TxMessage.ExtId=0;      			//扩展标识符
	TxMessage.IDE=CAN_ID_STD;     // 选择标准标识符
	TxMessage.RTR=CAN_RTR_DATA;    // 数据帧
	TxMessage.DLC=len;        	 	// 数据长度

	for(i=0;i<len;i++)
		TxMessage.Data[i]=msg[i];  	// 写入数据
	mbox= CAN_Transmit(CAN1, &TxMessage);
	i=0;		
	while((CAN_TransmitStatus(CAN1, mbox)!= CAN_TxStatus_Ok )&&(i<0xfff))
		i++; //检测发送成功
	if(i>=0xfff)
		return 0;//发送失败
	return 1;	//发送成功
}



