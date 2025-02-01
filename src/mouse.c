#include "game.h"
#include "mouse.h"

Raquet_CHR chr_cursor;
Raquet_Actor* act_cursor;

void Mouse_Init(void) {
    Palette pal_cursor[4] = {Raquet_GlobalPalette[0x0F], Raquet_GlobalPalette[0x0D], Raquet_GlobalPalette[0x10], Raquet_GlobalPalette[0x30]};
    chr_cursor = Raquet_LoadCHR(ppf_main, 38, pal_cursor);
    act_cursor = Raquet_AllocateActor();
    Raquet_CreateActor(act_cursor, chr_cursor);
    act_cursor -> bbox.x2 = 1;
    act_cursor -> bbox.y2 = 1;
}

void Mouse_Main(void) {

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

void Mouse_Draw(void) {
    Raquet_DrawActor(act_cursor);
}