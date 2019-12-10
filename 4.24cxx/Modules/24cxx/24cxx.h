#ifndef __24CXX_H
#define __24CXX_H

#include "sys.h"

/* 延时函数宏定义 */
#define delay_ms  Delay_ms

/* 24CXX 型号 */
#define AT24C01      127
#define AT24C02      255
#define AT24C04      511
#define AT24C08      1023
#define AT24C16      2047
#define AT24C32      4095
#define AT24C64      8191
#define AT24C128    16383
#define AT24C256    32767

#define EE_TYPE AT24C02

/* 24cxx 接口函数声明 */
void ATMEL_24CXX_Init(void);  //24cxx i2c 接口初始化
u8 ATMEL_24CXX_Check(void); //检查AT24CXX是否正常
u8 ATMEL_24CXX_ReadOneByte(u16 ReadAddr); //在AT24CXX指定地址读出一个数据
void ATMEL_24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite); //向AT24CXX指定地址写入一个数据
void ATMEL_24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len); //在AT24CXX里面的指定地址开始写入长度为Len的数据
void ATMEL_24CXX_ReadLenByte(u16 ReadAddr, u8 Len); //在AT24CXX里面的指定地址开始读出长度为Len的数据
void ATMEL_24CXX_Read(u16 ReadAddr, u8 *pBuffer, u16 NumToRead); //在AT24CXX里面的指定地址开始读出指定个数的数据
void ATMEL_24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite); //在AT24CXX里面的指定地址开始写入指定个数的数据
#endif
