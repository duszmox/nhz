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

## A Program Használata ▶️

A felhasználó a futtatáskor egy menüvel találkozik, ahol a program fő funkció jelennek mmeg.
A konzol ablak alsó felében fogja a felhasználó megtalálni a menüt, ahol a Fel (⬆️) - Le (⬇️) nyíl gombokkal tud mozogni.

### Menüpontok

* **`Menetrend kezelése`**  
Ez a menüpont egy sub-menübe visz, ahol két menü jelenhet meg, attól függően, hogy létezik-e a `menetrend.csv` file.

  * Ha nem létezik, akkor egy `Menetrend generálása` és egy `<- Vissza` gombbal találkozik a felhasználó.
    A `Menetrend generálása` gomb legenerálj a menetrendet, a későbbi pontban kifejtett formátumban, a `megallok.csv` file-ban található megállók felhasználásával.
    A `<- Vissza` gomb visszaviszi a felhasználót a főmenübe

  * Ha létezik, akkor egy `Menetrend újragenerálása`, egy `Menetrend törlése` és egy `<- Vissza` gombbal találkozik a felhasználó.
  A `Menetrend újragenerálása` gomb újra lefuttatja az előző pontban funkciót.  
  A `Menetrend törlése` gomb törli a `menetrend.csv` file-t és frissíti a megjelenített opciókat az elöbbi pontban kifejtettekre.  
  A `<- Vissza` gomb visszaviszi a felhasználót a főmenübe
  
* **`Útvonaltervezés`** vagy **~~`Útvonaltervezés`~~**  
Ez a menüpont ki van húzva, ha nincs legenerálva a `menetrend.csv`
Ha létezik a file, akkor egy sub-menübe kerül a felhasználó ahol öt másik menüpont várja.  

  * `Indulási állomás (*kiválasztott állomás* VAGY Nincs még kiválasztott)`  
  A menüpont kiválasztásakor egy listával találja szembe magát a felhasználó amiben a megállók közül tud válogatni. A megállók ABC sorrendben vannak rendezve. A Fel (⬆️) - Le (⬇️) nyíl gombokkal tud mozogni és az Enterrel választani. Ezen kívül van egy kereső sáv alul, ahova karaktereket beírva tud szűrni a listából. A keresés nem kis-nagy betű specikus és substringekkel is működik. *(Ékezetes karakterek esetén kis-nagy betű specifikus)*

  * `Célállomás (*kiválasztott állomás* VAGY Nincs még kiválasztott)`  
  A menüpont kiválasztásakor egy listával találja szembe magát a felhasználó amiben a megállók közül tud válogatni. A megállók ABC sorrendben vannak rendezve. A Fel (⬆️) - Le (⬇️) nyíl gombokkal tud mozogni és az Enterrel választani. Ezen kívül van egy kereső sáv alul, ahova karaktereket beírva tud szűrni a listából. A keresés nem kis-nagy betű specikus és substringekkel is működik. *(Ékezetes karakterek esetén kis-nagy betű specifikus)*

  * `Indulás időpontja (*megadott időpont* VAGY Nincs még megadva)`  
  A menüpont kiválasztásakor egy input felülettel találja magát szembe a felhasználó, ahol a program bekér tőle egy időpontot HH:MM formátumban. Az időpontokat `int` típusban kéretik beírni. Értelem szerűen 0-23-ig terjedhet a HH és 0-59-ig az MM.

  * `Tervezés`  
  A gombra nyomás nem működik abban az esetben, ha a felette lévő értékek egyike nincs megadva. Ha minden szükséges adat meg van adva, akkor visszaad egy útvonalat amit követve a felhasználó el tud jutni a célállomásához. Ha a megadott időponttól már nem tudja elérni célállomását aznap, akkor ezt közli a felhasználóval. Egyéb esetben ilyen fomátumban írja ki az útvonaltervet:  
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
M2,Örs Vezér Tere,Pillangó Utca,Puskás Ferenc Stadion,Keleti Pályaudvar,Blaha Lujza Tér,Astoria,Deák Ferenc Tér,Kossuth Lajos Tér,Batthyány Tér,Széll Kálmán Tér,Déli Pályaudvar
M3,Kőbánya-Kispest,Határ Út,Pöttyös Utca,Ecseri Út,Népliget,Nagyvárad Tér,Semmelweis Klinikák,Corvin-Negyed,Kálvin Tér,Ferenciek Tere,Deák Ferenc Tér,Arany János Utca,Nyugati Pályaudvar,Lehel Tér,Dózsa György Út,Göncz Árpád Városközpont,Forgách Utca,Gyöngyösi Utca,Újpest-Városkapu,Újpest-Központ
M4,Kelenföld Vasútállomás,Bikás Park,Újbuda-Központ,Móricz Zsigmond Körtér,Szent Gellért Tér,Fővám Tér,Kálvin Tér,Rákóczi Tér,II. János Pál Pápa Tér,Keleti Pályaudvar

```
