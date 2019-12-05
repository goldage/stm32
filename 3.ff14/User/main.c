#include "sys.h"

/**********************************Global Variable***********************************************************/
/* FatFs 文件系统 */
BYTE work[FF_MAX_SS];   /* Work area (larger is better for processing time) */
FATFS fs;               /* FatFs文件系统对象 */
FIL fnew;               /* 文件对象 */

/********************************************************************************************************/
extern void File_System_Test(void);

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
    //W25QXX_Init();

}

int main(void)
{
    prvSetupHardware();

    printf("Hello FatFs.\r\n");

    //W25Qxx_ReadWrite_Test();
    File_System_Test();
    
    while(1)
    {  
        LED_Test();
    }

}

void File_System_Test(void)
{
    FRESULT res_sd;                     /* 文件操作结果 */
    UINT fnum;                          /* 文件成功读写数量 */
    BYTE ReadBuffer[1024]={0};          /* 读缓冲区 */
    BYTE WriteBuffer[] = "Hey FatFs. \r\n"; /* 写缓冲区*/

    printf("\r\n****** Mount File System Test... ******\r\n ");
    res_sd = f_mount(&fs,"0:",1); 
    /* FLASH磁盘, FAT文件系统错误,重新格式化FLASH */
    if(res_sd == FR_NO_FILESYSTEM)
    {
        printf("Flash Disk Formatting...\r\n");
        /* 格式化FLASH,0,盘符 */
        res_sd=f_mkfs("0:",0,work,sizeof(work));
        if(res_sd==0)
        {
            printf("Flash Disk Format Finish\r\n");//格式化完成
            /* 格式化后，先取消挂载 */
            res_sd = f_mount(NULL,"0:",1);
            printf("Umount filesystem now.\r\n");
            /* 重新挂载*/
            res_sd = f_mount(&fs,"0:",1);
            printf("Remount filesystem now,res_sd = 0x%02x.\r\n",res_sd);
        }else 
            /* 格式化失败 */
            printf("Flash Disk Format Error,res_sd = %02x \r\n",res_sd);
        Delay_ms(1000);
    }
    else
    {
        printf("mount res_sd = 0x%02x.\r\n",res_sd);
    }

    
    printf("\r\n****** Write File Test... ******\r\n");
    /* 打开文件，如果文件不存在则创建它 */
    res_sd = f_open(&fnew, "fatfs.txt",FA_CREATE_ALWAYS | FA_WRITE );
    if ( res_sd == FR_OK )
    {
        printf("Open/Create fatfs.txt success,writing data into file...\r\n");
        /* 将指定存储区内容写入到文件内 */
        res_sd=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
        if(res_sd==FR_OK)
        {
          printf("Writing file success,fnum = %d\n",fnum);
          printf("The writen data is \r\n%s\r\n",WriteBuffer);
        }
        else
        {
            printf("f_write failed res_sd = %02x\n",res_sd);
        }    
    /* 不再读写，关闭文件 */
    f_close(&fnew);
    }
    else
    {
        printf("Open/Create file failed，res_sd = %02x.\r\n",res_sd);
    }
  
    printf("\r\n****** Read File Test... ******\r\n");
    res_sd = f_open(&fnew, "fatfs.txt", FA_OPEN_EXISTING | FA_READ); 
    if(res_sd == FR_OK)
    {;
        printf("Open file success.\r\n");
        res_sd = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum); 
        if(res_sd==FR_OK)
        {
          printf("Reading file success,fnum = %d\r\n",fnum);
          printf("The read data is：\r\n%s \r\n", ReadBuffer);	
        }
        else
        {
          printf("Read file failed.：(%d)\n",res_sd);
        }
    }
    else
    {
        printf("Open file failed.\r\n");
    }
    
    /* 不再读写，关闭文件 */
    f_close(&fnew);
    printf("Close file now.\r\n");

    /* 不再使用文件系统，取消挂载文件系统 */
    f_mount(NULL,"0:",1);
    printf("Umount filesystem now.\r\n");
}

