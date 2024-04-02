#ifndef ADC_H_
#define ADC_H_


#include "hardware.h"
#define BUS_CLOCK 0
typedef enum
{
	ADC_cds1,
	ADC_cds2,
	ADC_cds4,
	ADC_cds8,
} ADCClockDivideSelect_t;

typedef enum
{
	ADC_b8,
	ADC_b12,
	ADC_b10,
	ADC_b16,
} ADCBits_t;

typedef enum
{
	ADC_c24,
	ADC_c16,
	ADC_c10,
	ADC_c6,
	ADC_c4,
} ADCCycles_t;

typedef enum
{
	ADC_t4,
	ADC_t8,
	ADC_t16,
	ADC_t32,
	ADC_t1,
} ADCTaps_t;

typedef enum
{
	ADC_mA,
	ADC_mB,
} ADCMux_t;

typedef ADC_Type *ADC_t;
typedef uint8_t ADCChannel_t; /* Channel 0-23 */
typedef uint16_t ADCData_t;

void ADC_Init  (int adc ,ADCBits_t resolution, ADCCycles_t cycles, ADCClockDivideSelect_t  clock_divide_select, bool dma);

void ADC_SetInterruptMode  (int adc , bool mode);
bool ADC_IsInterruptPending (int adc);
void ADC_ClearInterruptFlag (int adc);

void ADC_SetResolution (int adc, ADCBits_t bits);
ADCBits_t ADC_GetResolution (int adc);
void ADC_SetCycles (int adc, ADCCycles_t cycles);
ADCCycles_t ADC_GetCycles (int adc);
void ADC_SetHardwareAverage (int adc, ADCTaps_t taps);
ADCTaps_t  ADC_GetHardwareAverage (int adc);

bool ADC_Calibrate (int adc);

void ADC_Start (int adc, ADCChannel_t, ADCMux_t);

bool ADC_IsReady (int adc);

ADCData_t ADC_getData (int adc);

ADCData_t* ADC_getDataPtr(uint8_t adc);


#endif /* ADC_H_ */
