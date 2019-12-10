#ifndef __I2C_H
#define __I2C_H

#include "sys.h"

/* 软件模拟 I2C 实现 */

/*延时函数宏定义 */
#define delay_us  Delay_us

/* I2C 引脚宏定义 */
#define RCC_I2C RCC_APB2Periph_GPIOB
#define I2C_SCL_PIN GPIO_Pin_6
#define I2C_SDA_PIN GPIO_Pin_7
#define I2C_PORT GPIOB

/* I2C IO操作函数 */
#define I2C_SCL     PBout(6) //SCL
#define I2C_SDA     PBout(7) //SDA
#define I2C_READ_SDA   	PBin(7) //SDA

/* I2C_SDA_PIN  IO 方向设置 */
#define I2C_SDA_IN()   {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=8<<28;} //上拉、下拉输入模式
#define I2C_SDA_OUT()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=3<<28;}//通用推挽输出模式

/* I2C 接口函数声明 */
void I2C_Configuration(void);   // I2C 初始化
void I2C_Start(void);                 //I2C 起始信号
void I2C_Stop(void);                 //I2C 停止信号
u8 I2C_Wait_Ack(void);           //等待应答信号到来
void I2C_ACK(void);               //产生ACK应答信号
void I2C_NAck(void);             //不产生ACK应答信号
void I2C_Send_Byte(u8 txd);  // I2C 发送一个字节
u8 I2C_Read_Byte(u8 ack);    // I2C 读取一个字节
#endif
