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



/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/


 void decoderInit(void);
 bool decoderGetFrame(unsigned char *frame_decode, MP3FrameInfo * _frameInfo);



#endif // _MP#DECODER_H_