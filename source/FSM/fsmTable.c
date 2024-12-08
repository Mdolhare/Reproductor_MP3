/*
 * fsmTable.c
 *
 *  Created on: Mar 5, 2024
 *      Author: Grupo 2
 */

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "fsmTable.h"
#include "../Drivers/HAL/SD/SD.h"
#include "../Drivers/HAL/display/displayLCD.h"
#include "../PlayMusic/playMusic.h"
#include "../Drivers/MCAL/Gpio/gpio.h"
#include "subMenuItems.h"
#include <stdint.h>



/*******************************************************************************
 * Foward declarations
 ******************************************************************************/
extern state_edge_t estado_init[];
extern state_edge_t insertar_sd[];

extern state_edge_t sleep[];

extern state_edge_t menu[];

extern state_edge_t volumen[];
extern state_edge_t ecualizador[];
extern state_edge_t canciones[];
extern state_edge_t pausa_play[];

extern state_edge_t ec_personalizado[];


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define LED_R PORTNUM2PIN(PB,22)
#define LED_B PORTNUM2PIN(PB,21)
#define LED_G PORTNUM2PIN(PE,26)

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
enum eventos{
	PAUSE_RESUME=10,
	VOLUMEN,
	ECUALIZADOR,
	CANCIONES,

	EC_PERZONALIZADO,
	EC_NORMAL,
	EC_ROCK,
	EC_POP,
	EC_JAZZ,
	BACK
};

enum vol_bars{
	N1,
	N2,
	N3,
	N4,
	N5
};

//volumen
#define MAX_VOL 30


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
//---------------------------Rutinas globales
static void init_libs(void);
static void do_nothing(void);
static void do_nothing_1(void);
static void do_nothing_2(void);
static void do_nothing_3(void);
static void do_nothing_SIN_SD(void);

static void move_up_row(void);
static void move_down_row(void);

//---------------------------Rutinas estado init
static void estado_init_config_SD(void);

//---------------------------Rutinas sleep
static void go_sleep(void);

//---------------------------Rutinas menu
static void show_menu();

//---------------------------Rutinas volumen
static void showVolumen(void);
static void incrementVolumen(void);
static void decrementVolumen(void);
static void selectVolumen(void);
static void dibujarBarra(int nivel);

//---------------------------Rutinas ecualizador
static void showEcualizador(void);
static void normalBands();
static void rockBands();
static void popBands();
static void jazzBands();


//---------------------------Items selector
static void nextItem(void);
static void prevItem(void);
static void selectItem(void);

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/
state_edge_t estado_init[] = {
	//eventos de hareware
	{MOV_ABAJO,		insertar_sd, 	init_libs},
	{MOV_ARRIBA,	insertar_sd, 	init_libs},
	{SELECT, 		insertar_sd, 	init_libs},
	{HAY_SD, 		insertar_sd, 	init_libs},
	{NO_HAY_SD,		insertar_sd,	init_libs},

	{TIME_OUT_SD, 	insertar_sd, 	init_libs},	//revisar
	{NONE, 			insertar_sd, 	init_libs}
};

state_edge_t insertar_sd[] = {
	//eventos de hareware
	{MOV_ABAJO,		insertar_sd, 	do_nothing},
	{MOV_ARRIBA,	insertar_sd, 	do_nothing},
	{SELECT, 		insertar_sd, 	do_nothing},
	{HAY_SD, 		menu, 			estado_init_config_SD},
	{NO_HAY_SD,		insertar_sd,	do_nothing_SIN_SD},
	
	{TIME_OUT_SD, sleep, go_sleep},	//revisar
	{NONE, insertar_sd, do_nothing}
};


state_edge_t sleep[] = {
	{NONE, estado_init, do_nothing},
	{NONE, estado_init, do_nothing}
};

state_edge_t menu[] = {
	//eventos de hareware
	{MOV_ABAJO,		menu, 			prevItem},
	{MOV_ARRIBA,	menu, 			nextItem},
	{SELECT, 		menu, 			selectItem},
	{HAY_SD, 		menu, 			do_nothing},
	{NO_HAY_SD,		insertar_sd,	do_nothing_SIN_SD},

	//eventos de transicion
	{PAUSE_RESUME, 	menu, 			playMusicPause},
	{VOLUMEN, 		volumen, 		showVolumen},
	{ECUALIZADOR, 	ecualizador, 	showEcualizador},
	{CANCIONES, 	canciones, 		do_nothing},

	{NONE, menu, do_nothing}
};

state_edge_t pausa_play[] = {
	{NONE, menu, do_nothing},
	{NONE, estado_init, do_nothing}
};

state_edge_t volumen[] = {
	//eventos de hareware
	{MOV_ABAJO,		volumen, 	decrementVolumen},
	{MOV_ARRIBA,	volumen, 	incrementVolumen},
	{SELECT, 		menu, 		selectVolumen},
	{HAY_SD, 		volumen, 	do_nothing},
	{NO_HAY_SD,		insertar_sd,do_nothing_SIN_SD},
	

	{NONE, volumen, do_nothing}

};

state_edge_t ecualizador[] = {
	//eventos de hareware
	{MOV_ABAJO,		ecualizador, 	prevItem},
	{MOV_ARRIBA,	ecualizador, 	nextItem},
	{SELECT, 		ecualizador, 	selectItem},
	{HAY_SD, 		ecualizador, 	do_nothing},
	{NO_HAY_SD,		insertar_sd,	do_nothing_SIN_SD},

	//eventos de transicion
	{EC_PERZONALIZADO, 	ec_personalizado, 	do_nothing_1},
	{EC_NORMAL, 		menu, 				normalBands},
	{EC_ROCK, 			menu, 				rockBands},
	{EC_POP, 			menu, 				popBands},
	{EC_JAZZ, 			menu, 				jazzBands},
	{BACK, 				menu, 				show_menu},

	{NONE, ecualizador, do_nothing}
};

state_edge_t canciones[] = {
	//eventos de hareware
	{MOV_ABAJO,		canciones, 	prevItem},
	{MOV_ARRIBA,	canciones, 	nextItem},
	{SELECT, 		canciones, 	selectItem},
	{HAY_SD, 		canciones, 	do_nothing},
	{NO_HAY_SD,		insertar_sd,do_nothing_SIN_SD},

	{NONE, estado_init, do_nothing}
};

state_edge_t ec_personalizado[] = {
	//eventos de hareware
	{MOV_ABAJO,		ec_personalizado, 	prevItem},
	{MOV_ARRIBA,	ec_personalizado, 	nextItem},
	{SELECT, 		ecualizador, 			selectItem},
	{HAY_SD, 		ec_personalizado, 	do_nothing},
	{NO_HAY_SD,		insertar_sd,			do_nothing_SIN_SD},

	{NONE, ec_personalizado, do_nothing_3}
};


/*******************************************************************************
 * STATIC SubMenu items
 ******************************************************************************/
static submenu_items_t * currentStateItem;

static submenu_items_t main_menu_items = {{	{0, PAUSE_RESUME, PAUSE_RESUME_COL, PAUSE_RESUME_FIL},
									    	{1, VOLUMEN, VOLUMEN_COL, VOLUMEN_FIL},
											{2, ECUALIZADOR, ECUALIZADOR_COL, ECUALIZADOR_FIL},
											{3, CANCIONES, CANCIONES_COL, CANCIONES_FIL}
											},
									     	0, 4};

static submenu_items_t ecualizador_items = {{	{0, EC_PERZONALIZADO, EC_PERZONALIZADO_COL, EC_PERZONALIZADO_FIL},
									    		{1, EC_NORMAL, EC_NORMAL_COL, EC_NORMAL_FIL},
												{2, EC_ROCK, EC_ROCK_COL, EC_ROCK_FIL},
												{3, EC_POP, EC_POP_COL, EC_POP_FIL},
												{4, EC_JAZZ, EC_JAZZ_COL, EC_JAZZ_FIL},
												{5, BACK, BACK_COL, BACK_FIL}
											},
									     	0, 6};



/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
static uint8_t row_index;
static uint8_t subMenu = 0;
static uint8_t volumen_value = 0;


static uint8_t nivel1[8] = {0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000};
static uint8_t nivel2[8] = {0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000};
static uint8_t nivel3[8] = {0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100};
static uint8_t nivel4[8] = {0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110};
static uint8_t nivel5[8] = {0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111};

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

state_t FSM_GetInitState(void) {
 	return estado_init;
}


/*******************************************************************************
 *******************************************************************************
                        		Rutinas
 *******************************************************************************
 ******************************************************************************/
//Rutinas globales entre estados
static void do_nothing(void){}

static void do_nothing_SIN_SD(void){
	//reset driver SD
	SD_reset();
	//mostrar que se inserte SD

	//Parar DAC
	audio_pause();
}

static void do_nothing_1(void){
	gpioWrite(LED_R, LOW);
	gpioWrite(LED_B, HIGH);
	gpioWrite(LED_G, HIGH);

}
static void do_nothing_2(void){
	gpioWrite(LED_R, HIGH);
	gpioWrite(LED_B, LOW);
	gpioWrite(LED_G, HIGH);

}
static void do_nothing_3(void){
	gpioWrite(LED_R, HIGH);
	gpioWrite(LED_B, HIGH);
	gpioWrite(LED_G, LOW);

}

static void init_libs(){
	//display
	displayLCD_Begin();
	displayLCD_createChar(N1, nivel1);
	displayLCD_createChar(N2, nivel2);
	displayLCD_createChar(N3, nivel3);
	displayLCD_createChar(N4, nivel4);
	displayLCD_createChar(N5, nivel5);

}

//---------------------------------Rutinas insertarsd
static void estado_init_config_SD(){
	//Inicializar nueva SD

	SD_cardStatus SDstatus;

	if(SDstatus != SD_OK && SDstatus != SD_RDY){
		bool ok = SD_initializationProcess();
		if(!ok){
			SDstatus = SD_getStatus();
			//tratar error
			while(1);
		}
		playMusicInit();
	}

	//mostrar menu
	show_menu();
}

//-----------------------------Rutinas sleep
static void go_sleep(void){
	//mandar a sleep los modulos

}

//-----------------------------Rutinas menu
static void show_menu(){
	currentStateItem = &main_menu_items;
	displayLCD_ShowStringLine(DISP_PAUSE_RESUME, PAUSE_RESUME_FIL);
	displayLCD_ShowStringLine(DISP_CANCIONES, CANCIONES_FIL);
	displayLCD_ShowStringLine(DISP_ECUALIZADOR, ECUALIZADOR_FIL);
	displayLCD_ShowStringLine(DISP_VOLUMEN, VOLUMEN_FIL);
	displayLCD_ShowCursorAt(currentStateItem->item[currentStateItem->item_selec].cursor_pos_col,
				currentStateItem->item[currentStateItem->item_selec].cursor_pos_fil);
}

//-----------------------------Rutinas volumen
static void showVolumen(void){
	displayLCD_HideCursor();
	displayLCD_Clear();
	displayLCD_ShowStringAt(DISP_VOLUMEN, 0, 0);
	displayLCD_ShowCharAt('-', 0,1);
	displayLCD_ShowCharAt('+', 19,1);
	dibujarBarra(volumen_value);
}

static void dibujarBarra(int nivel) {
    int lineas = nivel*3; // por usar 18 caracteres de 5 leds (18*5=90), y 30 posiciones
	int llenos = lineas / 5;       // Caracteres completamente llenos
    int parcial = lineas % 5;      // Nivel parcial del siguiente carácter

    // Posicionar en la fila inicial
    for (int i = 0; i < 18; i++) { // Dibujar hasta 6 bloques (30 posiciones/5 niveles)
        if (i < llenos) {
            displayLCD_ShowCharAt(N5,i+1,1); // Carácter completamente lleno
        }
        else if (i == llenos && parcial > 0) {
        	displayLCD_ShowCharAt(parcial-1,i+1,1); // Carácter parcial
        }
        else {
        	displayLCD_ShowCharAt(' ',i+1,1); // Carácter parcial
        }
    }
}

static void incrementVolumen(void){
	volumen_value++;
	if(volumen_value > MAX_VOL){
		volumen_value = MAX_VOL;
	}
	dibujarBarra(volumen_value);
}

static void decrementVolumen(void){
	if(volumen_value > 0){
		volumen_value--;
	}
	dibujarBarra(volumen_value);

}

static void selectVolumen(void){
	//setear volumen
	//audio_setVolume(volumen_value);
	show_menu();
}

//-----------------------------Rutinas ecualizador
static void showEcualizador(void){
	currentStateItem = &ecualizador_items;
	displayLCD_Clear();
	displayLCD_ShowStringAt(DISP_EC_NORMAL, EC_NORMAL_COL, EC_NORMAL_FIL);
	displayLCD_ShowStringAt(DISP_EC_ROCK, EC_ROCK_COL, EC_ROCK_FIL);
	displayLCD_ShowStringAt(DISP_EC_POP, EC_POP_COL, EC_POP_FIL);
	displayLCD_ShowStringAt(DISP_EC_JAZZ, EC_JAZZ_COL, EC_JAZZ_FIL);
	displayLCD_ShowStringLine(DISP_BACK, BACK_FIL);
	displayLCD_ShowStringLine(DISP_EC_PERZONALIZADO, EC_PERZONALIZADO_FIL);
	displayLCD_ShowCursorAt(currentStateItem->item[currentStateItem->item_selec].cursor_pos_col, 
				currentStateItem->item[currentStateItem->item_selec].cursor_pos_fil);
}

static void normalBands(){
	//setear ecualizador
	//audio_setEcualizador(NORMAL);
	show_menu();
}

static void rockBands(){
	//setear ecualizador
	//audio_setEcualizador(ROCK);
	show_menu();
}

static void popBands(){
	//setear ecualizador
	//audio_setEcualizador(POP);
	show_menu();
}

static void jazzBands(){
	//setear ecualizador
	//audio_setEcualizador(JAZZ);
	show_menu();
}


//-----------------------------Items selector
static void nextItem(void){
	currentStateItem->item_selec++;
	if(currentStateItem->item_selec >= currentStateItem->item_cant){
		currentStateItem->item_selec = 0;
	}

	displayLCD_ShowCursorAt(currentStateItem->item[currentStateItem->item_selec].cursor_pos_col, 
				currentStateItem->item[currentStateItem->item_selec].cursor_pos_fil);
}

static void prevItem(void){
	if(currentStateItem->item_selec == 0){
		currentStateItem->item_selec = currentStateItem->item_cant - 1;
	}
	else{
		currentStateItem->item_selec--;
	}	
	displayLCD_ShowCursorAt(currentStateItem->item[currentStateItem->item_selec].cursor_pos_col, 
				currentStateItem->item[currentStateItem->item_selec].cursor_pos_fil);
}

static void selectItem(void){
	EG_addEvent((currentStateItem->item[currentStateItem->item_selec].item_event_select));
}

