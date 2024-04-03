/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Grupo 2
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>

#include "SD.h"
#include "gpio.h"
#include "ff.h"
#include "audio.h"
#include "../helix/pub/mp3dec.h"
#include "pit.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define LED_B PORTNUM2PIN(PB,21)

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/


/*******************************************************************************
 *******************************************************************************
 *                      GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 *******************************************************************************/
void pitFunc(void);

void App_Init (void){
	SD_init();
	gpioMode(LED_B, OUTPUT);
	gpioWrite(LED_B,1);
}




/* Función que se llama constantemente en un ciclo infinito */
void App_Run(){
	bool ok=false;



	while(1){
		if(SD_isSDcard()){
			ok = SD_initializationProcess();
			if(ok){
				//se inicializo bien la tarjeta
				break;
			}
			while(1);
		}
		else{
				//
		}
	}


	MP3FrameInfo frameInfo;

	decoderInit();
	uint16_t frame_decode_1[3000] = {0};
	uint16_t frame_decode_2[3000] = {0};

	//lee para open
	if(decoderGetFrame(frame_decode_1, &frameInfo)){

		}
	else{
			while(1);
		}
	//Lee para primer frame, tener datos
	if(decoderGetFrame(frame_decode_1, &frameInfo)){

			}
		else{
				while(1);
			}



	bool transfer_to_dac_1 = true;
	bool buffer_complete = false;
	audio_init(frameInfo.samprate, frame_decode_1, frame_decode_2, frameInfo.outputSamps, &transfer_to_dac_1 );
	audio_resume();
	bool transfer_to_dac_1_prev = transfer_to_dac_1;
	while(1){

	if(transfer_to_dac_1 && !buffer_complete)
	{
		if(decoderGetFrame(frame_decode_2, &frameInfo)){
					buffer_complete = true;
		}
		else{
			while(1);
		}
	}

	else if(!transfer_to_dac_1 && !buffer_complete)
	{
		if(decoderGetFrame(frame_decode_1, &frameInfo)){
							buffer_complete = true;
		}
		else{
			while(1);
		}
	}

	if(transfer_to_dac_1_prev != transfer_to_dac_1)
	{
		buffer_complete = false;
		transfer_to_dac_1_prev = transfer_to_dac_1;
	}
	}
}

void pitFunc(void)
{
	int i;
}

void App_Run_3 (void) {

	bool ok = true;
	uint32_t data[50000] = {0};
	gpioWrite(LED_B,1);

	//Test SD driver
	while(1){
		if(SD_isSDcard()){
			ok = SD_initializationProcess();
			if(ok){
				//se inicializo bien la tarjeta
				break;
			}
			while(1);
		}
		else{
			gpioWrite(LED_B,1);
		}
	}

	//prueba de leer sectores de SD
	ok = SD_readSectors(0,1,data);

	ok = SD_readSectors(0,20,data);

	ok = SD_readSectors(1,3,data);

	ok = SD_readSectors(1,40,data);



	//Test FatFs con SD driver
	FATFS FatFs;		/* FatFs work area needed for each volume */
	FIL Fil;			/* File object needed for each open file */
	FRESULT fr;
	uint8_t buff[1000] = {0};
	uint32_t numBytes = 0;

	fr = f_mount(&FatFs, "", 0);		/* Give a work area to the default drive */

	fr = f_open(&Fil, "himno1.mp3", FA_READ);	/* En la SD debe existir un archivo con este mismo nombre*/

	if (fr == FR_OK) {
		//leer un archivo txt
	//	fr = f_read(&Fil, (void*)buff, sizeof(buff), &numBytes);
		if(numBytes == 0 && fr == FR_OK){
			//encontramos EndOfFile (EOF)
		}
		else if(numBytes > 0){
			//se leyeron datos, pero aun faltan en el archivo
		}

		//Al leer varias veces, se guarda internamente en Fil el puntero al byte donde
		//termina la lectura.
		//Ademas, lo que se lee simpre se guarda en (void*)buff
		//En estos siguientes ejemplos se pisa la lectura en el arreglo buff.
		//leo primeros 5 bytes (del 0 al 4), guardo en buff[0:4]
//		fr = f_read(&Fil, (void*)buff, 5, &numBytes);

		//leo siguientes 10 (del 5 al 14), guardo en buff[0:9]
		fr = f_read(&Fil, (void*)buff, 1000, &numBytes);

		//leo de mas
		fr = f_read(&Fil, (void*)buff, 10, &numBytes);



		fr = f_close(&Fil);							/* Close the file */
		if (fr == FR_OK /*&& bw == 11*/) {				/* Lights green LED if data written well */
			gpioWrite(LED_B,0);						/* gpio */
		}
	}

	while(1);


}

