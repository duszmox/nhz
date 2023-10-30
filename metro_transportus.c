#include "megallok.h"
#include "debugmalloc.h"

int main()
{
    Metro *metro = vonalak_beolvas();
    printf("Vonalak szama: %d\n", metro->vonalakSzama);
    Vonal *vonal = metro->vonalak;
    while (vonal != NULL)
    {
        printf("Vonal neve: %s\n", vonal->nev);
        printf("Megallok szama: %d\n", vonal->megallokSzama);
        Megallo *megallo = vonal->megallo;
        while (megallo != NULL)
        {
            printf("Megallo neve: %s\n", megallo->nev);
            megallo = megallo->kovetkezo;
        }
        vonal = vonal->kovetkezo;
    }
    gen_menetrend(*metro);

    return 0;
}