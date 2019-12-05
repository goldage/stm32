#ifndef __LED_H
#define __LED_H

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "delay.h"

#define LED_PIN GPIO_Pin_1
#define LED_PORT GPIOB
#define RCC_LED RCC_APB2Periph_GPIOB //LED5

void LED_Configuration(void);
void LED_Off(void);
void LED_ON(void);
void LED_Test(void);

#endif
