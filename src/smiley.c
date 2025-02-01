#include <time.h>
#include "smiley.h"
#include "game.h"
#include "mouse.h"
#include "buttons.h"

Palette pal_smiley[4];

typedef enum SMILEY_STATES {
    SMILEY_MAIN,
    SMILEY_STUNNED
} SMILEY_STATES;

SMILEY_STATES CUR_SMILEY_STATE;

Raquet_CHR chr_smiley_main;
Raquet_CHR chr_smiley_stunned;

#define SMILEY_X        (SCREEN_WIDTH/2) - 8
#define SMILEY_Y        0

Raquet_Actor* act_smiley;

int arr_smiley_main[4] = {39, 40, 55, 56};
int arr_smiley_stunned[4] = {41, 42, 57, 58};

void Smiley_Init(void) {

    Raquet_SetPalette(pal_smiley, Raquet_GlobalPalette[0x0D], Raquet_GlobalPalette[0x0D], Raquet_GlobalPalette[0x27], Raquet_GlobalPalette[0x20]);
    chr_smiley_main = Raquet_LoadCHRMult(ppf_main, arr_smiley_main, 2, 2, pal_smiley);

    chr_smiley_stunned = Raquet_LoadCHRMult(ppf_main, arr_smiley_stunned, 2, 2, pal_smiley);

    act_smiley = Raquet_AllocateActor();
    Raquet_CreateActor(act_smiley, chr_smiley_main);
    act_smiley -> position.x = SMILEY_X;
    act_smiley -> position.y = SMILEY_Y;

}

void Smiley_Main(void) {
    switch (CUR_SMILEY_STATE) {
        case SMILEY_MAIN:
            Raquet_PlaceCHR(chr_smiley_main, SMILEY_X, SMILEY_Y);
            if (Raquet_ActorColliding(SMILEY_X, SMILEY_Y, act_smiley, act_cursor)) {
                CUR_SMILEY_STATE = SMILEY_STUNNED;
            }
            break;

        case SMILEY_STUNNED:
            Raquet_PlaceCHR(chr_smiley_stunned, SMILEY_X, SMILEY_Y);
            if (!Raquet_ActorColliding(SMILEY_X, SMILEY_Y, act_smiley, act_cursor)) {
                CUR_SMILEY_STATE = SMILEY_MAIN;
            }

            if (Raquet_MouseCheck_Pressed(RAQUET_MOUSE_LEFT)) {
                srand(time(NULL));
                Setup_Board(rand());
                CUR_GAME_STATE = RUNNING;
            }
            break;
    }
}