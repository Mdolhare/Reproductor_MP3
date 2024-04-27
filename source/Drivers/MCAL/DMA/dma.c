/*******************************************************************************
  @file     dma.c
  @brief    Driver para DMA
  @author   GRUPO 2
 ******************************************************************************/
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "MK64F12.h"
#include "../DMA/dma.h"
#include "hardware.h"
#include "../Board/board.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define DMA_CHANNEL_AMOUNT 16

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

static dmaIrqFun_t irqs[DMA_CHANNEL_AMOUNT];


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
static void dma_IQR_handler(dma_channels_t channel);


/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/


/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void dma_config(tcd_cfg_t config, TCD_t* tcd) {

	tcd->SADDR = config.source;
	tcd->SOFF = config.sOffset;
	tcd->ATTR = DMA_ATTR_SSIZE(config.sTransferSize) | DMA_ATTR_DSIZE(config.dTransferSize)
					| DMA_ATTR_DMOD(config.dCircBuff) | DMA_ATTR_SMOD(config.sCircBuff);
	tcd->NBYTES_MLNO = config.byteAmount;
	tcd->SLAST = -config.sShiftBack;
	tcd->DADDR = config.destination;
	tcd->DOFF = config.dOffset;
	tcd->CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(config.minorLoopIter);
	tcd->DLASTSGA = -config.dShiftBack;
	tcd->CSR = 0;
	tcd->BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(config.minorLoopIter);

}

void dma_config_scatter_gather(tcd_cfg_t* config, TCD_t* tcd, uint8_t amount) {

	for (uint8_t i = 0; i < amount; i++) {
		tcd[i].SADDR = config[i].source;
		tcd[i].SOFF = config[i].sOffset;
		tcd[i].ATTR = DMA_ATTR_SSIZE(config[i].sTransferSize) | DMA_ATTR_DSIZE(config[i].dTransferSize)
						| DMA_ATTR_DMOD(config[i].dCircBuff) | DMA_ATTR_SMOD(config[i].sCircBuff);
		tcd[i].NBYTES_MLNO = config[i].byteAmount;
		tcd[i].SLAST = 0;
		tcd[i].DADDR = config[i].destination;
		tcd[i].DOFF = config[i].dOffset;
		tcd[i].CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(config[i].minorLoopIter);
		if(i<amount-1)
			tcd[i].DLASTSGA = (uint32_t)(&(tcd[i+1]));
		else
			tcd[i].DLASTSGA = (uint32_t)(&(tcd[0]));
		tcd[i].CSR = DMA_CSR_ESG_MASK;
		tcd[i].BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(config[i].minorLoopIter);
	}

}


void dma_begin(dma_channels_t channel, dma_sources_t source, bool timer, TCD_t* tcd) {

	SIM->SCGC7 |= SIM_SCGC7_DMA_MASK;
	SIM->SCGC6 |= SIM_SCGC6_DMAMUX_MASK;

	if(timer)
		DMAMUX->CHCFG[channel] |= DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_TRIG_MASK | DMAMUX_CHCFG_SOURCE(source);
	else
		DMAMUX->CHCFG[channel] |= DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_SOURCE(source);

	NVIC_ClearPendingIRQ(channel);
	NVIC_EnableIRQ(channel);

	DMA0->TCD[channel].SADDR = tcd->SADDR;
	DMA0->TCD[channel].DADDR = tcd->DADDR;

	DMA0->TCD[channel].SOFF = tcd->SOFF;
	DMA0->TCD[channel].DOFF = tcd->DOFF;

	DMA0->TCD[channel].ATTR = tcd->ATTR;

	DMA0->TCD[channel].NBYTES_MLNO = tcd->NBYTES_MLNO;

	DMA0->TCD[channel].CITER_ELINKNO = tcd->CITER_ELINKNO;
	DMA0->TCD[channel].BITER_ELINKNO = tcd->BITER_ELINKNO;

	DMA0->TCD[channel].SLAST = tcd->SLAST;
	DMA0->TCD[channel].DLAST_SGA = tcd->DLASTSGA;

	DMA0->TCD[channel].CSR = tcd->CSR;


	DMA0->ERQ |= DMA_ERQ_ERQ0_MASK << channel;

}


void dma_set_jump(dma_channels_t channel, uint8_t sOffset) {
	DMA0->TCD[channel].SOFF = sOffset;
}



void dma_add_irq(dma_channels_t channel, dmaIrqFun_t callback, TCD_t* tcd, bool halfMajor, bool fullMajor) {
	irqs[channel] = callback;
	tcd->CSR |= DMA_CSR_INTMAJOR(fullMajor) | DMA_CSR_INTHALF(halfMajor);
	DMA0->TCD[channel].CSR |= tcd->CSR;
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static void dma_IQR_handler(dma_channels_t channel) {
	irqs[channel]();
}


__ISR__ DMA0_IRQHandler(void) {
	DMA0->CINT |= 0;
	dma_IQR_handler(dmaCHANNEL0);
}

__ISR__ DMA1_IRQHandler(void) {
	DMA0->CINT |= 1;
	dma_IQR_handler(dmaCHANNEL1);
}

__ISR__ DMA2_IRQHandler(void) {
	DMA0->CINT |= 2;
	dma_IQR_handler(dmaCHANNEL2);
}

__ISR__ DMA3_IRQHandler(void) {
	DMA0->CINT |= 3;
	dma_IQR_handler(dmaCHANNEL3);
}

__ISR__ DMA4_IRQHandler(void) {
	DMA0->CINT |= 4;
	dma_IQR_handler(dmaCHANNEL4);
}

__ISR__ DMA5_IRQHandler(void) {
	DMA0->CINT |= 5;
	dma_IQR_handler(dmaCHANNEL5);
}

__ISR__ DMA6_IRQHandler(void) {
	DMA0->CINT |= 6;
	dma_IQR_handler(dmaCHANNEL6);
}

__ISR__ DMA7_IRQHandler(void) {
	DMA0->CINT |= 7;
	dma_IQR_handler(dmaCHANNEL7);
}

__ISR__ DMA8_IRQHandler(void) {
	DMA0->CINT |= 8;
	dma_IQR_handler(dmaCHANNEL8);
}

__ISR__ DMA9_IRQHandler(void) {
	DMA0->CINT |= 9;
	dma_IQR_handler(dmaCHANNEL9);
}

__ISR__ DMA10_IRQHandler(void) {
	DMA0->CINT |= 10;
	dma_IQR_handler(dmaCHANNEL10);
}

__ISR__ DMA12_IRQHandler(void) {
	DMA0->CINT |= 12;
	dma_IQR_handler(dmaCHANNEL12);
}

__ISR__ DMA13_IRQHandler(void) {
	DMA0->CINT |= 13;
	dma_IQR_handler(dmaCHANNEL13);
}

__ISR__ DMA14_IRQHandler(void) {
	DMA0->CINT |= 14;
	dma_IQR_handler(dmaCHANNEL14);
}

__ISR__ DMA15_IRQHandler(void) {
	DMA0->CINT |= 15;
	dma_IQR_handler(dmaCHANNEL15);
}



void DMA_Error_IRQHandler()
{
	/* Clear the error interrupt flag.*/
	DMA0->CERR |= 1;

	/* Turn the red LED on. */
	gpioWrite(PIN_LED_RED, 0);
}
