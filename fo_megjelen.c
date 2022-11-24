#include <SDL.h>
#include <SDL_image.h>
#include <SDL2_gfxPrimitives.h>
#include <math.h>
#include <stdlib.h>
#include "fo_megjelen.h"
#include <SDL_ttf.h>
#include "esemenyhalo.h"
#include <assert.h>

void sdl_init(char const *felirat, SDL_Window **pwindow, SDL_Renderer **prenderer) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }
    SDL_Window *window = SDL_CreateWindow(felirat, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1366, 768, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (window == NULL) {
        SDL_Log("Nem hozhato letre az ablak: %s", SDL_GetError());
        exit(1);
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL) {
        SDL_Log("Nem hozhato letre a megjelenito: %s", SDL_GetError());
        exit(1);
    }
    SDL_RenderClear(renderer);

    *pwindow = window;
    *prenderer = renderer;
}

void hatterbetolt(SDL_Renderer *rende, feliratt *hatter, char *fajl){
    SDL_Texture *hatte = IMG_LoadTexture(rende, fajl);
    if(hatte == NULL){
        SDL_Log("Nem nyithato meg a hatter: %s", IMG_GetError());
        exit(1);
    }
    if(hatter != NULL){
        hatter->kep = hatte;
    }
    SDL_RenderCopy(rende, hatte, NULL, NULL);

}

int kerekitett_tegla(gombadat g){
    return roundedRectangleRGBA(g.rende, g.x1, g.y1, g.x2, g.y2, g.gombsug, g.r, g.g, g.b, g.a);
}

int kerekitett_doboz(gombadat g){
    return roundedBoxRGBA(g.rende, g.x1, g.y1, g.x2, g.y2, g.gombsug-4, g.r, g.g, g.b, 130);
}

gombadat szovegkiirkozep(SDL_Renderer *rende, char* szoveg, int betumeretarany, int nevezo, int szamlalo, SDL_Color sz, feliratt *e, SDL_bool irjam){ // Ez az én mindenes függvényem. vízszintesen középre igazítja a szöveget, y pozÍcióját a felbontással arányosan adja meg, betölti a szöveg textúráját egy változóba (ha kérem tőle) és kiírja a képernyőre (ha kérem tőle), visszatérési értéke egy gombadat, ha gombot íratok vele akkor betolthetem a változómba
    int felbx, felby;
    if(SDL_GetRendererOutputSize(rende, &felbx, &felby) != 0){
        SDL_Log("nem mukszik a felbontas kero: %s", SDL_GetError());
        exit(1);
    }
    TTF_Font *betu = TTF_OpenFont("font/segoeprb.ttf", felby / betumeretarany); //Magassággal arányos szöveg
    if(betu == NULL)
        SDL_Log("Nincs font: %s", SDL_GetError());

    SDL_Surface *cim = TTF_RenderUTF8_Blended(betu, szoveg, sz);
    SDL_Texture *cim_t = SDL_CreateTextureFromSurface(rende, cim);

    SDL_Rect tegla = {0, 0, 0, 0};
    tegla.h = cim->h;
    tegla.w = cim->w;
    tegla.x = felbx/2-(cim->w)/2; //Ezzel vízszintesen középre igazítottam
    tegla.y = (szamlalo*felby)/nevezo;//felby/helymeretarany; //A felbontással arányosan teszi a címet a helyére
    if(irjam)
        SDL_RenderCopy(rende, cim_t, NULL, &tegla); //Ha csak a gombadatot vagy a textúrát akartam berakni a helyére, ez a függvény erre is képes
    hszog h = {0, 0, 0, 0, 0, 0};

    gombadat gomb = {rende, tegla.x-20, tegla.y+10, tegla.x + tegla.w+20, tegla.y + tegla.h-10, tegla.h/2-5, sz.r, sz.g, sz.b, sz.a};

    SDL_FreeSurface(cim);
    if(e == NULL)
        SDL_DestroyTexture(cim_t);
    else{
        e->kep = cim_t; //beölti az elemek struktúrába
        e->tegla = tegla;
        e->hsz = h;
    }
    TTF_CloseFont(betu);
    return gomb;
}

void fomenu_rajz_nemg(SDL_Renderer *rende, feliratt *e){
    SDL_RenderCopy(rende, e[0].kep, NULL, NULL);
    for(int i = 1; i < 5; i++)
        SDL_RenderCopy(rende, e[i].kep, NULL, &(e[i].tegla));
    SDL_RenderPresent(rende);
}
gombadat* fomenu_rajz(SDL_Renderer *rende, gombadat *g, feliratt *e){
    g = (gombadat*)malloc(sizeof(gombadat)*3);
    TTF_Init();
    SDL_Color feher = {255, 255, 255, 255};
    szovegkiirkozep(rende, "Amőba for Life", 7, 400, 1, feher, e+1, SDL_TRUE);
    g[0] = szovegkiirkozep(rende, "Új játék", 12, 20, 9, feher, e+2, SDL_TRUE);
    g[1] = szovegkiirkozep(rende, "Játék folytatása", 12, 20, 12, feher, e+3, SDL_TRUE);
    g[2] = szovegkiirkozep(rende, "Kilépés", 12, 20, 15, feher, e+4, SDL_TRUE);

    SDL_Log("Uj jatek: %d, %d  : %s",g[0].x1, g[0].x2, SDL_GetError());
    SDL_RenderPresent(rende);

    return g;
}

menu update_f(SDL_Event ev, SDL_Renderer *rende, gombadat * g, SDL_bool *ravittem, feliratt *elem, SDL_bool *lent){
    eger e;
    if(ev.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) //Ez egy alt+tab fix, a teljes képernyős változathoz
        fomenu_rajz_nemg(rende, elem); //Nemg rajz függvény az eltárolt textúrákat kirakja megint a képernyőre (mint egy refresh gomb)
    if(ev.type == SDL_QUIT || (ev.key.keysym.sym == SDLK_ESCAPE && ev.type == SDL_KEYDOWN))
        return kilep;
    e = egermove(ev, g); //Ez egy eseménykiértékelő függvény ami megmondja, hogyha kattintok akkor melyik gombra kattintottam
    if(e.katt){
        fomenu_rajz_nemg(rende, elem);
        kerekitett_doboz(g[e.gomb]);
        SDL_RenderPresent(rende);
        *lent = SDL_TRUE;
    }
    else {
        if(e.fol){
            if(e.folott){
                if(e.gomb == 2) return kilep;
                if(e.gomb == 1) return betolt;
                if(e.gomb == 0) return uj;
            }
            fomenu_rajz_nemg(rende, elem);
            *lent = SDL_FALSE;
        }
        if(e.folott && !(*lent)){
            kerekitett_tegla(g[e.gomb]);
            SDL_RenderPresent(rende);
            *ravittem = SDL_TRUE;
        }
        else if(*ravittem  && !(*lent)){
            fomenu_rajz_nemg(rende, elem);
            *ravittem = SDL_FALSE;
        }
    }
    return fo;

}
