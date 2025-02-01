#include "Raquet.h"
#include "game.h"
#include "util.h"

void Draw_Text(char* str, int x, int y, Palette pal[4]) {
    int o = 0;
    for (char* i = str; *i != '\0'; i++) {
        Raquet_CHR chr = Raquet_LoadCHR(ppf_main, *i, pal);
        Raquet_PlaceCHR(chr, x + (o * 8), y);
        ++o;
    }
}