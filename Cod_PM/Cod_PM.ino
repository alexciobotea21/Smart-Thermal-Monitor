#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Senzor temperatura DS18B20 pe D7
#define ONE_WIRE_BUS 7
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Pini
#define SWITCH_PIN 8
#define POT_PIN A0
#define BUZZER_PIN 2
#define BUTON_ONOFF 5

// LED-uri: D6 (PD6), D9 (PB1), D10 (PB2), D11 (PB3)
const byte LED_MASK_D = (1 << PD6);                  // D6
const byte LED_MASK_B = (1 << PB1) | (1 << PB2) | (1 << PB3); // D9, D10, D11

// Prag temperatura alerta
const float TEMP_PRAG = 28.0;

bool sistemPornit = false;

void setup() {
  // LCD
  lcd.begin(16, 2);
  lcd.backlight();

  // Senzor
  sensors.begin();

  // Serial
  Serial.begin(9600);

  // Pini
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  pinMode(BUTON_ONOFF, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  // Config LED-uri prin registre
  DDRD |= LED_MASK_D;    // D6 ca OUTPUT
  DDRB |= LED_MASK_B;    // D9, D10, D11 ca OUTPUT

  // Initial LED-uri oprite
  PORTD &= ~LED_MASK_D;
  PORTB &= ~LED_MASK_B;
}

void loop() {
  // Toggle sistem ON/OFF
  static bool lastButon = HIGH;
  bool currentButon = digitalRead(BUTON_ONOFF);
  if (lastButon == HIGH && currentButon == LOW) {
    sistemPornit = !sistemPornit;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(sistemPornit ? "Sistem pornit" : "Sistem oprit");
    delay(300);
  }
  lastButon = currentButon;

  if (!sistemPornit) {
    // Sistem inactiv
    PORTD &= ~LED_MASK_D;
    PORTB &= ~LED_MASK_B;
    digitalWrite(BUZZER_PIN, LOW);
    return;
  }

  // Mod test
  bool modTest = digitalRead(SWITCH_PIN) == LOW;
  float tempC;

  if (modTest) {
    int potValue = analogRead(POT_PIN);
    tempC = map(potValue, 0, 1023, 150, 400) / 10.0;
  } else {
    sensors.requestTemperatures();
    tempC = sensors.getTempCByIndex(0);
  }

  // Afisare pe LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(modTest ? "Mod Test" : "Mod Real");

  lcd.setCursor(0, 1);
  if (tempC != DEVICE_DISCONNECTED_C) {
    lcd.print("Temp: ");
    lcd.print(tempC, 1);
    lcd.print((char)223);
    lcd.print("C");

    Serial.print(modTest ? "[TEST] Temp: " : "[REAL] Temp: ");
    Serial.println(tempC);

    // ALERTA
    if (tempC >= TEMP_PRAG) {
      static unsigned long lastStep = 0;
      static int ledIndex = 0;

      if (millis() - lastStep >= 150) {
        lastStep = millis();
        // Faza LED-uri: doar unul aprins la un moment dat
        for (int i = 0; i < 4; i++) {
          if (i == ledIndex) {
            if (i == 0) PORTD |= (1 << PD6);
            else PORTB |= (1 << (PB1 + i - 1));
          } else {
            if (i == 0) PORTD &= ~(1 << PD6);
            else PORTB &= ~(1 << (PB1 + i - 1));
          }
        }
        ledIndex = (ledIndex + 1) % 4;
      }
      digitalWrite(BUZZER_PIN, HIGH);
      Serial.println("==> ALERTA!");
    } else {
      // LED-uri aprinse constant
      PORTD |= LED_MASK_D;
      PORTB |= LED_MASK_B;
      digitalWrite(BUZZER_PIN, LOW);
    }
  } else {
    lcd.print("Senzor error");
    Serial.println("Eroare DS18B20!");
    PORTD &= ~LED_MASK_D;
    PORTB &= ~LED_MASK_B;
    digitalWrite(BUZZER_PIN, LOW);
  }

  delay(100);
}
