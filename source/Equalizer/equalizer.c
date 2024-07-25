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
#define BLOCKSIZE 256

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

static q63_t biquadStateBand1Q31[4 * NUMSTAGES];
static q63_t biquadStateBand2Q31[4 * NUMSTAGES];
static q31_t biquadStateBand3Q31[4 * NUMSTAGES];
static q31_t biquadStateBand4Q31[4 * NUMSTAGES];
static q31_t biquadStateBand5Q31[4 * NUMSTAGES];
static q31_t biquadStateBand6Q31[4 * NUMSTAGES];
static q31_t biquadStateBand7Q31[4 * NUMSTAGES];
static q31_t biquadStateBand8Q31[4 * NUMSTAGES];

static arm_biquad_cas_df1_32x64_ins_q31 S1;
static arm_biquad_cas_df1_32x64_ins_q31 S2;
static arm_biquad_casd_df1_inst_q31 S3;
static arm_biquad_casd_df1_inst_q31 S4;
static arm_biquad_casd_df1_inst_q31 S5;
static arm_biquad_casd_df1_inst_q31 S6;
static arm_biquad_casd_df1_inst_q31 S7;
static arm_biquad_casd_df1_inst_q31 S8;

static q31_t inputBlock[BLOCKSIZE];
static q31_t outputBlock[BLOCKSIZE];


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void equalizerInit(int8_t* gainDB) {

	arm_biquad_cas_df1_32x64_init_q31(&S1, NUMSTAGES,
			(q31_t*) &coeffTable[(TABLE_LEN/NUMBER_OF_BANDS)*0 + 10*(gainDB[0] + 10)],
			 &biquadStateBand1Q31[0], NUMSTAGES);
	arm_biquad_cas_df1_32x64_init_q31(&S2, NUMSTAGES,
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

void equalizerFilter(int32_t* inputBuffer, int32_t* outputBuffer, int32_t len) {

	for (int i=0; i<(len); i+=BLOCKSIZE) {

		//arm_q15_to_q31(inputBuffer + i, inputBlock, BLOCKSIZE);

		arm_scale_q31(inputBlock, 0x7FFFFFFF, -3, inputBlock, BLOCKSIZE);

		arm_biquad_cas_df1_32x64_q31(&S1, inputBlock, outputBlock, BLOCKSIZE);
		arm_biquad_cas_df1_32x64_q31(&S2, outputBlock, outputBlock, BLOCKSIZE);
		arm_biquad_cascade_df1_fast_q31(&S3, outputBlock, outputBlock, BLOCKSIZE);
		arm_biquad_cascade_df1_fast_q31(&S4, outputBlock, outputBlock, BLOCKSIZE);
		arm_biquad_cascade_df1_fast_q31(&S5, outputBlock, outputBlock, BLOCKSIZE);
		arm_biquad_cascade_df1_fast_q31(&S6, outputBlock, outputBlock, BLOCKSIZE);
		arm_biquad_cascade_df1_fast_q31(&S7, outputBlock, outputBlock, BLOCKSIZE);
		arm_biquad_cascade_df1_fast_q31(&S8, outputBlock, outputBlock, BLOCKSIZE);

		//arm_q31_to_q15(outputBlock, outputBuffer + i, BLOCKSIZE);

		 //arm_scale_f32(outputF32 + (i * BLOCKSIZE), 8.0f, outputF32 + (i * BLOCKSIZE), BLOCKSIZE);
	}


}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


