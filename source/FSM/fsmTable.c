/*
 * fsmTable.c
 *
 *  Created on: Mar 5, 2024
 *      Author: Grupo 2
 */

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "fsmTable.h"
#include "../Drivers/HAL/SD/SD.h"
#include "../PlayMusic/playMusic.h"
#include "../Drivers/MCAL/Gpio/gpio.h"

#include <stdint.h>



/*******************************************************************************
 * Foward declarations
 ******************************************************************************/
extern state_edge_t insertar_SD[];
extern state_edge_t sleep[];
extern state_edge_t menu[];
extern state_edge_t volumen[];
extern state_edge_t ecualizador[];
extern state_edge_t canciones[];
extern state_edge_t pausa_play[];

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
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
//---------------------------Rutinas globales
static void do_nothing(void);
static void do_nothing_1(void);
static void do_nothing_2(void);
static void do_nothing_3(void);
static void do_nothing_SIN_SD(void);

static void move_up_row(void);
static void move_down_row(void);

//---------------------------Rutinas estado init
static void estado_init_config_SD(void);

//---------------------------Rutinas sleep
static void go_sleep(void);

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/
state_edge_t estado_init[] = {
	{POSICION_1, estado_init, do_nothing},
	{POSICION_2, estado_init, do_nothing},
	{POSICION_3, estado_init, do_nothing},
	{POSICION_4, estado_init, do_nothing},
	{MOV_ABAJO, estado_init, do_nothing},
	{MOV_ARRIBA, estado_init, do_nothing},
	{HAY_SD, menu, estado_init_config_SD},
	{NO_HAY_SD, estado_init, do_nothing_SIN_SD},
	{TIME_OUT_SD, sleep, go_sleep},
	{DEFAULT, estado_init, do_nothing}
};

state_edge_t sleep[] = {
	{START, estado_init, do_nothing},
	{DEFAULT, estado_init, do_nothing}
};

state_edge_t menu[] = {
	{POSICION_1, menu, do_nothing_1},//canciones
	{POSICION_2, menu, do_nothing_2},//volumne
	{POSICION_3, menu, do_nothing_3},//ecualizador
	{POSICION_4, menu, playMusicPause},
	{MOV_ABAJO, menu, move_down_row},
	{MOV_ARRIBA, menu, move_up_row},
	{APAGAR, sleep, do_nothing},
	{NO_HAY_SD, estado_init, do_nothing_SIN_SD},
	{DEFAULT, estado_init, do_nothing}
};

state_edge_t pausa_play[] = {
	{NADA, menu, do_nothing},
	{DEFAULT, estado_init, do_nothing}
};

state_edge_t volumen[] = {
	{POSICION_1, menu, do_nothing},
	{POSICION_2, menu, do_nothing},
	{POSICION_3, menu, do_nothing},
	{POSICION_4, menu, do_nothing},
	{MOV_ABAJO, volumen, do_nothing},
	{MOV_ARRIBA, volumen, do_nothing},
	{NO_HAY_SD, estado_init, do_nothing_SIN_SD},
	{DEFAULT, estado_init, do_nothing}

};

state_edge_t ecualizador[] = {
	//n posiciones segun numero de efectos posibles
	{POSICION_1, ecualizador, do_nothing},
	{POSICION_2, ecualizador, do_nothing},
	{POSICION_3, ecualizador, do_nothing},
	{POSICION_4, menu, do_nothing},
	{MOV_ABAJO, ecualizador, do_nothing},
	{MOV_ARRIBA, ecualizador, do_nothing},
	{NO_HAY_SD, estado_init, do_nothing_SIN_SD},
	{DEFAULT, estado_init, do_nothing}


};

state_edge_t canciones[] = {
	{POSICION_1, menu, do_nothing},
	{POSICION_2, menu, do_nothing},
	{POSICION_3, menu, do_nothing},
	{POSICION_4, menu, do_nothing},			//volver al menu
	{MOV_ABAJO, canciones, do_nothing},
	{MOV_ARRIBA, canciones, do_nothing},
	{NO_HAY_SD, estado_init, do_nothing_SIN_SD},
	{DEFAULT, estado_init, do_nothing}


};

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
static uint8_t row_index;

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

state_t FSM_GetInitState(void) {
 	return estado_init;
}


/*******************************************************************************
 *******************************************************************************
                        		Rutinas
 *******************************************************************************
 ******************************************************************************/
//Rutinas globales entre estados
static void do_nothing(void){}

static void do_nothing_SIN_SD(void){
	//reset driver SD
	SD_reset();
	//mostrar que se inserte SD

	//Parar DAC
	audio_pause();
}

static void do_nothing_1(void){
	gpioWrite(LED_R, LOW);
	gpioWrite(LED_B, HIGH);
	gpioWrite(LED_G, HIGH);

}
static void do_nothing_2(void){
	gpioWrite(LED_R, HIGH);
	gpioWrite(LED_B, LOW);
	gpioWrite(LED_G, HIGH);

}
static void do_nothing_3(void){
	gpioWrite(LED_R, HIGH);
	gpioWrite(LED_B, HIGH);
	gpioWrite(LED_G, LOW);

}




static void move_up_row(){
	row_index++;
	row_index &= 0b11;
}

static void move_down_row(){
	row_index--;
	row_index &= 0b11;
}


//-----------------------------Rutinas estado_init
static void estado_init_config_SD(){
	//Inicializar nueva SD

	SD_cardStatus SDstatus;

	if(SDstatus != SD_OK && SDstatus != SD_RDY){
		bool ok = SD_initializationProcess();
		if(!ok){
			SDstatus = SD_getStatus();
			//tratar error
			while(1);
		}
		playMusicInit();
	}
}


//-----------------------------Rutinas sleep
static void go_sleep(void){
	//mandar a sleep los modulos

}

//-----------------------------Rutinas menu




/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
