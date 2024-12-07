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
static q31_t biquadStateBand6Q31[4 * NUMSTAGES];
static q31_t biquadStateBand7Q31[4 * NUMSTAGES];
static q31_t biquadStateBand8Q31[4 * NUMSTAGES];

static arm_biquad_casd_df1_inst_q31 S1;
static arm_biquad_casd_df1_inst_q31 S2;
static arm_biquad_casd_df1_inst_q31 S3;
static arm_biquad_casd_df1_inst_q31 S4;
static arm_biquad_casd_df1_inst_q31 S5;
static arm_biquad_casd_df1_inst_q31 S6;
static arm_biquad_casd_df1_inst_q31 S7;
static arm_biquad_casd_df1_inst_q31 S8;



/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void equalizerInit(int8_t* gainDB) {

	arm_biquad_cascade_df1_init_q31(&S1, NUMSTAGES,
			(q31_t*) &coeffTable[(TABLE_LEN/NUMBER_OF_BANDS)*0 + 10*(gainDB[0] + 10)],
			 &biquadStateBand1Q31[0], NUMSTAGES);
	arm_biquad_cascade_df1_init_q31(&S2, NUMSTAGES,
		    (q31_t*) &coeffTable[(TABLE_LEN/NUMBER_OF_BANDS)*1 + 10*(gainDB[1] + 10)],
		    &biquadStateBand2Q31[0], NUMSTAGES);
	arm_biquad_cascade_df1_init_q31(&S3, NUMSTAGES,
		    (q31_t*) &coeffTable[(TABLE_LEN/NUMBER_OF_BANDS)*2 + 10*(gainDB[2] + 10)],
		    &biquadStateBand3Q31[0], NUMSTAGES);
	arm_biquad_cascade_df1_init_q31(&S4, NUMSTAGES,
		    (q31_t*) &coeffTable[(TABLE_LEN/NUMBER_OF_BANDS)*3 + 10*(gainDB[3] + 10)],
		    &biquadStateBand4Q31[0], NUMSTAGES);
	arm_biquad_cascade_df1_init_q31(&S5, NUMSTAGES,
		    (q31_t*) &coeffTable[(TABLE_LEN/NUMBER_OF_BANDS)*4 + 10*(gainDB[4] + 10)],
		    &biquadStateBand5Q31[0], NUMSTAGES);
	arm_biquad_cascade_df1_init_q31(&S6, NUMSTAGES,
			    (q31_t*) &coeffTable[(TABLE_LEN/NUMBER_OF_BANDS)*5 + 10*(gainDB[5] + 10)],
			    &biquadStateBand6Q31[0], NUMSTAGES);
	arm_biquad_cascade_df1_init_q31(&S7, NUMSTAGES,
			    (q31_t*) &coeffTable[(TABLE_LEN/NUMBER_OF_BANDS)*6 + 10*(gainDB[6] + 10)],
			    &biquadStateBand7Q31[0], NUMSTAGES);
	arm_biquad_cascade_df1_init_q31(&S8, NUMSTAGES,
			    (q31_t*) &coeffTable[(TABLE_LEN/NUMBER_OF_BANDS)*7 + 10*(gainDB[7] + 10)],
			    &biquadStateBand8Q31[0], NUMSTAGES);

}

void equalizerFilter(int16_t* inputBuffer, int16_t* outputBuffer, int32_t len) {

	q31_t inputBlock[128];
	q31_t outputBlock[128];

	int blocksize = len<<5;//len/10;

	for (int i=0; i<(len/blocksize); i++) {

		arm_q15_to_q31(inputBuffer + (i*blocksize), inputBlock, blocksize);
		//arm_float_to_q31(inputBuffer + (i*BLOCKSIZE), inputBlock, BLOCKSIZE);

		arm_scale_q31(inputBlock, 0x7FFFFFFF, -3, inputBlock, blocksize);

		arm_biquad_cascade_df1_fast_q31(&S1, inputBlock, outputBlock, blocksize);
		arm_biquad_cascade_df1_fast_q31(&S2, outputBlock, outputBlock, blocksize);
		arm_biquad_cascade_df1_fast_q31(&S3, outputBlock, outputBlock, blocksize);
		arm_biquad_cascade_df1_fast_q31(&S4, outputBlock, outputBlock, blocksize);
		arm_biquad_cascade_df1_fast_q31(&S5, outputBlock, outputBlock, blocksize);
		arm_biquad_cascade_df1_fast_q31(&S6, outputBlock, outputBlock, blocksize);
		arm_biquad_cascade_df1_fast_q31(&S7, outputBlock, outputBlock, blocksize);
		arm_biquad_cascade_df1_fast_q31(&S8, outputBlock, outputBlock, blocksize);

		arm_scale_q31(outputBlock, 0x7FFFFFFF, -3, outputBlock, blocksize);

		arm_q31_to_q15(outputBlock, outputBuffer + (i*blocksize), blocksize);
		//arm_q31_to_float(outputBlock, outputBuffer + (i*BLOCKSIZE), BLOCKSIZE);

		//arm_scale_f32(outputBuffer + (i*BLOCKSIZE), 8.0f, outputBuffer + (i*BLOCKSIZE), BLOCKSIZE);
	}


}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


