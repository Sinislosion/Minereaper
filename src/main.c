#include "Raquet.h"
#include "game.h"
#include "buttons.h"
#include "mouse.h"
#include "smiley.h"
#include <time.h>

Raquet_Sound snd_win;
Raquet_Sound mus_chip01;

int Get_Random(int seed, int a, int c, int m) {
    return (a*seed+c) % m;
}

void createthedog() {

    snd_win = Raquet_LoadSound(Raquet_AbsoluteToAsset("snd_win.wav"));
    mus_chip01 = Raquet_LoadSound(Raquet_AbsoluteToAsset("mus_chip01.wav"));

    Raquet_SetPalette(pal_button, Raquet_GlobalPalette[0x12], Raquet_GlobalPalette[0x0D], Raquet_GlobalPalette[0x10], Raquet_GlobalPalette[0x30]);
    Raquet_SetPalette(pal_button_pressed, Raquet_GlobalPalette[0x12], Raquet_GlobalPalette[0x0D], Raquet_GlobalPalette[0x00], Raquet_GlobalPalette[0x10]);
    Raquet_SetPalette(pal_text_gameover, Raquet_GlobalPalette[0x0D], Raquet_GlobalPalette[0x30], Raquet_GlobalPalette[0x0D], Raquet_GlobalPalette[0x0D]);

    Raquet_LoadPPFBank(&ppf_main, Raquet_AbsoluteToAsset("main.ppf"));
    int arr_buttonsprites[NUM_BUTTON_TYPES][4] = {
        {0, 1, 16, 17},   // 1
        {2, 3, 18, 19},   // 2
        {4, 5, 20, 21},   // 3
        {6, 7, 22, 23},   // 4
        {8, 9, 24, 25},   // 5
        {10, 11, 26, 27}, // 6
        {12, 13, 28, 29}, // 7
        {14, 15, 30, 31}, // 8
        {32, 33, 48, 49}, // FLAG
        {34, 35, 50, 51}, // BOMB
        {36, 37, 52, 53}  // NONE
    };
    chr_buttons[0] = Raquet_LoadCHRMult(ppf_main, arr_buttonsprites[0], 2, 2, pal_button);
    chr_buttons[1] = Raquet_LoadCHRMult(ppf_main, arr_buttonsprites[1], 2, 2, pal_button);
    chr_buttons[2] = Raquet_LoadCHRMult(ppf_main, arr_buttonsprites[2], 2, 2, pal_button);
    chr_buttons[3] = Raquet_LoadCHRMult(ppf_main, arr_buttonsprites[3], 2, 2, pal_button);
    chr_buttons[4] = Raquet_LoadCHRMult(ppf_main, arr_buttonsprites[4], 2, 2, pal_button);
    chr_buttons[5] = Raquet_LoadCHRMult(ppf_main, arr_buttonsprites[5], 2, 2, pal_button);
    chr_buttons[6] = Raquet_LoadCHRMult(ppf_main, arr_buttonsprites[6], 2, 2, pal_button);
    chr_buttons[7] = Raquet_LoadCHRMult(ppf_main, arr_buttonsprites[7], 2, 2, pal_button);
    chr_buttons[8] = Raquet_LoadCHRMult(ppf_main, arr_buttonsprites[8], 2, 2, pal_button);
    chr_buttons[9] = Raquet_LoadCHRMult(ppf_main, arr_buttonsprites[9], 2, 2, pal_button);
    chr_buttons[10] = Raquet_LoadCHRMult(ppf_main, arr_buttonsprites[10], 2, 2, pal_button);

    srand(time(NULL));
    Setup_Board(rand());


    // LOAD CURSOR
    Mouse_Init();

    // LOAD SMILEY
    Smiley_Init();

    Raquet_ShowCursor(0);

    Raquet_PlaySound(mus_chip01, -1, 2);

}

void runthedog() {
    if (Raquet_KeyCheck(SDL_SCANCODE_Z)) {
        CUR_GAME_STATE = YOU_WIN;
    }
    switch (CUR_GAME_STATE) {
        case RUNNING:
            Game_Running();
            break;
        case GAME_OVER:
            Game_GameOver();
            break;
        case YOU_WIN:
            Game_YouWin();
            break;
    }
}

int main() {
    Raquet_Main();
    return 0;
}
