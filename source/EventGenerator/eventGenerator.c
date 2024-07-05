
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "../Queue/cola.h"
#include "../Drivers/MCAL/Gpio/gpio.h"
#include "../FSM/fsmTable.h"
#include <stdint.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
static void new_mov_event(void);
static void new_enter_event(void);

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/


/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
static cola_t event_queue;
static uint8_t row_index;

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void EG_init(void){
	row_index = 0;
	colaInit(&event_queue);

	gpioMode(PORTNUM2PIN(PA,4), INPUT);
	gpioIRQ(PORTNUM2PIN(PA,4),GPIO_IRQ_MODE_FALLING_EDGE, new_mov_event);

	gpioMode(PORTNUM2PIN(PC,6), INPUT);
	gpioIRQ(PORTNUM2PIN(PC,6),GPIO_IRQ_MODE_FALLING_EDGE, new_enter_event);

}

events_t EG_getEvent(void){
	return colaPull(&event_queue);
}

bool EG_isNewEvent(void){
	return event_queue.count;
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
static void new_mov_event(void){
	row_index++;
	row_index &= 0b11;
	colaPush(&event_queue, MOV_ARRIBA);
}

static void new_enter_event(void){
	colaPush(&event_queue, row_index);
}


