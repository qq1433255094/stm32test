#ifndef __LED_H__
#define __LED_H__

#include "stm32f4xx_gpio.h"

void Led_Configuration(void);
void SystemState(char state);

#define  LED_GREEN_ON()      GPIO_ResetBits(GPIOA, GPIO_Pin_6)
#define  LED_GREEN_OFF()     GPIO_SetBits(GPIOA, GPIO_Pin_6)
#define  LED_GREEN_TOGGLE()  GPIO_ToggleBits(GPIOA, GPIO_Pin_6)

#define  LED_RED_ON()        GPIO_ResetBits(GPIOA, GPIO_Pin_7)
#define  LED_RED_OFF()       GPIO_SetBits(GPIOA, GPIO_Pin_7)
#define  LED_RED_TOGGLE()    GPIO_ToggleBits(GPIOA, GPIO_Pin_7)

#define  	NORMOL 1
#define 	UNNORMOL 0


#endif
