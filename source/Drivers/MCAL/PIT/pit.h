
#ifndef PIT_H
#define PIT_H

#include <stdbool.h>
#include <stdint.h>

#include "hardware.h"




typedef void (*pitIrqFun_t)(void);


#define CANT_PITS 4

enum pits {PIT_0, PIT_1, PIT_2, PIT_3};


void pitInit(void);


void pitSetAndBegin(uint32_t channel, uint32_t time);

void pitStopTimer(uint32_t channel);

void pitSetIRQFunc(uint32_t channel, pitIrqFun_t pitFunc);

void pitDisableIRQFunc(uint32_t channel);


#endif /* PIT_H */
