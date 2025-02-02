#ifndef MUSIC_H
#define MUSIC_H

#include "Raquet_Sound.h"

#define NUM_SONGS   2
extern Raquet_Sound Music_Songs[NUM_SONGS];

extern void Music_Init(void);

extern void Music_Play(void);

extern void Music_Stop(void);

#endif