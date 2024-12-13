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
#include "../Drivers/HAL/Matrix/matrix.h"

#include "hamming.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*typedef enum {
	HEIGHT1 = UINT16_MAX/8,
	HEIGHT2 = 2*UINT16_MAX/8,
	HEIGHT3 = 3*UINT16_MAX/8,
	HEIGHT4 = 4*UINT16_MAX/8,
	HEIGHT5 = 5*UINT16_MAX/8,
	HEIGHT6 = 6*UINT16_MAX/8,
	HEIGHT7 = 7*UINT16_MAX/8,
	HEIGHT8 = 8*UINT16_MAX/8,
} heights_t;*/

typedef enum {
	HEIGHT1 = 0,
	HEIGHT2 = 10,
	HEIGHT3 = 20,
	HEIGHT4 = 30,
	HEIGHT5 = 40,
	HEIGHT6 = 50,
	HEIGHT7 = 60,
	HEIGHT8 = 70,
} heights_t;


/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

static uint32_t myRound(float32_t num);

static void distributeBins(int16_t* fft);

static void sendBins(uint16_t* bins);

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/


/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static arm_rfft_instance_q15 fftInstance;

static uint16_t vumeterBins[VUMETER_BIN_AMOUNT];

static uint16_t limits[VUMETER_BIN_AMOUNT];

static uint16_t fft_abs[MAX_SAMPLE_SIZE];

static uint16_t sampleSize;

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void vumeterInit(uint16_t _sampleSize, uint32_t fs, float32_t lowBand, float32_t highBand) {

	arm_rfft_init_q15(&fftInstance, _sampleSize, 0, 1);

	sampleSize = _sampleSize;

	float32_t binWidth = fs/_sampleSize;
	float32_t freqMult = pow(highBand/lowBand, 1/((float)VUMETER_BIN_AMOUNT-1));

	float32_t centerFreq = lowBand;
	float32_t centerBin = (centerFreq/binWidth);

	for (int8_t bin=0; bin<VUMETER_BIN_AMOUNT; bin++) {
		centerFreq = centerFreq*freqMult;
		float32_t nextCenterBin = centerFreq/binWidth;

		float32_t highBin = (((nextCenterBin - centerBin)/2 + centerBin));
		limits[bin] = myRound(highBin);

		centerBin = nextCenterBin;
	}

	for (int8_t bin=0; bin<VUMETER_BIN_AMOUNT; bin++) {

		limits[bin] = ((uint64_t)limits[bin]*sampleSize)/limits[7]; // sampleSize era 4096

	}

	//matrixInit();
}

void vumeterTransform(int16_t* data) {

	static int16_t output[MAX_SAMPLE_SIZE*2];

	//for (int i = 0; i < MAX_SAMPLE_SIZE; i++)
		//data[i] = data[i]*hamming_window[i];

	arm_rfft_q15(&fftInstance, data, output);

	arm_cmplx_mag_q15(output, fft_abs, sampleSize*2);

	distributeBins(fft_abs);

	sendBins(vumeterBins);	//aca enviar a la matriz
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static uint32_t myRound(float32_t num) {
	return ((num - (int)num) < 0.5 ? floor(num) : ceil(num))*10;
}

static void distributeBins(int16_t* fft) {
	uint16_t lowerLimit = 0;
	uint32_t temp = 0;

	for (uint16_t bin = 0; bin < VUMETER_BIN_AMOUNT; bin++) {
		for (uint16_t i = lowerLimit; i < limits[bin]; i++) {
			temp += fft[i];
		}

		vumeterBins[bin] = (temp / (limits[bin] - lowerLimit));
		if (bin == 0)
			vumeterBins[bin] -= 15;

		temp = 0;
		lowerLimit = limits[bin] + 1;
	}
}

static void sendBins(uint16_t* bins) {

	for(uint8_t col=0; col<COLUMNS; col++) {

		if (bins[col] > HEIGHT1)
			updateLED(col, 0b00110000, 0b00110000, 0);
		else
			updateLED(col, 0, 0, 0);

		if (bins[col ] > HEIGHT2)
			updateLED(col + COLUMNS, 0b00110000, 0b00110000, 0);
		else
			updateLED(col + COLUMNS, 0, 0, 0);

		if (bins[col] > HEIGHT3)
			updateLED(col + 2*COLUMNS, 0b00110000, 0b00110000, 0);
		else
			updateLED(col + 2*COLUMNS, 0, 0, 0);

		if (bins[col ] > HEIGHT4)
			updateLED(col + 3*COLUMNS, 0b00110000, 0b00110000, 0);
		else
			updateLED(col + 3*COLUMNS, 0, 0, 0);

		if (bins[col ] > HEIGHT5)
			updateLED(col + 4*COLUMNS, 0b00110000, 0b00110000, 0);
		else
			updateLED(col + 4*COLUMNS, 0, 0, 0);

		if (bins[col ] > HEIGHT6)
			updateLED(col + 5*COLUMNS, 0b00110000, 0b00110000, 0);
		else
			updateLED(col + 5*COLUMNS, 0, 0, 0);

		if (bins[col] > HEIGHT7)
			updateLED(col + 6*COLUMNS, 0b00110000, 0b00110000, 0);
		else
			updateLED(col + 6*COLUMNS, 0, 0, 0);

		if (bins[col] > HEIGHT8)
			updateLED(col + 7*COLUMNS, 0b00110000, 0b00110000, 0);
		else
			updateLED(col + 7*COLUMNS, 0, 0, 0);

	}

}











