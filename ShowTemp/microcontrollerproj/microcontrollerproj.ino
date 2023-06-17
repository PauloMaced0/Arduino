#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

#define rxPin 10
#define txPin 11

SoftwareSerial bltSerial(rxPin, txPin);

LiquidCrystal lcd(13, 12, 5, 4, 3, 2);
const int sensorPin = A0; //usar o pin a que o sensor de temp esta ligado
float temperature = 0.0;
float highestTemp = 0.0;
float lowestTemp = 100.0;

byte customShape[] = {
  B00000,
  B00111,
  B00101,
  B00111,
  B00000,
  B00000,
  B00000,
  B00000
};

void setup_timer_1(void) {
 // Timer1 Configuration
  TCCR1A = 0; // Clear Timer1 control register A
  TCCR1B = 0; // Clear Timer1 control register B

  // Set Timer1 to CTC mode with prescaler 64
  TCCR1B |= (1 << WGM12) | (1 << CS12);

  // Set compare value for desired frequency on Timer1
  OCR1A = 31249; // (16MHz / (256 * 2)) - 1

  // Enable Timer1 compare match interrupt
  TIMSK1 |= (1 << OCIE1A);
}

ISR(TIMER1_COMPA_vect) {
  int SensorVal = analogRead(sensorPin);
  float voltage = (SensorVal / 1024.0) * 5.0;
  temperature = (voltage - 0.5) * 100;
  TIFR1 |= (1 << OCF1A);  // Clear Timer2 Compare Match A interrupt flag
}

void setup(void) {
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  
  bltSerial.begin(9600);
  lcd.begin(16, 2);
  
  setup_timer_1();

  interrupts();
}

void loop(void) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temperature:"); 
  lcd.setCursor(0, 1);
  lcd.print(temperature);
  lcd.createChar(6, customShape); 
  lcd.setCursor(6, 1);
  lcd.write((byte)6);
  lcd.print("C");

  if (temperature > highestTemp) {
     highestTemp = temperature;
  } 

  if (temperature < lowestTemp) {
     lowestTemp = temperature;
  }
  
  while (bltSerial.available() > 0) {
    char receiveData = (char) bltSerial.read();
    
    switch (receiveData) {
      case 'c':
        bltSerial.print(temperature);
        bltSerial.println(" ºC");
        break;
      case 'h':
        bltSerial.print(highestTemp);
        bltSerial.println(" ºC");
        break;
      case 'l':
        bltSerial.print(lowestTemp);
        bltSerial.println(" ºC");
      default:
        break;
    }
  }
  
  delay(100);
}
