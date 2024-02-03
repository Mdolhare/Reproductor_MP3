
#include "DAC.h"
#define CANT_PORTS 2
#define DAC_DATL_DATA0_WIDTH 8
static DAC_Type * const dac_base[CANT_PORTS] = ADC_BASE_PTRS;
void DAC_Init (bool dma)
{

	SIM->SCGC2 |= SIM_SCGC2_DAC0_MASK;
	DAC0->C0 = DAC_C0_DACEN_MASK | DAC_C0_DACRFS_MASK | DAC_C0_DACTRGSEL_MASK; //está en software ahora el trigger
	if (dma)
		DAC0->C1 |= DAC_C1_DMAEN_MASK;

}

void DAC_SetData ( DACData_t data)
{
	DAC0->DAT[0].DATL = DAC_DATL_DATA0(data);
	DAC0->DAT[0].DATH = DAC_DATH_DATA1(data >> DAC_DATL_DATA0_WIDTH);
}


uint8_t* DAC_getPtrToDat(void) {
	return &(DAC0->DAT[0].DATL);
}
