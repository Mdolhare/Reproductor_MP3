/***************************************************************************//**
  @file     Decodificador.h
  @brief    Decodificador con FTM y CMP
  @author   Grupo 2
 ******************************************************************************/

#ifndef DECODIFICADOR_H_
#define DECODIFICADOR_H_
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdbool.h>
#include <stdint.h>
#include "port.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define CANT_BITS 11
#define T_CERO 440
#define T_UNO 820
#define T_BIT 833

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef enum{
	DEC_PTD0 = PORTNUM2PIN(PD,0),
	DEC_PTD1 = PORTNUM2PIN(PD,1),
	DEC_CMP0 = 0xFF,
	DEC_CMP1 = 0xFE,
}InputPin_t;


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
void Dec_Init(InputPin_t pin);

bool Dec_isNewFrame();

uint16_t Dec_getFrame();



#endif /* DECODIFICADOR_H_ */
