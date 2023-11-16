#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "megallok.h"
#include "utvonalterv.h"

int main() {
    Metro *metro = menetrend_beolvas();
    if (metro == NULL) {
        printf("Hiba a menetrend beolvasasakor!\n");
        return 1;
    }
    // Utvonalterv *utvonalterv = malloc(sizeof(Utvonalterv));
    // if (utvonalterv == NULL) {
    //     printf("Nem sikerult helyet foglalni az utvonaltervnek!\n");
    //     return 1;
    // }
    // *utvonalterv = (Utvonalterv){NULL, NULL, NULL, NULL, 0, NULL, NULL};
    // utvonalterv->indulo = metro->vonalak->kovetkezo->megallo;
    // // utvonalterv->indulo = metro->vonalak->megallo;
    // utvonalterv->cel = metro->vonalak->megallo;
    // utvonalterv->indulasiIdo = malloc(sizeof(Idopont));
    // printf("Indulo: %s\n", utvonalterv->indulo->nev);
    // printf("Cel: %s\n\n", utvonalterv->cel->nev);
    // *utvonalterv->indulasiIdo = (Idopont){8, 0};
    // Utvonalterv *uj = utvonaltervezes(metro, utvonalterv);
    // if (uj == NULL) {
    //     printf("Nem sikerult utvonalat talalni!\n");
    //     return 1;
    // }
    // Utvonalterv *mozgo = uj;
    // int i = 0;
    // while (mozgo != NULL) {
    //     printf("%d\n", i);
    //     printf("Indulo: %s\n", mozgo->indulo->nev);
    //     printf("Vonal: %s\n", mozgo->vonal->nev);
    //     printf("Cel: %s\n", mozgo->cel->nev);
    //     i++;
    //     mozgo = mozgo->kovetkezo;
    // }
    if (are_megallok_on_same_vonal_string(metro, "Kelenföld Vasútállomás",
                                          "Kálvin Tér")) {
        printf("Igen\n");
    } else {
        printf("Nem\n");
    }

    return 0;
}