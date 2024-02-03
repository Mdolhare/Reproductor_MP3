
#include "ADC.h"
#define CANT_PORTS 2
static ADC_Type * const adc_base[CANT_PORTS] = ADC_BASE_PTRS;
#define TWO_POW_NUM_OF_CAL (1 << 4) //16

bool ADC_interrupt[2] = {false, false};


void ADC_Init (int adc ,ADCBits_t resolution, ADCCycles_t cycles, ADCClockDivideSelect_t  clock_divide_select, bool dma)
{
	//Clock Gating e interrupciones
	switch(adc)
	{
	case 0:
		SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;
		NVIC_EnableIRQ(ADC0_IRQn);

		break;
	case 1:
		SIM->SCGC3 |= SIM_SCGC3_ADC1_MASK;
		NVIC_EnableIRQ(ADC1_IRQn);
		break;
	}

	adc_base[adc]->CFG1 = ADC_CFG1_ADICLK(BUS_CLOCK);

	adc_base[adc]->CFG1 = ADC_CFG1_ADIV(clock_divide_select);

	if(dma)
		adc_base[adc]->SC2 |= ADC_SC2_DMAEN_MASK;

	ADC_SetResolution(adc, resolution);
	ADC_SetCycles(adc, cycles);
	ADC_Calibrate(adc);

}


void ADC_SetResolution (int adc, ADCBits_t bits)
{
	adc_base[adc]->CFG1 = (adc_base[adc]->CFG1 & ~ADC_CFG1_MODE_MASK) | ADC_CFG1_MODE(bits);
}

void ADC_SetCycles (int adc, ADCCycles_t cycles)
{
	if (cycles & ~ADC_CFG2_ADLSTS_MASK)
	{
		adc_base[adc]->CFG1 &= ~ADC_CFG1_ADLSMP_MASK;
	}
	else
	{
		adc_base[adc]->CFG1 |= ADC_CFG1_ADLSMP_MASK;
		adc_base[adc]->CFG2 = (adc_base[adc]->CFG2 & ~ADC_CFG2_ADLSTS_MASK) | ADC_CFG2_ADLSTS(cycles);
	}
}



void ADC_SetInterruptMode (int adc, bool mode)
{

	ADC_interrupt[adc] = mode;


}

bool ADC_IsInterruptPending (int adc)
{
	return adc_base[adc]->SC1[0] & ADC_SC1_COCO_MASK;
}

void ADC_ClearInterruptFlag (int adc)
{
	adc_base[adc]->SC1[0] = 0x00;
}



ADCBits_t ADC_GetResolution (int adc)
{
	return adc_base[adc]->CFG1 & ADC_CFG1_MODE_MASK;
}



ADCCycles_t ADC_GetSCycles (int adc)
{
	if (adc_base[adc]->CFG1 & ADC_CFG1_ADLSMP_MASK)
		return ADC_c4;
	else
		return adc_base[adc]->CFG2 & ADC_CFG2_ADLSTS_MASK;
}

void ADC_SetHardwareAverage (int adc, ADCTaps_t taps)
{
	if (taps & ~ADC_SC3_AVGS_MASK)
	{
		adc_base[adc]->SC3 &= ~ADC_SC3_AVGE_MASK;
	}
	else
	{
		adc_base[adc]->SC3 |= ADC_SC3_AVGE_MASK;
		adc_base[adc]->SC3 = (adc_base[adc]->SC3 & ~ADC_SC3_AVGS_MASK) | ADC_SC3_AVGS(taps);
	}
}
ADCTaps_t ADC_GetHardwareAverage (int adc)
{
	if (adc_base[adc]->SC3 & ADC_SC3_AVGE_MASK)
		return ADC_t1;
	else
		return adc_base[adc]->SC3 & ADC_SC3_AVGS_MASK;
}

bool ADC_Calibrate (int adc)
{
	int32_t  Offset		= 0;
	uint32_t Minus	[7] = {0,0,0,0,0,0,0};
	uint32_t Plus	[7] = {0,0,0,0,0,0,0};
	uint8_t  i;
	uint32_t scr3;

	/// SETUP
	adc_base[adc]->SC1[0] = 0x1F;
	scr3 = adc_base[adc]->SC3;
	adc_base[adc]->SC3 &= (ADC_SC3_AVGS(0x03) | ADC_SC3_AVGE_MASK);

	/// INITIAL CALIBRATION
	adc_base[adc]->SC3 &= ~ADC_SC3_CAL_MASK;
	adc_base[adc]->SC3 |=  ADC_SC3_CAL_MASK;
	while (!(adc_base[adc]->SC1[0] & ADC_SC1_COCO_MASK));
	if (adc_base[adc]->SC3 & ADC_SC3_CALF_MASK)
	{
		adc_base[adc]->SC3 |= ADC_SC3_CALF_MASK;
		return false;
	}
	adc_base[adc]->PG  = (0x8000 | ((adc_base[adc]->CLP0+adc_base[adc]->CLP1+adc_base[adc]->CLP2+adc_base[adc]->CLP3+adc_base[adc]->CLP4+adc_base[adc]->CLPS) >> (1 + TWO_POW_NUM_OF_CAL)));
	adc_base[adc]->MG  = (0x8000 | ((adc_base[adc]->CLM0+adc_base[adc]->CLM1+adc_base[adc]->CLM2+adc_base[adc]->CLM3+adc_base[adc]->CLM4+adc_base[adc]->CLMS) >> (1 + TWO_POW_NUM_OF_CAL)));

	// FURTHER CALIBRATIONS
	for (i = 0; i < TWO_POW_NUM_OF_CAL; i++)
	{
		adc_base[adc]->SC3 &= ~ADC_SC3_CAL_MASK;
		adc_base[adc]->SC3 |=  ADC_SC3_CAL_MASK;
		while (!(adc_base[adc]->SC1[0] & ADC_SC1_COCO_MASK));
		if (adc_base[adc]->SC3 & ADC_SC3_CALF_MASK)
		{
			adc_base[adc]->SC3 |= ADC_SC3_CALF_MASK;
			return 1;
		}
		Offset += (short)adc_base[adc]->OFS;
		Plus[0] += (unsigned long)adc_base[adc]->CLP0;
		Plus[1] += (unsigned long)adc_base[adc]->CLP1;
		Plus[2] += (unsigned long)adc_base[adc]->CLP2;
		Plus[3] += (unsigned long)adc_base[adc]->CLP3;
		Plus[4] += (unsigned long)adc_base[adc]->CLP4;
		Plus[5] += (unsigned long)adc_base[adc]->CLPS;
		Plus[6] += (unsigned long)adc_base[adc]->CLPD;
		Minus[0] += (unsigned long)adc_base[adc]->CLM0;
		Minus[1] += (unsigned long)adc_base[adc]->CLM1;
		Minus[2] += (unsigned long)adc_base[adc]->CLM2;
		Minus[3] += (unsigned long)adc_base[adc]->CLM3;
		Minus[4] += (unsigned long)adc_base[adc]->CLM4;
		Minus[5] += (unsigned long)adc_base[adc]->CLMS;
		Minus[6] += (unsigned long)adc_base[adc]->CLMD;
	}
	adc_base[adc]->OFS = (Offset >> TWO_POW_NUM_OF_CAL);
	adc_base[adc]->PG  = (0x8000 | ((Plus[0] +Plus[1] +Plus[2] +Plus[3] +Plus[4] +Plus[5] ) >> (1 + TWO_POW_NUM_OF_CAL)));
	adc_base[adc]->MG  = (0x8000 | ((Minus[0]+Minus[1]+Minus[2]+Minus[3]+Minus[4]+Minus[5]) >> (1 + TWO_POW_NUM_OF_CAL)));
	adc_base[adc]->CLP0 = (Plus[0] >> TWO_POW_NUM_OF_CAL);
	adc_base[adc]->CLP1 = (Plus[1] >> TWO_POW_NUM_OF_CAL);
	adc_base[adc]->CLP2 = (Plus[2] >> TWO_POW_NUM_OF_CAL);
	adc_base[adc]->CLP3 = (Plus[3] >> TWO_POW_NUM_OF_CAL);
	adc_base[adc]->CLP4 = (Plus[4] >> TWO_POW_NUM_OF_CAL);
	adc_base[adc]->CLPS = (Plus[5] >> TWO_POW_NUM_OF_CAL);
	adc_base[adc]->CLPD = (Plus[6] >> TWO_POW_NUM_OF_CAL);
	adc_base[adc]->CLM0 = (Minus[0] >> TWO_POW_NUM_OF_CAL);
	adc_base[adc]->CLM2 = (Minus[2] >> TWO_POW_NUM_OF_CAL);
	adc_base[adc]->CLM3 = (Minus[3] >> TWO_POW_NUM_OF_CAL);
	adc_base[adc]->CLM4 = (Minus[4] >> TWO_POW_NUM_OF_CAL);
	adc_base[adc]->CLMS = (Minus[5] >> TWO_POW_NUM_OF_CAL);
	adc_base[adc]->CLMD = (Minus[6] >> TWO_POW_NUM_OF_CAL);

	/// UN-SETUP
	adc_base[adc]->SC3 = scr3;

	return true;
}

void ADC_Start (int adc, uint8_t channel, ADCMux_t mux)
{
	adc_base[adc]->CFG2 = (adc_base[adc]->CFG2 & ~ADC_CFG2_MUXSEL_MASK) | ADC_CFG2_MUXSEL(mux);
	adc_base[adc]->SC1[0] = ADC_SC1_AIEN(ADC_interrupt[adc]) | ADC_SC1_ADCH(channel);
}

bool ADC_IsReady (int adc)
{
	return adc_base[adc]->SC1[0] & ADC_SC1_COCO_MASK;
}

ADCData_t ADC_getData (int adc)
{

	return adc_base[adc]->R[0];
}

ADCData_t* ADC_getDataPtr(uint8_t adc) {
	return &(adc_base[adc]->R[0]);
}


