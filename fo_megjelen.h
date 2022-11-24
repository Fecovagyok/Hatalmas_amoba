#ifndef FO_MEGJELEN_H_INCLUDED
#define FO_MEGJELEN_H_INCLUDED
void sdl_init(char const *felirat, SDL_Window **pwindow, SDL_Renderer **prenderer);

typedef struct hszog{
    int x1, x2, x3, y1, y2, y3;
} hszog;

typedef struct eger{  //Az eventes adat�tad�sokn�l nagy seg�ts�gemre van
    SDL_bool katt, folott, fol;
    int gomb;
} eger;

typedef struct gombadat{ // A kerek�tett sark� teglalap adait tartalmazza, hogy k�s�bb az event-tel hivatkozhassak r�
    SDL_Renderer *rende;
    int x1, y1, x2, y2, gombsug, r, g, b, a;
} gombadat;

typedef struct szin{
    int r, g, b, a;
} szin;

typedef struct feliratt{ //Gyakran van sz�ks�g a feliratok �jb�li ki�r�s�ra, ilyenkor nagyban seg�t ha azoknak a text�r�j�t elt�rolom, �s csak azokkal dolgozok tov�bb
    SDL_Texture *kep;
    SDL_Rect tegla;
    hszog hsz;
} feliratt;

typedef enum menu {kilep, fo, uj, jatek, betolt, nyert} menu;

void hatterbetolt(SDL_Renderer *rende, feliratt *hatter, char *fajl);
int kerekitett_tegla(gombadat g);
int kerekitett_doboz(gombadat g);

menu update_f(SDL_Event ev, SDL_Renderer *rende, gombadat* g, SDL_bool *ravittem, feliratt *elem, SDL_bool *lent);
gombadat* fomenu_rajz(SDL_Renderer*rende, gombadat* g, feliratt *e);

gombadat szovegkiirkozep(SDL_Renderer *rende, char* szoveg, int betumeretarany, int nevezo, int szamlalo, SDL_Color sz, feliratt *e, SDL_bool irjam);


#endif // FO_MEGJELEN_H_INCLUDED
