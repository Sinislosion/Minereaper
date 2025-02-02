#include "buttons.h"
#include "game.h"

Button arr_buttons[16][15];
Raquet_CHR chr_buttons[NUM_BUTTON_TYPES];

Palette pal_button[4];
Palette pal_button_pressed[4];

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

    if (x - 1 >= 0 && y + 1 < NUM_ROWS && arr_buttons[x - 1][y + 1].type != 10) {
        arr_buttons[x - 1][y + 1].isPressed = 1;
    }

    if (y + 1 < NUM_ROWS && arr_buttons[x][y + 1].type != 10) {
        arr_buttons[x][y + 1].isPressed = 1;
    }

    if (x + 1 < NUM_COLUMNS && y + 1 < NUM_ROWS && arr_buttons[x + 1][y + 1].type != 10) {
        arr_buttons[x + 1][y + 1].isPressed = 1;
    }

    if (x + 1 < NUM_COLUMNS && arr_buttons[x + 1][y].type != 10) {
        arr_buttons[x + 1][y].isPressed = 1;
    }

    if (x + 1 < NUM_COLUMNS && y - 1 >= 0 && arr_buttons[x + 1][y - 1].type != 10) {
        arr_buttons[x + 1][y - 1].isPressed = 1;
    }

    if (y - 1 >= 0 && arr_buttons[x][y - 1].type != 10) {
        arr_buttons[x][y - 1].isPressed = 1;
    }

    if (x - 1 >= 0 && y - 1 >= 0 && arr_buttons[x - 1][y - 1].type != 10) {
        arr_buttons[x - 1][y - 1].isPressed = 1;
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

void Setup_Board(int seed) {

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

    for (int y = 0; y < 14; ++y) {
        for (int x = 0; x < 16; ++x) {
            arr_buttons[x][y].chr = &chr_buttons[arr_buttons[x][y].type];
        }
    }

}