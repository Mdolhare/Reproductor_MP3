/*
 * equalizer.h
 *
 *  Created on: Apr 1, 2024
 *      Author: Grupo 2
 */

#ifndef EQUALIZER_H_
#define EQUALIZER_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/


/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/



/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

void equalizerInit(int8_t* gainDB);

void equalizerFilter(int32_t* inputBuffer, int32_t* outputBuffer, int32_t len);


/*******************************************************************************
 ******************************************************************************/




#endif /* EQUALIZER_H_ */
