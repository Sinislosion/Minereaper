#include "Raquet.h"
PPF_Bank ppf_main;

#define NUM_BUTTON_TYPES    11

Raquet_CHR chr_buttons[NUM_BUTTON_TYPES];
Raquet_CHR chr_cursor;

char* arr_buttons;

Raquet_Actor* act_cursor;

void createthedog() {
    Raquet_LoadPPFBank(&ppf_main, Raquet_AbsoluteToAsset("main.ppf"));
    Palette pal_button[4] = {Raquet_GlobalPalette[0x12], Raquet_GlobalPalette[0x0D], Raquet_GlobalPalette[0x10], Raquet_GlobalPalette[0x30]};
    Palette pal_cursor[4] = {Raquet_GlobalPalette[0x0F], Raquet_GlobalPalette[0x0D], Raquet_GlobalPalette[0x10], Raquet_GlobalPalette[0x30]};
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



    arr_buttons = (char*)malloc(sizeof(char) * 224);
    memset(arr_buttons, 10, sizeof(char) * 224);

    // LOAD CURSOR
    chr_cursor = Raquet_LoadCHR(ppf_main, 38, pal_cursor);
    act_cursor = Raquet_AllocateActor();
    Raquet_CreateActor(act_cursor, chr_cursor);

    Raquet_ShowCursor(0);
    
}

void Mouse_Main() {
    
    #ifndef INTEGER_SCALING
        int x = 0;
        int y = 0;
        float newx = 0;
        float newy = 0;
      
        SDL_GetMouseState(&x, &y);
        SDL_RenderWindowToLogical(Raquet_Renderer, x, y, &newx, &newy);

        act_cursor -> position.x = (int)newx;
        act_cursor -> position.y = (int)newy;
        
        printf("%d, %d\n", act_cursor -> position.x, act_cursor -> position.y);
    #else
        int x = 0;
        int y = 0;
        SDL_GetMouseState(&x, &y);
        
        act_cursor -> position.x = x / (Raquet_RectScreenScale.w / SCREEN_WIDTH);
        act_cursor -> position.y = y / (Raquet_RectScreenScale.h / SCREEN_HEIGHT);
    #endif

}

void runthedog() {
    Raquet_Clear(Raquet_GlobalPalette[0x11]);
  
    Mouse_Main();

    for (int y = 0; y < 14; ++y) {
        for (int x = 0; x < 16; ++x) {
            Raquet_PlaceCHR(chr_buttons[(int)arr_buttons[x * y]], x * 16, 16 + (y * 16));
        }
    }

    Raquet_DrawActor(act_cursor);
    
}

int main() {
    Raquet_Main();
    return 0;
}
