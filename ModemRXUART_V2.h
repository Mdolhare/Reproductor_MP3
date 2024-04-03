/***************************************************************************//**
  @file     ModemRxUART_V2.h
  @brief    modulo para mandar recibir datos tipo uart
  @author   Grupo 2
 ******************************************************************************/

#ifndef MODEMRXUART_V2_H_
#define MODEMRXUART_V2_H_
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdbool.h>
#include <stdint.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define DATA_PAR(data) ()

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
void modemRxUart_Init();

bool modemRxUart_isNewData();

uint16_t modemRxUart_getData();

uint16_t modemRxUart_getParity();



#endif /* MODEMRXUART_V2_H_ */
