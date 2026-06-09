BreathMill – IoT digitalni spirometar

## 1. Opis projekta

BreathMill je IoT sustav koji simulira rad incentive spirometra pomoću ESP32 mikrokontrolera, OLED zaslona i potenciometra.

Potenciometar se koristi kao emulator protoka zraka (daha), dok OLED prikazuje stanje sustava u realnom vremenu.

Glavni vizualni element sustava je animirana vjetrenjača koja mijenja brzinu ovisno o jačini protoka.

---

## 2. Cilj sustava

Cilj je omogućiti korisniku:

- postizanje stabilnog protoka 900–1200 ml/s
- održavanje stabilnosti minimalno 5 sekundi
- izbjegavanje prejakog protoka (>1200 ml/s)
- vizualnu povratnu informaciju u realnom vremenu

---

## 3. Hardverska arhitektura

### Komponente:
- ESP32 mikrokontroler
- OLED SSD1306 (I2C)
- potenciometar (simulacija protoka zraka)
- tipka za reset

### Pinout:

| Komponenta | Pin |
|------------|-----|
| Potenciometar | GPIO34 |
| Reset tipka | GPIO25 |
| OLED SDA | GPIO21 |
| OLED SCL | GPIO22 |

---

## 4. Obrada signala

### 4.1 Akvizicija

- ADC očitanje: 0–4095
- nulta točka: 2048
- desna strana = UDAH
- lijeva strana = ignorirano

---

### 4.2 Filtriranje

Koristi se eksponencijalni klizni prosjek (EMA):

y(t) = α x(t) + (1 - α) y(t-1)

gdje je:
- α = 0.2

---

### 4.3 Pretvorba u protok

ADC vrijednost se mapira u:

- 0 – 1300 ml/s

---

## 5. Numerička integracija

Ukupni volumen zraka računa se pomoću:

V = ∫ Q dt

Diskretno u implementaciji:

totalVolume += flow * dt;

---

## 6. Logika vježbe

### Zone protoka:

| Zona | Raspon |
|------|--------|
| LOW | < 600 ml/s |
| MED | 600–900 ml/s |
| TARGET | 900–1200 ml/s |
| DANGER | >1200 ml/s |

---

### Uvjeti uspjeha:

Vježba je uspješna ako:

- protok ≥ 900 ml/s
- protok < 1200 ml/s
- signal je stabilan
- traje ≥ 5 sekundi

---

### Neuspjeh:

Vježba se prekida ako:

- protok > 1200 ml/s

---

## 7. HMI (OLED prikaz)

OLED prikazuje:

- trenutni protok (ml/s)
- zonu
- trajanje stabilnosti
- ukupni volumen
- status (SUCCESS / FAIL)

---

## 8. Vjetrenjača

- animacija rotacije ovisno o protoku
- inercijski model

### FAIL stanje:
- vjetrenjača se “lomi”
- prikazuje mehanički kolaps

---

## 9. Reset funkcija

GPIO25 resetira:

- vježbu
- volumen
- stabilnost
- UI stanje

---

## 10. Zaključak

BreathMill demonstrira:

- obradu analognih signala
- EMA filtriranje
- numeričku integraciju
- real-time HMI
- medicinski model spirometra