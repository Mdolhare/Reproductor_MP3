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
#include "../EventGenerator/eventGenerator.h"
#include <string.h>

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

static uint8_t volumen = 15;
static bool vumeter_enable = true;



/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void playMusicInit(void) {
	decoderInit();
	transfer_to_dac_1 = true;
	buffer_complete = true;
	transfer_to_dac_1_prev = transfer_to_dac_1;

}

void setVolumen(uint8_t _volumen){
	if(_volumen < 30){
		volumen = _volumen;
	}
}

void vumeterEnable(bool enable){
	vumeter_enable = enable;
}


void playMusic() {
	static int count = 0;
	if(transfer_to_dac_1 && !buffer_complete)
	{


		if(decoderGetFrame(frame_decode_2, &frameInfo)){

			buffer_complete = true;

			for(int i=0;i<ARR_LEN/2 + 1;i++) {
				frame_decode_2[i] = ((frame_decode_2[2*i])*volumen)/30;//*(volume/30);
			}

			equalizerFilter(frame_decode_2, frame_decode_2, (frameInfo.outputSamps)/2);



			for (int i = 0; i < 3000; i++) {
				if ( /*(count == 1) &&*/ (i < 1024)) {
					frame_deepcopy_2[i] = frame_decode_2[i];
				}

				frame_decode_2[i] = (frame_decode_2[i]+32768)>>4;
			}
//			if (count == 1) {
				if(vumeter_enable){
					vumeterTransform(frame_deepcopy_2);
				}

//				count = 0;
//			}

//			count++;
		}
		else{
			//ENd song
			EG_addEvent(FINISH_SONG);
			//while(1);
		}
	}

	else if(!transfer_to_dac_1 && !buffer_complete)
	{
		if(decoderGetFrame(frame_decode_1, &frameInfo)){
			buffer_complete = true;

			for(int i=0;i<ARR_LEN/2 + 1;i++) {
				frame_decode_1[i] = ((frame_decode_1[2*i])*volumen)/30;//*(volume/30);
			}

			equalizerFilter(frame_decode_1, frame_decode_1, (frameInfo.outputSamps)/2);

			for (int i = 0; i < 3000; i++) {
				//frame_deepcopy_1[i] = frame_decode_1[i];
		 		frame_decode_1[i] = (frame_decode_1[i]+32768)>>4;
			}
			//vumeterTransform(frame_deepcopy_1);
			//count++;
		}
		else{
			EG_addEvent(FINISH_SONG);

//			while(1);
		}
	}

	if(transfer_to_dac_1_prev != transfer_to_dac_1)
	{
		buffer_complete = false;
		transfer_to_dac_1_prev = transfer_to_dac_1;
	}


}

void playPauseMusic(void){
	//debug
	//gpioWrite(LED_R, LOW);
	//gpioWrite(LED_B, LOW);
	//gpioWrite(LED_G, LOW);
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

void playNewSong(char * song){

	close();
	setPath(song);
	setIsFileOpen(false);
	audio_pause();
	
	//lee para open
	if(decoderGetFrame(frame_decode_1, &frameInfo)){

	}
	else {
		//Error
		while(1);
	}
	//readID3Tag();

	//Lee para primer frame, tener datos
	while(! decoderGetFrame(frame_decode_1, &frameInfo))
	if(1){

	}
	else{
		//Error
		while(1);
	}
	//static bool init = false;
	audio_init(frameInfo.samprate, frame_decode_1, frame_decode_2,
				frameInfo.outputSamps, &transfer_to_dac_1);
//	if (!init)
//	{

		vumeterInit(1024, frameInfo.samprate, 80, frameInfo.samprate/3);//frameInfo.samprate/2);

		int8_t gainDB[5] = {0,0,0,0,0};

		equalizerInit(gainDB);
//		init =  true;
//	}

	audio_resume();

}

bool getSongInfo(songInfo_t * song){
	ID3Tag_t * tag = getID3Tag();
	if(strcmp(tag->tag,"TAG")==0){
		strncpy(song->title, tag->title, 30);
		strncpy(song->artist, tag->artist, 30);
		strncpy(song->album, tag->album, 30);
		return true;
	}
	return false;
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/






