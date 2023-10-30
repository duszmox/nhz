#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Megallo
{
    char *nev;
    struct Megallo *kovetkezo;
    struct Megallo *elozo;
} Megallo;

typedef struct Vonal
{
    char *nev;
    Megallo *megallo;
    int megallokSzama;
    struct Vonal *kovetkezo;
} Vonal;

typedef struct Metro
{
    Vonal *vonalak;
    int vonalakSzama;
} Metro;

Metro *vonalak_beolvas()
{
    FILE *fp = fopen("megallok.csv", "r");
    if (fp == NULL)
    {
        printf("Hiba a fajl megnyitasakor!\n");
        exit(1);
    }

    Metro *metro = malloc(sizeof(Metro));
    *metro = (Metro){NULL, 0};

    int buffer_size = sizeof(char); // Initial buffer size
    char *buffer = malloc(buffer_size);
    int buffer_index = 0;

    char ch;
    while ((ch = fgetc(fp)) != EOF)
    {
        if (ch == '\n' || ch == EOF)
        {
            buffer[buffer_index] = '\0'; // Null-terminate the line
            buffer_index = 0;

            Vonal vonal = {NULL, NULL, 0, NULL};
            vonal.nev = strdup(strtok(buffer, ","));
            char *token = strtok(NULL, ",");
            while (token != NULL)
            {
                if (vonal.megallokSzama == 0)
                {
                    vonal.megallo = malloc(sizeof(Megallo));
                    vonal.megallo->nev = strdup(token);
                    vonal.megallo->kovetkezo = NULL;
                    vonal.megallo->elozo = NULL;
                }
                else
                {
                    Megallo *uj = malloc(sizeof(Megallo));
                    uj->nev = strdup(token);
                    uj->kovetkezo = NULL;
                    Megallo *mozgo = vonal.megallo;
                    while (mozgo->kovetkezo != NULL)
                    {
                        mozgo = mozgo->kovetkezo;
                    }
                    mozgo->kovetkezo = uj;
                    uj->elozo = mozgo;
                }
                token = strtok(NULL, ",");
                vonal.megallokSzama++;
            }
            if (metro->vonalak == NULL)
            {
                metro->vonalak = malloc(sizeof(Vonal));
                *metro->vonalak = vonal;
                metro->vonalak->kovetkezo = NULL;
                metro->vonalakSzama++;
            }
            else
            {
                Vonal *uj = malloc(sizeof(Vonal));
                *uj = vonal;
                uj->kovetkezo = NULL;
                Vonal *mozgo = metro->vonalak;
                while (mozgo->kovetkezo != NULL)
                {
                    mozgo = mozgo->kovetkezo;
                }
                mozgo->kovetkezo = uj;
                metro->vonalakSzama++;
            }
        }
        else
        {
            if (buffer_index == buffer_size)
            {
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
Megallo *elso_megallo(Vonal vonal)
{
    Megallo *megallo = vonal.megallo;
    while (megallo->elozo != NULL)
    {
        megallo = megallo->elozo;
    }
    return megallo;
}

Megallo *utolso_megallo(Vonal vonal)
{
    Megallo *megallo = vonal.megallo;
    while (megallo->kovetkezo != NULL)
    {
        megallo = megallo->kovetkezo;
    }
    return megallo;
}

void gen_menetrend(Metro metro)
{
    FILE *fp = fopen("menetrend.csv", "w");
    if (fp == NULL)
    {
        printf("Hiba a fajl megnyitasakor!\n");
        exit(1);
    }
    Vonal *vonal = metro.vonalak;
    while (vonal != NULL)
    {
        Megallo *elso = elso_megallo(*vonal);
        Megallo *utolso = utolso_megallo(*vonal);
        Megallo *megallo = elso;
        while (megallo != NULL)
        {
            fprintf(fp, "%s,%s,%s\n", vonal->nev, utolso->nev, megallo->nev);
            megallo = megallo->kovetkezo;
        }
        fprintf(fp, "\n");
        megallo = utolso;
        while (megallo != NULL)
        {
            fprintf(fp, "%s,%s,%s\n", vonal->nev, elso->nev, megallo->nev);
            megallo = megallo->elozo;
        }
        fprintf(fp, "\n");
        vonal = vonal->kovetkezo;
    }
    fclose(fp);
}
