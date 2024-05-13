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

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
static void do_nothing(void);
static void go_sleep(void);

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/
state_edge_t estado_init[] = {
	{HAY_SD, menu,do_nothing},
	{TIME_OUT_SD, sleep, go_sleep},
	{FIN_TABLA, estado_init, do_nothing}
};

state_edge_t sleep[] = {
	{START, estado_init, do_nothing},
	{FIN_TABLA, estado_init, do_nothing}
};

state_edge_t menu[] = {
	{POSICION_1, canciones, do_nothing},
	{POSICION_2, volumen, do_nothing},
	{POSICION_3, ecualizador, do_nothing},
	{POSICION_4, pausa_play, do_nothing},
	{MOV_ABAJO, menu, do_nothing},
	{MOV_ARRIBA, menu, do_nothing},
	{APAGAR, sleep, do_nothing},
	{FIN_TABLA, estado_init, do_nothing}
};


state_edge_t pausa_play[] = {
	{NADA, menu, do_nothing},
	{FIN_TABLA, estado_init, do_nothing}
};

state_edge_t volumen[] = {
	{POSICION_1, menu, do_nothing},
	{POSICION_2, menu, do_nothing},
	{POSICION_3, menu, do_nothing},
	{POSICION_4, menu, do_nothing},
	{MOV_ABAJO, volumen, do_nothing},
	{MOV_ARRIBA, volumen, do_nothing},
	{FIN_TABLA, estado_init, do_nothing}

};

state_edge_t ecualizador[] = {
	//n posiciones segun numero de efectos posibles
	{POSICION_1, ecualizador, do_nothing},
	{POSICION_2, ecualizador, do_nothing},
	{POSICION_3, ecualizador, do_nothing},
	{POSICION_4, ecualizador, do_nothing},
	{VOLVER, menu, do_nothing},
	{MOV_ABAJO, ecualizador, do_nothing},
	{MOV_ARRIBA, ecualizador, do_nothing},
	{FIN_TABLA, estado_init, do_nothing}


};

state_edge_t canciones[] = {
	{POSICION_1, menu, do_nothing},
	{POSICION_2, menu, do_nothing},
	{POSICION_3, menu, do_nothing},
	{VOLVER, menu, do_nothing},
	{MOV_ABAJO, canciones, do_nothing},
	{MOV_ARRIBA, canciones, do_nothing},
	{FIN_TABLA, estado_init, do_nothing}


};

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

state_t FSM_GetInitState(void) {

 	return estado_init;

}

//------------------------------Rutinas
void do_nothing(void){}

void go_sleep(void){
	//mandar a sleep los modulos

}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
