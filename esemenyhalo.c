#include "esemenyhalo.h"

eger egermove(SDL_Event ev, gombadat* g){
    eger e;
    e.folott = SDL_FALSE;
    e.katt = SDL_FALSE;
    e.fol = SDL_FALSE;
    e.gomb = 0;
    switch(ev.type){
    case SDL_MOUSEBUTTONDOWN:
        for(int i = 0; i < 3; i++){
            if(ev.button.button == SDL_BUTTON_LEFT && ev.button.x > g[i].x1 && ev.button.x < g[i].x2 && ev.button.y > g[i].y1 && ev.button.y < g[i].y2){
                e.katt = SDL_TRUE;
                e.gomb = i;
                return e;
            }
        }
        return e;
    case SDL_MOUSEMOTION:
        for(int i = 0; i < 3; i++){
            if(ev.motion.x > g[i].x1 && ev.motion.x < g[i].x2 && ev.motion.y > g[i].y1 && ev.motion.y < g[i].y2){
                e.folott = SDL_TRUE;
                e.gomb = i;
                return e;
            }
        }
        return e;
    case SDL_MOUSEBUTTONUP:
        e.fol = SDL_TRUE;
        for(int i = 0; i < 3; i++){
            if(ev.button.button == SDL_BUTTON_LEFT && ev.button.x > g[i].x1 && ev.button.x < g[i].x2 && ev.button.y > g[i].y1 && ev.button.y < g[i].y2){
                e.gomb = i;
                e.folott = SDL_TRUE;
            }
        }
    }
    return e;
}

SDL_bool rakottt(SDL_Event ev, palyaadat pd){

    if(ev.type == SDL_MOUSEBUTTONDOWN && ev.button.button == SDL_BUTTON_LEFT){
        if(ev.button.x < pd.oszlop*pd.negyzetmeret + pd.eltx && ev.button.x > pd.eltx && ev.button.y < pd.sor*pd.negyzetmeret + pd.elty && ev.button.y > pd.elty){
            return SDL_TRUE;
        }
    }
    return SDL_FALSE;
}

void m_re_pause(SDL_Event ev){
    if(ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_m){
        if(Mix_PausedMusic() == 1){
            Mix_ResumeMusic();
            SDL_Log("Zene resume");
        }
        else{
            Mix_PauseMusic();
            SDL_Log("Zene pause");
        }
    }
}


