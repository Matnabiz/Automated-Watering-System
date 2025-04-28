#include <Servo.h>

#define SOIL_SENSOR_PIN A0
#define DC_WATER_MOTOR_PIN 2
#define SERVO_PIN 8


int soilMoisture = 0;
Servo potRotator;

void setup() {
  pinMode(DC_WATER_MOTOR_PIN, OUTPUT);
  digitalWrite(DC_WATER_MOTOR_PIN, LOW);
  potRotator.attach(SERVO_PIN);
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
  if (soilMoisture > 850){
    potRotator.write(30);
  }
  else{
    potRotator.write(90);
  }
  delay(500);
}
