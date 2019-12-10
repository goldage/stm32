#include "24cxx.h"

/*******************************************************************************
* Function      : ATMEL_24CXX_Init.
* Description : 24CXX 初始化
* Arguments   : None
* Return         : None
*******************************************************************************/
void ATMEL_24CXX_Init(void)
{
    I2C_Configuration();
}

/*******************************************************************************
* Function      : ATMEL_24CXX_ReadOneByte.( Random Read )
* Description : 在AT24CXX指定地址读出一个数据
* Arguments   : ReadAddr:开始读数的地址
* Return         :返回值  :读到的数据
*******************************************************************************/
u8 ATMEL_24CXX_ReadOneByte(u16 ReadAddr)
{
    u8 temp = 0;

    I2C_Start();

    if( EE_TYPE > AT24C16) // If  EE_TYPE > AT24C16, read addr need 8-bit more.
    {
        I2C_Send_Byte(0XA0);//发送写命令
        IIC_Wait_Ack();
        I2C_Send_Byte(ReadAddr >> 8);//发送高地址
        I2C_Wait_Ack();
    }else{
        I2C_Send_Byte(0XA0 + ((ReadAddr / 256) << 1));//发送器件地址，写数据
    }

    I2C_Wait_Ack();
    I2C_Send_Byte(ReadAddr % 256); //发送低地址
    I2C_Wait_Ack();

    I2C_Start();
    I2C_Send_Byte(0XA1);//进入接收模式
    I2C_Wait_Ack();
    temp=I2C_Read_Byte(0);//读数据最后一个字节必须回应“nACK”
    I2C_Stop();//产生一个停止条件
    return temp;
}

/*******************************************************************************
* Function      : ATMEL_24CXX_WriteOneByte.( Byte Write )
* Description : 向AT24CXX指定地址写入一个数据
* Arguments   : ReadAddr:开始读数的地址
* Return         :返回值  :读到的数据
*******************************************************************************/
void ATMEL_24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{
    I2C_Start();

    if(EE_TYPE > AT24C16)
    {
        I2C_Send_Byte(0XA0);//发送写命令
        I2C_Wait_Ack();
        I2C_Send_Byte(WriteAddr >> 8);//发送高地址
    }else{
        I2C_Send_Byte(0XA0+((WriteAddr/256) << 1));//发送器件地址0XA0,写数据
    }

    I2C_Wait_Ack();
    I2C_Send_Byte(WriteAddr % 256);//发送低地址
    I2C_Wait_Ack();
    I2C_Send_Byte(DataToWrite);//发送字节
    I2C_Wait_Ack();
    I2C_Stop();
    delay_ms(10);
}

/*******************************************************************************
* Function      : ATMEL_24CXX_WriteLenByte.
* Description : 在AT24CXX里面的指定地址开始写入长度为Len的数据
                   该函数用于写入16bit或者32bit的数据.
* Arguments   : WriteAddr   :开始写入的地址
                   DataToWrite:数据数组首地址
                   Len              :要写入数据的长度2,4
* Return         :None
*******************************************************************************/
void ATMEL_24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len)
{
    u8 t;
    for(t = 0;  t < Len; t++)
    {
        ATMEL_24CXX_WriteOneByte(WriteAddr + t, (DataToWrite >> (8*t)) & 0xFF);
    }
}

/*******************************************************************************
* Function      : ATMEL_24CXX_ReadLenByte.
* Description : 在AT24CXX里面的指定地址开始读出长度为Len的数据
                   该函数用于读出16bit或者32bit的数据.
* Arguments   : ReadAddr   :开始读出的地址
                   Len              :要读出数据的长度2,4
* Return         :返回值  :读到的数据
*******************************************************************************/
void ATMEL_24CXX_ReadLenByte(u16 ReadAddr, u8 Len)
{
    u8 t;
    u32 temp;
    for(t = 0; t < Len; t++)
    {
        temp <<= 8;
        temp += ATMEL_24CXX_ReadOneByte(ReadAddr+Len-t-1);
    }
    return temp;
}

/*******************************************************************************
* Function      : ATMEL_24CXX_Check.
* Description : 检查AT24CXX是否正常
                   用了24C02的最后一个地址(255)来存储标志字
                   如果用其他24C系列,这个地址要修改
* Arguments   :None
* Return         :
                    1：检测失败
                    0：检测成功
*******************************************************************************/
u8 ATMEL_24CXX_Check(void)
{
    u8 temp;
    temp = ATMEL_24CXX_ReadOneByte(255);//避免每次开机都写入24C02
    if(temp = 0X55)return 0;
    else//排除第一次初始化的情况
    {
        ATMEL_24CXX_WriteOneByte(255,0X55);
        temp = ATMEL_24CXX_ReadOneByte(255);
        if(temp == 0X55)return 0;
    }
    return 1;
}

/*******************************************************************************
* Function      : ATMEL_24CXX_Read.
* Description : 在AT24CXX里面的指定地址开始读出指定个数的数据
* Arguments   :
                    ReadAddr :开始读出的地址 对24c02为 0~255
                    pBuffer  :数据数组首地址
                    NumToRead:要读出数据的个数
* Return         :  None
*******************************************************************************/
void ATMEL_24CXX_Read(u16 ReadAddr, u8 *pBuffer, u16 NumToRead)
{
    while(NumToRead)
    {
        *pBuffer++ = ATMEL_24CXX_ReadOneByte(ReadAddr++);
        NumToRead--;
    }
}

/*******************************************************************************
* Function      : ATMEL_24CXX_Write.
* Description : 在AT24CXX里面的指定地址开始写入指定个数的数据
* Arguments   :
                    WriteAddr :开始写入的地址 对24c02为0~255
                    pBuffer   :数据数组首地址
                    NumToWrite:要写入数据的个数
* Return         :  None
*******************************************************************************/
void ATMEL_24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
    while(NumToWrite--)
    {
        ATMEL_24CXX_WriteOneByte(WriteAddr, *pBuffer);
        WriteAddr++;
        pBuffer++;
    }
}
