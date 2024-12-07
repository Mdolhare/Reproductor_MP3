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
#include "../Equalizer/equalizer.h"

//debug
#include "../Drivers/MCAL/Gpio/gpio.h"
#define LED_R PORTNUM2PIN(PB,22)
#define LED_B PORTNUM2PIN(PB,21)
#define LED_G PORTNUM2PIN(PE,26)

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define ARR_LEN 8192

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

static void from_16bit_to_32bit(int16_t* input, int32_t* output, int32_t len);
static void from_32bit_to_16bit(int32_t* input, int16_t* output, int32_t len);
static void from_16bit_to_f32(int16_t* input, float* output, int32_t len);
static void from_f32_to_16bit(float* input, int16_t* output, int32_t len);

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/


/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static int16_t frame_decode_1[ARR_LEN];

static int16_t frame_deepcopy_1[ARR_LEN];

static float frame_long_1[ARR_LEN];

static int16_t frame_decode_2[ARR_LEN];

static int16_t frame_deepcopy_2[ARR_LEN];

static float frame_long_2[ARR_LEN];

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
	/*
	for(i = 0; i < 3000; i++){
		frame_decode_1[i] = frame_decode_1[i]/4;
	}
	*/

	transfer_to_dac_1 = true;
	buffer_complete = true;
	transfer_to_dac_1_prev = transfer_to_dac_1;

	audio_init(frameInfo.samprate, frame_decode_1, frame_decode_2,
			frameInfo.outputSamps, &transfer_to_dac_1);

	vumeterInit(4096, frameInfo.samprate, 80, 15000);

	int8_t gainDB[8] = {2,2,2,2,2,2,2,2};

	equalizerInit(gainDB);
}

void playMusic(void) {

	if(transfer_to_dac_1 && !buffer_complete)
	{
		if(decoderGetFrame(frame_decode_2, &frameInfo)){
			buffer_complete = true;

			for(int i=0;i<ARR_LEN/2 + 1;i++) {
				frame_decode_2[i] = frame_decode_2[2*i];
			}

			equalizerFilter(frame_decode_2, frame_decode_2, (frameInfo.outputSamps)/2);

			for (int i = 0; i < 5000; i++) {
				frame_deepcopy_2[i] = frame_decode_2[i];
				frame_decode_2[i] = (frame_decode_2[i]+32768)>>4;
			}
			vumeterTransform(frame_deepcopy_2);

		}
		else{
			while(1);
		}
	}

	else if(!transfer_to_dac_1 && !buffer_complete)
	{
		if(decoderGetFrame(frame_decode_1, &frameInfo)){
			buffer_complete = true;

			for(int i=0;i<ARR_LEN/2 + 1;i++) {
				frame_decode_1[i] = frame_decode_1[2*i];
			}

			equalizerFilter(frame_decode_1, frame_decode_1, (frameInfo.outputSamps)/2);

			for (int i = 0; i < 5000; i++) {
				frame_deepcopy_1[i] = frame_decode_1[i];
		 		frame_decode_1[i] = (frame_decode_1[i]+32768)>>4;
			}
			vumeterTransform(frame_deepcopy_1);

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

void playMusicPause(void){
	//debug
	gpioWrite(LED_R, LOW);
	gpioWrite(LED_B, LOW);
	gpioWrite(LED_G, LOW);

	static bool isPlaying = false;
	if(isPlaying){
		audio_pause();
		isPlaying = false;
	}
	else{
		audio_resume();
		isPlaying = true;
	}
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


static void from_16bit_to_32bit(int16_t* input, int32_t* output, int32_t len) {
	for (int i=0; i<len; i++) {
		output[i] = ((int32_t)input[i]);
	}
}


static void from_32bit_to_16bit(int32_t* input, int16_t* output, int32_t len) {
	for (int i=0; i<len; i++) {
		output[i] = (int16_t)input[i];
	}
}

static void from_16bit_to_f32(int16_t* input, float* output, int32_t len) {
	for (int i=0; i<len; i++) {
		output[i] = ((float)input[i]);
	}
}


static void from_f32_to_16bit(float* input, int16_t* output, int32_t len) {
	for (int i=0; i<len; i++) {
		output[i] = (int16_t)input[i];
	}
}







