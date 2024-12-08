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
#define EC_NORMAL_COL 			0
#define EC_NORMAL_FIL 			0
#define EC_ROCK_COL 			0
#define EC_ROCK_FIL 			1
#define EC_POP_COL 				10
#define EC_POP_FIL 				0
#define EC_JAZZ_COL 			10
#define EC_JAZZ_FIL 			1
#define BACK_COL 				0
#define BACK_FIL 				3



// Strings para mostrar en display del menu
#define DISP_PAUSE_RESUME 	"    Pause/Resume    "
#define DISP_VOLUMEN 		"      Volumen       "
#define DISP_ECUALIZADOR 	"    Ecualizador     "
#define DISP_CANCIONES 		"     Canciones      "

// String para mostrar en display del ecualizador
#define DISP_EC_PERZONALIZADO 	"    Personalizado   "
#define DISP_EC_NORMAL 			" Normal   "
#define DISP_EC_ROCK 			" Rock     "
#define DISP_EC_POP 			" Pop      "
#define DISP_EC_JAZZ 			" Jazz     "
#define DISP_BACK 				"        Back        "

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
