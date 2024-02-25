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

static TCD_t tcd[2] __attribute__((aligned (32)));

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

static void delayLoop(uint32_t veces);

void reset_start();

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (void) {

	/*tcd[0].SADDR = (uint32_t)data;
	tcd[0].SOFF = 2;
	tcd[0].ATTR = DMA_ATTR_SSIZE(dma16BIT) | DMA_ATTR_DSIZE(dma16BIT)
					| DMA_ATTR_DMOD(0) | DMA_ATTR_SMOD(0);
	tcd[0].NBYTES_MLNO = 2;
	tcd[0].SLAST = 0;
	tcd[0].DOFF = 0;
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
	tcd[1].DOFF = 0;
	tcd[1].CITER_ELINKNO = sizeof(data2)/sizeof(data2[0]);
	tcd[1].DLASTSGA = (uint32_t)&tcd[0];
	tcd[1].CSR = DMA_CSR_ESG_MASK | DMA_CSR_INTMAJOR_MASK;
	tcd[1].BITER_ELINKNO = sizeof(data2)/sizeof(data2[0]);
	tcd[0].DADDR = (uint32_t)cnv_ptr;
	tcd[1].DADDR = (uint32_t)(cnv_ptr);
	*/

}

/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void) {

	gpioMode(PORTNUM2PIN(PB,19), OUTPUT);

	PWM_init(PWM_PTC1, &pwmConfig);
	pitInit();
	uint32_t* cnv_ptr;
	cnv_ptr = PWM_getAdressCnVDMA(PWM_PTC1);

	dma_cfg_t dma_cfg = {2, 0, dma16BIT, dma16BIT, 2, (sizeof(data)/sizeof(data[0])), sizeof(data), 0, 0, 0};


	pitSetIRQFunc(PIT_0, reset_start);

	dma_add_irq(0, reset_start);
	DMA_init(0,  dmaFTM0_0, false, data, cnv_ptr,dma_cfg );

	//FTM_StopClock(FTM0);

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


void reset_start()
{
	gpioToggle(PORTNUM2PIN(PB,19));
	static bool start = true;
	static int counter = 0;
	if(start)
	{
			counter = 0;
			start = false;
			FTM_StopClock(FTM0);
			pitSetAndBegin(PIT_0, 50);



	}
	else
	{
		start = true;
		FTM_StartClock(FTM0);
		pitDisable(PIT_0);


	}
}

/*******************************************************************************
 ******************************************************************************/
