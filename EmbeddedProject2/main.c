#include "main.h"



/*硬件初始化*/
void Hardware_Init(void)
{
	Power_Detection_Configuration();//电压检测配置初始化
	Led_Configuration();						//LED初始化
	OLED_Configuration();						//OLED显示屏初始化
	PWM_Configuration();						//PWM配置初始化
	USART2_Configuration();					//遥控器配置初始化
	USART3_Configuration();					//串口通信3初始化
	CAN1_Configuration();						//CAN1通信初始化,用于主控和底盘驱动电机通信
	CAN2_Configuration();						//CAN2通信初始化,用于主控和云台主控的通信
}


extern unsigned char Can1TxSuccessFlag;
extern  RC_Ctl_t RC_Ctl;
short int gyr_z;

u8 CAN1Msg1[2]={0,1};
u8 CAN1Msg2[2]={0,0};
u8 Can1Flag=0;	//CAN1 通信标志位成功为1，失败为0

u8 CAN2Msg1[2]={1};
u8 CAN2Msg2[2]={0};
u8 CAN1Flag=0;	//CAN2 通信标志位成功为1，失败为0

	

int main(void)
{   
		//硬件初始化
		int flag_fire=0;
		int flag_hit=0;
		int cont;
		int data1,data2,data3,data4;
		u8 v1[2],v2[2],v3[2],v4[2];
		short k=0;
		short a;
		short j,max_data;
		float left=0,right=0;
		
		arm_pid_instance_f32 s;
		
		Hardware_Init();
		TIM_SetCompare2(TIM1,1300);
		delay_ms(1000);
//		OLED_Print(0,0,"AD test");
		printf(" Robomasters\n");
		LED_GREEN_ON();
		
		s.Kp=0.02;
		s.Ki=0.0001;
		s.Kd=0.01;
		
		arm_pid_init_f32(&s,1);
		
		RC_Ctl.rc.ch0=1024;
		RC_Ctl.rc.ch1=1024;
		RC_Ctl.rc.ch2=1024;
		
		
		while(1)
		{
			printf("%d\n",gyr_z);
			
			a=(short)(1024-RC_Ctl.rc.ch2)*0.8;
	//		a=40;
				k-=arm_pid_f32(&s,gyr_z-a-2);
		//		(1024-RC_Ctl.rc.ch2)*0
				if(abs(k)>1000)
				{
					if(k<0)
						k=-1000;
					else
						k=1000; 
				}
			//	k=0;
				
//				data1=-k;
//				data2=k;
//				data3=-k;
//				data4=k;


				left=0.7*left+0.3*(RC_Ctl.rc.ch1-1024);
				right=0.7*right+0.3*(RC_Ctl.rc.ch0-1024);
				
//				left=(RC_Ctl.rc.ch1-1024);
//				right=(RC_Ctl.rc.ch0-1024);
				
				data1=(short)left+(short)right-k;
				data2=(short)left-(short)right+k;
				data3=(short)left-(short)right-k;
				data4=(short)left+(short)right+k;
				
				
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
					j=1980.0/abs(max_data);
				else
					j=3 ;
				data1=data1*j;
				data2=data2*j;
				data3=data3*j;
				data4=data4*j;

				//////////////////////////////把数据放入数组
				v1[0]=(data1)>>8;
				v1[1]=(data1)&0x00ff;

				v2[0]=(data2)>>8;
				v2[1]=(data2)&0x00ff;

				v3[0]=(data3)>>8;
				v3[1]=(data3)&0x00ff;

				v4[0]=(data4)>>8;
				v4[1]=(data4)&0x00ff;
				

				Can1SendMsg(v1,2,11);
				Can1SendMsg(v2,2,12);
				Can1SendMsg(v3,2,13);
				Can1SendMsg(v4,2,14);
				delay_ms(5);
		}


    while(1)
    {
				/*遥控器操作*/
				if(RC_Ctl.rc.s2==1)
				{
					if(RC_Ctl.rc.s1==1)	//上
					{
						TIM_SetCompare1(TIM1,0);			//拨轮不转
						TIM_SetCompare2(TIM1,1300);		//初始化电调
						flag_fire=0;
					}
					
					if(RC_Ctl.rc.s1==3)	//中
					{
						if(flag_fire==0)
						{
							TIM_SetCompare2(TIM1,1550);
							delay_ms(200);
							TIM_SetCompare2(TIM1,1300);
							delay_ms(100);
						}
						TIM_SetCompare2(TIM1,1550);
						flag_fire=1;
					}
					
					if(RC_Ctl.rc.s1==2)
					{
						delay_ms(20);
						if(RC_Ctl.rc.s1==2)	//下
						{	
						TIM_SetCompare1(TIM1,2000);
						delay_ms(8);
						TIM_SetCompare1(TIM1,0);
						delay_ms(200);
						}
					}
				}
				
			/*鼠标操作*/
			if(RC_Ctl.rc.s2==3)
			{
				if(RC_Ctl.mouse.press_r==1)	//按下右键
				{
					if(flag_fire==0)	//软启动电机
					{
						TIM_SetCompare2(TIM1,1550);
						delay_ms(200);
						TIM_SetCompare2(TIM1,1300);
						delay_ms(100);
					}
					TIM_SetCompare2(TIM1,1550);
					flag_fire=1;	//启动标志
					
					TIM_SetCompare1(TIM1,0);	//拨轮不转
					
					cont=0;  //给右键按下时间计时
					while(RC_Ctl.mouse.press_r == 1)	//持续按住右键
					{
						cont++;
						delay_ms(20);
						if(cont>25)	//按住右键2秒
						{
							TIM_SetCompare1(TIM1,0);	//拨轮不转
							TIM_SetCompare2(TIM1,1300);	//关闭电机
							flag_fire=0;	
						}						
					}					
				}
				
				if(RC_Ctl.mouse.press_l==1)	//按下左键
				{
					if(flag_fire==1)
					{
						TIM_SetCompare1(TIM1,2000);	//启动拨轮
						delay_ms(8);
						TIM_SetCompare1(TIM1,0);
						delay_ms(200);
					}
				}
			}
		}
}
 
 
 
 