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

void App_Init (void){
	SD_init();
	gpioMode(LED_B, OUTPUT);
	gpioWrite(LED_B,1);
}




/* Función que se llama constantemente en un ciclo infinito */

void App_Run (void) {

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
	uint8_t buff[100] = {0};
	uint32_t numBytes = 0;

	fr = f_mount(&FatFs, "", 0);		/* Give a work area to the default drive */

	fr = f_open(&Fil, "hola_mundo.txt", FA_READ);	/* En la SD debe existir un archivo con este mismo nombre*/

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
		fr = f_read(&Fil, (void*)buff, 5, &numBytes);

		//leo siguientes 10 (del 5 al 14), guardo en buff[0:9]
		fr = f_read(&Fil, (void*)buff, 10, &numBytes);

		//leo de mas
		fr = f_read(&Fil, (void*)buff, 110, &numBytes);



		fr = f_close(&Fil);							/* Close the file */
		if (fr == FR_OK /*&& bw == 11*/) {				/* Lights green LED if data written well */
			gpioWrite(LED_B,0);						/* gpio */
		}
	}

	while(1);


}

