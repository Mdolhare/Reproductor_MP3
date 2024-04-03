/*******************************************************************************
  @file     +PWM.c+
  @brief    +Driver para PWM+
  @author   +GRUPO 2+
 ******************************************************************************/
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "PWM.h"
#include "FTMG2.h"

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
static void sarchFTMandChannel(pin_t pin, FTMn_t * ftm, FTMChannel_t * channel);


/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
static uint16_t mod;



/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
// period en micro segundos
void PWM_init(PWM_pin_t pin,PWM_pinID_t * pin_ID){

	FTMn_t ftm;
	FTMChannel_t channel = 0;
	uint8_t port = PIN2PORT(pin);
	//congif pin, search channel and ftm
	switch(port){
		case PA:
			ftm = FTM_0;
			*PCR_ADRESS(pin) = PORT_PCR_MUX(PORT_mAlt3) | PORT_PCR_DSE_MASK ;
			channel = PIN2NUM(pin) + 5; //num + 5 => pta0 -> CH5 (segun la tabla de FTMG2)
			break;
		case PC:
			if(PIN2NUM(pin) < 10){
				ftm = FTM_0;
				*PCR_ADRESS(pin) = PORT_PCR_MUX(PORT_mAlt4) /*| PORT_PCR_DSE_MASK*/ ;
				channel = PIN2NUM(pin) - 1;
			}
			else{
				ftm = FTM_3;
				*PCR_ADRESS(pin) = PORT_PCR_MUX(PORT_mAlt3) | PORT_PCR_DSE_MASK ;
				channel = PIN2NUM(pin) - 4;
			}
			break;
		case PD:
			ftm = FTM_3;
			*PCR_ADRESS(pin) = PORT_PCR_MUX(PORT_mAlt4) | PORT_PCR_DSE_MASK ;
			channel = PIN2NUM(pin); //coincide numero de pin con channel
			break;
	}

	pin_ID->ftm_id = ftm;
	pin_ID->channel = channel;


	FTM_Init(ftm);

	//period = resolution * mod
	mod = (PERIODO_NS) / FTM_PWM_RES_NS - 1 ;

	FTM_ConfigEPWM(ftm, mod, channel); //
	//config pin
}

uint32_t * PWM_getAdressCnVDMA(PWM_pin_t pin){
	FTMn_t ftm;
	FTMChannel_t channel = 0;
	sarchFTMandChannel(pin, &ftm, &channel);
	return FTM_GetAdressCnV(ftm,channel);
}

void PWM_setDutyPin(PWM_pin_t pin, float duty){
	FTMn_t ftm;
	FTMChannel_t channel = 0;
	sarchFTMandChannel(pin, &ftm, &channel);
	FTMData_t cnv = (FTMData_t)( (duty * (float)(mod +1)) / 100);
	FTM_SetCounterPWM(ftm, channel, cnv);	//Duty
}

void PWM_setDutyPinWithCnV(PWM_pinID_t * pin_ID, uint16_t cnv){
	FTM_SetCounterPWM(pin_ID->ftm_id, pin_ID->channel, cnv);	//Duty
}

uint16_t PWM_getCnVFromDuty(float duty){
	return (uint16_t)((duty * (float)(mod +1)) / 100);
}

float PWM_getMinDuty(){
	return (1.0/(float)mod)*100;
}

void PWM_initIRQ(PWM_pin_t pin, PWM_Fun_t fun){
	FTMn_t ftm;
	FTMChannel_t channel = 0;
	sarchFTMandChannel(pin, &ftm, &channel);
	FTM_SetOverflowMode(ftm, true, fun);
}

uint16_t PWM_scaleValueToCnVDuty(PWM_pinID_t * pin_ID ,uint16_t maxVal, uint16_t value){
	float scaleFactor = (float)FTM_GetModulus(pin_ID->ftm_id)/(float)maxVal;
	return (uint16_t)(value*scaleFactor);
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
static void sarchFTMandChannel(pin_t pin, FTMn_t * ftm, FTMChannel_t * channel){
	switch(PIN2PORT(pin)){
		case PA:
			*ftm = FTM_0;
			*channel = PIN2NUM(pin) + 5; //num + 5 => pta0 -> CH5 (segun la tabla de FTMG2)
			break;
		case PC:
			if(PIN2NUM(pin) < 10){
				*ftm = FTM_0;
				*channel = PIN2NUM(pin) - 1;
			}
			else{
				*ftm = FTM_3;
				*channel = PIN2NUM(pin) - 4;
			}
			break;
		case PD:
			*ftm = FTM_3;
			*channel = PIN2NUM(pin); //coincide numero de pin con channel
			break;
	}
}


