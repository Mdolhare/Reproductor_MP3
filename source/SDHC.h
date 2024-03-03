/***************************************************************************//**
  @file     SDHC.h
  @brief    Driver MCAL para SDHC
  @author   Grupo 2
 ******************************************************************************/
#ifndef SDHC_H_
#define SDHC_H_


/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdbool.h>
#include <stdint.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef enum{
	BUS_1bit 	= 0b00,
	BUS_4bit 	= 0b01,
	BUS_8bit 	= 0b10,
	RES 		= 0b11,
}sdhc_busWidth;

typedef enum{
	PRESCALEx1,
	PRESCALEx2 = 0x01,
	PRESCALEx4 = 0x02,
	PRESCALEx8 = 0x04,
	PRESCALEx16 = 0x08,
	PRESCALEx32 = 0x10,
	PRESCALEx64 = 0x20,
	PRESCALEx128 = 0x40,
	PRESCALEx256 = 0x80
}sdhc_prescaler;

typedef enum err_type{
	NO_ERR 				= 0b00000000,
	ERR_CMD_NO_SEND 	= 0b00000001,
	ERR_CMD_TIME_OUT 	= 0b00000010,
	ERR_CMD_CRC			= 0b00000100,
	ERR_CMD_DAT_TIME_OUT= 0b00001000,
	ERR_CMD_DAT_END_BIT	= 0b00010000,
	ERR_CMD_DAT_AC12	= 0b00100000,
}SDHC_errType;

typedef enum{
	NORMAL_CMD,
	SUSPEND_CMD,
	RESUME_CMD,
	ABORT_CMD
}SDHC_cmdType;

typedef enum{
	NO_R,R1,R1b,R2,R3,R4,R5,R5b,R6,R7
}SDHC_cmdRespType;

typedef enum {
	NO_DATA_T,
	SINGLE_T,
	INFINITE_T,
	MULTIPLE_T
}SDHC_cmdTransferType;

typedef enum {
	CMD_LENGTH_0,
	CMD_LENGTH_48,
	CMD_LENGTH_136 = 4
}SDHC_cmdLengthResp;

typedef struct{
	//data to send
	uint8_t cmd_index;
	uint32_t cmd_arg;						//bits [39:8] de trama CMD
	SDHC_cmdType cmd_type;
	SDHC_cmdRespType cmd_resp_type;
	SDHC_cmdTransferType cmd_transfer_type;
	uint32_t block_count;
	bool card_to_sdhc;						//data from card to sdhc => true, data from sdhc to card => false

	//data to receive
	SDHC_cmdLengthResp cmd_lenght_resp;
	uint32_t response[4];					//bits [39/127:8], excepto R6 => bits [39:9] de la trama de CMD_RESP
}SDHC_cmd_t;


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
void SDHC_init();

void SDHC_boot(sdhc_busWidth width, uint16_t blockSize);

void SDHC_enableCardDedection();

bool SDHC_isCardDetected();

void SDHC_setClockFrecuency(sdhc_prescaler presc, uint8_t div);

bool SDHC_sendCMD(SDHC_cmd_t * cmd);

SDHC_errType SDHC_getErrStatus();

bool SDHC_getCMDstatus();


#endif /* SDHC_H_ */
