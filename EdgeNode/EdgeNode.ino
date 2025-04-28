#include <Wire.h>

#define SOIL_SENSOR_PIN A0


int soilMoisture = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  soilMoisture = analogRead(SOIL_SENSOR_PIN);
  Serial.print("Soil Moisture Value: ");
  Serial.println(soilMoisture);
  delay(500);
}
