#include <Wire.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 8, 9, 10, 11, 12); // RS, E, D4, D5, D6, D7

char receivedChar = ' ';

void setup() {
  Wire.begin(0x50);  // Slave address
  Wire.onReceive(receiveEvent);
  
  lcd.begin(16, 2);
  lcd.print("Waiting...");
  Serial.begin(9600);
}

void loop() {
  delay(100);
}

void receiveEvent(int howMany) {
  if (Wire.available()) {
    receivedChar = Wire.read();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("RX: ");
    lcd.print(receivedChar);
    
    Serial.print("Received from STM32: ");
    Serial.println(receivedChar);
  }
}
