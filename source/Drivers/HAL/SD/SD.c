/***************************************************************************//**
  @file     SDHC.h
  @brief    Driver MCAL para SDHC
  @author   Grupo 2
 ******************************************************************************/
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "SD.h"
#include "../../MCAL/SDHC/SDHC.h"
#include "../../MCAL/PIT/pit.h"
#include "../../MCAL/Gpio/gpio.h"
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define SD_ARG_PATTERN 				(0b10110010)

#define SD_ARG_CMDn_RCA_MASK		(0xFFFF0000U)
#define SD_ARG_CMDn_RCA_SHIFT		(0x10U)
#define SD_ARG_CMDn_RCA(x)			(((uint32_t)(x)<<SD_ARG_CMDn_RCA_SHIFT) & SD_ARG_CMDn_RCA_MASK)

#define SD_ARG_CMD8_PTT_MASK		(0xFFU)
#define SD_ARG_CMD8_PTT_SHIFT		(0x0U)
#define SD_ARG_CMD8_PTT(x)			(((uint32_t)(x)<<SD_ARG_CMD8_PTT_SHIFT) & SD_ARG_CMD8_PTT_MASK)

#define SD_ARG_CMD8_VHS_MASK		(0xF00U)
#define SD_ARG_CMD8_VHS_SHIFT		(0x08U)
#define SD_ARG_CMD8_VHS(x)			(((uint32_t)(x)<<SD_ARG_CMD8_VHS_SHIFT) & SD_ARG_CMD8_VHS_MASK)

#define SD_ARG_ACMD6_BUS_W_MASK		(0x3U)
#define SD_ARG_ACMD6_BUS_W_SHIFT	(0x0U)
#define SD_ARG_ACMD6_BUS_W(x)		(((uint32_t)(x)<<SD_ARG_ACMD6_BUS_W_SHIFT) & SD_ARG_ACMD6_BUS_W_MASK)

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


#define SD_RESP_R1_C_STATE_MASK		(0x1E00U)
#define SD_RESP_R1_C_STATE_SHIFT	(0x9U)
#define SD_RESP_R1_C_STATE(x)		(((uint32_t)(x)<<SD_RESP_R1_C_STATE_SHIFT) & SD_RESP_R1_C_STATE_MASK)

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

#define SD_RESP_R6_RCA_MASK			(0xFFFF0000U)
#define SD_RESP_R6_RCA_SHIFT		(0x10U)
#define SD_RESP_R6_RCA(x)			(((uint32_t)(x)<<SD_RESP_R6_RCA_SHIFT) & SD_RESP_R6_RCA_MASK)

#define SD_RESP_R7_PTT_MASK			(0xFFU)
#define SD_RESP_R7_PTT_SHIFT		(0x0U)
#define SD_RESP_R7_PTT(x)			(((uint32_t)(x)<<SD_RESP_R7_PTT_SHIFT) & SD_RESP_R7_PTT_MASK)
#define SD_RESP_R7_VACC_MASK		(0xF00U)
#define SD_RESP_R7_VACC_SHIFT		(0x08U)
#define SD_RESP_R7_VACC(x)			(((uint32_t)(x)<<SD_RESP_R7_VACC_SHIFT) & SD_RESP_R7_VACC_MASK)

#define SD_RESP_R2_bits120to96_MASK	(0x00FFFFFFU)

#define SD_RESP_R1_GET_C_STATE(x)	(((uint32_t)(x)&SD_RESP_R1_C_STATE_MASK) >> SD_RESP_R1_C_STATE_SHIFT)

#define SD_OCR_VW_MASK				(0xFFFFFFU)
#define SD_OCR_VW_SHIFT				(0x0U)
#define SD_OCR_VW(x)				(((uint32_t)(x)<<SD_OCR_VW_SHIFT) & SD_OCR_VW_MASK)

#define SD_SCR_BUS_WIDTH_MASK		(0xF0000U)
#define SD_SCR_BUS_WIDTH_SHIFT		(0x10U)
#define SD_SCR_BUS_WIDTH(x)			(((uint32_t)(x)<<SD_SCR_BUS_WIDTH_SHIFT) & SD_SCR_BUS_WIDTH_MASK)

#define SD_CSD_PARTIAL_BLK_MASK		(0xF0000U)
#define SD_CSD_PARTIAL_BLK_SHIFT	(0x10U)
#define SD_CSD_PARTIAL_BLK(x)		(((uint32_t)(x)<<SD_CSD_PARTIAL_BLK_SHIFT) & SD_CSD_PARTIAL_BLK_MASK)

//PIT
#define SD_WAIT_TIME_S				(0.5)
#define SD_PIT_TIME_US				(1000000*SD_WAIT_TIME_S)

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

enum busWidthArgACMD6 {
	BUS_W_1b = 0b00,
	BUS_W_4b = 0b10,
};

enum busWidthSCR{
	BUS_W_1bit = 0b0001,
	BUS_W_4bit = 0b0100,
};

typedef enum {
	idle,
	ready,
	ident,
	stby,
	tran,
	data,
	rcv,
	prg,
	dis,
	res_for_IO = 15,
	flagErr = -1
}sd_stateR1;

typedef struct{
	SD_cardStatus cardStatus;
	bool isSDHC;
	bool isUHS2;
	uint32_t cid[4];
	uint32_t rca;
	uint32_t csd[4];
	uint32_t scr;
	uint8_t busWidth;
	uint32_t blockSize;
}sd_cardInfo;
/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
static bool SD_resetToIDLE();
static bool SD_operationConditionValidation();
static bool SD_configDataTransferMode();
static sd_stateR1 SD_getCurrentState();
static bool SD_sendAplicationCMD(uint32_t rca, SDHC_cmd_t * cmdSend);
static void SD_timerPIT(void);



/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/



/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
static sd_cardInfo cardInfo;

static bool isTimePassed;


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void SD_init(){
	SDHC_init();
	//SDHC_enableCardDedection();
	pitInit();

	cardInfo.cardStatus = SD_NOTINIT;
}
bool SD_isSDcard(){
	return SDHC_isCardDetected();
}

SD_cardStatus SD_getStatus(){
	return cardInfo.cardStatus;
}

bool SD_initializationProcess(){
	bool success = false;
	cardInfo.cardStatus = SD_INIT;
	//inicializacion de SDHC
	SDHC_boot(LITTLE_E);
	while(SDHC_getCMDstatus());

	//freq menor a 400KHz
	SDHC_setClockFrecuency(PRESCALEx16,15);

	//resetear para iniciar proceso desde IDLE
	bool no_err = SD_resetToIDLE();

	if(no_err){
		//configuracion de sd
		no_err = SD_operationConditionValidation();

		if(no_err){
			//sd en modo transfer data
			success = SD_configDataTransferMode();
			cardInfo.cardStatus = success ? SD_RDY : SD_NOTRDY;
		}
		else{
			cardInfo.cardStatus = SD_ERROR;
		}
	}
	else{
		cardInfo.cardStatus = SD_ERROR;
	}

	return success;
}

bool SD_readSectors(uint32_t address, uint32_t numSector, uint32_t * data){

	if((cardInfo.cardStatus != SD_RDY) && (cardInfo.cardStatus != SD_OK)){
		cardInfo.cardStatus = SD_NOTRDY;
		return false;
	}

	//Guardo datos de tarjeta SD
	sd_cardInfo cardInfo_copy;
	cardInfo_copy.blockSize = cardInfo.blockSize;
	cardInfo_copy.isSDHC = cardInfo.isSDHC;
	cardInfo_copy.rca = cardInfo.rca;
	cardInfo_copy.cardStatus = cardInfo.cardStatus;

	bool no_err = false;
	SDHC_cmd_t cmd;
	uint32_t rwml;

	SDHC_setReadWMLevelAndBurstLength(128,0);
	rwml = SDHC_getRWML();

	if(numSector == 1){
		cmd.cmd_index = 17;
		cmd.cmd_transfer_type = SINGLE_T;
			}
	else{
		cmd.cmd_index = 18;
		cmd.cmd_transfer_type = MULTIPLE_T;
	}
	cmd.cmd_arg = address;
	cmd.cmd_type = NORMAL_CMD;
	cmd.cmd_resp_type = R1;
	cmd.block_count = numSector;
	cmd.card_to_sdhc = true;
	no_err = SDHC_sendCMD(&cmd);

	if(no_err){
		no_err = false;
		uint32_t j = 0;
		while(! SDHC_isTransferReady()){
			if(SDHC_isBufferReadReady()){
				uint32_t i;
				for(i = 0; i < rwml; i++){
					data[j++] = SDHC_getData();
				}
			}
		}

		cardInfo.blockSize = cardInfo_copy.blockSize;
		cardInfo.isSDHC = cardInfo_copy.isSDHC;
		cardInfo.rca = cardInfo_copy.rca;
		cardInfo.cardStatus = cardInfo_copy.cardStatus;

		if( (j*SDHC_WORD_SIZE_BYTES) != (numSector*cardInfo.blockSize) ){
			//no se leyeron todos los bytes esperados
			if(SDHC_isBufferReadReady()){
				//leo ultimos datos
				uint32_t i;
				for(i = 0; i < rwml; i++){
					data[j++] = SDHC_getData();
				}
			}
			cardInfo.cardStatus = SD_ERROR;
		}
		else{
			no_err = true;
			cardInfo.cardStatus = SD_OK;
		}
	}
	return no_err;
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
static bool SD_resetToIDLE(){

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

	bool pattern_ok;
	bool vAccepted;

	//CMD8 (SEND_IF_COND) verificar Vdd de SDcard
	cmd.cmd_index = 8;
	cmd.cmd_arg = SD_ARG_CMD8_PTT(SD_ARG_PATTERN) | SD_ARG_CMD8_VHS(V2_7_TOV3_6);
	cmd.cmd_type = NORMAL_CMD;
	cmd.cmd_resp_type = R7;
	cmd.cmd_transfer_type = NO_DATA_T;
	no_err = SDHC_sendCMD(&cmd);
	if(no_err){
		pattern_ok = ((cmd.response[0] & SD_RESP_R7_PTT_MASK) == SD_RESP_R7_PTT(SD_ARG_PATTERN))? true : false;
		vAccepted = ((cmd.response[0] & SD_RESP_R7_VACC_MASK) == SD_RESP_R7_VACC(V2_7_TOV3_6))? true : false;
		if(pattern_ok && vAccepted){
			cmd8Response = true;
		}
		else{
			cmd8Response = false;
		}
	}

	//CMD55 (APP_CMD) y despues ACMD41 (SD_SEND_OP_COND)
	if(cmd8Response){
		bool initializationComplete = false;
		uint8_t tries = 10;
		isTimePassed = false;
		pitSetIRQFunc(PIT_0, SD_timerPIT);

		//primer ACMD41 para iniciar proceso de inicializacion (ocr != 0)
		cmd.cmd_index = 41;
		uint32_t ocr = SD_OCR_VW(V3_2TOV3_3) | SD_OCR_VW(V3_3TOV3_4);
		cmd.cmd_arg = SD_ARG_ACMD41_OCRS(ocr>>8) | SD_ARG_ACMD41_XPS_MASK | SD_ARG_ACMD41_HCS_MASK;
		cmd.cmd_type = NORMAL_CMD;
		cmd.cmd_resp_type = R3;
		cmd.cmd_transfer_type = NO_DATA_T;
		do{
			no_err = SD_sendAplicationCMD(0,&cmd);
			if(no_err){
				if(cmd.response[0] & SD_RESP_R3_BUSY_MASK){
					//Inicializacion completada
					cardInfo.isSDHC = (cmd.response[0] & SD_RESP_R3_CCS_MASK) ? true : false;
					cardInfo.isUHS2 = (cmd.response[0] & SD_RESP_R3_UHS2_MASK) ? true : false;
					initializationComplete = true;
				}
				else{
					//esperar 1 segundo antes de repetir
					pitSetAndBegin(PIT_0,SD_PIT_TIME_US);
					while(!isTimePassed);
					pitStopTimer(PIT_0);
					isTimePassed = false;
					//reenvio de ACMD41 con argunento 0
					cmd.cmd_arg = 0;
				}
			}
			else{
				tries = -1;
			}
		}while((!initializationComplete) && (--tries > 0));
		pitDisableIRQFunc(PIT_0);

		if(tries <= 0){
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
				//se lee CID
				for(i = 0; i < cmd.cmd_lenght_resp; i++ ){
					cardInfo.cid[i] = cmd.response[i];
				}
				cardInfo.cid[3] &= SD_RESP_R2_bits120to96_MASK;

				//asignar RCA con CMD3
				cmd.cmd_index = 3;
				cmd.cmd_arg = 0;
				cmd.cmd_type = NORMAL_CMD;
				cmd.cmd_resp_type = R6;
				cmd.cmd_transfer_type = NO_DATA_T;
				no_err = SDHC_sendCMD(&cmd);
				if(no_err){
					//terminada la inicializacion se guarda el RCA
					validation = true;
					cardInfo.rca = (cmd.response[0]&SD_RESP_R6_RCA_MASK) >> SD_RESP_R6_RCA_SHIFT;
				}
			}
		}
	}
	return validation;
}

static bool SD_configDataTransferMode(){

	//read current state
	sd_stateR1 state = SD_getCurrentState();//stby

	//Read CSD register	CMD9
	SDHC_cmd_t cmd;
	bool no_err;
	bool trasnferModeReady = false;

	cmd.cmd_index = 9;
	cmd.cmd_arg = SD_ARG_CMDn_RCA(cardInfo.rca);
	cmd.cmd_type = NORMAL_CMD;
	cmd.cmd_resp_type = R2;
	cmd.cmd_transfer_type = NO_DATA_T;
	no_err = SDHC_sendCMD(&cmd);
	if(no_err){
		uint8_t i = 0;
		for(i = 0; i < cmd.cmd_lenght_resp; i++ ){
			cardInfo.csd[i] = cmd.response[i];
		}
		cardInfo.csd[3] &= SD_RESP_R2_bits120to96_MASK;
	}
	else{
		trasnferModeReady = false;
	}


	state = SD_getCurrentState(); //stby
	//set freq 24MHz
	SDHC_setClockFrecuency(PRESCALEx2,1);
	SDHC_setDTOCounter(20);

	if(no_err){
		//envio CMD7 para, en transfer state, leer SCR
		cmd.cmd_index = 7;
		cmd.cmd_arg = SD_ARG_CMDn_RCA(cardInfo.rca);
		cmd.cmd_type = NORMAL_CMD;
		cmd.cmd_resp_type = R1b;
		cmd.cmd_transfer_type = NO_DATA_T;
		no_err = SDHC_sendCMD(&cmd);
		if(no_err){

			state = SD_getCurrentState();//tran

/*
			//en transfer mode
			//read SCR register ACMD51
			//previo al ACMD, se manda CMD55
			cmd.cmd_index = 55;
			cmd.cmd_arg = SD_ARG_CMDn_RCA(cardInfo.rca);
			cmd.cmd_type = NORMAL_CMD;
			cmd.cmd_resp_type = R1;
			cmd.cmd_transfer_type = NO_DATA_T;
			no_err = SDHC_sendCMD(&cmd);
			if(no_err){
				//listo para mandar ACMD51 y recibir SCR
				cmd.cmd_index = 51;
				cmd.cmd_arg = 0;
				cmd.cmd_type = NORMAL_CMD;
				cmd.cmd_resp_type = R1;
				cmd.cmd_transfer_type = NO_DATA_T;
				no_err = SDHC_sendCMD(&cmd);
				if(no_err){
					cardInfo.scr = cmd.response[0];
				}
			}
			state = SD_getCurrentState();//data
		*/
		}
	}

	//con scr obtenemos bus width
	cardInfo.busWidth = (cardInfo.scr & SD_SCR_BUS_WIDTH_MASK) >> SD_SCR_BUS_WIDTH_SHIFT;
	if( /*((cardInfo.busWidth & BUS_W_4bit) == BUS_W_4bit) && */no_err){
		//set 4bit bus
		cmd.cmd_index = 6;
		cmd.cmd_arg = SD_ARG_ACMD6_BUS_W(BUS_W_4b);
		cmd.cmd_type = NORMAL_CMD;
		cmd.cmd_resp_type = R1;
		cmd.cmd_transfer_type = NO_DATA_T;
		no_err = SD_sendAplicationCMD(cardInfo.rca, &cmd);
		if(no_err){
			//SDHC de 4 bits
			SDHC_setBusWidth(BUS_4bit);
		}

		state = SD_getCurrentState();//tran
	}

	//con csd obtenemos si partial block read operation esta disponible
	//sino esta disponible, no se puede setear block size
	//ademas, SDHC SDXH y SDUC tienen fijo el block size en 512 por default
	if(cardInfo.isSDHC && no_err){
		trasnferModeReady = true;
		cardInfo.blockSize = 512;
		SDHC_setBlockSize(512);
		cmd.cmd_index = 16;
		cmd.cmd_arg = 512;
		cmd.cmd_type = NORMAL_CMD;
		cmd.cmd_resp_type = R1;
		cmd.cmd_transfer_type = NO_DATA_T;
		no_err = SDHC_sendCMD(&cmd);
		if(no_err){
			SDHC_setBlockSize(512);
		}
	}

	return trasnferModeReady;
}

static sd_stateR1 SD_getCurrentState(){
	//verificar status
	SDHC_cmd_t cmd;
	cmd.cmd_index = 13;
	cmd.cmd_arg = SD_ARG_CMDn_RCA(cardInfo.rca);
	cmd.cmd_type = NORMAL_CMD;
	cmd.cmd_resp_type = R1;
	cmd.cmd_transfer_type = NO_DATA_T;
	if(SDHC_sendCMD(&cmd)){
		uint32_t currentStatus = cmd.response[0];
		return SD_RESP_R1_GET_C_STATE(currentStatus);
	}
	return flagErr;
}

static bool SD_sendAplicationCMD(uint32_t rca, SDHC_cmd_t * cmdSend){
	SDHC_cmd_t cmd;
	bool no_err = false;
	cmd.cmd_index = 55;
	cmd.cmd_arg = SD_ARG_CMDn_RCA(cardInfo.rca);
	cmd.cmd_type = NORMAL_CMD;
	cmd.cmd_resp_type = R1;
	cmd.cmd_transfer_type = NO_DATA_T;
	no_err = SDHC_sendCMD(&cmd);
	if(no_err){
		cmd.cmd_index = cmdSend->cmd_index;
		cmd.cmd_arg = cmdSend->cmd_arg;
		cmd.cmd_type = cmdSend->cmd_type;
		cmd.cmd_resp_type = cmdSend->cmd_resp_type;
		cmd.cmd_transfer_type = cmdSend->cmd_transfer_type;
		no_err = SDHC_sendCMD(&cmd);
		if(no_err){
			no_err = true;
			uint8_t i = 0;
			for(i = 0; i < cmd.cmd_lenght_resp; i++ ){
				cmdSend->response[i] = cmd.response[i];
			}
		}
	}
	return no_err;
}

static void SD_timerPIT(void){
	isTimePassed = true;
}
