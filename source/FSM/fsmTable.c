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
#include "../FileSystem/FileSys.h"
#include "../Equalizer/equalizer.h"
#include "../Drivers/HAL/Matrix/matrix.h"
#include "subMenuItems.h"
#include <stdint.h>
#include <string.h>



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
extern state_edge_t reproductor[];

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
	PLAYER=10,
	VOLUMEN,
	ECUALIZADOR,
	CANCIONES,

	NEW_SONG,
	NEXT_SONG,
	PREV_SONG,
	PAUSE_RESUME,
	EC_PERZONALIZADO,
	EC_CLASSIC,
	EC_ROCK,
	EC_POP,
	EC_JAZZ,
	EDIT_CUSTOM_BAND,
	APPLY,
	BACK
};

enum ec_bands{
	B1_PERZONALIZADO,
	B2_CLASSIC,
	B3_ROCK,
	B4_POP,
	B5_JAZZ
};

enum ec_bands_custom{
	B1,
	B2,
	B3,
	B4,
	B5
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
static void do_nothing(void);
static void do_nothing_1(void);
static void do_nothing_2(void);
static void do_nothing_3(void);
static void resetDiskAndWait(void);

static void move_up_row(void);
static void move_down_row(void);

//---------------------------Rutinas estado init
static void init_libs(void);

//---------------------------Rutinas de insterar_sd
static void estado_init_config_SD(void);
static void show_insertSD();

//---------------------------Rutinas sleep
static void go_sleep(void);

//---------------------------Rutinas menu
static void show_menu();

//---------------------------Rutinas reproductor
static void show_reproductor();
static void nextSong();
static void prevSong();
static void pauseResume();
static void show_song_info();

//---------------------------Rutinas canciones
static void show_canciones(void);
static void select_cancion(void);
static void next_canciones(void);
static void prev_canciones(void);

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
static void nextItemEc(void);
static void prevItemEc(void);
static void show_gain(uint8_t item_index);

//---------------------------Rutinas ec_personalizado
static void showEcPersonalizado(void);
static void nextItemEcPers(void);
static void prevItemEcPers(void);
static void selectItemEcPers(void);
static void setCustomBand(void);

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
	{FINISH_SONG,	insertar_sd,	init_libs},


	{TIME_OUT_SD, 	insertar_sd, 	init_libs},	//revisar
	{NONE, 			insertar_sd, 	init_libs}
};

state_edge_t insertar_sd[] = {
	//eventos de hareware
	{MOV_ABAJO,		insertar_sd, 	do_nothing},
	{MOV_ARRIBA,	insertar_sd, 	do_nothing},
	{SELECT, 		insertar_sd, 	do_nothing},
	{HAY_SD, 		menu, 			estado_init_config_SD},
	{NO_HAY_SD,		insertar_sd,	resetDiskAndWait},
	{FINISH_SONG,	insertar_sd,	do_nothing},

	
	{TIME_OUT_SD, sleep, go_sleep},	//revisar
	{NONE, menu, show_menu}
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
	{NO_HAY_SD,		insertar_sd,	resetDiskAndWait},
	{FINISH_SONG,	menu,			nextSong},


	//eventos de transicion
	{PLAYER, 		reproductor, 	show_reproductor},
	{VOLUMEN, 		volumen, 		showVolumen},
	{ECUALIZADOR, 	ecualizador, 	showEcualizador},
	{CANCIONES, 	canciones, 		show_canciones},

	{NONE, menu, do_nothing}
};

state_edge_t reproductor[] = {
	//eventos de hareware
	{MOV_ABAJO,		reproductor, 			prevItem},
	{MOV_ARRIBA,	reproductor, 			nextItem},
	{SELECT, 		reproductor, 			selectItem},
	{HAY_SD, 		reproductor, 			do_nothing},
	{NO_HAY_SD,		insertar_sd,	resetDiskAndWait},
	{FINISH_SONG,	reproductor,			nextSong},


	//eventos de transicion
	{NEXT_SONG, 	reproductor, 			nextSong},
	{PREV_SONG, 	reproductor, 			prevSong},
	{PAUSE_RESUME, 	reproductor, 			pauseResume},
	{BACK, 			menu, 					show_menu},


	{NONE, reproductor, do_nothing}
};

state_edge_t volumen[] = {
	//eventos de hareware
	{MOV_ABAJO,		volumen, 	decrementVolumen},
	{MOV_ARRIBA,	volumen, 	incrementVolumen},
	{SELECT, 		menu, 		selectVolumen},
	{HAY_SD, 		volumen, 	do_nothing},
	{NO_HAY_SD,		insertar_sd,resetDiskAndWait},
	{FINISH_SONG,	volumen,			nextSong},

	

	{NONE, volumen, do_nothing}

};

state_edge_t ecualizador[] = {
	//eventos de hareware
	{MOV_ABAJO,		ecualizador, 	prevItemEc},
	{MOV_ARRIBA,	ecualizador, 	nextItemEc},
	{SELECT, 		ecualizador, 	selectItem},
	{HAY_SD, 		ecualizador, 	do_nothing},
	{NO_HAY_SD,		insertar_sd,	resetDiskAndWait},
	{FINISH_SONG,	ecualizador,			nextSong},


	//eventos de transicion
	{EC_PERZONALIZADO, 	ec_personalizado, 	showEcPersonalizado},
	{EC_CLASSIC, 		menu, 				normalBands},
	{EC_ROCK, 			menu, 				rockBands},
	{EC_POP, 			menu, 				popBands},
	{EC_JAZZ, 			menu, 				jazzBands},
	{BACK, 				menu, 				show_menu},

	{NONE, ecualizador, do_nothing}
};

state_edge_t ec_personalizado[] = {
	//eventos de hareware
	{MOV_ABAJO,		ec_personalizado, 	prevItemEcPers},
	{MOV_ARRIBA,	ec_personalizado, 	nextItemEcPers},
	{SELECT, 		ec_personalizado, 	selectItemEcPers},
	{HAY_SD, 		ec_personalizado, 	do_nothing},
	{NO_HAY_SD,		insertar_sd,		resetDiskAndWait},
	{FINISH_SONG,	ec_personalizado,			nextSong},


	//eventos de transicion
	{EDIT_CUSTOM_BAND, 	ec_personalizado, do_nothing},
	{APPLY, 			ecualizador, 		setCustomBand},
	{BACK, 				ecualizador, 		showEcualizador},

	{NONE, ec_personalizado, do_nothing_3}
};

state_edge_t canciones[] = {
	//eventos de hareware
	{MOV_ABAJO,		canciones, 		prevItem},
	{MOV_ARRIBA,	canciones, 		nextItem},
	{SELECT, 		canciones, 		selectItem},
	{HAY_SD, 		canciones, 		do_nothing},
	{NO_HAY_SD,		insertar_sd,	resetDiskAndWait},
	{FINISH_SONG,	canciones,			nextSong},



	//eventos de transicion
	{NEW_SONG, 		canciones, 	select_cancion},
	{NEXT_SONG, 	canciones, 	next_canciones},
	{PREV_SONG, 	canciones, 	prev_canciones},
	{BACK, 			menu, 		show_menu},


	{NONE, canciones, do_nothing}
};


/*******************************************************************************
 * STATIC SubMenu items
 ******************************************************************************/
static submenu_items_t * currentStateItem;

static submenu_items_t main_menu_items = {{	{0, PLAYER, PAUSE_RESUME_COL, PAUSE_RESUME_FIL},
									    	{1, VOLUMEN, VOLUMEN_COL, VOLUMEN_FIL},
											{2, ECUALIZADOR, ECUALIZADOR_COL, ECUALIZADOR_FIL},
											{3, CANCIONES, CANCIONES_COL, CANCIONES_FIL}
											},
									     	0, 4};

static submenu_items_t ecualizador_items = {{	{B1_PERZONALIZADO, EC_PERZONALIZADO, EC_PERZONALIZADO_COL, EC_PERZONALIZADO_FIL},
									    		{B2_CLASSIC, EC_CLASSIC, EC_CLASSIC_COL, EC_CLASSIC_FIL},
												{B3_ROCK, EC_ROCK, EC_ROCK_COL, EC_ROCK_FIL},
												{B4_POP, EC_POP, EC_POP_COL, EC_POP_FIL},
												{B5_JAZZ, EC_JAZZ, EC_JAZZ_COL, EC_JAZZ_FIL},
												{5, BACK, BACK_COL_LINE, BACK_FIL_LINE}
											},
									     	0, 6};

static submenu_items_t personalizado_items = {{	{B1, EDIT_CUSTOM_BAND, EC_BAND_COL, EC_BAND_FIL},
									    		{B2, EDIT_CUSTOM_BAND, EC_BAND_COL+4, EC_BAND_FIL},
												{B3, EDIT_CUSTOM_BAND, EC_BAND_COL+4*2, EC_BAND_FIL},
												{B4, EDIT_CUSTOM_BAND, EC_BAND_COL+4*3, EC_BAND_FIL},
												{B5, EDIT_CUSTOM_BAND, EC_BAND_COL+4*4, EC_BAND_FIL},
												{5,	APPLY , APPLY_COL, APPLY_FIL},
												{6, BACK, BACK_COL, BACK_FIL}
											},
									     	0, 7};


static submenu_items_t player_items = {{	{0, PAUSE_RESUME, PP_COL, PP_FIL},
									    	{1, NEXT_SONG, NEXT_COL, NEXT_FIL},
											{2, PREV_SONG, PREV_COL, PREV_FIL},
											{3, BACK, BACK_COL, BACK_FIL},
											},
									     	0, 4};

static submenu_items_t canciones_items = {{	{0, NEW_SONG, 0, 0},
											{1, NEW_SONG, 0, 1},
											{2, NEW_SONG, 0, 2},
											{3, NEXT_SONG, NEXT_COL, NEXT_FIL},
											{4, PREV_SONG, PREV_COL, PREV_FIL},
											{5, BACK, BACK_COL, BACK_FIL}
											},
									     	0, 6};


/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
static uint8_t row_index_song = 0;
static uint8_t subMenu = 0;
static uint8_t volumen_value = 15;


static uint8_t nivel1[8] = {0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000};
static uint8_t nivel2[8] = {0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000};
static uint8_t nivel3[8] = {0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100};
static uint8_t nivel4[8] = {0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110};
static uint8_t nivel5[8] = {0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111};

static char song[3][256] = {0};
static int song_index = 0;
static int current_song = 0;
static int8_t * gain_select;
static int8_t gain_custom[5] = {0};
static bool editting_band = false;
static songInfo_t song_info;
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

static void resetDiskAndWait(void){
	//Parar DAC
	audio_pause();

	//reset driver SD
	SD_reset();

	//mostrar que se inserte SD
	show_insertSD();
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

	//mostrar que inserte SD
	show_insertSD();

	matrixInit();
	turnOffMatrix();

}

//---------------------------------Rutinas insertarsd
static void estado_init_config_SD(){
	//Inicializar nueva SD
	//Mostrar leyendo SD
	//display 
	displayLCD_ShowStringLine("    Leyendo SD     ", 0);

	SD_cardStatus SDstatus;

	//if(SDstatus != SD_OK && SDstatus != SD_RDY){
	//bool ok = SD_initializationProcess();
	if(mountSD()){
		readSD();
		//inicializar estructura de canciones


		playMusicInit();
		show_menu();
	}
	else{
		SDstatus = SD_getStatus();
		displayLCD_ShowStringLine("  Error Leyendo SD  ", 0);
		//tratar error
		//while(1);
		EG_addEvent(NO_HAY_SD);
	}

	//}

	//mostrar menu
	
}

static void show_insertSD(){
	displayLCD_Clear();
	displayLCD_ShowStringLine("     Inserte SD     ", 0);

}


//-----------------------------Rutinas sleep
static void go_sleep(void){
	//mandar a sleep los modulos

}

//-----------------------------Rutinas menu
static void show_menu(){
	vumeterEnable(true);
	currentStateItem = &main_menu_items;
	displayLCD_ShowStringLine(DISP_PAUSE_RESUME, PAUSE_RESUME_FIL);
	displayLCD_ShowStringLine(DISP_CANCIONES, CANCIONES_FIL);
	displayLCD_ShowStringLine(DISP_ECUALIZADOR, ECUALIZADOR_FIL);
	displayLCD_ShowStringLine(DISP_VOLUMEN, VOLUMEN_FIL);
	displayLCD_ShowCursorAt(currentStateItem->item[currentStateItem->item_selec].cursor_pos_col,
				currentStateItem->item[currentStateItem->item_selec].cursor_pos_fil);
}

//-----------------------------Rutinas reproductor
static void show_reproductor(){
	currentStateItem = &player_items;
	displayLCD_Clear();
	displayLCD_ShowStringLine(DISP_ITEMS_PLAYERS, PP_FIL);
	displayLCD_ShowCursorAt(currentStateItem->item[currentStateItem->item_selec].cursor_pos_col,
				currentStateItem->item[currentStateItem->item_selec].cursor_pos_fil);
	show_song_info();
}

static void nextSong(void){
	//reproducir siguiente cancion
	if(current_song < getCantSongs()-1){
		current_song++;
	}
	else{
		current_song = 0;

	}
	playNewSong(getPath(current_song));
	//show_song_info();
	//audio_nextSong();
	//show_reproductor();

}

static void prevSong(void){
	//reproducir cancion anterior
	if(current_song > 0){
		current_song--;
	}
	else{
		current_song = getCantSongs()-1;
	}
	playNewSong(getPath(current_song));
	//show_song_info();
	//audio_prevSong();
	//show_reproductor();
}

static void pauseResume(void){
	
	if(isFileOpen()){
		playPauseMusic();
	}
	else{
		playNewSong(getPath(0));
		current_song = 0;
		playPauseMusic();
	}
	show_song_info();
}

static void show_song_info(){
	if(getSongInfo(&song_info)){
		char buff_1[21] = {0};
		char buff_2[21] = {0};
		char buff_3[21] = {0};
		strncpy(buff_1, song_info.title, 20);
		strncpy(buff_2, song_info.artist, 20);
		strncpy(buff_3, song_info.album, 20);
		displayLCD_ShowStringLine(buff_1, 0);
		displayLCD_ShowStringLine(buff_2, 1);
		displayLCD_ShowStringLine(buff_3, 2);
	}
}


//-----------------------------Rutinas canciones
static void select_cancion(void){
	playNewSong(song[currentStateItem->item_selec]);
	current_song = song_index - (2-currentStateItem->item_selec) - 1;
	//reproducir cancion seleccionada
	//audio_playSong(song[song_index]);
	//show_reproductor();
}

static void show_canciones(void){
	currentStateItem = &canciones_items;
	displayLCD_Clear();

	static bool init = true;
	if(init){
		init = false;
		next_canciones();
	}

	char buff_song1[21] = {0};
	char buff_song2[21] = {0};
	char buff_song3[21] = {0};
	strncpy(buff_song1,&(song[0][1]), 19);
	strncpy(buff_song2,&(song[1][1]), 19);
	strncpy(buff_song3,&(song[2][1]), 19);
	displayLCD_ShowStringAt(buff_song1, 1, 0);
	displayLCD_ShowStringAt(buff_song2, 1, 1);
	displayLCD_ShowStringAt(buff_song3, 1, 2);
	displayLCD_ShowStringLine(DISP_ITEMS_CANCIONES, 3);
	displayLCD_ShowCursorAt(currentStateItem->item[currentStateItem->item_selec].cursor_pos_col,
					currentStateItem->item[currentStateItem->item_selec].cursor_pos_fil);

	//displayLCD_ShowCursorAt(0, 0);
}

static void next_canciones(void)
{
	//acordarme de verificar que hay canciones en los path (que sean distintos de 0)
	//voy incrementando el song index y con eso pido los path y los guardo en los arreglos de cada cancion
	for(int i = 0; i < 256;i++){
		song[0][i] = 0;
		song[1][i] = 0;
		song[2][i] = 0;
	}
	for(int i = 0; i < 3; i++)
	{
		if(song_index + i < getCantSongs())
		{
			char * path = getPath(song_index + i);
			if(path[0] != 0){
				strncpy(song[i], path, strlen(path));
			}
			else
			{
				song[i][0] = 0;
			}
		}
	}
	song_index += 3;
	if(song_index > getCantSongs()){
		song_index = 0;
	}
	show_canciones();
}

static void prev_canciones()
{
	for(int i = 0; i < 256;i++){
			song[0][i] = 0;
			song[1][i] = 0;
			song[2][i] = 0;
	}
	for(int i = 2; i >= 0; i--)
	{
		if(song_index - i > 0)
		{
			char * path = getPath(song_index);
			if(path[0] != 0)
			{
				strncpy(song[i], path,strlen(path));
			}
			else
			{
				song[i][0] = 0;
			}
		}
	}
	song_index-=3;
	if(song_index < 0){
		song_index = getCantSongs() / 3;
	}
	show_canciones();

}

//-----------------------------Rutinas volumen
static void showVolumen(void){
	displayLCD_HideCursor();
	displayLCD_Clear();
	displayLCD_ShowStringAt(DISP_VOLUMEN, 0, 0);
	displayLCD_ShowCharAt('-', 0, 1);
	displayLCD_ShowCharAt('+', 19, 1);
	dibujarBarra(volumen_value);
}

static void dibujarBarra(int nivel) {
    int lineas = nivel*3; // por usar 18 caracteres de 5 leds (18*5=90), y 30 posiciones
	int llenos = lineas / 5;       // Caracteres completamente llenos
    int parcial = lineas % 5;      // Nivel parcial del siguiente carácter

    // Posicionar en la fila inicial
	char buff[21] = {0};
	buff[0] = '-';
	buff[19] = '+';
    for (int i = 0; i < 18; i++) { // Dibujar hasta 6 bloques (30 posiciones/5 niveles)
        if (i < llenos) {
			buff[i+1] = N5;
            //displayLCD_ShowCharAt(N5,i+1,1); // Carácter completamente lleno
        }
        else if (i == llenos && parcial > 0) {
			buff[i+1] = parcial-1;
        	//displayLCD_ShowCharAt(parcial-1,i+1,1); // Carácter parcial
        }
        else {
        	buff[i+1] = ' ';
//        	displayLCD_ShowCharAt(' ',i+1,1); // Carácter parcial
        }
    }
	displayLCD_ShowStringLine(buff, 1);

}

static void incrementVolumen(void){
	volumen_value++;
	if(volumen_value > MAX_VOL){
		volumen_value = MAX_VOL;
	}
	setVolumen(volumen_value);
	dibujarBarra(volumen_value);
}

static void decrementVolumen(void){
	if(volumen_value > 0){
		volumen_value--;
	}
	setVolumen(volumen_value);
	dibujarBarra(volumen_value);

}

static void selectVolumen(void){
	//setear volumen
	//audio_setVolume(volumen_value);
	show_menu();
}

//-----------------------------Rutinas ecualizador
static void showEcualizador(void){
	vumeterEnable(false);
	currentStateItem = &ecualizador_items;
	displayLCD_Clear();
	displayLCD_ShowStringAt(DISP_EC_CLASSIC, EC_CLASSIC_COL, EC_CLASSIC_FIL);
	displayLCD_ShowStringAt(DISP_EC_ROCK, EC_ROCK_COL, EC_ROCK_FIL);
	displayLCD_ShowStringAt(DISP_EC_POP, EC_POP_COL, EC_POP_FIL);
	displayLCD_ShowStringAt(DISP_EC_JAZZ, EC_JAZZ_COL, EC_JAZZ_FIL);
	displayLCD_ShowStringLine(DISP_BACK_LINE, BACK_FIL_LINE);
	displayLCD_ShowStringLine(DISP_EC_PERZONALIZADO, EC_PERZONALIZADO_FIL);
	displayLCD_ShowCursorAt(currentStateItem->item[currentStateItem->item_selec].cursor_pos_col, 
				currentStateItem->item[currentStateItem->item_selec].cursor_pos_fil);
}

static void normalBands(){
	//setear ecualizador
	//audio_setEcualizador(NORMAL);
	equalizerInit(gain_select);

	show_menu();
}

static void rockBands(){

	equalizerInit(gain_select);


	//setear ecualizador
	//audio_setEcualizador(ROCK);
	show_menu();
}

static void popBands(){
	equalizerInit(gain_select);
	//setear ecualizador
	//audio_setEcualizador(POP);
	show_menu();
}

static void jazzBands(){
	equalizerInit(gain_select);
	//setear ecualizador
	//audio_setEcualizador(JAZZ);
	show_menu();
}

static void nextItemEc(void){
	currentStateItem->item_selec++;
	if(currentStateItem->item_selec >= currentStateItem->item_cant){
		currentStateItem->item_selec = 0;
	}
	show_gain(currentStateItem->item_selec);
	displayLCD_ShowCursorAt(currentStateItem->item[currentStateItem->item_selec].cursor_pos_col, 
				currentStateItem->item[currentStateItem->item_selec].cursor_pos_fil);
}

static void prevItemEc(void){
	if(currentStateItem->item_selec == 0){
		currentStateItem->item_selec = currentStateItem->item_cant - 1;
	}
	else{
		currentStateItem->item_selec--;
	}	
	show_gain(currentStateItem->item_selec);
	displayLCD_ShowCursorAt(currentStateItem->item[currentStateItem->item_selec].cursor_pos_col, 
				currentStateItem->item[currentStateItem->item_selec].cursor_pos_fil);
}

static void show_gain(uint8_t item_index){
	switch(item_index){
		/*
		case 1:
			displayLCD_ShowStringLine("Normal", 0);
			break;
		*/
		case B1_PERZONALIZADO:
			gain_select = getGainPreset(CUSTOM, gain_custom);
			break;
		case B2_CLASSIC:
			gain_select = getGainPreset(CLASSIC, gain_custom);
			break;
		case B3_ROCK:
			gain_select = getGainPreset(ROCK, gain_custom);
			break;
		case B4_POP:
			gain_select = getGainPreset(POP, gain_custom);
			break;
		case B5_JAZZ:
			gain_select = getGainPreset(JAZZ, gain_custom);
			break;
		default:
			gain_select = getGainPreset(NORMAL, gain_custom);
			break;	
	}
}

//-----------------------------Rutinas ec_personalizado
static void showEcPersonalizado(void){
	currentStateItem = &personalizado_items;
	displayLCD_Clear();
	displayLCD_ShowStringLine(DISP_EC_PERZONALIZADO, 0);
	displayLCD_ShowStringLine(DISP_EC_BANDS, EC_BAND_FIL);
	//mostrar bandas 
	displayLCD_ShowNumberAt(gain_custom[0], 1, EC_BAND_FIL+1);
	displayLCD_ShowNumberAt(gain_custom[1], 1*4+1, EC_BAND_FIL+1);
	displayLCD_ShowNumberAt(gain_custom[2], 2*4+1, EC_BAND_FIL+1);
	displayLCD_ShowNumberAt(gain_custom[3], 3*4+1, EC_BAND_FIL+1);
	displayLCD_ShowNumberAt(gain_custom[4], 4*4+1, EC_BAND_FIL+1);
	displayLCD_ShowStringLine(DISP_EC_BACK_APPLY, APPLY_FIL);
	displayLCD_ShowCursorAt(currentStateItem->item[currentStateItem->item_selec].cursor_pos_col, 
				currentStateItem->item[currentStateItem->item_selec].cursor_pos_fil);
}

static void nextItemEcPers(void){
	if(editting_band){
		gain_custom[currentStateItem->item_selec] += 1;
		if(gain_custom[currentStateItem->item_selec] > 10){
			gain_custom[currentStateItem->item_selec] = 10;
		}
		//mostrar ganancia
		getGainPreset(CUSTOM,gain_custom);
		displayLCD_ShowNumberAt(gain_custom[currentStateItem->item_selec],
				currentStateItem->item[currentStateItem->item_selec].cursor_pos_col, 
				currentStateItem->item[currentStateItem->item_selec].cursor_pos_fil+1);
	}
	else{
		currentStateItem->item_selec++;
		if(currentStateItem->item_selec >= currentStateItem->item_cant){
			currentStateItem->item_selec = 0;
		}
		displayLCD_ShowCursorAt(currentStateItem->item[currentStateItem->item_selec].cursor_pos_col, 
					currentStateItem->item[currentStateItem->item_selec].cursor_pos_fil);
	}
}

static void prevItemEcPers(void){
	if(editting_band){
		gain_custom[currentStateItem->item_selec] -= 1;
		if(gain_custom[currentStateItem->item_selec] < -10){
			gain_custom[currentStateItem->item_selec] = -10;
		}
		//mostrar ganancia
		getGainPreset(CUSTOM,gain_custom);
		displayLCD_ShowNumberAt(gain_custom[currentStateItem->item_selec],
				currentStateItem->item[currentStateItem->item_selec].cursor_pos_col, 
				currentStateItem->item[currentStateItem->item_selec].cursor_pos_fil+1);
	}
	else{
		if(currentStateItem->item_selec == 0){
			currentStateItem->item_selec = currentStateItem->item_cant - 1;
		}
		else{
			currentStateItem->item_selec--;
		}	
		displayLCD_ShowCursorAt(currentStateItem->item[currentStateItem->item_selec].cursor_pos_col, 
					currentStateItem->item[currentStateItem->item_selec].cursor_pos_fil);
	}
}

static void selectItemEcPers(void){
	if(currentStateItem->item_selec < 5 ){ //edit band segun item_selec
		if(editting_band){
			editting_band = false;
		}
		else{
			editting_band = true;	
			//indicar que se va a modificar la banda
		}
	}
	EG_addEvent((currentStateItem->item[currentStateItem->item_selec].item_event_select));
}

static void setCustomBand(void){
	gain_select = getGainPreset(CUSTOM, gain_custom);

	showEcualizador();
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

