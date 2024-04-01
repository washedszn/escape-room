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
  
  // Map the potentiometer value to a range from 0 to 9999 for the 7-segment display
  int displayValue = map(potValue, 0, 1023, 0, 9999);

  // Display the potentiometer value on the TM1638 7-segment display
  char valueToShow[5]; // TM1638plus library requires a char array
  sprintf(valueToShow, "%04d", displayValue);
  tm1638.displayText(valueToShow);

  // Display the potentiometer value on the LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Potentiometer:");
  lcd.setCursor(0, 1);
  lcd.print(displayValue);

  // Use the potentiometer value to generate a tone
  tone(BUZZER_PIN, map(potValue, 0, 1023, 100, 2000)); // The frequency range is arbitrary

  // Change the RGB LED color based on potentiometer value
  // As an example, we'll just map the value to the red channel
  setColor(map(potValue, 0, 1023, 0, 255), 0, 0); // Red color intensity changes with potentiometer

  // Debug output
  Serial.print("Potentiometer Value: ");
  Serial.println(potValue);

  delay(500); // Update every half second

  // Turn off the tone
  noTone(BUZZER_PIN);
  
  // Optionally, turn off the LED or set to a different color
  // setColor(0, 0, 0); // Turn off the RGB LED
}
