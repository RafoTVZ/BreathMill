# BreathMill – IoT vjetrenjača za trening stabilnog daha

> BreathMill je projekt IoT digitalnog spirometra razvijen u sklopu kolegija Razvoj ugradbenih sustava. Cilj projekta je kroz jednostavan i vizualno razumljiv scenarij vjetrenjače pomoći korisniku u vježbanju stabilnog i kontroliranog udaha.

## Opis projekta

Ovaj projekt je rezultat timskog rada u sklopu projektnog zadatka kolegija Razvoj ugradbenih sustava na Tehničkom veleučilištu u Zagrebu.

BreathMill je digitalni spirometar koji koristi ESP32 mikrokontroler, I2C OLED zaslon, potenciometar i reset tipku. Zbog ograničenja laboratorijske simulacije, stvarni senzor protoka zraka zamijenjen je potenciometrom. Pomicanjem potenciometra simulira se jačina daha korisnika.

Projekt koristi kreativni scenarij vjetrenjače. Korisnikov dah pokreće virtualnu vjetrenjaču, a cilj je održati stabilan protok zraka u ciljnoj zoni tijekom 5 sekundi. Ako je protok preslab, vjetrenjača se okreće sporije. Ako je protok stabilan, vjetrenjača radi optimalno. Ako protok prijeđe 1200 ml/s, vjetrenjača se prikazuje kao pokvarena, a pokušaj se poništava zbog prekoračenja sigurnosne granice.

Motivacija projekta je prikazati medicinski koncept vježbe disanja kroz jednostavno i razumljivo korisničko sučelje. Vizualna povratna informacija na OLED zaslonu korisniku pomaže razumjeti nalazi li se njegov udah u ciljnoj zoni i je li dovoljno stabilan.

## Funkcijski zahtjevi

Sustav omogućuje:

* periodično očitavanje vrijednosti potenciometra bez blokirajućih kašnjenja u glavnoj logici
* pretvorbu vrijednosti potenciometra u simulirani protok daha
* određivanje smjera protoka: mirovanje, udah i izdah
* filtriranje ulaznog signala pomoću EMA filtera
* prikaz trenutnog protoka na OLED zaslonu
* prikaz razina protoka: 600 ml/s, 900 ml/s i 1200 ml/s
* prikaz indikatora stabilnosti protoka
* prikaz vjetrenjače čija se brzina mijenja ovisno o protoku daha
* prikaz pokvarene vjetrenjače u slučaju prekoračenja dopuštenog protoka
* praćenje trajanja stabilnog protoka u ciljnoj zoni
* detekciju uspješne vježbe ako je protok stabilan najmanje 5 sekundi
* poništavanje pokušaja ako protok prijeđe 1200 ml/s
* izračun ukupnog volumena numeričkom integracijom protoka kroz vrijeme
* prikaz rezultata korisnika nakon završetka pokušaja
* resetiranje pokušaja pomoću tipke

## Tehnologije i komponente

Projekt koristi sljedeće tehnologije i komponente:

* ESP32 mikrokontroler
* I2C OLED zaslon SSD1306
* potenciometar za simulaciju protoka zraka
* reset tipka
* Wokwi simulator
* Arduino/C++ razvojno okruženje
* GitHub za verzioniranje koda i dokumentaciju

## Instalacija i pokretanje

Projekt se razvija i testira u Wokwi simulatoru.

Osnovni koraci za pokretanje:

1. Otvoriti Wokwi projekt.
2. Pokrenuti simulaciju.
3. Pomicati potenciometar kako bi se simulirao protok daha.
4. Pratiti prikaz protoka, stabilnosti, vjetrenjače i rezultata na OLED zaslonu.
5. U slučaju uspjeha ili neuspjeha koristiti reset tipku za pokretanje novog pokušaja.
6. Provjeriti serijski ispis za dodatne informacije o stanju sustava.

## Link na projekt

* https://wokwi.com/projects/466356005804855297

## Članovi tima

* Antonio Rafajec – OLED prikaz, prikaz i animacija vjetrenjače, Wokwi shema, reset tipka, projektna dokumentacija/Wiki
* Antonela Miletić – logika vježbe, očitanje potenciometra, mapiranje ADC vrijednosti na protok, filtriranje signala, uvjeti uspjeha/neuspjeha, izračun volumena i testiranje osnovne logike

## Kontribucije

Rad na projektu dijeli se prema funkcionalnim cjelinama:

* razvoj logike očitanja i filtriranja signala
* razvoj logike uspješne i neuspješne vježbe
* izračun volumena numeričkom integracijom protoka kroz vrijeme
* razvoj OLED prikaza
* razvoj prikaza i animacije vjetrenjače
* izrada i dorada Wokwi sheme
* izrada dokumentacije
* testiranje i provjera rada u Wokwi simulatoru

Članovi tima komuniciraju putem dogovorenih komunikacijskih kanala, a promjene se prate kroz GitHub commitove.

## Napredna funkcionalnost

Kao napredna funkcionalnost implementiran je izračun ukupnog volumena numeričkom integracijom protoka kroz vrijeme.

Protok se računa u ml/s, a volumen se dobiva zbrajanjem doprinosa protoka u vremenskim intervalima tijekom izvođenja vježbe.

## Kodeks ponašanja

Članovi tima obvezuju se na odgovorno, korektno i profesionalno ponašanje tijekom rada na projektu.

Očekuje se:

* poštivanje dogovorenih rokova
* jasna komunikacija među članovima tima
* konstruktivno rješavanje problema
* poštivanje tuđeg rada i doprinosa
* dokumentiranje vlastitih promjena

## Licenca

Projekt je izrađen u obrazovne svrhe u sklopu kolegija Razvoj ugradbenih sustava na Tehničkom veleučilištu u Zagrebu.

Materijali i korištene biblioteke podliježu vlastitim licencama.
