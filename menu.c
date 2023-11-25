#include <locale.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#if defined(__APPLE__) || defined(__linux__)
#include <ncurses.h>
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <conio.h>
#include <io.h>
#define F_OK 0
#define access _access
#endif

#include "debugmalloc.h"
#include "megallok.h"
#include "metroGraph.h"
#include "utvonalterv.h"

enum menus {
    MAIN_MENU,
    MENETREND_MENU1,
    MENETREND_MENU2,
    UTVALTERV_MENU,
    MEGALLO_SELECTOR,
    IDOPONT_SELECTOR,
    UTVONALTERV_VISUALIZER
} typedef menu_type;

enum selector_type { INDULO, CEL, NOT_SELECTED } typedef selector_type;

typedef struct MenuItem {
    char *text;
} MenuItem;
typedef struct Menu {
    menu_type type;
    MenuItem *items;
    int selected;
    struct Menu *parent;
    int size;
    bool accepts_input;
} Menu;

typedef struct AString {
    char *key;
    int size;
} AString;

void gen_m();

void allocate_string(char **str, char const *text) {
    *str = malloc(sizeof(char) * strlen(text) + 1);
    strcpy(*str, text);
}
void increase_selected(int *selected, int size) {
    *selected = (*selected + 1) % size;
}
void decrease_selected(int *selected, int size) {
    *selected = (*selected - 1) < 0 ? size - 1 : *selected - 1;
}
void gen_m() {
    MetroGen *metro = vonalak_beolvas();
    gen_menetrend(*metro);
    free_metro(metro);
}
void init_ncurses() {
    setlocale(LC_ALL, "");
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
}

Menu *gen_utvonalmenu(Utvonalterv *utvonalterv, Menu *parent) {
    Menu *utvonalterv_menu = malloc(sizeof(Menu));
    utvonalterv_menu->items = malloc(sizeof(MenuItem) * 4);
    utvonalterv_menu->parent = parent;
    utvonalterv_menu->size = 4;
    utvonalterv_menu->selected = 0;
    utvonalterv_menu->accepts_input = false;
    utvonalterv_menu->type = UTVALTERV_MENU;
    char *induloMegallo = utvonalterv->indulo == NULL
                              ? "Nincs megadva"
                              : utvonalterv->indulo->nev;
    char *indulo = malloc(sizeof(char) * strlen(induloMegallo) + 1 +
                          strlen("Indulási állomás: ()"));
    sprintf(indulo, "Indulási állomás: (%s)", induloMegallo);
    allocate_string(&utvonalterv_menu->items[0].text, indulo);
    char *celMegallo =
        utvonalterv->cel == NULL ? "Nincs megadva" : utvonalterv->cel->nev;
    char *cel = malloc(sizeof(char) * strlen(celMegallo) + 1 +
                       strlen("Célállomás: ()"));
    sprintf(cel, "Célállomás: (%s)", celMegallo);
    allocate_string(&utvonalterv_menu->items[1].text, cel);
    char *indulasiIdo = utvonalterv->indulasiIdo == NULL
                            ? "Nincs megadva"
                            : idopont_to_string(*utvonalterv->indulasiIdo);

    char *indulasiIdoText = malloc(sizeof(char) * strlen(indulasiIdo) + 1 +
                                   strlen("Indulás időpontja: ()"));
    sprintf(indulasiIdoText, "Indulás időpontja: (%s)", indulasiIdo);
    allocate_string(&utvonalterv_menu->items[2].text, indulasiIdoText);
    allocate_string(&utvonalterv_menu->items[3].text, "Tervezés");
    free(indulo);
    free(cel);
    free(indulasiIdoText);
    if (utvonalterv->indulasiIdo != NULL) free(indulasiIdo);
    return utvonalterv_menu;
}

Menu *gen_megallo_selector_menu(Metro *metro, Menu *parent, char *searchKey,
                                int megalloSelectorIdx) {
    MegalloList *megallok = megallo_search(metro, searchKey);
    sort_megallo_array(megallok->megallo);
    Menu *megallo_selector_menu = malloc(sizeof(Menu));
    megallo_selector_menu->items = malloc(sizeof(MenuItem) * 4);
    megallo_selector_menu->parent = parent;
    megallo_selector_menu->size =
        megallok->size < 4 ? megallok->size == 0 ? 1 : megallok->size : 4;
    megallo_selector_menu->selected = megalloSelectorIdx;
    megallo_selector_menu->accepts_input = true;
    megallo_selector_menu->type = MEGALLO_SELECTOR;
    if (megallok->size == 0) {
        allocate_string(&megallo_selector_menu->items[0].text, "Nincs találat");
        free_megallo_list(megallok);
        return megallo_selector_menu;
    } else {
        Megallo *mozgo = megallok->megallo;
        if (megallo_selector_menu->selected > megallok->size - 4) {
            int i = 0;
            while (mozgo != NULL && i < megallok->size - 4) {
                mozgo = mozgo->kovetkezo;
                i++;
            }
        } else {
            int i = 0;
            while (mozgo != NULL && i < megallo_selector_menu->selected) {
                mozgo = mozgo->kovetkezo;
                i++;
            }
        }
        int i = 0;
        while (mozgo != NULL && i < 4) {
            allocate_string(&megallo_selector_menu->items[i].text, mozgo->nev);
            mozgo = mozgo->kovetkezo;
            i++;
        }
        free_megallo_list(megallok);
        return megallo_selector_menu;
    }
}
void free_menu(Menu *menu) {
    for (int i = 0; i < menu->size; i++) {
        free(menu->items[i].text);
    }
    free(menu->items);
    free(menu);
}
void add_char_to_astring(AString *astring, char ch) {
    astring->key = realloc(astring->key, sizeof(char) * (astring->size + 2));
    astring->key[astring->size] = ch;
    astring->key[astring->size + 1] = '\0';
    astring->size++;
}

void remove_last_char_from_string(AString *astring) {
    if (astring->size > 0) {
        // Find the start of the last character (assuming UTF-8)
        int i = astring->size - 1;
        while (i > 0 && (astring->key[i] & 0xC0) == 0x80) {
            --i;
        }

        // Resize the string
        astring->key = realloc(astring->key, sizeof(char) * i + 1);
        astring->key[i] = '\0';
        astring->size = i;
    }
}
void clear_astring(AString *astring) {
    free(astring->key);
    astring->key = malloc(sizeof(char));
    strcpy(astring->key, "");
    astring->size = 0;
}

Menu *utvonalterv_visualizer_menu(Utvonalterv *utvonalterv, Menu *parent) {
    Menu *menu = malloc(sizeof(Menu));
    menu->type = UTVONALTERV_VISUALIZER;
    menu->parent = parent;
    Utvonalterv *mozgo = utvonalterv;
    int size = 0;
    if (mozgo == NULL) {
        size = 1;
    } else {
        while (mozgo != NULL) {
            size += 2;  // ket megallo
            size += 3;
            mozgo = mozgo->kovetkezo;
            size += 3;
        }
        size -= 2;
    }
    menu->size = size;
    menu->items = malloc(sizeof(MenuItem) * size);
    if (size == 1) {
        allocate_string(&menu->items[0].text, "Hiba történt a tervezés során");
        menu->selected = 1;
        menu->accepts_input = false;
        return menu;
    }
    mozgo = utvonalterv;
    int i = 0;
    while (mozgo != NULL) {
        char *induloIdopont = idopont_to_string(*mozgo->indulasiIdo);
        char *indulo = malloc(sizeof(char) * strlen(mozgo->indulo->nev) + 1 +
                              strlen("O-()  - ") + strlen(mozgo->vonal->nev) +
                              strlen(induloIdopont));
        sprintf(indulo, "O-(%s) - %s %s", mozgo->vonal->nev, mozgo->indulo->nev,
                induloIdopont);
        free(induloIdopont);
        allocate_string(&menu->items[i].text, indulo);
        i++;
        allocate_string(&menu->items[i].text, "|");
        i++;
        int *megalloTav =
            megallo_distance(mozgo->vonal, mozgo->indulo->nev, mozgo->cel->nev);
        int idoKulonbseg =
            ido_kulonbseg_perc(*mozgo->indulasiIdo, *mozgo->erkezesiIdo);
        char *interText =
            malloc(sizeof(char) * (floor(log10(abs(*megalloTav))) + 1 +
                                   floor(log10(abs(idoKulonbseg))) + 1 +
                                   strlen("|--  megálló -  perc") + 1));
        sprintf(interText, "|-- %d megálló - %d perc", abs(*megalloTav),
                idoKulonbseg);
        allocate_string(&menu->items[i].text, interText);
        i++;
        allocate_string(&menu->items[i].text, "|");
        i++;
        char *celIdopont = idopont_to_string(*mozgo->erkezesiIdo);
        char *cel = malloc(sizeof(char) * strlen(mozgo->cel->nev) + 1 +
                           strlen("O-()  - ") + strlen(mozgo->vonal->nev) +
                           strlen(celIdopont));
        sprintf(cel, "O-(%s) - %s %s", mozgo->vonal->nev, mozgo->cel->nev,
                celIdopont);
        free(celIdopont);
        allocate_string(&menu->items[i].text, cel);
        i++;
        if (mozgo->kovetkezo != NULL) {
            allocate_string(&menu->items[i].text, "|");
            i++;
            allocate_string(&menu->items[i].text, "|-- átszállás");
            i++;
            allocate_string(&menu->items[i].text, "|");
            i++;
        }
        mozgo = mozgo->kovetkezo;
        free(indulo);
        free(cel);
        free(megalloTav);
        free(interText);
    }
    allocate_string(&menu->items[i].text, "");
    menu->selected = size;
    menu->accepts_input = false;

    menu->selected = 1;
    menu->accepts_input = false;
    return menu;
}

void print_header() {
#if defined(__APPLE__) || defined(__linux__)
    mvprintw(0, 0,
             "   __  ________________  ____    _________  ___   _  _________  "
             "____  "
             "___ ________  ______");
    mvprintw(1, 0,
             "  /  |/  / __/_  __/ _ \\/ __ \\  /_  __/ _ \\/ _ | / |/ / __/ _ "
             "\\/ "
             "__ \\/ _ /_  __/ / / / __/");
    mvprintw(
        2, 0,
        " / /|_/ / _/  / / / , _/ /_/ /   / / / , _/ __ |/    _\\ \\/ ___/ "
        "/_/ "
        "/ , _// / / /_/ _\\ \\");
    mvprintw(3, 0,
             "/_/  /_/___/ /_/ /_/|_|\\____/   /_/ /_/|_/_/ |_/_/|_/___/_/   "
             "\\____/_/|_|/_/  \\____/___/");
    mvprintw(4, 0, "\n");
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    printf(
        "   __  ________________  ____    _________  ___   _  _________  "
        "____  "
        "___ ________  ______\n");
    printf(
        "  /  |/  / __/_  __/ _ \\/ __ \\  /_  __/ _ \\/ _ | / |/ / __/ _ "
        "\\/ "
        "__ \\/ _ /_  __/ / / / __/\n");
    printf(
        " / /|_/ / _/  / / / , _/ /_/ /   / / / , _/ __ |/    _\\ \\/ ___/ "
        "/_/ "
        "/ , _// / / /_/ _\\ \\\n");
    printf(
        "/_/  /_/___/ /_/ /_/|_|\\____/   /_/ /_/|_/_/ |_/_/|_/___/_/   "
        "\\____/_/|_|/_/  \\____/___/\n");
    printf("\n");
#endif
}

int main() {
    init_ncurses();

    // Menu setup
    Menu *current_menu;
    int selected = 0;
    int megalloSelectorIdx = 0;
    selector_type selectorType;
    Menu mainMenu = {
        MAIN_MENU, (MenuItem[]){{"Menetrend kezelése"}, {"Útvonaltervezés"}},
        0,         NULL,
        2,         false,
    };
    Menu menetrendMenu1NoMenetrend = {
        MENETREND_MENU1,
        (MenuItem[]){{"Menetrend Generálása"}},
        0,
        &mainMenu,
        1,
        false,
    };
    Menu menetrendMenu1Menetrend = {
        MENETREND_MENU2,
        (MenuItem[]){{"Menetrend Törlése"}, {"Menetrend Újragenerálása"}},
        0,
        &mainMenu,
        2,
        false,
    };
    Menu idopontSelectorMenu = {
        IDOPONT_SELECTOR, (MenuItem[]){}, 0, NULL, 0, true,
    };
    current_menu = &mainMenu;

    int ch;  // character input

    Utvonalterv *utvonalterv = malloc(sizeof(Utvonalterv));
    *utvonalterv = (Utvonalterv){NULL, NULL, NULL, NULL, NULL, NULL};
    bool utvonaltervAlloced = false;
    Metro *metro = NULL;

    AString searchKey = {NULL, 0};

    AString idopont = {NULL, 0};

    int headerSize = 5;

    while (true) {
        if (access("menetrend.csv", F_OK) == 0) {
            if (utvonaltervAlloced) {
                free(mainMenu.items[1].text);
                utvonaltervAlloced = false;
            }
            allocate_string(&mainMenu.items[1].text, "Útvonaltervezés");
            utvonaltervAlloced = true;
        } else {
            if (utvonaltervAlloced) {
                free(mainMenu.items[1].text);
                utvonaltervAlloced = false;
            }
            allocate_string(&mainMenu.items[1].text, "Ú̶t̶v̶o̶n̶a̶l̶t̶e̶r̶v̶e̶z̶é̶s̶");
            utvonaltervAlloced = true;
        }
        print_header();
        if (current_menu == &menetrendMenu1NoMenetrend &&
            access("menetrend.csv", F_OK) == 0) {
            current_menu = &menetrendMenu1Menetrend;
        }
        if (current_menu == &menetrendMenu1Menetrend &&
            access("menetrend.csv", F_OK) != 0) {
            current_menu = &menetrendMenu1NoMenetrend;
        }
        for (int i = 0; i < current_menu->size; i++) {
#if defined(__APPLE__) || defined(__linux__)
            if (i == selected) attron(A_REVERSE);
            mvprintw(i + headerSize, 0, "%s", current_menu->items[i].text);
            if (i == selected) attroff(A_REVERSE);
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
            if (i == selected) _textbackground(WHITE);
            printf("%s\n", current_menu->items[i].text);
            if (i == selected) _textbackground(BLACK);
#endif
        }
        if (current_menu->parent != NULL) {
            int back_index;
            if (current_menu->type == MEGALLO_SELECTOR) {
                MegalloList *megallok = megallo_search(metro, searchKey.key);
                back_index = count_megallok(megallok->megallo) < 4
                                 ? current_menu->size
                                 : 4;
                free_megallo_list(megallok);
            } else {
                back_index = current_menu->size;
            }

#if defined(__APPLE__) || defined(__linux__)
            if (back_index == selected) attron(A_REVERSE);
            if (current_menu->type != IDOPONT_SELECTOR)
                mvprintw(back_index + headerSize, 0, "<- Vissza");
            if (back_index == selected) attroff(A_REVERSE);

#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
            if (back_index == selected) _textbackground(WHITE);
            if (current_menu->type != IDOPONT_SELECTOR) printf("<- Vissza\n");
            if (back_index == selected) _textbackground(BLACK);

#endif

        } else {
#if defined(__APPLE__) || defined(__linux__)
            if (current_menu->size == selected) attron(A_REVERSE);
            mvprintw(current_menu->size + headerSize, 0, "Kilépés");
            if (current_menu->size == selected) attroff(A_REVERSE);
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
            if (current_menu->size == selected) _textbackground(WHITE);
            printf("Kilépés\n");
            if (current_menu->size == selected) _textbackground(BLACK);
#endif
        }
        if (current_menu->accepts_input) {
            switch (current_menu->type) {
                case MEGALLO_SELECTOR:
#if defined(__APPLE__) || defined(__linux__)
                    mvprintw(current_menu->size + headerSize + 1, 0,
                             "Keresés: %s ", searchKey.key);
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
                    printf("Keresés: %s ", searchKey.key);
#endif
                    break;
                case IDOPONT_SELECTOR:
#if defined(__APPLE__) || defined(__linux__)
                    mvprintw(current_menu->size + headerSize, 0,
                             "Időpont (óó:pp): %s", idopont.key);
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
                    printf("Időpont (óó:pp): %s", idopont.key);
#endif

                    break;
                default:
                    break;
            }
        }

        ch = getch();
        if (ch == KEY_UP) {
            if (current_menu->type == UTVONALTERV_VISUALIZER) continue;
            if (current_menu->type == MEGALLO_SELECTOR) {
                if (current_menu->size < 4) {
                    decrease_selected(&selected, current_menu->size + 1);
                    if (selected == current_menu->size) {
                        megalloSelectorIdx = current_menu->size - 1;
                    } else {
                        if (selected != current_menu->size - 1)
                            megalloSelectorIdx--;
                    }
                } else if (megalloSelectorIdx != 0 && selected < 4) {
                    current_menu->selected--;
                    megalloSelectorIdx--;
                    Menu *tmp = current_menu;
                    current_menu = gen_megallo_selector_menu(
                        metro, current_menu->parent, searchKey.key,
                        megalloSelectorIdx);
                    free_menu(tmp);
                } else {
                    decrease_selected(&selected, current_menu->size + 1);
                }
            } else {
                decrease_selected(&selected, current_menu->size + 1);
            }
        } else if (ch == KEY_DOWN) {
            if (current_menu->type == UTVONALTERV_VISUALIZER) continue;
            if (current_menu->type == MEGALLO_SELECTOR) {
                MegalloList *megallok = megallo_search(metro, searchKey.key);
                if (current_menu->size < 4) {
                    increase_selected(&selected, current_menu->size + 1);
                    if (selected == 0) {
                        megalloSelectorIdx = 0;
                    } else {
                        if (megalloSelectorIdx < current_menu->size - 1)
                            megalloSelectorIdx++;
                    }
                } else if (megalloSelectorIdx !=
                               count_megallok(megallok->megallo) - 4 &&
                           selected != 4) {
                    current_menu->selected++;
                    megalloSelectorIdx++;
                    Menu *tmp = current_menu;
                    current_menu = gen_megallo_selector_menu(
                        metro, current_menu->parent, searchKey.key,
                        megalloSelectorIdx);
                    for (int i = 0; i < tmp->size; i++) {
                        free(tmp->items[i].text);
                    }
                    free(tmp->items);
                    free(tmp);
                } else {
                    increase_selected(&selected, current_menu->size + 1);
                }
                free_megallo_list(megallok);
            } else {
                increase_selected(&selected, current_menu->size + 1);
            }
        } else if (ch == '\n') {
            if (selected == current_menu->size) {
                if (current_menu->parent != NULL) {
                    if (current_menu->type == MEGALLO_SELECTOR) {
                        megalloSelectorIdx = 0;
                    } else if (current_menu->type == IDOPONT_SELECTOR) {
                        if (idopont.size == 5) {
                            utvonalterv->indulasiIdo = malloc(sizeof(Idopont));
                            *utvonalterv->indulasiIdo =
                                string_to_idopont(idopont.key);
                            Menu *tmp = current_menu->parent;
                            current_menu = gen_utvonalmenu(
                                utvonalterv, current_menu->parent->parent);
                            free_menu(tmp);
                            selected = 0;
                            clear_astring(&idopont);
                            current_menu->selected = 0;
                            continue;
                        } else {
                            clear_astring(&idopont);
                            Menu *tmp = current_menu->parent;
                            current_menu = gen_utvonalmenu(
                                utvonalterv, current_menu->parent->parent);
                            free_menu(tmp);
                            continue;
                        }
                    }
                    if (current_menu->type == UTVALTERV_MENU) {
                        free_utvonalterv(utvonalterv);
                        utvonalterv = malloc(sizeof(Utvonalterv));
                        *utvonalterv =
                            (Utvonalterv){NULL, NULL, NULL, NULL, 0, NULL};
                        clear_astring(&searchKey);
                        clear_astring(&idopont);
                        if (metro != NULL) free_metro_network(metro);
                        metro = NULL;
                    }
                    Menu *parent = current_menu->parent;
                    if (UTVALTERV_MENU == current_menu->type ||
                        MEGALLO_SELECTOR == current_menu->type ||
                        UTVONALTERV_VISUALIZER == current_menu->type) {
                        free_menu(current_menu);
                    }
                    current_menu = parent;
                    selected = 0;
                    selectorType = NOT_SELECTED;
                } else {
                    if (utvonaltervAlloced) {
                        free(mainMenu.items[1].text);
                        utvonaltervAlloced = false;
                    }
                    free_utvonalterv(utvonalterv);
                    free(searchKey.key);
                    free(idopont.key);
                    if (metro != NULL) free_metro_network(metro);
                    break;
                }
            } else {
                if (current_menu->type == MAIN_MENU) {
                    if (selected == 0) {
                        current_menu = &menetrendMenu1NoMenetrend;
                        selected = 0;
                    }
                    if (selected == 1) {
                        if (access("menetrend.csv", F_OK) == 0) {
                            current_menu =
                                gen_utvonalmenu(utvonalterv, &mainMenu);
                            selected = 0;
                            metro = menetrend_beolvas();
                            free_utvonalterv(utvonalterv);
                            utvonalterv = malloc(sizeof(Utvonalterv));
                            *utvonalterv = (Utvonalterv){NULL, NULL, NULL,
                                                         NULL, NULL, NULL};
                        }
                    }

                } else if (current_menu->type == MENETREND_MENU1) {
                    if (selected == 0) gen_m();
                } else if (current_menu->type == MENETREND_MENU2) {
                    if (selected == 0) {
                        remove("menetrend.csv");
                        if (metro != NULL) free_metro_network(metro);
                        metro = NULL;
                    }
                    if (selected == 1) gen_m();

                } else if (current_menu->type == UTVALTERV_MENU) {
                    if (selected == 0 || selected == 1) {
                        clear_astring(&searchKey);
                        megalloSelectorIdx = 0;
                        current_menu = gen_megallo_selector_menu(
                            metro, current_menu, searchKey.key,
                            megalloSelectorIdx);
                        selectorType = selected == 0 ? INDULO : CEL;
                        selected = 0;
                    }
                    if (selected == 2) {
                        clear_astring(&idopont);
                        idopontSelectorMenu.parent = current_menu;
                        current_menu = &idopontSelectorMenu;
                        selected = 0;
                    }
                    if (selected == 3) {
                        if (utvonalterv->indulo != NULL &&
                            utvonalterv->cel != NULL &&
                            utvonalterv->indulasiIdo != NULL) {
                            Utvonalterv *utvonalterv2 =
                                utvonaltervezes(metro, utvonalterv->indulo->nev,
                                                utvonalterv->cel->nev,
                                                *utvonalterv->indulasiIdo);
                            current_menu = utvonalterv_visualizer_menu(
                                utvonalterv2, current_menu);
                            selected = current_menu->size;
                            free_utvonalterv(utvonalterv2);
                        }
                    }

                } else if (current_menu->type == MEGALLO_SELECTOR) {
                    MegalloList *megallok =
                        megallo_search(metro, searchKey.key);
                    if (megallok->size == 0) {
                        continue;  // ha nincs talalat, akkor nem csinalunk
                                   // semmit
                    }
                    Megallo *megallo = megallok->megallo;
                    sort_megallo_array(megallo);
                    int i = 0;
                    while (megallo != NULL && current_menu->size < 4
                               ? i < megalloSelectorIdx
                               : i < megalloSelectorIdx + selected) {
                        megallo = megallo->kovetkezo;
                        i++;
                    }
                    switch (selectorType) {
                        case INDULO:
                            utvonalterv->indulo =
                                find_megallo_for_string(metro, megallo->nev);
                            break;
                        case CEL:
                            utvonalterv->cel =
                                find_megallo_for_string(metro, megallo->nev);
                            break;
                        default:
                            break;
                    }
                    selected = 0;
                    megalloSelectorIdx = 0;
                    selectorType = NOT_SELECTED;

                    Menu *parent = current_menu->parent->parent;
                    free_menu(current_menu->parent);
                    free_menu(current_menu);
                    current_menu = gen_utvonalmenu(utvonalterv, parent);

                    clear_astring(&searchKey);
                    free_megallo_list(megallok);
                }
            }
        } else if (current_menu->accepts_input) {
            if (ch == KEY_BACKSPACE || ch == KEY_DC || ch == 127) {
                switch (current_menu->type) {
                    case MEGALLO_SELECTOR:
                        if (searchKey.size > 0) {
                            remove_last_char_from_string(&searchKey);
                            megalloSelectorIdx = 0;
                            selected = 0;
                            Menu *tmp = current_menu;
                            current_menu = gen_megallo_selector_menu(
                                metro, current_menu->parent, searchKey.key,
                                megalloSelectorIdx);
                            free_menu(tmp);
                        }
                        break;
                    case IDOPONT_SELECTOR:
                        if (idopont.size > 0) {
                            remove_last_char_from_string(&idopont);
                        }
                        break;

                    default:
                        break;
                }
            } else {
                switch (current_menu->type) {
                    case MEGALLO_SELECTOR:
                        add_char_to_astring(&searchKey, ch);
                        megalloSelectorIdx = 0;
                        selected = 0;
                        Menu *tmp = current_menu;
                        current_menu = gen_megallo_selector_menu(
                            metro, current_menu->parent, searchKey.key,
                            megalloSelectorIdx);
                        free_menu(tmp);
                        break;
                    case IDOPONT_SELECTOR:
                        switch (idopont.size) {
                            case 0:
                                if (ch >= '0' && ch <= '2') {
                                    add_char_to_astring(&idopont, ch);
                                }
                                break;
                            case 1:
                                if ((idopont.key[0] == '0' && ch >= '0' &&
                                     ch <= '9') ||
                                    (idopont.key[0] == '1' && ch >= '0' &&
                                     ch <= '9') ||
                                    (idopont.key[0] == '2' && ch >= '0' &&
                                     ch <= '3')) {
                                    add_char_to_astring(&idopont, ch);
                                }
                                break;
                            case 2:
                                add_char_to_astring(&idopont, ':');
                                break;
                            case 3:
                                if (ch >= '0' && ch <= '5') {
                                    add_char_to_astring(&idopont, ch);
                                }
                                break;
                            case 4:
                                if (ch >= '0' && ch <= '9') {
                                    add_char_to_astring(&idopont, ch);
                                }
                                break;
                            default:
                                break;
                        }
                    default:
                        break;
                }
            }
        }
        clear();
    }

    endwin();

    return 0;
}
