```text
   __  ________________  ____    _________  ___   _  _________  ____  ___ ________  ______
  /  |/  / __/_  __/ _ \/ __ \  /_  __/ _ \/ _ | / |/ / __/ _ \/ __ \/ _ /_  __/ / / / __/
 / /|_/ / _/  / / / , _/ /_/ /   / / / , _/ __ |/    _\ \/ ___/ /_/ / , _// / / /_/ _\ \  
/_/  /_/___/ /_/ /_/|_|\____/   /_/ /_/|_/_/ |_/_/|_/___/_/   \____/_/|_|/_/  \____/___/  
                                                                                          
```

## A Program Célja 🏁

A program Budapest metróhálózatát szimulálja. A felhasználó útvonaltervet tervezhet a kiinduló állomás, a célállomás és indulási időpont megadásával. A program visszaad egy útvonal tervet, ami tartalmazza:

* a kiindulási állomást, és hogy mikor indul onnan a szerelvény,
* az átszállási pontot, ha van, és hogy mikor érkezik be oda a szerelvény, és, hogy mikor indul a másik vonalon
* a célállomást, és, hogy mikor érkezik be a szerelvény

**A program feltételezi, hogy el lehet jutni A megállóból B megállóba**

## A Program Használata ▶️

A felhasználó a futtatáskor egy menüvel fog találkozni, ahol a program fő funkcióval fog találkozni.
A menü kitölti a teljes konzol abalkot, és felül ASCII artban kiírja a Metro Transportus nevet.  
A konzol ablak alsó felében fogja a felhasználó megtalálni a menüt, ahol a Fel (⬆️) - Le (⬇️) nyíl gombokkal tud mozogni. Ehhez `#ifdef`-et kell használni, hogy a platformhoz megfelelő header-t include-oljam a fileban, hogy működjük a billentyűzet vezérlés.

### Menüpontok

* **`Menetrend kezelése`**  
Ez a menüpont egy sub-menübe visz, ahol két menü jelenhet meg, attól függően, hogy létezik-e a `menetrend.csv` file.

  * Ha nem létezik, akkor egy `Menetrend generálása` és egy `<- Vissza` gombbal találkozik a felhasználó.
    A `Menetrend generálása` gomb legenerálj a menetrendet, a későbbi pontban kifejtett formátumban, a `megallok.csv` (Ha `megallok.csv` nem létezik, hibát ír alul) file-ban található megállók felhasználásával. Ennek a file-nak a formátumát is későbbi pontban fejtem ki. (Ha több azonos nevű metró vonal van, csak az elsőt veszi figyelembe)
    A `<- Vissza` gomb visszaviszi a felhasználót a főmenübe

  * Ha létezik, akkor egy `Menetrend újragenerálása`, egy `Menetrend törlése` és egy `<- Vissza` gombbal találkozik a felhasználó.
  A `Menetrend újragenerálása` gomb újra lefuttatja az előző pontban funkciót.  
  A `Menetrend törlése` gomb törli a `menetrend.csv` file-t és frissíti a megjelenített opciókat az elöbbi pontban kifejtettekre.  
  A `<- Vissza` gomb visszaviszi a felhasználót a főmenübe
  
* **`Útvonaltervezés`** vagy **~~`Útvonaltervezés`~~**  
Ez a menüpont ki van húzva, és a képernyő alján hiba üzenetet ír ki megnyomáskor, ha nincs legenerálva a `menetrend.csv`
Ha létezik a file, akkor egy sub-menübe kerül a felhasználó ahol öt másik menüpont várja.  

  * `Indulási állomás (*kiválasztott állomás* VAGY Nincs még kiválasztott)`  
  A menüpont kiválasztásakor egy listával találja szembe magát a felhasználó amiben a megállók közül tud válogatni. A megállók ABC sorrendben vannak rendezve. A Fel (⬆️) - Le (⬇️) nyíl gombokkal tud mozogni és az Enterrel választani. Ezen kívül van egy kereső sáv alul, ahova karaktereket beírva tud szűrni a listából. A keresés nem kis-nagy betű specikus és substringekkel is működik.

  * `Célállomás (*kiválasztott állomás* VAGY Nincs még kiválasztott)`  
  A menüpont kiválasztásakor egy listával találja szembe magát a felhasználó amiben a megállók közül tud válogatni. A megállók ABC sorrendben vannak rendezve. A Fel (⬆️) - Le (⬇️) nyíl gombokkal tud mozogni és az Enterrel választani. Ezen kívül van egy kereső sáv alul, ahova karaktereket beírva tud szűrni a listából. A keresés nem kis-nagy betű specikus és substringekkel is működik.

  * `Indulás időpontja (*megadott időpont* VAGY Nincs még megadva)`  
  A menüpont kiválasztásakor egy input felülettel találja magát szembe a felhasználó, ahol a program bekér tőle egy időpontot HH:MM formátumban. Az időpontokat `int` típusban kéretik beírni. Értelem szerűen 0-23-ig terjedhet a HH és 0-59-ig az MM.

  * `Tervezés`  
  A gombra nyomás hibát ír ki abban az esetben, ha a felette lévő értékek egyike nincs megadva. Ha minden szükséges adat meg van adva, akkor visszaad egy útvonalat amit követve a felhasználó el tud jutni a célállomásához. Ha a megadott időponttól már nem tudja elérni célállomását aznap, akkor ezt közli a felhasználóval. Egyéb esetben ilyen fomátumban írja ki az útvonaltervet:  
  **Minta Átszállással**

    ```text
    O-(MX) Indulási Megálló neve - HH:MM
    |
    |-- X megálló - Y perc
    |
    O-(MX) Átszállási Megálló neve - HH:MM
    |
    |-- átszállás
    |
    O-(MZ) Átszállási Megálló neve - HH:MM
    |
    |-- X megálló - Y perc
    |
    O-(MZ) Célállomás Megálló neve - HH:MM

    <- Vissza
    ```

    A vissza gomb itt is megjelenik, alapból ki van választva, Enterre visszatér az előző menübe

    **Minta Átszállás nélkül**

    ```text
    O-(MX) Indulási Megálló neve - HH:MM
    |
    |-- X megálló - Y perc
    |
    O-(MX) Célállomás Megálló neve - HH:MM


    <- Vissza
    ```

    A vissza gomb itt is megjeleik, alapból ki van választva, Enterre visszatér az előző menübe

  * `<- Vissza`  
   Visszatér a főmenübe

* **`Kilépés`**  
A gomb kiválasztásakor kilép az alkalmazásból

## File formátumok 📄

### megallok.csv

Minden sor egy járatot tartalmaz. Az adatok a csv formátumhoz híven vesszővel vannak elválasztva. Minden sor első eleme a járat neve (pl.: M3), a több elem a járat megállói (pl.: Corvin-negyed). **A program feltételezi, hogy kettőmegálló ugyanazzal a névvel egy átszállási pont!**

```csv
M1,Vörösmarty Tér,Deák Ferenc Tér,Bajcsy-Zsilinszky Út,Opera,Oktogon,Vörösmarty Utca,Kodály Körönd,Bajza Utca,Hősök Tere,Széchenyi Fürdő,Mexikói Út
.
.
.
M4,Kelenföld Vasútállomás,Bikás Park,Újbuda-Központ,Móricz Zsigmond Körtér,Szent Gellért Tér,Fővám Tér,Kálvin Tér,Rákóczi Tér,II. János Pál Pápa Tér,Keleti Pályaudvar
```

###  menetrend.csv

Minden sor egy megállót tartalmaz. Az adatok a csv formátumhoz híven vesszővel vannak elválasztva. Minden sor első eleme a járat neve (pl.: M2), a második a vonal végállomásának neve (pl.: Mexikói Út), a harmadik az aktuális megálló neve ahova az indulási időpontok tartoznak (pl.: Oktogon), a további elemek pedig az indulási időpontokat jelzi (pl.: 6:55). **A program feltételezi, hogy a metró szerelvény ugyanabban a percben érkezik be és indul el.**

```csv
M1,Mexikói út,Vörösmarty Tér,1:01,1:06,1:11,1:16,1:21,...,23:46,23:51,23:56
M1,Vörösmarty Tér,Vörösmarty Tér,1:00,1:05,1:10,1:15,1:20,...,23:45,23:50,23:55
.
.
.
M4,Kelenföld Vasútállomás,Keti Pályaudvar,1:00,1:05,1:10,1:15,1:20,...,23:45,23:50,23:55
M4,Keleti Pályaudvar,Keti Pályaudvar,1:00,1:05,1:10,1:15,1:20,...,23:45,23:50,23:55
```
