#include "benagep.h"

SDL_bool vegejobb(int j, palyaadat *pd){
    if(j-3 == pd->oszlop)
        return SDL_TRUE;
    else
        return SDL_FALSE;
}

SDL_bool vegebal(int j){
    if(j-4 == 0)
        return SDL_TRUE;
    else
        return SDL_FALSE;
}

SDL_bool vegefol(int i){
    if(i-4 == 0)
        return SDL_TRUE;
    else
        return SDL_FALSE;
}

SDL_bool vegele(int i, palyaadat *pd){
    if(i-3 == pd->oszlop)
        return SDL_TRUE;
    else
        return SDL_FALSE;
}

typedef struct xy{ //koordináták
    int x, y;
} xy;

typedef struct veg{ //egy sorozat végének adatai
    SDL_bool szabad;
    xy koord;
    int hanyszabad, hanyszovets;
} veg;

typedef enum irany {vizs = 1, fuggo = 2, atlobf = 3, atlojf = 4, barmi = 0} irany;

typedef struct allas{ //Egy elem struktúrája a listában
    int szam;
    irany iran;
    veg veg1, veg2;
} allas;

typedef struct allaslist{ //Láncolt lista
    allas sorallas;
    struct allaslist *kov;
} allaslist;

typedef struct lista{ //A fenti láncolt listának az első és utolsó eleme, hogy könnyen be leheseen szúrni
    allaslist *eleje, *vege;
} lista;

void list_init(lista *l){
    l->eleje = malloc(sizeof(allaslist));
    l->eleje->sorallas.iran = barmi;
    l->eleje->sorallas.szam = -1;
    l->eleje->kov = NULL;
    l->vege = l->eleje;
}

void list_fuz(lista *l, allas adat){
    allaslist *uj = malloc(sizeof(allaslist));
    uj->kov = NULL;
    uj->sorallas = adat;
    l->vege->kov = uj;
    l->vege = uj;
}

void free_lista(allaslist *l){
    allaslist *mozgo;
    allaslist *lemarad = l;
    for(mozgo = l->kov; mozgo != NULL; mozgo = mozgo->kov){
        free(lemarad);
        lemarad = mozgo;
    }
    free(lemarad);
}

SDL_bool mindegyik_egy(allas *adatok){
    for(int i = 1; i < 5; i++){
        if(adatok[i].szam != 1)
            return SDL_FALSE;
    }
    return SDL_TRUE;
}

SDL_bool osszes_veg1_foglalt(allas *adatok, int *kiszabad){// sorozat = 1 esetén
    for(int i = 1; i < 5; i++){
        if(adatok[i].veg1.szabad){
            *kiszabad = i;
            return SDL_FALSE;
        }
    }
    return SDL_TRUE;
}

SDL_bool osszes_veg2_foglalt(allas *adatok, int *kiszabad){ //sorozaat = 1 eseténn
    for(int i = 1; i < 5; i++){
        if(adatok[i].veg2.szabad){
            *kiszabad = i;
            return SDL_FALSE;
        }
    }
    return SDL_TRUE;
}

void sokfor_vizsbal(nyeradat n, allas *a){ // számolja egy megadott ponttól
    int irszamol = 1;
    for(int i = 1; i < 5; i++){
        if(n.r[n.y][n.x-i] == n.jelen)
            irszamol += 1;
        else
            break;
    }
    a->iran = vizs;
    a->szam += irszamol-1;
    a->veg1.koord.x = n.x - irszamol;
    a->veg1.koord.y = n.y;
    for(int i = irszamol; i < 5; i++){
        if(n.r[n.y][n.x-i] == ures)
            a->veg1.hanyszabad += 1;
        else
            break;
    }
    if(a->veg1.hanyszabad > 0)
        a->veg1.szabad = SDL_TRUE;
    else
        a->veg1.szabad = SDL_FALSE;
}

void sokfor_vizsjobb(nyeradat n, allas *a){
    int irszamol = 1;
    for(int i = 1; i < 5; i++){
        if(n.r[n.y][n.x+i] == n.jelen)
            irszamol += 1;
        else
            break;
    }
    a->szam += irszamol;
    a->veg2.koord.x = n.x + irszamol;
    a->veg2.koord.y = n.y;
    for(int i = irszamol; i < 5; i++){
        if(n.r[n.y][n.x+i] == ures)
            a->veg2.hanyszabad += 1;
        else
            break;
    }
    if(a->veg2.hanyszabad > 0)
        a->veg2.szabad = SDL_TRUE;
    else
        a->veg2.szabad = SDL_FALSE;
}

void sokfor_fuggfol(nyeradat n, allas *a){
    int irszamol = 1;
    for(int i = 1; i < 5; i++){
        if(n.r[n.y-i][n.x] == n.jelen)
            irszamol += 1;
        else
            break;
    }
    a->iran = fuggo;
    a->szam += irszamol-1;
    a->veg1.koord.x = n.x;
    a->veg1.koord.y = n.y - irszamol;
    for(int i = irszamol; i < 5; i++){
        if(n.r[n.y-i][n.x] == ures)
            a->veg1.hanyszabad += 1;
        else
            break;
    }
    if(a->veg1.hanyszabad > 0)
        a->veg1.szabad = SDL_TRUE;
    else
        a->veg1.szabad = SDL_FALSE;
}

void sokfor_fuggle(nyeradat n, allas *a){
    int irszamol = 1;
    for(int i = 1; i < 5; i++){
        if(n.r[n.y+i][n.x] == n.jelen)
            irszamol += 1;
        else
            break;
    }
    a->szam += irszamol;
    a->veg2.koord.x = n.x;
    a->veg2.koord.y = n.y + irszamol;
    for(int i = irszamol; i < 5; i++){
        if(n.r[n.y+i][n.x] == ures)
            a->veg2.hanyszabad += 1;
        else
            break;
    }
    if(a->veg2.hanyszabad > 0)
        a->veg2.szabad = SDL_TRUE;
    else
        a->veg2.szabad = SDL_FALSE;
}

void sokfor_atlobalfel(nyeradat n, allas *a){
    int irszamol = 1;
    for(int i = 1; i < 5; i++){
        if(n.r[n.y-i][n.x-i] == n.jelen)
            irszamol += 1;
        else
            break;
    }
    a->iran = atlobf;
    a->szam += irszamol -1;
    a->veg1.koord.x = n.x - irszamol;
    a->veg1.koord.y = n.y - irszamol;
    for(int i = irszamol; i < 5; i++){
        if(n.r[n.y-i][n.x-i] == ures)
            a->veg1.hanyszabad += 1;
        else
            break;
    }
    if(a->veg1.hanyszabad > 0)
        a->veg1.szabad = SDL_TRUE;
    else
        a->veg1.szabad = SDL_FALSE;
}

void sokfor_atlojobble(nyeradat n, allas *a){
    int irszamol = 1;
    for(int i = 1; i < 5; i++){
        if(n.r[n.y+i][n.x+i] == n.jelen)
            irszamol += 1;
        else
            break;
    }
    a->szam += irszamol;
    a->veg2.koord.x = n.x + irszamol;
    a->veg2.koord.y = n.y + irszamol;
    for(int i = irszamol; i < 5; i++){
        if(n.r[n.y+i][n.x+i] == ures)
            a->veg2.hanyszabad += 1;
        else
            break;
    }
    if(a->veg2.hanyszabad > 0)
        a->veg2.szabad = SDL_TRUE;
    else
        a->veg2.szabad = SDL_FALSE;
}

void sokfor_atlojobbfel(nyeradat n, allas *a){
    int irszamol = 1;
    for(int i = 1; i < 5; i++){
        if(n.r[n.y-i][n.x+i] == n.jelen)
            irszamol += 1;
        else
            break;
    }
    a->iran = atlojf;
    a->szam += irszamol -1;
    a->veg1.koord.x = n.x + irszamol;
    a->veg1.koord.y = n.y - irszamol;
    for(int i = irszamol; i < 5; i++){
        if(n.r[n.y-i][n.x+i] == ures)
            a->veg1.hanyszabad += 1;
        else
            break;
    }
    if(a->veg1.hanyszabad > 0)
        a->veg1.szabad = SDL_TRUE;
    else
        a->veg1.szabad = SDL_FALSE;
}

void sokfor_atloballe(nyeradat n, allas *a){
    int irszamol = 1;
    for(int i = 1; i < 5; i++){
        if(n.r[n.y+i][n.x-i] == n.jelen)
            irszamol += 1;
        else
            break;
    }
    a->szam += irszamol;
    a->veg2.koord.x = n.x - irszamol;
    a->veg2.koord.y = n.y + irszamol;
    for(int i = irszamol; i < 5; i++){
        if(n.r[n.y+i][n.x-i] == ures)
            a->veg2.hanyszabad += 1;
        else
            break;
    }
    if(a->veg2.hanyszabad > 0)
        a->veg2.szabad = SDL_TRUE;
    else
        a->veg2.szabad = SDL_FALSE;
}

void adatok_init(allas *adatok){
    adatok[1].szam = 0;
    adatok[1].veg1.hanyszabad = 0;
    adatok[1].veg2.hanyszabad = 0;
    adatok[2].veg1.hanyszabad = 0;
    adatok[2].veg2.hanyszabad = 0;
    adatok[2].szam = 0;
    adatok[3].veg1.hanyszabad = 0;
    adatok[3].veg2.hanyszabad = 0;
    adatok[3].szam = 0;
    adatok[4].szam = 0;
    adatok[4].veg1.hanyszabad = 0;
    adatok[4].veg2.hanyszabad = 0;
}

void struktura_1_elem_eseten(allas *adatok){
    SDL_Log("MINDEGYIK EGY");
    adatok[0].iran = barmi;
    adatok[0].szam = 1;
    int melyik, melyik2;
    adatok[0].veg1.szabad = !osszes_veg1_foglalt(adatok, &melyik);
    adatok[0].veg2.szabad = !osszes_veg2_foglalt(adatok, &melyik2);
    if(adatok[0].veg1.szabad){
        adatok[0].veg1.koord = adatok[melyik].veg1.koord;
        SDL_Log("Van  EGY szabad");
    }
    if(adatok[0].veg2.szabad){
        adatok[0].veg2.koord = adatok[melyik2].veg2.koord;
        SDL_Log("Van  ketto szabad");
    }
}

void sorozatszamol(negyzet **r, int x, int y, negyzet jelen, lista *lis){
    allas adatok[5];
    adatok_init(adatok);

    nyeradat n;
    n.jelen = jelen;
    n.x = x;
    n.y = y;
    n.r = r;

    sokfor_vizsbal(n, adatok + 1); /*vízszintes*/
    sokfor_vizsjobb(n, adatok + 1);

    sokfor_fuggfol(n, adatok + 2); /*függõleges*/
    sokfor_fuggle(n, adatok + 2);

    sokfor_atlobalfel(n, adatok + 3); /*atló fölülrõl*/
    sokfor_atlojobble(n, adatok +3);

    sokfor_atlojobbfel(n, adatok + 4); /*atlo alulról*/
    sokfor_atloballe(n, adatok + 4);
    for(int i = 1; i < 5; i++){
        SDL_Log("sorozat: %d x: %d y: %d", adatok[i].szam, adatok[i].veg1.koord.x, adatok[i].veg1.koord.y);
    }
    if(mindegyik_egy(adatok)){
        struktura_1_elem_eseten(adatok);
        list_fuz(lis, adatok[0]);
    }
    else{
        for(int i = 1; i < 5; i++){
            list_fuz(lis, adatok[i]);
        }
    }
}

void elsorak(SDL_Event *ev, palyaadat *pd, negyzet **r){
    for(int i = 4; i < pd->sor + 4; i++){
        for(int j = 4; j < pd->oszlop + 4; j++){
            if(r[i][j] == kor){
                if(!vegejobb(j, pd)){
                    ev->button.x = pd->eltx + (j-3)*pd->negyzetmeret + 2;
                    ev->button.y = pd->elty + (i-4)*pd->negyzetmeret +2;
                }
                else if(!vegebal(j)){
                    ev->button.x = pd->eltx + (j-5)*pd->negyzetmeret + 2;
                    ev->button.y = pd->elty + (i-4)*pd->negyzetmeret +2;
                }
            }
        }
    }
}

void befejez(allaslist *jatekos, allaslist *gep, SDL_Event *ev){
    free_lista(jatekos);
    free_lista(gep);

    SDL_PushEvent(ev);
}

void brutal_struktura_feltolt(palyaadat *pd, negyzet **r, lista *jatekos, lista *gep){
    for(int i = 4; i < pd->sor +4; i++){
        for(int j = 4; j < pd->oszlop +4; j++){
            if(r[i][j] == kor){
                sorozatszamol(r, j, i, kor, jatekos);
                SDL_Log("jatekost szamolok");
            }
            if(r[i][j] == iksz){
                SDL_Log("gepet szamolok");
                sorozatszamol(r, j, i, iksz, gep);

            }
        }
    }
}

int neki_negyes(palyaadat *pd, lista *gep, lista *jatekos, SDL_Event *ev){
    allaslist *mozgo;
    for(mozgo = gep->eleje->kov; mozgo != NULL; mozgo = mozgo->kov){
        if(mozgo->sorallas.szam == 4){
            if(mozgo->sorallas.veg1.szabad){
                ev->button.x = pd->eltx + (mozgo->sorallas.veg1.koord.x-4)*pd->negyzetmeret +2;
                ev->button.y = pd->elty + (mozgo->sorallas.veg1.koord.y-4)*pd->negyzetmeret +2;
                befejez(jatekos->eleje, gep->eleje, ev);
                return 0;
            }
            if(mozgo->sorallas.veg2.szabad){
                ev->button.x = pd->eltx + (mozgo->sorallas.veg2.koord.x-4)*pd->negyzetmeret +2;
                ev->button.y = pd->elty + (mozgo->sorallas.veg2.koord.y-4)*pd->negyzetmeret +2;
                befejez(jatekos->eleje, gep->eleje, ev);
                return 0;
            }
        }
    }
    return 1;
}

int neki_harmas(palyaadat *pd, lista *gep, lista *jatekos, SDL_Event *ev){
    allaslist *mozgo;
    for(mozgo = gep->eleje->kov; mozgo != NULL; mozgo = mozgo->kov){
        if(mozgo->sorallas.szam == 3){
            if(mozgo->sorallas.veg1.hanyszabad > 1 && mozgo->sorallas.veg2.szabad){
                ev->button.x = pd->eltx + (mozgo->sorallas.veg1.koord.x-4)*pd->negyzetmeret +2;
                ev->button.y = pd->elty + (mozgo->sorallas.veg1.koord.y-4)*pd->negyzetmeret +2;
                befejez(jatekos->eleje, gep->eleje, ev);
                return 0;
            }
            if(mozgo->sorallas.veg2.hanyszabad > 1 && mozgo->sorallas.veg1.szabad){
                ev->button.x = pd->eltx + (mozgo->sorallas.veg2.koord.x-4)*pd->negyzetmeret +2;
                ev->button.y = pd->elty + (mozgo->sorallas.veg2.koord.y-4)*pd->negyzetmeret +2;
                befejez(jatekos->eleje, gep->eleje, ev);
                return 0;
            }
        }
    }
    return 1;
}

int gepnek_2(palyaadat *pd, lista *gep, lista *jatekos, SDL_Event *ev){
    allaslist *mozgo;
    for(mozgo = gep->eleje->kov; mozgo != NULL; mozgo = mozgo->kov){
        if(mozgo->sorallas.szam == 2){
            if(mozgo->sorallas.veg1.szabad){
                ev->button.x = pd->eltx + (mozgo->sorallas.veg1.koord.x-4)*pd->negyzetmeret +2;
                ev->button.y = pd->elty + (mozgo->sorallas.veg1.koord.y-4)*pd->negyzetmeret +2;
                befejez(jatekos->eleje, gep->eleje, ev);
                return 0;
            }
            if(mozgo->sorallas.veg2.szabad){
                ev->button.x = pd->eltx + (mozgo->sorallas.veg2.koord.x-4)*pd->negyzetmeret +2;
                ev->button.y = pd->elty + (mozgo->sorallas.veg2.koord.y-4)*pd->negyzetmeret +2;
                befejez(jatekos->eleje, gep->eleje, ev);
                return 0;
            }
        }
    }
    return 1;
}

int gepnek_1_vagy_beszorult(palyaadat *pd, lista *gep, lista *jatekos, SDL_Event *ev, negyzet **r){
    allaslist *mozgo;
    for(mozgo = gep->eleje->kov; mozgo != NULL; mozgo = mozgo->kov){
        if(mozgo->sorallas.szam == 1){
            if(mozgo->sorallas.veg1.szabad){
                SDL_Log("sorozat:%d  x: %d, y: %d", mozgo->sorallas.szam, mozgo->sorallas.veg1.koord.x -3, mozgo->sorallas.veg1.koord.y -3);
                ev->button.x = pd->eltx + (mozgo->sorallas.veg1.koord.x-4)*pd->negyzetmeret +2;
                ev->button.y = pd->elty + (mozgo->sorallas.veg1.koord.y-4)*pd->negyzetmeret +2;
                befejez(jatekos->eleje, gep->eleje, ev);
                return 0;
            }
            if(mozgo->sorallas.veg2.szabad){
                ev->button.x = pd->eltx + (mozgo->sorallas.veg2.koord.x-4)*pd->negyzetmeret +2;
                ev->button.y = pd->elty + (mozgo->sorallas.veg2.koord.y-4)*pd->negyzetmeret +2;
                SDL_Log("sorozat:%d  x: %d, y: %d", mozgo->sorallas.szam, mozgo->sorallas.veg1.koord.x -3, mozgo->sorallas.veg1.koord.y -3);
                befejez(jatekos->eleje, gep->eleje, ev);
                return 0;
            }
            for(int i = 4; i < pd->sor + 4; i++){
                for(int j = 4; j < pd->oszlop + 4; j++){
                    if(r[i][j] == ures){
                        ev->button.x = pd->eltx + (j-3)*pd->negyzetmeret + 2;
                        ev->button.y = pd->elty + (i-4)*pd->negyzetmeret +2;
                        befejez(jatekos->eleje, gep->eleje, ev);
                        return 0;
                    }
                }
            }
        }
    }
    return 1;
}

void gep_lep(negyzet **r, palyaadat *pd){
    SDL_Event ev;
    ev.type = SDL_MOUSEBUTTONDOWN;
    ev.button.button = SDL_BUTTON_LEFT;
    if(pd->rakszamol == 1){
        elsorak(&ev, pd, r);
        SDL_PushEvent(&ev);
        return;
    }
    lista jatekos;
    lista gep;
    list_init(&jatekos);
    list_init(&gep);

    brutal_struktura_feltolt(pd, r, &jatekos, &gep); // A struktúrarendserzből könnyű kiolvasni, hogy milyen egymásutánjai vannak a két játékosnak különkölün

    if(neki_negyes(pd, &gep, &jatekos, &ev) == 0)
        return;
    if(neki_negyes(pd, &jatekos, &gep, &ev) == 0)
        return;
    if(neki_harmas(pd, &gep, &jatekos, &ev) == 0)
        return;
    if(neki_harmas(pd, &jatekos, &gep, &ev) == 0)
        return;
    if(gepnek_2(pd, &gep, &jatekos, &ev) == 0)
        return;
    if(gepnek_1_vagy_beszorult(pd, &gep, &jatekos, &ev, r) == 0)
        return;
    befejez(jatekos.eleje, gep.eleje, &ev);
}
