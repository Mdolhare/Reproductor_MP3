/*******************************************************************************
  @file     +DemoduladorV2.c+
  @brief    +Demodulador con FTM y CMP+
  @author   +GRUPO 2+
 ******************************************************************************/
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "Decodificador.h"
#include "InputCapture.h"
#include "pit.h"
#include "gpio.h"
#include "PWM.h"
#include "hardware.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define CANT_PERIODOS ((CANT_BITS)*2)
#define MARGEN_PERIODO 20
#define MARGEN_DIF 80
#define NO_DATA 0xFFFF
#define CANT_BUFF 2

#define VAL_IN_MARG(x,y,marg) (((x)<=((y)+(marg))) && ((x)>=((y)-(marg))))
/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
static void readPeriodo();
static void readPeriodo2();

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
static uint16_t valPeriodos[CANT_BUFF][CANT_PERIODOS];
static IC_pinID_t pinID_IC;
static bool frameReady;
static uint8_t bufferReady;
static uint16_t contFrame;



/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void Dec_Init(InputPin_t pin){

	//inicio variables
	for(int i = 0; i < CANT_PERIODOS; i++){
		valPeriodos[0][i] = 0;
		valPeriodos[1][i] = 0;
	}
	bufferReady = 0;
	//inicio modulo IC
	IC_Pin_t pin_IC;
	switch(pin){
		case DEC_PTD0:
			pin_IC = PTD0_IC_0;
			break;
		case DEC_PTD1:
			pin_IC = PTD1_IC_1;
			break;
		case DEC_CMP0:
			pin_IC = CMP0_IC_2;
			break;
		case DEC_CMP1:
			pin_IC = CMP1_IC_3;
			break;
		default:
			pin_IC = PTD0_IC_0;
			break;
	}
	IC_init(pin_IC, IC_bothEdge, &pinID_IC);

	//inicio modulo PIT
	pitInit();
	pitSetAndBegin(PIT_2, 833/2);
	pitSetIRQFunc(PIT_2, readPeriodo);

}

bool Dec_isNewFrame(){
	return frameReady;
}

uint16_t Dec_getFrame(){
	uint16_t frame = 0;
	frameReady = false;
//	uint16_t lastPeriodo = (valPeriodos[bufferReady][1] != NO_DATA) ?
//			valPeriodos[bufferReady][1] : valPeriodos[bufferReady][0];
	uint16_t lastPeriodo =  valPeriodos[bufferReady][1];

	uint16_t prom[11];

	for(int i = 0,k = 0; i < CANT_PERIODOS; i+=2,k++){
		prom[k] = valPeriodos[bufferReady][i]+valPeriodos[bufferReady][i+1];
		prom[k] /= 2;
	}

	uint16_t th = 630;
	for(int i = 0; i < CANT_PERIODOS/2; i++){
		if(i>=1){
			if( (prom[i-1] < th) && (prom[i] > th)){
				frame |= (1<<i);
			}
			else if((prom[i-1] > th) && (prom[i] > th)){
				frame |= (1<<i);

			}
		}
		else{
			if(prom[i] > th){
				frame |= (1<<i);
			}
		}
	}

	/*
	for(int i = 2, k = 0; i < CANT_PERIODOS-3; i+=2,k++){
	//	uint8_t j = (valPeriodos[bufferReady][i] != NO_DATA) ? i : i+1;
		uint8_t j = i;

		if(lastPeriodo < valPeriodos[bufferReady][j]-MARGEN_PERIODO){
			//incremento de periodo

			//uint8_t m = (valPeriodos[bufferReady][i+2] != NO_DATA) ? i+2 : i+3;
			uint8_t m = i+2;
			if(valPeriodos[bufferReady][j] > valPeriodos[bufferReady][m]+10){
				//me fijo el periodo que le sigue un decremento
				frame |= (1<<k);
			}
			else if(!VAL_IN_MARG(valPeriodos[bufferReady][j],T_CERO, MARGEN_PERIODO)){
				frame |= (1<<k);
			}

		}
		else if(VAL_IN_MARG(valPeriodos[bufferReady][j],T_UNO, MARGEN_PERIODO)){
			//si no es mayor, verifico si es un 1 (833)
			frame |= (1<<k);
		}

		if(valPeriodos[bufferReady][i] != NO_DATA){
			lastPeriodo = valPeriodos[bufferReady][i];
		}
		else if(valPeriodos[bufferReady][i+1] != NO_DATA){
			lastPeriodo = valPeriodos[bufferReady][i+1];
		}
	}
*/


	if(contFrame==3){
		int a = 0;
	}
	contFrame++;
	//return (frame<<1)|0x400; //stop paridada 8bits_dato start
	return frame;
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
static void readPeriodo(){

	static uint8_t numPeriodo = 0;
	static uint8_t isBitStart = false;
	static uint8_t buffToWrite = 0;
	static uint16_t lastPeriodo = 0;
	static uint8_t contStart = 0;


	gpioToggle(PORTNUM2PIN(PB,9));

	if(IC_isNewPeriod(&pinID_IC)){
		uint16_t periodo = 2 * IC_getPeriod(&pinID_IC);
		lastPeriodo = periodo;
		if(isBitStart){
			valPeriodos[buffToWrite][numPeriodo] = periodo;
			numPeriodo++;
		}
		else if(VAL_IN_MARG(periodo, T_CERO, MARGEN_PERIODO)){
			//entra cuando encuentra start
			isBitStart = true;
			valPeriodos[buffToWrite][numPeriodo] = periodo;
			numPeriodo++;
			if(contFrame==6){
			//	gpioWrite(PORTNUM2PIN(PE,26),1);
			}
		}

	}
	else if(isBitStart){
		valPeriodos[buffToWrite][numPeriodo] = lastPeriodo;
		numPeriodo++;
	}

	if(numPeriodo == CANT_PERIODOS){
		//fin de frame
		isBitStart = false;
		numPeriodo = 0;
		frameReady = true;
		bufferReady = buffToWrite;
		buffToWrite = (buffToWrite + 1) & 1; //cuenta circular 0/1
	}

}

static void readPeriodo2(){
	static uint8_t numPeriodo = 0;
		gpioToggle(PORTNUM2PIN(PE,26));

	if(numPeriodo<CANT_PERIODOS){
		uint16_t periodo = 2 * IC_getPeriod(&pinID_IC);

	}
	else{

	}
	if(numPeriodo == CANT_PERIODOS){

	}
}


