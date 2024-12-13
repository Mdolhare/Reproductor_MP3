/*
 * playMusic.h
 *
 *  Created on: Apr 1, 2024
 *      Author: Grupo 2
 */

#ifndef PLAY_MUSIC_H_
#define PLAY_MUSIC_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef struct {
    char title[30];
    char artist[30];
    char album[30];
}songInfo_t;

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/




/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/
void playMusicInit(void);
void setVolumen(uint8_t _volumen);
void playMusic(void);
void playPauseMusic(void);
void playNewSong(char * song);
void vumeterEnable(bool enable);
bool getSongInfo(songInfo_t * song);


/*******************************************************************************
 ******************************************************************************/




#endif /* PLAY_MUSIC_H_ */
