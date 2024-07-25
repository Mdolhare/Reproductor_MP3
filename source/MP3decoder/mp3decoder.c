
#include "../Drivers/HAL/SD/SD.h"
#include "../Drivers/MCAL/Gpio/gpio.h"
#include "../FAT/ff.h"
#include "mp3decoder.h"

#define MAX_SIZE_FRAME 1940 //chequear

static HMP3Decoder decoder;
static MP3FrameInfo frameInfo;

static bool  file_was_open = false;
static unsigned char* out_buffer[4096*400];


static FATFS FatFs;		/* FatFs work area needed for each volume */
static FIL FileMP3;		/* File object needed for each open file */
static FRESULT fr;



void decoderInit(void) {
    decoder = MP3InitDecoder();
}


uint16_t decoderGetBitrate(unsigned char *buf, uint32_t bytesLeft) {
    uint32_t offset = MPF3indSyncWord(buf, bytesLeft);
    unsigned char* buffer = buf + offset;
    bytesLeft -= offset;

    MP3GetNextFrameInfo(decoder, &frameInfo, buffer);

    return frameInfo.bitrate;
}



bool decoderGetFrame(unsigned char *frame_decode,MP3FrameInfo * _frameInfo)
{
	uint8_t buffer_info_file[MAX_SIZE_FRAME];
	  //ver stereo
	uint32_t offset;
	uint32_t bytesLeft = 0;
	uint32_t bytesRead = 0;
	//
	//
	if (!file_was_open)
	{
		fr = f_mount(&FatFs, "", 0);
		fr = f_open(&FileMP3, "Boca_Yo_Te_Amo.mp3", FA_READ);
		file_was_open = true;
	}
	else
	{
		while(!f_eof(&FileMP3))
		{
			if(f_read(&FileMP3, buffer_info_file + bytesLeft, MAX_SIZE_FRAME -  bytesLeft, &bytesRead  ) == FR_OK)
			{

			    offset = MP3FindSyncWord(buffer_info_file, bytesRead);
			    if(offset < 0) //error
			    {
			    	continue;
			    	 //bytesLeft = bytesRead;

			    }
			    unsigned char* buffer_info_file_aux  = buffer_info_file + offset;
			    bytesLeft = bytesRead - offset;

			    MP3GetNextFrameInfo(decoder, _frameInfo, buffer_info_file_aux);

			    uint8_t error = MP3Decode(decoder, &buffer_info_file_aux, &bytesLeft,(int16_t*)frame_decode, 0);

			    f_lseek(&FileMP3, f_tell(&FileMP3)- bytesLeft);

			    if (!error)
			    {
			    	return true;
			    }
			    else
			    {
			    	return false;
			    }

			}
			else
			{
				return false;
			}


		}
		return false;
	}


}

