/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Grupo 2
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "Drivers/MCAL/DMA/dma.h"
#include "Drivers/MCAL/PIT/pit.h"
#include "Drivers/MCAL/DAC/DAC.h"
#include "sen.h"
#include "sen2.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define EVER (;;)

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static TCD_t tcd[2] __attribute__((aligned(32)));
static tcd_cfg_t configs[2];

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (void) {

}

/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void) {

	DAC_Init(true);
	pitInit();

	uint8_t* destination = DAC_getPtrToDat();

	configs[0].source = (uint32_t)sen;
	configs[0].destination = (uint32_t)destination;
	configs[0].sOffset = 2;
	configs[0].dOffset = 0;
	configs[0].sTransferSize = dma16BIT;
	configs[0].dTransferSize = dma16BIT;
	configs[0].byteAmount = 2;
	configs[0].minorLoopIter = sizeof(sen)/sizeof(sen[0]);
	configs[0].sShiftBack = 0;
	configs[0].dShiftBack = (uint32_t)&tcd[1];
	configs[0].sCircBuff = 0;
	configs[0].dCircBuff = 0;

	configs[1].source = (uint32_t)sen2;
	configs[1].destination = (uint32_t)destination;
	configs[1].sOffset = 2;
	configs[1].dOffset = 0;
	configs[1].sTransferSize = dma16BIT;
	configs[1].dTransferSize = dma16BIT;
	configs[1].byteAmount = 2;
	configs[1].minorLoopIter = sizeof(sen2)/sizeof(sen2[0]);
	configs[1].sShiftBack = 0;
	configs[1].dShiftBack = (uint32_t)&tcd[0];
	configs[1].sCircBuff = 0;
	configs[1].dCircBuff = 0;

	dma_config_scatter_gather(configs, tcd, 2);

	//dma_add_irq(dmaCHANNEL0, flagHandler, &(tcd[0]), false, true);
	//dma_add_irq(dmaCHANNEL0, flagHandler, &(tcd[1]), false, true);

	dma_begin(dmaCHANNEL0, dmaDMA_MUX_5, true, &(tcd[0]));

	pitSetAndBegin(PIT_0, 1000);

	while(true);
}



/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/



/*******************************************************************************
 ******************************************************************************/
