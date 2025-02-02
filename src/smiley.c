#include <time.h>
#include "smiley.h"
#include "game.h"
#include "mouse.h"
#include "buttons.h"
#include "music.h"

#define SMILEY_WIN_FRAMES   3

Palette pal_smiley[4];

typedef enum SMILEY_STATES {
    SMILEY_MAIN,
    SMILEY_STUNNED,
    SMILEY_THINKING,
    SMILEY_DEATH,
    SMILEY_WIN
} SMILEY_STATES;

SMILEY_STATES CUR_SMILEY_STATE = SMILEY_MAIN;

Raquet_CHR chr_smiley_main;
Raquet_CHR chr_smiley_stunned;
Raquet_CHR chr_smiley_death;
Raquet_CHR chr_smiley_thinking[5];
Raquet_CHR chr_smiley_win[SMILEY_WIN_FRAMES];

#define SMILEY_X        (SCREEN_WIDTH/2) - 8
#define SMILEY_Y        0

Raquet_Actor* act_smiley;

int arr_smiley_main[4] = {39, 40, 55, 56};
int arr_smiley_stunned[4] = {41, 42, 57, 58};
int arr_smiley_death[4] = {43, 44, 59, 60};
int arr_smiley_thinking[5][4] = {
    {96, 97, 112, 113},
    {98, 99, 112, 113},
    {100, 101, 112, 113},
    {114, 115, 112, 113},
    {116, 117, 112, 113}
};

int arr_smiley_winface[SMILEY_WIN_FRAMES][4] = {
    {102, 103, 112, 113},
    {104, 105, 120, 121},
    {106, 107, 122, 123}
};

int smileyWinFrame = 0;

float animTimer = 0;
int animFrame = 0;

int timeToThink = 0;
int thinkTime = 60;

void Smiley_Init(void) {

    Raquet_SetPalette(pal_smiley, Raquet_GlobalPalette[0x17], Raquet_GlobalPalette[0x0D], Raquet_GlobalPalette[0x27], Raquet_GlobalPalette[0x20]);

    chr_smiley_main = Raquet_LoadCHRMult(ppf_main, arr_smiley_main, 2, 2, pal_smiley);
    chr_smiley_stunned = Raquet_LoadCHRMult(ppf_main, arr_smiley_stunned, 2, 2, pal_smiley);
    chr_smiley_death = Raquet_LoadCHRMult(ppf_main, arr_smiley_death, 2, 2, pal_smiley);

    for (int i = 0; i < 5; ++i) {
        chr_smiley_thinking[i] = Raquet_LoadCHRMult(ppf_main, arr_smiley_thinking[i], 2, 2, pal_smiley);
    }

    for (int i = 0; i < SMILEY_WIN_FRAMES; ++i) {
        chr_smiley_win[i] = Raquet_LoadCHRMult(ppf_main, arr_smiley_winface[i], 2, 2, pal_smiley);
    }

    act_smiley = Raquet_AllocateActor();
    Raquet_CreateActor(act_smiley, chr_smiley_main);
    act_smiley -> position.x = SMILEY_X;
    act_smiley -> position.y = SMILEY_Y;

    timeToThink = 0;
    thinkTime = (rand() % 60) + 240;

}

void Smiley_Main(void) {
    switch (CUR_SMILEY_STATE) {
        case SMILEY_MAIN:
            Raquet_PlaceCHR(chr_smiley_main, SMILEY_X, SMILEY_Y);
            if (Raquet_ActorColliding(SMILEY_X, SMILEY_Y, act_smiley, act_cursor)) {
                CUR_SMILEY_STATE = SMILEY_STUNNED;
            }
            if (CUR_GAME_STATE == GAME_OVER) {
                CUR_SMILEY_STATE = SMILEY_DEATH;
            }

            timeToThink++;
            if (timeToThink > thinkTime) {
                timeToThink = 0;
                CUR_SMILEY_STATE = SMILEY_THINKING;
            }

            smileyWinFrame = rand() % SMILEY_WIN_FRAMES;

            break;

        case SMILEY_STUNNED:
            Raquet_PlaceCHR(chr_smiley_stunned, SMILEY_X, SMILEY_Y);
            if (!Raquet_ActorColliding(SMILEY_X, SMILEY_Y, act_smiley, act_cursor)) {
                CUR_SMILEY_STATE = SMILEY_MAIN;
            }

            if (Raquet_MouseCheck_Pressed(RAQUET_MOUSE_LEFT)) {
                srand(time(NULL));
                firstClick = 1;
                Setup_Board(rand());
                CUR_GAME_STATE = RUNNING;
                Music_Play();
            }

            if (CUR_GAME_STATE == GAME_OVER) {
                CUR_SMILEY_STATE = SMILEY_DEATH;
            }
            break;

        case SMILEY_DEATH:
            Raquet_PlaceCHR(chr_smiley_death, SMILEY_X, SMILEY_Y);
            if (Raquet_MouseCheck_Pressed(RAQUET_MOUSE_LEFT)) {
                srand(time(NULL));
                firstClick = 1;
                Setup_Board(rand());
                CUR_SMILEY_STATE = SMILEY_STUNNED;
                CUR_GAME_STATE = RUNNING;
                Music_Play();
            }
            break;

        case SMILEY_THINKING:
            animTimer += 0.1;
            if (animTimer >= 1) {
                animTimer = 0;
                animFrame++;
                if (animFrame > 4) {
                    animFrame = 0;
                }
            }

            if (timeToThink > thinkTime * 2 || Raquet_ActorColliding(SMILEY_X, SMILEY_Y, act_smiley, act_cursor)) {
                animTimer = 0;
                animFrame = 0;
                if (Raquet_ActorColliding(SMILEY_X, SMILEY_Y, act_smiley, act_cursor)) {
                    CUR_SMILEY_STATE = SMILEY_STUNNED;
                } else {
                    CUR_SMILEY_STATE = SMILEY_MAIN;
                }
                thinkTime = (rand() % 60) + 240;
                timeToThink = 0;
            }

            timeToThink++;

            Raquet_PlaceCHR(chr_smiley_thinking[animFrame], SMILEY_X, SMILEY_Y);
            break;

        case SMILEY_WIN:
            if (Raquet_ActorColliding(SMILEY_X, SMILEY_Y, act_smiley, act_cursor)) {
                if (Raquet_MouseCheck(RAQUET_MOUSE_LEFT)) {
                    srand(time(NULL));
                    firstClick = 1;
                    Setup_Board(rand());
                    CUR_SMILEY_STATE = SMILEY_STUNNED;
                    CUR_GAME_STATE = RUNNING;
                    Music_Play();
                }
            }
            Raquet_PlaceCHR(chr_smiley_win[smileyWinFrame], SMILEY_X, SMILEY_Y);
            break;
    }

    if (CUR_GAME_STATE == YOU_WIN) {
        CUR_SMILEY_STATE = SMILEY_WIN;
    }

    if (CUR_GAME_STATE == GAME_OVER) {
        CUR_SMILEY_STATE = SMILEY_DEATH;
    }
}