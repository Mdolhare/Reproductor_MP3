/***************************************************************************//**
  @file     SDHC.h
  @brief    Driver MCAL para SDHC
  @author   Grupo 2
 ******************************************************************************/
#ifndef SD_H_
#define SD_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdbool.h>
#include <stdint.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef enum {
	SD_INIT = 0,
	SD_NOTINIT,
	SD_OK,
	SD_ERROR,
	SD_RDY,
	SD_NOTRDY,
} SD_cardStatus;



/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
void SD_init();

bool SD_initializationProcess();

SD_cardStatus SD_getStatus();

bool SD_isSDcard();

bool SD_readSectors(uint32_t address, uint32_t numSector, uint32_t * data);

#endif /* SD_H_ */
