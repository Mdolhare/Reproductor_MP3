/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "../Queue/cola.h"
#include "../Drivers/MCAL/Gpio/gpio.h"
#include "../FSM/fsmTable.h"
#include "../Drivers/MCAL/Systick/SysTick.h"
#include "../Drivers/HAL/Encoder/Encoder.h"
#include "../Drivers/MCAL/SDHC/SDHC.h"
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
static void SDcard_event(void);
static void search_SDstate(void);

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

	EG_addEvent(NONE);

	DRV_Enc_Init();
	SysTick_Init();
	SysTick_Add(new_mov_event);
	SysTick_Add(new_enter_event);
//	SysTick_Add(search_SDstate);



	gpioIRQ(SDHC_SW_DETECT, GPIO_IRQ_MODE_BOTH_EDGES, SDcard_event);
	SDcard_event();
}

hw_events_t EG_getEvent(void){
	return colaPull(&event_queue);
}

bool EG_isNewEvent(void){
	return event_queue.count;
}

void EG_addEvent(uint8_t event){
	colaPush(&event_queue, event);
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
static void new_mov_event(void){
	int paso = get_paso();
	if(paso > 0){
		colaPush(&event_queue, MOV_ARRIBA);
	}
	else if(paso < 0 ){
		colaPush(&event_queue, MOV_ABAJO);
	}
}

static void new_enter_event(void){
	if(get_boton())
		colaPush(&event_queue, SELECT);
}

static void SDcard_event(){
	gpioDisableIRQ(SDHC_SW_DETECT);
	uint32_t i = 0xFFFFF;
	while(i--);
	if(SDHC_isCardDetected()){
		colaPush(&event_queue, HAY_SD);
	}
	else{
		colaPush(&event_queue, NO_HAY_SD);
	}
	gpioIRQ(SDHC_SW_DETECT, GPIO_IRQ_MODE_BOTH_EDGES, SDcard_event);
}

static void search_SDstate(){
	gpioToggle(PORTNUM2PIN(PC,3));
}

