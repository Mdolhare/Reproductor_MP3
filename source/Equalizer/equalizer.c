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
#define BLOCKSIZE 32

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

static int32_t biquadStateBand1Q31[4 * NUMSTAGES];
static int32_t biquadStateBand2Q31[4 * NUMSTAGES];
static int32_t biquadStateBand3Q31[4 * NUMSTAGES];
static int32_t biquadStateBand4Q31[4 * NUMSTAGES];
static int32_t biquadStateBand5Q31[4 * NUMSTAGES];

static arm_biquad_casd_df1_inst_q31 S1;
static arm_biquad_casd_df1_inst_q31 S2;
static arm_biquad_casd_df1_inst_q31 S3;
static arm_biquad_casd_df1_inst_q31 S4;
static arm_biquad_casd_df1_inst_q31 S5;

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void equalizerInit(int8_t gainDB) {

	arm_biquad_cascade_df1_init_q31(&S1, NUMSTAGES,
			(int32_t*) &coeffTable[190*0 + 10*(gainDB[0] + 9)],
			 &biquadStateBand1Q31[0], NUMSTAGES);
	arm_biquad_cascade_df1_init_q31(&S2, NUMSTAGES,
		    (int32_t*) &coeffTable[190*1 + 10*(gainDB[1] + 9)],
		    &biquadStateBand2Q31[0], NUMSTAGES);
	arm_biquad_cascade_df1_init_q31(&S3, NUMSTAGES,
		    (int32_t*) &coeffTable[190*2 + 10*(gainDB[2] + 9)],
		    &biquadStateBand3Q31[0], NUMSTAGES);
	arm_biquad_cascade_df1_init_q31(&S4, NUMSTAGES,
		    (int32_t*) &coeffTable[190*3 + 10*(gainDB[3] + 9)],
		    &biquadStateBand4Q31[0], NUMSTAGES);
	arm_biquad_cascade_df1_init_q31(&S5, NUMSTAGES,
		    (int32_t*) &coeffTable[190*4 + 10*(gainDB[4] + 9)],
		    &biquadStateBand5Q31[0], NUMSTAGES);

}

void equalizerFilter(int32_t* inputBuffer, int32_t* outputBuffer) {

	arm_biquad_cascade_df1_fast_q31(&S1, inputBuffer, outputBuffer, BLOCKSIZE);
	arm_biquad_cascade_df1_fast_q31(&S2, outputBuffer, outputBuffer, BLOCKSIZE);
	arm_biquad_cascade_df1_fast_q31(&S3, outputBuffer, outputBuffer, BLOCKSIZE);
	arm_biquad_cascade_df1_fast_q31(&S4, outputBuffer, outputBuffer, BLOCKSIZE);
	arm_biquad_cascade_df1_fast_q31(&S5, outputBuffer, outputBuffer, BLOCKSIZE);

}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


