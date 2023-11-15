
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

typedef struct AtszallasiMegallo {
    Megallo *megallo;
    Vonal *vonal;
    struct AtszallasiMegallo *kovetkezo;
} AtszallasiMegallo;

typedef struct Utvonalterv {
    Megallo *indulo;
    Megallo *cel;
    Idopont *indulasiIdo;
    Idopont *erkezesiIdo;
    int atszallasokSzama;
    struct Utvonalterv *kovetkezo;
    Vonal *vonal;
} Utvonalterv;

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
                // printf("%s\n", megalloNev);
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
        while (vonalakForMegallo != NULL) {
            i++;
            vonalakForMegallo = vonalakForMegallo->kovetkezo;
        }
        if (i > 1) {
            AtszallasiMegallo *uj = malloc(sizeof(AtszallasiMegallo));
            *uj = (AtszallasiMegallo){NULL, NULL, NULL};
            uj->megallo = mozgo;
            uj->vonal = vonal;
            uj->kovetkezo = NULL;
            if (atszallasiMegallok->megallo == NULL) {
                *atszallasiMegallok = *uj;
            } else {
                AtszallasiMegallo *mozgo2 = atszallasiMegallok;
                while (mozgo2->kovetkezo != NULL) {
                    mozgo2 = mozgo2->kovetkezo;
                }
                mozgo2->kovetkezo = uj;
            }
            uj->vonal->elozo = NULL;
            uj->vonal->kovetkezo = NULL;
        }
        mozgo = mozgo->kovetkezo;
    }
    return atszallasiMegallok;
}
int count_utvonal_distance(Metro *metro, Utvonalterv *utvonalterv) {
    int tav = 0;
    Utvonalterv *mozgo = utvonalterv;
    while (mozgo != NULL) {
        tav += *megallo_distance(
            find_vonal_for_megallo_string(metro, mozgo->indulo->nev),
            mozgo->indulo->nev, mozgo->cel->nev);
        mozgo = mozgo->kovetkezo;
    }
    return tav;
}
Utvonalterv *utvonaltervezes(Metro *metro, Utvonalterv *utvonalterv) {
    if ((utvonalterv->indulo == NULL || utvonalterv->cel == NULL) ||
        (strcmp(utvonalterv->indulo->nev, utvonalterv->cel->nev) == 0)) {
        return NULL;
    }
    Vonal *possibleInduloMegalloVonalak =
        find_vonal_for_megallo_string(metro, utvonalterv->indulo->nev);
    Vonal *possibleCelMegallokVonalak =
        find_vonal_for_megallo_string(metro, utvonalterv->cel->nev);
    // print the size of the two lists
    Vonal *mozgo = possibleInduloMegalloVonalak;
    int k = 0;
    while (mozgo != NULL) {
        k++;
        mozgo = mozgo->kovetkezo;
    }
    // find if there is an overlap between the two lists
    mozgo = possibleCelMegallokVonalak;
    int j = 0;
    while (mozgo != NULL) {
        j++;
        mozgo = mozgo->kovetkezo;
    }
    if (k == 0 || j == 0) {
        return NULL;
    }
    printf("%s: %d\n", utvonalterv->indulo->nev, k);
    printf("%s: %d\n", utvonalterv->cel->nev, j);
    mozgo = possibleInduloMegalloVonalak;
    Vonal *talaltVonalak = NULL;
    while (mozgo != NULL) {
        Vonal *mozgo2 = possibleCelMegallokVonalak;
        while (mozgo2 != NULL) {
            if (strcmp(mozgo->nev, mozgo2->nev) == 0) {
                if (talaltVonalak == NULL) {
                    talaltVonalak = malloc(sizeof(Vonal));
                    *talaltVonalak = *mozgo;
                    talaltVonalak->elozo = NULL;
                    talaltVonalak->kovetkezo = NULL;
                    break;
                } else {
                    Vonal *mozgo3 = talaltVonalak;
                    while (mozgo3->kovetkezo != NULL) {
                        mozgo3 = mozgo3->kovetkezo;
                    }
                    mozgo3->kovetkezo = malloc(sizeof(Vonal));
                    *mozgo3->kovetkezo = *mozgo;
                    mozgo3->kovetkezo->kovetkezo = NULL;
                    mozgo3->kovetkezo->elozo = mozgo3;
                    break;
                }
            }
            mozgo2 = mozgo2->kovetkezo;
        }
        mozgo = mozgo->kovetkezo;
    }
    if (talaltVonalak == NULL) {
        Vonal *mozgo = possibleInduloMegalloVonalak;
        while (mozgo != NULL) {
            AtszallasiMegallo *atszallasiMegallok =
                atszallasi_megallok_on_vonal(metro, mozgo);
            AtszallasiMegallo *mozgo2 = atszallasiMegallok;
            while (mozgo2 != NULL) {
                Utvonalterv *atszallas = malloc(sizeof(Utvonalterv));
                *atszallas = (Utvonalterv){mozgo2->megallo,
                                           utvonalterv->cel,
                                           utvonalterv->indulasiIdo,
                                           NULL,
                                           utvonalterv->atszallasokSzama + 1,
                                           NULL,
                                           mozgo2->vonal};

                Utvonalterv *uj = malloc(sizeof(Utvonalterv));
                *uj = *utvonalterv;

                uj->kovetkezo = utvonaltervezes(metro, atszallas);
                uj->cel = mozgo2->megallo;
                uj->atszallasokSzama = uj->atszallasokSzama + 1;
                free(atszallas);
                // append uj to utvonaltervek
                if (talaltVonalak == NULL) {
                    talaltVonalak = malloc(sizeof(Vonal));
                    *talaltVonalak = *mozgo;
                    talaltVonalak->elozo = NULL;
                    talaltVonalak->kovetkezo = NULL;
                    break;
                } else {
                    Vonal *mozgo3 = talaltVonalak;
                    while (mozgo3->kovetkezo != NULL) {
                        mozgo3 = mozgo3->kovetkezo;
                    }
                    mozgo3->kovetkezo = malloc(sizeof(Vonal));
                    *mozgo3->kovetkezo = *mozgo;
                    mozgo3->kovetkezo->kovetkezo = NULL;
                    mozgo3->kovetkezo->elozo = mozgo3;
                    break;
                }
            }
        }
    }
    mozgo = talaltVonalak;
    Megallo *indulo = NULL;
    Megallo *cel = NULL;
    while (mozgo != NULL) {
        Megallo *mozgo2 = mozgo->megallo;
        while (mozgo2 != NULL) {
            if (strcmp(mozgo2->nev, utvonalterv->indulo->nev) == 0) {
                indulo = mozgo2;
            }
            if (strcmp(mozgo2->nev, utvonalterv->cel->nev) == 0) {
                cel = mozgo2;
            }
            mozgo2 = mozgo2->kovetkezo;
        }
        mozgo = mozgo->kovetkezo;
    }
    if (indulo == NULL || cel == NULL) {
        return NULL;
    }
    int *megalloDistance =
        megallo_distance(talaltVonalak, indulo->nev, cel->nev);
    if (megalloDistance == NULL) {
        return NULL;
    }
    Utvonalterv *uj = malloc(sizeof(Utvonalterv));
    *uj = (Utvonalterv){NULL, NULL, NULL, NULL, 0, NULL, NULL};
    uj->indulo = indulo;
    uj->cel = cel;
    int indulasiIdoIndex = 0;
    int i = 0;
    while (i < *megalloDistance > 0 ? indulo->ido1Hossz : indulo->ido2Hossz) {
        if ((bool)ido_kisebb(*utvonalterv->indulasiIdo,
                             *(*megalloDistance > 0 ? indulo->ido1 + i
                                                    : indulo->ido2 + i))) {
            indulasiIdoIndex = i;
            break;
        }
        i++;
    }
    if (*megalloDistance > 0) {
        uj->indulasiIdo = malloc(sizeof(Idopont));
        *uj->indulasiIdo = *(indulo->ido1 + i);
        uj->erkezesiIdo = malloc(sizeof(Idopont));
        *uj->erkezesiIdo = *(cel->ido1 + i);
    } else {
        *uj->indulasiIdo = indulo->ido2[indulasiIdoIndex];
        *uj->erkezesiIdo = cel->ido2[indulasiIdoIndex];
    }
    uj->atszallasokSzama = utvonalterv->atszallasokSzama;
    uj->vonal = talaltVonalak;
    return uj;

    // Vonal *mozgo = possibleInduloMegalloVonalak;
    // Utvonalterv *utvonaltervek = malloc(sizeof(Utvonalterv));
    // int utvonaltervekSzama = 0;
    // while (mozgo != NULL) {
    //     int *megalloDistance = megallo_distance(mozgo,
    //     utvonalterv->indulo->nev,
    //                                             utvonalterv->cel->nev);
    //     if (megalloDistance != NULL) {
    //         Utvonalterv *uj = malloc(sizeof(Utvonalterv));
    //         *uj = (Utvonalterv){NULL, NULL, NULL, NULL, 0, NULL};
    //         uj->indulo = utvonalterv->indulo;
    //         uj->cel = utvonalterv->cel;
    //         int indulasiIdoIndex = 0;
    //         int i = 0;
    //         while (i < *megalloDistance > 0 ? mozgo->megallo->ido1Hossz
    //                                         : mozgo->megallo->ido2Hossz) {
    //             if ((bool)ido_kisebb(
    //                     *utvonalterv->indulasiIdo,
    //                     *(*megalloDistance > 0 ? mozgo->megallo->ido1 + i
    //                                            : mozgo->megallo->ido2 + i)))
    //                                            {
    //                 indulasiIdoIndex = i;
    //                 break;
    //             }
    //             i++;
    //         }
    //         if (*megalloDistance > 0) {
    //             uj->indulasiIdo = malloc(sizeof(Idopont));
    //             *uj->indulasiIdo = *(utvonalterv->indulo->ido1 + i);
    //             uj->erkezesiIdo = malloc(sizeof(Idopont));
    //             *uj->erkezesiIdo = *(utvonalterv->cel->ido1 + i);
    //         } else {
    //             *uj->indulasiIdo =
    //             utvonalterv->indulo->ido2[indulasiIdoIndex]; *uj->erkezesiIdo
    //             = utvonalterv->cel->ido2[indulasiIdoIndex];
    //         }
    //         uj->atszallasokSzama = utvonalterv->atszallasokSzama;
    //         return uj;
    //     } else {
    //         AtszallasiMegallo *atszallasiMegallok =
    //             atszallasi_megallok_on_vonal(metro, mozgo);
    //         // find the indulo megallo in atszallasiMegallok and remove it
    //         AtszallasiMegallo *mozgo2 = atszallasiMegallok;
    //         AtszallasiMegallo *lemarado = NULL;
    //         while (mozgo2 != NULL) {
    //             if (strcmp(mozgo2->megallo->nev, utvonalterv->indulo->nev) ==
    //                 0) {
    //                 if (lemarado == NULL) {
    //                     *atszallasiMegallok = *mozgo2->kovetkezo;
    //                 } else {
    //                     lemarado->kovetkezo = mozgo2->kovetkezo;
    //                 }
    //                 break;
    //             }
    //             mozgo2 = mozgo2->kovetkezo;
    //         }
    //         mozgo2 = atszallasiMegallok;
    //         while (mozgo2 != NULL) {
    //             Utvonalterv *atszallas = malloc(sizeof(Utvonalterv));
    //             *atszallas = (Utvonalterv){mozgo2->megallo,
    //                                        utvonalterv->cel,
    //                                        utvonalterv->indulasiIdo,
    //                                        NULL,
    //                                        utvonalterv->atszallasokSzama + 1,
    //                                        NULL};

    //             Utvonalterv *uj = malloc(sizeof(Utvonalterv));
    //             *uj = *utvonalterv;

    //             uj->kovetkezo = utvonaltervezes(metro, atszallas);
    //             uj->cel = mozgo2->megallo;
    //             uj->atszallasokSzama = uj->atszallasokSzama + 1;
    //             free(atszallas);
    //             // append uj to utvonaltervek
    //             if (utvonaltervekSzama == 0) {
    //                 *utvonaltervek = *uj;
    //                 utvonaltervekSzama++;
    //             } else {
    //                 utvonaltervek =
    //                     realloc(utvonaltervek,
    //                             sizeof(Utvonalterv) * (utvonaltervekSzama +
    //                             1));
    //                 *(utvonaltervek + utvonaltervekSzama) = *uj;
    //                 utvonaltervekSzama++;
    //             }
    //         }
    //     }
    //     free(megalloDistance);
    //     mozgo = mozgo->kovetkezo;
    // }
    // if (utvonaltervekSzama == 0) {
    //     return NULL;
    // }
    // int min = 0;
    // int i = 0;
    // while (i < utvonaltervekSzama) {
    //     if (count_utvonal_distance(metro, utvonaltervek + i) <
    //         count_utvonal_distance(metro, utvonaltervek + min)) {
    //         min = i;
    //     }
    //     i++;
    // }
    // return utvonaltervek + min;
}
