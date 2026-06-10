# BreathMill – tehnička dokumentacija

## 1. Opis projekta

BreathMill je IoT digitalni spirometar razvijen u sklopu kolegija Razvoj ugradbenih sustava. Sustav simulira rad poticajnog spirometra pomoću ESP32 mikrokontrolera, OLED zaslona i potenciometra.

Zbog ograničenja laboratorijske simulacije, stvarni senzor protoka zraka zamijenjen je potenciometrom. Potenciometar služi kao emulator protoka daha, a korisnik njegovim pomicanjem simulira jačinu udaha.

Glavni vizualni element sustava je vjetrenjača prikazana na OLED zaslonu. Brzina okretanja vjetrenjače mijenja se ovisno o protoku daha. Ako je protok previsok, pokušaj se poništava i prikazuje se pokvarena vjetrenjača.

## 2. Cilj sustava

Cilj sustava je omogućiti korisniku vježbanje stabilnog i kontroliranog udaha.

Korisnik treba održati protok u ciljnoj zoni tijekom 5 sekundi. Ciljna zona odgovara protoku od najmanje 900 ml/s, ali manjem od 1200 ml/s.

Sustav korisniku daje povratnu informaciju kroz:

* OLED prikaz trenutnog protoka
* prikaz zone protoka
* indikator stabilnosti
* animaciju vjetrenjače
* prikaz ukupnog volumena
* prikaz uspješnog ili neuspješnog pokušaja

## 3. Hardverska arhitektura

### 3.1 Komponente

Korištene komponente su:

* ESP32 mikrokontroler
* OLED SSD1306 zaslon putem I2C komunikacije
* potenciometar za simulaciju protoka zraka
* reset tipka
* Wokwi simulator

### 3.2 Pinout

| Komponenta        | Pin na ESP32 | Opis                                             |
| ----------------- | -----------: | ------------------------------------------------ |
| Potenciometar SIG |       GPIO34 | Analogni ulaz za očitanje položaja potenciometra |
| Potenciometar VCC |          3V3 | Napajanje potenciometra                          |
| Potenciometar GND |          GND | Masa potenciometra                               |
| OLED SDA          |       GPIO21 | I2C podatkovna linija                            |
| OLED SCL          |       GPIO22 | I2C taktna linija                                |
| OLED VCC          |          3V3 | Napajanje OLED zaslona                           |
| OLED GND          |          GND | Masa OLED zaslona                                |
| Reset tipka       |       GPIO25 | Resetiranje pokušaja                             |
| Reset tipka       |          GND | Spajanje na masu                                 |

Reset tipka koristi `INPUT_PULLUP`, pa je pritisak tipke detektiran kada je očitanje pina `LOW`.

## 4. Obrada signala

### 4.1 Akvizicija signala

ESP32 očitava analogni signal s potenciometra preko pina `GPIO34`.

ADC vrijednost je u rasponu od približno 0 do 4095. Središnja vrijednost potenciometra predstavlja mirovanje i definirana je kao:

```cpp
#define ADC_CENTER 2048
```

Za izbjegavanje osjetljivosti na male promjene oko sredine koristi se mrtva zona:

```cpp
#define DEAD_ZONE 80
```

Smjer se određuje prema položaju potenciometra:

* vrijednost oko sredine predstavlja `MIROVANJE`
* vrijednost veća od sredine predstavlja `UDAH`
* vrijednost manja od sredine predstavlja `IZDAH`

U trenutnoj implementaciji protok za vježbu računa se samo za smjer `UDAH`. Ako je potenciometar pomaknut ulijevo, odnosno u smjeru `IZDAH`, protok se postavlja na `0`.

### 4.2 Pretvorba ADC vrijednosti u protok

Analogna vrijednost potenciometra pretvara se u simulirani protok zraka izražen u ml/s.

Maksimalna vrijednost protoka u implementaciji je:

```cpp
#define MAX_FLOW 1300.0
```

Vrijednost je postavljena malo iznad sigurnosnog praga od 1200 ml/s kako bi se moglo testirati prekoračenje i neuspješan pokušaj.

### 4.3 Filtriranje signala

Za zaglađivanje signala koristi se eksponencijalni klizni prosjek, odnosno EMA filter.

Koeficijent filtriranja je:

```cpp
#define EMA_ALPHA 0.2
```

Formula filtriranja:

```text
filteredFlow = EMA_ALPHA * input + (1 - EMA_ALPHA) * filteredFlow
```

EMA filter smanjuje nagle promjene signala i omogućuje stabilniji prikaz protoka na OLED zaslonu.

## 5. Zone protoka

Sustav razlikuje četiri zone protoka:

| Zona     | Raspon protoka | Značenje                                           |
| -------- | -------------: | -------------------------------------------------- |
| `LOW`    |     < 600 ml/s | Protok je preslab                                  |
| `MED`    |   600–899 ml/s | Protok je srednji, ali još nije dovoljan za uspjeh |
| `TARGET` |  900–1199 ml/s | Ciljna zona                                        |
| `DANGER` |    ≥ 1200 ml/s | Protok je previsok i pokušaj se poništava          |

Pragovi su definirani kao:

```cpp
#define FLOW_LEVEL_1 600.0
#define FLOW_LEVEL_2 900.0
#define FLOW_LIMIT 1200.0
```

## 6. Logika vježbe

### 6.1 Uvjet uspjeha

Vježba je uspješna ako su istovremeno zadovoljeni sljedeći uvjeti:

* protok je najmanje 900 ml/s
* protok je manji od 1200 ml/s
* signal je stabilan
* uvjeti traju neprekidno najmanje 5 sekundi

Trajanje stabilnosti definirano je kao:

```cpp
#define REQUIRED_STABLE_MS 5000
```

Ako korisnik izađe iz ciljane zone prije isteka 5 sekundi, stabilnost se prekida i mjerenje stabilnog vremena počinje ponovno.

### 6.2 Stabilnost signala

Osim što protok mora biti u ciljnoj zoni, sustav provjerava i stabilnost signala.

Signal se smatra stabilnim ako razlika između trenutnog i prethodnog filtriranog protoka nije veća od zadane tolerancije:

```cpp
bool stableSignal = abs(flow - previousFlow) < 40;
```

Time se sprječava da nagli trzaji potenciometra budu priznati kao stabilan udah.

### 6.3 Uvjet neuspjeha

Ako protok dosegne ili prijeđe 1200 ml/s, pokušaj se poništava.

U tom slučaju sustav:

* označava pokušaj kao neuspješan
* prekida praćenje stabilnosti
* prikazuje pokvarenu vjetrenjaču
* ispisuje poruku o poništenju na serijski monitor
* prikazuje volumen ostvaren prije poništenja

## 7. Numerička integracija volumena

Kao napredna funkcionalnost implementiran je izračun ukupnog volumena numeričkom integracijom protoka kroz vrijeme.

Protok je izražen u ml/s, a vremenski interval u sekundama. Ukupni volumen računa se prema izrazu:

```text
V = ∫ Q dt
```

Diskretno u programu:

```cpp
totalVolume += flow * dt;
```

gdje je:

* `totalVolume` ukupni volumen u ml
* `flow` filtrirani protok u ml/s
* `dt` vremenski interval između dva očitanja u sekundama

Očitavanje i obrada izvode se svakih 200 ms:

```cpp
#define SAMPLE_MS 200
```

## 8. OLED prikaz

OLED zaslon prikazuje trenutno stanje vježbe u stvarnom vremenu.

Na zaslonu se prikazuje:

* naziv sustava `BreathMill`
* trenutni protok
* zona protoka
* vrijeme stabilnosti
* ukupni volumen
* indikator ciljane zone
* marker trenutnog protoka
* vjetrenjača
* status `SUCCESS` ili `FAIL`

OLED zaslon koristi I2C komunikaciju i adresu `0x3C`.

## 9. Vjetrenjača

Vjetrenjača predstavlja kreativni grafički prikaz protoka daha.

Brzina okretanja vjetrenjače ovisi o trenutnom protoku. Što je veći protok, to se vjetrenjača brže okreće.

Za postizanje glađeg prikaza koristi se jednostavan model inercije. Trenutna brzina vjetrenjače ne mijenja se naglo, nego se postupno približava ciljnoj brzini:

```cpp
currentSpeed += (targetSpeed - currentSpeed) * 0.08;
```

Ako korisnik prijeđe sigurnosni prag od 1200 ml/s, prikazuje se pokvarena vjetrenjača kao vizualna oznaka neuspješnog pokušaja.

## 10. Reset funkcija

Sustav ima reset tipku spojenu na `GPIO25`.

Pritiskom na tipku resetiraju se:

* filtrirani protok
* prethodni protok
* ukupni volumen
* stabilnost
* vrijeme početka stabilnosti
* status uspjeha
* status neuspjeha
* stanje pokvarene vjetrenjače

Nakon resetiranja korisnik može započeti novi pokušaj.

## 11. Neblokirajući rad

Glavna logika očitanja i obrade signala ne koristi blokirajuće kašnjenje za periodično mjerenje. Umjesto toga koristi se funkcija `millis()`.

Očitavanje se izvodi kada prođe definirani interval:

```cpp
if (now - lastSample >= SAMPLE_MS)
```

Na taj način sustav može kontinuirano pratiti vrijeme stabilnosti, ažurirati OLED prikaz i obrađivati stanje vježbe.

## 12. Testirani slučajevi

Tijekom razvoja provjereni su sljedeći slučajevi:

* mirovanje potenciometra oko središnje vrijednosti
* smjer `IZDAH`, pri kojem se protok za vježbu postavlja na 0
* smjer `UDAH`, pri kojem se računa protok
* prelazak kroz zone `LOW`, `MED`, `TARGET` i `DANGER`
* stabilan protok u ciljnoj zoni tijekom 5 sekundi
* resetiranje stabilnosti kada korisnik izađe iz ciljane zone
* poništenje pokušaja ako protok prijeđe 1200 ml/s
* prikaz uspjeha i neuspjeha na OLED zaslonu
* rad reset tipke
* izračun volumena tijekom vježbe

## 13. Zaključak

BreathMill demonstrira primjenu ugradbenog sustava za simulaciju digitalnog spirometra. Projekt uključuje očitanje analognog signala, filtriranje, numeričku integraciju, logiku uspješne i neuspješne vježbe te prikaz stanja na OLED zaslonu.

Kreativni scenarij vjetrenjače omogućuje korisniku intuitivnu vizualnu povratnu informaciju, dok pragovi protoka i uvjet stabilnosti ostaju usklađeni s medicinskim modelom zadatka.