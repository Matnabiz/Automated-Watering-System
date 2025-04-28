#include <Wire.h>

#define EDGE_NODE_ADDRESS 8

#define SOIL_MOISTURE_THRESHOLD_HIGH 800
#define SOIL_MOISTURE_THRESHOLD_LOW 500

unsigned long lastRequestTime = 0;
const unsigned long requestInterval = 2000;

void setup() {
  Wire.begin();
  Serial.begin(9600);
}

void loop() {
  unsigned long currentTime = millis();
  
  if (currentTime - lastRequestTime > requestInterval) {
    lastRequestTime = currentTime;

    int soilMoisture = requestSoilMoisture(EDGE_NODE_ADDRESS);
    Serial.print("Edge Node Soil Moisture: ");
    Serial.println(soilMoisture);

    process(EDGE_NODE_ADDRESS, soilMoisture);
  }
}

int requestSoilMoisture(byte nodeAddress) {
  Wire.requestFrom(nodeAddress, 2);
  int moisture = 0;
  if (Wire.available() >= 2) {
    int highByte = Wire.read();
    int lowByte = Wire.read();
    moisture = (highByte << 8) | lowByte;
  }
  return moisture;
}


void process(byte nodeAddress, int soilMoisture) {
  if (soilMoisture > SOIL_MOISTURE_THRESHOLD_HIGH) {
    sendCommand(nodeAddress, 'S');
  } 
  else if (soilMoisture < SOIL_MOISTURE_THRESHOLD_LOW) {
    sendCommand(nodeAddress, 'W');
  } 
  else {
    sendCommand(nodeAddress, 'N');
  }
}

void sendCommand(byte nodeAddress, char command) {
  Wire.beginTransmission(nodeAddress);
  Wire.write(command);
  Wire.endTransmission();
}
