#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "megallok.h"
#include "utvonalterv.h"

int main() {
    Metro *metro = menetrend_beolvas();
    MegalloList *megallok = megallo_search(metro, "");
    Megallo *mozgo = megallok->megallo;
    while (mozgo != NULL) {
        printf("%s,", mozgo->nev);
        mozgo = mozgo->kovetkezo;
    }
    printf("\n");
    printf("%d", count_megallok(megallok->megallo));
    return 0;
}