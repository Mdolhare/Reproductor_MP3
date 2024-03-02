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
#include "matrix.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define EVER (;;)

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/








//DUTY_ZERO,DUTY_ZERO,DUTY_ZERO,DUTY_ZERO,DUTY_ZERO,DUTY_ZERO,DUTY_ZERO,DUTY_ZERO,
//	DUTY_ONE,DUTY_ONE,DUTY_ONE,DUTY_ONE,DUTY_ONE,DUTY_ONE,DUTY_ONE,DUTY_ONE,




static uint16_t data2[8] = {1,1,1,1,1,1,1,1};

static uint16_t data3[24];


static bool dma_first = false;
static counter;
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
	for (int i = 0;  i<64; i++)
	{
		updateLED(i, 0b1110000,  0b00000000, 0b00000000);
	}
	 matrixInit(PWM_PTC1);


}

/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void) {


	for (int i = 0;  i<64; i++)
	{
		updateLED(i, 0b1110000,  0b00000000, 0b1110000);
	}
	delayLoop(1000000);

	//matrixInit(PWM_PTC1);
	//FTM_StopClock(FTM0);
	turnOffMatrix();
	delayLoop(1000000);


}



/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static void delayLoop(uint32_t veces) {
    while (veces--);
}


/*******************************************************************************
 ******************************************************************************/
