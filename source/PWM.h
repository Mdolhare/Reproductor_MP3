/***************************************************************************//**
  @file     PWM.h
  @brief    Driver de PWM
  @author   Grupo 2
 ******************************************************************************/

#ifndef PWM_H_
#define PWM_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "port.h"
#include "gpio.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define PERIODO_NS 1250 //nanoseg

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef void (*PWM_Fun_t)(void);

typedef enum{
	PWM_PTA1 = PORTNUM2PIN(PA,1),
	PWM_PTA2 = PORTNUM2PIN(PA,2),
	PWM_PTC2 = PORTNUM2PIN(PC,2),
	PWM_PTC3 = PORTNUM2PIN(PC,3),
	PWM_PTC4 = PORTNUM2PIN(PC,2)
}PWM_pin_t;

typedef struct{
	uint8_t ftm_id;
	uint8_t channel;
}PWM_pinID_t;

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

void PWM_init(PWM_pin_t pin, PWM_pinID_t * pin_ID);

uint32_t * PWM_getAdressCnVDMA(PWM_pin_t pin);

void PWM_setDutyPin(PWM_pin_t pin, float duty);

void PWM_setDutyPinWithCnV(PWM_pinID_t * pin_ID, uint16_t cnv);

float PWM_getMinDuty();

uint16_t PWM_getCnVFromDuty(float duty);

void PWM_initIRQ(PWM_pin_t pin, PWM_Fun_t fun);

uint16_t PWM_scaleValueToCnVDuty(PWM_pinID_t * pin_ID, uint16_t maxVal, uint16_t value);


#endif /* PWM_H_ */
