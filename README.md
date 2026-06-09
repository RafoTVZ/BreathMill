# BreathMill – IoT vjetrenjača za trening stabilnog daha

> BreathMill je projekt digitalnog spirometra razvijen u sklopu kolegija Razvoj ugradbenih sustava. Cilj projekta je kroz jednostavan i vizualno razumljiv scenarij vjetrenjače pomoći korisniku u vježbanju stabilnog i kontroliranog izdisaja.

## Opis projekta

Ovaj projekt je rezultat timskog rada u sklopu projektnog zadatka kolegija Razvoj ugradbenih sustava na Tehničkom veleučilištu u Zagrebu.

BreathMill je IoT digitalni spirometar koji koristi ESP32 mikrokontroler, OLED zaslon i potenciometar za simulaciju protoka zraka. Zbog ograničenja laboratorijske simulacije, stvarni senzor protoka zraka zamijenjen je potenciometrom. Pomicanjem potenciometra simulira se jačina daha korisnika.

Projekt koristi kreativni scenarij vjetrenjače: korisnikov dah pokreće virtualnu vjetrenjaču, a cilj je održati stabilan protok zraka u ciljnoj zoni tijekom 5 sekundi. Ako je protok preslab, vjetrenjača se okreće presporo. Ako je protok stabilan, vjetrenjača radi optimalno. Ako protok prijeđe 1200 ml/s, vjetrenjača se "pokvari" i pokušaj se poništava zbog prekoračenja sigurnosne granice.

Motivacija projekta je prikazati medicinski koncept vježbe disanja kroz jednostavno i razumljivo korisničko sučelje. Vizualna povratna informacija na OLED zaslonu korisniku pomaže razumjeti koliko je njegov izdisaj stabilan.

## Funkcijski zahtjevi

Sustav mora omogućiti:

- periodično očitavanje vrijednosti potenciometra
- pretvorbu vrijednosti potenciometra u simulirani protok daha
- filtriranje ulaznog signala radi smanjenja naglih promjena
- prikaz trenutnog protoka na OLED zaslonu
- prikaz tri razine protoka: 600 ml/s, 900 ml/s i 1200 ml/s
- prikaz indikatora stabilnosti protoka
- praćenje trajanja stabilnog izdisaja
- detekciju uspješne vježbe ako je protok stabilan najmanje 5 sekundi
- poništavanje pokušaja ako protok prijeđe 1200 ml/s
- izračun ukupnog volumena izdisaja numeričkom integracijom protoka kroz vrijeme
- prikaz rezultata korisnika nakon završetka pokušaja
- prikaz animirane vjetrenjače čija se brzina mijenja ovisno o protoku daha

## Tehnologije

Projekt koristi sljedeće tehnologije i komponente:

- ESP32 mikrokontroler
- OLED zaslon
- potenciometar za simulaciju protoka zraka
- Wokwi simulator
- Arduino/C++ razvojno okruženje
- GitHub za verzioniranje koda i dokumentaciju

## Instalacija i pokretanje

Projekt će se razvijati i testirati u Wokwi simulatoru.

Osnovni koraci za pokretanje:

1. Otvoriti Wokwi projekt.
2. Pokrenuti simulaciju.
3. Pomicati potenciometar kako bi se simulirao protok daha.
4. Pratiti prikaz vjetrenjače, protoka i rezultata na OLED zaslonu.
5. Provjeriti serijski ispis za dodatne informacije o stanju sustava.

## Članovi tima

- Antonio Rafajec, Antonela Miletić – razvoj programskog rješenja, dokumentacija, Wokwi simulacija, OLED prikaz, testiranje

## Link na projekt

- https://wokwi.com/projects/466356005804855297

## Kontribucije

Rad na projektu dijeli se prema funkcionalnim cjelinama:

- razvoj logike očitanja i filtriranja signala
- razvoj logike uspješne/neuspješne vježbe
- razvoj OLED prikaza i animacije vjetrenjače
- izrada dokumentacije
- testiranje i provjera rada u Wokwi simulatoru

Članovi tima komuniciraju putem dogovorenih komunikacijskih kanala, a promjene se prate kroz GitHub commitove.

## Kodeks ponašanja

Članovi tima obvezuju se na odgovorno, korektno i profesionalno ponašanje tijekom rada na projektu.

Očekuje se:

- poštivanje dogovorenih rokova
- jasna komunikacija među članovima tima
- konstruktivno rješavanje problema
- poštivanje tuđeg rada i doprinosa
- dokumentiranje vlastitih promjena

## Licenca

Projekt je izrađen u obrazovne svrhe u sklopu kolegija Razvoj ugradbenih sustava na Tehničkom veleučilištu u Zagrebu.

Materijali i korištene biblioteke podliježu vlastitim licencama.
