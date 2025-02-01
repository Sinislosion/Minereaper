#include "Raquet.h"

#ifndef BUTTONS_H
#define BUTTONS_H

#include "game.h"

typedef struct Button {
    Raquet_CHR* chr;
    uint8_t x, y, isBeingPressed, isPressed, type, isFlagged;
} Button;

extern Button arr_buttons[16][15];
extern Raquet_CHR chr_buttons[NUM_BUTTON_TYPES];

extern Palette pal_button[4];
extern Palette pal_button_pressed[4];

extern void Fill_Empty(int x, int y);

extern void Create_Number_Button(int x, int y);

extern void Draw_Button(int x, int y);

extern void Setup_Board(int seed);

#endif
