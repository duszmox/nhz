#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "idopontok.h"

typedef struct Megallo {
    char *nev;
    struct Megallo *kovetkezo;
    struct Megallo *elozo;
} Megallo;

typedef struct Vonal {
    char *nev;
    Megallo *megallo;
    int megallokSzama;
    struct Vonal *kovetkezo;
} Vonal;

typedef struct Metro {
    Vonal *vonalak;
    int vonalakSzama;
} Metro;

Metro *vonalak_beolvas() {
    FILE *fp = fopen("megallok.csv", "r");
    if (fp == NULL) {
        printf("Hiba a fajl megnyitasakor!\n");
        exit(1);
    }

    Metro *metro = malloc(sizeof(Metro));
    *metro = (Metro){NULL, 0};

    int buffer_size = sizeof(char);  // Initial buffer size
    char *buffer = malloc(buffer_size);
    int buffer_index = 0;

    char ch;
    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '\n' || ch == EOF) {
            if (buffer_index == buffer_size) {
                buffer_size += sizeof(ch);
                buffer = realloc(buffer, buffer_size);
            }
            buffer[buffer_index] = '\0';  // Null-terminate the line
            buffer_index = 0;

            Vonal vonal = {NULL, NULL, 0, NULL};
            vonal.nev = strdup(strtok(buffer, ","));
            char *token = strtok(NULL, ",");
            while (token != NULL) {
                if (vonal.megallokSzama == 0) {
                    vonal.megallo = malloc(sizeof(Megallo));
                    vonal.megallo->nev = strdup(token);
                    vonal.megallo->kovetkezo = NULL;
                    vonal.megallo->elozo = NULL;
                } else {
                    Megallo *uj = malloc(sizeof(Megallo));
                    uj->nev = strdup(token);
                    uj->kovetkezo = NULL;
                    Megallo *mozgo = vonal.megallo;
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
                metro->vonalak = malloc(sizeof(Vonal));
                *metro->vonalak = vonal;
                metro->vonalak->kovetkezo = NULL;
                metro->vonalakSzama++;
            } else {
                Vonal *uj = malloc(sizeof(Vonal));
                *uj = vonal;
                uj->kovetkezo = NULL;
                Vonal *mozgo = metro->vonalak;
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
// return the first Megallo of the Vonal
Megallo *elso_megallo(Vonal vonal) {
    Megallo *megallo = vonal.megallo;
    while (megallo->elozo != NULL) {
        megallo = megallo->elozo;
    }
    return megallo;
}

Megallo *utolso_megallo(Vonal vonal) {
    Megallo *megallo = vonal.megallo;
    while (megallo->kovetkezo != NULL) {
        megallo = megallo->kovetkezo;
    }
    return megallo;
}

void gen_menetrend(Metro metro) {
    time_t t;
    srand((unsigned)time(&t));
    FILE *fp = fopen("menetrend.csv", "w");
    if (fp == NULL) {
        printf("Hiba a fajl megnyitasakor!\n");
        exit(1);
    }
    Vonal *vonal = metro.vonalak;
    while (vonal != NULL) {
        Idopont elso_indulas = (Idopont){0, rand() % 5};
        int indulasi_intervallum = 3;
        int megallo_tavolsag = 2;
        int utolso_indulas_int =
            24 * 60 - megallo_tavolsag * vonal->megallokSzama - 1;
        int indulasok_szama = utolso_indulas_int / indulasi_intervallum;
        printf("%d\n", indulasok_szama);
        printf("\n");
        Megallo *elso = elso_megallo(*vonal);
        Megallo *utolso = utolso_megallo(*vonal);
        Megallo *megallo = elso;
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
                fprintf(fp, "%d:%s,", indulas.ora, bovitett_perc);
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
                fprintf(fp, "%d:%s,", indulas.ora, bovitett_perc);
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

void free_metro(Metro *metro) {
    Vonal *vonal = metro->vonalak;
    while (vonal != NULL) {
        Megallo *megallo = vonal->megallo;
        while (megallo != NULL) {
            Megallo *kov = megallo->kovetkezo;
            free(megallo);
            megallo = kov;
        }
        Vonal *kov = vonal->kovetkezo;
        free(vonal);
        vonal = kov;
    }
    free(metro);
}