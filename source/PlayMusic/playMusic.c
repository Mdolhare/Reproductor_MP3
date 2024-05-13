/*
 * playMusic.c
 *
 *  Created on: Apr 1, 2024
 *      Author: Grupo 2
 */

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "playMusic.h"
#include "../Audio/audio.h"
#include "../MP3decoder/mp3decoder.h"
#include "../../helix/pub/mp3dec.h"
#include "../Vumeter/vumeter.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

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
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/


/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static int16_t frame_decode_1[3000];
static int16_t frame_decode_2[3000];

static bool transfer_to_dac_1;
static bool buffer_complete;
static bool transfer_to_dac_1_prev;

static MP3FrameInfo frameInfo;


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void playMusicInit(void) {

	decoderInit();

	//lee para open
	if(decoderGetFrame(frame_decode_1, &frameInfo)){

	}
	else {
		while(1);
	}
	//Lee para primer frame, tener datos
	if(decoderGetFrame(frame_decode_1, &frameInfo)){

	}
	else{
		while(1);
	}
	uint32_t i = 0;
	for(i = 0; i < 3000; i++){
		frame_decode_1[i] = frame_decode_1[i]/4;
	}


	transfer_to_dac_1 = true;
	buffer_complete = false;
	transfer_to_dac_1_prev = transfer_to_dac_1;

	audio_init(frameInfo.samprate, frame_decode_1, frame_decode_2,
			frameInfo.outputSamps, &transfer_to_dac_1);

//	vumeterInit(frameInfo.outputSamps, frameInfo.samprate, 80, 15000);
}

void playMusic(void) {

	if(transfer_to_dac_1 && !buffer_complete)
	{
		if(decoderGetFrame(frame_decode_2, &frameInfo)){
			buffer_complete = true;
			for (int i = 0; i < 3000; i++) {
				frame_decode_2[i] = (frame_decode_2[i]+32768)>>4;
			}
			//vumeterTransform(frame_decode_2);
		}
		else{
			while(1);
		}
	}

	else if(!transfer_to_dac_1 && !buffer_complete)
	{
		if(decoderGetFrame(frame_decode_1, &frameInfo)){
			buffer_complete = true;
			for (int i = 0; i < 3000; i++) {
				frame_decode_1[i] = (frame_decode_1[i]+32768)>>4;
			}
			//vumeterTransform(frame_decode_1);
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



/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


