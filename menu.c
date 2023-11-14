#include <locale.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef WIN32
#include <io.h>
#define F_OK 0
#define access _access
#endif

#include "megallok.h"
#include "utvonalterv.h"

enum menus {
    MAIN_MENU,
    MENETREND_MENU1,
    MENETREND_MENU2,
    UTVALTERV_MENU,
    MEGALLO_SELECTOR,
    IDOPONT_SELECTOR,
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

typedef struct Utvonalterv {
    Megallo *indulo;
    Megallo *cel;
    Idopont *indulasiIdo;
    Idopont *erkezesiIdo;
    int atszallasokSzama;
    struct Utvonalterv *kovetkezo;
} Utvonalterv;

typedef struct AString {
    char *key;
    int size;
} AString;

void gen_m();

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
    char *indulo =
        malloc(sizeof(char) * strlen(induloMegallo) + 1 + strlen("Induló: ()"));
    sprintf(indulo, "Induló: (%s)", induloMegallo);
    utvonalterv_menu->items[0].text = malloc(sizeof(char) * strlen(indulo) + 1);
    strcpy(utvonalterv_menu->items[0].text, indulo);
    char *celMegallo =
        utvonalterv->cel == NULL ? "Nincs megadva" : utvonalterv->cel->nev;
    char *cel =
        malloc(sizeof(char) * strlen(celMegallo) + 1 + strlen("Cél: ()"));
    sprintf(cel, "Cél: (%s)", celMegallo);
    utvonalterv_menu->items[1].text = malloc(sizeof(char) * strlen(cel) + 1);
    strcpy(utvonalterv_menu->items[1].text, cel);
    char *indulasiIdo = utvonalterv->indulasiIdo == NULL
                            ? "Nincs megadva"
                            : idopont_to_string(*utvonalterv->indulasiIdo);

    char *indulasiIdoText = malloc(sizeof(char) * strlen(indulasiIdo) + 1 +
                                   strlen("Indulási idő: ()"));
    sprintf(indulasiIdoText, "Indulási idő: (%s)", indulasiIdo);
    utvonalterv_menu->items[2].text =
        malloc(sizeof(char) * strlen(indulasiIdoText) + 1);
    strcpy(utvonalterv_menu->items[2].text, indulasiIdoText);
    char *tervezes = "Tervezés";
    utvonalterv_menu->items[3].text =
        malloc(sizeof(char) * strlen(tervezes) + 1);
    strcpy(utvonalterv_menu->items[3].text, tervezes);
    return utvonalterv_menu;
}

Menu *gen_megallo_selector_menu(Metro *metro, Menu *parent, char *searchKey,
                                int megalloSelectorIdx) {
    MegalloList *megallok = malloc(sizeof(MegalloList));
    *megallok = *megallo_search(metro, searchKey);
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
        megallo_selector_menu->items[0].text =
            malloc(sizeof(char) * strlen("Nincs találat") + 1);
        strcpy(megallo_selector_menu->items[0].text, "Nincs találat");
        megallo_selector_menu->items[1].text = malloc(sizeof(char));
        megallo_selector_menu->items[1].text[0] = '\0';
        megallo_selector_menu->items[2].text = malloc(sizeof(char));
        megallo_selector_menu->items[2].text[0] = '\0';
        megallo_selector_menu->items[3].text = malloc(sizeof(char));
        megallo_selector_menu->items[3].text[0] = '\0';

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
            megallo_selector_menu->items[i].text =
                malloc(sizeof(char) * strlen(mozgo->nev) + 1);
            strcpy(megallo_selector_menu->items[i].text, mozgo->nev);
            mozgo = mozgo->kovetkezo;
            i++;
        }
        if (i < 4) {
            for (int j = i; j < 4; j++) {
                megallo_selector_menu->items[j].text = malloc(sizeof(char));
                megallo_selector_menu->items[j].text[0] = '\0';
            }
        }

        return megallo_selector_menu;
    }
}

int main() {
    init_ncurses();
    Menu *current_menu;
    int selected = 0;
    Menu main_menu = {
        MAIN_MENU, (MenuItem[]){{"Menetrend kezelése"}, {"Útvonaltervezés"}},
        0,         NULL,
        2,         false,
    };
    Menu menetrend_menu1_no_menetrend = {
        MENETREND_MENU1,
        (MenuItem[]){{"Menetrend Generálása"}},
        0,
        &main_menu,
        1,
        false,
    };
    Menu menetrend_menu1_menetrend = {
        MENETREND_MENU2,
        (MenuItem[]){{"Menetrend Törlése"}, {"Menetrend Újragenerálása"}},
        0,
        &main_menu,
        2,
        false,
    };
    Menu idopont_selector_menu = {
        IDOPONT_SELECTOR, (MenuItem[]){}, 0, NULL, 0, true,
    };
    current_menu = &main_menu;
    int ch;
    Utvonalterv utvonalterv = {
        NULL, NULL, NULL, NULL, 0, NULL,
    };
    bool utvonalterv_alloced = false;
    Metro *metro = NULL;
    AString searchKey = {NULL, 0};
    int megalloSelectorIdx = 0;
    AString idopont = {NULL, 0};
    selector_type selectorType;
    while (1) {
        if (access("menetrend.csv", F_OK) == 0) {
            if (utvonalterv_alloced) {
                free(main_menu.items[1].text);
                utvonalterv_alloced = false;
            }
            main_menu.items[1].text =
                malloc(sizeof(char) * strlen("Útvonaltervezés") + 1);
            strcpy(main_menu.items[1].text, "Útvonaltervezés");
            utvonalterv_alloced = true;
        } else {
            if (utvonalterv_alloced) {
                free(main_menu.items[1].text);
                utvonalterv_alloced = false;
            }
            main_menu.items[1].text =
                malloc(sizeof(char) * strlen("Ú̶t̶v̶o̶n̶a̶l̶t̶e̶r̶v̶e̶z̶é̶s̶") + 1);
            strcpy(main_menu.items[1].text, "Ú̶t̶v̶o̶n̶a̶l̶t̶e̶r̶v̶e̶z̶é̶s̶");
            utvonalterv_alloced = true;
        }
        mvprintw(0, 0, "Selected: %d %d", megalloSelectorIdx, selected);
        if (current_menu == &menetrend_menu1_no_menetrend &&
            access("menetrend.csv", F_OK) == 0) {
            current_menu = &menetrend_menu1_menetrend;
        }
        if (current_menu == &menetrend_menu1_menetrend &&
            access("menetrend.csv", F_OK) != 0) {
            current_menu = &menetrend_menu1_no_menetrend;
        }
        for (int i = 0; i < current_menu->size; i++) {
            if (i == selected) {
                attron(A_REVERSE);
            }
            mvprintw(i + 1, 0, "%s", current_menu->items[i].text);
            if (i == selected) {
                attroff(A_REVERSE);
            }
        }
        if (current_menu->parent != NULL) {
            int back_index =
                current_menu->type == MEGALLO_SELECTOR
                    ? count_megallok(
                          megallo_search(metro, searchKey.key)->megallo) < 4
                          ? current_menu->size
                          : 4
                    : current_menu->size;
            if (back_index == selected) {
                attron(A_REVERSE);
            }
            mvprintw(back_index + 1, 0, "Vissza");
            if (back_index == selected) {
                attroff(A_REVERSE);
            }
        } else {
            if (current_menu->size == selected) {
                attron(A_REVERSE);
            }
            mvprintw(current_menu->size + 1, 0, "Kilépés");
            if (current_menu->size == selected) {
                attroff(A_REVERSE);
            }
        }
        if (current_menu->accepts_input) {
            switch (current_menu->type) {
                case MEGALLO_SELECTOR:
                    mvprintw(current_menu->size + 2, 0, "Keresés: %s",
                             searchKey.key);
                    break;
                case IDOPONT_SELECTOR:
                    mvprintw(current_menu->size, 0, "Időpont (óó:pp): %s",
                             idopont.key);
                    break;
                default:
                    break;
            }
        }

        ch = getch();
        if (ch == KEY_UP) {
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
                    current_menu = gen_megallo_selector_menu(
                        metro, current_menu->parent, searchKey.key,
                        megalloSelectorIdx);
                } else {
                    decrease_selected(&selected, current_menu->size + 1);
                }
            } else {
                decrease_selected(&selected, current_menu->size + 1);
            }
        } else if (ch == KEY_DOWN) {
            if (current_menu->type == MEGALLO_SELECTOR) {
                if (current_menu->size < 4) {
                    increase_selected(&selected, current_menu->size + 1);
                    if (selected == 0) {
                        megalloSelectorIdx = 0;
                    } else {
                        if (megalloSelectorIdx < current_menu->size - 1)
                            megalloSelectorIdx++;
                    }
                } else if (megalloSelectorIdx !=
                               count_megallok(
                                   megallo_search(metro, searchKey.key)
                                       ->megallo) -
                                   4 &&
                           selected != 4) {
                    current_menu->selected++;
                    megalloSelectorIdx++;
                    current_menu = gen_megallo_selector_menu(
                        metro, current_menu->parent, searchKey.key,
                        megalloSelectorIdx);
                } else {
                    increase_selected(&selected, current_menu->size + 1);
                }
            } else {
                increase_selected(&selected, current_menu->size + 1);
            }
        } else if (ch == 'q') {
            break;
        } else if (ch == '\n') {
            if (selected == current_menu->size) {
                if (current_menu->parent != NULL) {
                    if (current_menu->type == MEGALLO_SELECTOR) {
                        megalloSelectorIdx = 0;
                    } else if (current_menu->type == IDOPONT_SELECTOR) {
                        if (idopont.size == 5) {
                            utvonalterv.indulasiIdo = malloc(sizeof(Idopont));
                            *utvonalterv.indulasiIdo =
                                string_to_idopont(idopont.key);
                            current_menu = gen_utvonalmenu(
                                &utvonalterv, current_menu->parent->parent);
                            selected = 0;
                            idopont.key = malloc(sizeof(char));
                            idopont.size = 0;
                            current_menu->selected = 0;
                            continue;
                        }
                    }
                    if (current_menu->type == UTVALTERV_MENU) {
                        utvonalterv =
                            (Utvonalterv){NULL, NULL, NULL, NULL, 0, NULL};
                    }
                    current_menu = current_menu->parent;
                    selected = 0;
                    selectorType = NOT_SELECTED;
                } else {
                    break;
                }
            } else {
                if (current_menu->type == MAIN_MENU) {
                    if (selected == 0) {
                        current_menu = &menetrend_menu1_no_menetrend;
                        selected = 0;
                    }
                    if (selected == 1) {
                        if (access("menetrend.csv", F_OK) == 0) {
                            current_menu =
                                gen_utvonalmenu(&utvonalterv, &main_menu);
                            selected = 0;
                            metro = menetrend_beolvas();
                            utvonalterv =
                                (Utvonalterv){NULL, NULL, NULL, NULL, 0, NULL};
                        }
                    }

                } else if (current_menu->type == MENETREND_MENU1) {
                    if (selected == 0) {
                        gen_m();
                    }
                } else if (current_menu->type == MENETREND_MENU2) {
                    if (selected == 0) {
                        remove("menetrend.csv");
                        metro = NULL;
                    }
                    if (selected == 1) {
                        gen_m();
                    }
                } else if (current_menu->type == UTVALTERV_MENU) {
                    if (selected == 0) {
                        searchKey.key = malloc(sizeof(char));
                        searchKey.size = 0;
                        megalloSelectorIdx = 0;
                        current_menu = gen_megallo_selector_menu(
                            metro, current_menu, searchKey.key,
                            megalloSelectorIdx);
                        selectorType = INDULO;
                        selected = 0;
                    }
                    if (selected == 1) {
                        searchKey.key = malloc(sizeof(char));
                        searchKey.size = 0;
                        megalloSelectorIdx = 0;
                        current_menu = gen_megallo_selector_menu(
                            metro, current_menu, searchKey.key,
                            megalloSelectorIdx);
                        selectorType = CEL;
                        selected = 0;
                    }
                    if (selected == 2) {
                        idopont.key = malloc(sizeof(char));
                        idopont.size = 0;
                        idopont_selector_menu.parent = current_menu;
                        current_menu = &idopont_selector_menu;
                        selected = 0;
                    }

                } else if (current_menu->type == MEGALLO_SELECTOR) {
                    Megallo *megallo =
                        megallo_search(metro, searchKey.key)->megallo;
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
                            utvonalterv.indulo = megallo;
                            break;
                        case CEL:
                            utvonalterv.cel = megallo;
                            break;
                        default:
                            break;
                    }
                    selected = 0;
                    megalloSelectorIdx = 0;
                    current_menu = gen_utvonalmenu(
                        &utvonalterv, current_menu->parent->parent);
                    selectorType = NOT_SELECTED;
                }
            }
        } else if (current_menu->accepts_input) {
            if (ch == KEY_BACKSPACE || ch == KEY_DC || ch == 127) {
                switch (current_menu->type) {
                    case MEGALLO_SELECTOR:
                        if (searchKey.size > 0) {
                            searchKey.key =
                                realloc(searchKey.key,
                                        sizeof(char) * strlen(searchKey.key) -
                                            sizeof(char) + 1);
                            searchKey.key[strlen(searchKey.key) - 1] = '\0';
                            searchKey.size--;
                            megalloSelectorIdx = 0;
                            current_menu->selected = 0;
                            selected = 0;
                            current_menu = gen_megallo_selector_menu(
                                metro, current_menu->parent, searchKey.key,
                                megalloSelectorIdx);
                        }
                        break;
                    case IDOPONT_SELECTOR:
                        if (idopont.size > 0) {
                            idopont.key =
                                realloc(idopont.key,
                                        sizeof(char) * strlen(idopont.key) -
                                            sizeof(char) + 1);
                            idopont.key[strlen(idopont.key) - 1] = '\0';
                            idopont.size--;
                        }
                        break;

                    default:
                        break;
                }
            } else {
                switch (current_menu->type) {
                    case MEGALLO_SELECTOR:
                        searchKey.key =
                            realloc(searchKey.key,
                                    sizeof(char) * strlen(searchKey.key) +
                                        sizeof(char) + 1);
                        searchKey.key[strlen(searchKey.key)] = ch;
                        searchKey.key[strlen(searchKey.key) + 1] = '\0';
                        searchKey.size++;
                        megalloSelectorIdx = 0;
                        current_menu->selected = 0;
                        selected = 0;
                        current_menu = gen_megallo_selector_menu(
                            metro, current_menu->parent, searchKey.key,
                            megalloSelectorIdx);
                        break;
                    case IDOPONT_SELECTOR:
                        switch (idopont.size) {
                            case 0:
                                if (ch >= '0' && ch <= '2') {
                                    idopont.key = realloc(idopont.key,
                                                          sizeof(char) * 2 + 1);
                                    idopont.key[0] = ch;
                                    idopont.key[1] = '\0';
                                    idopont.size++;
                                }
                                break;
                            case 1:
                                if (idopont.key[0] == '0' && ch >= '0' &&
                                    ch <= '9') {
                                    idopont.key = realloc(idopont.key,
                                                          sizeof(char) * 3 + 1);
                                    idopont.key[1] = ch;
                                    idopont.key[2] = '\0';
                                    idopont.size++;
                                } else if (idopont.key[0] == '1' && ch >= '0' &&
                                           ch <= '9') {
                                    idopont.key = realloc(idopont.key,
                                                          sizeof(char) * 3 + 1);
                                    idopont.key[1] = ch;
                                    idopont.key[2] = '\0';
                                    idopont.size++;
                                } else if (idopont.key[0] == '2' && ch >= '0' &&
                                           ch <= '3') {
                                    idopont.key =
                                        realloc(idopont.key, sizeof(char) * 3 +
                                                                 sizeof(char));
                                    idopont.key[1] = ch;
                                    idopont.key[2] = '\0';
                                    idopont.size++;
                                }
                                break;
                            case 2:
                                if (ch == ':') {
                                    idopont.key =
                                        realloc(idopont.key, sizeof(char) * 4 +
                                                                 sizeof(char));
                                    idopont.key[2] = ch;
                                    idopont.key[3] = '\0';
                                    idopont.size++;
                                }
                                break;
                            case 3:
                                if (ch >= '0' && ch <= '5') {
                                    idopont.key =
                                        realloc(idopont.key, sizeof(char) * 5 +
                                                                 sizeof(char));
                                    idopont.key[3] = ch;
                                    idopont.key[4] = '\0';
                                    idopont.size++;
                                }
                                break;
                            case 4:
                                if (ch >= '0' && ch <= '9') {
                                    idopont.key =
                                        realloc(idopont.key, sizeof(char) * 6 +
                                                                 sizeof(char));
                                    idopont.key[4] = ch;
                                    idopont.key[5] = '\0';
                                    idopont.size++;
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
