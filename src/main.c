#include "Raquet.h"
PPF_Bank ppf_main;
Raquet_CHR chr_buttons[11];

void createthedog() {
    Raquet_LoadPPFBank(&ppf_main, Raquet_AbsoluteToAsset("main.ppf"));
    Palette pal_button[4] = {Raquet_GlobalPalette[0x12], Raquet_GlobalPalette[0x0D], Raquet_GlobalPalette[0x10], Raquet_GlobalPalette[0x30]};
    for (int i = 0; i < 11; ++i) {
        i *= 2;
        int arr[4] = {0, 1, 16, 17};
        chr_buttons[i] = Raquet_LoadCHRMult(ppf_main, arr, 2, 2, pal_button);
        i /= 2;
    }
}

void runthedog() {
    Raquet_Clear(Raquet_GlobalPalette[0x10]);
    Raquet_PlaceCHR(chr_buttons[0], 0, 0);
}

int main() {
    Raquet_Main();
    return 0;
}