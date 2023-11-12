
#ifndef HEADER_FILE
#define HEADER_FILE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "idopontok.h"
#endif

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

Metro *menetrend_beolvas() {
    FILE *fp = fopen("menetrend.csv", "r");
    if (fp == NULL) {
        printf("Hiba a fajl megnyitasakor!\n");
        exit(1);
    }

    Metro *metro = malloc(sizeof(Metro));
    *metro = (Metro){NULL, 0};

    int buffer_size = sizeof(char);  // egy karakter biztos van a fileban
    char *buffer = malloc(buffer_size);
    int buffer_index = 0;
    char ch;
    Vonal *vonal = malloc(sizeof(Vonal));
    *vonal = (Vonal){NULL, NULL, 0, NULL, NULL};
    int round = 0;
    Megallo *megallo = malloc(sizeof(Megallo));
    *megallo = (Megallo){NULL, NULL, NULL, NULL, NULL, 0, 0};
    vonal->megallo = megallo;
    metro->vonalak = vonal;
    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '\n' || ch == EOF) {
            if (buffer_index == buffer_size) {
                buffer_size += sizeof(ch);
                buffer = realloc(buffer, buffer_size);
            }
            buffer[buffer_index] = '\0';  // Null-terminate the line
            buffer_index = 0;

            if (strcmp(buffer, "") == 0) {
                if (round == 1) {
                    Vonal **tmp = &vonal;
                    metro->vonalakSzama++;

                    vonal->kovetkezo = malloc(sizeof(Vonal));
                    *vonal->kovetkezo = (Vonal){NULL, NULL, 0, NULL, *tmp};
                    vonal = vonal->kovetkezo;
                    round = 0;
                    vonal->megallo = malloc(sizeof(Megallo));
                    *vonal->megallo =
                        (Megallo){NULL, NULL, NULL, NULL, NULL, 0, 0};
                    megallo = vonal->megallo;
                    continue;
                } else {
                    round++;
                    megallo = megallo->elozo;
                    megallo->kovetkezo = NULL;
                    megallo = vonal->megallo;
                    continue;
                }
            }
            if (vonal->nev == NULL) {
                vonal->nev = malloc(sizeof(char) * buffer_size);
                strcpy(vonal->nev, strtok(buffer, ","));
            } else {
                strtok(buffer, ",");
            }

            int numberOfCommas = 0;
            for (int i = 0; i < buffer_size; i++) {
                if (buffer[i] == ',') {
                    numberOfCommas++;
                }
            }
            numberOfCommas -= 3;
            Idopont *ido = malloc(sizeof(Idopont) * numberOfCommas);
            if (ido == NULL) {
                printf("Nem sikerult helyet foglalni!\n");
                exit(1);
            }
            if (round == 0) {
                strtok(NULL, ",");
                // printf("%s\n", asd);
                char *megalloNev = strtok(NULL, ",");
                megallo->nev = malloc(sizeof(char) * buffer_size);
                strcpy(megallo->nev, megalloNev);
                // printf("%s\n", megallo->nev);
                megallo->ido1 = ido;
                megallo->ido1Hossz = numberOfCommas;
            } else {
                strtok(NULL, ",");
                strtok(NULL, ",");
                megallo->ido2 = ido;
                megallo->ido2Hossz = numberOfCommas;
                // printf("%s\n", megallo->nev);
            }
            int i = 0;
            while (i < numberOfCommas) {
                ido[i] = (Idopont){0, 0};
                ido[i].ora = atoi(strtok(NULL, ":"));
                ido[i].perc = atoi(strtok(NULL, ","));
                i++;
            }
            if (round == 0) {
                Megallo **tmp = &megallo;
                megallo->kovetkezo = malloc(sizeof(Megallo));
                *megallo->kovetkezo =
                    (Megallo){NULL, NULL, *tmp, NULL, NULL, 0, 0};
                megallo = megallo->kovetkezo;
                vonal->megallokSzama++;
            } else {
                megallo = megallo->kovetkezo;
            }
        } else {
            if (buffer_index == buffer_size) {
                // Resize the buffer if necessary
                buffer_size += sizeof(ch);
                buffer = realloc(buffer, buffer_size);
            }
            buffer[buffer_index++] = ch;
            // printf("%s\n", megallo->nev);
        }
    }
    vonal = vonal->elozo;
    vonal->kovetkezo = NULL;

    free(buffer);
    fclose(fp);
    return metro;
}

bool is_on_vonal(Vonal *vonal, char *megallo) {
    Megallo *mozgo = vonal->megallo;
    while (mozgo != NULL) {
        if (strcmp(mozgo->nev, megallo) == 0) {
            return true;
        }
        mozgo = mozgo->kovetkezo;
    }
    return false;
}
int *megallo_distance(Vonal *vonal, char *megallo1, char *megallo2) {
    if (!is_on_vonal(vonal, megallo1) || !is_on_vonal(vonal, megallo2)) {
        return NULL;
    }
    int tav = 0;
    Megallo *mozgo = vonal->megallo;
    while (mozgo != NULL) {
        if (strcmp(mozgo->nev, megallo1) == 0) {
            break;
        }
        tav++;
        mozgo = mozgo->kovetkezo;
    }
    int tav2 = 0;
    mozgo = vonal->megallo;
    while (mozgo != NULL) {
        if (strcmp(mozgo->nev, megallo2) == 0) {
            break;
        }
        tav2++;
        mozgo = mozgo->kovetkezo;
    }
    int *distance = malloc(sizeof(int));
    *distance = tav2 - tav;
    return distance;
}
Megallo *megallo_search(Metro *metro, char *megallo_chunk) {
    //    return all megallok where megallonev contains megallonev_chunk, it's
    //    not case sensitive
    int megallokSzama = 0;
    Megallo *megallok = malloc(sizeof(Megallo));
    *megallok = (Megallo){NULL, NULL, NULL, NULL, NULL, 0, 0};
    Vonal *mozgo = metro->vonalak;
    while (mozgo != NULL) {
        Megallo *mozgo2 = mozgo->megallo;
        while (mozgo2 != NULL) {
            if (strstr(mozgo2->nev, megallo_chunk) != NULL) {
                Megallo **tmp = &megallok;
                megallokSzama++;
                megallok = realloc(megallok, sizeof(Megallo) * megallokSzama);
                megallok[megallokSzama - 1] = *mozgo2;
                megallok[megallokSzama - 1].kovetkezo = NULL;
                megallok[megallokSzama - 1].elozo = *tmp;
                if (megallok[megallokSzama - 1].elozo != NULL) {
                    megallok[megallokSzama - 1].elozo->kovetkezo =
                        &megallok[megallokSzama - 1];
                }
            }
            mozgo2 = mozgo2->kovetkezo;
        }
        mozgo = mozgo->kovetkezo;
    }
    if (megallokSzama == 0) {
        // return all megallok
        Megallo *mozgo = metro->vonalak->megallo;
        while (mozgo != NULL) {
            Megallo **tmp = &megallok;
            megallokSzama++;
            megallok = realloc(megallok, sizeof(Megallo) * megallokSzama);
            megallok[megallokSzama - 1] = *mozgo;
            megallok[megallokSzama - 1].kovetkezo = NULL;
            megallok[megallokSzama - 1].elozo = *tmp;
            if (megallok[megallokSzama - 1].elozo != NULL) {
                megallok[megallokSzama - 1].elozo->kovetkezo =
                    &megallok[megallokSzama - 1];
            }
            mozgo = mozgo->kovetkezo;
        }
    }
    return megallok;
}
Vonal *find_vonal_for_megallo(Metro *metro, Megallo *megallo) {
    Vonal *mozgo = metro->vonalak;
    while (mozgo != NULL) {
        Megallo *mozgo2 = mozgo->megallo;
        while (mozgo2 != NULL) {
            if (strcmp(mozgo2->nev, megallo->nev) == 0) {
                return mozgo;
            }
            mozgo2 = mozgo2->kovetkezo;
        }
        mozgo = mozgo->kovetkezo;
    }
    return NULL;
}
void sort_megallo_array(Megallo *megallok) {
    int i, j;
    Megallo temp;
    for (i = 0; i < sizeof(megallok) - 1; i++) {
        for (j = 0; j < sizeof(megallok) - i - 1; j++) {
            if (strcmp(megallok[j].nev, megallok[j + 1].nev) > 0) {
                temp = megallok[j];
                megallok[j] = megallok[j + 1];
                megallok[j + 1] = temp;
            }
        }
    }
}
int count_megallok(Megallo *megallok) {
    int count = 0;
    Megallo *mozgo = megallok;
    while (mozgo != NULL) {
        count++;
        mozgo = mozgo->kovetkezo;
    }
    return count;
}