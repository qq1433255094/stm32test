#include "main.h"

/*-----USART2_RX-----PA3----*/ 
//for D-BUS

unsigned char sbus_rx_buffer[18];
int count=0,data1,data2,data3,data4,data[4],max_data;
u8 v1[2],v2[2],v3[2],v4[2],fire[2];
float k=3;
extern char Rx_data[8];
struct
{
	u8 w;
	u8 s;
	u8 a;
	u8 d;
	u8 q;
	u8 e;
	u8 shift;
	u8 ctrl;
}key;

struct
{
	int x;
	int y;
}mouse;

struct
{
	int x;
	int y;
	int p;
	int up;
	int vul_x;
	int vul_y;
}move={0,0,0,0,0,0};
		
void USART2_Configuration(void)
{
    USART_InitTypeDef usart2;
		GPIO_InitTypeDef  gpio;
    NVIC_InitTypeDef  nvic;
    DMA_InitTypeDef   dma;
	
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_DMA1,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource3 ,GPIO_AF_USART2);
	
		gpio.GPIO_Pin = GPIO_Pin_3 ;
		gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOA,&gpio);
    
    USART_DeInit(USART2);
		usart2.USART_BaudRate = 100000;   //SBUS 100K baudrate
		usart2.USART_WordLength = USART_WordLength_8b;
		usart2.USART_StopBits = USART_StopBits_1;
		usart2.USART_Parity = USART_Parity_Even;
		usart2.USART_Mode = USART_Mode_Rx;
    usart2.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_Init(USART2,&usart2);
    
		USART_Cmd(USART2,ENABLE);
    USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);
    
    nvic.NVIC_IRQChannel = DMA1_Stream5_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
    
    DMA_DeInit(DMA1_Stream5);
    dma.DMA_Channel= DMA_Channel_4;												//设置DMA通道
    dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->DR);	//设置DMA源：内存地址&串口数据寄存器地址
    dma.DMA_Memory0BaseAddr = (uint32_t)sbus_rx_buffer;		//内存地址（要传输变量的地址）
    dma.DMA_DIR = DMA_DIR_PeripheralToMemory;							//传输方向：从外设到内存
    dma.DMA_BufferSize = 18;															//传输大小
    dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//外设地址不增
    dma.DMA_MemoryInc = DMA_MemoryInc_Enable;							//内存地址自增
    dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据单位
    dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			//内存数据单位 8Bit
    dma.DMA_Mode = DMA_Mode_Circular;											//循环传输模式
    dma.DMA_Priority = DMA_Priority_VeryHigh;							//传输优先级：高
    dma.DMA_FIFOMode = DMA_FIFOMode_Disable;							//不使能FIFO模式
    dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
    dma.DMA_MemoryBurst = DMA_Mode_Normal;
    dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DMA1_Stream5,&dma);

    DMA_ITConfig(DMA1_Stream5,DMA_IT_TC,ENABLE);
    DMA_Cmd(DMA1_Stream5,ENABLE);
}

RC_Ctl_t RC_Ctl;
void DMA1_Stream5_IRQHandler(void)
{		
    if(DMA_GetITStatus(DMA1_Stream5, DMA_IT_TCIF5))
    {
        DMA_ClearFlag(DMA1_Stream5, DMA_FLAG_TCIF5);
        DMA_ClearITPendingBit(DMA1_Stream5, DMA_IT_TCIF5);
		
				RC_Ctl.rc.ch0 = (sbus_rx_buffer[0]| (sbus_rx_buffer[1] << 8)) & 0x07ff; //!< Channel 0	 
				RC_Ctl.rc.ch1 = ((sbus_rx_buffer[1] >> 3) | (sbus_rx_buffer[2] << 5)) & 0x07ff; //!< Channel 1
				RC_Ctl.rc.ch2 = ((sbus_rx_buffer[2] >> 6) | (sbus_rx_buffer[3] << 2) |(sbus_rx_buffer[4] << 10)) & 0x07ff; //!< Channel 2  
				RC_Ctl.rc.ch3 = ((sbus_rx_buffer[4] >> 1) | (sbus_rx_buffer[5] << 7)) & 0x07ff; //!< Channel 3
				RC_Ctl.rc.s1  = ((sbus_rx_buffer[5] >> 4)& 0x000C) >> 2; //!< Switch left
				RC_Ctl.rc.s2  = ((sbus_rx_buffer[5] >> 4)& 0x0003); 	//!< Switch right								
				RC_Ctl.mouse.x       = sbus_rx_buffer[6] | (sbus_rx_buffer[7] << 8); //!< Mouse X axis
				RC_Ctl.mouse.y       = sbus_rx_buffer[8] | (sbus_rx_buffer[9] << 8); //!< Mouse Y axis
				RC_Ctl.mouse.z       = sbus_rx_buffer[10] | (sbus_rx_buffer[11] << 8); //!< Mouse Z axis
				RC_Ctl.mouse.press_l = sbus_rx_buffer[12]; //!< Mouse Left Is Press 
				RC_Ctl.mouse.press_r = sbus_rx_buffer[13]; //!< Mouse Right Is Press 
				RC_Ctl.key.v         = sbus_rx_buffer[14] | (sbus_rx_buffer[15]<<8);
				
				key.w     = (RC_Ctl.key.v & 1   )>>0;
				key.s     = (RC_Ctl.key.v & 2   )>>1;
				key.a     = (RC_Ctl.key.v & 4   )>>2;
				key.d     = (RC_Ctl.key.v & 8   )>>3;
				key.q     = (RC_Ctl.key.v & 64  )>>6;
				key.e     = (RC_Ctl.key.v & 128 )>>7;
				key.shift = (RC_Ctl.key.v & 16  )>>4;
				key.ctrl  = (RC_Ctl.key.v & 32  )>>5;

					if(RC_Ctl.mouse.x > 32768)
						mouse.x = RC_Ctl.mouse.x-65536;
					else
						mouse.x = RC_Ctl.mouse.x;
					if(RC_Ctl.mouse.y > 32768)
						mouse.y = RC_Ctl.mouse.y-65536;
					else
						mouse.y = RC_Ctl.mouse.y;					
				///////////////////////////////////遥控器控制
					if(RC_Ctl.rc.s2==1)
					{
						data1=(RC_Ctl.rc.ch1-1024)+(RC_Ctl.rc.ch0-1024)-(1024*0.6-RC_Ctl.rc.ch2*0.6);
						data2=(RC_Ctl.rc.ch1-1024)-(RC_Ctl.rc.ch0-1024)+(1024*0.6-RC_Ctl.rc.ch2*0.6);
						data3=(RC_Ctl.rc.ch1-1024)-(RC_Ctl.rc.ch0-1024)-(1024*0.6-RC_Ctl.rc.ch2*0.6);
						data4=(RC_Ctl.rc.ch1-1024)+(RC_Ctl.rc.ch0-1024)+(1024*0.6-RC_Ctl.rc.ch2*0.6);
						
						move.up=RC_Ctl.rc.ch3;
					}
	
				////////////////////////////////////键盘控制
				  if(RC_Ctl.rc.s2==3)
					{
						move.p = 0;
						if(key.w==0 && key.s==0)
						{
							if(move.y>0)
								move.y-=10;
							if(move.y<0)
								move.y+=10;
						}
						
						if(key.w==1 && key.s==0)
						{
							if(move.y<300)
							move.y+=10;
						}
						
						
						if(key.w==0 && key.s==1)
						{
							if(move.y>-300)
								move.y-=10;
						}
						
						if(key.a==0 && key.d==0)
						{
							if(move.x>0)
								move.x-=10;
							if(move.x<0)
								move.x+=10;
						}
						
						if(key.a==1 && key.d==0)
						{
							if(move.x>-300)
								move.x-=10;;
						}
						
						
						if(key.a==0 && key.d==1)
						{
							if(move.x<300)
							move.x+=10;
						}

						move.p = -mouse.x*7;
						if(key.shift==1)
						{
						data1=2*move.y + 2*move.x - move.p;
						data2=2*move.y - 2*move.x + move.p;
						data3=2*move.y - 2*move.x - move.p;
						data4=2*move.y + 2*move.x + move.p;
						}
						else
						{
						data1=move.y + move.x - move.p;
						data2=move.y - move.x + move.p;
						data3=move.y - move.x - move.p;
						data4=move.y + move.x + move.p;
						}
						move.up = 1024 - mouse.y*5;
					}
					
				////////////////////////////////////速度乘3
				if(abs(data1)>=abs(data2) && abs(data1)>=abs(data3) && abs(data1)>=abs(data4))
					max_data=data1;
				else
					if(abs(data2)>=abs(data3) && abs(data2)>=abs(data4))
						max_data=data2;
					else
						if(abs(data3)>= abs(data4))
							max_data=data3;
						else
							max_data=data4;
				if(abs(max_data)>660)
					k=1980.0/abs(max_data);
				else
					k=3 ;
				data1=data1*k;
				data2=data2*k;
				data3=data3*k;
				data4=data4*k;

				//////////////////////////////把数据放入数组
				v1[0]=(data1)>>8;
				v1[1]=(data1)&0x00ff;

				v2[0]=(data2)>>8;
				v2[1]=(data2)&0x00ff;

				v3[0]=(data3)>>8;
				v3[1]=(data3)&0x00ff;

				v4[0]=(data4)>>8;
				v4[1]=(data4)&0x00ff;
				
				fire[0]=move.up>>8;
				fire[1]=move.up&0x00ff;
				
				//////////////////////////////////发送数据
				if(RC_Ctl.rc.s2!=2) 
				{
//					Can1SendMsg(v1,2,1);
//					Can1SendMsg(v2,2,2);
//					Can1SendMsg(v3,2,3);
//					Can1SendMsg(v4,2,4);
//					Can2SendMsg(fire,2,5);
				}
				else
				{
					;
				}
				
				//////////////////////////////////
			count++;
			if(count>30)
			{
				count=0;
//			printf("  data1:%d\t",data1);
//			printf("data2:%d\t",data2);
//			printf("data3:%d\t",data3);
//			printf("data4:%d\n",data4);
//			printf("RC_Ctl.mouse.press_l :%d\n",RC_Ctl.mouse.press_l);
//			printf("RC_Ctl.mouse.press_r :%d\n",RC_Ctl.mouse.press_r);
//			printf("RC_Ctl.mouse.x :%d\n",mouse.x);
//			printf("RC_Ctl.mouse.y :%d\n",mouse.y);
//			printf("key.w:%d\n",key.w);
//			printf("key.s:%d\n",key.s);
//			printf("key.a:%d\n",key.a);
//			printf("key.d:%d\n",key.d);
//			printf("key.q:%d\n",key.q);
//			printf("key.e:%d\n",key.e);
//			printf("key.shift:%d\n",key.shift);
//			printf("key.ctrl:%d\n",key.ctrl);
			}
    }
}
