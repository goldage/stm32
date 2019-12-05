#include "w25qxx.h"

u16 W25QXX_TYPE=W25Q128;	//默认是W25Q128

/*******************************************************************************
* Function    : W25QXX_Init.
* Description : 初始化SPI FLASH的IO口.
                4K bytes为一个 Sector
                16个 Sector 为 1 个 Block
                W25Q128 容量为 16M 字节,共有 128 个 Block ,4096 个 Sector.
* Arguments   : None
* Return      : None
*******************************************************************************/
void W25QXX_Init(void)
{
    W25QXX_CS=1;                //SPI FLASH不选中
    SPIx_Init();                //初始化SPI
    W25QXX_TYPE=W25QXX_ReadID();//读取FLASH ID.
    //printf("W25QXX_TYPE = %02x\r\n",W25QXX_TYPE);
}

/*******************************************************************************
* Function    : W25QXX_ReadSR.
* Description : 读取W25QXX的状态寄存器
                |BIT7  6   5   4   3   2   1   0|
                SPR   RV  TB BP2 BP1 BP0 WEL BUSY
                SPR:默认0,状态寄存器保护位,配合WP使用
                TB,BP2,BP1,BP0:FLASH区域写保护设置
                WEL:写使能锁定
                BUSY:忙标记位(1,忙;0,空闲)
                默认:0x00
* Arguments   : None
* Return      : None
*******************************************************************************/
u8 W25QXX_ReadSR(void)
{
    u8 byte=0;
    W25QXX_CS=0;                            //使能器件
    SPIx_ReadWriteByte(W25X_ReadStatusReg); //发送读取状态寄存器命令
    byte=SPIx_ReadWriteByte(0Xff);          //读取一个字节
    W25QXX_CS=1;                            //取消片选
    return byte;
}

/*******************************************************************************
* Function    : W25QXX_ReadSR.
* Description : 写W25QXX状态寄存器
                |BIT7  6   5   4   3   2   1   0|
                SPR   RV  TB BP2 BP1 BP0 WEL BUSY
                只有SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可以写
* Arguments   : None
* Return      : None
*******************************************************************************/
void W25QXX_Write_SR(u8 sr)
{
    W25QXX_CS=0;                            //使能器件
    SPIx_ReadWriteByte(W25X_WriteStatusReg);//发送写取状态寄存器命令
    SPIx_ReadWriteByte(sr);                 //写入一个字节
    W25QXX_CS=1;                            //取消片选
}

/*******************************************************************************
* Function    : W25QXX_Write_Enable.
* Description : 向FLASH发送 写使能 命令，将WEL置位
* Arguments   : None
* Return      : None
*******************************************************************************/
void W25QXX_Write_Enable(void)
{
    W25QXX_CS=0;                            //使能器件
    SPIx_ReadWriteByte(W25X_WriteEnable);   //发送写使能
    W25QXX_CS=1;                            //取消片选
}

/*******************************************************************************
* Function    : W25QXX_Write_Disable.
* Description : 向FLASH发送 写使能 命令，将WEL清零
* Arguments   : None
* Return      : None
*******************************************************************************/
void W25QXX_Write_Disable(void)
{
    W25QXX_CS=0;                            //使能器件
    SPIx_ReadWriteByte(W25X_WriteDisable);  //发送写禁止指令
    W25QXX_CS=1;                            //取消片选
}

/*******************************************************************************
* Function    : W25QXX_ReadID.
* Description : 读取芯片ID.
* Arguments   : None
* Return      :
                0XEF13,表示芯片型号为W25Q80
                0XEF14,表示芯片型号为W25Q16
                0XEF15,表示芯片型号为W25Q32
                0XEF16,表示芯片型号为W25Q64
                0XEF17,表示芯片型号为W25Q128
*******************************************************************************/
u16 W25QXX_ReadID(void)
{
    u16 Temp = 0;
    W25QXX_CS=0;
    SPIx_ReadWriteByte(0x90);//发送读取ID命令
    SPIx_ReadWriteByte(0x00);
    SPIx_ReadWriteByte(0x00);
    SPIx_ReadWriteByte(0x00);
    Temp|=SPIx_ReadWriteByte(0xFF)<<8;
    Temp|=SPIx_ReadWriteByte(0xFF);
    W25QXX_CS=1;
    return Temp;
}

/*******************************************************************************
* Function    : W25QXX_Read.
* Description : 读取SPI FLASH,在指定地址开始读取指定长度的数据
                pBuffer:数据存储区
                ReadAddr:开始读取的地址(24bit)
                NumByteToRead:要读取的字节数(最大65535)
* Arguments   : None
* Return      : None
*******************************************************************************/
void W25QXX_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)
{
    u16 i;
    W25QXX_CS=0;                                //使能器件
    SPIx_ReadWriteByte(W25X_ReadData);          //发送读取命令
    SPIx_ReadWriteByte((u8)((ReadAddr)>>16));   //发送24bit地址
    SPIx_ReadWriteByte((u8)((ReadAddr)>>8));
    SPIx_ReadWriteByte((u8)ReadAddr);
    for(i=0;i<NumByteToRead;i++)
    {
        pBuffer[i]=SPIx_ReadWriteByte(0XFF);    //循环读数
    }
    W25QXX_CS=1;
}

/***********************************************************************************
* Function    : W25QXX_Write_Page.
* Description : SPI在一页内写入少于256个字节的数据
                在指定地址开始写入最大256字节的数据
                pBuffer:数据存储区
                WriteAddr:开始写入的地址(24bit)
                NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数
* Arguments   : None
* Return      : None
*************************************************************************************/
void W25QXX_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
    u16 i;
    W25QXX_Write_Enable();                  	//SET WEL
    W25QXX_CS=0;                            	//使能器件
    SPIx_ReadWriteByte(W25X_PageProgram);      	//发送写页命令
    SPIx_ReadWriteByte((u8)((WriteAddr)>>16)); 	//发送24bit地址
    SPIx_ReadWriteByte((u8)((WriteAddr)>>8));
    SPIx_ReadWriteByte((u8)WriteAddr);
    for(i=0;i<NumByteToWrite;i++)SPIx_ReadWriteByte(pBuffer[i]);//循环写入
    W25QXX_CS=1;                            	//取消片选
    W25QXX_Wait_Busy();					   		//等待写入结束
}

/**************************************************************************************
* Function    : W25QXX_Write_NoCheck.
* Description : 无检验写SPI FLASH
                具有自动换页功能
                必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败
                在指定地址开始写入指定长度的数据,但是要确保地址不越界
                pBuffer:数据存储区
                WriteAddr:开始写入的地址(24bit)
                NumByteToWrite:要写入的字节数(最大65535)
* Arguments   : None
* Return      : None
**************************************************************************************/
void W25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
    u16 pageremain;
    pageremain=256-WriteAddr%256; //单页剩余的字节数
    if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//不大于256个字节
    while(1)
    {
        W25QXX_Write_Page(pBuffer,WriteAddr,pageremain);
        if(NumByteToWrite==pageremain)break;//写入结束了
        else //NumByteToWrite>pageremain
        {
            pBuffer+=pageremain;
            WriteAddr+=pageremain;

            NumByteToWrite-=pageremain;             //减去已经写入了的字节数
            if(NumByteToWrite>256)pageremain=256;   //一次可以写入256个字节
            else pageremain=NumByteToWrite;         //不够256个字节了
        }
    };
}

//写SPI FLASH
//在指定地址开始写入指定长度的数据
//该函数带擦除操作!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
/*******************************************************************************
* Function    : W25QXX_Write.
* Description : 写SPI FLASH
                带擦除操作
                在指定地址开始写入指定长度的数据
                pBuffer:数据存储区
                WriteAddr:开始写入的地址(24bit)
                NumByteToWrite:要写入的字节数(最大65535)
* Arguments   : None
* Return      : None
*******************************************************************************/
u8 W25QXX_BUFFER[4096];
void W25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
    u32 secpos;
    u16 secoff;
    u16 secremain;
    u16 i;
    u8 * W25QXX_BUF;
    W25QXX_BUF=W25QXX_BUFFER;
    secpos=WriteAddr/4096;//扇区地址
    secoff=WriteAddr%4096;//在扇区内的偏移
    secremain=4096-secoff;//扇区剩余空间大小
    //printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//测试用
    if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//不大于4096个字节
    while(1)
    {
        W25QXX_Read(W25QXX_BUF,secpos*4096,4096);//读出整个扇区的内容
        for(i=0;i<secremain;i++)//校验数据
        {
            if(W25QXX_BUF[secoff+i]!=0XFF)break;//需要擦除
        }
        if(i<secremain)//需要擦除
        {
            W25QXX_Erase_Sector(secpos);		//擦除这个扇区
            for(i=0;i<secremain;i++)	   		//复制
            {
                W25QXX_BUF[i+secoff]=pBuffer[i];
            }
            W25QXX_Write_NoCheck(W25QXX_BUF,secpos*4096,4096);//写入整个扇区

        }else W25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);//写已经擦除了的,直接写入扇区剩余区间.
        if(NumByteToWrite==secremain)break;//写入结束了
        else//写入未结束
        {
            secpos++;//扇区地址增1
            secoff=0;//偏移位置为0

            pBuffer+=secremain;  				//指针偏移
            WriteAddr+=secremain;				//写地址偏移
            NumByteToWrite-=secremain;			//字节数递减
            if(NumByteToWrite>4096)secremain=4096;//下一个扇区还是写不完
            else secremain=NumByteToWrite;		//下一个扇区可以写完了
        }
    };
}

/*******************************************************************************
* Function    : W25QXX_Erase_Chip.
* Description : 擦除整个芯片
                等待时间较长
* Arguments   : None
* Return      : None
*******************************************************************************/
void W25QXX_Erase_Chip(void)
{
    W25QXX_Write_Enable();                      //SET WEL
    W25QXX_Wait_Busy();
    W25QXX_CS=0;                                //使能器件
    SPIx_ReadWriteByte(W25X_ChipErase);         //发送片擦除命令
    W25QXX_CS=1;                                //取消片选
    W25QXX_Wait_Busy();                         //等待芯片擦除结束
}

/*******************************************************************************
* Function    : W25QXX_Erase_Sector.
* Description : 擦除一个扇区
                Dst_Addr:扇区地址 根据实际容量设置
                擦除一个扇区的最少时间:150ms
* Arguments   : None
* Return      : None
*******************************************************************************/
void W25QXX_Erase_Sector(u32 Dst_Addr)
{
    //监视flash擦除情况,测试用
    //printf("Dst_Addr fe:0x%02x\r\n",Dst_Addr);
    Dst_Addr*=4096;
    W25QXX_Write_Enable();                      //SET WEL
    W25QXX_Wait_Busy();
    W25QXX_CS=0;                                //使能器件
    SPIx_ReadWriteByte(W25X_SectorErase);       //发送扇区擦除指令
    SPIx_ReadWriteByte((u8)((Dst_Addr)>>16));   //发送24bit地址
    SPIx_ReadWriteByte((u8)((Dst_Addr)>>8));
    SPIx_ReadWriteByte((u8)Dst_Addr);
    W25QXX_CS=1;                                //取消片选
    W25QXX_Wait_Busy();                         //等待擦除完成
}

/*******************************************************************************
* Function    : W25QXX_Wait_Busy.
* Description : 等待WIP(BUSY)标志被置0，即等待FLASH内部数据写入完毕
* Arguments   : None
* Return      : None
*******************************************************************************/
void W25QXX_Wait_Busy(void)
{
    while((W25QXX_ReadSR()&0x01)==0x01);        //等待BUSY位清空
}

/*******************************************************************************
* Function    : W25QXX_PowerDown.
* Description : 进入掉电模式
* Arguments   : None
* Return      : None
*******************************************************************************/
void W25QXX_PowerDown(void)
{
    W25QXX_CS=0;                                //使能器件
    SPIx_ReadWriteByte(W25X_PowerDown);         //发送掉电命令
    W25QXX_CS=1;                                //取消片选
    delay_us(3);                                //等待TPD
}

/*******************************************************************************
* Function    : W25QXX_WAKEUP.
* Description : 唤醒Flash
* Arguments   : None
* Return      : None
*******************************************************************************/
void W25QXX_WAKEUP(void)
{
    W25QXX_CS=0;                                //使能器件
    SPIx_ReadWriteByte(W25X_ReleasePowerDown);  //  send W25X_PowerDown command 0xAB
    W25QXX_CS=1;                                //取消片选
    delay_us(3);                                //等待TRES1
}


const u8 TEXT_Buffer[]={"Laurence SPI TEST,store at 0x000000."};
#define SIZE sizeof(TEXT_Buffer)
void W25Qxx_ReadWrite_Test(void)
{
    u8 stat_reg = 0;
    u16 flash_id = 0;
    uint8_t DataBuffer[16] = {0};
    uint8_t srDataBuf[16]= {0};
    u32 FLASH_ADDR = 0;
    u8 datatemp[SIZE];

    /* W25Q128 读取 Manufacturer and Device ID */
    flash_id = W25QXX_ReadID();
    printf("flash_id = %02x\r\n",flash_id);

    /* W25Q128 读取状态寄存器 */
    W25QXX_Write_Disable();
    stat_reg = W25QXX_ReadSR();
    printf("disable stat_reg = %x\r\n",stat_reg);

    W25QXX_Write_Enable();
    stat_reg = W25QXX_ReadSR();
    printf("enable stat_reg = %x\r\n",stat_reg);

    W25QXX_Write_SR(0x00);
    W25QXX_Wait_Busy();

    stat_reg = W25QXX_ReadSR();
    printf("after write stat_reg = %x\r\n",stat_reg);

    /* W25Q128 写测试 */
    printf("Start Write W25Q128....\r\n");
    W25QXX_Write((u8*)TEXT_Buffer,FLASH_ADDR,SIZE);
    printf("W25Q128 Write Finished!\r\n");	//提示传送完成

    /* W25Q128 读测试 */
    printf("Start Read W25Q128.... \r\n");
    W25QXX_Read(datatemp,FLASH_ADDR,SIZE);
    printf("The Data Readed Is:  \r\n");	//提示传送完成
    printf("%s\r\n",datatemp);              //显示读到的字符串
}
