
#ifndef UTVONALTERV_H
#define UTVONALTERV_H
#include <stdbool.h>

#include "idopontok.h"
typedef struct Megallo {
    char *nev;
    struct Megallo *kovetkezo;
    struct Megallo *elozo;
    Idopont *ido1;
    Idopont *ido2;
    int ido1Hossz;
    int ido2Hossz;
} Megallo;

typedef struct Vonal {
    char *nev;
    Megallo *megallo;
    int megallokSzama;
    struct Vonal *kovetkezo;
    struct Vonal *elozo;
} Vonal;

typedef struct Metro {
    Vonal *vonalak;
    int vonalakSzama;
} Metro;
typedef struct MegalloList {
    Megallo *megallo;
    int size;
} MegalloList;

typedef struct AtszallasiMegallo {
    Megallo *megallo;
    Vonal *vonal;
    struct AtszallasiMegallo *kovetkezo;
} AtszallasiMegallo;

typedef struct Utvonalterv {
    Megallo *indulo;
    Megallo *cel;
    Idopont *indulasiIdo;
    Idopont *erkezesiIdo;
    Vonal *vonal;
    struct Utvonalterv *kovetkezo;
} Utvonalterv;

Metro *menetrend_beolvas();
bool is_on_vonal(Vonal *vonal, char *megallo);
int *megallo_distance(Vonal *vonal, char *megallo1, char *megallo2);
MegalloList *megallo_search(Metro *metro, char *megallo_chunk);
Vonal *find_vonal_for_megallo(Metro *metro, Megallo *megallo);
void sort_megallo_array(Megallo *megallok);
int count_megallok(Megallo *megallok);
Vonal *find_vonal_for_megallo_string(Metro *metro, char *megalloNev);
AtszallasiMegallo *atszallasi_megallok_on_vonal(Metro *Metro, Vonal *vonal);
int count_utvonal_distance(Metro *metro, Utvonalterv *utvonalterv);
struct Vonal *are_megallok_on_same_vonal_string(Metro *metro, char *megallo1,
                                                char *megallo2);
void free_megallo_list(MegalloList *megalloList);
void free_metro_network(Metro *metro);
struct Megallo *find_megallo_for_string(Metro *metro, char *megalloNev);
void free_utvonalterv(Utvonalterv *utvonalterv);
void free_vonals(Vonal *vonalak);
void free_atszallasi_megallok(AtszallasiMegallo **atszallasiMegallok, int size);
#endif
