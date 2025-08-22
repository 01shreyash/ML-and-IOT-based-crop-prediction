// Blynk Smart Plant Monitoring System with LCD + Serial Output for ML
#define BLYNK_TEMPLATE_ID "TMPL3KZyp6GTt"
#define BLYNK_TEMPLATE_NAME "Smart Plant Monitoring System"

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

// LCD: I2C address 0x27 (change to 0x3F if needed), 16 cols, 2 rows
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Sensor and network configuration
#define DHTPIN D4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

char auth[] = "gUS3gVzz8FvaQkCfALq2BvUMGNCEl34F";
char ssid[] = "YJ";
char pass[] = "12345678";

// Pins
#define RELAY_PIN_1 D3
#define PUSH_BUTTON_1 D7
#define VPIN_BUTTON_1 V12
#define PIR D5
#define soil A0

// Global state
int relay1State = LOW;
int pushButton1State = HIGH;
int PIR_ToggleValue = 0;

float temperature = 0;
int moisturePercent = 0;

BlynkTimer timer;
WidgetLED motionLED(V5); // Blynk LED widget for motion

void setup() {
  Serial.begin(9600);
  Wire.begin(D2, D1);    // SDA = D2, SCL = D1
  lcd.init();
  lcd.backlight();
  delay(1000);

  lcd.setCursor(0, 0);
  lcd.print("LCD Check OK");
  delay(2000);
  lcd.clear();

  pinMode(RELAY_PIN_1, OUTPUT);
  digitalWrite(RELAY_PIN_1, relay1State);

  pinMode(PUSH_BUTTON_1, INPUT_PULLUP);
  pinMode(PIR, INPUT);

  dht.begin();
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);

  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();
  lcd.setCursor(11, 1);
  lcd.print("W:OFF");

  timer.setInterval(2000L, soilMoistureSensor);
  timer.setInterval(2000L, DHT11sensor);
  timer.setInterval(500L, checkPhysicalButton);
}

void DHT11sensor() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  temperature = t;
  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);

  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(t, 1);
  lcd.print(" H:");
  lcd.print(h, 1);
  lcd.print(" ");
}

void soilMoistureSensor() {
  int value = analogRead(soil);
  value = map(value, 0, 1024, 0, 100);
  value = (value - 100) * -1;

  moisturePercent = value;
  Blynk.virtualWrite(V3, value);

  lcd.setCursor(0, 1);
  lcd.print("S:");
  lcd.print(value);
  lcd.print("% ");

  // ✅ Serial output for ML model
  Serial.flush();  // Optional: ensures buffer is sent out cleanly before writing new line
  Serial.print("Moisture:");
  Serial.print(moisturePercent);
  Serial.print(",Temperature:");
  Serial.println(temperature);
}

void PIRsensor() {
  bool value = digitalRead(PIR);
  if (value) {
    Blynk.logEvent("motiondetection", "WARNING! Motion Detected!");
    motionLED.on();
  } else {
    motionLED.off();
  }
}

BLYNK_WRITE(V6) {
  PIR_ToggleValue = param.asInt();
}

BLYNK_CONNECTED() {
  Blynk.syncVirtual(VPIN_BUTTON_1);
}

BLYNK_WRITE(VPIN_BUTTON_1) {
  relay1State = param.asInt();
  digitalWrite(RELAY_PIN_1, relay1State);
}

void checkPhysicalButton() {
  if (digitalRead(PUSH_BUTTON_1) == LOW) {
    if (pushButton1State != LOW) {
      relay1State = !relay1State;
      digitalWrite(RELAY_PIN_1, relay1State);
      Blynk.virtualWrite(VPIN_BUTTON_1, relay1State);
    }
    pushButton1State = LOW;
  } else {
    pushButton1State = HIGH;
  }
}

void loop() {
  if (PIR_ToggleValue == 1) {
    lcd.setCursor(5, 1);
    lcd.print("M:ON ");
    PIRsensor();
  } else {
    lcd.setCursor(5, 1);
    lcd.print("M:OFF");
    motionLED.off();
  }

  if (relay1State == HIGH) {
    lcd.setCursor(11, 1);
    lcd.print("W:ON ");
  } else {
    lcd.setCursor(11, 1);
    lcd.print("W:OFF");
  }

  Blynk.run();
  timer.run();
}
