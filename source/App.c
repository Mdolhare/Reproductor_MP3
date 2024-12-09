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
#include "Equalizer/equalizer.h"
#include "Drivers/HAL/Matrix/matrix.h"


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

static void callback(void);

/*******************************************************************************
 *******************************************************************************
 *                      GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 *******************************************************************************/
void pitFunc(void);
void playPause(void);

/* Función que se llama 1 vez, al comienzo del programa */
void App_Init_2 (void) {

	uint8_t gains[5] = {1,2,3,4,5};

	matrixInit();
	equalizerInit(gains);

}

/* Función que se llama constantemente en un ciclo infinito */
/*
void App_Run_2 (void) {
	if(d_flag){
		delayMicroseconds();
		d_flag=false;
	}
}*/


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
			state = fsm(state,ev);
		}

		playMusic(30);
	}

}



void App_Run_2(){

	while(1);
}

/* Función que se llama constantemente en un ciclo infinito */


void pitFunc(void)
{
	int i;
}
/*
void callback(void) {
	d_flag = true;
}*/


