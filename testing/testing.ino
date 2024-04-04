#include <LiquidCrystal_I2C.h>
#include <TM1638plus.h>

// Set the pins for the TM1638 module
#define TM1638_DIO 11
#define TM1638_CLK 12
#define TM1638_STB 13

// Initialize TM1638plus
TM1638plus tm1638(TM1638_STB, TM1638_CLK, TM1638_DIO);

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Define the buzzer and potentiometer pins
#define BUZZER_PIN 7
#define POT_PIN A0

// Define RGB LED pins
#define RED_PIN 10
#define GREEN_PIN 9
#define BLUE_PIN 8

void setup() {
  // Initialize the TM1638 module
  tm1638.displayBegin();

  // Initialize the LCD
  lcd.init();
  lcd.backlight();

  // Initialize the buzzer pin as output
  pinMode(BUZZER_PIN, OUTPUT);

  // Initialize RGB LED pins as output
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  
  // Set up serial communication for debugging
  Serial.begin(9600);

  // Greet on the TM1638 7-segment display
  tm1638.displayText("HELLO");
  delay(1000); // Wait for a second

  // Greet on the LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("System Check:");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(2000); // Wait for two seconds
}

void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(RED_PIN, redValue);
  analogWrite(GREEN_PIN, greenValue);
  analogWrite(BLUE_PIN, blueValue);
}

void loop() {
  // Read the potentiometer value
  int potValue = analogRead(POT_PIN);
  int displayValue = map(potValue, 0, 1023, 0, 9999);
  
  // Update the display with the potentiometer value
  char valueToShow[5];
  sprintf(valueToShow, "%04d", displayValue);
  tm1638.displayText(valueToShow);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Potentiometer:");
  lcd.setCursor(0, 1);
  lcd.print(displayValue);

  // Change the RGB LED color based on potentiometer value
  setColor(map(potValue, 0, 1023, 0, 255), 0, 0);

  // Debug output
  Serial.print("Potentiometer Value: ");
  Serial.println(potValue);

  // Use the TM1638 module to check if the last button is pressed
  byte buttons = tm1638.readButtons();
  if (buttons & 0x80) { // Assuming the last button sets the highest bit
    // Mute the buzzer if the last button is pressed
    noTone(BUZZER_PIN);
  } else {
    // Otherwise, generate a tone based on the potentiometer value
    tone(BUZZER_PIN, map(potValue, 0, 1023, 100, 2000));
  }

  delay(500); // Update every half second
}
