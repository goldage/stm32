#ifndef __LED_H
#define __LED_H

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "delay.h"

#define LED_PIN GPIO_Pin_14
#define LED_PORT GPIOC
#define RCC_LED RCC_APB2Periph_GPIOC

void LED_Configuration(void);
void LED_Off(void);
void LED_ON(void);
void LED_Test(void);

#endif
