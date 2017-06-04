#include "main.h"

//ADC123_IN2(PA2)
#include "stm32f4xx_adc.h"
#include "stm32f4xx_rcc.h"

void Power_Detection_Configuration(void)
{
		ADC_InitTypeDef adc;
		GPIO_InitTypeDef gpio;
    
		//使能ADC1和GPIO时钟信号
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
		gpio.GPIO_Pin = GPIO_Pin_2;
		gpio.GPIO_Mode = GPIO_Mode_AN;								/*!< GPIO Analog Mode 模拟量输入模式*/
		gpio.GPIO_OType = GPIO_OType_PP;							//通用推挽输出模式	
		gpio.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_Init(GPIOA,&gpio);
    
    adc.ADC_Resolution = ADC_Resolution_12b;			//AD采样精度，有6位8位10位12位这里使用10位采样分辨率
																									//由于本次test为电压计故需要高精度的分辨率
    adc.ADC_ScanConvMode = DISABLE;								//禁止扫描模式，扫描模式用于多通道采集，这里是单通道采集				
    adc.ADC_ContinuousConvMode = ENABLE;					// 开启连续转换模式，只需要在需要的时候开启转换即可
    adc.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;	//不使用外部中断触发转换
    adc.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;				//使用外部分频时钟触发
    adc.ADC_DataAlign = ADC_DataAlign_Right;			//数据采用右对齐的方式
    adc.ADC_NbrOfConversion = 1;									//产生触发以后转换次数的执行次数
    ADC_Init(ADC1,&adc);
    
    ADC_Cmd(ADC1,ENABLE);													//使能ADC1
    
    ADC_RegularChannelConfig(ADC1,ADC_Channel_2,1,ADC_SampleTime_56Cycles);//配置ADC1的通道2为56个采样周期
		
}

//电源电压检测
void Power_Detect(void)
{
    unsigned int AD_Value;
		float Volt=0;
		char AD_Char[5];
		const unsigned char IntCovIntoStr[10]={48,49,50,51,52,53,54,55,56,57};

    ADC_SoftwareStartConv(ADC1);
	
		//等待完成转换
    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)==RESET);//ADC_FLAG_EOC: End of conversion flag　that　means　weather　END OF this

    AD_Value = ADC_GetConversionValue(ADC1);
		SpeedControl(AD_Value);
		Volt=(3.3/4095.0*AD_Value);
		//printf("\nThe volt is %f",Volt);
		
		
		//显示当前电压值
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
