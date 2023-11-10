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

typedef struct MenuItem {
    char *text;
} MenuItem;
typedef struct Menu {
    MenuItem *items;
    int selected;
    struct Menu *parent;
    int size;
} Menu;

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

int main() {
    init_ncurses();
    Menu *current_menu;
    int selected = 0;
    Menu main_menu = {
        (MenuItem[]){{"Menetrend kezelése"}, {"Útvonaltervezés"}},
        0,
        NULL,
        2,
    };
    Menu menetrend_menu1_no_menetrend = {
        (MenuItem[]){{"Menetrend Generálása"}},
        0,
        &main_menu,
        1,
    };
    Menu menetrend_menu1_menetrend = {
        (MenuItem[]){{"Menetrend Törlése"}, {"Menetrend Újragenerálása"}},
        0,
        &main_menu,
        2,
    };

    current_menu = &main_menu;
    int ch;
    while (1) {
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

        ch = getch();
        if (ch == KEY_UP) {
            decrease_selected(&selected, current_menu->size + 1);
        } else if (ch == KEY_DOWN) {
            increase_selected(&selected, current_menu->size + 1);
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
                if (current_menu == &main_menu) {
                    if (selected == 0) {
                        current_menu = &menetrend_menu1_no_menetrend;
                        selected = 0;
                    }
                } else if (current_menu == &menetrend_menu1_no_menetrend) {
                    if (selected == 0) {
                        gen_m();
                    }
                } else if (current_menu == &menetrend_menu1_menetrend) {
                    if (selected == 0) {
                        remove("menetrend.csv");
                    }
                    if (selected == 1) {
                        gen_m();
                    }
                }
            }
        }
        // clear the screen
        clear();
    }

    endwin();  // Cleanup ncurses before exiting

    return 0;
}
