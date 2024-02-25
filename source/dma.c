/*******************************************************************************
  @file     dma.c
  @brief    Driver para DMA
  @author   GRUPO 2
 ******************************************************************************/
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "../SDK/CMSIS/MK64F12.h"
#include "dma.h"
#include "hardware.h"
#include "board.h"

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

static dmaIrqFun_t funcs[DMA_CHANNEL_AMOUNT];

static uint32_t* espia;
static uint32_t* espia2;


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

void DMA_init(uint32_t channel, uint32_t source,
		bool timer, uint32_t* sourceBuffer, uint32_t* destinationBuffer,
		dma_cfg_t config) {

	SIM->SCGC7 |= SIM_SCGC7_DMA_MASK;
	SIM->SCGC6 |= SIM_SCGC6_DMAMUX_MASK;

	if(timer)
		DMAMUX->CHCFG[channel] |= DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_TRIG_MASK | DMAMUX_CHCFG_SOURCE(source);
	else
		DMAMUX->CHCFG[channel] |= DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_SOURCE(source);

	NVIC_ClearPendingIRQ(channel);
	NVIC_EnableIRQ(channel);

	DMA0->TCD[channel].SADDR = (uint32_t)(sourceBuffer);
	DMA0->TCD[channel].DADDR = (uint32_t)(destinationBuffer);

	DMA0->TCD[channel].SOFF = config.sOffset;
	DMA0->TCD[channel].DOFF = config.dOffset;

	DMA0->TCD[channel].ATTR = DMA_ATTR_SSIZE(config.sTransferSize) | DMA_ATTR_DSIZE(config.dTransferSize)
			| DMA_ATTR_DMOD(config.dCircBuff) | DMA_ATTR_SMOD(config.sCircBuff);

	DMA0->TCD[channel].NBYTES_MLNO = config.byteAmount;

	DMA0->TCD[channel].CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(config.minorLoopIter);
	DMA0->TCD[channel].BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(config.minorLoopIter);

	DMA0->TCD[channel].SLAST = -config.sShiftBack;
	DMA0->TCD[channel].DLAST_SGA = -config.dShiftBack;

	DMA0->TCD[channel].CSR = DMA_CSR_INTMAJOR_MASK;


	DMA0->ERQ |= DMA_ERQ_ERQ0_MASK << channel;

}
void DMA_init_tcd(dma_channels_t channel, dma_sources_t source, bool timer, TCD_t tcd) {

	SIM->SCGC7 |= SIM_SCGC7_DMA_MASK;
	SIM->SCGC6 |= SIM_SCGC6_DMAMUX_MASK;

	if(timer)
		DMAMUX->CHCFG[channel] |= DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_TRIG_MASK | DMAMUX_CHCFG_SOURCE(source);
	else
		DMAMUX->CHCFG[channel] |= DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_SOURCE(source);

	NVIC_ClearPendingIRQ(channel);
	NVIC_EnableIRQ(channel);

	DMA0->TCD[channel].SADDR = tcd.SADDR;
	DMA0->TCD[channel].DADDR = tcd.DADDR;


	DMA0->TCD[channel].SOFF = tcd.SOFF;
	DMA0->TCD[channel].DOFF = tcd.DOFF;

	DMA0->TCD[channel].ATTR = tcd.ATTR;

	DMA0->TCD[channel].NBYTES_MLNO = tcd.NBYTES_MLNO;

	DMA0->TCD[channel].CITER_ELINKNO = tcd.CITER_ELINKNO;
	DMA0->TCD[channel].BITER_ELINKNO = tcd.BITER_ELINKNO;

	DMA0->TCD[channel].SLAST = tcd.SLAST;
	DMA0->TCD[channel].DLAST_SGA = tcd.DLASTSGA;

	DMA0->TCD[channel].CSR = tcd.CSR;


	DMA0->ERQ |= DMA_ERQ_ERQ0_MASK << channel;

}



void dma_set_jump(uint8_t channel, uint8_t sOffset) {
	DMA0->TCD[channel].SOFF = sOffset;
}



void dma_add_irq(uint8_t channel, dmaIrqFun_t callback) {
	funcs[channel] = callback;
}


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


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

__ISR__ DMA0_IRQHandler(void)
{
	/* Clear the interrupt flag. */
	DMA0->CINT |= 0;

	/* Toggle the blue LED. */
	//gpioToggle(PIN_LED_BLUE);

	/* Change the source buffer contents. */
	//MinorTransferDone = 1;
	funcs[0]();
}


__ISR__ DMA1_IRQHandler(void)
{
	gpioWrite(PTB9, 1);
	/* Clear the interrupt flag. */
	DMA0->CINT |= 1;

	/* Toggle the blue LED. */
	//gpioToggle(PIN_LED_GREEN);

	/* Change the source buffer contents. */
	//MinorTransferDone = 1;
	//funcs[1]();
	gpioWrite(PTB9, 0);
}


__ISR__ DMA2_IRQHandler(void)
{
	/* Clear the interrupt flag. */
	DMA0->CINT |= 2;

	/* Toggle the blue LED. */
	gpioToggle(PIN_LED_BLUE);

	/* Change the source buffer contents. */
	//MinorTransferDone = 1;
	//funcs[1]();
}

void DMA_Error_IRQHandler()
{
	/* Clear the error interrupt flag.*/
	DMA0->CERR |= 1;

	/* Turn the red LED on. */
	gpioWrite(PIN_LED_RED, 0);
}

