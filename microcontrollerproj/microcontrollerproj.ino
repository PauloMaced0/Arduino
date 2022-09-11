#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2, 1);
const int sensorPin = A0; //usar o pin a que o sensor de temp esta ligado
const float baselineTemp = 24.0;
const int motorPin = 9;
  
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(motorPin,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int SensorVal = analogRead(sensorPin);
  float voltage = (SensorVal / 1024.0) * 5.0;
  float temperature = (voltage - 0.5) * 100;
  Serial.print("Degrees C: ");
  Serial.print(temperature);

  if(temperature > baselineTemp){
    lcd.print("Degrees C: ");
    lcd.print(temperature);
    lcd.setCursor(0, 1);
    lcd.print("Motor On");
    // lcd.clear();
  
    digitalWrite(motorPin, HIGH);
    
  } else {
    lcd.print("Degrees C: ");
    lcd.print(temperature);

    lcd.setCursor(0, 1);
    lcd.print("Motor Off");
    // lcd.clear();
    
    digitalWrite(motorPin, LOW);
  }
  
}
