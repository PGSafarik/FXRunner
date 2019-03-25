Readme CS 

Popis programu
========================
GUI spoustec aplikaci s historii. Umoznuje spusteni v terminalu, jako root, ci zadat pracovni adresar aplikace. Umoznuje spustit vice aplikaci po sobe (okno FXRunneru se nezavre po spusteni aplikace).   

Balicek projektu obsahuje projektove soubory IDE Code::Blocks 

Zavislosti
========================
Foxtoolkit 1.7.64 (http://www.fox-toolkit.org)
cmake
g++

UPOZORNENI: Knihovna fxlib musi byt v dobe kompilace nainstalovana na vasem systemu. Zdrojove kody foxlib naleznete na adrese http://www.fox-toolkit.org/download.html. 

Mate-li knihovnu nainstalovanou na jinem, nez standardnim umisteni, je nutno tuto skutecnost zanest v souboru CMakeLists.txt v adresari projektu. Vyhledejte a zmente radky nastavujici cestu ke knihovne:
    INCLUDE_DIRECTORIES( "/Cesta/vasi/instalaci/foxlib/include" )   #Foxlib headers
    LINK_DIRECTORIES( "/Cesta/vasi/instalaci/foxlib/lib" )          #Foxlib library
Tyto radky jsou ve vychozim stavu zakomentovany, po uprave tedy jeste odstrante pocatecni znaky '#'.     

Kompilace a instalace
========================
Archiv s projektem rozbalte a prejdete do projektoveho adresare. Nasledne spustte prikazy pro kompilaci a instalaci projektu:
    cd build
    cmake .. && make && make install
Pokud vse probehne v poradku, vysledny binarni soubor se nainstaluje do /usr/local/bin. Direktivu 'make install' lze samozrejme vynechat, soubor fxrunner zustane pak pouze v adresari build. Toho lze vyuzit, pokud chcete prevzit kontrolu nad instalacnim procesem (napriklad vytvorit distribucni balik, apod.)

Kompilaci projektu lze provest tez z Code::Blocks. Projekt je nastaven tak, aby vysledny binarni soubor, vcetne objektovych souboru byly vytvareny v adresari build.

Kompletni vycisteni adresare build provedete prikazem
    rm -r * 
(predpoklada se, ze se aktualne nachazite v adresari build)

Zname chyby
========================
Chyby v nasledujicim seznamu se snazim vyresit a opravit
- Zaskrtnuti nekolika voleb spusteni spolecne se vzajemne vyrusi


Plany do budoucna
========================
(netrideno)
- Editaci a smazani historie v aplikaci
- Moznost vytvareni davkovych souboru
- Vytvareni davek z vybranych prikazu v historii, jejich pojmenovani, ukladani a spousteni
- Lokalizace
- Dokonceni vytvareni a nacitani "desktop" souboru
- Panel oblibenych prikazu
- Samostatny dialog nastaveni
