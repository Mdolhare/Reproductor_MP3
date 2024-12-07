/*
 * audio.c
 *
 *  Created on: 2 abr. 2024
 *      Author: grupo 2
 */
#include <stdint.h>
#include "../Drivers/MCAL/DMA/dma.h"
#include "../Drivers/MCAL/DAC/DAC.h"
#include "../Drivers/MCAL/PIT/pit.h"

static uint32_t T;
static TCD_t tcd[2] __attribute__((aligned(32)));
static tcd_cfg_t configs[2];
static bool* flag;

static void flagHandler(void);

void audio_init(uint32_t _fs, uint16_t* source, uint16_t* source2, uint32_t frameSize, bool* _flag) {
	//pitInit();
	flag = _flag;

	DAC_Init(true);
/*
    finished = true;

    wordBufferIndex=0;
    shiftCounter=0;
*/
    //DAC_Init(true);

    T = 1000000/_fs;





    uint8_t* destination = DAC_getPtrToDat();

    configs[0].source = (uint32_t)source;
    configs[0].destination = (uint32_t)destination;
    configs[0].sOffset = 2;
    configs[0].dOffset = 0;
    configs[0].sTransferSize = dma16BIT;
    configs[0].dTransferSize = dma16BIT;
    configs[0].byteAmount = 2;
    configs[0].minorLoopIter = frameSize/2;
    configs[0].sShiftBack = 0;
    configs[0].dShiftBack = (uint32_t)&tcd[1];
    configs[0].sCircBuff = 0;
    configs[0].dCircBuff = 0;

    configs[1].source = (uint32_t)source2;
	configs[1].destination = (uint32_t)destination;
	configs[1].sOffset = 2;
	configs[1].dOffset = 0;
	configs[1].sTransferSize = dma16BIT;
	configs[1].dTransferSize = dma16BIT;
	configs[1].byteAmount = 2;
	configs[1].minorLoopIter = frameSize/2;
	configs[1].sShiftBack = 0;
	configs[1].dShiftBack = (uint32_t)&tcd[0];
	configs[1].sCircBuff = 0;
	configs[1].dCircBuff = 0;



    dma_config_scatter_gather(configs, tcd, 2);

    dma_add_irq(dmaCHANNEL0, flagHandler, &(tcd[0]), false, true);
    dma_add_irq(dmaCHANNEL0, flagHandler, &(tcd[1]), false, true);

    dma_begin(dmaCHANNEL0, dmaDMA_MUX_5, true, &(tcd[0]));



}

void audio_resume() {
	pitSetAndBegin(PIT_0, T);
}

void audio_pause(){
	pitStopTimer(PIT_0);
}


static void flagHandler(void) {

	*flag = !(*flag);

	if (!(*flag)) {
		int a = 0;
	}

}

