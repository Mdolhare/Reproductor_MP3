/***************************************************************************//**
  @file     board.h
  @brief    Board management
  @author   Nicolás Magliola
 ******************************************************************************/

#ifndef _BOARD_H_
#define _BOARD_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "../Gpio/gpio.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/***** BOARD defines **********************************************************/

// On Board User LEDs
#define PIN_LED_RED     PORTNUM2PIN(PB,22)// ???
#define PIN_LED_GREEN   PORTNUM2PIN(PE,26)// PTE26
#define PIN_LED_BLUE    PORTNUM2PIN(PB,21) // PTB21

#define LED_ACTIVE      LOW

#define PTB9 PORTNUM2PIN(PB,9)


// On Board User Switches

#define SW_ACTIVE       // ???
#define SW_INPUT_TYPE   // ???
#define DATA PORTNUM2PIN(PB,20)
#define CLK_IN PORTNUM2PIN(PC,1)
#define ENABLE PORTNUM2PIN(PC,8)


#define SWITCH2 PORTNUM2PIN(PC,6)
/*******************************************************************************
 ******************************************************************************/

#endif // _BOARD_H_
