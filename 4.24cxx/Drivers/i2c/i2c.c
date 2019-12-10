#include "i2c.h"

/* I2C 延时函数 */
static void I2C_Delay(void)
{
    delay_us(2);
}

/* I2C 初始化 */
void I2C_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_I2C , ENABLE); //使能I2C Port端口时钟

    GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN | I2C_SDA_PIN;    //端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;               //50MHz
    GPIO_Init(I2C_PORT, &GPIO_InitStructure);                                  //根据设定参数初始化I2C Port
}

/*******************************************************************************
* Function      : I2C_Start.
* Description :  I2C 起始信号
                    SCL 为高电平时，主机将 SDA 拉低，表示数据传输即将开始。
* Arguments   : None
* Return         : None
*******************************************************************************/
void I2C_Start(void)
{
    I2C_SDA_OUT();//sda线设置为输出
    I2C_SDA = 1;
    I2C_SCL = 1;
    I2C_Delay();
    I2C_SDA = 0; //START:when CLK is high,DATA change form high to low
    I2C_Delay();
    I2C_SCL = 1;//钳住I2C总线，准备发送或接收数据
}

/*******************************************************************************
* Function      : I2C_Stop.
* Description :  I2C 停止信号
                   在 SDA 为低电平时，主机将 SCL 拉高并保持高电平，然后在将 SDA 拉高，
                   表示传输结束。
* Arguments   : None
* Return         : None
*******************************************************************************/
void I2C_Stop(void)
{
    I2C_SDA_OUT();//sda线设置为输出
    I2C_SCL = 0;
    I2C_SDA = 0;//STOP:when CLK is high DATA change form low to high
    I2C_Delay();
    I2C_SCL = 1;
    I2C_SDA = 1;//发送I2C总线结束信号
    I2C_Delay();
}

/*******************************************************************************
* Function      : I2C_Wait_Ack.
* Description : 等待应答信号到来
* Arguments   : None
* Return         :
                1：接收应答失败
                0：接收应答成功
*******************************************************************************/
u8 I2C_Wait_Ack(void)
{
    u8 ucErrTime = 0;

    I2C_SDA_IN(); /* SDA is set to input */

    I2C_SDA = 1; I2C_Delay();
    I2C_SCL = 1; I2C_Delay();

    while(I2C_READ_SDA) /* SDA is high, waiting for the IIC device to pull low */
    {
        ucErrTime++;
        if(ucErrTime > 250) /* 40*250=1ms did not reply, IIC master send a stop signal */
        {
            I2C_Stop();
            return 1;
        }
    }
    I2C_SCL = 0;//时钟输出0
    return 0;
}

/*******************************************************************************
* Function      : I2C_ACK.
* Description : 产生ACK应答信号
* Arguments   : None
* Return         : None
*******************************************************************************/
void I2C_ACK(void)
{
    I2C_SCL = 0;
    I2C_SDA_OUT();
    I2C_SDA= 0;//ACK：低电平脉冲
    I2C_Delay();
    I2C_SCL = 1;
    I2C_Delay();
    I2C_SCL = 0;
}

/*******************************************************************************
* Function      : I2C_NAck.
* Description : 不产生ACK应答信号
* Arguments   : None
* Return         : None
*******************************************************************************/
void I2C_NAck(void)
{
    I2C_SCL = 0;
    I2C_SDA_OUT();
    I2C_SDA = 1;//nACK
    I2C_Delay();
    I2C_SCL = 1;
    I2C_Delay();
    I2C_SCL = 0;
}

/*******************************************************************************
* Function      : I2C_Send_Byte.
* Description : I2C 发送一个字节
* Arguments   : txd : 要发送的字节
* Return         : None
*******************************************************************************/
void I2C_Send_Byte(u8 txd)
{
    u8 t;
    I2C_SDA_OUT();

    /* The High or Low state of the data line can only change whe the clock signal on the SCL line is LOW */
    I2C_SCL = 0; //拉低时钟开始数据传输

    for(t = 0 ; t < 8; t++)
    {
        I2C_SDA = (txd & 0x80)>>7; /* MSB */
        txd <<= 1;
        I2C_SCL = 1;
        I2C_Delay();
        I2C_SCL = 0;
        I2C_Delay();
    }
    //IIC_Wait_Ack();
}

/*******************************************************************************
* Function      : I2C_Read_Byte.
* Description : I2C 读取一个字节
* Arguments   :
                    ack=1：发送ACK
                    ack=0：发送nACK
* Return         :读取到的字节
*******************************************************************************/
u8 I2C_Read_Byte(u8 ack)
{
    u8 i, receive = 0;

    I2C_SDA_IN();/* SDA is set to input */

    for(i = 0; i < 8; i++)
    {
        I2C_SCL = 0;
        I2C_Delay();
        I2C_SCL = 1;
        receive <<=  1;
        if(I2C_READ_SDA)receive++; // 1
        I2C_Delay();
    }

    if(!ack)
        I2C_NAck();// send nACK
    else
        I2C_ACK();// send ACK
    return receive;
}
