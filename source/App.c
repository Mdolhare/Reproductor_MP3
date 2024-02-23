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

static uint16_t data[8] = { 10 ,20,30,40,50,20,30,50};

static uint32_t data2[16];

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


}

/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void) {

	gpioMode(PTB9, OUTPUT);

	gpioWrite(PTB9, HIGH);

	gpioMode(PIN_LED_RED, OUTPUT);

	gpioWrite(PIN_LED_RED, HIGH);

	//FTM_Init();

    PWM_init(PWM_PTC1, &pwmConfig);

    uint32_t* cnv_ptr;

    cnv_ptr = PWM_getAdressCnVDMA(PWM_PTC1);

    dma_cfg_t dma_cfg = {2, 0, dma16BIT, dma16BIT, 2, (sizeof(data)/sizeof(data[0])), sizeof(data), 0, 0, 0};

    DMA_init(0, dmaFTM0_0, false, data, cnv_ptr, dma_cfg);

    //hw_EnableInterrupts();
    //(PWM_getAdressCnVDMA(PWM_PTC1))

    for EVER {
    	int a = 0;
    	a++;
    }

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
