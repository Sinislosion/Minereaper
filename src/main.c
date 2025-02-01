#include "Raquet.h"
#include <time.h>

PPF_Bank ppf_main;

#define NUM_BUTTON_TYPES    11
#define NUM_BUTTONS         224
#define NUM_BOMBS           25

#define NUM_ROWS            14
#define NUM_COLUMNS         16

typedef struct Button {
    Raquet_CHR* chr;
    uint8_t x, y, isBeingPressed, isPressed, type, isFlagged;
} Button;

typedef enum GAME_STATE {
    TITLE,
    RUNNING,
    GAME_OVER
} GAME_STATE;

GAME_STATE CUR_GAME_STATE = RUNNING;

Palette pal_button[4];
Palette pal_button_pressed[4];
Palette pal_text_gameover[4];

Raquet_CHR chr_buttons[NUM_BUTTON_TYPES];
Raquet_CHR chr_cursor;
Raquet_CHR chr_smile;

Button arr_buttons[16][15];

Raquet_Actor* act_cursor;

int Get_Random(int seed, int a, int c, int m) {
    return (a*seed+c) % m;
}

void Fill_Empty(int x, int y) {
    if (arr_buttons[x][y].isPressed == 1 || arr_buttons[x][y].type != 10) {
        return;
    }

    arr_buttons[x][y].isPressed = 1;

    if (x - 1 >= 0) {
        Fill_Empty(x - 1, y);
    }

    if (x + 1 < NUM_COLUMNS) {
        Fill_Empty(x + 1, y);
    }

    if (y - 1 >= 0) {
        Fill_Empty(x, y - 1);
    }
    
    if (y + 1 < NUM_ROWS) {
        Fill_Empty(x, y + 1);
    }

    if (x - 1 >= 0 && arr_buttons[x - 1][y].type != 10) {
        arr_buttons[x - 1][y].isPressed = 1;
    }
    
    if (x + 1 < NUM_COLUMNS && arr_buttons[x + 1][y].type != 10) {
        arr_buttons[x + 1][y].isPressed = 1;
    }

    if (y + 1 < NUM_ROWS && arr_buttons[x][y + 1].type != 10) {
        arr_buttons[x][y + 1].isPressed = 1;
    }
    
    if (y - 1 >= 0 && arr_buttons[x][y - 1].type != 10) {
        arr_buttons[x][y - 1].isPressed = 1;
    }
    
    if (x - 1 >= 0 && y - 1 >= 0 && arr_buttons[x - 1][y - 1].type != 10) {
        arr_buttons[x - 1][y - 1].isPressed = 1;
    }

    if (x + 1 < NUM_COLUMNS && y + 1 < NUM_ROWS && arr_buttons[x + 1][y + 1].type != 10) {
        arr_buttons[x + 1][y + 1].isPressed = 1;
    }

    if (x - 1 <= 0 && y + 1 < NUM_ROWS && arr_buttons[x - 1][y + 1].type != 10) {
        arr_buttons[x - 1][y + 1].isPressed = 1;
    }

    if (x + 1 < NUM_COLUMNS && y - 1 >= 0 && arr_buttons[x + 1][y - 1].type != 10) {
        arr_buttons[x + 1][y - 1].isPressed = 1;
    }

}

void Create_Number_Button(int x, int y) {
    if (arr_buttons[x][y].type == 10) {
        int numbombs = 0;
        numbombs += (arr_buttons[x - 1][y].type == 9);
        numbombs += (arr_buttons[x + 1][y].type == 9);
        numbombs += (arr_buttons[x][y + 1].type == 9);
        numbombs += (arr_buttons[x][y - 1].type == 9);
        
        // DIAGONAL CHECKING
        numbombs += (arr_buttons[x - 1][y - 1].type == 9);
        numbombs += (arr_buttons[x - 1][y + 1].type == 9);
        numbombs += (arr_buttons[x + 1][y - 1].type == 9);
        numbombs += (arr_buttons[x + 1][y + 1].type == 9);

        arr_buttons[x][y].type = numbombs - 1;
    }
}

void Setup_Board(int seed) {
    srand(seed);
    for (int i = 0; i < NUM_BOMBS; ++i) {
        int x = rand() % 16;
        int y = rand() % 14;
        arr_buttons[x][y].type = 9;
    }
    
    for (int y = 0; y < 14; ++y) {
        for (int x = 0; x < 16; ++x) {
            if (arr_buttons[x][y].type == 9) {
                Create_Number_Button(x - 1, y);
                Create_Number_Button(x + 1, y);
                Create_Number_Button(x, y + 1);
                Create_Number_Button(x, y - 1);
                
                Create_Number_Button(x - 1, y - 1);
                Create_Number_Button(x + 1, y + 1);
                Create_Number_Button(x - 1, y + 1);
                Create_Number_Button(x + 1, y - 1);

            }
        }

    }

}

void createthedog() {
    Raquet_SetPalette(pal_button, Raquet_GlobalPalette[0x12], Raquet_GlobalPalette[0x0D], Raquet_GlobalPalette[0x10], Raquet_GlobalPalette[0x30]);
    Raquet_SetPalette(pal_button_pressed, Raquet_GlobalPalette[0x12], Raquet_GlobalPalette[0x0D], Raquet_GlobalPalette[0x00], Raquet_GlobalPalette[0x10]);
    Raquet_SetPalette(pal_text_gameover, Raquet_GlobalPalette[0x0D], Raquet_GlobalPalette[0x30], Raquet_GlobalPalette[0x0D], Raquet_GlobalPalette[0x0D]);

    Raquet_LoadPPFBank(&ppf_main, Raquet_AbsoluteToAsset("main.ppf"));
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

    srand(time(NULL));
    for (int y = 0; y < 14; ++y) {
        for (int x = 0; x < 16; ++x) {
            arr_buttons[x][y].isFlagged = 0;
            arr_buttons[x][y].isPressed = 0;
            arr_buttons[x][y].isBeingPressed = 0;
            arr_buttons[x][y].type = 10;
            arr_buttons[x][y].x = x;
            arr_buttons[x][y].y = y;
        }
    }  
    Setup_Board(rand());
    
    for (int y = 0; y < 14; ++y) {
        for (int x = 0; x < 16; ++x) {
            arr_buttons[x][y].chr = &chr_buttons[arr_buttons[x][y].type];
        }
    }


    // LOAD CURSOR
    chr_cursor = Raquet_LoadCHR(ppf_main, 38, pal_cursor);
    act_cursor = Raquet_AllocateActor();
    Raquet_CreateActor(act_cursor, chr_cursor);

    // LOAD SMILEY
    int arr_smiley[4] = {39, 40, 55, 56};
    Palette pal_smiley[4] = {Raquet_GlobalPalette[0x21], Raquet_GlobalPalette[0x0D], Raquet_GlobalPalette[0x27], Raquet_GlobalPalette[0x20]};
    chr_smile = Raquet_LoadCHRMult(ppf_main, arr_smiley, 2, 2, pal_smiley);

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

void Draw_Button(int x, int y) {
    int finx = x + (15 * x);
    int finy = y + ((15 * y)) + 16;

    if (arr_buttons[x][y].isPressed) {
        if (arr_buttons[x][y].type == 10) {
            Raquet_SwapCHRPalette(&chr_buttons[10], pal_button_pressed);
            Raquet_PlaceCHR(chr_buttons[10], finx, finy);
            Raquet_SwapCHRPalette(&chr_buttons[10], pal_button);
        } else {
            Raquet_PlaceCHR(*arr_buttons[x][y].chr, finx, finy);
        }
    } else if (arr_buttons[x][y].isBeingPressed) {
        Raquet_SwapCHRPalette(&chr_buttons[10], pal_button_pressed);
        Raquet_PlaceCHR(chr_buttons[10], finx, finy);
        Raquet_SwapCHRPalette(&chr_buttons[10], pal_button);
    } else if (arr_buttons[x][y].isFlagged != 0) {
        Raquet_PlaceCHR(chr_buttons[8], finx, finy);
    } else {
        Raquet_PlaceCHR(chr_buttons[10], finx, finy);
    }
}

void Draw_Text(char* str, int x, int y, Palette pal[4]) {
    int o = 0;
    for (char* i = str; *i != '\0'; i++) {
        Raquet_CHR chr = Raquet_LoadCHR(ppf_main, *i, pal);
        Raquet_PlaceCHR(chr, x + (o * 8), y);
        ++o;
    }
}

void Game_GameOver() {
    Mouse_Main();

    for (int y = 0; y < 14; ++y) {
        for (int x = 0; x < 16; ++x) {
            Draw_Button(x, y);
        }
    }

    Draw_Text("GAME OVER", (SCREEN_WIDTH / 2 - 36), (SCREEN_HEIGHT / 2) - 4, pal_text_gameover);


    Raquet_PlaceCHR(chr_smile, (SCREEN_WIDTH / 2) - 8, 0);
    Raquet_DrawActor(act_cursor);
}

void Game_Running() {
    Raquet_Clear(Raquet_GlobalPalette[0x11]);
  
    Mouse_Main();

    for (int y = 0; y < 14; ++y) {
        for (int x = 0; x < 16; ++x) {
      
            if (Raquet_MouseCheck(RAQUET_MOUSE_LEFT) && act_cursor -> position.y > 15) {
                if (act_cursor -> position.x / 16 == x && ((act_cursor -> position.y / 16) - 1) == y) {
                    arr_buttons[x][y].isBeingPressed = 1;
                } else {
                    arr_buttons[x][y].isBeingPressed = 0;
                }
            } else {
                arr_buttons[x][y].isBeingPressed = 0;
            }
            
            if (Raquet_MouseCheck_Released(RAQUET_MOUSE_LEFT) && act_cursor -> position.y > 15) {
                if (act_cursor -> position.x / 16 == x && ((act_cursor -> position.y / 16) - 1) == y) {
                    arr_buttons[x][y].chr = &chr_buttons[arr_buttons[x][y].type];
                    if (arr_buttons[x][y].type == 10) {
                        Fill_Empty(x, y);
                    } else if (arr_buttons[x][y].type == 9) {
                        CUR_GAME_STATE = GAME_OVER;
                    }
                    arr_buttons[x][y].isPressed = 1;
                }       
            }

            if (Raquet_MouseCheck_Pressed(RAQUET_MOUSE_RIGHT) && act_cursor -> position.y > 15) {
                if (act_cursor -> position.x / 16 == x && ((act_cursor -> position.y / 16) - 1) == y) {
                    switch (arr_buttons[x][y].isFlagged) {
                        case 0:
                            arr_buttons[x][y].isFlagged = 1;
                            break;
                        
                        case 1:
                            arr_buttons[x][y].isFlagged = 0;
                            break;
                    }
                }
            }
            

            Draw_Button(x, y);
        }
    }
    
    Raquet_PlaceCHR(chr_smile, (SCREEN_WIDTH / 2) - 8, 0);
    Raquet_DrawActor(act_cursor);

}

void runthedog() {
    switch (CUR_GAME_STATE) {
        case RUNNING:
            Game_Running();
            break;
        case GAME_OVER:
            Game_GameOver();
            break;
    }
}

int main() {
    Raquet_Main();
    return 0;
}
