#include "main.h"

/*----CAN1_TX-----PA12----*/
/*----CAN1_RX-----PA11----*/

void CAN1_Configuration(void)
{
    CAN_InitTypeDef        can;					//
    CAN_FilterInitTypeDef  can_filter;
    GPIO_InitTypeDef       gpio;
    NVIC_InitTypeDef       nvic;

		//����ʱ������
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
    
    can.CAN_TTCM = DISABLE;		//����CAN��ʱ�䴥��ͨ��ģʽ
    can.CAN_ABOM = DISABLE;		//��CAN��⵽���ʹ������մ���ʱһ��ֵʱ�Զ���������״̬
    can.CAN_AWUM = DISABLE;		//�Ƿ����Զ����ѹ���
    can.CAN_NART = DISABLE;		//�Ƿ��ֹ�����Զ��ش�
    can.CAN_RFLM = DISABLE;		//���ý���FIFO�Ƿ���������ʹ����FIFO���ʱ������һ�����յı��ģ��������Ǹ���ԭ����
    can.CAN_TXFP = ENABLE;		//���ձ��ĵ�����˳�����������ķ������ȼ���˳��
    can.CAN_Mode = CAN_Mode_Normal;//��������ģʽ
    can.CAN_SJW  = CAN_SJW_1tq;//����ͬ����Ծ���
    can.CAN_BS1 = CAN_BS1_9tq;	//��λ�����1
    can.CAN_BS2 = CAN_BS2_4tq;	//��λ�����2
    can.CAN_Prescaler = 3;   		//CAN BaudRate 42/(1+9+4)/3=1Mbps//3��3.24
    CAN_Init(CAN1, &can);

		can_filter.CAN_FilterNumber=0;//��������0
		can_filter.CAN_FilterMode=CAN_FilterMode_IdMask;//��ʶ������ģʽ
		can_filter.CAN_FilterScale=CAN_FilterScale_32bit;//32λģʽ
		can_filter.CAN_FilterIdHigh=0x0000;
		can_filter.CAN_FilterIdLow=0x0000;
		can_filter.CAN_FilterMaskIdHigh=0x0000;
		can_filter.CAN_FilterMaskIdLow=0x0000;
		can_filter.CAN_FilterFIFOAssignment=0;//�洢λ��Ϊ0
		can_filter.CAN_FilterActivation=ENABLE;//ʹ�ܹ�����
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
	TxMessage.StdId=(id<<3);      // ��׼��ʶ��
	TxMessage.ExtId=0;      			//��չ��ʶ��
	TxMessage.IDE=CAN_ID_STD;     // ѡ���׼��ʶ��
	TxMessage.RTR=CAN_RTR_DATA;    // ����֡
	TxMessage.DLC=len;        	 	// ���ݳ���

	for(i=0;i<len;i++)
		TxMessage.Data[i]=msg[i];  	// д������
	mbox= CAN_Transmit(CAN1, &TxMessage);
	i=0;		
	while((CAN_TransmitStatus(CAN1, mbox)!= CAN_TxStatus_Ok )&&(i<0xfff))
		i++; //��ⷢ�ͳɹ�
	if(i>=0xfff)
		return 0;//����ʧ��
	return 1;	//���ͳɹ�
}



