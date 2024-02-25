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
	uint32_t source;
	uint32_t destination;
	uint16_t sOffset;
	uint16_t dOffset;
	uint16_t sTransferSize;
	uint16_t dTransferSize;
	uint16_t byteAmount;
	uint16_t minorLoopIter;
	uint16_t sShiftBack;
	uint32_t dShiftBack;
	uint16_t sCircBuff;
	uint16_t dCircBuff;
} tcd_cfg_t;


typedef struct {
	uint32_t SADDR;
	uint16_t SOFF;
	uint16_t ATTR;
	union {
		uint32_t NBYTES_MLNO;
		uint32_t NBYTES_MLOFFNO;
		uint32_t NBYTES_MLOFFYES;
	};
	uint32_t SLAST;
	uint32_t DADDR;
	uint16_t DOFF;
	union {
		uint16_t CITER_ELINKNO;
		uint16_t CITER_ELINKYES;
	};
	uint32_t DLASTSGA;
	uint16_t CSR;
	union {
		uint16_t BITER_ELINKNO;
		uint16_t BITER_ELINKYES;
	};
}TCD_t;


typedef enum {
	dmaCHANNEL0,
	dmaCHANNEL1,
	dmaCHANNEL2,
	dmaCHANNEL3,
	dmaCHANNEL4,
	dmaCHANNEL5,
	dmaCHANNEL6,
	dmaCHANNEL7,
	dmaCHANNEL8,
	dmaCHANNEL9,
	dmaCHANNEL10,
	dmaCHANNEL11,
	dmaCHANNEL12,
	dmaCHANNEL13,
	dmaCHANNEL14,
	dmaCHANNEL15,
} dma_channels_t;



typedef enum {
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
} dma_sources_t;

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
void dma_config(tcd_cfg_t config, TCD_t* tcd);

void dma_config_scatter_gather(tcd_cfg_t* config, TCD_t* tcd, uint8_t amount);

void dma_begin(dma_channels_t channel, dma_sources_t source, bool timer, TCD_t* tcd);

void dma_set_jump(dma_channels_t channel, uint8_t sOffset);

void dma_add_irq(dma_channels_t channel, dmaIrqFun_t callback, TCD_t* tcd, bool halfMajor, bool fullMajor);

/*******************************************************************************
 ******************************************************************************/

#endif // _DMA_H_
