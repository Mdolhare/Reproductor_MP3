/*
 * fsmTable.h
 *
 *  Created on: Mar 4, 2024
 *      Author: Grupo 2
 */

#ifndef FSMTABLE_H_
#define FSMTABLE_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
enum eventos{
	POSICION_1,
	POSICION_2,
	POSICION_3,
	POSICION_4,
	MOV_ABAJO,
	MOV_ARRIBA,
	VOLVER,
	HAY_SD,
	TIME_OUT_SD,
	START,
	APAGAR,
	NADA,
	FIN_TABLA
};

typedef	unsigned char byte_t;

typedef struct state_edge_s state_edge_t;

struct state_edge_s{
	byte_t event;
	state_edge_t* nextState;
	void (*actionPtr)(void);
};

typedef state_edge_t* state_t;

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

state_t FSM_GetInitState(void);

/*******************************************************************************
 ******************************************************************************/


#endif /* FSMTABLE_H_ */
