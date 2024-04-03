/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Grupo 2
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>

#include "gpio.h"
#include "board.h"
#include "pit.h"
#include "dma.h"
#include "DAC.h"
#include "ModuladorFSK.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define EVER (;;)

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/
uint32_t fs = 100000;

uint16_t sourceBuffer[83];

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/


/*******************************************************************************
 *******************************************************************************
 *                      GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 *******************************************************************************/



/*

void callback(void) {
	static uint8_t data = 0;
	modulador_cpfsk(data, 1200, 2200, 1200, 100000, sourceBuffer);
	data = !data;
}


void App_Init (void) {

	gpioMode(PIN_LED_BLUE, OUTPUT);
	gpioWrite(PIN_LED_BLUE, 1);

	gpioMode(PIN_LED_GREEN, OUTPUT);
	gpioWrite(PIN_LED_GREEN, 1);

	DAC_Init(true);
}



void App_Run (void) {

	uint32_t T = 1000000/fs;

	pitInit();
	pitSetAndBegin(PIT0, T);

	dma_cfg_t config = {2, 0, dma16BIT, 2, sizeof(sourceBuffer)/sizeof(sourceBuffer)[0],
			sizeof(sourceBuffer), 0, 0, 0};

	uint8_t* direc = getPtrToDat();

	DMA_init(0, dmaDMA_MUX_5, true, sourceBuffer, direc, config);

	dma_add_irq(0, callback);

	for EVER {

	}
}
*/

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 *******************************************************************************/
