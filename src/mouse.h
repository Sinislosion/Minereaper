#ifndef MOUSE_H
#define MOUSE_H

#include "Raquet.h"

extern Raquet_CHR chr_cursor;
extern Raquet_Actor* act_cursor;

extern void Mouse_Init(void);
extern void Mouse_Main(void);
extern void Mouse_Draw(void);

#endif