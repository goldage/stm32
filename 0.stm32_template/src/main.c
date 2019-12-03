/*************************************************************************************
* Test-program for Olimex “STM32-H103”, header board for “STM32F103RBT6”.
* After program start green LED (LED_E) will blink.
*
* Program has to be compiled with optimizer setting "-O0".
* Otherwise delay via while-loop will not work correctly.
*************************************************************************************/
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
int main(int argc, char *argv[])
{
 	GPIO_InitTypeDef GPIO_InitStructure;
/* GPIOA Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
/* Configure PA11 to mode: 50MHZ, pushpull output */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; // GPIO No. 11
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 50MHZ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // push-pull output
	GPIO_Init(GPIOA, &GPIO_InitStructure); // GPIOC init

/* GPIOA Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
/* Configure PA8 to mode: 50MHZ, pushpull output */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; // GPIO No. 8
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; // 50MHZ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // push-pull output
	GPIO_Init(GPIOA, &GPIO_InitStructure); // GPIOC init

	while(1)
	{
		//GPIO_SetBits(GPIOA,GPIO_Pin_8);
		//GPIO_ResetBits(GPIOA,GPIO_Pin_8);
		GPIO_SetBits(GPIOA,GPIO_Pin_11);
		//GPIO_ResetBits(GPIOA,GPIO_Pin_11);

	}
}
