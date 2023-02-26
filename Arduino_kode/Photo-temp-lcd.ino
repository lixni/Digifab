#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

const int temperatureSensorPin = A0;
const int lightSensorPin = A1;

void setup() {
  lcd.begin(16, 2);
  lcd.print("Temp:");
  lcd.setCursor(0, 1);
  lcd.print("Light:");
}

void loop() {
  int temperatureValue = analogRead(temperatureSensorPin);
  int lightValue = analogRead(lightSensorPin);

  float temperatureCelsius = map(temperatureValue, 0, 1023, 0, 150); // convert raw sensor value to temperature in Celsius
  int lightPercentage = map(lightValue, 0, 1023, 0, 100); // convert raw sensor value to percentage

  lcd.setCursor(7, 0);
  lcd.print(temperatureCelsius, 1);
  lcd.print("C");

  lcd.setCursor(7, 1);
  lcd.print(lightPercentage);
  lcd.print("%");

  delay(500);
}
