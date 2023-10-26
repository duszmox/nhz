# Metro Transportus

<details open>
  <summary><strong>A Program Célja 🏁</strong></summary>
A program Budapest metróhálózatát szimulálja. A felhasználó útvonaltervet tervezhet a kiinduló állomás, a célállomás és indulási időpont megadásával. A program visszaad egy útvonal tervet, ami tartalmazza:

* a kiindulási állomást, és hogy mikor indul onnan a szerelvény,
* az átszállási pontot, ha van, és hogy mikor érkezik be oda a szerelvény, és, hogy mikor indul a másik vonalon
* a célállomást, és, hogy mikor érkezik be a szerelvény

</details>

<details open>
  <summary><strong>A Program Használata ▶️</strong></summary>
A felhasználó a futtatáskor egy menüvel fog találkozni, ahol a program fő funkcióval fog találkozni. <br>
A menü kitölti a teljes konzol abalkot, és felül ASCII artban kiírja a Metro Transportus nevet.<br>
A konzol ablak alsó felében fogja a felhasználó megtalálni a menüt, ahol a Fel (⬆️) - Le (⬇️) nyíl gombokkal tud mozogni. Ehhez #ifdef-et kell használni, hogy a platformhoz megfelelő header-t include-oljam a fileban, hogy működjük a billentyűzet vezérlés. <br>

#### Menüpontok

<details>
<summary>Menetrend kezelése</summary>
Ez a menüpont egy sub-menübe visz, ahol két menü jelenhet meg, attól függően, hogy létezik-e a <code>menetrend.txt</code> file. <br>
<li>
    Ha nem létezik, akkor egy <code>Menetrend generálása</code> és egy <code><- Vissza</code> gombbal találkozik a felhasználó. <br>
    A <code>Menetrend generálása</code> gomb legenerálj a menetrendet, a későbbi pontban kifejtett formátumban, a <code>megallok.txt</code> file-ban található megállók felhasználásával. Ennek a file-nak a formátumát is későbbi pontban fejtem ki. <br>
    A <code><- Vissza</code> gomb visszaviszi a felhasználót a főmenübe

</li>
<li>
    Ha létezik, akkor egy <code>Menetrend újragenerálása</code>, egy <code>Menetrend törlése</code> és egy <code><- Vissza</code> gombbal találkozik a felhasználó. <br>
    A <code>Menetrend újragenerálása</code> gomb újra lefuttatja az előző pontban funkciót. <br>
    A <code>Menetrend törlése</code> gomb törli a <code>Menetrend.txt</code> file-t és frissíti a megjelenített opciókat az elöbbi pontban kifejtettekre. <br>
    A <code><- Vissza</code> gomb visszaviszi a felhasználót a főmenübe
</li>

</details>
</details>
