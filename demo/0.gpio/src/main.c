/*************************************************************************************
* Test-program for Olimex “STM32-H103”, header board for “STM32F103RBT6”.
* After program start green LED (LED_E) will blink.
*
* Program has to be compiled with optimizer setting "-O0".
* Otherwise delay via while-loop will not work correctly.
*************************************************************************************/
#include "stm32f10x.h"
#include "led.h"

int main(int argc, char *argv[])
{

    LED_GPIO_Config();

	while(1)
	{
        LED_ON();
	}
}
