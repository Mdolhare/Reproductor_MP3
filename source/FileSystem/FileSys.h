/*
 * FileSys.h
 *
 *  Created on: Apr 1, 2024
 *      Author: Grupo 2
 */

#ifndef FILESYS_H_
#define FILESYS_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>

#include "FAT/ff.h"
//#include "node.h"
#include "../Drivers/HAL/SD/SD.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
//#define NAMELENGTH 12+1    	//sale de FILINFO
//#define PATHLENGHT 50		//lo mismo para este

#define CANTSONGS_MAX	50

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/



/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
void InitFileSys(void);

bool mountSD(void);

void readSD(void);

char * getPath(int _i);

int getCantSongs(void);


/*******************************************************************************
 ******************************************************************************/




#endif /* FILESYS_H_ */
