#include "jatek_megjelen.h"
#include "benagep.h"

typedef struct vizszint{
    SDL_Renderer *rende;
    int x1, x2;
    szin sz;
} vizszint;

typedef struct fuggoleg{
    SDL_Renderer *rende;
    int y1, y2;
    szin sz;
} fuggoleg;

void vizszinthuz(vizszint vonal, int y){
    lineRGBA(vonal.rende, vonal.x1, y, vonal.x2, y, vonal.sz.r, vonal.sz.g, vonal.sz.b, vonal.sz.a);
}
void fuggohuz(fuggoleg vonal, int x){
    lineRGBA(vonal.rende, x, vonal.y1, x, vonal.y2, vonal.sz.r, vonal.sz.g, vonal.sz.b, vonal.sz.a);
}

void szovegkiir(SDL_Renderer *rende, char const *szoveg, int betumeretarany, int x, int  y){
    int felbx, felby;
    if(SDL_GetRendererOutputSize(rende, &felbx, &felby) != 0){
        SDL_Log("nem mukszik a felbontas kero: %s", SDL_GetError());
        exit(1);
    }
    TTF_Font *betu = TTF_OpenFont("font/segoeprb.ttf", felby / betumeretarany);
    if(betu == NULL){
        SDL_Log("Nincs font: %s", SDL_GetError());
        exit(1);
    }

    SDL_Log("x: %d  y: %d", x, y);
    SDL_Color fekete = {30, 30, 30};
    SDL_Surface *cim = TTF_RenderUTF8_Blended(betu, szoveg, fekete);
    SDL_Texture *cim_t = SDL_CreateTextureFromSurface(rende, cim);

    SDL_Rect tegla = {0, 0, 0, 0};

    tegla.h = cim->h;
    tegla.w = cim->w;
    tegla.x = x;
    tegla.y = y;
    SDL_RenderCopy(rende, cim_t, NULL, &tegla);

    SDL_FreeSurface(cim);
    SDL_DestroyTexture(cim_t);
    TTF_CloseFont(betu);
}

void jatek_init(SDL_Renderer *rende, negyzet ***r, palyaadat pd){ //Kezdeti értéket ad a pályának
    *r = (negyzet**)malloc(sizeof(negyzet*)*(pd.sor+8));
    assert(*r != 0);
    for(int i = 0; i < (pd.sor+8); i++){
        (*r)[i] = (negyzet*)malloc(sizeof(negyzet)*(pd.oszlop+8));
        assert((*r)[i] != 0);
    }

    for(int i = 0; i < pd.sor+8; i++){
        for(int j = 0; j < pd.oszlop+8; j++){
            (*r)[i][j] = ninit;

        }
    }
    for(int i = 4; i < pd.sor +4; i++){
        for(int j = 4; j < pd.oszlop+4; j++){
            (*r)[i][j] = ures;
        }
    }
    aacircleRGBA(rende, pd.fb.x-pd.fb.x/17, (pd.fb.y/40)*6, pd.fb.x/53, 0, 0, 0, 255);
}

void jatek_kirajzol(SDL_Renderer *rende, palyaadat pd){ //Egyszeri kirajzolás
    szin racs = {50, 50, 50, 130};
    vizszint v1 = {rende, pd.eltx, pd.eltx + pd.oszlop*pd.negyzetmeret, racs};
    fuggoleg v2 = {rende, pd.elty, pd.elty + pd.sor*pd.negyzetmeret, racs};
    //hatterbetolt(rende, NULL, "hatter4.png");
    boxRGBA(rende, 0, 0, pd.fb.x, pd.fb.y, 235, 235, 235, 255);

    for(int i = 0; i <= pd.sor; i++){
        vizszinthuz(v1, pd.negyzetmeret*i + pd.elty);
    }
    for(int i = 0; i <= pd.oszlop; i++)
        fuggohuz(v2, pd.negyzetmeret*i + pd.eltx);
    szovegkiir(rende, "A következő", 15, pd.fb.x - (pd.fb.x/80)*19, pd.fb.y/96);
    szovegkiir(rende, "játékos:", 15,  pd.fb.x - (pd.fb.x/80)*19, pd.fb.y/11);
}

void jatek_ujrarajzol(SDL_Renderer *rende, negyzet jelen, palyaadat pd, int oszlop, int sor){ //Katintásra történő változás a képernyőn - X és O kiírása, a jelenlegi játékos megváltoztatása
    int x1 = oszlop*pd.negyzetmeret + 4 + pd.eltx;
    int y1 = sor*pd.negyzetmeret + 4 + pd.elty;
    int x2 = x1 + pd.negyzetmeret - 8;
    int y2 = y1 + pd.negyzetmeret - 8;
    if(jelen == iksz){
        lineRGBA(rende, x1, y1, x2, y2, 50, 50, 50, 255);
        lineRGBA(rende, x1, y2, x2, y1, 50, 50, 50, 255);
        lineRGBA(rende, x1, y1-1, x2, y2-1, 50, 50, 50, 255);
        lineRGBA(rende, x1, y2-1, x2, y1-1, 50, 50, 50, 255);
        x1 = pd.fb.x-pd.fb.x/17 - pd.fb.x/53;
        x2 = pd.fb.x-pd.fb.x/17 + pd.fb.x/53;
        y1 = (pd.fb.y/40)*6 - pd.fb.x/53;
        y2 = (pd.fb.y/40)*6 + pd.fb.x/53;
        boxRGBA(rende, x1, y1, x2, y2, 235, 235, 235, 255);
        aacircleRGBA(rende, pd.fb.x-pd.fb.x/17, (pd.fb.y/40)*6, pd.fb.x/53, 0, 0, 0, 255);

    }
    else{
        aacircleRGBA(rende, (x1 + x2)/2, (y1 + y2)/2, pd.negyzetmeret/2 -3, 0, 0, 0, 255);
        x1 = pd.fb.x-pd.fb.x/17 - pd.fb.x/53;
        x2 = pd.fb.x-pd.fb.x/17 + pd.fb.x/53;
        y1 = (pd.fb.y/40)*6 - pd.fb.x/53;
        y2 = (pd.fb.y/40)*6 + pd.fb.x/53;
        boxRGBA(rende, x1, y1, x2, y2, 235, 235, 235, 255);
        thickLineRGBA(rende, x1, y1, x2, y2, 2, 0, 0, 0, 255);
        thickLineRGBA(rende, x1, y2, x2, y1, 2, 0, 0, 0, 255);
    }
}

void jatek_betoltrajz_seged(SDL_Renderer *rende, negyzet jelen, palyaadat pd, int s, int o){
    int x1 = o*pd.negyzetmeret + 4 + pd.eltx;
    int y1 = s*pd.negyzetmeret + 4 + pd.elty;
    int x2 = x1 + pd.negyzetmeret - 8;
    int y2 = y1 + pd.negyzetmeret - 8;
    if(jelen == iksz){
        lineRGBA(rende, x1, y1, x2, y2, 50, 50, 50, 255);
        lineRGBA(rende, x1, y2, x2, y1, 50, 50, 50, 255);
        lineRGBA(rende, x1, y1-1, x2, y2-1, 50, 50, 50, 255);
        lineRGBA(rende, x1, y2-1, x2, y1-1, 50, 50, 50, 255);
    }
    else{
        aacircleRGBA(rende, (x1 + x2)/2, (y1 + y2)/2, pd.negyzetmeret/2 -3, 0, 0, 0, 255);
    }
}

void jatek_betoltrajz(SDL_Renderer *rende, negyzet **r, palyaadat pd, negyzet jelen){
    for(int i = 4; i < pd.sor + 4; i++){
        for(int j = 4; j < pd.oszlop + 4; j++){
            if(r[i][j] != ures)
                jatek_betoltrajz_seged(rende, r[i][j], pd, i-4, j-4);
        }
    }
    int x1 = pd.fb.x-pd.fb.x/17 - pd.fb.x/53;
    int x2 = pd.fb.x-pd.fb.x/17 + pd.fb.x/53;
    int y1 = (pd.fb.y/40)*6 - pd.fb.x/53;
    int y2 = (pd.fb.y/40)*6 + pd.fb.x/53;
    if(jelen == iksz){
        thickLineRGBA(rende, x1, y1, x2, y2, 2, 0, 0, 0, 255);
        thickLineRGBA(rende, x1, y2, x2, y1, 2, 0, 0, 0, 255);
    }
    else
        aacircleRGBA(rende, pd.fb.x-pd.fb.x/17, (pd.fb.y/40)*6, pd.fb.x/53, 0, 0, 0, 255);
}

negyzet masik(negyzet jelen){
    switch(jelen){
    case iksz:
        return kor;
    case kor:
        return iksz;
    case ninit:
        return ninit;
    default:
        return ures;
    }

}

menu update_j(SDL_Renderer *rende, SDL_Event ev, negyzet **r, palyaadat *pd, negyzet *jelen){
    if(ev.window.event == SDL_WINDOWEVENT_FOCUS_GAINED){ //Alt+tab fix
        jatek_kirajzol(rende, *pd);
        for(int i = 4; i < pd->sor + 4; i++){
            for(int j = 4; j < pd->oszlop + 4; j++){
                if(r[i][j] != ures)
                    jatek_betoltrajz_seged(rende, r[i][j], *pd, i-4, j-4);
            }
        }
        SDL_RenderPresent(rende);
    }

    SDL_bool rakott = SDL_FALSE;
    int posx, posy;
    if((ev.key.keysym.sym == SDLK_ESCAPE) && (ev.type == SDL_KEYDOWN))
        return fo;
    if(ev.type == SDL_QUIT)
        return kilep;
    if(pd->jatekos == 1 && *jelen == iksz){
        gep_lep(r, pd); //A gép egérkatt eseményt küldd, mintha játékos lépte volna
        SDL_WaitEvent(&ev);
    }
    rakott = rakottt(ev, *pd); //Kiértékeli az eseményt
    if(rakott){
        posx = (ev.button.x-pd->eltx)/pd->negyzetmeret + 4; //A keretet kezelem
        posy = (ev.button.y-pd->elty)/pd->negyzetmeret + 4;
        SDL_Log("posx: %d  posy: %d", posx-3, posy-3);
        if(r[posy][posx] == ures){ //Nem rakhatok ugyanoda kétszer
            pd->rakszamol += 1;
            SDL_Log("Rakott: %d",pd->rakszamol);
            r[posy][posx] = *jelen;
        jatek_ujrarajzol(rende, *jelen, *pd, posx-4, posy-4);
        SDL_RenderPresent(rende);

        menu veg = nyerte(r, posx, posy, *jelen, pd);
        if(veg == nyert)
            return nyert;
        else
            *jelen = masik(*jelen);
        }

    }
    return jatek;
}

menu nyeres(SDL_Renderer *rende, menu veg, negyzet jelen, const palyaadat *pd){
    SDL_Event ev;
    SDL_Color fekete = {0, 0, 0, 255};
    if(pd->rakszamol == pd->sor*pd->oszlop){
        szovegkiirkozep(rende, "A meccs eredménye döntetlen lett!", 12, 10, 3, fekete, NULL, SDL_TRUE);
        SDL_RenderPresent(rende);
        while(ev.key.keysym.sym != SDLK_ESCAPE){
            SDL_WaitEvent(&ev);
        }
        return fo;
    }
    else if(jelen == iksz){
        szovegkiirkozep(rende, "Gratulálok az X játékosnak! Ő nyert!", 12, 10, 3, fekete, NULL, SDL_TRUE);
        SDL_RenderPresent(rende);
        while(ev.key.keysym.sym != SDLK_ESCAPE){
            SDL_WaitEvent(&ev);
        }
        return fo;
    }
    else{
        szovegkiirkozep(rende, "Gratulálok a kör játékosnak! Ő nyert!", 12, 10, 3, fekete, NULL, SDL_TRUE);
        SDL_RenderPresent(rende);
        while(ev.key.keysym.sym != SDLK_ESCAPE){
            SDL_WaitEvent(&ev);
        }
        return fo;
    }
}
