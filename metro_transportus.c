#include "debugmalloc.h"
#include "megallok.h"
#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#endif

#ifdef _WIN32
int highlight_option(int index) {
    int i;
    char menu[4][6] = {"Print", "1", "2", "Exit"};
    clrscr();
    for (i = 0; i < 4; i++) {
        if (i == index) {
            textbackground(WHITE);
            textcolor(BLACK);
            cprintf("%s\n", menu[i]);
            textbackground(BLACK);
            textcolor(WHITE);
        } else {
            printf("%s\n", menu[i]);
        }
    }
    return 0;
}
#endif

void print_title(void) {
    printf(
        "   __  ________________  ____    _________  ___   _  _________  ____  "
        "___ ________  ______\n");
    printf(
        "  /  |/  / __/_  __/ _ \\/ __ \\  /_  __/ _ \\/ _ | / |/ / __/ _ \\/ "
        "__ \\/ _ /_  __/ / / / __/\n");
    printf(
        " / /|_/ / _/  / / / , _/ /_/ /   / / / , _/ __ |/    _\\ \\/ ___/ /_/ "
        "/ , _// / / /_/ _\\ \\\n");
    printf(
        "/_/  /_/___/ /_/ /_/|_|\\____/   /_/ /_/|_/_/ |_/_/|_/___/_/   "
        "\\____/_/|_|/_/  \\____/___/\n");
    printf("\n");
}

int main() {
    MetroGen *metro = vonalak_beolvas();
    print_title();
    printf("Vonalak szama: %d\n", metro->vonalakSzama);

    gen_menetrend(*metro);
    free_metro(metro);
    // del_menetrend();
    return 0;
}