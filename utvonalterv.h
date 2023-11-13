
#ifndef HEADER_FILE
#define HEADER_FILE
#include <ctype.h>
#include <stdbool.h>
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
typedef struct MegalloList {
    Megallo *megallo;
    int size;
} MegalloList;
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
                printf("%s\n", megalloNev);
                megallo->nev = malloc(sizeof(char) * strlen(megalloNev) + 1);
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
MegalloList *megallo_search(Metro *metro, char *megallo_chunk) {
    int megallokSzama = 0;
    Megallo *megallok = malloc(sizeof(Megallo));
    *megallok = (Megallo){NULL, NULL, NULL, NULL, NULL, 0, 0};
    Vonal *vonal = metro->vonalak;

    while (vonal != NULL) {
        Megallo *mozgo = vonal->megallo;
        while (mozgo != NULL) {
            Megallo *mozgo2 = megallok;
            if (megallokSzama != 0) {
                while (mozgo2 != NULL) {
                    if (strcmp(mozgo2->nev, mozgo->nev) == 0) {
                        break;
                    }
                    mozgo2 = mozgo2->kovetkezo;
                }
                if (mozgo2 != NULL) {
                    mozgo = mozgo->kovetkezo;
                    continue;
                }
            }

            if (strcasestr(mozgo->nev, megallo_chunk) != NULL ||
                strcmp(megallo_chunk, "") == 0) {
                Megallo *uj = malloc(sizeof(Megallo));
                *uj = (Megallo){NULL, NULL, NULL, NULL, NULL, 0, 0};
                uj->nev = malloc(sizeof(char) * strlen(mozgo->nev) + 1);
                strcpy(uj->nev, mozgo->nev);
                uj->ido1 = mozgo->ido1;
                uj->ido2 = mozgo->ido2;
                uj->ido1Hossz = mozgo->ido1Hossz;
                uj->ido2Hossz = mozgo->ido2Hossz;
                uj->kovetkezo = NULL;
                uj->elozo = NULL;
                if (megallokSzama == 0) {
                    *megallok = *uj;
                } else {
                    Megallo *mozgo2 = megallok;
                    while (mozgo2->kovetkezo != NULL)
                        mozgo2 = mozgo2->kovetkezo;
                    mozgo2->kovetkezo = uj;
                    uj->elozo = mozgo2;
                }
                megallokSzama++;
            }
            mozgo = mozgo->kovetkezo;
        }
        vonal = vonal->kovetkezo;
    }

    MegalloList *megalloList = malloc(sizeof(MegalloList));
    *megalloList = (MegalloList){NULL, 0};
    megalloList->megallo = megallok;
    megalloList->size = megallokSzama;
    return megalloList;
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
    unsigned long i, j;
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