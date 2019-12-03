#include "led.h"

void LED_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_LED,ENABLE);

  GPIO_InitStructure.GPIO_Pin = LED_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(LED_PORT,  &GPIO_InitStructure);
  GPIO_ResetBits(LED_PORT,LED_PIN);
}

void LED_Off(void)
{
    GPIO_SetBits(LED_PORT,LED_PIN);
}

void LED_ON(void)
{
    GPIO_ResetBits(LED_PORT,LED_PIN);
}

void LED_Test(void)
{
  LED_ON();
  Delay_ms(100);
  LED_Off();
  Delay_ms(100);
}
