#include <Arduino.h>

#define POT_PIN 34

#define ADC_CENTER 2048
#define MAX_FLOW 1300.0
#define DEAD_ZONE 80

#define FILTER_ALPHA 0.2

#define FLOW_LEVEL_1 600.0
#define FLOW_LEVEL_2 900.0
#define FLOW_LIMIT 1200.0

#define REQUIRED_STABLE_TIME_MS 5000
#define SAMPLE_INTERVAL_MS 200

float filteredFlow = 0.0;
float totalVolume = 0.0;

unsigned long lastSampleTime = 0;
unsigned long stableStartTime = 0;

bool stableTimerRunning = false;
bool exerciseSuccess = false;
bool exerciseFailed = false;

float calculateFlow(int rawValue) {
  int offset = rawValue - ADC_CENTER;

  if (abs(offset) < DEAD_ZONE) {
    return 0.0;
  }

  if (offset < 0) {
    return 0.0;
  }

  float flow = (offset / 2047.0) * MAX_FLOW;

  if (flow > MAX_FLOW) {
    flow = MAX_FLOW;
  }

  return flow;
}

float applyEmaFilter(float newValue) {
  filteredFlow = FILTER_ALPHA * newValue + (1.0 - FILTER_ALPHA) * filteredFlow;
  return filteredFlow;
}

String getDirection(int rawValue) {
  int offset = rawValue - ADC_CENTER;

  if (abs(offset) < DEAD_ZONE) {
    return "MIROVANJE";
  }

  if (offset > 0) {
    return "UDAH";
  }

  return "IZDAH";
}

String getFlowZone(float flow) {
  if (flow >= FLOW_LIMIT) {
    return "TOO_HIGH";
  }

  if (flow >= FLOW_LEVEL_2) {
    return "TARGET";
  }

  if (flow >= FLOW_LEVEL_1) {
    return "MEDIUM";
  }

  return "LOW";
}

void resetExercise() {
  stableTimerRunning = false;
  stableStartTime = 0;
  exerciseSuccess = false;
  exerciseFailed = false;
  totalVolume = 0.0;
  filteredFlow = 0.0;
}

void updateVolume(float flow, float deltaTimeSeconds, String direction) {
  if (!exerciseSuccess && !exerciseFailed && direction == "UDAH") {
    totalVolume += flow * deltaTimeSeconds;
  }
}

void updateExerciseState(float flow) {
  if (exerciseSuccess || exerciseFailed) {
    return;
  }

  if (flow >= FLOW_LIMIT) {
    exerciseFailed = true;
    stableTimerRunning = false;
    Serial.println("POKUSAJ PONISTEN: protok je presao 1200 ml/s.");
    Serial.print("Volumen prije ponistenja: ");
    Serial.print(totalVolume, 1);
    Serial.println(" ml");
    return;
  }

  bool inTargetZone = flow >= FLOW_LEVEL_2 && flow < FLOW_LIMIT;

  if (inTargetZone) {
    if (!stableTimerRunning) {
      stableTimerRunning = true;
      stableStartTime = millis();
    }

    unsigned long stableDuration = millis() - stableStartTime;

    if (stableDuration >= REQUIRED_STABLE_TIME_MS) {
      exerciseSuccess = true;
      Serial.println("VJEZBA USPJESNA: protok je stabilan 5 sekundi.");
      Serial.print("Ukupni volumen: ");
      Serial.print(totalVolume, 1);
      Serial.println(" ml");
    }
  } else {
    stableTimerRunning = false;
    stableStartTime = 0;
  }
}

void printStatus(int rawValue, String direction, float rawFlow, float smoothFlow) {
  String zone = getFlowZone(smoothFlow);

  Serial.print("ADC: ");
  Serial.print(rawValue);

  Serial.print(" | Smjer: ");
  Serial.print(direction);

  Serial.print(" | Sirovi protok: ");
  Serial.print(rawFlow, 1);

  Serial.print(" ml/s | Filtrirani protok: ");
  Serial.print(smoothFlow, 1);

  Serial.print(" ml/s | Zona: ");
  Serial.print(zone);

  Serial.print(" | Volumen: ");
  Serial.print(totalVolume, 1);
  Serial.print(" ml");

  if (stableTimerRunning && !exerciseSuccess && !exerciseFailed) {
    float stableSeconds = (millis() - stableStartTime) / 1000.0;
    Serial.print(" | Stabilno: ");
    Serial.print(stableSeconds, 1);
    Serial.print(" s");
  }

  if (exerciseSuccess) {
    Serial.print(" | STATUS: USPJEH");
  }

  if (exerciseFailed) {
    Serial.print(" | STATUS: NEUSPJEH");
  }

  Serial.println();
}

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("BreathMill - volumen i logika vjezbe");
}

void loop() {
  unsigned long currentTime = millis();

  if (currentTime - lastSampleTime >= SAMPLE_INTERVAL_MS) {
    float deltaTimeSeconds = (currentTime - lastSampleTime) / 1000.0;
    lastSampleTime = currentTime;

    int rawValue = analogRead(POT_PIN);

    float rawFlow = calculateFlow(rawValue);
    float smoothFlow = applyEmaFilter(rawFlow);
    String direction = getDirection(rawValue);

    updateVolume(smoothFlow, deltaTimeSeconds, direction);
    updateExerciseState(smoothFlow);
    printStatus(rawValue, direction, rawFlow, smoothFlow);
  }
}