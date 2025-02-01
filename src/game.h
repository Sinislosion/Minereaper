#ifndef GAME_H
#define GAME_H

#include "Raquet.h"

#define NUM_BUTTON_TYPES    11
#define NUM_BUTTONS         224
#define NUM_BOMBS           30

#define NUM_ROWS            14
#define NUM_COLUMNS         16

typedef enum GAME_STATE {
    TITLE,
    RUNNING,
    GAME_OVER
} GAME_STATE;

extern GAME_STATE CUR_GAME_STATE;

extern PPF_Bank ppf_main;

extern uint8_t firstClick;

extern Palette pal_text_gameover[4];

extern void Game_GameOver(void);
extern void Game_Running(void);

#endif
