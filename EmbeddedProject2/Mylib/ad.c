#include "main.h"

//ADC123_IN2(PA2)
#include "stm32f4xx_adc.h"
#include "stm32f4xx_rcc.h"

void Power_Detection_Configuration(void)
{
		ADC_InitTypeDef adc;
		GPIO_InitTypeDef gpio;
    
		//ʹ��ADC1��GPIOʱ���ź�
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
		gpio.GPIO_Pin = GPIO_Pin_2;
		gpio.GPIO_Mode = GPIO_Mode_AN;								/*!< GPIO Analog Mode ģ��������ģʽ*/
		gpio.GPIO_OType = GPIO_OType_PP;							//ͨ���������ģʽ	
		gpio.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_Init(GPIOA,&gpio);
    
    adc.ADC_Resolution = ADC_Resolution_12b;			//AD�������ȣ���6λ8λ10λ12λ����ʹ��10λ�����ֱ���
																									//���ڱ���testΪ��ѹ�ƹ���Ҫ�߾��ȵķֱ���
    adc.ADC_ScanConvMode = DISABLE;								//��ֹɨ��ģʽ��ɨ��ģʽ���ڶ�ͨ���ɼ��������ǵ�ͨ���ɼ�				
    adc.ADC_ContinuousConvMode = ENABLE;					// ��������ת��ģʽ��ֻ��Ҫ����Ҫ��ʱ����ת������
    adc.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;	//��ʹ���ⲿ�жϴ���ת��
    adc.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;				//ʹ���ⲿ��Ƶʱ�Ӵ���
    adc.ADC_DataAlign = ADC_DataAlign_Right;			//���ݲ����Ҷ���ķ�ʽ
    adc.ADC_NbrOfConversion = 1;									//���������Ժ�ת��������ִ�д���
    ADC_Init(ADC1,&adc);
    
    ADC_Cmd(ADC1,ENABLE);													//ʹ��ADC1
    
    ADC_RegularChannelConfig(ADC1,ADC_Channel_2,1,ADC_SampleTime_56Cycles);//����ADC1��ͨ��2Ϊ56����������
		
}

//��Դ��ѹ���
void Power_Detect(void)
{
    unsigned int AD_Value;
		float Volt=0;
		char AD_Char[5];
		const unsigned char IntCovIntoStr[10]={48,49,50,51,52,53,54,55,56,57};

    ADC_SoftwareStartConv(ADC1);
	
		//�ȴ����ת��
    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)==RESET);//ADC_FLAG_EOC: End of conversion flag��that��means��weather��END OF this

    AD_Value = ADC_GetConversionValue(ADC1);
		SpeedControl(AD_Value);
		Volt=(3.3/4095.0*AD_Value);
		//printf("\nThe volt is %f",Volt);
		
		
		//��ʾ��ǰ��ѹֵ
		AD_Char[0]=IntCovIntoStr[(int)Volt/1000];
		AD_Char[1]=IntCovIntoStr[(int)Volt%1000/100];
		AD_Char[2]=IntCovIntoStr[(int)Volt%100/10];
		AD_Char[3]=IntCovIntoStr[(int)Volt%10];
		AD_Char[4]='\0';
		

		
		OLED_Print(0,2,AD_Char);
	
    if(AD_Value < 3000)
    {
        SystemState(NORMOL);
    }
		if(AD_Value > 3000)
    {
        SystemState(UNNORMOL);
    }
}
