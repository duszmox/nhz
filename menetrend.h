#ifndef MENETREND_H
#define MENETREND_H
typedef struct MegalloGen {
    char *nev;
    struct MegalloGen *kovetkezo;
    struct MegalloGen *elozo;
} MegalloGen;

typedef struct VonalGen {
    char *nev;
    MegalloGen *megallo;
    int megallokSzama;
    struct VonalGen *kovetkezo;
} VonalGen;

typedef struct MetroGen {
    VonalGen *vonalak;
    int vonalakSzama;
} MetroGen;
struct MetroGen *vonalak_beolvas();
void gen_menetrend(MetroGen metro);
void free_metro(MetroGen *metro);
void del_menetrend();

#endif