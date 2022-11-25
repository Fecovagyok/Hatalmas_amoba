//
// Created by tumpek on 11/25/22.
//

#include "palya_manage.h"

void keret_init(negyzet **palya, palyaadat *pd){
    for(int i = 0; i < pd->sor+8; i++){ // Utolso
        for(int j = 0; j < 4; j++) {
            palya[i][j] = ninit;
            palya[i][pd->oszlop + 8 - j - 1] = ninit;
        }
    }
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < pd->oszlop + 8; j++){
            palya[i][j] = ninit;
            palya[pd->sor + 8 - i - 1][j] = ninit;
        }
    }
}

void ures_init(negyzet **palya, palyaadat *pd){
    for(int i = 4; i < pd->sor + 4; i++){
        for(int j = 4; j < pd->oszlop + 4; j++){
            palya[i][j] = ures;
        }
    }
}

void palya_init(negyzet **palya, palyaadat *pd){
    ures_init(palya, pd);
    keret_init(palya, pd);
}
