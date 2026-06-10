#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define POT_PIN 34
#define RESET_BTN 25

// ================= FLOW MODEL =================
#define ADC_CENTER 2048
#define DEAD_ZONE 80
#define MAX_FLOW 1300.0

#define FLOW_LEVEL_1 600.0
#define FLOW_LEVEL_2 900.0
#define FLOW_LIMIT   1200.0

#define EMA_ALPHA 0.2
#define SAMPLE_MS 200
#define REQUIRED_STABLE_MS 5000

// ================= STATE =================
float filteredFlow = 0;
float previousFlow = 0;
float totalVolume = 0;

unsigned long lastSample = 0;
unsigned long stableStart = 0;

bool stable = false;
bool success = false;
bool failed = false;
bool brokenWindmill = false;

// ================= WINDMILL =================
float currentSpeed = 0;
int frame = 0;
unsigned long lastAnim = 0;

// ================= RESET =================
void resetExercise()
{
  filteredFlow = 0;
  previousFlow = 0;
  totalVolume = 0;

  stable = false;
  success = false;
  failed = false;

  stableStart = 0;

  brokenWindmill = false;
}

// ================= FLOW =================
float calculateFlow(int raw)
{
  int offset = raw - ADC_CENTER;

  if (abs(offset) < DEAD_ZONE) return 0;

  if (offset < 0) return 0;

  return (offset / 2047.0) * MAX_FLOW;
}

float ema(float in)
{
  filteredFlow = EMA_ALPHA * in + (1 - EMA_ALPHA) * filteredFlow;
  return filteredFlow;
}

// ================= LOGIKA =================
void updateLogic(float flow, float dt)
{
  if (success || failed) return;

  if (flow >= FLOW_LIMIT)
  {
    failed = true;
    brokenWindmill = true;

    stable = false;
    Serial.println("POKUSAJ PONISTEN: protok je presao 1200 ml/s.");
    Serial.print("Volumen prije ponistenja: ");
    Serial.print(totalVolume, 1);
    Serial.println(" ml"); 
    return;
  }

  bool stableSignal = abs(flow - previousFlow) < 40;
  bool inZone = flow >= FLOW_LEVEL_2 && flow < FLOW_LIMIT && stableSignal;

  if (inZone)
  {
    if (!stable)
    {
      stable = true;
      stableStart = millis();
      Serial.println("[INFO] Stabilna zona zapoceta");
    }

    float stableTime = millis() - stableStart;

    if (stableTime >= REQUIRED_STABLE_MS)
    {
      success = true;
      Serial.println("VJEZBA USPJESNA: protok je stabilan 5 sekundi.");
      Serial.print("Ukupni volumen: ");
      Serial.print(totalVolume, 1); Serial.println(" ml");
     }
  }
  else
  {
    if (stable)
      Serial.println("[INFO] Stabilnost prekinuta");
      stable = false;
      stableStart = 0;
  }

  previousFlow = flow;

  totalVolume += flow * dt;
}

// ================= WINDMILL =================
void updateWindmill(float flow)
{
  float targetSpeed = map(constrain(flow, 0, 1200), 0, 1200, 0, 100);

  currentSpeed += (targetSpeed - currentSpeed) * 0.08;

  int delayTime = 500 - currentSpeed * 4;
  delayTime = constrain(delayTime, 40, 500);

  if (millis() - lastAnim > delayTime)
  {
    frame = (frame + 1) % 4;
    lastAnim = millis();
  }
}

// ================= DRAW WINDMILL =================
void drawWindmill(int x, int y)
{
  display.drawLine(x, y, x, y + 20, SSD1306_WHITE);

  if (frame == 0)
  {
    display.drawLine(x, y, x, y - 10, SSD1306_WHITE);
    display.drawLine(x, y, x + 10, y, SSD1306_WHITE);
    display.drawLine(x, y, x, y + 10, SSD1306_WHITE);
    display.drawLine(x, y, x - 10, y, SSD1306_WHITE);
  }
  else if (frame == 1)
  {
    display.drawLine(x, y, x + 7, y - 7, SSD1306_WHITE);
    display.drawLine(x, y, x + 7, y + 7, SSD1306_WHITE);
    display.drawLine(x, y, x - 7, y + 7, SSD1306_WHITE);
    display.drawLine(x, y, x - 7, y - 7, SSD1306_WHITE);
  }
  else if (frame == 2)
  {
    display.drawLine(x, y, x, y - 10, SSD1306_WHITE);
    display.drawLine(x, y, x + 10, y, SSD1306_WHITE);
    display.drawLine(x, y, x, y + 10, SSD1306_WHITE);
    display.drawLine(x, y, x - 10, y, SSD1306_WHITE);
  }
  else
  {
    display.drawLine(x, y, x + 7, y - 7, SSD1306_WHITE);
    display.drawLine(x, y, x + 7, y + 7, SSD1306_WHITE);
    display.drawLine(x, y, x - 7, y + 7, SSD1306_WHITE);
    display.drawLine(x, y, x - 7, y - 7, SSD1306_WHITE);
  }

  display.fillCircle(x, y, 2, SSD1306_WHITE);
}

void drawBrokenWindmill(int x, int y)
{
  display.drawLine(x, y, x + 12, y + 22, SSD1306_WHITE);

  display.drawLine(x, y, x - 6, y - 8, SSD1306_WHITE);
  display.drawLine(x, y, x + 3, y + 9, SSD1306_WHITE);

  display.drawLine(x - 10, y + 5, x - 14, y + 10, SSD1306_WHITE);
  display.drawLine(x + 10, y - 6, x + 14, y - 12, SSD1306_WHITE);

  display.drawPixel(x + 8, y + 12, SSD1306_WHITE);
  display.drawPixel(x - 12, y + 6, SSD1306_WHITE);

  display.drawLine(x - 6, y, x + 6, y, SSD1306_WHITE);

  display.fillCircle(x, y, 2, SSD1306_WHITE);
}

// ================= OLED =================
void drawOLED(float flow)
{
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.print("BreathMill");

  display.setCursor(0, 10);
  display.print("Flow:");
  display.print((int)flow);

  display.setCursor(0, 20);
  if (flow < 600) display.print("LOW");
  else if (flow < 900) display.print("MED");
  else if (flow < 1200) display.print("TARGET");
  else display.print("DANGER");

  display.setCursor(0, 30);
  if (stable)
    display.print((millis() - stableStart) / 1000.0);
  else
    display.print("0.0");

  display.print("s");

  display.setCursor(0, 40);
  display.print("V:");
  display.print((int)totalVolume);

  display.drawRect(118, 5, 8, 50, SSD1306_WHITE);
  display.drawRect(118, 20, 8, 12, SSD1306_WHITE);

  int markerY = map(constrain(flow, 0, 1200), 0, 1200, 50, 5);
  display.fillRect(119, markerY, 6, 3, SSD1306_WHITE);

  if (brokenWindmill)
  {
    drawBrokenWindmill(90, 28);
  }
  else
  {
    drawWindmill(90, 28);
  }

  if (success)
  {
    display.setCursor(70, 54);
    display.print("SUCCESS");
  }

  if (failed)
  {
    display.setCursor(70, 54);
    display.print("FAIL");
  }

  display.display();
}

void printStatus(
    int rawValue,
    float rawFlow,
    float smoothFlow)
{
  Serial.print("ADC: ");
  Serial.print(rawValue);

  Serial.print(" | Smjer: ");

  int offset = rawValue - ADC_CENTER;

  if (abs(offset) < DEAD_ZONE)
    Serial.print("MIROVANJE");
  else if (offset > 0)
    Serial.print("UDAH");
  else
    Serial.print("IZDAH");

  Serial.print(" | Sirovi protok: ");
  Serial.print(rawFlow, 1);

  Serial.print(" ml/s");

  Serial.print(" | Filtrirani protok: ");
  Serial.print(smoothFlow, 1);

  Serial.print(" ml/s");

  Serial.print(" | Zona: ");

  if (smoothFlow < FLOW_LEVEL_1)
    Serial.print("LOW");
  else if (smoothFlow < FLOW_LEVEL_2)
    Serial.print("MED");
  else if (smoothFlow < FLOW_LIMIT)
    Serial.print("TARGET");
  else
    Serial.print("DANGER");

  Serial.print(" | Volumen: ");
  Serial.print(totalVolume, 1);
  Serial.print(" ml");

  if (stable)
  {
    Serial.print(" | Stabilno: ");
    Serial.print((millis() - stableStart) / 1000.0, 1);
    Serial.print(" s");
  }

  if (success)
    Serial.print(" | STATUS: USPJEH");

  if (failed)
    Serial.print(" | STATUS: NEUSPJEH");

  Serial.println();
}

// ================= SETUP =================
void setup()
{
  Serial.begin(115200);

  pinMode(RESET_BTN, INPUT_PULLUP);

  Wire.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    while (true);

  display.clearDisplay();
  display.display();
}

// ================= LOOP =================
void loop()
{
  // RESET BUTTON
  if (digitalRead(RESET_BTN) == LOW)
  {
    resetExercise();
    Serial.println("[RESET] Novi pokusaj pokrenut");
    delay(200);
  }

  unsigned long now = millis();

  if (now - lastSample >= SAMPLE_MS)
  {
    float dt = (now - lastSample) / 1000.0;
    lastSample = now;

    int raw = analogRead(POT_PIN);

    float rawFlow = calculateFlow(raw);

    float flow = ema(rawFlow);

    printStatus(
        raw,
        rawFlow,
        flow);
    
    updateLogic(flow, dt);
    updateWindmill(flow);

    drawOLED(flow);
  }
}