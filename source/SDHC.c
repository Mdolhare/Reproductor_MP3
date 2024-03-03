/***************************************************************************//**
  @file     SDHC.h
  @brief    Driver MCAL para SDHC
  @author   Grupo 2
 ******************************************************************************/
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "SDHC.h"
#include "port.h"
#include "MK64F12.h"
#include <stdbool.h>
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define DAT0		(PORTNUM2PIN(PE,1)) 	//SDHC0_D0
#define DAT1 		(PORTNUM2PIN(PE,0))		//SDHC0_D1
#define DAT2 		(PORTNUM2PIN(PE,5))		//SDHC0_D2
#define DAT3_CD 	(PORTNUM2PIN(PE,4))		//SDHC0_D3
#define CMD 		(PORTNUM2PIN(PE,3))		//SDHC0_CMD
#define CLK 		(PORTNUM2PIN(PE,2))		//SDHC0_DCLK
#define SW_DETECT 	(PORTNUM2PIN(PE,6))		//PTE6

#define SDHC_CMD_ERR 		(SDHC_IRQSTAT_CTOE_MASK | SDHC_IRQSTAT_CCE_MASK)
#define SDHC_CMD_DAT_ERR	(SDHC_IRQSTAT_DTOE_MASK | SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_AC12E_MASK)


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
enum cmd_resp_lenght_type{						//CMD response:
	NO_RESPONSE,					//
	RESPONSE_136bits,				//R2 crc enable
	RESPONSE_48bits,				//R3,4 index/crc disable // R1,5,6 index/crc enable
	RESPONSE_48bits_CHECK_BUSY		//R1b,5b index/crc enable
};


typedef struct {
	bool isCard;
	SDHC_errType err;
}SDHC_status;

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
static bool SDHC_checkErrInCMD(bool useDatLine);
static void SDHC_irqHandler();

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/


/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
static SDHC_status status;

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void SDHC_init(){

	//Config Hardware
	*PCR_ADRESS(DAT0) = PORT_PCR_MUX(PORT_mAlt4);
	*PCR_ADRESS(DAT1) = PORT_PCR_MUX(PORT_mAlt4);
	*PCR_ADRESS(DAT2) = PORT_PCR_MUX(PORT_mAlt4);
	*PCR_ADRESS(DAT3_CD) = PORT_PCR_MUX(PORT_mAlt4);
	*PCR_ADRESS(CMD) = PORT_PCR_MUX(PORT_mAlt4);
	*PCR_ADRESS(CLK) = PORT_PCR_MUX(PORT_mAlt4);
	*PCR_ADRESS(SW_DETECT) = PORT_PCR_MUX(PORT_mGPIO) | PORT_PCR_PE_MASK ;

	SIM->SCGC3 |= SIM_SCGC3_SDHC_MASK;
	SIM->SOPT2 &= ~SIM_SOPT2_SDHCSRC_MASK;

	NVIC_EnableIRQ(SDHC_IRQn);

}

void SDHC_enableCardDedection(){
	//Habilita lectura de evento
	SDHC->IRQSTATEN |= SDHC_IRQSTATEN_CINSEN_MASK | SDHC_IRQSTATEN_CRMSEN_MASK;

	//Dispara interrupcion
	SDHC->IRQSIGEN |= SDHC_IRQSIGEN_CINSIEN_MASK | SDHC_IRQSIGEN_CRMIEN_MASK;
}

void SDHC_setClockFrecuency(sdhc_prescaler presc, uint8_t div){
		SDHC->SYSCTL |= SDHC_SYSCTL_SDCLKFS(presc) | SDHC_SYSCTL_DVS(div);
}

SDHC_errType SDHC_getErrStatus(){
	return status.err;
}

bool SDHC_sendCMD(SDHC_cmd_t * cmd){
	uint32_t xfertypReg = 0;
	xfertypReg |= SDHC_XFERTYP_CMDINX(cmd->cmd_index) | SDHC_XFERTYP_CMDTYP(cmd->cmd_type);
	bool useDatLine = false;
	status.err = 0;
	switch(cmd->cmd_resp_type){
		case NO_R:
			xfertypReg |= SDHC_XFERTYP_RSPTYP(NO_RESPONSE);
			cmd->cmd_lenght_resp = CMD_LENGTH_0;
			break;
		case R2:
			xfertypReg |= SDHC_XFERTYP_RSPTYP(RESPONSE_136bits) | SDHC_XFERTYP_CCCEN_MASK;
			cmd->cmd_lenght_resp = CMD_LENGTH_136;
			break;
		case R3:
		case R4:
			xfertypReg |= SDHC_XFERTYP_RSPTYP(RESPONSE_48bits);
			cmd->cmd_lenght_resp = CMD_LENGTH_48;
			break;
		case R1:
		case R5:
		case R6:
			xfertypReg |= SDHC_XFERTYP_RSPTYP(RESPONSE_48bits) | SDHC_XFERTYP_CICEN_MASK
							| SDHC_XFERTYP_CCCEN_MASK;
			cmd->cmd_lenght_resp = CMD_LENGTH_48;
			break;
		case R1b:
		case R5b:
			xfertypReg |= SDHC_XFERTYP_RSPTYP(RESPONSE_48bits_CHECK_BUSY) | SDHC_XFERTYP_CICEN_MASK
							| SDHC_XFERTYP_CCCEN_MASK;
			cmd->cmd_lenght_resp = CMD_LENGTH_48;
			useDatLine = true;
			break;
	}

	if(cmd->cmd_transfer_type != NO_DATA_T){
		xfertypReg |= SDHC_XFERTYP_DPSEL_MASK |
					((cmd->cmd_transfer_type == SINGLE_T) ? 0 : SDHC_XFERTYP_MSBSEL_MASK)|
					((cmd->cmd_transfer_type == MULTIPLE_T) ? SDHC_XFERTYP_BCEN_MASK : 0)|
					((cmd->card_to_sdhc) ? SDHC_XFERTYP_DTDSEL_MASK : 0);
		xfertypReg |= ((cmd->cmd_transfer_type == MULTIPLE_T) ? SDHC_XFERTYP_AC12EN_MASK : 0);

		SDHC->BLKATTR |= SDHC_BLKATTR_BLKCNT(cmd->block_count);
	}

	//wait response of command
	while(! (SDHC->IRQSTAT & SDHC_IRQSTAT_CC_MASK) );
	SDHC->IRQSTAT |= SDHC_IRQSTAT_CC_MASK;

	uint8_t i;
	for(i = 0; i < cmd->cmd_lenght_resp; i++){
		cmd->response[i] = SDHC->CMDRSP[i];
	}

	bool err = SDHC_checkErrInCMD(useDatLine);

	return err;
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
static bool SDHC_checkErrInCMD(bool useDatLine){
	if( SDHC->PRSSTAT & (SDHC_CMD_ERR | (useDatLine ? SDHC_CMD_DAT_ERR:0) ) ){
		if(SDHC->PRSSTAT & SDHC_IRQSTAT_CTOE_MASK){
			status.err |= ERR_CMD_TIME_OUT;
		}
		if(SDHC->PRSSTAT & SDHC_IRQSTAT_CCE_MASK){
			status.err |= ERR_CMD_CRC;
		}
		if(useDatLine){
			if(SDHC->PRSSTAT & SDHC_IRQSTAT_DTOE_MASK){
				status.err |= ERR_CMD_DAT_TIME_OUT;
			}
			if(SDHC->PRSSTAT & SDHC_IRQSTAT_DEBE_MASK){
				status.err |= ERR_CMD_DAT_END_BIT;
			}
			if(SDHC->PRSSTAT & SDHC_IRQSTAT_AC12E_MASK){
				status.err |= ERR_CMD_DAT_AC12;
			}
		}
		return false;
	}
	return true;
}



static void SDHC_irqHandler(){

	if(SDHC->IRQSTAT & SDHC_IRQSTAT_CRM_MASK){
		SDHC->IRQSTAT |= SDHC_IRQSTAT_CRM_MASK;
		SDHC->IRQSIGEN &= SDHC_IRQSIGEN_CRMIEN_MASK;
		SDHC->IRQSIGEN |= SDHC_IRQSIGEN_CINSIEN_MASK;
		status.isCard = false;
	}
	if(SDHC->IRQSTAT & SDHC_IRQSTAT_CINS_MASK){
		SDHC->IRQSTAT |= SDHC_IRQSTAT_CINS_MASK;		//apago flag
		SDHC->IRQSIGEN &= SDHC_IRQSIGEN_CINSIEN_MASK;	//deshabilito futuras irq de insertion
		SDHC->IRQSIGEN |= SDHC_IRQSIGEN_CRMIEN_MASK;	//habilito futuras irq de remove
		status.isCard = true;
	}



}






__ISR__ SDHC_IRQHandler(){
	SDHC_irqHandler();
}
