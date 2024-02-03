/***************************************************************************//**
  @file     dma.h
  @brief    Driver DMA
  @author   Grupo 2
 ******************************************************************************/

#ifndef _DMA_H_
#define _DMA_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>



/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

typedef void (*dmaIrqFun_t)(void);


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef struct {
	uint16_t sOffset;
	uint16_t dOffset;
	uint16_t transferSize;
	uint16_t byteAmount;
	uint16_t minorLoopIter;
	uint16_t sShiftBack;
	uint16_t dShiftBack;
	uint16_t sCircBuff;
	uint16_t dCircBuff;
} dma_cfg_t;


enum DMA_CHANNELS {
	dmaDISABLED,
	dmaUART0_RX=2,
	dmaUART0_TX,
	dmaUART1_RX,
	dmaUART1_TX,
	dmaUART2_RX,
	dmaUART2_TX,
	dmaUART3_RX,
	dmaUART3_TX,
	dmaUART4,
	dmaUART5,
	dmaI2S0_RX,
	dmaI2S0_TX,
	dmaSPI0_RX,
	dmaSPI0_TX,
	dmaSPI1,
	dmaSPI2,
	dmaI2C0,
	dmaI2C1_2,
	dmaFTM0_0,
	dmaFTM0_1,
	dmaFTM0_2,
	dmaFTM0_3,
	dmaFTM0_4,
	dmaFTM0_5,
	dmaFTM0_6,
	dmaFTM0_7,
	dmaFTM1_0,
	dmaFTM1_1,
	dmaFTM2_0,
	dmaFTM2_1,
	dmaFTM3_0,
	dmaFTM3_1,
	dmaFTM3_2,
	dmaFTM3_3,
	dmaFTM3_4,
	dmaFTM3_5,
	dmaFTM3_6,
	dmaFTM3_7,
	dmaADC0,
	dmaADC1,
	dmaCMP0,
	dmaCMP1,
	dmaCMP2,
	dmaDAC0,
	dmaDAC1,
	dmaCMT,
	dmaPDB,
	dmaPORT_A,
	dmaPORT_B,
	dmaPORT_C,
	dmaPORT_D,
	dmaPORT_E,
	dmaDMA_MUX_0=58,
	dmaDMA_MUX_1,
	dmaDMA_MUX_2,
	dmaDMA_MUX_3,
	dmaDMA_MUX_4,
	dmaDMA_MUX_5,
};

enum DMA_BIT_TRANSFER_AMOUNT {
	dma8BIT,
	dma16BIT,
	dma32BIT,
	dma16BYTE=4,
	dma32BYTE,
};

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/



/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
void DMA_init(uint32_t channel, uint32_t source,
		bool timer, uint16_t* sourceBuffer, uint8_t* destinationBuffer,
		dma_cfg_t config);

void dma_set_jump(uint8_t channel, uint8_t sOffset);

void dma_add_irq(uint8_t channel, dmaIrqFun_t callback);

/*******************************************************************************
 ******************************************************************************/

#endif // _DMA_H_
