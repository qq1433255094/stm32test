#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>

void Delay()
{
	int i;
	for (i = 0; i < 1000000; i++)
		asm("nop");
}

int main()
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  for (;;)
  {
	  GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_SET);
	  Delay();
	  GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_RESET);
	  Delay();
  }
}
