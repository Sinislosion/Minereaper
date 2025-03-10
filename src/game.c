#include <time.h>
#include "game.h"
#include "mouse.h"
#include "buttons.h"
#include "music.h"
#include "util.h"
#include "smiley.h"

GAME_STATE CUR_GAME_STATE = RUNNING;

PPF_Bank ppf_main;

int firstClick = 1;

Palette pal_text_gameover[4];

void Game_GameOver(void) {
    Mouse_Main();

    for (int y = 0; y < 14; ++y) {
        for (int x = 0; x < 16; ++x) {
            Draw_Button(x, y);
        }
    }

    Draw_Text("GAME OVER", (SCREEN_WIDTH / 2 - 36), (SCREEN_HEIGHT / 2) - 4, pal_text_gameover);

    Smiley_Main();
    Mouse_Draw();
}

void Game_YouWin(void) {
    Mouse_Main();

    for (int y = 0; y < 14; ++y) {
        for (int x = 0; x < 16; ++x) {
            Draw_Button(x, y);
        }
    }

    Draw_Text("YOU WIN", (SCREEN_WIDTH / 2 - 28), (SCREEN_HEIGHT / 2) - 4, pal_text_gameover);

    Smiley_Main();
    Mouse_Draw();
}

void Game_Running(void) {
    Raquet_Clear(Raquet_GlobalPalette[0x11]);

    Mouse_Main();

    int weWin = 1;

    for (int y = 0; y < NUM_ROWS; ++y) {
        for (int x = 0; x < NUM_COLUMNS; ++x) {

            if (arr_buttons[x][y].type < 8 && arr_buttons[x][y].isPressed == 0) {
                weWin = 0;
            }

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

                    if (firstClick > 0 && arr_buttons[x][y].type == 9) {
                        while (arr_buttons[x][y].type == 9) {
                            Setup_Board(rand());
                        }
                        firstClick = 0;
                    }
                    if (arr_buttons[x][y].type == 10) {
                        Fill_Empty(x, y);
                    } else if (arr_buttons[x][y].type == 9) {
                        Raquet_StopChannel(2);
                        CUR_GAME_STATE = GAME_OVER;
                    }
                    firstClick = 0;
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

    if (weWin > 0) {
        for (int y = 0; y < NUM_ROWS; ++y) {
            for (int x = 0; x < NUM_COLUMNS; ++x) {
                arr_buttons[x][y].isPressed = 1;
            }
        }
        Raquet_PlaySound(snd_win, 0, 0);
        Music_Stop();
        CUR_GAME_STATE = YOU_WIN;
    }

    Smiley_Main();
    Mouse_Draw();

}