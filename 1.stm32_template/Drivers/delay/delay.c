#include "delay.h"

u32 TimingDelay;

/*******************************************************************************
* Function    : SysTick_ITConfig.
* Description : Laurence add for enable or disable systick clk irq.
* Arguments   : NewState: ENABLE / DISABLE
* Return      : None
*******************************************************************************/
void SysTick_ITConfig(FunctionalState NewState)
{
    if (NewState != DISABLE)
    {
        SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    }
    else
    {
        SysTick->CTRL &= (~SysTick_CTRL_ENABLE_Msk);
    }
}

/*******************************************************************************
* Function    : SysTick_Init.
* Description : 启动系统滴答定时器 SysTick.
                SystemFrequency / 1000    1ms中断一次
                SystemFrequency / 100000  10us中断一次
                SystemFrequency / 1000000 1us中断一次
* Arguments   : None
* Return      : None
*******************************************************************************/
void SysTick_Init(void)
{
    RCC_ClocksTypeDef RCC_Clocks;

    /* SysTick end of count event each 1ms */
    RCC_GetClocksFreq(&RCC_Clocks);

    SysTick_Config(RCC_Clocks.HCLK_Frequency  / 100000);// 配置定时器进入中断的时间

    SysTick_ITConfig(ENABLE);
}

/*******************************************************************************
* Function    : Delay_us.
* Description : us延时程序,10us为一个单位.
* Arguments   : nTime: Delay_us( 1 ) 则实现的延时为 1 * 10us = 10us
* Return      : None
*******************************************************************************/
void Delay_us(__IO u32 nTime)
{
    TimingDelay = nTime; // 配置t

    while (TimingDelay != 0);
}

void Delay_ms(__IO u32 nTime)
{
    Delay_us(nTime * 100); // 100 * 10 us = 1ms
}

/*******************************************************************************
* Function    : TimingDelay_Decrement.
* Description : 获取节拍程序.
                在 SysTick 中断函数 SysTick_Handler()调用
* Arguments   : None
* Return      : None
*******************************************************************************/
void TimingDelay_Decrement(void)
{
    if (TimingDelay != 0x00)
    {
        TimingDelay--;
    }
}

void SysTick_Handler(void)
{
    TimingDelay_Decrement();
}
