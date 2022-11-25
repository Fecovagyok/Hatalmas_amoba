#include "jatek.h"
#include "palya_manage.h"
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#include <fileapi.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

void directoryCheck(char* dir_name){
#ifdef _WIN32
    if(CreateDirectoryA(dir_name, NULL) ||GetLastError() == ERROR_ALREADY_EXISTS){
        // Minden rendben
        SDL_LOG("Saving directory is okay");
    }
#else
    struct stat st = {0};
    if(stat(dir_name, &st) == -1){
        mkdir(dir_name, 0700);
        SDL_Log("Saving directory created");
    }
#endif

}

void nyerszamol(nyeradat n, int xe, int ye){
    for(int i = 1; i < 5 && *n.irany < 5; i++){
        if(n.r[n.y+ye*i][n.x+xe*i] == n.jelen)
            *n.irany += 1;
        else
            break;
    }
}

menu nyerte(negyzet **r, int x, int y, negyzet jelen, const palyaadat *pd){
    if(pd->rakszamol == pd->sor*pd->oszlop)
        return nyert;

    int vizs = 1;
    int fugg = 1;
    int atlob = 1;
    int atloj = 1;
    nyeradat n;
    n.jelen = jelen;
    n.x = x;
    n.y = y;
    n.r = r;

    n.irany = &vizs;
    nyerszamol(n, -1, 0); // A függvény megvizsgálja, hogy bizonyos irányban mennyi bábuja van egy játékosnak (a második paraméterek - együtthatók - az irányt adják meg)
    nyerszamol(n, 1, 0);

    n.irany = &fugg;
    nyerszamol(n, 0, -1);
    nyerszamol(n, 0, 1);

    n.irany = &atlob;
    nyerszamol(n, -1, -1);
    nyerszamol(n, 1, 1);

    n.irany = &atloj;
    nyerszamol(n, 1, -1);
    nyerszamol(n, -1, 1);

    if(fugg == 5 || vizs == 5 || atloj == 5 || atlob == 5)
        return nyert;
    else
        return jatek;
}

int mentes(palyaadat pd, negyzet **palya, negyzet j){
    FILE *fp;

    directoryCheck("save");
    fp = fopen("save/mentes.dat", "wt");
    if(fp == NULL){
        perror("Nem tudok fajlt irni");
        return -1;
    }
    fprintf(fp, "letezik %d, %d, %d, %d, %d, %d, %d, %d\n", pd.sor, pd.oszlop, pd.negyzetmeret, j, pd.jatekos, pd.rakszamol, pd.eltx, pd.elty);
    for(int i = 4; i < pd.sor + 4; i++){
        for(int j = 4; j < pd.oszlop + 4; j++){
            fprintf(fp, "%d ", palya[i][j]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
    return 0;
}

int jatek_betolt(SDL_Renderer *rende, negyzet ***r, palyaadat* pd, negyzet *jelen){
    FILE* fp;
    fp = fopen("save/mentes.dat", "rb");
    if(fp == NULL){
        SDL_Log("Nincs mentett fajl");
        return -1;
    }
    char be[101];
    fscanf(fp, "%s", be);
    if(strcmp(be, "letezik") != 0){
        perror("A fajlt nem en irtam");
        return 1;
    }
    fscanf(fp,"%d, %d, %d, %d, %d, %d, %d, %d\n", (int*)(&(pd->sor)), (int*)(&(pd->oszlop)), (int*)(&(pd->negyzetmeret)), (int*)(jelen), (int*)(&(pd->jatekos)),
           (int*)(&(pd->rakszamol)), (int*)(&(pd->eltx)), (int*)(&(pd->elty)));

    *r = (negyzet**)malloc(sizeof(negyzet*)*(pd->sor+8));
    assert(*r != 0);
    for(int i = 0; i < (pd->sor+8); i++){
        (*r)[i] = (negyzet*)malloc(sizeof(negyzet)*(pd->oszlop+8));
        assert((*r)[i] != 0);
    }

    keret_init(*r, pd);

    for(int i = 4; i < pd->sor + 4; i++){
        for(int j = 4; j < pd->oszlop + 4; j++){
            fscanf(fp, "%d ", (int*)(&((*r)[i][j])));
        }
        fscanf(fp, "\n");
    }
    fclose(fp);
    return 0;
}

menu jatek_f(SDL_Renderer *rende, SDL_bool betol, palyaadat *pd){
    SDL_RenderClear(rende);
    negyzet **palya; //Egy négyzet lehetséges adatai - enum
    negyzet jelenleg; //Jelenlegi játékos hasonló adatokkal jellemzhető
    if(betol){
        if(jatek_betolt(rende, &palya, pd, &jelenleg) != 0)
            return uj;
        else{
            jatek_kirajzol(rende, *pd);
            jatek_betoltrajz(rende, palya, *pd, jelenleg);
        }
    }
    else{
        jatek_kirajzol(rende, *pd);
        jatek_init(rende, &palya, *pd);
        jelenleg = kor;
        pd->rakszamol = 0;
    }
    SDL_RenderPresent(rende);
    SDL_Event ev;
    menu veg = jatek;

    while(veg != fo && veg != kilep && veg != nyert){
        SDL_WaitEvent(&ev);
        m_re_pause(ev); //a zenét lepausolja
        veg = update_j(rende, ev, palya, pd, &jelenleg);
    }

    if(veg == nyert){
        veg = nyeres(rende, veg, jelenleg, pd);
        remove("save/mentes.dat");
    }
    else
        mentes(*pd, palya, jelenleg);


    for(int i = 0; i < pd->sor+8; i++){
        free(palya[i]);
    }
    free(palya);
    return veg;
}
