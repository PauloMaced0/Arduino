#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int sensorPin = A0; //usar o pin a que o sensor de temp esta ligado
volatile float temperature = 0.0;

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
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.begin(16, 2);
  
  setup_timer_1();

  //Setup Enable interrupts
  interrupts();
}

void loop(void) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temperature:"); 
  lcd.setCursor(0, 1);
  lcd.print(temperature);
  lcd.createChar(6, customShape); // Create a custom character at position 0
  lcd.setCursor(6, 1);
  lcd.write((byte)6); // Display the custom shape on the LCD
  lcd.print("C");
  delay(100); // f = 10hz
}
