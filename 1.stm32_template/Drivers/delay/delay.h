#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f10x.h"
#include "misc.h"

void SysTick_Init(void);
void Delay_ms(__IO u32 nTime);
void Delay_us(__IO u32 nTime);

#endif
