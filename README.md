```
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

A felhasználó a futtatáskor egy menüvel fog találkozni, ahol a program fő funkcióval fog találkozni. <br>
A menü kitölti a teljes konzol abalkot, és felül ASCII artban kiírja a Metro Transportus nevet.<br>
A konzol ablak alsó felében fogja a felhasználó megtalálni a menüt, ahol a Fel (⬆️) - Le (⬇️) nyíl gombokkal tud mozogni. Ehhez #ifdef-et kell használni, hogy a platformhoz megfelelő header-t include-oljam a fileban, hogy működjük a billentyűzet vezérlés. <br>

### Menüpontok

**`Menetrend kezelése`**  
Ez a menüpont egy sub-menübe visz, ahol két menü jelenhet meg, attól függően, hogy létezik-e a <code>menetrend.csv</code> file. <br>

* Ha nem létezik, akkor egy <code>Menetrend generálása</code> és egy <code><- Vissza</code> gombbal találkozik a felhasználó. <br>
    A <code>Menetrend generálása</code> gomb legenerálj a menetrendet, a későbbi pontban kifejtett formátumban, a <code>megallok.csv</code> (Ha <code>megallok.csv</code> nem létezik, hibát ír alul) file-ban található megállók felhasználásával. Ennek a file-nak a formátumát is későbbi pontban fejtem ki. (Ha több azonos nevű metró vonal van, csak az elsőt veszi figyelembe) <br>
    A <code><- Vissza</code> gomb visszaviszi a felhasználót a főmenübe

* Ha létezik, akkor egy <code>Menetrend újragenerálása</code>, egy <code>Menetrend törlése</code> és egy <code><- Vissza</code> gombbal találkozik a felhasználó.  <br>
    A <code>Menetrend újragenerálása</code> gomb újra lefuttatja az előző pontban funkciót. <br>
    A <code>Menetrend törlése</code> gomb törli a <code>menetrend.csv</code> file-t és frissíti a megjelenített opciókat az elöbbi pontban kifejtettekre. <br>
    A <code><- Vissza</code> gomb visszaviszi a felhasználót a főmenübe

  <br>
  
**`Útvonaltervezés`** vagy **~~`Útvonaltervezés`~~**
Ez a menüpont ki van húzva és a képernyő alján hibát üzenetet ír ki megnyomáskor, ha nincs legenerálva a <code>menetrend.csv</code> <br>
Ha létezik a file, akkor egy sub-menübe kerül a felhasználó ahol öt másik menüpont várja.

* <code>Indulási állomás (*kiválasztott állomás* VAGY Nincs még kiválasztott)</code> <br>
A menüpont kiválasztásakor egy listával találja szembe magát a felhasználó amiben a megállók közül tud válogatni. A megállók ABC sorrendben vannak rendezve. A Fel (⬆️) - Le (⬇️) nyíl gombokkal tud mozogni és az enterrel választani. Ezen kívül van egy kereső sáv alul, ahova karaktereket beírva tud szűrni a listából. A keresés kis-nagy betű specikus és substringekkel is működik. 

* <code>Célállomás (*kiválasztott állomás* VAGY Nincs még kiválasztott)</code> <br>
A menüpont kiválasztásakor egy listával találja szembe magát a felhasználó amiben a megállók közül tud válogatni. A megállók ABC sorrendben vannak rendezve. A Fel (⬆️) - Le (⬇️) nyíl gombokkal tud mozogni és az enterrel választani. Ezen kívül van egy kereső sáv alul, ahova karaktereket beírva tud szűrni a listából. A keresés kis-nagy betű specikus és substringekkel is működik. 

* <code>Indulás időpontja (*megadott időpont* VAGY Nincs még megadva)</code>
 <br>

A menüpont kiválasztásakor egy input felülettel találja magát szembe a felhasználó, ahol a program bekér tőle egy időpontot HH:MM formátumban. Az időpontokat <code>int</code> típusban kéretik beírni. Értelem szerűen 0-23-ig terjedhet a HH és 0-59-ig az MM.

* <code>Tervezés</code> <br>
A gombra nyomás hibát ír ki abban az esetben, ha a felette lévő értékek egyike nincs megadva. <br>
Ha minden szükséges adat meg van adva, akkor visszaad egy útvonalat amit követve a felhasználó el tud jutni a célállomásához. Ha a megadott időponttól már nem tudja elérni célállomását aznap, akkor ezt közli a felhasználóval. <br> Egyéb esetben ilyen fomátumban írja ki az útvonaltervet:  

####  Minta  Átszállás

    O-(MX) Megálló neve - HH:MM
    |
    |-- X megálló - Y perc
    |
    O-(MX) Megálló neve - HH:MM
    |
    |-- átszállás
    |
    O-(MX) Megálló neve - HH:MM
    |
    |-- X megálló - Y perc
    |
    O-(MX) Megálló neve - HH:MM


    <- Vissza
A vissza gomb itt is megjeleik, alapból ki van választva, enterre visszatér az előző menübe

####  Minta  Átszállás nélkül

    O-(MX) Megálló neve - HH:MM
    |
    |-- X megálló - Y perc
    |
    O-(MX) Megálló neve - HH:MM


    <- Vissza
A vissza gomb itt is megjeleik, alapból ki van választva, enterre visszatér az előző menübe
* <code><- Vissza</code>  
 Visszatér a főmenübe

**`Kilépés`**  
A gomb kiválasztásakor kilép az alkalmazásból


## File formátumok 📄
#### Megallok.csv
Minden sor egy járatot tartalmaz. Az adatok a csv formátumhoz híven vesszővel vannak elválasztva. Minden sor első eleme a járat neve, a több elem a járat megállói.
```
M1,Vörösmarty Tér,Deák Ferenc Tér,Bajcsy-Zsilinszky Út,Opera,Oktogon,Vörösmarty Utca,Kodály Körönd,Bajza Utca,Hősök Tere,Széchenyi Fürdő,Mexikói Út
.
.
.
M4,Kelenföld Vasútállomás,Bikás Park,Újbuda-Központ,Móricz Zsigmond Körtér,Szent Gellért Tér,Fővám Tér,Kálvin Tér,Rákóczi Tér,Ii. János Pál Pápa Tér,Keleti Pályaudvar
```
####  Menetrend.csv
Minden sor egy megállót tartalmaz. Az adatok a csv formátumhoz híven vesszővel vannak elválasztva. Minden sor első eleme a járat neve, a második a megálló neve, a további elemek pedig az indulási időpontokat jelzi.
```
M1,Vörösmarty Tér,1:00,1:05,1:10,1:15,1:20,...,23:45,23:50,23:55
.
.
.
M4,Keti Pályaudvar,1:00,1:05,1:10,1:15,1:20,...,23:45,23:50,23:55
```