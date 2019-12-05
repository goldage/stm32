#ifndef __SPI_H
#define __SPI_H

#include "sys.h"

#define SPI_PORT        GPIOB
#define SPIX            SPI2
#define SPI_MISO_PIN    GPIO_Pin_14
#define SPI_MOSI_PIN    GPIO_Pin_15
#define SPI_CLK_PIN     GPIO_Pin_13
#define SPI_CS_PIN      GPIO_Pin_12

void SPIx_Init(void);
u8 SPIx_ReadWriteByte(u8 TxData);

#endif
