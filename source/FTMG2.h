/***************************************************************************//**
  @file     FTMG2.h
  @brief    Driver para FTM
  @author   Grupo 2
 */
/****************************************************************************
 * PTE5 FTM3_CH0
 * PTE6 FTM3_CH1
 * PTA0 FTM0_CH5 ALT3 -----------------------> OUTPUT FRDM-K64F
 * PTA1 FTM0_CH6 ALT3 -----------------------> OUTPUT FRDM-K64F
 * PTA2 FTM0_CH7 ALT3 -----------------------> OUTPUT FRDM-K64F
 * PTA3 FTM0_CH0 ALT3
 * PTA4 FTM0_CH1 ALT3
 * PTA5 FTM0_CH2 ALT3
 * PTA12 FTM1_CH0/FTM1_QD_PHA
 * PTA13 FTM1_CH1/FTM1_QD_PHB
 * PTA18 FTM0_FLT2/FLT_CLKIN0
 * PTA19 FTM1_FLT0/FLT_CLKIN1
 * PTB0 FTM1_CH0/FTM1_QD_PHA
 * PTB1 FTM1_CH1/FTM1_QD_PHB
 * PTB2 FTM0_FLT3 ALT6 -----------------------> OUTPUT FRDM-K64F
 * PTB3 FTM0_FLT0 ALT6 -----------------------> OUTPUT FRDM-K64F
 * PTB10 FTM0_FLT1 ALT6 -----------------------> OUTPUT FRDM-K64F
 * PTB11 FTM0_FLT2 ALT6 -----------------------> OUTPUT FRDM-K64F
 * PTB16 FTM_CLKIN0
 * PTB17 FTM_CLKIN1
 * PTB18 FTM2_CH0/FTM2_QD_PHA ALT3/6 -----------------------> OUTPUT FRDM-K64F
 * PTB19 FTM2_CH1/FTM2_QD_PHB ALT3/6 -----------------------> OUTPUT FRDM-K64F
 * PTC1 FTM0_CH0 ALT4 -----------------------> OUTPUT FRDM-K64F
 * PTC2 FTM0_CH1 ALT4 -----------------------> OUTPUT FRDM-K64F
 * PTC3 FTM0_CH2 ALT4 -----------------------> OUTPUT FRDM-K64F
 * PTC4 FTM0_CH3 ALT4 -----------------------> OUTPUT FRDM-K64F
 * PTC5 FTM0_CH2 ALT7 -----------------------> OUTPUT FRDM-K64F
 * PTC8 FTM3_CH4 ALT3 -----------------------> OUTPUT FRDM-K64F
 * PTC9 FTM3_CH5/FTM2_FLT0 ALT3/6 -----------------------> OUTPUT FRDM-K64F
 * PTC10 FTM3_CH6 ALT3 -----------------------> OUTPUT FRDM-K64F
 * PTC11 FTM3_CH7 ALT3 -----------------------> OUTPUT FRDM-K64F
 * PTC12 FTM3_FLT0
 * PTD0 FTM3_CH0 ALT4 -----------------------> OUTPUT FRDM-K64F
 * PTD1 FTM3_CH1 ALT4 -----------------------> OUTPUT FRDM-K64F
 * PTD2 FTM3_CH2 ALT4 -----------------------> OUTPUT FRDM-K64F
 * PTD3 FTM3_CH3 ALT4 -----------------------> OUTPUT FRDM-K64F
 * PTD4 FTM0_CH4
 * PTD5 FTM0_CH5
 * PTD6 FTM0_CH6/FTM0_FLT0
 * PTD7 FTM0_CH7/FTM0_FLT1
 *
 */
#ifndef FTMG2_H_
#define FTMG2_H_


/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
//#include "hardware.h"
#include <stdbool.h>
#include "port.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define FTM_CH_0 0
#define FTM_CH_1 1
#define FTM_CH_2 2
#define FTM_CH_3 3
#define FTM_CH_4 4
#define FTM_CH_5 5
#define FTM_CH_6 6
#define FTM_CH_7 7

#define FTM_CLOCK_HZ 50000000UL

#define FTM_PWM_RES_NS 80
#define FTM_IC_RES_NS 640

#define CANT_FTM 4
#define CANT_CHN 8




/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef FTM_Type * FTM_t;
typedef uint16_t FTMData_t;
typedef uint8_t FTMChannel_t; /* FTM0/FTM3: Channel 1-8; FTM1/FTM2: Channel 1-2 */


typedef enum {
	FTM_0,
	FTM_1,
	FTM_2,
	FTM_3
} FTMn_t;

typedef enum
{
	FTM_mInputCapture,
	FTM_mOutputCompare,
	FTM_mPulseWidthModulation,
} FTMMode_t;

typedef enum
{
	FTM_eRising 		= 0x01,
	FTM_eFalling 		= 0x02,
	FTM_eEither 		= 0x03,
} FTMEdge_t;

typedef enum
{
	FTM_eToggle 		= 0x01,
	FTM_eClear 			= 0x02,
	FTM_eSet 			= 0x03,
} FTMEffect_t;

typedef enum
{
	FTM_lAssertedHigh	= 0x02,
	FTM_lAssertedLow 	= 0x03,
} FTMLogic_t;

typedef enum
{
	FTM_PSC_x1		= 0x00,
	FTM_PSC_x2		= 0x01,
	FTM_PSC_x4		= 0x02,
	FTM_PSC_x8		= 0x03,
	FTM_PSC_x16		= 0x04,
	FTM_PSC_x32		= 0x05,
	FTM_PSC_x64		= 0x06,
	FTM_PSC_x128	= 0x07,

} FTM_Prescal_t;

typedef enum
{
	FTM_inCMP0 = 1,
	FTM_inCMP1,
} FTM_CMPasICInput_t;


typedef void (*FTM_IrqFun_t)(FTMn_t,uint8_t);
typedef void (*FTM_IrqOVFun_t)(void);

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

//__ISR__ 	FTM0_IRQHandler					 (void);
//__ISR__ 	FTM1_IRQHandler					 (void);
//__ISR__ 	FTM2_IRQHandler					 (void);
//__ISR__ 	FTM3_IRQHandler					 (void);

void 		FTM_Init 						 (FTMn_t);

void        FTM_SetPrescaler 				 (FTM_t, FTM_Prescal_t);
void 		FTM_SetModulus 					 (FTM_t ftm, FTMData_t cntin, FTMData_t cnt, FTMData_t data);
FTMData_t 	FTM_GetModulus 					 (FTMn_t);

void 		FTM_StartClock					 (FTM_t);
void 		FTM_StopClock					 (FTM_t);

void 		FTM_SetOverflowMode   			 (FTMn_t, bool, FTM_IrqOVFun_t); //externa
bool 		FTM_IsOverflowPending 			 (FTM_t);
void 		FTM_ClearOverflowFlag 			 (FTM_t);

void        FTM_SetWorkingMode				 (FTM_t, FTMChannel_t, FTMMode_t);
FTMMode_t   FTM_GetWorkingMode				 (FTM_t, FTMChannel_t);
void        FTM_SetInputCaptureEdge 		 (FTM_t, FTMChannel_t, FTMEdge_t);
FTMEdge_t   FTM_GetInputCaptureEdge 		 (FTM_t, FTMChannel_t);
void        FTM_SetOutputCompareEffect 	 	 (FTM_t, FTMChannel_t, FTMEffect_t);
FTMEffect_t FTM_GetOutputCompareEffect 		 (FTM_t, FTMChannel_t);
void        FTM_SetPulseWidthModulationLogic (FTM_t, FTMChannel_t, FTMLogic_t);
FTMLogic_t  FTM_GetPulseWidthModulationLogic (FTM_t, FTMChannel_t);

void        FTM_SetCounter 					 (FTMn_t, FTMChannel_t, FTMData_t); //externa
void 		FTM_SetCounterPWM 				 (FTMn_t, FTMChannel_t, FTMData_t);
FTMData_t   FTM_GetCounter 					 (FTMn_t, FTMChannel_t);

void 		FTM_SetInterruptMode   			 (FTM_t, FTMChannel_t, bool);
bool 		FTM_IsInterruptPending 			 (FTM_t, FTMChannel_t);
void 		FTM_ClearInterruptFlag 			 (FTM_t, FTMChannel_t);

uint32_t *	FTM_GetAdressCnV				 (FTMn_t, FTMChannel_t);
void 		FTM_setDMARequest				 (FTMn_t, FTMChannel_t);
//****************************************************************
//PWM
void FTM_ConfigEPWM(FTMn_t, uint16_t, FTMChannel_t);

//IC
void FTM_ConfigIC(FTMn_t, FTMChannel_t, FTMEdge_t, FTM_IrqFun_t);
void FTM_setCMPasICinput(FTM_CMPasICInput_t);

#endif /* FTMG2_H_ */
