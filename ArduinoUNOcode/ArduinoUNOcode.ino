void setup() {
  Serial.begin(9600);
}
void loop() {
  int raw = analogRead(A0);
  float percent = map(raw, 1023, 0, 0, 100);  // برای FC-28
  Serial.print("Moisture: "); Serial.print(percent); Serial.println("%");
  delay(1000);
}
