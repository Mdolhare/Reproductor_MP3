/*******************************************************************************
  @file     +FTM.c+
  @brief    +Driver para FTMx+
  @author   +GRUPO 2+
 ******************************************************************************/
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "MK64F12.h"
#include "FTMG2.h"
#include "gpio.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define CANT_FTM 4
#define CANT_CHANNELS 8
#define CANT_CHANNELS_FTM2 2
#define CANT_CHANNELS_FTM1 2

#define FTM_PWMLOAD_CHnSEL(chn) 		(1<<(chn & 0x7))
#define FTM_FILTER_CHnFVAL(chn,val) 	( val << (chn * 4) )
#define FTM_COMBINE_SYNCENn(chn) 		( FTM_COMBINE_SYNCEN0_MASK << ( ((chn & 0b110) >> 1)*8 )  )

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
static FTM_Type * const ftm_base[CANT_FTM] = FTM_BASE_PTRS;
static IRQn_Type const ftmIRQs_base[CANT_FTM] = FTM_IRQS;


/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
static FTM_IrqFun_t FTM_channelIRQs[CANT_FTM][CANT_CHANNELS];
static FTM_IrqOVFun_t FTM_ovIRQs[CANT_FTM];


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void FTM_Init (FTMn_t ftm_id){
	SIM->SCGC6 |= SIM_SCGC6_FTM0_MASK;
	SIM->SCGC6 |= SIM_SCGC6_FTM1_MASK;
	SIM->SCGC6 |= SIM_SCGC6_FTM2_MASK;
	SIM->SCGC3 |= SIM_SCGC3_FTM2_MASK;
	SIM->SCGC3 |= SIM_SCGC3_FTM3_MASK;

	NVIC_EnableIRQ(ftmIRQs_base[ftm_id]);

	//config de pwm
	ftm_base[ftm_id]->MODE |= FTM_MODE_FTMEN_MASK;

}

//****************************************************************************
//PWM
void FTM_ConfigEPWM(FTMn_t ftm_id, uint16_t mod, FTMChannel_t chn){
	//init counter
	FTM_t ftm = ftm_base[ftm_id];

	//StopClock
	FTM_StopClock(ftm);

	//Config FTMCounter
	FTM_SetPrescaler(ftm, FTM_PSC_x4);

	ftm->PWMLOAD = FTM_PWMLOAD_LDOK_MASK | FTM_PWMLOAD_CHnSEL(chn);

	FTMData_t cnt = 0;
	FTMData_t cntin = 0;
	FTM_SetModulus(ftm, cntin, cnt, mod);
	//FTM_SetOverflowMode(FTM0, true);

	FTM_SetWorkingMode(ftm, chn, FTM_mPulseWidthModulation);			// MSA  / B Elijo tipo de Modo
	FTM_SetPulseWidthModulationLogic(ftm, chn, FTM_lAssertedHigh);   // ELSA / B Config de PWM
	FTM_SetCounter(ftm_id, chn, mod/2);	//Duty

	//
	//Config sync PWM
	ftm->SYNC |= FTM_SYNC_CNTMAX_MASK;
	ftm->SYNCONF |= FTM_SYNCONF_SWWRBUF_MASK | FTM_SYNCONF_SYNCMODE_MASK ;
	ftm->COMBINE |= FTM_COMBINE_SYNCENn(chn);
	FTM_StartClock(ftm);	//Arranca modulo FTM0

}

void FTM_ConfigIC(FTMn_t ftm_id, FTMChannel_t chn, FTMEdge_t edge,FTM_IrqFun_t fun){
	FTM_t ftm = ftm_base[ftm_id];
	FTM_StopClock(ftm);
	//Config counter
	FTM_SetPrescaler(ftm, FTM_PSC_x32);
	FTMData_t cnt = 0;
	FTMData_t cntin = 0;
	FTMData_t mod = 0xFFFF;
	FTM_SetModulus(ftm, cntin, cnt, mod);

	//Config pin IC
	FTM_SetWorkingMode(ftm, chn, FTM_mInputCapture);   // Select IC Function
//	ftm->CONTROLS[chn].CnSC |= FTM_CnSC_MSA(1);
	//ftm->CONTROLS[chn+1].CnSC |= FTM_CnSC_MSA(1);

	FTM_SetInputCaptureEdge (ftm, chn, edge);  // Capture on edge
//	FTM_SetInputCaptureEdge (ftm, chn+1, FTM_eFalling);  // Capture on edge

//	ftm->COMBINE |= 0x04; //DECAEN

	//config filter
	ftm->FILTER = (ftm->FILTER & ~FTM_FILTER_CHnFVAL(chn,0xF)) |
					FTM_FILTER_CHnFVAL(chn,0xF);


	//Config IRQ pin
	FTM_channelIRQs[ftm_id][chn] = fun;
	FTM_SetInterruptMode (ftm, chn, true);  // Enable interrupts

	FTM_ClearInterruptFlag(ftm,chn);
//	FTM_ClearInterruptFlag(ftm,chn+1);
	//ftm->COMBINE |= 0x08; //DECA0 on

	FTM_StartClock(ftm);
}

//****************************************************************** Setters
void FTM_SetPrescaler (FTM_t ftm, FTM_Prescal_t data)
{
	ftm->SC = (ftm->SC & ~FTM_SC_PS_MASK) | FTM_SC_PS(data);
}

void FTM_SetModulus (FTM_t ftm, FTMData_t cntin, FTMData_t cnt, FTMData_t mod)
{
	ftm->MOD = FTM_MOD_MOD(mod);
	ftm->CNTIN = FTM_CNTIN_INIT(cntin);
	ftm->CNT = FTM_CNT_COUNT(cnt);
}

FTMData_t FTM_GetModulus (FTMn_t ftm_id)
{
	return ftm_base[ftm_id]->MOD & FTM_MOD_MOD_MASK;
}

void FTM_StartClock (FTM_t ftm)
{
	ftm->SC |= FTM_SC_CLKS(0x01);
}

void FTM_StopClock (FTM_t ftm)
{
	ftm->SC &= ~FTM_SC_CLKS(0x01);
}

void FTM_SetOverflowMode (FTMn_t ftm_id, bool mode, FTM_IrqOVFun_t fun)
{
	FTM_t ftm = ftm_base[ftm_id];
	FTM_ovIRQs[ftm_id] = fun;
	ftm->SC = (ftm->SC & ~FTM_SC_TOIE_MASK) | FTM_SC_TOIE(mode);
}

bool FTM_IsOverflowPending (FTM_t ftm)
{
	return ftm->SC & FTM_SC_TOF_MASK;
}

void FTM_ClearOverflowFlag (FTM_t ftm)
{
	ftm->SC &= ~FTM_SC_TOF_MASK;
}

void FTM_SetWorkingMode (FTM_t ftm, FTMChannel_t channel, FTMMode_t mode)
{
	ftm->CONTROLS[channel].CnSC = (ftm->CONTROLS[channel].CnSC & ~(FTM_CnSC_MSB_MASK | FTM_CnSC_MSA_MASK)) |
			                      (FTM_CnSC_MSB( (mode >> 1) & 0x01 ) | FTM_CnSC_MSA((mode) & 0x01));
}

FTMMode_t FTM_GetWorkingMode (FTM_t ftm, FTMChannel_t channel)
{
	return (ftm->CONTROLS[channel].CnSC & (FTM_CnSC_MSB_MASK | FTM_CnSC_MSA_MASK)) >> FTM_CnSC_MSA_SHIFT;
}

void FTM_SetInputCaptureEdge (FTM_t ftm, FTMChannel_t channel, FTMEdge_t edge)
{
	ftm->CONTROLS[channel].CnSC = (ftm->CONTROLS[channel].CnSC & ~(FTM_CnSC_ELSB_MASK | FTM_CnSC_ELSA_MASK)) |
				                  (FTM_CnSC_ELSB((edge >> 1) & 0X01) | FTM_CnSC_ELSA((edge >> 0) & 0X01));
}

FTMEdge_t FTM_GetInputCaptureEdge (FTM_t ftm, FTMChannel_t channel)
{
	return (ftm->CONTROLS[channel].CnSC & (FTM_CnSC_ELSB_MASK | FTM_CnSC_ELSA_MASK)) >> FTM_CnSC_ELSA_SHIFT;
}

void FTM_SetOutputCompareEffect (FTM_t ftm, FTMChannel_t channel, FTMEffect_t effect)
{
	ftm->CONTROLS[channel].CnSC = (ftm->CONTROLS[channel].CnSC & ~(FTM_CnSC_ELSB_MASK | FTM_CnSC_ELSA_MASK)) |
				                  (FTM_CnSC_ELSB((effect >> 1) & 0X01) | FTM_CnSC_ELSA((effect >> 0) & 0X01));
}

FTMEffect_t FTM_GetOutputCompareEffect (FTM_t ftm, FTMChannel_t channel)
{
	return (ftm->CONTROLS[channel].CnSC & (FTM_CnSC_ELSB_MASK | FTM_CnSC_ELSA_MASK)) >> FTM_CnSC_ELSA_SHIFT;
}

void FTM_SetPulseWidthModulationLogic (FTM_t ftm, FTMChannel_t channel, FTMLogic_t logic)
{
	ftm->CONTROLS[channel].CnSC = (ftm->CONTROLS[channel].CnSC & ~(FTM_CnSC_ELSB_MASK | FTM_CnSC_ELSA_MASK)) |
				                  (FTM_CnSC_ELSB((logic >> 1) & 0X01) | FTM_CnSC_ELSA((logic >> 0) & 0X01));
}

FTMLogic_t FTM_GetPulseWidthModulationLogic (FTM_t ftm, FTMChannel_t channel)
{
	return (ftm->CONTROLS[channel].CnSC & (FTM_CnSC_ELSB_MASK | FTM_CnSC_ELSA_MASK)) >> FTM_CnSC_ELSA_SHIFT;
}

void FTM_SetCounter (FTMn_t ftm_id, FTMChannel_t channel, FTMData_t data)
{
	ftm_base[ftm_id]->CONTROLS[channel].CnV = FTM_CnV_VAL(data);
}

void FTM_SetCounterPWM (FTMn_t ftm_id, FTMChannel_t channel, FTMData_t data)
{
	ftm_base[ftm_id]->CONTROLS[channel].CnV = FTM_CnV_VAL(data);
	ftm_base[ftm_id]->SYNC |= FTM_SYNC_SWSYNC_MASK;

}

FTMData_t FTM_GetCounter (FTMn_t ftm_id, FTMChannel_t channel)
{
	return ftm_base[ftm_id]->CONTROLS[channel].CnV & FTM_CnV_VAL_MASK;
}

void FTM_SetInterruptMode (FTM_t ftm, FTMChannel_t channel, bool mode)
{
	ftm->CONTROLS[channel].CnSC = (ftm->CONTROLS[channel].CnSC & ~FTM_CnSC_CHIE_MASK) | FTM_CnSC_CHIE(mode);
}

bool FTM_IsInterruptPending (FTM_t ftm, FTMChannel_t channel)
{
	return ftm->CONTROLS[channel].CnSC & FTM_CnSC_CHF_MASK;
}

void FTM_ClearInterruptFlag (FTM_t ftm, FTMChannel_t channel)
{
	ftm->CONTROLS[channel].CnSC &= ~FTM_CnSC_CHF_MASK;
}

uint32_t *	FTM_GetAdressCnV (FTMn_t ftm_id, FTMChannel_t channel){
	return &ftm_base[ftm_id]->CONTROLS[channel].CnV;
}

void FTM_setDMARequest(FTMn_t ftm_id, FTMChannel_t channel){
	ftm_base[ftm_id]->CONTROLS[channel].CnSC |= FTM_CnSC_CHIE_MASK | FTM_CnSC_DMA_MASK;
}

void FTM_setCMPasICinput(FTM_CMPasICInput_t cmp){
	SIM->SOPT4 |= SIM_SOPT4_FTM2CH0SRC(cmp);
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
__ISR__ FTM0_IRQHandler(void){

	if((FTM0->SC & FTM_SC_TOF_MASK) && (FTM0->SC & FTM_SC_TOIE_MASK)){
		FTM0->SC &= ~FTM_SC_TOF_MASK;
		(*FTM_ovIRQs[FTM_0])();
	}
	for(int i = 0; i<CANT_CHANNELS;i++){
		if((FTM0->CONTROLS[i].CnSC & FTM_CnSC_CHF_MASK) && !(FTM0->CONTROLS[i].CnSC & FTM_CnSC_DMA_MASK)){
			FTM0->CONTROLS[i].CnSC &= ~FTM_CnSC_CHF_MASK;
			(*FTM_channelIRQs[FTM_0][i])(FTM_0,i);
		}
	}
}
__ISR__ 	FTM1_IRQHandler					 (void){
	if((FTM1->SC & FTM_SC_TOF_MASK) && (FTM1->SC & FTM_SC_TOIE_MASK)){
		FTM1->SC &= ~FTM_SC_TOF_MASK;
		(*FTM_ovIRQs[FTM_1])();
	}
	for(int i = 0; i<CANT_CHANNELS_FTM1;i++){
		if((FTM1->CONTROLS[i].CnSC & FTM_CnSC_CHF_MASK) && !(FTM1->CONTROLS[i].CnSC & FTM_CnSC_DMA_MASK)){
			FTM1->CONTROLS[i].CnSC &= ~FTM_CnSC_CHF_MASK;
			(*FTM_channelIRQs[FTM_1][i])(FTM_1,i);
		}
	}
}
__ISR__ 	FTM2_IRQHandler					 (void){
	if((FTM2->SC & FTM_SC_TOF_MASK) && (FTM3->SC & FTM_SC_TOIE_MASK)){
		FTM2->SC &= ~FTM_SC_TOF_MASK;
		gpioToggle(PORTNUM2PIN(PB,9));
		(*FTM_ovIRQs[FTM_2])();
	}
	for(int i = 0; i < CANT_CHANNELS_FTM2;i++){
		if((FTM2->CONTROLS[i].CnSC & FTM_CnSC_CHF_MASK) && (FTM2->CONTROLS[i].CnSC & FTM_CnSC_CHIE_MASK)){
			FTM2->CONTROLS[i].CnSC &= ~FTM_CnSC_CHF_MASK;
			//FTM2->CONTROLS[i-1].CnSC &= ~FTM_CnSC_CHF_MASK;
		//	gpioToggle(PORTNUM2PIN(PB,9));
			(*FTM_channelIRQs[FTM_2][i])(FTM_2,i);
			//FTM2->COMBINE |= 0x08; //DECA0 on
		}
	}
/*	gpioToggle(PORTNUM2PIN(PB,9));
	(*FTM_channelIRQs[FTM_2][1])(FTM_2,1);
	//FTM2->CONTROLS[0].CnSC &= ~FTM_CnSC_CHF_MASK;
	//FTM2->CONTROLS[1].CnSC &= ~FTM_CnSC_CHF_MASK;
*/
}
__ISR__ 	FTM3_IRQHandler					 (void){
	if((FTM3->SC & FTM_SC_TOF_MASK) && (FTM3->SC & FTM_SC_TOIE_MASK)){
		FTM3->SC &= ~FTM_SC_TOF_MASK;
		(*FTM_ovIRQs[FTM_3])();
	}
	for(int i = 0; i<CANT_CHANNELS;i++){
		if((FTM3->CONTROLS[i].CnSC & FTM_CnSC_CHF_MASK) && !(FTM3->CONTROLS[i].CnSC & FTM_CnSC_DMA_MASK)){
			FTM3->CONTROLS[i].CnSC &= ~FTM_CnSC_CHF_MASK;
			(*FTM_channelIRQs[FTM_3][i])(FTM_3,i);
		}
	}
}
