#ifndef UJ_BEOLVAS_H_INCLUDED
#define UJ_BEOLVAS_H_INCLUDED
#include <SDL_ttf.h>
#include "fo_megjelen.h"

typedef enum inputm{semmi = 0, sor = 11, oszlop = 12} inputm;

typedef struct segedbool{
    SDL_bool folottvolt, kattvolt, solo;
} segedbool;

typedef struct textinput{
    char **dest;
    SDL_Color szin;
    TTF_Font *betu;
    inputm input;
} textinput;

menu update_uj(SDL_Event ev, SDL_Renderer *rende, textinput *t, feliratt *e, segedbool *seged, gombadat *g);
#endif // UJ_BEOLVAS_H_INCLUDED
