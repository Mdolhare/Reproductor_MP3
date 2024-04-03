/*******************************************************************************
  @file     +PWM.c+
  @brief    +Driver para PWM+
  @author   +GRUPO 2+
 ******************************************************************************/
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "InputCapture.h"
#include "FTMG2.h"
#include "gpio.h"
#include "MK64F12.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define CANT_IC_MED 2


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
static void saveCapture(FTMn_t ftm, uint8_t channel);
static void saveCapture2(FTMn_t ftm, uint8_t channel);
static void ICsearchFTMandChannel(pin_t pin, FTMn_t * ftm, FTMChannel_t * channel);
static void ICconfigPin(pin_t pin);


/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
static uint16_t IC_med0[CANT_FTM][CANT_CHN];
static uint16_t IC_medPeriodo[CANT_FTM][CANT_CHN];
static bool is2Edge[CANT_FTM][CANT_CHN];

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void IC_init(IC_Pin_t pin, ICMode_t mode, IC_pinID_t * pin_id){
	FTMn_t ftm;
	FTMChannel_t channel;

	ICsearchFTMandChannel(pin, &ftm, &channel);
	pin_id->ftm_id = ftm;
	pin_id->channel = channel;

	ICconfigPin(pin);

	FTM_Init(ftm);

	FTMEdge_t edge;
	switch(mode){
		case IC_riseEdge:
			edge = FTM_eRising;
		break;
		case IC_fallEdge:
			edge = FTM_eFalling;
		break;
		case IC_bothEdge:
			edge = FTM_eEither;
		break;
		default:
			edge = FTM_eEither;
		break;
	}

	for(int i = 0; i < CANT_FTM ; i++ ){
		for(int j = 0; j < CANT_CHN; j++){
			is2Edge[i][j] = false;
			IC_med0[i][j] = 0;
			IC_medPeriodo[i][j] = 0;
		}
	}


	FTM_ConfigIC(ftm, channel, edge, saveCapture);
}

void IC_setEnableDMARequest(IC_pinID_t * pin_id){
	FTM_setDMARequest(pin_id->ftm_id, pin_id->channel);
}

uint32_t * getAdressCnVDMA(IC_pinID_t * pin_id){
	return FTM_GetAdressCnV(pin_id->ftm_id, pin_id->channel);
}

bool IC_isNewPeriod(IC_pinID_t * pin_id){
	return is2Edge[pin_id->ftm_id][pin_id->channel];
}

//devuelve entero con los microsegundos del periodo
uint16_t IC_getPeriod(IC_pinID_t * pin_id){
//	gpioWrite(PORTNUM2PIN(PE,26), 0);
	is2Edge[pin_id->ftm_id][pin_id->channel] = false;
	uint16_t med = 0;
	med = IC_medPeriodo[pin_id->ftm_id][pin_id->channel] + 1;
	uint16_t periodo = (uint16_t)((FTM_IC_RES_NS * (uint32_t)med)/1000);
	return periodo;
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
static void saveCapture(FTMn_t ftm_id, uint8_t channel){
	static uint8_t state = 0;
	static uint8_t numValues = 0;
	static bool first = true;
	/*if(first){
		if(state == 0)
		{
			IC_med0[ftm_id][channel] = FTM_GetCounter (ftm_id, channel); //
			state=1;
		}
		else if(state == 1)
		{
			uint16_t cnv = FTM_GetCounter(ftm_id, channel);
			IC_medPeriodo[ftm_id][channel] = cnv - IC_med0[ftm_id][channel];
			state = 0;					// Set break point here and watch "med" value
			is2Edge[ftm_id][channel] = true;
			first = false;
			IC_med0[ftm_id][channel] = cnv;
		}
	}
	else{
		uint16_t cnv = FTM_GetCounter(ftm_id, channel);
		IC_medPeriodo[ftm_id][channel] = cnv - IC_med0[ftm_id][channel];
		IC_med0[ftm_id][channel] = cnv;
		is2Edge[ftm_id][channel] = true;
	}*/
	if(state == 0)
	{
		IC_med0[ftm_id][channel] = FTM_GetCounter (ftm_id, channel); //
		state=1;
	}
	else if(state == 1)
	{
		uint16_t cnv = FTM_GetCounter(ftm_id, channel);
		IC_medPeriodo[ftm_id][channel] = cnv - IC_med0[ftm_id][channel];
		state = 0;					// Set break point here and watch "med" value
		is2Edge[ftm_id][channel] = true;
	//	first = false;
	//	IC_med0[ftm_id][channel] = cnv;
	}
/*	uint16_t cnv = FTM_GetCounter(ftm_id, channel);
		IC_medPeriodo[ftm_id][channel] = cnv - IC_med0[ftm_id][channel];
		IC_med0[ftm_id][channel] = cnv;
		is2Edge[ftm_id][channel] = true;
*/
}
static void saveCapture2(FTMn_t ftm_id, uint8_t channel){
	uint16_t cnv_0 = 0;
	uint16_t cnv_1 = 0;

	cnv_0 = FTM_GetCounter (ftm_id, channel-1);

	cnv_1 = FTM_GetCounter (ftm_id, channel);

	IC_medPeriodo[ftm_id][channel-1] = cnv_1 - cnv_0;

	is2Edge[ftm_id][channel-1] = true;

	FTM2->CONTROLS[0].CnSC &= ~FTM_CnSC_CHF_MASK;
	FTM2->CONTROLS[1].CnSC &= ~FTM_CnSC_CHF_MASK;



	/*
	if(state == 0)
	{
		IC_med0[ftm_id][channel] = FTM_GetCounter (ftm_id, channel); //
		state=1;
	}
	else if(state == 1)
	{
		uint16_t cnv = FTM_GetCounter(ftm_id, channel);
		IC_medPeriodo[ftm_id][channel] = cnv - IC_med0[ftm_id][channel];
		state = 0;					// Set break point here and watch "med" value
		is2Edge[ftm_id][channel] = true;
	}
*/
}
static void saveCapture3(FTMn_t ftm_id, uint8_t channel){
	static uint8_t state = 0;
	static uint8_t numValues = 0;
	static bool first = true;

	if(state == 0)
	{
		IC_med0[ftm_id][channel] = FTM_GetCounter (ftm_id, channel); //
		state=1;
	}
	else if(state == 1)
	{
		uint16_t cnv = FTM_GetCounter(ftm_id, channel);
		IC_medPeriodo[ftm_id][channel] = cnv - IC_med0[ftm_id][channel];
		state = 0;					// Set break point here and watch "med" value
		is2Edge[ftm_id][channel] = true;
	//	first = false;
	//	IC_med0[ftm_id][channel] = cnv;
	}

}

static void ICsearchFTMandChannel(pin_t pin, FTMn_t * ftm, FTMChannel_t * channel){
	switch(pin){
		case PTD0_IC_0:
			*ftm = FTM_3;
			*channel = 0;
			break;
		case PTD1_IC_1:
			*ftm = FTM_3;
			*channel = 1;
			break;
		case CMP0_IC_2:
		case CMP1_IC_3:
			*ftm = FTM_2;
			*channel = 0;
			break;
	}
}

static void ICconfigPin(pin_t pin){

	switch(pin){
		case PTD0_IC_0:
		case PTD1_IC_1:
			*PCR_ADRESS(pin) = PORT_PCR_MUX(PORT_mAlt4);
			break;
		case CMP0_IC_2:
			FTM_setCMPasICinput(FTM_inCMP0);
			break;
		case CMP1_IC_3:
			FTM_setCMPasICinput(FTM_inCMP1);
			break;
	}
}

