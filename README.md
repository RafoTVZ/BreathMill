# BreathMill – IoT vjetrenjača za trening stabilnog daha

## Opis projekta

BreathMill je IoT digitalni spirometar namijenjen vježbanju stabilnog i kontroliranog izdisaja. Projekt koristi kreativni scenarij vjetrenjače: korisnikov dah pokreće virtualnu vjetrenjaču, a cilj je održavati stabilan protok zraka u zadanoj ciljnoj zoni tijekom 5 sekundi.

Zbog ograničenja laboratorijske simulacije, stvarni senzor protoka zraka zamijenjen je potenciometrom koji simulira jačinu daha korisnika. Pomicanjem potenciometra mijenja se vrijednost simuliranog protoka, dok ESP32 obrađuje signal, prikazuje stanje vježbe na OLED zaslonu i određuje je li pokušaj uspješan.

## Platforma

Projekt se temelji na sljedećim komponentama:

- ESP32 mikrokontroler
- OLED zaslon
- potenciometar za simulaciju protoka zraka
- Wokwi simulacijsko okruženje

## Funkcionalnosti uređaja

Sustav će omogućiti:

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

## Logika vježbe

Cilj korisnika je održavati protok daha u ciljnoj zoni. Ako je protok preslab, vjetrenjača se okreće presporo i pokušaj se ne smatra uspješnim. Ako je protok stabilan i dovoljno jak, vjetrenjača se okreće optimalnom brzinom. Ako protok prijeđe 1200 ml/s, pokušaj se poništava jer se smatra da je korisnik prejakim izdisajem izazvao nestabilan protok.

Uspješna vježba ostvaruje se kada su zadovoljeni sljedeći uvjeti:

- protok je najmanje 900 ml/s
- protok ne prelazi 1200 ml/s
- signal ostaje stabilan unutar ciljane zone
- uvjeti traju neprekidno 5 sekundi

## OLED prikaz

Na OLED zaslonu prikazivat će se:

- animirana vjetrenjača čija se brzina mijenja ovisno o protoku
- trenutna vrijednost protoka
- pragovi od 600 ml/s, 900 ml/s i 1200 ml/s
- indikator stabilnosti protoka
- brojač stabilnih sekundi
- završni rezultat pokušaja

## Napredna funkcionalnost

Kao napredna funkcionalnost implementirat će se model inercije vjetrenjače. Brzina vrtnje lopatica neće se mijenjati trenutno, nego postupno, čime se simulira masa vjetrenjače i otpor zraka. Time korisnik dobiva jasniji vizualni biofeedback o stabilnosti daha.

Moguće dodatne nadogradnje uključuju spremanje najboljeg rezultata u trajnu memoriju i slanje rezultata putem Wi-Fi veze.

## Sažetak

BreathMill prikazuje digitalni spirometar kroz jednostavan i razumljiv scenarij vjetrenjače. Projekt povezuje medicinski uvjet stabilnog protoka daha s vizualnim prikazom na OLED zaslonu, čime korisnik dobiva povratnu informaciju o kvaliteti izdisaja.
