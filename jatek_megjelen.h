#ifndef JATEK_MEGJELEN_H_INCLUDED
#define JATEK_MEGJELEN_H_INCLUDED

typedef struct felb{
    int x, y;
}felb;

typedef struct palyaadat{
    int sor, oszlop, negyzetmeret, jatekos, eltx, elty, xlimit, rakszamol;
    felb fb;
} palyaadat;

typedef enum negyzet {
    ures, kor, iksz, ninit
} negyzet;

#include "fo_megjelen.h"
#include  <assert.h>

void jatek_kirajzol(SDL_Renderer *rende, palyaadat pd);
void jatek_init(SDL_Renderer *rende, negyzet ***r, palyaadat pd);
void jatek_betoltrajz(SDL_Renderer *rende, negyzet **r, palyaadat pd, negyzet jelen);
void szovegkiir(SDL_Renderer *rende, char const *szoveg, int betumeretarany, int x, int  y);
negyzet masik(negyzet jelen);
menu update_j(SDL_Renderer *rende, SDL_Event ev, negyzet** r, palyaadat *pd, negyzet *jelen);
menu nyeres(SDL_Renderer *rende, menu veg, negyzet jelen, const palyaadat *pd);

#endif // JATEK_MEGJELEN_H_INCLUDED
