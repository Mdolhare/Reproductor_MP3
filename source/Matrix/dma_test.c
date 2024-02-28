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

static uint16_t data2[8] = {100,100,100,200,300,100,100,100};

static uint32_t data3[18];

static TCD_t tcd[2] __attribute__((aligned (32)));

static tcd_cfg_t configs[2];

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

static void delayLoop(uint32_t veces);

static void callback(void);

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/* Función que se llama 1 vez, al comienzo del programa */
void App_Init2 (void) {

}

/* Función que se llama constantemente en un ciclo infinito */
void App_Run2 (void) {

	gpioMode (PTB9, OUTPUT);
	gpioWrite (PTB9, 0);

	pitInit();

	configs[0].source=(uint32_t)data;
	configs[0].destination=(uint32_t)data3;
	configs[0].sOffset=2;
	configs[0].dOffset=4;
	configs[0].sTransferSize=dma16BIT;
	configs[0].dTransferSize=dma16BIT;
	configs[0].byteAmount=2;
	configs[0].minorLoopIter=sizeof(data)/sizeof(data[0]);
	configs[0].sShiftBack=sizeof(data);
	configs[0].dShiftBack=sizeof(data)*2;
	configs[0].sCircBuff=0;
	configs[0].dCircBuff=0;

	configs[1].source=(uint32_t)data2;
	configs[1].destination=(uint32_t)(&data3[8]);
	configs[1].sOffset=2;
	configs[1].dOffset=4;
	configs[1].sTransferSize=dma16BIT;
	configs[1].dTransferSize=dma16BIT;
	configs[1].byteAmount=2;
	configs[1].minorLoopIter=sizeof(data2)/sizeof(data2[0]);
	configs[1].sShiftBack=0;
	//configs[1].dShiftBack=(uint32_t)&tcd[0];
	configs[1].sCircBuff=0;
	configs[1].dCircBuff=0;

	//dma_config_scatter_gather(configs, tcd, 2);	//MODO SCATTER AND GATHER

	dma_config(configs[0], &(tcd[0]));	//USO NORMAL

	dma_add_irq(dmaCHANNEL0, callback, &(tcd[0]), false, true);

	dma_begin(dmaCHANNEL0, dmaDMA_MUX_5, true, &(tcd[0]));

	pitSetAndBegin(PIT_0, 100);


    for EVER;

}



/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static void callback(void) {
	gpioToggle(PTB9);
}


static void delayLoop(uint32_t veces)
{
    while (veces--);
}


/*******************************************************************************
 ******************************************************************************/
