#include "main.h"



/*Ӳ����ʼ��*/
void Hardware_Init(void)
{
	Power_Detection_Configuration();//��ѹ������ó�ʼ��
	Led_Configuration();						//LED��ʼ��
	OLED_Configuration();						//OLED��ʾ����ʼ��
	PWM_Configuration();						//PWM���ó�ʼ��
	USART2_Configuration();					//ң�������ó�ʼ��
	USART3_Configuration();					//����ͨ��3��ʼ��
	CAN1_Configuration();						//CAN1ͨ�ų�ʼ��,�������غ͵����������ͨ��
	CAN2_Configuration();						//CAN2ͨ�ų�ʼ��,�������غ���̨���ص�ͨ��
}


extern unsigned char Can1TxSuccessFlag;
extern  RC_Ctl_t RC_Ctl;
short int gyr_z;

u8 CAN1Msg1[2]={0,1};
u8 CAN1Msg2[2]={0,0};
u8 Can1Flag=0;	//CAN1 ͨ�ű�־λ�ɹ�Ϊ1��ʧ��Ϊ0

u8 CAN2Msg1[2]={1};
u8 CAN2Msg2[2]={0};
u8 CAN1Flag=0;	//CAN2 ͨ�ű�־λ�ɹ�Ϊ1��ʧ��Ϊ0

	

int main(void)
{   
		//Ӳ����ʼ��
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
				
				
				////////////////////////////////////�ٶȳ�3
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

				//////////////////////////////�����ݷ�������
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
				/*ң��������*/
				if(RC_Ctl.rc.s2==1)
				{
					if(RC_Ctl.rc.s1==1)	//��
					{
						TIM_SetCompare1(TIM1,0);			//���ֲ�ת
						TIM_SetCompare2(TIM1,1300);		//��ʼ�����
						flag_fire=0;
					}
					
					if(RC_Ctl.rc.s1==3)	//��
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
						if(RC_Ctl.rc.s1==2)	//��
						{	
						TIM_SetCompare1(TIM1,2000);
						delay_ms(8);
						TIM_SetCompare1(TIM1,0);
						delay_ms(200);
						}
					}
				}
				
			/*������*/
			if(RC_Ctl.rc.s2==3)
			{
				if(RC_Ctl.mouse.press_r==1)	//�����Ҽ�
				{
					if(flag_fire==0)	//���������
					{
						TIM_SetCompare2(TIM1,1550);
						delay_ms(200);
						TIM_SetCompare2(TIM1,1300);
						delay_ms(100);
					}
					TIM_SetCompare2(TIM1,1550);
					flag_fire=1;	//������־
					
					TIM_SetCompare1(TIM1,0);	//���ֲ�ת
					
					cont=0;  //���Ҽ�����ʱ���ʱ
					while(RC_Ctl.mouse.press_r == 1)	//������ס�Ҽ�
					{
						cont++;
						delay_ms(20);
						if(cont>25)	//��ס�Ҽ�2��
						{
							TIM_SetCompare1(TIM1,0);	//���ֲ�ת
							TIM_SetCompare2(TIM1,1300);	//�رյ��
							flag_fire=0;	
						}						
					}					
				}
				
				if(RC_Ctl.mouse.press_l==1)	//�������
				{
					if(flag_fire==1)
					{
						TIM_SetCompare1(TIM1,2000);	//��������
						delay_ms(8);
						TIM_SetCompare1(TIM1,0);
						delay_ms(200);
					}
				}
			}
		}
}
 
 
 
 