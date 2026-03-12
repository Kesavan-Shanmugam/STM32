#include <Wire.h>
#include <LiquidCrystal.h>

// LCD connections: RS=7, EN=8, D4=9, D5=10, D6=11, D7=12
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

String receivedData = "";  // To store received word

void setup() {
  Wire.begin(0x50);  // Slave I2C address (same as STM32 target)
  Wire.onReceive(receiveEvent);

  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Waiting...");
  
  Serial.begin(9600);
}

void loop() {
  delay(100);
}

// This function triggers automatically when STM32 sends data
void receiveEvent(int howMany) {
  receivedData = "";  // Clear previous data

  while (Wire.available()) {
    char c = Wire.read();   // Read one character
    receivedData += c;      // Append to string
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("RX: ");
  lcd.print(receivedData);

  Serial.print("Received from STM32: ");
  Serial.println(receivedData);
}
