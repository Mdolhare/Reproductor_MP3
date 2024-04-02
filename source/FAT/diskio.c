/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "SD.h"

/* Definitions of physical drive number for each drive */
#define DEV_SD		0	/* Example: Map SD to physical drive 0 */
//#define DEV_MMC		1	/* Example: Map MMC card to physical drive 1 */
//#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat=0;

	switch (pdrv) {
	case DEV_SD :
	{
		if(!SD_isSDcard()){
			stat = STA_NODISK;
		}
		else if(SD_getStatus() == SD_NOTINIT){
			stat = STA_NOINIT;
		}
		// translate the reslut code here
		return stat;
	}
	/*
	case DEV_MMC :
		result = MMC_disk_status();

		// translate the reslut code here

		return stat;

	case DEV_USB :
		result = USB_disk_status();

		// translate the reslut code here

		return stat;
*/
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
	bool ok = false;
	switch (pdrv) {
	case DEV_SD :
		{
			if(!SD_isSDcard()){
				stat = STA_NODISK;
			}
			else if(SD_getStatus() == SD_NOTINIT){
				stat = STA_NOINIT;
				ok = SD_initializationProcess();
				if(ok){
					stat = 0;
				}
				else{
					stat = STA_NOINIT;
				}
			}
			else{
				//inicializado previamente
				stat = 0;
			}


			// translate the reslut code here

			return stat;
		}
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res;
	int result;
	bool ok = false;
	switch (pdrv) {
	case DEV_SD:
		// translate the arguments here

		ok = SD_readSectors(sector, count, buff);
		if(ok){
			res = RES_OK;
		}
		else{
			switch(SD_getStatus()){
				case SD_ERROR:
					res = RES_ERROR;
					break;
				case SD_NOTRDY:
					res = RES_NOTRDY;
					break;
				default:
					res = RES_PARERR;
			}
		}

		// translate the reslut code here

		return res;
	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case DEV_SD :
		// translate the arguments here

		result = RAM_disk_write(buff, sector, count);

		// translate the reslut code here

		return res;
	}

	return RES_PARERR;
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
/*
	switch (pdrv) {
	case DEV_SD :

		// Process of the command for the RAM drive

		return res;
	}
*/
	return RES_PARERR;
}

