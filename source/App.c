/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Nicolás Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "board.h"
#include "gpio.h"
#include "pit.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

static void delayLoop(uint32_t veces);
static bool high1;
void setFlag1H();

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (void)
{
	gpioMode (PORTNUM2PIN(PB,23), OUTPUT);

}
void setFlag1H()
{
	high1 = true;
}

/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void)
{
	gpioMode (PORTNUM2PIN(PE,26), OUTPUT);
	pitInit();
	while (1) {
		high1 = false;

		for(int i = 0; i<24; i++) {


			gpioWrite(PORTNUM2PIN(PE,26), HIGH);
			pitSetAndBegin( PIT_0, 10); //800n
			pitSetIRQFunc(PIT_0,  setFlag1H);

			while(!high1) {}

			high1 = false;
			gpioWrite(PORTNUM2PIN(PE,26), LOW);
			pitSetAndBegin( PIT_0, 20); //460n

			while(!high1) {}

			high1= false;

		}
		gpioWrite(PORTNUM2PIN(PE,26), LOW);

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
