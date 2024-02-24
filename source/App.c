/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Grupo 2
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "board.h"
#include "gpio.h"
#include "pit.h"
#include "PWM.h"
#include "dma.h"
//#include "FTM.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define EVER (;;)

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static PWM_pinID_t pwmConfig;

static uint16_t data[8] = {10,20,30,40,50,20,30,50};

static uint16_t data2[8] = {100,100,100,100,100,100,100,100};

static uint32_t data3[16];

static TCD_t tcd[2] __attribute__((aligned (32)));

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

static void delayLoop(uint32_t veces);


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (void) {

	tcd[0].SADDR = (uint32_t)data;
	tcd[0].SOFF = 2;
	tcd[0].ATTR = DMA_ATTR_SSIZE(dma16BIT) | DMA_ATTR_DSIZE(dma16BIT)
					| DMA_ATTR_DMOD(0) | DMA_ATTR_SMOD(0);
	tcd[0].NBYTES_MLNO = 2;
	tcd[0].SLAST = 0;
	tcd[0].DADDR = (uint32_t)data3;
	tcd[0].DOFF = 2;
	tcd[0].CITER_ELINKNO = sizeof(data)/sizeof(data[0]);
	tcd[0].DLASTSGA = (uint32_t)&tcd[1];
	tcd[0].CSR = DMA_CSR_ESG_MASK | DMA_CSR_INTMAJOR_MASK;
	tcd[0].BITER_ELINKNO = sizeof(data)/sizeof(data[0]);

	tcd[1].SADDR = (uint32_t)data2;
	tcd[1].SOFF = 2;
	tcd[1].ATTR = DMA_ATTR_SSIZE(dma16BIT) | DMA_ATTR_DSIZE(dma16BIT)
					| DMA_ATTR_DMOD(0) | DMA_ATTR_SMOD(0);
	tcd[1].NBYTES_MLNO = 2;
	tcd[1].SLAST = 0;
	tcd[1].DADDR = (uint32_t)data3;
	tcd[1].DOFF = 2;
	tcd[1].CITER_ELINKNO = sizeof(data2)/sizeof(data2[0]);
	tcd[1].DLASTSGA = (uint32_t)&tcd[0];
	tcd[1].CSR = DMA_CSR_ESG_MASK | DMA_CSR_INTMAJOR_MASK;
	tcd[1].BITER_ELINKNO = sizeof(data2)/sizeof(data2[0]);

}

/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void) {

	DMA_init_tcd(0, dmaDMA_MUX_5, false, tcd[0]);

    for EVER;

}



/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static void delayLoop(uint32_t veces)
{
    while (veces--);
}


/*******************************************************************************
 ******************************************************************************/
