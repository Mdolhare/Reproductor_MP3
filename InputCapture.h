/***************************************************************************//**
  @file     PWM.h
  @brief    Driver de PWM
  @author   Grupo 2
 ******************************************************************************/

#ifndef INPUTCAPTURE_H_
#define INPUTCAPTURE_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "port.h"
#include <stdbool.h>
#include <stdint.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define CANT_IC_PIN 3

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef void (*IC_Fun_t)(uint8_t);

typedef enum
{
	IC_riseEdge,
	IC_fallEdge,
	IC_bothEdge,
	IC_DualEdge_pulso,
	IC_DualEdge_periodo,
} ICMode_t;

typedef enum
{
	PTD0_IC_0 = PORTNUM2PIN(PD,0),
	PTD1_IC_1 = PORTNUM2PIN(PD,1),
	CMP0_IC_2 = 0xFF, //FTM2_CH0
	CMP1_IC_3 = 0xFE, //FTM2_CH0
} IC_Pin_t;

typedef struct{
	uint8_t ftm_id;
	uint8_t channel;
}IC_pinID_t;



/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
void IC_init(IC_Pin_t pin, ICMode_t mode, IC_pinID_t * pin_id);

void IC_setEnableDMARequest(IC_pinID_t * pin_id);

uint32_t * getAdressCnVDMA(IC_pinID_t * pin_id);

bool IC_isNewPeriod(IC_pinID_t * pin_id);

uint16_t IC_getPeriod(IC_pinID_t * pin_id);




#endif /* INPUTCAPTURE_H_ */
