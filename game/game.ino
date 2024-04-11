#include "HardwareConfig.h"
#include <LiquidCrystal_I2C.h>
#include "PotentiometerLock.h"
#include "RedLightGreenLight.h"
#include "MorseCodeChallenge.h"
#include "MelodyMe.h"
#include "Utilities.h"

// Global challenge instances
PotentiometerLock potentiometerLock;
RedLightGreenLight redLightGreenLight;
MorseCodeChallenge morseCodeChallenge;
MelodyMe melodyMe;

// Game Constants
const unsigned long gameDuration = 900000; // 15 minutes in milliseconds

// Game State
unsigned long gameStartTime;
bool gameRunning = false;
int currentChallenge = 0;

// Function Prototypes
void updateTimerDisplay();
void runChallengesSequentially();

void setup() {
  initializeHardware();

  // Start the game
  gameStartTime = millis();
  gameRunning = true;

  // Immediately start with the challenges
  runChallengesSequentially();
}

void loop() {
  if (gameRunning) {
    updateTimerDisplay();
    // Challenges are run sequentially from setup, no need to call anything here
  }
}

void updateTimerDisplay() {
  unsigned long currentTime = millis();
  unsigned long timePassed = currentTime - gameStartTime;
  unsigned long timeLeft = gameDuration - timePassed;

  if (timeLeft <= 0) {
    // Game over
    gameRunning = false;
    // Display game over message
    tm1638.displayText("TIME UP");
    lcd.clear();
    lcd.print("Time's up!");
  } else {
    // Update timer display
    int minutes = (timeLeft / 60000);
    int seconds = (timeLeft % 60000) / 1000;
    char timerDisplay[5]; // TM1638plus library requires a char array
    sprintf(timerDisplay, "%02d%02d", minutes, seconds);
    tm1638.displayText(timerDisplay); // Display on the right side
  }
}
void runChallengesSequentially() {
  // Run each challenge in order
  lcd.clear();
  lcd.print("Potentiometer Lock");
  delay(1000);
  potentiometerLock.run();

  lcd.clear();
  lcd.print("Red Light Green Light");
  delay(1000);
  redLightGreenLight.run();

  lcd.clear();
  lcd.print("Morse Code Challenge");
  delay(1000);
  morseCodeChallenge.run();

  lcd.clear();
  lcd.print("Melody Me");
  delay(1000);
  melodyMe.run();

  // After all challenges, perhaps show a completion message or loop back
  lcd.clear();
  lcd.print("All Challenges Complete!");
  gameRunning = false; // Stop the game or loop back to start
}