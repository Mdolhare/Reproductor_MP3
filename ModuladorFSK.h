	/***************************************************************************//**
  @file     ModuladorFSK.h
  @brief    Modulador FSK
  @author   Grupo 2
 ******************************************************************************/
#ifndef MODULADORFSK_H_
#define MODULADORFSK_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdint.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef struct{
	uint32_t f_cero; //mayor frecuencia
	uint32_t f_uno;  //menor frecuencia
	uint8_t * bitStream;
	uint8_t numBit;
	uint32_t fs;
	float bitTime;
}ModFSK_Data_t;


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

void modulador_cpfsk(uint8_t bit, uint32_t f1, uint32_t f2,
		uint32_t fb, uint32_t fs, uint16_t* outArray, uint16_t * sen_base) ;




#endif /* MODULADORFSK_H_ */
