#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT11
#define SOIL_MOISTURE_PIN A0
#define RELAY_PIN 7

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

int soilMoistureValue = 0;
int soilMoistureThreshold = 300; // Set the soil moisture threshold

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH); // Turn off the relay initially
  lcd.init();
  lcd.backlight();
  dht.begin();
  Serial.begin(9600);
}

void loop() {
  // Read soil moisture
  soilMoistureValue = analogRead(SOIL_MOISTURE_PIN);

  // Read temperature and humidity
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    lcd.setCursor(0, 0);
    lcd.print("Sensor error");
    return;
  }

  // Display data on LCD
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print(humidity);
  lcd.print(" %");

  delay(2000); // Wait for 2 seconds

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Moisture:");
  lcd.setCursor(0, 0);
  lcd.print(soilMoistureValue);

  // Control the relay based on soil moisture value
  if (soilMoistureValue < soilMoistureThreshold) {
    digitalWrite(RELAY_PIN, LOW); // Turn on the water pump
    lcd.setCursor(0, 1);
    lcd.print("Watering...");
  } else {
    digitalWrite(RELAY_PIN, HIGH); // Turn off the water pump
  }

  delay(2000); // Wait for 2 seconds
  lcd.clear();
}

