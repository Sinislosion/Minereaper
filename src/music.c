#include "music.h"

Raquet_Sound Music_Songs[NUM_SONGS];

void Music_Init() {
    Music_Songs[0] = Raquet_LoadSound(Raquet_AbsoluteToAsset("mus_ieatbombs.mp3"));
    Music_Songs[1] = Raquet_LoadSound(Raquet_AbsoluteToAsset("mus_chip01.mp3"));
}

void Music_Play() {
    Raquet_StopChannel(2);
    int track = rand() % NUM_SONGS;
    Raquet_PlaySound(Music_Songs[track], -1, 2);
}

void Music_Stop() {
    Raquet_StopChannel(2);
}