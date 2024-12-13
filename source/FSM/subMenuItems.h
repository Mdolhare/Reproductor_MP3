#ifndef SUBMENUITEMS_H
#define SUBMENUITEMS_H

#include <stdint.h>

// ubicacion items menu en display
#define PAUSE_RESUME_COL 	0
#define PAUSE_RESUME_FIL 	0
#define VOLUMEN_COL 		0
#define VOLUMEN_FIL 		1
#define ECUALIZADOR_COL 	0
#define ECUALIZADOR_FIL 	2
#define CANCIONES_COL 		0
#define CANCIONES_FIL 		3

// ubicacion items ecualizador en display
#define EC_PERZONALIZADO_COL 	0
#define EC_PERZONALIZADO_FIL 	2
#define EC_CLASSIC_COL 			0
#define EC_CLASSIC_FIL 			0
#define EC_ROCK_COL 			0
#define EC_ROCK_FIL 			1
#define EC_POP_COL 				10
#define EC_POP_FIL 				0
#define EC_JAZZ_COL 			10
#define EC_JAZZ_FIL 			1
#define BACK_COL_LINE			0
#define BACK_FIL_LINE			3

// ubicacion items players
#define BACK_COL 				15
#define BACK_FIL 				3
#define NEXT_COL 				8
#define NEXT_FIL 				3
#define PREV_COL 				0
#define PREV_FIL 				3
#define PP_COL 					3
#define PP_FIL 					3


// ubicacion items canciones
#define NEXT_COL 				6
#define NEXT_FIL 				3
#define PREV_COL 				0
#define PREV_FIL 				3

// ubicacion items ecualizador personalizado
#define EC_BAND_FIL 			1
#define EC_BAND_COL				1
#define APPLY_FIL 				3
#define APPLY_COL				0



// Strings para mostrar en display del menu
#define DISP_PAUSE_RESUME 	"      Player        "
#define DISP_VOLUMEN 		"      Volumen       "
#define DISP_ECUALIZADOR 	"    Ecualizador     "
#define DISP_CANCIONES 		"     Canciones      "

// String para mostrar en el player " <  ||  >     Back"
#define DISP_ITEMS_PLAYERS 	" <  ||  >       Back"
#define DISP_PREV 			"<"
#define DISP_PP 			"||"
#define DISP_NEXT 			">"
#define DISP_BACK 			"Back"

// String para mostrar en display del ecualizador
#define DISP_EC_PERZONALIZADO 	"    Personalizado   "
#define DISP_EC_CLASSIC			" Clasico  "
#define DISP_EC_ROCK 			" Rock     "
#define DISP_EC_POP 			" Pop      "
#define DISP_EC_JAZZ 			" Jazz     "
#define DISP_BACK_LINE 			"        Back        "

// String para mostrar en display del ecualizador personalizado
#define DISP_EC_BANDS			"  B1  B2  B3  B4  B5"
#define DISP_EJEMPLO			" +00 +00 +00 +00 +00"
#define DISP_EC_BACK_APPLY		" Apply          Back"

// String para mostrar en display de canciones
#define DISP_ITEMS_CANCIONES	" Prev  Next     Back"



//Items definitions
#define MAX_ITEMS 10
typedef struct{
	uint8_t item_numer;
	uint8_t item_event_select;
	uint8_t cursor_pos_col;
	uint8_t cursor_pos_fil;
}item_t;

typedef struct{
	item_t item[MAX_ITEMS];
	uint8_t item_selec;
	uint8_t item_cant;
}submenu_items_t;


#endif // SUBMENUITEMS_H
