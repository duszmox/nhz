#include "idopontok.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debugmalloc.h"

int idopontcmp(Idopont idopont1, Idopont idopont2) {
    if (idopont1.ora < idopont2.ora) {
        return -1;
    } else if (idopont1.ora > idopont2.ora) {
        return 1;
    } else {
        if (idopont1.perc < idopont2.perc) {
            return -1;
        } else if (idopont1.perc > idopont2.perc) {
            return 1;
        } else {
            return 0;
        }
    }
}
Idopont idopont_osszead(Idopont idopont1, Idopont idopont2) {
    Idopont idopont = idopont1;
    idopont.ora += idopont2.ora;
    idopont.perc += idopont2.perc;
    if (idopont.perc >= 60) {
        idopont.ora++;
        idopont.perc -= 60;
    }
    if (idopont.ora >= 24) {
        idopont.ora -= 24;
    }
    return idopont;
}
char *bovit_nullaval(Idopont idopont, idopont_tipus it) {
    char *s = (char *)malloc(2 * sizeof(char) + 1);

    if (it == ORA) {
        if (idopont.ora < 10) {
            sprintf(s, "0%d", idopont.ora);
        } else {
            sprintf(s, "%d", idopont.ora);
        }
    } else {
        if (idopont.perc < 10) {
            sprintf(s, "0%d", idopont.perc);
        } else {
            sprintf(s, "%d", idopont.perc);
        }
    }
    return s;
}

char *idopont_to_string(Idopont idopont) {
    if (idopont.ora < 0 || idopont.ora > 23 || idopont.perc < 0 ||
        idopont.perc > 59) {
        return NULL;
    }
    char *ora = bovit_nullaval(idopont, ORA);
    char *perc = bovit_nullaval(idopont, PERC);
    char *s = (char *)malloc(5 * sizeof(char) + 1);
    sprintf(s, "%s:%s", ora, perc);
    free(ora);
    free(perc);
    return s;
}

Idopont string_to_idopont(char *s) {
    Idopont idopont = {0, 0};
    sscanf(s, "%d:%d", &idopont.ora, &idopont.perc);
    return idopont;
}

bool ido_kisebb(Idopont ido1, Idopont ido2) {
    if (ido1.ora < ido2.ora) {
        return true;
    } else if (ido1.ora > ido2.ora) {
        return false;
    } else {
        if (ido1.perc < ido2.perc) {
            return true;
        } else {
            return false;
        }
    }
}
int ido_kulonbseg_perc(Idopont ido1, Idopont ido2) {
    int ido1_perc = ido1.ora * 60 + ido1.perc;
    int ido2_perc = ido2.ora * 60 + ido2.perc;
    return ido2_perc - ido1_perc;
}