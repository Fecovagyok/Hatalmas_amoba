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

SDL_bool input_text(char *dest, size_t hossz, SDL_Rect teglalap, SDL_Color hatter, SDL_Color szoveg, TTF_Font *font, SDL_Renderer *renderer)
{
    /* Ez tartalmazza az aktualis szerkesztest */
    char composition[SDL_TEXTEDITINGEVENT_TEXT_SIZE];
    composition[0] = '\0';
    /* Ezt a kirajzolas kozben hasznaljuk */
    char textandcomposition[hossz + SDL_TEXTEDITINGEVENT_TEXT_SIZE + 1];
    /* Max hasznalhato szelesseg */
    int maxw = teglalap.w - 2;
    int maxh = teglalap.h - 2;

    dest[0] = '\0';

    SDL_bool enter = SDL_FALSE;
    SDL_bool kilep = SDL_FALSE;

    SDL_StartTextInput();
    while (!kilep && !enter)
    {
        /* doboz kirajzolasa */
        boxRGBA(renderer, teglalap.x, teglalap.y, teglalap.x + teglalap.w - 1, teglalap.y + teglalap.h - 1, hatter.r, hatter.g, hatter.b, 255);
        rectangleRGBA(renderer, teglalap.x, teglalap.y, teglalap.x + teglalap.w - 1, teglalap.y + teglalap.h - 1, szoveg.r, szoveg.g, szoveg.b, 255);
        /* szoveg kirajzolasa */
        int w;
        strcpy(textandcomposition, dest);
        strcat(textandcomposition, composition);
        if (textandcomposition[0] != '\0')
        {
            SDL_Surface *felirat = TTF_RenderUTF8_Blended(font, textandcomposition, szoveg);
            SDL_Texture *felirat_t = SDL_CreateTextureFromSurface(renderer, felirat);
            SDL_Rect cel = { teglalap.x, teglalap.y, felirat->w < maxw ? felirat->w : maxw, felirat->h < maxh ? felirat->h : maxh };
            SDL_RenderCopy(renderer, felirat_t, NULL, &cel);
            SDL_FreeSurface(felirat);
            SDL_DestroyTexture(felirat_t);
            w = cel.w;
        }
        else
        {
            w = 0;
        }
        /* kurzor kirajzolasa */
        if (w < maxw)
        {
            vlineRGBA(renderer, teglalap.x + w + 2, teglalap.y + 2, teglalap.y + teglalap.h - 3, szoveg.r, szoveg.g, szoveg.b, 192);
        }
        /* megjeleniti a képernyon az eddig rajzoltakat */
        SDL_RenderPresent(renderer);

        SDL_Event event;
        SDL_WaitEvent(&event);
        switch (event.type)
        {
        /* Kulonleges karakter */
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_BACKSPACE)
            {
                int textlen = strlen(dest);
                do
                {
                    if (textlen == 0)
                    {
                        break;
                    }
                    if ((dest[textlen-1] & 0x80) == 0x00)
                    {
                        /* Egy bajt */
                        dest[textlen-1] = 0x00;
                        break;
                    }
                    if ((dest[textlen-1] & 0xC0) == 0x80)
                    {
                        /* Bajt, egy tobb-bajtos szekvenciabol */
                        dest[textlen-1] = 0x00;
                        textlen--;
                    }
                    if ((dest[textlen-1] & 0xC0) == 0xC0)

                    {
                        /* Egy tobb-bajtos szekvencia elso bajtja */
                        dest[textlen-1] = 0x00;
                        break;
                    }
                }
                while(true);
            }
            if (event.key.keysym.sym == SDLK_RETURN)
            {
                enter = true;
            }
            break;

        /* A feldolgozott szoveg bemenete */
        case SDL_TEXTINPUT:
            if (strlen(dest) + strlen(event.text.text) < hossz)
            {
                strcat(dest, event.text.text);
            }

            /* Az eddigi szerkesztes torolheto */
            composition[0] = '\0';
            break;

        /* Szoveg szerkesztese */
        case SDL_TEXTEDITING:
            strcpy(composition, event.edit.text);
            break;

        case SDL_QUIT:
            /* visszatesszuk a sorba ezt az eventet, mert
             * sok mindent nem tudunk vele kezdeni */
            SDL_PushEvent(&event);
            kilep = true;
            break;
        }
    }

    /* igaz jelzi a helyes beolvasast; = ha enter miatt allt meg a ciklus */
    SDL_StopTextInput();
    return enter;
}
SDL_bool uj_input(char *dest, TTF_Font *font, SDL_Renderer *renderer, char* textandcomp, char* comp, int maxw, int maxh, SDL_Colour f, SDL_Rect t)
{
    /* doboz kirajzolasa */
    /* f kirajzolasa */
    int w;
    strcpy(textandcomp, dest);
    strcat(textandcomp, comp);
    if (textandcomp[0] != '\0')
    {
        SDL_Surface *felirat = TTF_RenderUTF8_Blended(font, textandcomp, f);
        SDL_Texture *felirat_t = SDL_CreateTextureFromSurface(renderer, felirat);
        SDL_Rect cel = { t.x, t.y, felirat->w < maxw ? felirat->w : maxw, felirat->h < maxh ? felirat->h : maxh };
        SDL_RenderCopy(renderer, felirat_t, NULL, &cel);
        SDL_FreeSurface(felirat);
        SDL_DestroyTexture(felirat_t);
        w = cel.w;
    }
    else
    {
        w = 0;
    }
    /* kurzor kirajzolasa */
    if (w < maxw)
    {
        vlineRGBA(renderer, t.x + w + 2, t.y + 2, t.y + t.h - 3, f.r, f.g, f.b, 192);
    }
    /* megjeleniti a képernyon az eddig rajzoltakat */
    SDL_RenderPresent(renderer);

    SDL_Event event;
    SDL_WaitEvent(&event);
    switch (event.type)
    {
    /* Kulonleges karakter */
    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_BACKSPACE)
        {
            int textlen = strlen(dest);
            do
            {
                if (textlen == 0)
                {
                    break;
                }
                if ((dest[textlen-1] & 0x80) == 0x00)
                {
                    /* Egy bajt */
                    dest[textlen-1] = 0x00;
                    break;
                }
                if ((dest[textlen-1] & 0xC0) == 0x80)
                {
                    /* Bajt, egy tobb-bajtos szekvenciabol */
                    dest[textlen-1] = 0x00;
                    textlen--;
                }
                if ((dest[textlen-1] & 0xC0) == 0xC0)
                {
                    /* Egy tobb-bajtos szekvencia elso bajtja */
                    dest[textlen-1] = 0x00;
                    break;
                }
            }
            while(true);
        }
        if (event.key.keysym.sym == SDLK_RETURN)
        {
            //enter = true;
        }
        break;

    /* A feldolgozott szoveg bemenete */
    case SDL_TEXTINPUT:
        if (strlen(dest) + strlen(event.text.text) < 100)
        {
            strcat(dest, event.text.text);
        }

        /* Az eddigi szerkesztes torolheto */
        comp[0] = '\0';
        break;

    /* Szoveg szerkesztese */
    case SDL_TEXTEDITING:
        strcpy(comp, event.edit.text);
        break;

    case SDL_QUIT:
        /* visszatesszuk a sorba ezt az eventet, mert
         * sok mindent nem tudunk vele kezdeni */
        SDL_PushEvent(&event);
        //kilep = true;
        break;
    }

    /* igaz jelzi a helyes beolvasast; = ha enter miatt allt meg a ciklus */
    return SDL_TRUE;
}

void refresh(SDL_Renderer *rende, feliratt *e, SDL_bool solo){
    SDL_RenderCopy(rende, e[0].kep, NULL, NULL);
    for(int i = solo + 1; i < solo + 8; i++){
        SDL_RenderCopy(rende, e[i].kep, NULL, &(e[i].tegla));
        //SDL_Log("%d: %s", i, SDL_GetError());
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

menu update_uj(SDL_Event ev, SDL_Renderer *rende, textinput *t, feliratt *e, segedbool *seged)
{
    SDL_bool folehuz[4] = {0, 0, 0, 0};
    switch(ev.type)
    {
    case SDL_MOUSEMOTION:
        if(!(seged->kattvolt)){
            folehuz[0] = e[9].hsz.x1 < ev.motion.x && e[9].hsz.x2 > ev.motion.x && e[9].hsz.y2 < ev.motion.y && ev.motion.y < e[9].hsz.y3;
            folehuz[1] = e[10].hsz.x1 > ev.motion.x && e[10].hsz.x2 < ev.motion.x && e[10].hsz.y2 < ev.motion.y && ev.motion.y < e[10].hsz.y3;
            if(!(seged->folottvolt)){
                if(folehuz[0]){
                    filledTrigonRGBA(rende, e[9].hsz.x1, e[9].hsz.y1, e[9].hsz.x2, e[9].hsz.y2, e[9].hsz.x3, e[9].hsz.y3, 255, 255, 255, 70);
                    SDL_RenderPresent(rende);
                    seged->folottvolt = SDL_TRUE;
                }
                if(folehuz[1]){
                    filledTrigonRGBA(rende, e[10].hsz.x1, e[10].hsz.y1, e[10].hsz.x2, e[10].hsz.y2, e[10].hsz.x3, e[10].hsz.y3, 255, 255, 255, 70);
                    SDL_RenderPresent(rende);
                    seged->folottvolt = SDL_TRUE;
                }
            }
            else if(!folehuz[0] && !folehuz[1]){
                refresh(rende, e, seged->solo);
                SDL_RenderPresent(rende);
                //SDL_Log("kattvolt: %d", seged->kattvolt);
                seged->folottvolt = SDL_FALSE;
            }
        }
        break;

    case SDL_MOUSEBUTTONDOWN:
        if(ev.button.button == SDL_BUTTON_LEFT){
            folehuz[0] = e[9].hsz.x1 < ev.button.x && e[9].hsz.x2 > ev.button.x && e[9].hsz.y2 < ev.button.y && ev.button.y < e[9].hsz.y3;
            folehuz[1] = e[10].hsz.x1 > ev.button.x && e[10].hsz.x2 < ev.button.x && e[10].hsz.y2 < ev.button.y && ev.button.y < e[10].hsz.y3;
            folehuz[2] = e[11].tegla.x < ev.button.x && e[11].tegla.x + e[11].tegla.w > ev.button.x && e[11].tegla.y < ev.button.y && e[11].tegla.y + e[11].tegla.h > ev.button.y;
            folehuz[3] = e[12].tegla.x < ev.button.x && e[12].tegla.x + e[12].tegla.w > ev.button.x && e[12].tegla.y < ev.button.y && e[12].tegla.y + e[12].tegla.h > ev.button.y;
            if(folehuz[0]){
                seged->kattvolt = SDL_TRUE;
                seged->solo = !(seged->solo);
                refresh(rende, e, seged->solo);
                filledTrigonRGBA(rende, e[9].hsz.x1, e[9].hsz.y1, e[9].hsz.x2, e[9].hsz.y2, e[9].hsz.x3, e[9].hsz.y3, 255, 255, 255, 150);
                SDL_RenderPresent(rende);
            }
            else if(folehuz[1]){
                seged->kattvolt = SDL_TRUE;
                seged->solo = !(seged->solo);
                refresh(rende, e, seged->solo);
                filledTrigonRGBA(rende, e[10].hsz.x1, e[10].hsz.y1, e[10].hsz.x2, e[10].hsz.y2, e[10].hsz.x3, e[10].hsz.y3, 255, 255, 255, 150);
                SDL_RenderPresent(rende);
            }
            else if(folehuz[2]){
                if(t->input == semmi)SDL_StartTextInput();
                t->input = sor;
                SDL_Log("textandcomp: %c", t->textandcomp[0]);
            }
            else if(folehuz[3]){
                if(t->input == semmi)SDL_StartTextInput();
                t->input = oszlop;
                SDL_Log("oszlop, %s", SDL_GetError());
            }
            if(!folehuz[2] && !folehuz[3]){
                t->input = semmi;
                SDL_Log("semmi, %s", SDL_GetError());
                SDL_StopTextInput();
                refresh(rende, e, seged->solo);
                SDL_RenderPresent(rende);
            }
        }
        break;

    case SDL_MOUSEBUTTONUP:
        if(ev.button.button == SDL_BUTTON_LEFT && seged->kattvolt){
            folehuz[0] = e[9].hsz.x1 < ev.button.x && e[9].hsz.x2 > ev.button.x && e[9].hsz.y2 < ev.button.y && ev.button.y < e[9].hsz.y3;
            folehuz[1] = e[10].hsz.x1 > ev.button.x && e[10].hsz.x2 < ev.button.x && e[10].hsz.y2 < ev.button.y && ev.button.y < e[10].hsz.y3;
            refresh(rende, e, seged->solo);
            if(seged->folottvolt){
                if(folehuz[0]){
                    filledTrigonRGBA(rende, e[9].hsz.x1, e[9].hsz.y1, e[9].hsz.x2, e[9].hsz.y2, e[9].hsz.x3, e[9].hsz.y3, 255, 255, 255, 70);
                }
                if(folehuz[1]){
                    filledTrigonRGBA(rende, e[10].hsz.x1, e[10].hsz.y1, e[10].hsz.x2, e[10].hsz.y2, e[10].hsz.x3, e[10].hsz.y3, 255, 255, 255, 70);
                }
            }
            SDL_RenderPresent(rende);
            seged->kattvolt = SDL_FALSE;
            break;
        }
    case SDL_KEYDOWN:
        if(t->input != semmi){
            SDL_DestroyTexture(e[t->input-5].kep);
            e[t->input -5].kep = NULL;
            if (ev.key.keysym.sym == SDLK_BACKSPACE){
                int textlen = strlen(t->dest[t->input-11]);
                do{
                    if (textlen == 0){
                    break;
                    }
                    if ((t->dest[t->input-11][textlen-1] & 0x80) == 0x00){
                        /* Egy bajt */
                        t->dest[t->input-11][textlen-1] = 0x00;

                        break;
                    }
                    /*if ((t->dest[t->input-11][textlen-1] & 0xC0) == 0x80){
                        /* Bajt, egy tobb-bajtos szekvenciabol *//*
                        t->dest[t->input-11][textlen-1] = 0x00;
                        textlen--;
                    }
                    if ((t->dest[t->input-11][textlen-1] & 0xC0) == 0xC0){
                        /* Egy tobb-bajtos szekvencia elso bajtja *//*
                        t->dest[t->input-11][textlen-1] = 0x00;

                        break;
                    }*/
                }
                while(true);
            }
            break;
        }
    case SDL_TEXTINPUT:
    if(t->input != semmi){
            SDL_Log("dest: %d", ev.text.text[0]);
            if ((ev.text.text[0] >= '0') && (ev.text.text[0] <= '9') && (strlen(t->dest[t->input-11]) < 2)){
                strcat(t->dest[t->input-11], ev.text.text);
            }

            /* Az eddigi szerkesztes torolheto */
            t->comp[0] = '\0';
        }
        break;
    /* Szoveg szerkesztese */
    /*case SDL_TEXTEDITING:
        if(t->input != semmi){
            SDL_Log("Editing: %s", ev.edit.text);
            strcpy(t->comp, ev.edit.text);
        }*/
    }
    if(t->input != semmi){
        int w;
        //strcpy(t->textandcomp, t->dest[t->input-11]);
       // strcat(t->textandcomp, t->comp);
        if (t->dest[t->input -11][0] != '\0'){
            SDL_Surface *felirat = TTF_RenderUTF8_Blended(t->betu, t->dest[t->input-11], t->sz);
            SDL_Texture *felirat_t = SDL_CreateTextureFromSurface(rende, felirat);
            SDL_Rect cel = { e[t->input].tegla.x + 2, e[t->input].tegla.y -18, felirat->w < t->maxw ? felirat->w : t->maxw, felirat->h/*felirat->h < t->maxh ? felirat->h : t->maxh*/ };
            SDL_RenderCopy(rende, felirat_t, NULL, &cel);
            SDL_FreeSurface(felirat);
            SDL_DestroyTexture(e[t->input-5].kep);
            e[t->input-5].kep = felirat_t;
            e[t->input-5].tegla = cel;
            w = cel.w;
            refresh(rende, e, seged->solo);
            vlineRGBA(rende, e[t->input].tegla.x + w + 2, e[t->input].tegla.y + 2, e[t->input].tegla.y + e[t->input].tegla.h - 3, 255, 255, 255, 192);
            SDL_RenderPresent(rende);
        }
        else if(ev.key.keysym.sym == SDLK_BACKSPACE || ev.button.button == SDL_BUTTON_LEFT){
            w = 2;
            refresh(rende, e, seged->solo);
            vlineRGBA(rende, e[t->input].tegla.x + w + 2, e[t->input].tegla.y + 2, e[t->input].tegla.y + e[t->input].tegla.h - 3, 255, 255, 255, 192);
            SDL_RenderPresent(rende);

        }
        /* kurzor kirajzolasa */
        //if (w < t->maxw){
            //vlineRGBA(rende, e[t->input].tegla.x + w + 2, e[t->input].tegla.y + 2, e[t->input].tegla.y + e[t->input].tegla.h - 3, 255, 255, 255, 255);
        //}
        /* megjeleniti a képernyon az eddig rajzoltakat */
        //SDL_RenderPresent(rende);
    }
    return fo;
}
