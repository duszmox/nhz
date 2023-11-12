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
    MEGALLO_SELECTOR
} typedef menu_type;

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
typedef struct MegalloList {
    Megallo *megallo;
    int size;
} MegalloList;

typedef struct SearchKey {
    char *key;
    int size;
} SearchKey;

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
    char *indulasiIdo = utvonalterv->indulasiIdo == NULL ? "Nincs megadva" : "";

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

Menu *gen_megallo_selector_menu(Metro *metro, Menu *parent, char *searchKey) {
    Megallo *megallok = megallo_search(metro, searchKey);
    sort_megallo_array(megallok);
    MegalloList *megalloList = malloc(sizeof(MegalloList));
    megalloList->megallo = megallok;
    megalloList->size = count_megallok(megallok);
    Menu *megallo_selector_menu = malloc(sizeof(Menu));
    megallo_selector_menu->items = malloc(sizeof(MenuItem) * 4);
    megallo_selector_menu->parent = parent;
    megallo_selector_menu->size = 4;
    megallo_selector_menu->selected = 0;
    megallo_selector_menu->accepts_input = true;
    megallo_selector_menu->type = MEGALLO_SELECTOR;
    // set the items to the 4 megallos closest to the selected one

    return megallo_selector_menu;
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

    current_menu = &main_menu;
    int ch;
    Utvonalterv utvonalterv = {
        NULL, NULL, NULL, NULL, 0, NULL,
    };
    bool utvonalterv_alloced = false;
    Metro *metro = {NULL, 0};
    SearchKey searchKey = {NULL, 0};
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
        mvprintw(0, 0, "Selected: %d", selected);
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
            if (current_menu->size == selected) {
                attron(A_REVERSE);
            }
            mvprintw(current_menu->size + 1, 0, "Vissza");
            if (current_menu->size == selected) {
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
            mvprintw(current_menu->size + 2, 0, "Keresés: %s", searchKey.key);
        }

        ch = getch();
        if (ch == KEY_UP) {
            if (current_menu->type == MEGALLO_SELECTOR) {
                if (current_menu->selected != 0) {
                    current_menu->selected--;
                } else {
                    decrease_selected(&selected, current_menu->size + 1);
                }
            } else {
                decrease_selected(&selected, current_menu->size + 1);
            }
        } else if (ch == KEY_DOWN) {
            if (current_menu->type == MEGALLO_SELECTOR) {
                if (current_menu->selected !=
                    count_megallok(megallo_search(metro, searchKey.key))) {
                    current_menu->selected++;
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
                    current_menu = current_menu->parent;
                    selected = 0;
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
                        }
                    }

                } else if (current_menu->type == MENETREND_MENU1) {
                    if (selected == 0) {
                        gen_m();
                    }
                } else if (current_menu->type == MENETREND_MENU2) {
                    if (selected == 0) {
                        remove("menetrend.csv");
                        *metro = (Metro){NULL, 0};
                    }
                    if (selected == 1) {
                        gen_m();
                    }
                } else if (current_menu->type == UTVALTERV_MENU) {
                    if (selected == 0) {
                        metro = menetrend_beolvas();
                        searchKey.key = malloc(sizeof(char));
                        searchKey.size = 0;
                        current_menu = gen_megallo_selector_menu(
                            metro, current_menu, searchKey.key);
                    }
                } else if (current_menu->type == MEGALLO_SELECTOR) {
                    if (selected == 0) {
                    }
                }
            }
        } else if (current_menu->accepts_input) {
            if (ch == KEY_BACKSPACE || ch == KEY_DC || ch == 127) {
                if (searchKey.size > 0) {
                    searchKey.key = realloc(
                        searchKey.key, sizeof(char) * strlen(searchKey.key) -
                                           sizeof(char) + 1);
                    searchKey.key[strlen(searchKey.key) - 1] = '\0';
                    searchKey.size--;
                }
            } else {
                searchKey.key = realloc(
                    searchKey.key,
                    sizeof(char) * strlen(searchKey.key) + sizeof(char) + 1);
                searchKey.key[strlen(searchKey.key)] = ch;
                searchKey.key[strlen(searchKey.key) + 1] = '\0';
                searchKey.size++;
            }
        }
        // clear the screen
        clear();
    }

    endwin();  // Cleanup ncurses before exiting

    return 0;
}
