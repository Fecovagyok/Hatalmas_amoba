#include <SDL.h>
#include <SDL_image.h>
#include <SDL2_gfxPrimitives.h>
#include <math.h>
#include <stdlib.h>
#include "fo_megjelen.h"
#include "play.h"
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "fomenu.h"
#include "esemenyhalo.h"

menu fomenu(SDL_Renderer *rende){
    feliratt elemek[5]; //fo_megjelen.h-ban deklarálva - képi elemeket - texturákat és alakzatadatokat tartalmaz, hogy azokat könnyedén újra tudjam tölteni

    hatterbetolt(rende, elemek, "hatter2.png"); //Hátteret a képernyőre és textúráját az elemek tömbbe elmenti
    gombadat *g = NULL; //fo_megjelen.h-ban van deklarálva kerekített téglalap formájú gombot adatait tartlamazza
    g = fomenu_rajz(rende, g, elemek); //A feliratokat kirajzolja, a gombadatoknak értéket ad és az elemek tömböt feltölti
    SDL_Event ev;
    SDL_bool ravittem = SDL_FALSE; //Az eventek kezelésénél hasznos (segedváltozó)
    SDL_bool lent = SDL_FALSE;
    menu kilepvagykov = fo; //menu nevű enummal kezelem azt hogy melyik menübe dobjon a menühív függvény, szintén ezt használom arra, hogy megálljanak a cilusok
    while(kilepvagykov == fo){
        SDL_WaitEvent(&ev);
        m_re_pause(ev);
        kilepvagykov = update_f(ev, rende, g, &ravittem, elemek, &lent); //A menügombok animációit vezérli visszatérési értéke meghatározza a következõ menüt
    }
    free(g);
    for(int i = 0; i < 5; i++)
        SDL_DestroyTexture(elemek[i].kep); //Az eltárolt textúrákat természetesen kiürítem a memóriából
    return kilepvagykov;
}
