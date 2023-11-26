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

Paraméterként kap egy megállót objektumot és egy másik megálló nevét string-ként. Végigmegy a Megallo objektum láncolt listáján és ha megtalálja a megalloNev névvel rendelkező Megallo-t, akkor visszatérési értékként adja annak a Megallo-nak a memóriacímét. Ha nincs ilyen megálló, akkor `NULL` pointerrel tér vissza.

---

```c
int *megallo_distance(Vonal *vonal, char *megallo1, char *megallo2)
```

Paraméterként kapott Vonalon megnézi, hogy szerepel-e a további két paraméteren kapott string-gel rendelkező megálló, ha létezik, akkor egy egész számban eltárolja a távolságuk. *(Ha megallo1 előbb van a vonalon mint megallo2, akkor a távolság pozitív, ellenkező esetben negatív. Ha mindkét megálló ugyanaz, akkor a táv 0)*. Ha mindketten rajta vannak a vonalon akkor a távolság egészre mutató pointerrel tér vissza a függvény, egyéb esetben egy `NULL` pointer a return value.

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

Paraméterként kapott kettő megálló neve alapján megnézi, hogy van-e olyan vonal a metróhálózaton, ahol mindeketten rajta vannak. Ha van, akkor visszaadja annak pointerét, ha nem, akkor pedig egy `NULL` pointerrel tér vissza.

---

```c
struct Megallo *find_megallo_for_string(Metro *metro, const char *megalloNev)
```

Paraméterként kapott megálló névhez keres a metróhálózaton egy Megállót, aminek ugyanaz a neve. Ha talál, annak a Megállónak a memóriacímével tér vissza, ha nem, akkor egy `NULL` pointerrel.

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

## metroGraph

A metróhálózat gráf alapú ábrázolását és az útvonaltervezést végzi el. Az utvonalterv file-ban definiált függvényeket használja a súlyozott gráf felépítéséhez, majd pedig dijsktra algoritmusát lefuttatva megkeresi a legrövidebb utat a kettő megálló között.

### metroGraph | definiált típusok

#### El

```c
struct El {
    int celAllomas;
    int suly;
    struct El* kov;
}
```

Láncolt lista. A metróhálózat gráfjának egyik éle. Tartalmazza azt, hogy az adott él melyik másik gráf pontra mutat, ennek a típusa egy egész. Azt, hogy mekkora az él súlya *(Azaz hány megálló van a két megálló között)* És mivel ez egy láncolt lista, ezért tartalmazza a következő elemre mutató pointert is.

---

#### AllomasVertex

```c
struct AllomasVertex {
    int taroltMegallokSzama; // átszállási lehetőségek száma
    struct Megallo** megallok; // megállók pointereinek tömbje
    struct El* elek; // pontból mutató élek
}
```

Ez egy pont a gráfon. Tartalmazza azt, hogy ezen a ponton hány megálló van, azaz hány vonalra lehet itt átszállni. *(Például a Deák Ferenc Tér esetén ez **3**)*. Tartalmazza ezeknek a megállóknak a pointereit is, egy pointer tömbben. Illetve az összes a pontból mutató Él-nek a láncolt listájának első elemére mutató pointert.

---

#### MetroGraph

```c
struct MetroGraph {
    int allomasVSzam;
    struct AllomasVertex* tomb;
}
```

Ez a gráfnak a típusa. Tartalmazza az összes `AllomasVertex`-et egy tömbben, illetve ennek a tömbnek a méretét.

### metroGraph | függvények

```c
struct MetroGraph* createGraph(int allomasVSzam)
```

Létrehoz egy MetroGraph típusú objektumot, aminek a paraméterben megadott számú pontja van. Visszatérési érteke az a MetroGraph objektumra mutató pointer.

---

```c
void addEl(struct MetroGraph* graph, int src, int cel, int suly)
```

Hozzáad a paraméterként kapott gráf (graph), paraméterként kapott (src) pontjának az Él-ek láncolt listájához egy a paraméterként (cél) kapott pontba mutató paraméterként (suly) kapott súlyú Él-t.

---

```c
void freeGraph(struct MetroGraph* graph)
```

Paraméterként (graph) kapott gráf-ot, annak pontjait és pontjainak az éleit felszabadítja.

---

```c
int* get_allomas_vertex_by_name(struct MetroGraph* graph,
                                const char* megalloNev)
```

Paraméterként (graph) kapott gráfon megkeresi a paraméterként (megalloNev) kapott névvel rendelkező megállónak az indexét, ha van ilyen akkor egy egészre mutató pointerrel tér vissza, ha nincs, akkor egy `NULL` pointerrel.

---

```c
void resize_metro_graph(struct MetroGraph* graph)
```

Paraméterként (graph) kapott gráfot bővít plusz egy ponttal.

---

```c
int* dijkstra(struct MetroGraph* graph, int source)
```

Paraméterként (graph) kapott gráfon lefuttatja dijstkra algoritmusát a paraméterként kapott (source) pontból kiindulva. Visszatérési értéke egy egész tömb, ami `graph->allomasVSzam` hosszú, és a távolságát tartalmazza minden pontnak a kezdeti ponttól. *(Forrás: <http://cs.bme.hu/bsz2/bsz2_jegyzet.pdf> 160. oldal)*

---

```c
int minDistance(int* distance, bool* visited, int V)
```

A legrövidebb utat távolságot keresi meg (és azzal tér vissza), a még meg nem látogatott csúcsok között. (Ezt a függvényt a dijsktra algoritmus használja fel).

---

```c
int findPrevious(int* distance, struct El* edges)
```

A paraméterként kapott ("elkészült") távolságok tömbbön halad visszafele és keresi meg az adott élhez vezető úton az előzőt.

---

```c
struct Utvonalterv* dijkstra_to_utvonalterv(struct MetroGraph* graph, int* distance, int veg, int start, Idopont indulasiIdo, Metro* metro)
```

A dijsktra algoritmus által generált távolságok tömb segítségével megekersi a kiiduló (start) pontból a legrövidebb utat a cél állomáshoz (veg). Minden ponton való áthaladás egy újabb elem lesz a visszatérési értékként generált útvonaltervben. Ha nem lehet két pont közt útvonalat tervezni, akkor `NULL` pointerrel értékkel tér vissza.

---

```c
struct Utvonalterv* utvonaltervezes(Metro* metro, char* indulo, char* cel, Idopont indulasiIdo)
```

Két paraméterként kapott megállónév (indulo, cel) által reprezentált megálló között keresi meg a legrövidebb útvonalat. Ehhez felépíti a metróhálózaton található átszállási megállók és az induló illetve cél állomások segítségével a metroGraph gráfot, amin lefuttatja a dijkstra algoritmusát. Az algoritmus által generált távolságok tömbből pedig útvonaltervet generál a dijkstra_to_utvonalterv függvénnyel. Ha a két megálló között nem lehet útvonalat tervezni, akkor `NULL` értékkel tér vissza, egyéb esetben egy `Útvonalterv` típusú láncolt listávál.

## metro_transportus.c

A program belépő pontja. Ez tartalmazza a main-t, itt a menü, és innen lehet minden funkciót elérni.

### metro_transportus | definiált típusok

#### Menü típusai enum

```c
enum menus {
    MAIN_MENU,
    MENETREND_MENU1,
    MENETREND_MENU2,
    UTVALTERV_MENU,
    MEGALLO_SELECTOR,
    IDOPONT_SELECTOR,
    UTVONALTERV_VISUALIZER
} typedef menu_type;
```

Ez egy enum, amik közül lehet kiválasztani, hogy mi az adott menü típusa.

---

#### Selector típus

```c
enum selector_type { INDULO, CEL, NOT_SELECTED } typedef selector_type;
```

Ez a `MEGALLO_SELECTOR` típusú menü-re vonatkozik. Az induló és cél állomás kiválasztására ugyanazok a függvények vannak használva, egy ilyen típusú változóval lehet kiválasztani, hogy melyik változót állítsuk.

---

#### Menu item

```c
typedef struct MenuItem {
    char *text;
} MenuItem;
```

A menü egyik elemét reprezentálja.

---

#### Menu

```c
typedef struct Menu {
    menu_type type; // menü típusa
    MenuItem *items; // menü elemei
    int selected; // melyik elem van kiválasztva
    struct Menu *parent; // melyik menüből érkeztünk
    int size; // hány eleme van
    bool accepts_input; // fogad-e szöveges bemenetet
} Menu;
```

Egy menüt reprezentál. Tárolja a típusát, ami egy előre megadott enum-ból választható ki. Tárolja az elemeit egy tömbben, annka a tömbnek a méretét. Azt is, hogy épp melyik elem van kiválasztva, illetve, hogy fogad-e szöveges bemenetet a menü.

---

#### Astring

```c
typedef struct AString {
    char *key;
    int size;
} AString;
```

Ez a bekért szövegek tárolására alkalmas típus. Tartalmazza a szöveget, illetve annak méretét.

### metro_transportus | függvények

```c
void allocate_string(char **str, char const *text)
```

Paraméterként kapott (**str) pointer-nek a szövegébe tölti bele a paraméterként kapott (text) konstant szöveget.

---

```c
void increase_selected(int *selected, int size)
```

Paraméterként kapott (selected) az aktuális kiválasztott értéket növeli az aktuális menü méretéhez képest. Ha a menü méreténél nagyobbra szeretné növelni a program a kiválasztottat, akkor 0-ra visszaugrik az érték.

---

```c
void decrease_selected(int *selected, int size)
```

Paraméterként kapott (selected) az aktuális kiválasztott értéket csökkent az aktuális menü méretéhez képest. Ha 0-nál kisebbre szeretné csökkenteni a program a kiválasztottat, akkor a menü méretére visszaugrik az érték.

---

```c
void gen_m()
```

Legenerálja a `menetrend.csv` file-t.

---

```c
void init_ncurses()
```

MacOS-en és Linux-on az ncureses library-t használja a program a getch() függvényhez, ennek a library-nak van pár függvénye amit inicializásnál le kell futattni, ezek a függvények vannak ide kiszervezve.

---

```c
struct Menu *gen_utvonalmenu(Utvonalterv *utvonalterv, Menu *parent)
```

Paraméterként kapott útvonaltervhez készít egy menü objektumot, majd visszaadja annak memóriacímét.

---

```c
void free_menu(Menu *menu)
```

Felaszabadítja a paraméterként kapott menü elemeinek szövegeit, illetve magát a menüt is.

---

```c
void add_char_to_astring(AString *astring, char ch)
```

Paraméterként kapott (astring) Astring típusú változóhoz hozzáfűz egy paraméterként kapott (ch) karaktert.

---

```c
void remove_last_char_from_string(AString *astring)
```

Paraméterként kapott (astring) Astring típusú változónak törli az utolsó karakterét.

---

```c
void clear_astring(AString *astring)
```

Paraméterként kapott (astring) Astring típusú változóban tárolt értéket egyenlővé teszi egy üres string-gel.

---

```c
struct Menu *utvonalterv_visualizer_menu(Utvonalterv *utvonalterv, Menu *parent) 
```

Paraméterként kapott útvonaltervet vizualizál és egy Menu típusú változóra mutató pointerként viszaadja. *(Fontos megjegyezni, hogy amikor ezt a menüt a program megjeleníti a fel le gombok nem működnek, csak a vissza gomb kattintható)*

---

```c
void print_header()
```

Platformhoz megfelelő módon megjeleníti a kijelzőn a Metro Transportus ASCII art-ot

---

#### main()

A program az aktuális menüt egy `current_menu` nevű Menu pointerben tárolja.  
A statikus menük a main elején inicializálódnak. A kezdő menü a mainMenu lesz.

A program egy while(true) örök loop-ban fut, egészen addig amíg a kilépés gombot nem választja a felhasználó.  
A getch() függvény segítségével bekér egy karaktert a felhasználótól. Ha ez a karakter a fel, vagy a le, akkor a hozzá illő increase-/decrease_selected függvényt meghívja, ezzel változtatva a kiválasztott menüpontot.  
Az egyes menüpontok mögött rejlő function mindig a menü típusától függ. Ha a kiválasztott menüpont száma megegyezik a menü méretével, akkor mindig a vissza/kilépés funkció fog lefutni.  
Ha olyan menüben vagyunk ahol az `accepts_input` igaz értékkel rendelkezik, akkor a menühöz illő Astring típusú változóba tudunk írni.

## Első futtatás előtt

MacOS és Linux OS-eken a program az ncurses library-t használja. Ennélkül nem fog lefordulni a program.

MacOS-en az alábbi paranccsal telepíthető *(a brew package manager szükséges a parancs futtatásáshoz)*:

```sh
brew install ncurses
```

Ubuntu/Debian distro-kon ez a parancs:

```sh
sudo apt-get install libncurses5-dev libncursesw5-dev
```

Egyéb distribúciók esetén az alábbi cikket ajánlom segítségként: <https://www.cyberciti.biz/faq/linux-install-ncurses-library-headers-on-debian-ubuntu-centos-fedora/>

**Ezenkívül a compile parancs futtatásakor hozzá kell adni a parancshoz a `-lncursesw` tag-et, ennélkül nem fog rendesen működni a program.**

Megeshet még, hogy a fordító nem találja a header file-okat a telepítés után sem, ekko a `-L` és `-I` tag-eket is hozzá kell fűzni az ncurses elérési útjával együtt. Például: `-L/opt/homebrew/opt/ncurses/lib -I/opt/homebrew/opt/ncurses/include`

*A program csak MacOS-en lett tesztelve, ezért előfordulhatnak előre nem látott grafikai problémák egy Windows-os, vagy Linuxos futtatás esetén.*
