#include "sys.h"

/**********************************Global Variable***********************************************************/

/*************************************************************************************************************/

/* Hardward Initial */
static void prvSetupHardware(void)
{
    /* SystemClock Configuration, Clock source uses external crystal oscillator(HSEs), 8*9 = 72MHz */
    SystemClock_HSE(9);
    /* LED Configuration */
    LED_Configuration();
    /* Systick Configuration */
    SysTick_Init();
    /* Segger Configuration */
    SEGGER_RTT_Init();

}

int main(void)
{
    prvSetupHardware();

    while(1)
    {
        LED_Test();
    }

}
