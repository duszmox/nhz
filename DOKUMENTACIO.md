# Programozói Dokumentáció

## idopontok

Header file. Az időpont management-tel foglalkozik. Definiálja egy időpont változó struktúráját, és a hozzákapcsolódó függvényeket tartaltalmazza.

### Definiált típusok

#### Időpont

```c
typedef struct Idopont { // Időpont típus
    int ora; // óra (0-23)
    int perc; // perc (0-59)
} Idopont;
```

Időpontok tárolására használható

---

#### Időpont típus enum

```c
typedef enum idopont_tipus { ORA, PERC } idopont_tipus;
```

A nullával bővítő függvény használja, hogy tudja, hogy amit éppen bővít az óra-e vagy perc.

### Függvények

```c
Idopont idopont_osszead(Idopont idopont1, Idopont idopont2);
```

Ez a függvény két Idopont típusú objektumot kap bemenetként (idopont1 és idopont2), majd összeadja az órákat és perceket. Az eredményül kapott időpontot visszaadja a függvény.

---

```c
char *idopont_to_string(Idopont idopont);
```

Ez a függvény egy Idopont típusú objektumot kap bemenetként (idopont), ellenőrzi, hogy az óra és perc értékek megfelelő értékekkel rendelkeznek-e , majd visszatérési értékként létrehoz egy időpontot ábrázoló dinamikusan fogalt karakterláncot.

---

```c
char *bovit_nullaval(Idopont idopont, idopont_tipus it)
```

Ez a függvény egy Idopont típusú objektumot (idopont) és egy idopont_tipus típusú enum értéket (it) kap bemenetként. A függvény létrehoz egy karakterláncot, amely a megfelelő típusú időegységet (óra vagy perc) tartalmazza, és hozzáad egy nullát az érték elé, ha az kevesebb, mint 10. A karakterláncot dinamikusan allokálja és adja vissza.

---

```c
Idopont string_to_idopont(char *s)
```

Ez a függvény egy karakterláncot (s) kap bemenetként, amely egy időpontot reprezentál "óra:perc" formátumban. A függvény létrehoz egy Idopont objektumot (amelynek mindkét értéke 0, arra az esetre ha mégse sikerülne beolvasni), majd az sscanf függvény segítségével beolvassa a karakterláncból az óra és perc értékeket, végül visszaadja az így létrehozott Idopont objektumot.

---

```c
bool ido_kisebb(Idopont ido1, Idopont ido2)
```

Ez a függvény összehasonlít két Idopont típusú objektumot, ido1-et és ido2-t. A függvény igazat (true) ad vissza, ha az első időpont (ido1) korábbi, mint a második időpont (ido2), és hamisat (false) ad vissza, ha az első időpont későbbi vagy ugyanaz, mint a második időpont.

---

```c
int ido_kulonbseg_perc(Idopont ido1, Idopont ido2)
```

Ez a függvény kiszámolja két időpont közötti különbséget percekben. Ezt int-ként adja vissza.

## menetrend

Az első menüpont függvényeit tartalmazza. Beolvassa a megállók csv file-ból a megállókat és legenerálja a menetrendet.

## utvonalterv

A második menüpont függvényeit tartalmazza. Beolvassa a menetrend csv file-t. Definiálja a program fő adatainat struktúráját. Keres a megállók között, hosszokat állapít meg két megálló között, és egyéb más apróbb függvényeket tartalmaz, amik elősegítik az útvonaltervezést.

## menu.c

Jelenleg ez tartalmazza a menü-t, amin keresztül lehet interaktálni az adatokkal. Egyenlőre csak ncureses-el működik, ami windows operációs rendszeren nem elérhető library, így csak linuxon és macOS-es futtatható.

## metro_transportus.c

Egyenlőre nincs jelentősége a file-nak, majd ez lesz a belépőpontja az app-nak
