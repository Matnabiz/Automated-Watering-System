#include <Wire.h>

#define SOIL_SENSOR_PIN A0
#define DC_WATER_MOTOR_PIN 2


int soilMoisture = 0;

void setup() {
  pinMode(DC_WATER_MOTOR_PIN, OUTPUT);
  digitalWrite(DC_WATER_MOTOR_PIN, LOW);
  Serial.begin(9600);
}

void loop() {
  soilMoisture = analogRead(SOIL_SENSOR_PIN);
  Serial.print("Soil Moisture Value: ");
  Serial.println(soilMoisture);
  if (soilMoisture > 850){
    digitalWrite(DC_WATER_MOTOR_PIN, HIGH);
  }
  else {
    digitalWrite(DC_WATER_MOTOR_PIN, LOW);
  }
  delay(500);
}
