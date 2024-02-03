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

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

void DMA_init(uint32_t channel, uint32_t source,
		bool timer, uint16_t* sourceBuffer, uint8_t* destinationBuffer,
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

	DMA0->TCD[channel].ATTR = DMA_ATTR_SSIZE(config.transferSize) | DMA_ATTR_DSIZE(config.transferSize)
			| DMA_ATTR_DMOD(config.dCircBuff) | DMA_ATTR_SMOD(config.sCircBuff);

	DMA0->TCD[channel].NBYTES_MLNO = config.byteAmount;

	DMA0->TCD[channel].CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(config.minorLoopIter);
	DMA0->TCD[channel].BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(config.minorLoopIter);

	DMA0->TCD[channel].SLAST = -config.sShiftBack;
	DMA0->TCD[channel].DLAST_SGA = -config.dShiftBack;

	DMA0->TCD[channel].CSR = DMA_CSR_INTMAJOR_MASK;


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
	gpioWrite(PIN_LED_GREEN, 1);
	/* Clear the interrupt flag. */
	DMA0->CINT |= 0;

	/* Toggle the blue LED. */
	//gpioToggle(PIN_LED_BLUE);

	/* Change the source buffer contents. */
	//MinorTransferDone = 1;
	funcs[0]();
	gpioWrite(PIN_LED_GREEN, 0);
}


__ISR__ DMA1_IRQHandler(void)
{
	/* Clear the interrupt flag. */
	DMA0->CINT |= 1;

	/* Toggle the blue LED. */
	//gpioToggle(PIN_LED_GREEN);

	/* Change the source buffer contents. */
	//MinorTransferDone = 1;
	funcs[1]();
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
