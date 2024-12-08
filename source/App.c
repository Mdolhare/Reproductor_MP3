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
#include <stdio.h>



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
#include "Drivers/MCAL/I2C/i2c.h"
#include "Drivers/HAL/display/displayLCD.h"

static void showVolumen(void);
static void dibujarBarra(int nivel);
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
FATFS fsobj;
FRESULT fr;     /* Return value */
DIR dj;         /* Directory object */
FILINFO fno;    /* File information */

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
static i2c_cfg_t cfg;
static bool flag_v1 = false;
/*
static uint8_t nivel1[8] = {0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000};
static uint8_t nivel2[8] = {0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000};
static uint8_t nivel3[8] = {0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100};
static uint8_t nivel4[8] = {0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110};
static uint8_t nivel5[8] = {0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111};

enum vol_bars{
	N1,
	N2,
	N3,
	N4,
	N5
};
*/

//strncpy(line1, lcd_chars, 40);         // Copiar los primeros 40 caracteres
//strncpy(line2, lcd_chars + 40, 40);   // Copiar los siguientes 40 caracteres

void callback();

/*******************************************************************************
 *******************************************************************************
 *                      GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 *******************************************************************************/
void pitFunc(void);
void playPause(void);

void App_Init (void) {

	gpioMode(PORTNUM2PIN(PC,3), OUTPUT);
	gpioWrite(PORTNUM2PIN(PC,3), LOW);


	//Debug con leds
	gpioMode(LED_R, OUTPUT);
	gpioMode(LED_B, OUTPUT);
	gpioMode(LED_G, OUTPUT);
	gpioWrite(LED_R, HIGH);
	gpioWrite(LED_B, HIGH);
	gpioWrite(LED_G, HIGH);

	gpioMode(PORTNUM2PIN(PA,4), INPUT);

	gpioMode(PORTNUM2PIN(PC,6), INPUT);
	//-
	SD_init();
	EG_init();
	pitInit();
	displayLCD_Init(0x27);
}

void App_Init_1 (void) {
	displayLCD_Init(0x27);

    gpioMode(PORTNUM2PIN(PA,4), INPUT);
    gpioIRQ(PORTNUM2PIN(PA,4),GPIO_IRQ_MODE_FALLING_EDGE, callback);


}

void callback(void) {
	flag_v1 = true;
	// LCD_begin();
    //LCD_display();
 //   LCD_backlight();

}

void App_Run_7 (void) {
static int i = 0;
	if(flag_v1){
		if( i == 0){
			displayLCD_Begin();

			i++;
			//displayLCD_ShowCursorAt(19,0);
			//displayLCD_ShowStringLine("Hello Word 0", 0);
			//displayLCD_ShowStringLine("Hello Word 1", 1);
			//displayLCD_ShowStringLine("Hello Word 2", 2);
			//displayLCD_ShowStringLine("Hello Word 3", 3);

			//create chars
/*			displayLCD_createChar(0, nivel1);
			displayLCD_createChar(1, nivel2);
			displayLCD_createChar(2, nivel3);
			displayLCD_createChar(3, nivel4);
			displayLCD_createChar(4, nivel5);
*/
			showVolumen();

		}


		if(i<31){
			dibujarBarra(i-1);
			i++;
		}
		else{
			i=1;
		}
		//DISP_SetCursor(9,1);
		//DISP_WriteChar(line1[i]);
		flag_v1 = false;
	}


}

void App_Run(){
	bool ok=false;
	//EG_addEvent(NONE);
	//EG_addEvent(HAY_SD);
	state_t state = FSM_GetInitState();
	uint8_t ev = 0;
	bool hay_sd = false;
	while(1){
		if(EG_isNewEvent()){
			ev = EG_getEvent();
			state = fsm(state,ev);
		}
		if(SD_getStatus() == SD_OK){
			playMusic();
		}
	}


/*
 * TEST READ FILES
	while(1){
		if(SD_isSDcard() ){
			if(ok==false){
				ok = SD_initializationProcess();
			}
			bool isElement = true;
			f_mount(&fsobj,"",0);
			FILINFO archivos[100] = {0};
			uint8_t i = 1;
			fr = f_findfirst(&dj, &(archivos[0]), "\0ir", "*.*");
			do{
				if(archivos[i].fname[0] == 0)
					isElement = false;
				else
					fr = f_findnext(&dj, &(archivos[i++]));

			}while(fr == FR_OK && isElement);
			while(1);

		}
		else{
			SD_reset();
			ok = false;
		}
	}

*/

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
