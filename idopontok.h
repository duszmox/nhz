#ifndef IDOPONTOK_H
#define IDOPONTOK_H
#include <stdbool.h>
typedef struct Idopont {
    int ora;
    int perc;
} Idopont;
typedef enum idopont_tipus { ORA, PERC } idopont_tipus;

Idopont idopont_osszead(Idopont idopont1, Idopont idopont2);
char *idopont_to_string(Idopont idopont);
Idopont string_to_idopont(char *s);
bool ido_kisebb(Idopont ido1, Idopont ido2);
int ido_kulonbseg_perc(Idopont ido1, Idopont ido2);
char *bovit_nullaval(Idopont idopont, idopont_tipus it);
#endif
