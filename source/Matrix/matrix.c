/*******************************************************************************
  @file     matrix.c
  @brief    Driver to use WS2812
  @author   Grupo 2 2023
 ******************************************************************************/
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "matrix.h"
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define DUTY_ONE  40
#define DUTY_ZERO 20
#define COLOR_BITS 8
#define PIXELS 64
#define ALL_BITS COLOR_BITS*PIXELS*3
#define BIT_MENOS_SIGNIFICATIVO 0b00000001
/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef struct LED{
	uint16_t green[COLOR_BITS];
	uint16_t red[COLOR_BITS];
	uint16_t blue[COLOR_BITS];
}LED;
/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
static void reset_start();
static void reset_stop();

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
static TCD_t tcd;
static PWM_pinID_t pwmConfig;
static LED mat[PIXELS];
/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void matrixInit(PWM_pin_t pin)
{
	pitInit();
	PWM_init(pin, &pwmConfig);  //choose PWM_PTC1
	uint32_t* cnv_ptr;
	cnv_ptr = PWM_getAdressCnVDMA(pin);
	tcd_cfg_t dma_cfg = {(uint32_t)mat, (uint32_t)cnv_ptr, 2, 0,
					dma16BIT, dma16BIT, 2, (sizeof(mat)/sizeof(mat[0].green[0])), sizeof(mat), 0, 0, 0};
	dma_config(dma_cfg, &tcd);
	dma_begin(dmaCHANNEL0,  dmaFTM0_0, false, &tcd);
	dma_add_irq(dmaCHANNEL0, reset_start, &tcd, false, true);
	pitSetIRQFunc(PIT_0, reset_stop);
}

void turnOffMatrix()
{

	for (int i = 0; i<PIXELS; i++)
	{
		updateLED(i, 0,0,0);
	}
}
void updateLED(int i, uint8_t color_g,  uint8_t color_r,  uint8_t color_b)
{
	for(int j = 0; j<COLOR_BITS; j++)
	{
		mat[i].green[j] = (((color_g)>>j)&BIT_MENOS_SIGNIFICATIVO)>0 ? DUTY_ONE : DUTY_ZERO;
		mat[i].red[j] = (((color_r)>>j)&BIT_MENOS_SIGNIFICATIVO)>0 ? DUTY_ONE : DUTY_ZERO;
		mat[i].blue[j] = (((color_b)>>j)&BIT_MENOS_SIGNIFICATIVO)>0 ? DUTY_ONE : DUTY_ZERO;
	}

}



/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/



static void reset_start() {

		while(!FTM_IsInterruptPending(FTM0, 0))
		{

		}

		FTM0->OUTMASK |= FTM_OUTMASK_CH0OM(1);

		FTM_StopClock(FTM0);

		pitSetAndBegin(PIT_0, 55);
}



static void  reset_stop(){

		pitDisable(PIT_0);

		FTM0->OUTMASK &= ~FTM_OUTMASK_CH0OM(1);

		FTM_StartClock(FTM0);

}
