/***************************************************************************//**
  @file     map3decoder.h
  @brief    Driver to decoder an mp3 file
  @author   Grupo 2 2023
 ******************************************************************************/

#ifndef _MP3DECODER_H_
#define _MP3DECODER_H_



/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "../../helix/pub/mp3dec.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/



/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef struct {
	char tag[3];
	char title[30];
	char artist[30];
	char album[30];
	char year[4];
	char comment[30];
	char genre;
} ID3Tag_t;

typedef struct {
  char tag[3];
  char title[30];
  char artist[30];
  char album[30];
  char year[4];
  char comment[30];
  unsigned char genre;
} ID3Tag;


/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/


void decoderInit(void);
bool decoderGetFrame(unsigned char *frame_decode, MP3FrameInfo * _frameInfo);
void setPath(char * newPath);
bool isFileOpen(void);
void setIsFileOpen(bool b);
ID3Tag_t * getID3Tag(void);
void readID3Tag(void);

#endif // _MP#DECODER_H_
