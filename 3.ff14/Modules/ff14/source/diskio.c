/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "sys.h"

/* Definitions of physical drive number for each drive */
//#define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
//#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
//#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */
#define EX_FLASH 0    /*physical drive number */
#define FLASH_SECTOR_SIZE 4096
#define FLASH_BLOCK_SIZE   16
u32     FLASH_SECTOR_COUNT = 4096 ; /* W25Q128(16*1024*1024/4096) */

#define DEVICE_ID 0x17 /* W25Q128 :0xef17 */



/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
     
    case EX_FLASH :
        stat = (BYTE)(W25QXX_ReadID());
    
        if(stat == DEVICE_ID)
        {
            /*Initial success*/
            return 0;
        }
        else
        {
            return STA_NOINIT;
        }
    }
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;
    int i;
    
	switch (pdrv) {
        
    case EX_FLASH :
        
        W25QXX_Init();
    
        i = 500;
        while(i--);
        
        W25QXX_WAKEUP();
        stat = disk_status(EX_FLASH);
        
        return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res;
	int result;

    if (!count)return RES_PARERR;

	switch (pdrv) {
    case EX_FLASH :
        
        for(;count>0;count--)
        {
            W25QXX_Read(buff,sector*FLASH_SECTOR_SIZE,FLASH_SECTOR_SIZE);
            sector++;
            buff+=FLASH_SECTOR_SIZE;
        }

        return RES_OK;
    default:
        return RES_ERROR;
    }

}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res;
	int result;

    if (!count)return RES_PARERR;
    
	switch (pdrv) {
    case EX_FLASH :
           
            for(;count>0;count--)
            {  
                W25QXX_Write((u8*)buff,sector*FLASH_SECTOR_SIZE,FLASH_SECTOR_SIZE);
                sector++;
                buff+=FLASH_SECTOR_SIZE;
            }
            return RES_OK;
    default:
            return RES_ERROR;
    }
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
	int result;

    if(pdrv == EX_FLASH)
    {
        switch(cmd)
        {
            case CTRL_SYNC:
                res = RES_OK;
                break;	 
            case GET_SECTOR_SIZE:
                *(WORD*)buff = FLASH_SECTOR_SIZE;
                res = RES_OK;
                break;
            case GET_BLOCK_SIZE:
                *(WORD*)buff = FLASH_BLOCK_SIZE;
                res = RES_OK;
                break;
            case GET_SECTOR_COUNT:
                *(DWORD*)buff = FLASH_SECTOR_COUNT;
                res = RES_OK;
                break;
            default:
                res = RES_PARERR;
                break;
        }
    }else
    {
        res=RES_ERROR;
    }

	return res;
}

/* 
    User defined function to give a current time to fatfs module
    31-25: Year(0-127 org.1980), 24-21: Month(1-12), 20-16: Day(1-31) 
    15-11: Hour(0-23), 10-5: Minute(0-59), 4-0: Second(0-29 *2)
*/
DWORD get_fattime (void)
{
    return 0;
}


