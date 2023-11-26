
#ifndef METROGRAPH_H
#define METROGRAPH_H
#include <stdbool.h>

#include "idopontok.h"
#include "utvonalterv.h"
struct El {
    int celAllomas;
    int suly;
    struct El* kov;
};

struct AllomasVertex {
    int taroltMegallokSzama;
    struct Megallo** megallok;
    struct El* elek;
};

struct MetroGraph {
    int allomasVSzam;
    struct AllomasVertex* tomb;
};

Utvonalterv* utvonaltervezes(Metro* metro, char* indulo, char* cel,
                             Idopont indulasiIdo);

#endif