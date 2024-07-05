/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Grupo 2
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "MK64F12.h"
#include "arm_math.h"

#include <stdint.h>
#include <stdbool.h>



#include "Drivers/HAL/SD/SD.h"
#include "Drivers/MCAL/Gpio/gpio.h"
#include "FAT/ff.h"
#include "Audio/audio.h"
#include "../helix/pub/mp3dec.h"
#include "Drivers/MCAL/PIT/pit.h"
#include "sen.h"
#include "Vumeter/vumeter.h"
#include "FSM/fsm.h"
#include "FSM/fsmTable.h"


//en el concector jack VERDE es GND


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define LED_R PORTNUM2PIN(PB,22)
#define LED_B PORTNUM2PIN(PB,21)
#define LED_G PORTNUM2PIN(PE,26)

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
void playPause(void);

void App_Init (void) {

	gpioMode(LED_R, OUTPUT);
	gpioMode(LED_B, OUTPUT);
	gpioMode(LED_G, OUTPUT);
	gpioWrite(LED_R, HIGH);
	gpioWrite(LED_B, HIGH);
	gpioWrite(LED_G, HIGH);
	//-
	SD_init();

}

void App_Init_2 (void) {

//	vumeterTransform(sine_wave);

}

void App_Run(){
	bool ok=false;
	while(1){
		if(SD_isSDcard()){
			ok = SD_initializationProcess();
			SD_cardStatus a;
			if(ok){
				//se inicializo bien la tarjeta
				break;
			}
			else{

				a = SD_getStatus();
			}
			while(1);
		}
		else{
				//
		}
	}
	EG_init();
	playMusicInit();//despues de haber SD hago init playMusic, no antes

	state_t state = FSM_GetInitState();
	uint8_t ev=0;
	while(1){

		if(EG_isNewEvent()){
			ev = EG_getEvent();
			fsm(state,ev);
		}

		playMusic();
	}

}



void App_Run_5(){
	vumeterInit(80, 44100, 80, 15000);
	vumeterTransform(sine_wave);

	while(1);
}

/* Función que se llama constantemente en un ciclo infinito */
void App_Run_1(void) {
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
	int16_t frame_decode_1[3000] = {0};
	int16_t frame_decode_2[3000] = {0};
	uint16_t frame_decode_3[3000] = {0};
	uint16_t frame_decode_4[3000] = {0};
	uint16_t frame_decode_5[3000] = {0};
	uint16_t frame_decode_6[3000] = {0};
	uint16_t frame_decode_7[3000] = {0};
	uint16_t frame_decode_8[3000] = {0};



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
	uint32_t i = 0;
	for(i = 0; i < 3000; i++){
		frame_decode_1[i] = frame_decode_1[i]/4;
	}
//	decoderGetFrame(frame_decode_2, &frameInfo);
//	decoderGetFrame(frame_decode_3, &frameInfo);
//	decoderGetFrame(frame_decode_4, &frameInfo);
//	decoderGetFrame(frame_decode_5, &frameInfo);
//	decoderGetFrame(frame_decode_6, &frameInfo);
//	decoderGetFrame(frame_decode_7, &frameInfo);
//	decoderGetFrame(frame_decode_8, &frameInfo);




	bool transfer_to_dac_1 = true;
	bool buffer_complete = false;
	audio_init(frameInfo.samprate, frame_decode_1, frame_decode_2, frameInfo.outputSamps, &transfer_to_dac_1 );
	gpioIRQ(PORTNUM2PIN(PA,4),GPIO_IRQ_MODE_FALLING_EDGE, playPause);
	audio_resume();
	bool transfer_to_dac_1_prev = transfer_to_dac_1;
	while(1){

		if(transfer_to_dac_1 && !buffer_complete)
		{
			if(decoderGetFrame(frame_decode_2, &frameInfo)){
				buffer_complete = true;
				for (int i = 0; i < 3000; i++) {
					//frame_decode_2[i] += 0x8000U;
					//frame_decode_2[i] *= (double)0xFFFU / 0xFFFFU;
					frame_decode_2[i] = (frame_decode_2[i]+32768)>>4;
				}
			}
			else{
				while(1);
			}
		}

		else if(!transfer_to_dac_1 && !buffer_complete)
		{
			if(decoderGetFrame(frame_decode_1, &frameInfo)){
				buffer_complete = true;
				//for(i = 0; i < 3000; i++){
				//	frame_decode_1[i] = frame_decode_1[i]/16 + 8192;
				//}
				for (int i = 0; i < 3000; i++) {
					//frame_decode_1[i] += 0x8000U;
					//frame_decode_1[i] *= (double)0xFFFU / 0xFFFFU;
					frame_decode_1[i] = (frame_decode_1[i]+32768)>>4;
				}
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


