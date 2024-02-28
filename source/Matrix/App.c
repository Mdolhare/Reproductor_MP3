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
#define DUTY_ONE  40
#define DUTY_ZERO 20
#define EVER (;;)

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static PWM_pinID_t pwmConfig;



static uint16_t data[73] =	{DUTY_ONE,DUTY_ONE,DUTY_ONE,DUTY_ONE,DUTY_ONE,DUTY_ONE,DUTY_ONE,DUTY_ONE,DUTY_ONE,
		DUTY_ZERO,DUTY_ZERO,DUTY_ZERO,DUTY_ZERO,DUTY_ZERO,DUTY_ZERO,DUTY_ZERO,DUTY_ZERO,
		DUTY_ONE,DUTY_ONE,DUTY_ONE,DUTY_ONE,DUTY_ONE,DUTY_ONE,DUTY_ONE,DUTY_ONE,
		DUTY_ONE,DUTY_ONE,DUTY_ONE,DUTY_ONE,DUTY_ONE,DUTY_ONE,DUTY_ONE,DUTY_ONE,
		DUTY_ZERO,DUTY_ZERO,DUTY_ZERO,DUTY_ZERO,DUTY_ZERO,DUTY_ZERO,DUTY_ZERO,DUTY_ZERO,
		DUTY_ONE,DUTY_ONE,DUTY_ONE,DUTY_ONE,DUTY_ONE,DUTY_ONE,DUTY_ONE,DUTY_ONE,
		DUTY_ONE,DUTY_ONE,DUTY_ONE,DUTY_ONE,DUTY_ONE,DUTY_ONE,DUTY_ONE,DUTY_ONE,
		DUTY_ZERO,DUTY_ZERO,DUTY_ZERO,DUTY_ZERO,DUTY_ZERO,DUTY_ZERO,DUTY_ZERO,DUTY_ZERO,
		DUTY_ONE,DUTY_ONE,DUTY_ONE,DUTY_ONE,DUTY_ONE,DUTY_ONE,DUTY_ONE,DUTY_ONE};



//DUTY_ZERO,DUTY_ZERO,DUTY_ZERO,DUTY_ZERO,DUTY_ZERO,DUTY_ZERO,DUTY_ZERO,DUTY_ZERO,





static uint16_t data2[8] = {1,1,1,1,1,1,1,1};

static uint16_t data3[24];

static TCD_t tcd;
static bool dma_first = true;

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

static void delayLoop(uint32_t veces);

static void reset_start();
static void reset_stop();

static void dma_start();
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



	if(dma_first)
	{

	dma_config(dma_cfg, &tcd);

	dma_begin(dmaCHANNEL0,  dmaFTM0_0, false, &tcd);

	dma_add_irq(dmaCHANNEL0, reset_start, &tcd, false, true);

	pitSetIRQFunc(PIT_0, reset_stop);

	}

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

		while(!FTM_IsInterruptPending(FTM_0, 0))
			{

			}
			FTM0->OUTMASK |= FTM_OUTMASK_CH0OM(1);

				FTM_StopClock(FTM0);

				pitSetAndBegin(PIT_0, 55);


}

static void dma_start()
{
	dma_first = true;
	pitDisable(PIT_1);
}

static void  reset_stop(){

	pitDisable(PIT_0);

	FTM_StartClock(FTM0);

	while(!FTM_IsInterruptPending(FTM_0, 0))
	{

	}
	FTM0->OUTMASK &= ~FTM_OUTMASK_CH0OM(1);


}
/*******************************************************************************
 ******************************************************************************/
