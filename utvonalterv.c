#include "utvonalterv.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "debugmalloc.h"
#include "idopontok.h"

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
            buffer[buffer_index] = '\0';
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
                    free(megallo->kovetkezo);
                    megallo->kovetkezo = NULL;
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
                char *megalloNev = strtok(NULL, ",");
                megallo->nev = malloc(sizeof(char) * strlen(megalloNev) + 1);
                strcpy(megallo->nev, megalloNev);
                megallo->ido1 = ido;
                megallo->ido1Hossz = numberOfCommas + 2;
            } else {
                strtok(NULL, ",");
                strtok(NULL, ",");
                megallo->ido2 = ido;
                megallo->ido2Hossz = numberOfCommas + 2;
            }
            int i = 0;
            while (i <= numberOfCommas + 1) {
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
                megallo = megallo->elozo;
            }
        } else {
            if (buffer_index == buffer_size) {
                buffer_size += sizeof(ch);
                buffer = realloc(buffer, buffer_size);
            }
            buffer[buffer_index++] = ch;
        }
    }
    vonal = vonal->elozo;
    free(vonal->kovetkezo->megallo);
    free(vonal->kovetkezo);
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
                uj->ido1 = malloc(sizeof(Idopont) * mozgo->ido1Hossz);
                *uj->ido1 = *mozgo->ido1;
                uj->ido2 = malloc(sizeof(Idopont) * mozgo->ido2Hossz);
                *uj->ido2 = *mozgo->ido2;
                uj->ido1Hossz = mozgo->ido1Hossz;
                uj->ido2Hossz = mozgo->ido2Hossz;
                uj->kovetkezo = NULL;
                uj->elozo = NULL;
                if (megallokSzama == 0) {
                    *megallok = *uj;
                    free(uj);
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
    int swapped;
    struct Megallo *ptr1;
    struct Megallo *lptr = NULL;

    /* Checking for empty list */
    if (megallok == NULL) return;

    do {
        swapped = 0;
        ptr1 = megallok;

        while (ptr1->kovetkezo != lptr) {
            if (strcmp(ptr1->nev, ptr1->kovetkezo->nev) > 0) {
                // Swap data of two nodes
                char *temp = ptr1->nev;
                ptr1->nev = ptr1->kovetkezo->nev;
                ptr1->kovetkezo->nev = temp;
                swapped = 1;
            }
            ptr1 = ptr1->kovetkezo;
        }
        lptr = ptr1;
    } while (swapped);
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
Vonal *find_vonal_for_megallo_string(Metro *metro, char *megalloNev) {
    Vonal *mozgo = metro->vonalak;
    Vonal *talaltVonalak = NULL;
    while (mozgo != NULL) {
        Megallo *mozgoMegallo = mozgo->megallo;
        while (mozgoMegallo != NULL) {
            if (strcmp(mozgoMegallo->nev, megalloNev) == 0) {
                if (talaltVonalak == NULL) {
                    talaltVonalak = malloc(sizeof(Vonal));
                    *talaltVonalak = *mozgo;
                    talaltVonalak->elozo = NULL;
                    talaltVonalak->kovetkezo = NULL;
                    break;
                } else {
                    Vonal *mozgo2 = talaltVonalak;
                    while (mozgo2->kovetkezo != NULL) {
                        mozgo2 = mozgo2->kovetkezo;
                    }
                    mozgo2->kovetkezo = malloc(sizeof(Vonal));
                    *mozgo2->kovetkezo = *mozgo;
                    mozgo2->kovetkezo->kovetkezo = NULL;
                    mozgo2->kovetkezo->elozo = mozgo2;
                    break;
                }
            }
            mozgoMegallo = mozgoMegallo->kovetkezo;
        }
        mozgo = mozgo->kovetkezo;
    }
    return talaltVonalak;
}
AtszallasiMegallo *atszallasi_megallok_on_vonal(Metro *Metro, Vonal *vonal) {
    AtszallasiMegallo *atszallasiMegallok = malloc(sizeof(AtszallasiMegallo));
    *atszallasiMegallok = (AtszallasiMegallo){NULL, NULL, NULL};
    Megallo *mozgo = vonal->megallo;
    while (mozgo != NULL) {
        Vonal *vonalakForMegallo =
            find_vonal_for_megallo_string(Metro, mozgo->nev);
        int i = 0;
        Vonal *vonalakForMegallo2 = vonalakForMegallo;
        while (vonalakForMegallo2 != NULL) {
            i++;
            vonalakForMegallo2 = vonalakForMegallo2->kovetkezo;
        }
        if (i > 1) {
            AtszallasiMegallo *uj = malloc(sizeof(AtszallasiMegallo));
            *uj = (AtszallasiMegallo){NULL, NULL, NULL};
            uj->megallo = malloc(sizeof(Megallo));
            *uj->megallo = *mozgo;
            uj->vonal = malloc(sizeof(Vonal));
            *uj->vonal = *vonal;
            uj->kovetkezo = NULL;
            uj->vonal->elozo = NULL;
            uj->vonal->kovetkezo = NULL;
            if (atszallasiMegallok->megallo == NULL) {
                *atszallasiMegallok = *uj;
                free(uj);
            } else {
                AtszallasiMegallo *mozgo2 = atszallasiMegallok;
                while (mozgo2->kovetkezo != NULL) {
                    mozgo2 = mozgo2->kovetkezo;
                }
                mozgo2->kovetkezo = uj;
            }
        }
        mozgo = mozgo->kovetkezo;
        free_vonals(vonalakForMegallo);
    }
    return atszallasiMegallok;
}
int count_utvonal_distance(Metro *metro, Utvonalterv *utvonalterv) {
    int tav = 0;
    Utvonalterv *mozgo = utvonalterv;
    while (mozgo != NULL) {
        Vonal *vonalakForMegallo =
            find_vonal_for_megallo_string(metro, mozgo->indulo->nev);
        int *distance = megallo_distance(vonalakForMegallo, mozgo->indulo->nev,
                                         mozgo->cel->nev);
        tav += *distance;
        free(distance);
        free(vonalakForMegallo);
        mozgo = mozgo->kovetkezo;
    }
    return tav;
}
struct Vonal *are_megallok_on_same_vonal_string(Metro *metro, char *megallo1,
                                                char *megallo2) {
    Vonal *mozgo = metro->vonalak;
    while (mozgo != NULL) {
        Megallo *mozgo2 = mozgo->megallo;
        while (mozgo2 != NULL) {
            if (strcmp(mozgo2->nev, megallo1) == 0) {
                Megallo *mozgo3 = mozgo->megallo;
                while (mozgo3 != NULL) {
                    if (strcmp(mozgo3->nev, megallo2) == 0) {
                        return mozgo;
                    }
                    mozgo3 = mozgo3->kovetkezo;
                }
            }
            mozgo2 = mozgo2->kovetkezo;
        }
        mozgo = mozgo->kovetkezo;
    }
    return NULL;
}

struct Megallo *find_megallo_for_string(Metro *metro, char *megalloNev) {
    Vonal *mozgo = metro->vonalak;
    while (mozgo != NULL) {
        Megallo *mozgo2 = mozgo->megallo;
        while (mozgo2 != NULL) {
            if (strcmp(mozgo2->nev, megalloNev) == 0) {
                return mozgo2;
            }
            mozgo2 = mozgo2->kovetkezo;
        }
        mozgo = mozgo->kovetkezo;
    }
    return NULL;
}

void free_megallo_list(MegalloList *megalloList) {
    Megallo *mozgo = megalloList->megallo;
    while (mozgo != NULL) {
        Megallo *mozgo2 = mozgo->kovetkezo;
        free(mozgo->nev);
        free(mozgo->ido1);
        free(mozgo->ido2);
        free(mozgo);
        mozgo = mozgo2;
    }
    free(megalloList);
}

void free_metro_network(Metro *metro) {
    Vonal *mozgo = metro->vonalak;
    while (mozgo != NULL) {
        Vonal *mozgo2 = mozgo->kovetkezo;
        Megallo *mozgo3 = mozgo->megallo;
        while (mozgo3 != NULL) {
            Megallo *mozgo4 = mozgo3->kovetkezo;
            if (mozgo3->nev != NULL) free(mozgo3->nev);
            if (mozgo3->ido1 != NULL) free(mozgo3->ido1);
            if (mozgo3->ido2 != NULL) free(mozgo3->ido2);
            free(mozgo3);
            mozgo3 = mozgo4;
        }
        free(mozgo->nev);
        free(mozgo);
        mozgo = mozgo2;
    }
    free(metro);
}

bool is_megallo_p_on_metro(Metro *metro, Megallo *megallo) {
    Vonal *mozgo = metro->vonalak;
    while (mozgo != NULL) {
        Megallo *mozgo2 = mozgo->megallo;
        while (mozgo2 != NULL) {
            if (megallo == mozgo2) {
                return true;
            }
            mozgo2 = mozgo2->kovetkezo;
        }
        mozgo = mozgo->kovetkezo;
    }
    return false;
}

void free_utvonalterv(Utvonalterv *utvonalterv) {
    Utvonalterv *mozgo = utvonalterv;
    while (mozgo != NULL) {
        Utvonalterv *mozgo2 = mozgo->kovetkezo;
        free(mozgo->indulasiIdo);
        free(mozgo->erkezesiIdo);
        free(mozgo);
        mozgo = mozgo2;
    }
}
void free_vonals(Vonal *vonal) {
    Vonal *mozgo = vonal;
    while (mozgo != NULL) {
        Vonal *mozgo2 = mozgo->kovetkezo;
        free(mozgo);
        mozgo = mozgo2;
    }
}

void free_atszallasi_megallok(AtszallasiMegallo **atszallasiMegallok,
                              int size) {
    for (int i = 0; i < size; i++) {
        AtszallasiMegallo *mozgo = atszallasiMegallok[i];
        while (mozgo != NULL) {
            AtszallasiMegallo *mozgo2 = mozgo->kovetkezo;
            free(mozgo->megallo);
            free(mozgo->vonal);
            free(mozgo);
            mozgo = mozgo2;
        }
    }
    free(atszallasiMegallok);
}