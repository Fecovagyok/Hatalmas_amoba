#ifndef JATEK_H_INCLUDED
#define JATEK_H_INCLUDED

#include "jatek_megjelen.h"

typedef struct nyeradat{
    int *irany;
    negyzet **r;
    int x, y;
    negyzet jelen;
} nyeradat;

menu jatek_f(SDL_Renderer *rende, SDL_bool betol, palyaadat *pd);
menu nyerte(negyzet **r, int x, int y, negyzet jelen, const palyaadat *pd);


#endif // JATEK_H_INCLUDED
