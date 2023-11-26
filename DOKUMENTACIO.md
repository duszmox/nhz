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

A második menüpont függvényeit tartalmazza. Beolvassa a menetrend csv file-t. Definiálja a program fő adatainak struktúráját. Keres a megállók között, hosszokat állapít meg két megálló között, és egyéb más apróbb függvényeket tartalmaz, amik elősegítik az útvonaltervezést.

### utvonalterv | definiált típusok

#### Megallo

```c
typedef struct Megallo {
    char *nev; // megálló neve
    struct Megallo *kovetkezo; // következő megálló a láncolt listában
    struct Megallo *elozo; // előző megálló a láncolt listában
    struct Idopont *ido1; // Az egyik irányba közlekedő metró megállási időpontjai
    struct Idopont *ido2; // A másik irányba közlekedő metró megállási időpontjai
    int ido1Hossz; // Az ido1 tömb hossza
    int ido2Hossz; // Az ido2 tömb hossza
} Megallo; 
```

Duplán láncolt lista. Egy megállót leíró objektum. Tartalmazza a megálló nevét, az előző és következő elemet a láncolt listában, az egyik, illetve a másik irányba közlekedő metroszerelvény megállási időpontjait `Idopont` típusú tömbben, illetve ennek a két tömbnek a hosszát.

---

#### Vonal

```c
typedef struct Vonal {
    char *nev; // vonal neve
    struct Megallo *megallo; // a vonalon található megállók láncolt lista első eleme
    int megallokSzama; // a vonalon található megállók száma
    struct Vonal *kovetkezo; // előző vonal a metróhálózaton
    struct Vonal *elozo; // következő vonal a metróhálózaton
} Vonal;
```

Duplán láncolt fésűs lista. Egy vonalat leíró objektum. Tartalmazza a vonal nevét, a metrón található előző és következő vonal memóriacímét, a vonalon található megállók láncolt listájának első elemét, és ennek a listánaka a hosszát.

---

#### Metro

```c
typedef struct Metro {
    struct Vonal *vonalak;
    int vonalakSzama;
} Metro;
```

A metróhálózatot leíró objektum. Tartalmazza a vonalak láncolt listáját, és ennek a listának a hosszát. A legtöbb függvény egy ilyen típusú változót kér be, amikor bármit kell kezdeni a megállókkal, vagy vonalakkal.

---

#### MegalloList

```c
typedef struct MegalloList {
    struct Megallo *megallo;
    int size;
} MegalloList;
```

Egy megállókat összefogó objektum. Olyan függvények használják akik nem egymás melletti megállókat szeretnének visszaadni, például a `megallo_search` függvény. Tartalmaz egy `Megallo` típusú láncolt listának az első elemére mutató pointert és a láncolt lista hosszát.

---

#### ÁtszállásiMegallo

```c
typedef struct AtszallasiMegallo {
    struct Megallo *megallo; // a megálló ahol az átszállást el lehet végezni a vonalon
    struct Vonal *vonal; // a vonal ahonnan az átszállás történik
    struct AtszallasiMegallo *kovetkezo; // következő átszállási megálló a vonalon
} AtszallasiMegallo;
```

Láncolt lista. Egy vonalon található összes olyan megállót ahonnan át lehet szállni másik vonalakra. *(A másik megállókat, vagy a vonalakat nem adja meg, csak az adott vonalon található megállókat és vonalat.)*

---

#### Utvonalterv

```c
typedef struct Utvonalterv {
    struct Megallo *indulo; // kiinduló állomás
    struct Megallo *cel; // célállomás
    struct Idopont *indulasiIdo; // indulás időpontja
    struct Idopont *erkezesiIdo; // érkezés időpontja
    struct Vonal *vonal; // a vonal, ahol az utazás történik
    struct Utvonalterv *kovetkezo; // Átszállás a következő vonalra
} Utvonalterv;
```

Láncolt lista. Egy útvonaltervet lehet benne eltárolni. Egy útvonalterveben viszont lehetnek átszállások, ezért minden utazást egy vonalon saját elemként tekintünk a láncolt listában. Például a Forgáts Utcától az Oktogonig két elemű lenne a lista, mert az M3-al el kell menni a Deál Ferenc Tér-ig, ahol pedig át kell szállni az M1-re. Az első elemben az indulo a Forgáts Utca lenne, a cél pedig a Deák Ferenc Tér. A második a Deák Ferenc Tér-től indulna és az Oktogon lenne a cél.  
Megálló neveken kívűl azt is tárolja, hogy az egyes átszállások mikor indulnak és mikor érnek célba a kívánt állomáson.  
Illetve az is el van benne tárolva, hogy az utazást melyik vonalon kell végezzük.

### utvonalterv | függvények

```c
struct Metro *menetrend_beolvas()
```

Beolvassa a `menetrend.csv` file-t, eltárolja egy `Metro` típusú objektumban és visszadja az arra mutató pointert. *(A `menetrend.csv` file-t maga a program generálja a `void gen_menetrend(MetroGen metro)` függvénnyel)*

---

```c
bool is_on_vonal(Vonal *vonal, const char *megallo)
```

A függvény bekér egy Vonal változóra mutató pointert és egy megálló nevét, majd return értékként igazat, vagy hamisat ad, attól függően, hogy van-e ilyen nevű megálló a Vonal változó Megalló típusú láncolt listájában.

---

```c
struct MegalloList *megallo_search(Metro *metro, char *megallo_chunk) 
```

A függvény bekéri a metróhálózatot tartalmazó változót és a keresett kulcsszót (megallo_chunk), ami alapján ő összeállít egy listát az összes olyan megállóról, aminek neve tartalmazza a keresett kulcsszót. Minden megálló neve egyszer szerepel a listában. A kulcsszó nem case-sensitive, azaz kis és nagy betűtől függetlenül működik. *(Ez nem vonatkozik a több byte-on tárolt betűkre)*  
A megállók tárolására létrehoz egy `MegalloList` típusú változót és az arra mutató pointerrel tér vissza a függvény.

---

```c
struct Megallo *is_string_on_megallo_vonal(Megallo *megallo, char *megalloNev)
```

Paraméterként kap egy megállót objektumot és egy másik megálló nevét string-ként. Végigmegy a Megallo objektum láncolt listáján és ha megtalálja a megalloNev névvel rendelkező Megallo-t, akkor visszatérési értékként adja annak a Megallo-nak a memóriacímét. Ha nincs ilyen megálló, akkor NULL pointerrel tér vissza.

---

```c
int *megallo_distance(Vonal *vonal, char *megallo1, char *megallo2)
```

Paraméterként kapott Vonalon megnézi, hogy szerepel-e a további két paraméteren kapott string-gel rendelkező megálló, ha létezik, akkor egy egész számban eltárolja a távolságuk. *(Ha megallo1 előbb van a vonalon mint megallo2, akkor a távolság pozitív, ellenkező esetben negatív. Ha mindkét megálló ugyanaz, akkor a táv 0)*. Ha mindketten rajta vannak a vonalon akkor a távolság egészre mutató pointerrel tér vissza a függvény, egyéb esetben egy NULL pointer a return value.

---

```c
void sort_megallo_array(Megallo *megallok)
```

Paraméterként kapott Megallo típusú láncolt listát sort-ol a megállók neve szerint, ABC sorrendben. Visszatérési értéke void, mert a paramétert változtatja.

---

```c
int count_megallok(Megallo *megallok)
```

Paraméterként kapott Megallo típusú láncolt lista hosszát adja vissza egész típusú változóként.

---

```c
struct Vonal *find_vonal_for_megallo_string(struct Metro *metro, const char *megalloNev)
```

Paraméterként kapott megálló névhez megkeresi az összes vonalat a metróhálózaton, ahol szerepel ilyen nevű megálló. Visszatérési értéke egy Vonal típusú láncolt lista az összes vonallal, ahol van ilyen nevű megálló.

---

```c
struct AtszallasiMegallo *atszallasi_megallok_on_vonal(struct Metro *Metro, struct Vonal *vonal)
```

Paraméterként kapott vonalon megkeresi az összes megállót, ahol át lehet szállni a metróhálózaton található többi vonal egyikére. A visszatérési értéke egy `AtszallasiMegallo` típusú láncolt lista, ami tartalmazza a vonalon található összes átszállási megállót. Ennek a formátuma fentebb van kifejtve az `utvonalterv | definiált típusok` heading alatt.

---

```c
struct Vonal *are_megallok_on_same_vonal_string(Metro *metro,
                                                const char *megallo1,
                                                const char *megallo2)
```

Paraméterként kapott kettő megálló neve alapján megnézi, hogy van-e olyan vonal a metróhálózaton, ahol mindeketten rajta vannak. Ha van, akkor visszaadja annak pointerét, ha nem, akkor pedig egy NULL pointerrel tér vissza.

---

```c
struct Megallo *find_megallo_for_string(Metro *metro, const char *megalloNev)
```

Paraméterként kapott megálló névhez keres a metróhálózaton egy Megállót, aminek ugyanaz a neve. Ha talál, annak a Megállónak a memóriacímével tér vissza, ha nem, akkor egy NULL pointerrel.

---

```c
void free_megallo_list(MegalloList *megalloList)
```

Paraméterként kapott `MegalloList` típusú láncolt lista minden elemét és elemeinek változóit is felszabadítja, végül pedig magát is.

---

```c
void free_metro_network(Metro *metro)
```

Paraméterként kapott `Metro` típusú metróhálózat minden vonalát, megállóját, és azok egyes változóit is felszabadítja, végül pedig magát is.

---

```c
void free_utvonalterv(Utvonalterv *utvonalterv)
```

Paraméterként kapott `Utvonalterv` típusú láncolt lista minden elemét és annak változóit szabadítja fel.

---

```c
void free_vonals(Vonal *vonal)
```

Paraméterként kapott `Vonal` típusú láncolt lista minden elemét és annak változóit szabadítja fel.

---

```c
void free_atszallasi_megallok(AtszallasiMegallo **atszallasiMegallok,
                              int size)
```

Paraméterként kapott `AtszallasiMegallo` típusú láncolt listák listájának minden láncolt listáját és azoknak az elemeit is felszabadítja.

## menu.c

Jelenleg ez tartalmazza a menü-t, amin keresztül lehet interaktálni az adatokkal. Egyenlőre csak ncureses-el működik, ami windows operációs rendszeren nem elérhető library, így csak linuxon és macOS-es futtatható.

## metro_transportus.c

Egyenlőre nincs jelentősége a file-nak, majd ez lesz a belépőpontja az app-nak
