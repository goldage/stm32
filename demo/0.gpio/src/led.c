#include "led.h"

void LED_GPIO_Config(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
    /* GPIOA Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    /* Configure PA11 to mode: 50MHZ, pushpull output */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; // GPIO No. 11
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 50MHZ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // push-pull output
	GPIO_Init(GPIOA, &GPIO_InitStructure); // GPIOC init

    /* Configure PA8 to mode: 50MHZ, pushpull output */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; // GPIO No. 8
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 50MHZ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // push-pull output
	GPIO_Init(GPIOA, &GPIO_InitStructure); // GPIOC init
}

void LED_ON(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_11);
	//GPIO_SetBits(GPIOA,GPIO_Pin_8);
}

void LED_OFF(void)
{
    GPIO_ResetBits(GPIOA,GPIO_Pin_11);
	//GPIO_SetBits(GPIOA,GPIO_Pin_8);
}

