# Dokumentacio

## idopontok.h

Header file. Az időpont management-tel foglalkozik. Definiálja egy időpont változó struktúráját, és a hozzákapcsolódó függvényeket tartaltalmazza.

## megallok.h

Az első menüpont függvényeit tartalmazza. Beolvassa a megállók csv file-ból a megállókat és legenerálja a menetrendet.

## utvonalterv.h

A második menüpont függvényeit tartalmazza. Beolvassa a menetrend csv file-t. Definiálja a program fő adatainat struktúráját. Keres a megállók között, hosszokat állapít meg két megálló között, és egyéb más apróbb függvényeket tartalmaz, amik elősegítik az útvonaltervezést.

## menu.c

Jelenleg ez tartalmazza a menü-t, amin keresztül lehet interaktálni az adatokkal. Egyenlőre csak ncureses-el működik, ami windows operációs rendszeren nem elérhető library, így csak linuxon és macOS-es futtatható.

## metro_transportus.c

Egyenlőre nincs jelentősége a file-nak, majd ez lesz a belépőpontja az app-nak
