/***************************************************************************//**
  @file     dma.h
  @brief    Driver DMA
  @author   Grupo 2
 ******************************************************************************/

#ifndef _CMP_H_
#define _CMP_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include "../Port/port.h"
#include "../Gpio/gpio.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
enum{
	level0,
	level1,
	level2,
	level3
};

typedef enum{
	IN0,
	IN1,
	IN2,
	IN3,
	IN4,
	IN5,
	IN6,
	IN7
} INn_t;

typedef enum {
	CMP_0,
	CMP_1,
	CMP_2
} CMPn_t;

typedef enum
{
	PTC7_CMP0_IN1 = PORTNUM2PIN(PC,7),
	PTC8_CMP0_IN2 = PORTNUM2PIN(PC,8),
	PTC9_CMP0_IN3 = PORTNUM2PIN(PC,9),
	PTC2_CMP1_IN0 = PORTNUM2PIN(PC,2),
	PTC3_CMP1_IN1 = PORTNUM2PIN(PC,3),
} CMP_Pin_t;

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
typedef CMP_Type * CMP_t;


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
void CMP_init(CMPn_t cmp_id, INn_t in, CMP_Pin_t pin);

bool CMP_get_cout(void);

/*******************************************************************************
 ******************************************************************************/

#endif // _CMP_H_