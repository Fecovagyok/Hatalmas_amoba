#include <SDL.h>
#include <SDL_image.h>
#include <SDL2_gfxPrimitives.h>
#include <math.h>
#include <stdlib.h>
#include "fomenu.h"
#include "jatek.h"
#include <SDL_mixer.h>
#include "play.h"
#include "uj.h"

void menuhiv(menu *m, SDL_Renderer *rende, palyaadat *pd){
    switch(*m){
        case fo:
            *m = fomenu(rende);
            break;
        case uj:
            *m = uj_jatek(rende, pd);
            break;
        case jatek:
           *m = jatek_f(rende, SDL_FALSE, pd);
            break;
        case betolt:
            *m = jatek_f(rende, SDL_TRUE, pd);
            break;
        default:
            *m = kilep;
    }
}

int main(int argc, char *argv[]) {
    //zenel(); //A zene kiválasztására nem maradt idő, úgyhogy ezt kihagytam
    SDL_Window *window;
    SDL_Renderer *renderer;
    sdl_init("Amőba for Life", &window, &renderer);
    menu m = fo;
    palyaadat pd = {25, 25, 30, 2, 0, 0};
    zenel();
    if(SDL_GetRendererOutputSize(renderer, &(pd.fb.x), &(pd.fb.y)) != 0)
        SDL_Log("Nem tudtam lekerni a felbontast: %s", SDL_GetError());
    while(m != kilep){
        menuhiv(&m, renderer, &pd);
    }
    Mix_CloseAudio();
    TTF_Quit();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return 0;
}
