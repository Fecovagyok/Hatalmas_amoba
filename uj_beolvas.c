#include <SDL_image.h>
#include <SDL2_gfxPrimitives.h>
#include <math.h>
#include <stdlib.h>
#include <SDL_ttf.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "uj_beolvas.h"
#include "fo_megjelen.h"
#include "jatek_megjelen.h"

void refresh(SDL_Renderer *rende, feliratt *e, SDL_bool solo){ //Az előző rajz_nemg-hez hasonló
    SDL_RenderCopy(rende, e[0].kep, NULL, NULL);
    for(int i = solo + 1; i < solo + 8; i++){
        SDL_RenderCopy(rende, e[i].kep, NULL, &(e[i].tegla));
    }
    for(int i = 9; i <= 10; i++){
        aatrigonRGBA(rende, e[i].hsz.x1, e[i].hsz.y1, e[i].hsz.x2, e[i].hsz.y2, e[i].hsz.x3, e[i].hsz.y3, 255, 255, 255, 255);
        aatrigonRGBA(rende, e[i].hsz.x1-1, e[i].hsz.y1, e[i].hsz.x2-1, e[i].hsz.y2, e[i].hsz.x3-1, e[i].hsz.y3, 255, 255, 255, 255);
    }
    for(int i = 11; i <= 12; i++){
        rectangleRGBA(rende, e[i].tegla.x, e[i].tegla.y, e[i].tegla.x + e[i].tegla.w, e[i].tegla.y + e[i].tegla.h, 255, 255, 255, 255);
        rectangleRGBA(rende, e[i].tegla.x + 1, e[i].tegla.y + 1, e[i].tegla.x + e[i].tegla.w + 1, e[i].tegla.y + e[i].tegla.h + 1, 255, 255, 255, 255);
    }
}

void egermotion(SDL_Event ev, SDL_Renderer *rende, segedbool *seged, gombadat *g, SDL_bool *folehuz, feliratt *e){
    if(!(seged->kattvolt)){
            folehuz[0] = e[9].hsz.x1 < ev.motion.x && e[9].hsz.x2 > ev.motion.x && e[9].hsz.y2 < ev.motion.y && ev.motion.y < e[9].hsz.y3;
            folehuz[1] = e[10].hsz.x1 > ev.motion.x && e[10].hsz.x2 < ev.motion.x && e[10].hsz.y2 < ev.motion.y && ev.motion.y < e[10].hsz.y3;
            folehuz[4] = e[5].tegla.x < ev.motion.x && e[5].tegla.x + e[5].tegla.w > ev.motion.x && ev.motion.y > e[5].tegla.y && e[5].tegla.y + e[5].tegla.h > ev.motion.y;
            if(!(seged->folottvolt)){
                if(folehuz[0]){
                    filledTrigonRGBA(rende, e[9].hsz.x1, e[9].hsz.y1, e[9].hsz.x2, e[9].hsz.y2, e[9].hsz.x3, e[9].hsz.y3, 255, 255, 255, 70);
                    SDL_RenderPresent(rende);
                    seged->folottvolt = SDL_TRUE;
                }
                else if(folehuz[1]){
                    filledTrigonRGBA(rende, e[10].hsz.x1, e[10].hsz.y1, e[10].hsz.x2, e[10].hsz.y2, e[10].hsz.x3, e[10].hsz.y3, 255, 255, 255, 70);
                    SDL_RenderPresent(rende);
                    seged->folottvolt = SDL_TRUE;
                }
                else if(folehuz[4]){
                    kerekitett_tegla(*g);
                    SDL_RenderPresent(rende);
                    seged->folottvolt = SDL_TRUE;
                }
            }
            else if(!folehuz[0] && !folehuz[1] && !folehuz[4]){
                refresh(rende, e, seged->solo);
                SDL_RenderPresent(rende);
                seged->folottvolt = SDL_FALSE;
            }
        }
}

void egerbalkatt(SDL_Event ev, SDL_Renderer *rende, segedbool *seged, gombadat *g, SDL_bool *folehuz, feliratt *e, textinput *t){
        if(ev.button.button == SDL_BUTTON_LEFT){
            if(folehuz[0] = (e[9].hsz.x1 < ev.button.x && e[9].hsz.x2 > ev.button.x && e[9].hsz.y2 < ev.button.y && ev.button.y < e[9].hsz.y3)){
                seged->kattvolt = SDL_TRUE;
                seged->solo = !(seged->solo);
                refresh(rende, e, seged->solo);
                filledTrigonRGBA(rende, e[9].hsz.x1, e[9].hsz.y1, e[9].hsz.x2, e[9].hsz.y2, e[9].hsz.x3, e[9].hsz.y3, 255, 255, 255, 150);
                SDL_RenderPresent(rende);
            }
            else if(folehuz[1] = (e[10].hsz.x1 > ev.button.x && e[10].hsz.x2 < ev.button.x && e[10].hsz.y2 < ev.button.y && ev.button.y < e[10].hsz.y3)){
                seged->kattvolt = SDL_TRUE;
                seged->solo = !(seged->solo);
                refresh(rende, e, seged->solo);
                filledTrigonRGBA(rende, e[10].hsz.x1, e[10].hsz.y1, e[10].hsz.x2, e[10].hsz.y2, e[10].hsz.x3, e[10].hsz.y3, 255, 255, 255, 150);
                SDL_RenderPresent(rende);
            }
            else if(folehuz[2] = (e[11].tegla.x < ev.button.x && e[11].tegla.x + e[11].tegla.w > ev.button.x && e[11].tegla.y < ev.button.y && e[11].tegla.y + e[11].tegla.h > ev.button.y)){
                if(t->input == semmi)SDL_StartTextInput();
                t->input = sor;
            }
            else if(folehuz[3] = e[12].tegla.x < ev.button.x && e[12].tegla.x + e[12].tegla.w > ev.button.x && e[12].tegla.y < ev.button.y && e[12].tegla.y + e[12].tegla.h > ev.button.y){
                if(t->input == semmi)SDL_StartTextInput();
                t->input = oszlop;
                SDL_Log("oszlop, %s", SDL_GetError());
            }
            else if(folehuz[4] = e[5].tegla.x < ev.button.x && e[5].tegla.x + e[5].tegla.w > ev.button.x && ev.button.y > e[5].tegla.y && e[5].tegla.y + e[5].tegla.h > ev.button.y){
                seged->kattvolt = SDL_TRUE;
                kerekitett_doboz(*g);
                SDL_RenderPresent(rende);
            }
            if((!folehuz[2] && !folehuz[3] && t->input != semmi)){
                t->input = semmi;
                SDL_Log("semmi, %s", SDL_GetError());
                SDL_StopTextInput();
                refresh(rende, e, seged->solo);
                SDL_RenderPresent(rende);
            }
        }
}

menu egerbalkattfel(SDL_Event ev, SDL_Renderer *rende, segedbool *seged, gombadat *g, SDL_bool *folehuz, feliratt *e, textinput *t){
        if(ev.button.button == SDL_BUTTON_LEFT && seged->kattvolt){
            refresh(rende, e, seged->solo);
            if(seged->folottvolt){
                if(folehuz[0] = (e[9].hsz.x1 < ev.button.x && e[9].hsz.x2 > ev.button.x && e[9].hsz.y2 < ev.button.y && ev.button.y < e[9].hsz.y3)){
                    filledTrigonRGBA(rende, e[9].hsz.x1, e[9].hsz.y1, e[9].hsz.x2, e[9].hsz.y2, e[9].hsz.x3, e[9].hsz.y3, 255, 255, 255, 60);
                }
                else if(folehuz[1] = (e[10].hsz.x1 > ev.button.x && e[10].hsz.x2 < ev.button.x && e[10].hsz.y2 < ev.button.y && ev.button.y < e[10].hsz.y3)){
                    filledTrigonRGBA(rende, e[10].hsz.x1, e[10].hsz.y1, e[10].hsz.x2, e[10].hsz.y2, e[10].hsz.x3, e[10].hsz.y3, 255, 255, 255, 60);
                }
                else if(folehuz[4] = e[5].tegla.x < ev.button.x && e[5].tegla.x + e[5].tegla.w > ev.button.x && ev.button.y > e[5].tegla.y && e[5].tegla.y + e[5].tegla.h > ev.button.y){
                    int so, osz;
                    if(t->dest[0][1] == '\0')
                        so = t->dest[0][0] - '0';
                    else
                        so = (t->dest[0][0] - '0')*10 + t->dest[0][1] -'0';
                    if(t->dest[1][1] == '\0')
                        osz = t->dest[1][0] - '0';
                    else
                        osz = (t->dest[1][0] - '0')*10 + t->dest[1][1] - '0';
                    if(osz < 5 || so < 5){
                        SDL_Color piros = {255, 40, 40};
                        szovegkiirkozep(rende, "A megadott adat nem lehet 5-nél kisebb!", 13, 40, 7, piros, NULL, SDL_TRUE);
                    }
                    else if((so > 43) || (osz > 60)){
                        SDL_Color piros = {255, 40, 40};
                        szovegkiirkozep(rende, "A megadott adotok túl nagyok! (max 60 oszlop és 43 sor)", 17, 40, 8, piros, NULL, SDL_TRUE);
                    }
                    else
                        return jatek;
                }
            }
            SDL_RenderPresent(rende);
            seged->kattvolt = SDL_FALSE;
        }
        return uj;
}

void inputtorol(SDL_Event ev, textinput *t, feliratt *e){
    if(t->input != semmi){
        SDL_DestroyTexture(e[t->input-5].kep);
        e[t->input -5].kep = NULL;
        if (ev.key.keysym.sym == SDLK_BACKSPACE){
            int textlen = strlen(t->dest[t->input-11]);
            t->dest[t->input-11][textlen-1] = 0x00;
        }
    }
}

void input_kiir_es_egyeb(SDL_Renderer *rende, SDL_Event ev, textinput *t, feliratt *e, SDL_bool *folehuz, segedbool *seged){
    if(t->input != semmi){
        if (t->dest[t->input -11][0] != '\0' && !folehuz[4]){
            SDL_Surface *felirat = TTF_RenderUTF8_Blended(t->betu, t->dest[t->input-11], t->szin);
            SDL_Texture *felirat_t = SDL_CreateTextureFromSurface(rende, felirat);
            SDL_Rect cel = { e[t->input].tegla.x + 2, e[t->input].tegla.y -18, felirat->w, felirat->h};
            SDL_RenderCopy(rende, felirat_t, NULL, &cel);
            SDL_FreeSurface(felirat);
            SDL_DestroyTexture(e[t->input-5].kep);
            e[t->input-5].kep = felirat_t;
            e[t->input-5].tegla = cel;
            refresh(rende, e, seged->solo);
            vlineRGBA(rende, e[t->input].tegla.x + cel.w + 2, e[t->input].tegla.y + 2, e[t->input].tegla.y + e[t->input].tegla.h - 3, 255, 255, 255, 192);
            SDL_RenderPresent(rende);
        }
        else if(ev.key.keysym.sym == SDLK_BACKSPACE || (ev.button.button == SDL_BUTTON_LEFT && !folehuz[1] && !folehuz[2] && !folehuz[4])){
            refresh(rende, e, seged->solo);
            vlineRGBA(rende, e[t->input].tegla.x + 4, e[t->input].tegla.y + 2, e[t->input].tegla.y + e[t->input].tegla.h - 3, 255, 255, 255, 192);
            SDL_RenderPresent(rende);
        }
    }
}

menu update_uj(SDL_Event ev, SDL_Renderer *rende, textinput *t, feliratt *e, segedbool *seged, gombadat *g){
    SDL_bool folehuz[5] = {0, 0, 0, 0, 0};
    if(ev.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) refresh(rende, e, seged->solo);
    switch(ev.type)
    {
    case SDL_MOUSEMOTION:
        egermotion(ev, rende, seged, g, folehuz, e);
        break;

    case SDL_MOUSEBUTTONDOWN:
        egerbalkatt(ev, rende, seged, g, folehuz, e, t);
        break;

    case SDL_MOUSEBUTTONUP:
        if(egerbalkattfel(ev, rende, seged, g, folehuz, e, t) == jatek)
            return jatek;
        break;

    case SDL_KEYDOWN:
        if(ev.key.keysym.sym == SDLK_ESCAPE)
            return fo;
        inputtorol(ev, t, e);
        break;
    case SDL_TEXTINPUT:
        if(t->input != semmi){
            if ((ev.text.text[0] >= '0') && (ev.text.text[0] <= '9') && (strlen(t->dest[t->input-11]) < 2)){ //Csak egy bájtos szekvenciák lehetségesek, mert csak számjegyeket enged az input
                strcat(t->dest[t->input-11], ev.text.text);
            }
        }
    }
    input_kiir_es_egyeb(rende, ev, t, e, folehuz, seged);
    return uj;
}
