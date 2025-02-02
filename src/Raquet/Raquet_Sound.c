#include "Raquet_Sound.h"

Raquet_Sound Raquet_LoadSound(const char * file) {
    return Mix_LoadWAV(file);
}

void Raquet_PlaySound(Raquet_Sound wav, int loops, int channel) {
    Mix_PlayChannel(channel, wav, loops);
}

void Raquet_DestroySound(Raquet_Sound wav) {
    Mix_FreeChunk(wav);
}

void Raquet_StopChannel(int channel) {
    Mix_HaltChannel(channel);
}
