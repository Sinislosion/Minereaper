#ifndef SMILEY_H
#define SMILEY_H

#include "game.h"

extern Palette pal_smiley[4];
extern Raquet_CHR chr_smiley;

extern Raquet_Actor* act_smiley;

extern int arr_smiley_main[4];

extern void Smiley_Init(void);
extern void Smiley_Main(void);

#endif