#include "debugmalloc.h"
#include "megallok.h"

int main() {
    Metro *metro = vonalak_beolvas();
    printf("Vonalak szama: %d\n", metro->vonalakSzama);

    gen_menetrend(*metro);
    free_metro(metro);
    return 0;
}