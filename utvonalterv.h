
#ifndef UTVONALTERV_H
#define UTVONALTERV_H
#include <stdbool.h>

#include "idopontok.h"
typedef struct Megallo {
    char *nev;
    struct Megallo *kovetkezo;
    struct Megallo *elozo;
    struct Idopont *ido1;
    struct Idopont *ido2;
    int ido1Hossz;
    int ido2Hossz;
} Megallo;

typedef struct Vonal {
    char *nev;
    struct Megallo *megallo;
    int megallokSzama;
    struct Vonal *kovetkezo;
    struct Vonal *elozo;
} Vonal;

typedef struct Metro {
    struct Vonal *vonalak;
    int vonalakSzama;
} Metro;
typedef struct MegalloList {
    struct Megallo *megallo;
    int size;
} MegalloList;

typedef struct AtszallasiMegallo {
    struct Megallo *megallo;
    struct Vonal *vonal;
    struct AtszallasiMegallo *kovetkezo;
} AtszallasiMegallo;

typedef struct Utvonalterv {
    struct Megallo *indulo;
    struct Megallo *cel;
    struct Idopont *indulasiIdo;
    struct Idopont *erkezesiIdo;
    struct Vonal *vonal;
    struct Utvonalterv *kovetkezo;
} Utvonalterv;

struct Metro *menetrend_beolvas();
int *megallo_distance(Vonal *vonal, char *megallo1, char *megallo2);
struct Megallo *find_megallo_for_string(Metro *metro, const char *megalloNev);
struct MegalloList *megallo_search(Metro *metro, char *megallo_chunk);
void sort_megallo_array(Megallo *megallok);
int count_megallok(Megallo *megallok);
struct Vonal *find_vonal_for_megallo_string(struct Metro *metro,
                                            const char *megalloNev);
struct AtszallasiMegallo *atszallasi_megallok_on_vonal(struct Metro *Metro,
                                                       struct Vonal *vonal);
struct Vonal *are_megallok_on_same_vonal_string(Metro *metro,
                                                const char *megallo1,
                                                const char *megallo2);
void free_megallo_list(MegalloList *megalloList);
void free_metro_network(Metro *metro);
void free_utvonalterv(Utvonalterv *utvonalterv);
void free_vonals(Vonal *vonalak);
void free_atszallasi_megallok(AtszallasiMegallo **atszallasiMegallok, int size);
struct Megallo *is_string_on_megallo_vonal(Megallo *megallo, char *megalloNev);
#endif
