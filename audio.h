/*
 * audio.h
 *
 *  Created on: 2 abr. 2024
 *      Author: juanm
 */

#ifndef AUDIO_H_
#define AUDIO_H_

void audio_init(uint32_t _fs, uint16_t* source, uint16_t* source2, uint32_t frameSize, bool* _flag);


void audio_resume();
void audio_pause();
#endif /* AUDIO_H_ */
