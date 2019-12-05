#include "sys.h"

/**********************************Global Variable***********************************************************/

/********************************************************************************************************/

/* Hardward Initial */
static void prvSetupHardware(void)
{
    /* SystemClock Configuration, Clock source uses external crystal oscillator(HSEs), 8*9 = 72MHz */
    SystemClock_HSE(9);

    /* Segger Configuration */
    SEGGER_RTT_Init();

    /* Systick Configuration */
    Delay_init();
    
    /* LED Configuration */
    LED_Configuration();
    
    /* SPI Configuration */
    W25QXX_Init();

}

int main(void)
{
    prvSetupHardware();

    printf("Hello W25Q128.\r\n");

    W25Qxx_ReadWrite_Test();
    
    while(1)
    {  
        LED_Test();
    }

}
