
// // SMARTKEMBO DISCOVERY SCIENCE //
// #include <LiquidCrystal.h> //LCD Library

// #define NOTE_C4 262 
// #define NOTE_D4 294 
// #define NOTE_E4 330 
// #define NOTE_F4 349 
// #define NOTE_G4 392 
// #define NOTE_A4 440 
// #define NOTE_B4 494 
// #define NOTE_C5 523 

// int temp;
// int T_Sensor = A3;
// int M_Sensor = A0;
// int W_led = 7;
// int P_led = 13;
// int Speaker = 9;
// int val;
// int cel;  

// LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// void setup()
//  {
//     lcd.begin(16, 2);
//     lcd.clear(); 
//     pinMode(13,OUTPUT);
//     pinMode(7,INPUT);
//     pinMode(9,OUTPUT);
    
//     val = analogRead(T_Sensor); //Read Temperature sensor value 
//     int mv = ( val/1024.0)*5000; 
//     cel = mv/10;
    
  
    
//     lcd.setCursor(0,0);
//     lcd.print("PROJECT BY");
//     lcd.setCursor(0,1);
//     lcd.print("SMARTKEMBO ");
//     delay(1000);
   
// }

// void loop()
// {
  
//   lcd.clear();
//   int Moisture = analogRead(M_Sensor); //Read Moisture Sensor Value 

//   lcd.setCursor(0,0);
//   lcd.print("TEMP:");
//   lcd.setCursor(5,0);
//   lcd.print(cel);
//   lcd.setCursor(7,0);
//   lcd.print("*C");
 
//  if (Moisture> 700)   // for dry soil
//   { 
//         lcd.setCursor(11,0);
//         lcd.print("DRY");
//         lcd.setCursor(11,1);
//         lcd.print("SOIL");
//        if (digitalRead(W_led)==1) //test the availability of water in storage
//        {
//          digitalWrite(13, HIGH);
//          lcd.setCursor(0,1);
//          lcd.print("PUMP:ON");
//        }
//        else
//        {
//          digitalWrite(13, LOW);
//          lcd.setCursor(0,1);
//          lcd.print("PUMP:OFF");
           
//            tone(Speaker, NOTE_C4, 500); 
//            delay(500); 
//            tone(Speaker, NOTE_D4, 500);
//            delay(500); 
//            tone(Speaker, NOTE_E4, 500); 
//            delay(500); 
//            tone(Speaker, NOTE_F4, 500); 
//            delay(500); 
//            tone(Speaker, NOTE_G4, 500); 
//            delay(500);
//        }
//     }
 
//      if (Moisture>= 300 && Moisture<=700) //for Moist Soil
//     { 
//       lcd.setCursor(11,0);
//      lcd.print("MOIST");
//      lcd.setCursor(11,1);
//      lcd.print("SOIL");
//      digitalWrite(13,LOW);
//      lcd.setCursor(0,1);
//      lcd.print("PUMP:OFF");    
//   }
 
//   if (Moisture < 300)  // For wet soil
//   { 
//     lcd.setCursor(11,0);
//      lcd.print("WET");
//      lcd.setCursor(11,1);
//      lcd.print("SOIL");
//      digitalWrite(13,LOW);
//      lcd.setCursor(0,1);
//      lcd.print("PUMP:OFF");
//   }
//  delay(1000);    
// }  


// SMARTKEMBO DISCOVERY SCIENCE + Master Communication //
#include <LiquidCrystal.h>
#include <Wire.h>

// Notes for Buzzer
#define NOTE_C4 262 
#define NOTE_D4 294 
#define NOTE_E4 330 
#define NOTE_F4 349 
#define NOTE_G4 392 

// Pin Definitions
#define T_SENSOR_PIN A3
#define M_SENSOR_PIN A0
#define W_LED_PIN 7
#define PUMP_PIN 13
#define SPEAKER_PIN 9
#define SLAVE_ADDRESS 8

// Variables
int tempSensorVal;
int cel;
int soilMoisture;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Setup
void setup() {
  lcd.begin(16, 2);
  lcd.clear(); 

  pinMode(PUMP_PIN, OUTPUT);
  pinMode(W_LED_PIN, INPUT);
  pinMode(SPEAKER_PIN, OUTPUT);
  
  Wire.begin(SLAVE_ADDRESS);   // Join I2C bus as Slave
  Wire.onRequest(sendDataToMaster);
  Wire.onReceive(receiveCommandFromMaster);

  Serial.begin(9600);

  lcd.setCursor(0,0);
  lcd.print("PROJECT BY");
  lcd.setCursor(0,1);
  lcd.print("SMARTKEMBO");
  delay(2000);  
}

// Loop
void loop() {
  lcd.clear();

  // Read Sensors
  tempSensorVal = analogRead(T_SENSOR_PIN);
  soilMoisture = analogRead(M_SENSOR_PIN);

  int mv = (tempSensorVal / 1024.0) * 5000;
  cel = mv / 10; // Celsius

  // Display Temperature
  lcd.setCursor(0,0);
  lcd.print("TEMP:");
  lcd.setCursor(5,0);
  lcd.print(cel);
  lcd.setCursor(7,0);
  lcd.print("*C");

  // Soil Moisture Logic
  if (soilMoisture > 700) { // Dry soil
    lcd.setCursor(11,0);
    lcd.print("DRY");
    lcd.setCursor(11,1);
    lcd.print("SOIL");

    if (digitalRead(W_LED_PIN) == HIGH) { // Water available
      digitalWrite(PUMP_PIN, HIGH);
      lcd.setCursor(0,1);
      lcd.print("PUMP:ON");
    } else { // No water
      digitalWrite(PUMP_PIN, LOW);
      lcd.setCursor(0,1);
      lcd.print("PUMP:OFF");
      playNoWaterAlarm();
    }

  } else if (soilMoisture >= 300 && soilMoisture <= 700) { // Moist soil
    lcd.setCursor(11,0);
    lcd.print("MOIST");
    lcd.setCursor(11,1);
    lcd.print("SOIL");
    digitalWrite(PUMP_PIN, LOW);
    lcd.setCursor(0,1);
    lcd.print("PUMP:OFF");

  } else if (soilMoisture < 300) { // Wet soil
    lcd.setCursor(11,0);
    lcd.print("WET");
    lcd.setCursor(11,1);
    lcd.print("SOIL");
    digitalWrite(PUMP_PIN, LOW);
    lcd.setCursor(0,1);
    lcd.print("PUMP:OFF");
  }

  delay(1000);
}

// Send soil moisture when master requests
void sendDataToMaster() {
  Wire.write((soilMoisture >> 8) & 0xFF);
  Wire.write(soilMoisture & 0xFF);
}

// Receive commands from master (optional, currently just prints them)
void receiveCommandFromMaster(int bytes) {
  if (bytes > 0) {
    char command = Wire.read();
    Serial.print("Received Command: ");
    Serial.println(command);
    // You can add extra logic based on received command if needed
  }
}

// Play alarm if no water
void playNoWaterAlarm() {
  tone(SPEAKER_PIN, NOTE_C4, 500);
  delay(500);
  tone(SPEAKER_PIN, NOTE_D4, 500);
  delay(500);
  tone(SPEAKER_PIN, NOTE_E4, 500);
  delay(500);
  tone(SPEAKER_PIN, NOTE_F4, 500);
  delay(500);
  tone(SPEAKER_PIN, NOTE_G4, 500);
  delay(500);
}
