#include "menetrend.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "debugmalloc.h"
#include "idopontok.h"

struct MetroGen *vonalak_beolvas() {
    FILE *fp = fopen("megallok.csv", "r");
    if (fp == NULL) {
        printf("Hiba a fajl megnyitasakor!\n");
        exit(1);
    }

    MetroGen *metro = malloc(sizeof(MetroGen));
    *metro = (MetroGen){NULL, 0};  // struktura inicializalasa

    int buffer_size = sizeof(char);  // egy karakter biztos van a fileban
    char *buffer = malloc(buffer_size);
    int buffer_index = 0;

    char ch;
    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '\n' || ch == EOF) {
            if (buffer_index == buffer_size) {
                buffer_size += sizeof(ch);
                buffer = realloc(buffer, buffer_size);
            }
            buffer[buffer_index] = '\0';
            buffer_index = 0;
            if (buffer[0] == ',') continue;
            if (strstr(buffer, ",,") != NULL) continue;
            VonalGen vonal = {NULL, NULL, 0, NULL};
            vonal.nev = strdup(strtok(buffer, ","));
            VonalGen *mozgo = metro->vonalak;
            bool talalt = false;
            while (mozgo != NULL) {
                if (strcmp(mozgo->nev, vonal.nev) == 0) {
                    talalt = true;
                    break;
                }
                mozgo = mozgo->kovetkezo;
            }
            if (talalt) {
                continue;
            }
            char *token = strtok(NULL, ",");
            while (token != NULL) {
                if (vonal.megallokSzama == 0) {
                    vonal.megallo = malloc(sizeof(MegalloGen));
                    vonal.megallo->nev = strdup(token);
                    vonal.megallo->kovetkezo = NULL;
                    vonal.megallo->elozo = NULL;
                } else {
                    MegalloGen *uj = malloc(sizeof(MegalloGen));
                    uj->nev = strdup(token);
                    uj->kovetkezo = NULL;
                    MegalloGen *mozgo = vonal.megallo;
                    while (mozgo->kovetkezo != NULL) {
                        mozgo = mozgo->kovetkezo;
                    }
                    mozgo->kovetkezo = uj;
                    uj->elozo = mozgo;
                }
                token = strtok(NULL, ",");
                vonal.megallokSzama++;
            }
            if (metro->vonalak == NULL) {
                metro->vonalak = malloc(sizeof(VonalGen));
                *metro->vonalak = vonal;
                metro->vonalak->kovetkezo = NULL;
                metro->vonalakSzama++;
            } else {
                VonalGen *uj = malloc(sizeof(VonalGen));
                *uj = vonal;
                uj->kovetkezo = NULL;
                VonalGen *mozgo = metro->vonalak;
                while (mozgo->kovetkezo != NULL) {
                    mozgo = mozgo->kovetkezo;
                }
                mozgo->kovetkezo = uj;
                metro->vonalakSzama++;
            }
        } else {
            if (buffer_index == buffer_size) {
                // Resize the buffer if necessary
                buffer_size += sizeof(ch);
                buffer = realloc(buffer, buffer_size);
            }
            buffer[buffer_index++] = ch;
        }
    }

    free(buffer);
    fclose(fp);
    return metro;
}

struct MegalloGen *elso_megallo(VonalGen vonal) {
    MegalloGen *megallo = vonal.megallo;
    while (megallo->elozo != NULL) {
        megallo = megallo->elozo;
    }
    return megallo;
}

struct MegalloGen *utolso_megallo(VonalGen vonal) {
    MegalloGen *megallo = vonal.megallo;
    while (megallo->kovetkezo != NULL) {
        megallo = megallo->kovetkezo;
    }
    return megallo;
}

void gen_menetrend(MetroGen metro) {
    time_t t;
    srand((unsigned)time(&t));
    FILE *fp = fopen("menetrend.csv", "w");
    if (fp == NULL) {
        printf("Hiba a fajl megnyitasakor!\n");
        exit(1);
    }
    VonalGen *vonal = metro.vonalak;
    while (vonal != NULL) {
        Idopont elso_indulas = (Idopont){0, rand() % 5};
        int indulasi_intervallum = 3;
        int megallo_tavolsag = 2;
        int utolso_indulas_int = 24 * 60 -
                                 megallo_tavolsag * vonal->megallokSzama -
                                 elso_indulas.perc;
        int indulasok_szama = utolso_indulas_int / indulasi_intervallum;

        MegalloGen *elso = elso_megallo(*vonal);
        MegalloGen *utolso = utolso_megallo(*vonal);
        MegalloGen *megallo = elso;
        int megallo_tav = 0;
        while (megallo != NULL) {
            fprintf(fp, "%s,%s,%s,", vonal->nev, utolso->nev, megallo->nev);
            Idopont indulas = elso_indulas;
            indulas = idopont_osszead(
                indulas, (Idopont){0, megallo_tavolsag * megallo_tav});
            for (int i = 0; i < indulasok_szama; i++) {
                indulas = idopont_osszead(indulas,
                                          (Idopont){0, indulasi_intervallum});
                char *bovitett_perc = bovit_nullaval(indulas, PERC);
                if (i == indulasok_szama - 1) {
                    fprintf(fp, "%d:%s", indulas.ora, bovitett_perc);
                } else {
                    fprintf(fp, "%d:%s,", indulas.ora, bovitett_perc);
                }
                free(bovitett_perc);
            }
            fprintf(fp, "\n");
            megallo = megallo->kovetkezo;
            megallo_tav++;
        }
        fprintf(fp, "\n");

        megallo = utolso;
        megallo_tav = 0;
        while (megallo != NULL) {
            fprintf(fp, "%s,%s,%s,", vonal->nev, elso->nev, megallo->nev);
            Idopont indulas = elso_indulas;
            indulas = idopont_osszead(
                indulas, (Idopont){0, megallo_tavolsag * megallo_tav});
            for (int i = 0; i < indulasok_szama; i++) {
                indulas = idopont_osszead(indulas,
                                          (Idopont){0, indulasi_intervallum});
                char *bovitett_perc = bovit_nullaval(indulas, PERC);
                if (i == indulasok_szama - 1) {
                    fprintf(fp, "%d:%s", indulas.ora, bovitett_perc);
                } else {
                    fprintf(fp, "%d:%s,", indulas.ora, bovitett_perc);
                }
                free(bovitett_perc);
            }
            fprintf(fp, "\n");
            megallo = megallo->elozo;
            megallo_tav++;
        }
        fprintf(fp, "\n");
        vonal = vonal->kovetkezo;
    }
    fclose(fp);
}

void free_metro(MetroGen *metro) {
    VonalGen *vonal = metro->vonalak;
    while (vonal != NULL) {
        MegalloGen *megallo = vonal->megallo;
        while (megallo != NULL) {
            MegalloGen *kov = megallo->kovetkezo;
            free(megallo);
            megallo = kov;
        }
        VonalGen *kov = vonal->kovetkezo;
        free(vonal);
        vonal = kov;
    }
    free(metro);
}

void del_menetrend() { remove("menetrend.csv"); }
