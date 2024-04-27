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

#include "Drivers/HAL/SD/SD.h"
#include "Drivers/MCAL/Gpio/gpio.h"
#include "FAT/ff.h"
#include "Audio/audio.h"
#include "../helix/pub/mp3dec.h"
#include "Drivers/MCAL/PIT/pit.h"

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

void App_Init (void) {

	SD_init();
	gpioMode(LED_B, OUTPUT);
	gpioWrite(LED_B,1);

}




/* Función que se llama constantemente en un ciclo infinito */
void App_Run(void) {
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

