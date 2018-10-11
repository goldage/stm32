#ifndef __LED_H
#define __LED_H

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

void LED_GPIO_Config(void);
void LED_ON(void);
void LED_OFF(void);

#endif
