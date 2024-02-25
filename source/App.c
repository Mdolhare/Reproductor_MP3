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
#include "FTMG2.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define EVER (;;)

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static PWM_pinID_t pwmConfig;

static uint16_t data[72] = {20,40,20,40,20,40,20,40,20,40,20,40,20,40,20,40,20,40,20,40,20,40,20,40,
		20,40,20,40,20,40,20,40,20,40,20,40,20,40,20,40,20,40,20,40,20,40,20,40,
		20,40,20,40,20,40,20,40,20,40,20,40,20,40,20,40,20,40,20,40,20,40,20,40};

static uint16_t data2[8] = {1,1,1,1,1,1,1,1};

static uint16_t data3[24];

static TCD_t tcd;


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

static void delayLoop(uint32_t veces);

static void reset_start();

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

	gpioMode(PORTNUM2PIN(PB,19), OUTPUT);

	PWM_init(PWM_PTC1, &pwmConfig);
	pitInit();
	uint32_t* cnv_ptr;
	cnv_ptr = PWM_getAdressCnVDMA(PWM_PTC1);

	tcd_cfg_t dma_cfg = {(uint32_t)data, (uint32_t)cnv_ptr, 2, 0,
			dma16BIT, dma16BIT, 2, (sizeof(data)/sizeof(data[0])), sizeof(data), 0, 0, 0};


	dma_config(dma_cfg, &tcd);

	dma_begin(dmaCHANNEL0,  dmaFTM0_0, false, &tcd);

	dma_add_irq(dmaCHANNEL0, reset_start, &tcd, false, true);

	pitSetIRQFunc(PIT_0, reset_start);

	//FTM_StopClock(FTM0);

    for EVER;

}



/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static void delayLoop(uint32_t veces) {
    while (veces--);
}


static void reset_start() {
	gpioToggle(PORTNUM2PIN(PB,19));
	static bool start = true;
	static int counter = 0;
	if(start) {
		FTM_StopClock(FTM0);
		counter = 0;
		start = false;
		pitSetAndBegin(PIT_0, 50);
	}
	else {
		FTM_StartClock(FTM0);
		start = true;
		pitDisable(PIT_0);
	}
}

/*******************************************************************************
 ******************************************************************************/
