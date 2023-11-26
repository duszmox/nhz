# Programozói Dokumentáció

## idopontok

Header file. Az időpont management-tel foglalkozik. Definiálja egy időpont változó struktúráját, és a hozzákapcsolódó függvényeket tartaltalmazza.

### idopontok | definiált típusok

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

### idopontok | függvények

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

### menetrend | definiált típusok

#### MegalloGen

```c
typedef struct MegalloGen {
    char *nev;
    struct MegalloGen *kovetkezo;
    struct MegalloGen *elozo;
} MegalloGen;
```

Duplán láncolt lista. Egy megálló nevét és a soron következő, illetve előző megálló pointerét tartalmazza.

---

#### VonalGen

```c
typedef struct VonalGen {
    char *nev;
    MegalloGen *megallo;
    int megallokSzama;
    struct VonalGen *kovetkezo;
} VonalGen;
```

Láncolt lista. Egy vonal nevét, a hozzá tartozó `MegalloGen` típusú láncolt lista első elemét, ennek a listának a számát és a következő vonalra mutató pointert tartalmazza.

---

#### MetroGen

```c
typedef struct MetroGen {
    VonalGen *vonalak;
    int vonalakSzama;
} MetroGen;
```

Ez az összefoglaló objektuma a metróhálózatnak. Tartalmazza a vonalak láncolt listájának első elemére mutató pointert, illetve azt, hogy hány eleme van annak a láncolt listának.

### menetrend | függvények

```c
struct MetroGen *vonalak_beolvas()
```

Beolvassa a `megallok.csv` file-ban található vonalakat és azok megállóit. Ezt eltárolja egy MetroGen típusú változóban, és visszaadja az erre a változóra mutató memóriacímet. Ha ugyanolyan névvel rendelkezik kettő vonal, akkor csak az egyiket olvassa be. Ha egy vonalnak nincs neve, vagy egyik megállójának nincs neve *(értsd kettő vessző van egymás után)* akkor azt a vonalat kihagyja.

---

```c
struct MegalloGen *elso_megallo(VonalGen vonal)
```

Visszaadja egy vonalon található első megállónak a memóriacímét.

---

```c
struct MegalloGen *utolso_megallo(VonalGen vonal)
```

Visszaadja egy vonalon található utolsó megállónak a memóriacímét.

---

```c
void gen_menetrend(MetroGen metro)
```

Megadott MetroGen objektumból legenerál egy menetrendet. Minden sor egy megállót tartalmaz. Az adatok a csv formátumhoz híven vesszővel vannak elválasztva. Minden sor első eleme a járat neve (pl.: M2), a második a vonal végállomásának neve (pl.: Mexikói Út), a harmadik az aktuális megálló neve ahova az indulási időpontok tartoznak (pl.: Oktogon), a további elemek pedig az indulási időpontokat jelzi (pl.: 6:55)  
Példa:

```txt
M1,Mexikói út,Vörösmarty Tér,1:01,1:06,1:11,1:16,1:21,...,23:46,23:51,23:56
M1,Vörösmarty Tér,Vörösmarty Tér,1:00,1:05,1:10,1:15,1:20,...,23:45,23:50,23:55
.
.
.
M4,Kelenföld Vasútállomás,Keti Pályaudvar,1:00,1:05,1:10,1:15,1:20,...,23:45,23:50,23:55
M4,Keleti Pályaudvar,Keti Pályaudvar,1:00,1:05,1:10,1:15,1:20,...,23:45,23:50,23:55
```

---

```c
void free_metro(MetroGen *metro)
```

Felszabadítja a paraméterként kapott MetroGen típusú változóban tárolt megállókat, vonalakat és magát a metróhálózat változót is.

---

```c
void del_menetrend()
```

Törli a `menetrend.csv` file-t.

## utvonalterv

A második menüpont függvényeit tartalmazza. Beolvassa a menetrend csv file-t. Definiálja a program fő adatainat struktúráját. Keres a megállók között, hosszokat állapít meg két megálló között, és egyéb más apróbb függvényeket tartalmaz, amik elősegítik az útvonaltervezést.

## menu.c

Jelenleg ez tartalmazza a menü-t, amin keresztül lehet interaktálni az adatokkal. Egyenlőre csak ncureses-el működik, ami windows operációs rendszeren nem elérhető library, így csak linuxon és macOS-es futtatható.

## metro_transportus.c

Egyenlőre nincs jelentősége a file-nak, majd ez lesz a belépőpontja az app-nak
