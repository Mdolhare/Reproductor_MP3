/*
 * vumeter.c
 *
 *  Created on: Feb 29, 2024
 *      Author: Grupo 2
 */


/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "MK64F12.h"
#include "arm_math.h"

#include "vumeter.h"

#include <math.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

static int32_t myRound(float32_t num);

static void distributeBins(int16_t* fft);

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/


/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static arm_rfft_instance_q15 fftInstance;

static int16_t vumeterBins[VUMETER_BIN_AMOUNT];

static int16_t limits[VUMETER_BIN_AMOUNT];

static int16_t fft_abs[MAX_SAMPLE_SIZE];

static int16_t sampleSize;

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void vumeterInit(int16_t _sampleSize, int32_t fs, float32_t lowBand, float32_t highBand) {

	arm_rfft_init_q15(&fftInstance, _sampleSize, 0, 1);

	sampleSize = _sampleSize;

	float32_t binWidth = fs/_sampleSize;
	float32_t freqMult = pow(highBand/lowBand, 1/((float)VUMETER_BIN_AMOUNT-1));

	float32_t centerFreq = lowBand;
	float32_t centerBin = centerFreq/binWidth;

	for (int8_t bin=0; bin<VUMETER_BIN_AMOUNT; bin++) {
		centerFreq = centerFreq*freqMult;
		float32_t nextCenterBin = centerFreq/binWidth;

		float32_t highBin = (nextCenterBin - centerBin)/2 + centerBin;
		limits[bin] = myRound(highBin);

		centerBin = nextCenterBin;
	}
}

void vumeterTransform(int16_t* data) {

	static int16_t output[MAX_SAMPLE_SIZE*2];

	arm_rfft_q15(&fftInstance, data, output);

	arm_cmplx_mag_q15(output, fft_abs, sampleSize);

	distributeBins(fft_abs);	//solucionar problema de ganancia a altas frecuencias

	//aca enviar a la matriz
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static int32_t myRound(float32_t num) {
	return (num - (int)num) < 0.5 ? floor(num) : ceil(num);
}

static void distributeBins(int16_t* fft) {
	int16_t lowerLimit = 0;
	int32_t temp = 0;

	for (int16_t bin = 0; bin < VUMETER_BIN_AMOUNT; bin++) {
		for (int16_t i = lowerLimit; i <= limits[bin]; i++) {
			temp += fft[i];
		}
		vumeterBins[bin] = temp / (limits[bin] - lowerLimit);
		temp = 0;
		lowerLimit = limits[bin] + 1;
	}
}













