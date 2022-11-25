#include <SDL.h>
#include <SDL_image.h>
#include <SDL2_gfxPrimitives.h>
#include <math.h>
#include <stdlib.h>
#include <SDL_mixer.h>

void zenel(void){
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1)
    {
        SDL_Log("Nem sikerült audio-t nyitni: %s", Mix_GetError());
    }
    Mix_Music *zene = Mix_LoadMUS("audio/tomor.mp3");
    if(zene == NULL){
        SDL_Log("Nem sikerult megnyitni az mp3-mat");
        return;
    }
    Mix_PlayMusic(zene, 1);
    Mix_VolumeMusic(100);

}
