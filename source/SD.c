/***************************************************************************//**
  @file     SDHC.h
  @brief    Driver MCAL para SDHC
  @author   Grupo 2
 ******************************************************************************/
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "SD.h"
#include "SDHC.h"
#include "pit.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define SD_ARG_PATTERN 				(0b10110010)

#define SD_ARG_CMD8_PTT_MASK		(0xFFU)
#define SD_ARG_CMD8_PTT_SHIFT		(0x0U)
#define SD_ARG_CMD8_PTT(x)			(((uint32_t)(x)<<SD_ARG_CMD8_PTT_SHIFT) & SD_ARG_CMD8_PTT_MASK)

#define SD_ARG_CMD8_VHS_MASK		(0xF00U)
#define SD_ARG_CMD8_VHS_SHIFT		(0x08U)
#define SD_ARG_CMD8_VHS(x)			(((uint32_t)(x)<<SD_ARG_CMD8_VHS_SHIFT) & SD_ARG_CMD8_VHS_MASK)

#define SD_ARG_CMD55_RCA_MASK		(0xFF00U)
#define SD_ARG_CMD55_RCA_SHIFT		(0x10U)
#define SD_ARG_CMD55_RCA(x)			(((uint32_t)(x)<<SD_ARG_CMD55_RCA_SHIFT) & SD_ARG_CMD55_RCA_MASK)

#define SD_ARG_ACMD41_OCRS_MASK		(0xFFFF00U)
#define SD_ARG_ACMD41_OCRS_SHIFT	(0x8U)
#define SD_ARG_ACMD41_OCRS(x)		(((uint32_t)(x)<<SD_ARG_ACMD41_OCRS_SHIFT) & SD_ARG_ACMD41_OCRS_MASK)
#define SD_ARG_ACMD41_S18R_MASK		(0x1000000U)
#define SD_ARG_ACMD41_S18R_SHIFT	(0x18U)
#define SD_ARG_ACMD41_S18R(x)		(((uint32_t)(x)<<SD_ARG_ACMD41_S18R_SHIFT) & SD_ARG_ACMD41_S18R_MASK)
#define SD_ARG_ACMD41_XPS_MASK		(0x10000000U)
#define SD_ARG_ACMD41_XPS_SHIFT		(0x28U)
#define SD_ARG_ACMD41_XPS(x)		(((uint32_t)(x)<<SD_ARG_ACMD41_XPS_SHIFT) & SD_ARG_ACMD41_XPS_MASK)
#define SD_ARG_ACMD41_HCS_MASK		(0x40000000U)
#define SD_ARG_ACMD41_HCS_SHIFT		(0x1EU)
#define SD_ARG_ACMD41_HCS(x)		(((uint32_t)(x)<<SD_ARG_ACMD41_HCS_SHIFT) & SD_ARG_ACMD41_HCS_MASK)
#define SD_ARG_ACMD41_BUSY_MASK		(0x80000000U)
#define SD_ARG_ACMD41_BUSY_SHIFT	(0x1FU)
#define SD_ARG_ACMD41_BUSY(x)		(((uint32_t)(x)<<SD_ARG_ACMD41_BUSY_SHIFT) & SD_ARG_ACMD41_BUSY_MASK)


#define SD_RESP_R7_PTT_MASK			(0xFFU)
#define SD_RESP_R7_PTT_SHIFT		(0x0U)
#define SD_RESP_R7_PTT(x)			(((uint32_t)(x)<<SD_RESP_R7_PTT_SHIFT) & SD_RESP_R7_PTT_MASK)
#define SD_RESP_R7_VACC_MASK		(0xF00U)
#define SD_RESP_R7_VACC_SHIFT		(0x08U)
#define SD_RESP_R7_VACC(x)			(((uint32_t)(x)<<SD_RESP_R7_VACC_SHIFT) & SD_RESP_R7_VACC_MASK)

#define SD_RESP_R3_OCRS_MASK		(0xFFFF00U)
#define SD_RESP_R3_OCRS_SHIFT		(0x08U)
#define SD_RESP_R3_OCRS(x)			(((uint32_t)(x)<<SD_RESP_R3_OCRS_SHIFT) & SD_RESP_R3_OCRS_MASK)
#define SD_RESP_R3_S18R_MASK		(0x1000000U)
#define SD_RESP_R3_S18R_SHIFT		(0x18U)
#define SD_RESP_R3_S18R(x)			(((uint32_t)(x)<<SD_RESP_R3_S18R_SHIFT) & SD_RESP_R3_S18R_MASK)
#define SD_RESP_R3_UHS2_MASK		(0x20000000U)
#define SD_RESP_R3_UHS2_SHIFT		(0x1DU)
#define SD_RESP_R3_UHS2(x)			(((uint32_t)(x)<<SD_RESP_R3_UHS2_SHIFT) & SD_RESP_R3_UHS2_MASK)
#define SD_RESP_R3_CCS_MASK			(0x40000000U)
#define SD_RESP_R3_CCS_SHIFT		(0x1EU)
#define SD_RESP_R3_CCS(x)			(((uint32_t)(x)<<SD_RESP_R3_CCS_SHIFT) & SD_RESP_R3_UHS2_CCS)
#define SD_RESP_R3_BUSY_MASK		(0x80000000U)
#define SD_RESP_R3_BUSY_SHIFT		(0x1FU)
#define SD_RESP_R3_BUSY(x)			(((uint32_t)(x)<<SD_RESP_R3_BUSY_SHIFT) & SD_RESP_R3_BUSY_MASK)

#define SD_RESP_R6_RCA_MASK			(0xFF00U)
#define SD_RESP_R6_RCA_SHIFT		(0x10U)
#define SD_RESP_R6_RCA(x)			(((uint32_t)(x)<<SD_RESP_R6_RCA_SHIFT) & SD_RESP_R6_RCA_MASK)


#define SD_OCR_VW_MASK				(0xFFFFFFU)
#define SD_OCR_VW_SHIFT				(0x0U)
#define SD_OCR_VW(x)				(((uint32_t)(x)<<SD_OCR_VW_SHIFT) & SD_OCR_VW_MASK)

#define SD_CID_bits120to96_MASK		(0x00FFFFFFU)

//PIT
#define PIT_TIME_US					(1000000)

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
enum voltageSupplied {
	NOT_DEF 	= 0b0000,
	V2_7_TOV3_6	= 0b0001,
	LVR			= 0b0010,
	RESV_1		= 0b0100,
	RESV_2		= 0b1000
};

enum voltageWindowOCR {
	RES_LVR		= 0x000080,
	V2_7TOV2_8	= 0x008000,
	V2_8TOV2_9	= 0x010000,
	V2_9TOV3_0	= 0x020000,
	V3_0TOV3_1	= 0x040000,
	V3_1TOV3_2	= 0x080000,
	V3_2TOV3_3	= 0x100000,
	V3_3TOV3_4	= 0x200000,
	V3_4TOV3_5	= 0x400000,
	V3_5TOV3_6	= 0x800000
};
/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
static bool SD_resetToIDLE();
static bool SD_operationConditionValidation();
static void timerPIT(void);



/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/



/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
static bool isSDHC;
static bool isUHS2;
static uint32_t cid[4];
static uint32_t rca;

static bool isOneSecondPassed;


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void SD_init(){
	SDHC_init();
	//SDHC_enableCardDedection();
	pitInit();


}
bool SD_isSDcard(){
	return SDHC_isCardDetected();
}

bool SD_initializationProcess(){
	bool success = SD_resetToIDLE();

	if(success){
		success = SD_operationConditionValidation();
		if(success){
			//transfer data init

		}
		else{
			return false;
		}

	}
	else{
		return false;
	}
	return success;
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
static bool SD_resetToIDLE(){
	SDHC_boot(BUS_4bit,32);
	while(SDHC_getCMDstatus());

	SDHC_cmd_t cmd;
	bool no_err;
	cmd.cmd_index = 0;
	cmd.cmd_type = NORMAL_CMD;
	cmd.cmd_resp_type = NO_R;
	cmd.cmd_transfer_type = NO_DATA_T;

	no_err = SDHC_sendCMD(&cmd);
	return no_err;
}


static bool SD_operationConditionValidation(){
	bool no_err;
	bool validation = false;
	bool cmd8Response = false;
	SDHC_cmd_t cmd;

	//CMD8 (SEND_IF_COND) verificar Vdd de SDcard
	cmd.cmd_index = 8;
	cmd.cmd_arg = SD_ARG_CMD8_PTT(SD_ARG_PATTERN) | SD_ARG_CMD8_VHS(V2_7_TOV3_6);
	cmd.cmd_type = NORMAL_CMD;
	cmd.cmd_resp_type = R7;
	cmd.cmd_transfer_type = NO_DATA_T;
	no_err = SDHC_sendCMD(&cmd);
	if(no_err){
		bool pattern_ok = ((cmd.response[0] & SD_RESP_R7_PTT_MASK) == SD_RESP_R7_PTT(SD_ARG_PATTERN))? true : false;
		bool vAccepted = ((cmd.response[0] & SD_RESP_R7_VACC_MASK) == SD_RESP_R7_VACC(SD_ARG_PATTERN))? true : false;
		if(pattern_ok && vAccepted){
			cmd8Response = true;
		}
		else{
			cmd8Response = false;
		}
	}

	//CMD55 (APP_CMD) y despues ACMD41 (SD_SEND_OP_COND)
	if(cmd8Response){
		cmd.cmd_index = 55;
		cmd.cmd_arg = SD_ARG_CMD55_RCA(0x0000);
		cmd.cmd_type = NORMAL_CMD;
		cmd.cmd_resp_type = R1;
		cmd.cmd_transfer_type = NO_DATA_T;
		no_err = SDHC_sendCMD(&cmd);

		if(no_err){
			uint32_t ocr = SD_OCR_VW(V3_2TOV3_3) | SD_OCR_VW(V3_3TOV3_4);
			//primer ACMD41 para iniciar proceso de inicializacion (ocr != 0)
			cmd.cmd_index = 41;
			cmd.cmd_arg = SD_ARG_ACMD41_OCRS(ocr>>8) | SD_ARG_ACMD41_XPS_MASK | SD_ARG_ACMD41_HCS_MASK;
			cmd.cmd_type = NORMAL_CMD;
			cmd.cmd_resp_type = R3;
			cmd.cmd_transfer_type = NO_DATA_T;

			bool initializationComplete = false;
			uint8_t tries = 5;
			pitSetIRQFunc(PIT_0, timerPIT);
			pitSetAndBegin(PIT_0,PIT_TIME_US);
			isOneSecondPassed = false;
			do{
				no_err = SDHC_sendCMD(&cmd);
				if(no_err){
					if(cmd.response[0] & SD_RESP_R3_BUSY_MASK){
						//Inicializacion completada
						isSDHC = (cmd.response[0] & SD_RESP_R3_CCS_MASK) ? true : false;
						isUHS2 = (cmd.response[0] & SD_RESP_R3_UHS2_MASK) ? true : false;
						initializationComplete = true;
					}
					else{
						//esperar 1 segundo antes de repetir
						pitSetIRQFunc(PIT_0, timerPIT);
						pitSetAndBegin(PIT_0,PIT_TIME_US);
						while(!isOneSecondPassed); //timer cuenta cada 0.1s => 10.timer = 1seg
						isOneSecondPassed = false;
						cmd.cmd_arg = 0;
					}
				}
				else{
					tries = 0;
				}
			}while((!initializationComplete) && tries--);
			pitDisableIRQFunc(PIT_0);
			if(!tries){
				validation = false;
			}
			else{
				//CID number con CMD2
				cmd.cmd_index = 2;
				cmd.cmd_arg = 0;
				cmd.cmd_type = NORMAL_CMD;
				cmd.cmd_resp_type = R2;
				cmd.cmd_transfer_type = NO_DATA_T;
				no_err = SDHC_sendCMD(&cmd);
				if(no_err){
					uint8_t i = 0;
					for(i = 0; i < cmd.cmd_lenght_resp; i++ ){
						cid[i] = cmd.response[i];
					}
					cid[3] &= SD_CID_bits120to96_MASK;

					//asignar RCA con CMD3
					cmd.cmd_index = 3;
					cmd.cmd_arg = 0;
					cmd.cmd_type = NORMAL_CMD;
					cmd.cmd_resp_type = R6;
					cmd.cmd_transfer_type = NO_DATA_T;
					no_err = SDHC_sendCMD(&cmd);
					if(no_err){
						validation = true;
						rca = (cmd.response[0]&SD_RESP_R6_RCA_MASK) >> SD_RESP_R6_RCA_SHIFT;
					}
					else{
						validation = false;
					}
				}
				else{
					validation = false;
				}
			}
		}
		else{
			validation = false;
		}
	}
	return validation;
}

static void timerPIT(void){
	isOneSecondPassed = true;
}
