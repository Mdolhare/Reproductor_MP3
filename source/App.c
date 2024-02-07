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

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define EVER (;;)

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static PWM_pinID_t pwmConfig;

static uint16_t data[] = { 20 ,80,20,80,20,80,20,80};



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

	gpioMode(PIN_LED_GREEN, OUTPUT);

	gpioWrite(PIN_LED_GREEN, HIGH);


    PWM_init(PWM_PTC2, &pwmConfig);

    uint32_t* cnv_ptr;

    //cnv_ptr = PWM_getAdressCnVDMA(PWM_PTC2);

    dma_cfg_t dma_cfg = {2, 0, dma16BIT, 2, (sizeof(data)/sizeof(data[0])), sizeof(data), 0, 0, 0};

    DMA_init(0, dmaFTM0_1, false, data, PWM_getAdressCnVDMA(PWM_PTC2), dma_cfg);

    hw_EnableInterrupts();

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
