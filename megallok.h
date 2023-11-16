#ifndef MEGALLOK_H
#define MEGALLOK_H
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
MetroGen *vonalak_beolvas();
MegalloGen *elso_megallo(VonalGen vonal);
MegalloGen *utolso_megallo(VonalGen vonal);
void gen_menetrend(MetroGen metro);
void free_metro(MetroGen *metro);
void del_menetrend();

#endif