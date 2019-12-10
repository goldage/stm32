#include "sys.h"

/**********************************Global Variable***********************************************************/
//要写入到24c02的字符串数组
const u8 TEXT_Buffer[]={"Laurence IIC  EEPROM TEST."};
#define SIZE sizeof(TEXT_Buffer)
/*************************************************************************************************************/

/* Hardward Initial */
static void prvSetupHardware(void)
{
    /* SystemClock Configuration, Clock source uses external crystal oscillator(HSEs), 8*9 = 72MHz */
    SystemClock_HSE(9);
    /* LED Configuration */
    LED_Configuration();
    /* Systick Configuration */
    Delay_init();
    /* Segger Configuration */
    SEGGER_RTT_Init();
    /* I2C Configuration*/
    //I2C_Configuration();
    /* ATMEL 24C02 Configuration */
    ATMEL_24CXX_Init();
}

int main(void)
{
    u8 datatemp[SIZE];
    u8 temp;

    prvSetupHardware();

    while(ATMEL_24CXX_Check())
    {
        LED_Test();
    }
    while(1)
    {
        printf("Start writing 24C02.\r\n");
        ATMEL_24CXX_Write(0, (u8*)TEXT_Buffer, SIZE);
        printf("Write done.\r\n");

        Delay_ms(50);

        printf("Start reading 24C02.\r\n");
        ATMEL_24CXX_Read(0,datatemp, SIZE);
        printf("Read done\r\n");
        printf("\r\n%s\r\n",datatemp);

        temp = ATMEL_24CXX_ReadOneByte(255);
        printf("Addr 255 = %02x\r\n",temp);
        LED_Test();
    }
}
