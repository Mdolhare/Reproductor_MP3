
#include "pit.h"
#include "MK64F12.h"
#include "hardware.h"
#include "gpio.h"

static pitIrqFun_t pitIQRs[CANT_PITS];


void pitInit(void) {
	// Clock Gating for PIT
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
	// PIT Module enable
	PIT->MCR &= ~PIT_MCR_MDIS_MASK;


	/* ===================================== */
	/* Configure timer operation when in debug mode */

        PIT->MCR &= ~PIT_MCR_FRZ_MASK;

      //  PIT->MCR |= PIT_MCR_FRZ_MASK;

    /* ===================================== */

    NVIC_EnableIRQ(PIT0_IRQn);
    NVIC_EnableIRQ(PIT1_IRQn);
    NVIC_EnableIRQ(PIT2_IRQn);
    NVIC_EnableIRQ(PIT3_IRQn);


    //PIT->CHANNEL[0].LDVAL = 2000000-1;//256000-1;
    //PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK;
    //PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;

    //NVIC_SetPriority(PIT0_IRQn, 0);
    //NVIC_SetPriority(PIT1_IRQn, 1);


}

void pitSetAndBegin(uint32_t channel, uint32_t time) {
	PIT->CHANNEL[channel].TCTRL |= PIT_TCTRL_TEN_MASK;
	uint32_t val = time*50-1;
	PIT->CHANNEL[channel].LDVAL = val;
}

void pitSetAndBegin20NS(uint32_t channel, uint32_t time) {
	PIT->CHANNEL[channel].TCTRL |= PIT_TCTRL_TEN_MASK;
	uint32_t val = time;
	PIT->CHANNEL[channel].LDVAL = val;
}

void pitSetIRQFunc(uint32_t channel, pitIrqFun_t pitFunc) {
	pitIQRs[channel] = pitFunc;
	PIT->CHANNEL[channel].TCTRL |= PIT_TCTRL_TIE_MASK;
}

void pitDisableIRQFunc(uint32_t channel) {
	PIT->CHANNEL[channel].TCTRL &= ~PIT_TCTRL_TIE_MASK;
}

void pitDisable(uint32_t channel)
{
	PIT->CHANNEL[channel].TCTRL &= ~PIT_TCTRL_TEN_MASK;

}


static void pitIRQHandler(uint32_t pit) {
	pitIQRs[pit]();
}


void pitSetAndBegin2(uint32_t channel, uint32_t time) {
	PIT->CHANNEL[channel].TCTRL |= PIT_TCTRL_TEN_MASK;
	uint32_t val = time;
	PIT->CHANNEL[channel].LDVAL = val;
}


__ISR__ PIT0_IRQHandler(void) {
	//gpioWrite(PORTNUM2PIN(PB,23), HIGH);
	pitIRQHandler(PIT_0);
	PIT->CHANNEL[0].TFLG = 1;
	//gpioWrite(PORTNUM2PIN(PB,23), LOW);
}

__ISR__ PIT1_IRQHandler(void) {
	pitIRQHandler(PIT_1);
	PIT->CHANNEL[1].TFLG = 1;
}

__ISR__ PIT2_IRQHandler(void) {
	pitIRQHandler(PIT_2);
	PIT->CHANNEL[2].TFLG = 1;
}

__ISR__ PIT3_IRQHandler(void) {
	pitIRQHandler(PIT_3);
	PIT->CHANNEL[3].TFLG = 1;
}




