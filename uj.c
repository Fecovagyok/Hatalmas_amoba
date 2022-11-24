#include <SDL_image.h>
#include <SDL2_gfxPrimitives.h>
#include <math.h>
#include <stdlib.h>
#include <SDL_ttf.h>
#include "fo_megjelen.h"
#include "jatek_megjelen.h"
#include "esemenyhalo.h"
#include "uj_beolvas.h"

void ir(TTF_Font *b, SDL_Renderer *rende, const char *sz, felb fb, int nev, int szaml, feliratt *e){
    SDL_Color f = {255, 255, 255, 255};
    SDL_Surface *cim = TTF_RenderUTF8_Blended(b, sz, f);
    SDL_Texture *cim_t = SDL_CreateTextureFromSurface(rende, cim);

    SDL_Rect tegla = {0, 0, 0, 0};
    tegla.h = cim->h;
    tegla.w = cim->w;
    tegla.x = (fb.x/20)*7 - cim->w;
    tegla.y = (fb.y/nev)*szaml;;
    SDL_RenderCopy(rende, cim_t, NULL, &tegla);
    hszog h = {0, 0, 0, 0, 0, 0};

    SDL_FreeSurface(cim);
    if(e == NULL)
        SDL_DestroyTexture(cim_t);
    else{
        e->kep = cim_t;
        e->tegla = tegla;
        e->hsz = h;
    }
}

void nyilrajz(SDL_Renderer *rende, feliratt *e, felb fb){
    hszog h;
    h.x1 = e[1].tegla.x-fb.x/27;
    h.y1 = e[1].tegla.y + e[1].tegla.h/2 + fb.y/108;
    h.x2 = h.x3 = e[1].tegla.x-fb.x/77;
    h.y2 = e[1].tegla.y + fb.y/20;
    h.y3 = e[1].tegla.y+e[1].tegla.h -fb.y/31;

    SDL_Rect nulltegla = {0, 0, 0, 0};
    aatrigonRGBA(rende, h.x1, h.y1, h.x2, h.y2, h.x3, h.y3, 255, 255, 255, 255);
    aatrigonRGBA(rende, h.x1-1, h.y1, h.x2-1, h.y2, h.x3-1, h.y3, 255, 255, 255, 255);
    e[9].kep = NULL;
    e[9].tegla = nulltegla;
    e[9].hsz = h;

    h.x1 = e[1].tegla.x + e[1].tegla.w + fb.x/27;
    h.x2 = h.x3 = e[1].tegla.x + e[1].tegla.w + fb.x/77;
    e[6].kep = NULL;
    e[7].kep = NULL;
    aatrigonRGBA(rende, h.x1, h.y1, h.x2, h.y2, h.x3, h.y3, 255, 255, 255, 255);
    aatrigonRGBA(rende, h.x1+1, h.y1, h.x2+1, h.y2, h.x3+1, h.y3, 255, 255, 255, 255);
    e[10].kep = NULL;
    e[10].tegla = nulltegla;
    e[10].hsz = h;
}

void input_rajz(SDL_Renderer *rende, feliratt *e, felb fb){
    SDL_Rect t;
    t.y = (fb.y/20)*9 + fb.y/46;
    t.w = fb.x/16;
    t.x = fb.x/2 -t.w/2;
    t.h = e[3].tegla.h -fb.y/28;
    e[11].tegla = t;
    rectangleRGBA(rende, t.x, t.y, t.x +t.w, t.y + t.h, 255, 255, 255, 255);
    rectangleRGBA(rende, t.x+1, t.y+1, t.x +t.w+1, t.y + t.h+1, 255, 255, 255, 255);
    t.y = e[4].tegla.y + fb.y/46;
    rectangleRGBA(rende, t.x, t.y, t.x +t.w, t.y + t.h, 255, 255, 255, 255);
    rectangleRGBA(rende, t.x+1, t.y+1, t.x +t.w+1, t.y + t.h+1, 255, 255, 255, 255);
    e[12].tegla = t;
}

void uj_rajz(SDL_Renderer *rende, feliratt *e, gombadat *g, TTF_Font *betu, felb fb){ //Előző két menühöz hasonló SDL struktúrák és függvények
    SDL_Colour feher = {255, 255, 255, 255};
    szovegkiirkozep(rende, "Új játék beállításai", 9, 100, 3, feher, e+2, SDL_TRUE);
    szovegkiirkozep(rende, "2 játékos", 12, 30, 9, feher, e+1, SDL_TRUE);
    szovegkiirkozep(rende, "1 játékos", 12, 30, 9, feher, e+8, SDL_FALSE);
    nyilrajz(rende, e, fb);
    ir(betu, rende, "Sorok száma:", fb, 20, 9, e+3);
    ir(betu, rende, "Oszlopok száma:", fb, 20, 11, e+4);
    input_rajz(rende, e, fb);

    *g = szovegkiirkozep(rende, "Kezdés", 12, 10, 8, feher, e+5, SDL_TRUE);
    SDL_RenderPresent(rende);
}

void pd_ertekad(char **be, palyaadat *pd){
    if(be[0][1] == '\0')
        pd->sor = be[0][0] - '0';
    else
        pd->sor = (be[0][0] - '0')*10 + be[0][1] - '0';

    if(be[1][1] == '\0')
        pd->oszlop = be[1][0] - '0';
    else
        pd->oszlop = (be[1][0] - '0')*10 + be[1][1] - '0';

    if(pd->sor > 36 || pd->oszlop > 48){
        int sormin = pd->fb.y/pd->sor;
        int oszlopmin = (pd->xlimit)/pd->oszlop;
        pd->negyzetmeret = sormin < oszlopmin? sormin: oszlopmin;
    }
    else
        pd->negyzetmeret = pd->fb.x/64;
    pd->eltx = pd->xlimit/2 - (pd->oszlop*pd->negyzetmeret)/2;
    pd->elty = pd->fb.y/2 - (pd->sor*pd->negyzetmeret)/2;
    SDL_Log("jatekos: %d sor: %d oszlop: %d", pd->jatekos, pd->sor, pd->oszlop);
}

menu uj_jatek(SDL_Renderer *rende, palyaadat *pd){
    feliratt elemek[13];
    gombadat g;
    felb fb;
    SDL_Color feher = {255, 255, 255, 255};
    SDL_GetRendererOutputSize(rende, &(fb.x), &(fb.y));
    hatterbetolt(rende, elemek, "hatter2.png");
    TTF_Font *betu = TTF_OpenFont("font/segoeprb.ttf", fb.y / 14);
    uj_rajz(rende, elemek, &g, betu, fb);

    char be[2][2 + 1];
    be[0][0] = '\0';
    be[1][0] = '\0';
    char *asz[2] = {&be[0][0], &be[1][0]};
    textinput tinput;
    tinput.szin = feher;
    tinput.dest = asz;
    tinput.betu = betu;
    tinput.input = semmi;

    segedbool seged = {0, 0, 0};
    menu veg = uj;
    SDL_Event ev;

    while(veg == uj){
        SDL_WaitEvent(&ev);
        m_re_pause(ev);
        veg = update_uj(ev, rende, &tinput, elemek, &seged, &g);
    }
    pd->xlimit = pd->fb.x - (pd->fb.x/80)*19 -5;
    pd->jatekos = !(seged.solo) +1;
    SDL_Log("Jatekos: %d", pd->jatekos);
    pd_ertekad(asz, pd);

    for(int i = 0; i < 13; i++){
        SDL_DestroyTexture(elemek[i].kep);
    }
    TTF_CloseFont(betu);
    return veg;
}
