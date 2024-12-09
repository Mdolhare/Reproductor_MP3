/*
 * equalizer.c
 *
 *  Created on: Apr 1, 2024
 *      Author: Grupo 2
 */

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "MK64F12.h"
#include "arm_math.h"

#include "equalizer.h"
#include "coeffTable.h"

#include "../Drivers/HAL/Matrix/matrix.h"
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define NUMSTAGES 2
#define BLOCKSIZE 300
//#define BLOCKSIZE 36
#define SIZEFRAME 1152

#define NUMBER_OF_BLOCKS (FRAME_SIZE/BLOCKSIZE)

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

static void show_equ_gain(int8_t* gainDB);

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/


/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static q31_t biquadStateBand1Q31[4 * NUMSTAGES];
static q31_t biquadStateBand2Q31[4 * NUMSTAGES];
static q31_t biquadStateBand3Q31[4 * NUMSTAGES];
static q31_t biquadStateBand4Q31[4 * NUMSTAGES];
static q31_t biquadStateBand5Q31[4 * NUMSTAGES];


static arm_biquad_casd_df1_inst_q31 S1;
static arm_biquad_casd_df1_inst_q31 S2;
static arm_biquad_casd_df1_inst_q31 S3;
static arm_biquad_casd_df1_inst_q31 S4;
static arm_biquad_casd_df1_inst_q31 S5;


static uint8_t gain[5] = 0;

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void equalizerInit(int8_t* gainDB) {

	arm_biquad_cascade_df1_init_q31(&S1, NUMSTAGES,
			(q31_t*) &coeffTable[(TABLE_LEN/NUMBER_OF_BANDS)*0 + 10*(gainDB[0] + 10)],
			 &(biquadStateBand1Q31[0]), NUMSTAGES);
	arm_biquad_cascade_df1_init_q31(&S2, NUMSTAGES,
		    (q31_t*) &coeffTable[(TABLE_LEN/NUMBER_OF_BANDS)*1 + 10*(gainDB[1] + 10)],
		    &(biquadStateBand2Q31[0]), NUMSTAGES);
	arm_biquad_cascade_df1_init_q31(&S3, NUMSTAGES,
		    (q31_t*) &coeffTable[(TABLE_LEN/NUMBER_OF_BANDS)*3 + 10*(gainDB[2] + 10)],
		    &(biquadStateBand3Q31[0]), NUMSTAGES);
	arm_biquad_cascade_df1_init_q31(&S4, NUMSTAGES,
		    (q31_t*) &coeffTable[(TABLE_LEN/NUMBER_OF_BANDS)*5 + 10*(gainDB[3] + 10)],
		    &(biquadStateBand4Q31[0]), NUMSTAGES);
	arm_biquad_cascade_df1_init_q31(&S5, NUMSTAGES,
		    (q31_t*) &coeffTable[(TABLE_LEN/NUMBER_OF_BANDS)*7 + 10*(gainDB[4] + 10)],
		    &(biquadStateBand5Q31[0]), NUMSTAGES);


}

uint8_t* getGain(presets_t preset, uint8_t* gainDB) {

	uint8_t gains[5] = 0;

	switch (preset) {
		case NORMAL: break;
		case JAZZ:
			gains[3] = -5;
			gains[4] = -1;
			gains[5] = 2;
		break;
		case POP:
			gains[4] = 2;
			gains[5] = -2;
		break;
		case ROCK:
			gains[3] = 2;
			gains[4] = -2;
			gains[5] = 1;
		break;
		case CLASSIC:
			gains[4] = -6;
			gains[5] = 1;
		break;
		case CUSTOM:
			for (int i = 0; i<5; i++)
				gains[i] = gainDB[i];
		break;
		default: break;

	}

	for (int i = 0; i<5; i++)
		gain[i] = gains[i];

	show_equ_gain(gain);

	return gain;
}


void equalizerFilter(int16_t* inputBuffer, int16_t* outputBuffer, int32_t len) {

	q31_t inputBlock[5000];
	q31_t outputBlock[5000];

	int blocksize = len;//len/10;

	for (int i=0; i<(len/blocksize); i++) {

		arm_q15_to_q31(inputBuffer + (i*blocksize), inputBlock, blocksize);
		//arm_float_to_q31(inputBuffer + (i*BLOCKSIZE), inputBlock, BLOCKSIZE);

		arm_scale_q31(inputBlock, 0x7FFFFFFF, -3, inputBlock, blocksize);

		arm_biquad_cascade_df1_fast_q31(&S1, inputBlock, outputBlock, blocksize);
		arm_biquad_cascade_df1_fast_q31(&S2, outputBlock, outputBlock, blocksize);
		arm_biquad_cascade_df1_fast_q31(&S3, outputBlock, outputBlock, blocksize);
		arm_biquad_cascade_df1_fast_q31(&S4, outputBlock, outputBlock, blocksize);
		arm_biquad_cascade_df1_fast_q31(&S5, outputBlock, outputBlock, blocksize);


		arm_q31_to_q15(outputBlock, outputBuffer + (i*blocksize), blocksize);
		//arm_q31_to_float(outputBlock, outputBuffer + (i*BLOCKSIZE), BLOCKSIZE);

		//arm_scale_q15(outputBuffer + (i*BLOCKSIZE), 0x7FFF, -3, outputBuffer + (i*BLOCKSIZE), blocksize);

		//arm_scale_f32(outputBuffer + (i*BLOCKSIZE), 8.0f, outputBuffer + (i*BLOCKSIZE), BLOCKSIZE);
	}


}




/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static void show_equ_gain(int8_t* gainDB) {

	int g = 0;

	for(uint8_t col=0; col<8; col++) {

		if (col < 2)
			g = col;
		else if (col%2 == 0)
			g++;


		if (gainDB[g] < 0) {

			for (uint8_t i=0; i<ROWS/2; i++) {

				if (i<=(-gainDB[g]/2))
					if (gainDB[g]%2)
						updateLED(i*COLUMNS + (ROWS/2)*COLUMNS + col, 0, 0b00110000, 0b00110000);
					else
						updateLED(i*COLUMNS + (ROWS/2)*COLUMNS + col, 0, 0, 0b00110000);
				else
					updateLED(i*COLUMNS + (ROWS/2)*COLUMNS + col, 0b11000000, 0b11000000, 0b11000000);
			}
			for (uint8_t i=0; i<ROWS/2; i++) {
				updateLED((ROWS/2 - 1)*COLUMNS - i*COLUMNS + col, 0b11000000, 0b11000000, 0b11000000);
			}

		}
		else if (gainDB[g] > 0) {

			for (uint8_t i=0; i<ROWS/2; i++) {

				if (i<=(gainDB[g]/2))
					if (gainDB[g]%2)
						updateLED((ROWS/2 - 1)*COLUMNS - i*COLUMNS + col, 0, 0b00110000, 0b00110000);
					else
						updateLED((ROWS/2 - 1)*COLUMNS - i*COLUMNS + col, 0, 0, 0b00110000);
				else
					updateLED((ROWS/2 - 1)*COLUMNS - i*COLUMNS + col, 0b11000000, 0b11000000, 0b11000000);

			}
			for (uint8_t i=0; i<ROWS/2; i++) {
				updateLED(i*COLUMNS + (ROWS/2)*COLUMNS + col, 0b11000000, 0b11000000, 0b11000000);
			}

		}
		else {	//0dB
			for (uint8_t i=0; i<ROWS; i++) {
				updateLED(i*COLUMNS + col, 0b11000000, 0b11000000, 0b11000000);
			}
		}

	}
}
















