#include "HardwareConfig.h"
#include <LiquidCrystal_I2C.h>
#include "PotentiometerLock.h"
#include "RedLightGreenLight.h"
#include "MorseCodeChallenge.h"
#include "MelodyMe.h"
#include "Utilities.h"

// Game Constants
const unsigned long gameDuration = 900000; // 15 minutes in milliseconds
unsigned long buttonPressedTime = 0;
const unsigned long longPressTime = 1000;

// Game State
unsigned long gameStartTime;
bool gameRunning = false;
bool challengeCompleted[4] = {false, false, false, false}; // Assuming 4 challenges
int currentChallenge = 0;

enum GameState {
  MainMenu,
  InChallenge
};

GameState currentState = MainMenu;

// Custom character bytes
byte completedSymbol[8] = {
  0b00000,
  0b00001,
  0b00010,
  0b10100,
  0b01100,
  0b00000,
  0b00000,
  0b00000
};

byte incompleteSymbol[8] = {
  0b00000,
  0b10001,
  0b01010,
  0b00100,
  0b01010,
  0b10001,
  0b00000,
  0b00000
};

byte selectedSymbol[8] = {
  0b00000,
  0b00100,
  0b01110,
  0b11111,
  0b01110,
  0b00100,
  0b00000,
  0b00000
};

// Function to create custom characters
void createCustomCharacters() {
  lcd.createChar(0, completedSymbol);
  lcd.createChar(1, incompleteSymbol);
  lcd.createChar(2, selectedSymbol);
}

// Function Prototypes
void updateTimerDisplay();
void checkButtonPress();
void displayMainMenu();
void runChallenge(int challengeNumber);
void completeChallenge(int challengeNumber);

void setup() {
  initializeHardware();
  createCustomCharacters();

  // Set up Nucleo-64 button pin
  pinMode(USER_BTN, INPUT); // Replace USER_BTN with the actual button pin number

  // Start the game
  gameStartTime = millis();
  gameRunning = true;
  displayMainMenu();
}

void loop() {
  if (gameRunning) {
    updateTimerDisplay();
    checkButtonPress();
    checkChallengeCompletion();
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

void checkButtonPress() {
  bool buttonState = digitalRead(USER_BTN);
  static bool lastButtonState = HIGH; // Assume button starts unpressed
  static unsigned long buttonPressedTime = 0;
  unsigned long now = millis();

  // Button press detected
  if (buttonState == LOW && lastButtonState == HIGH) {
    buttonPressedTime = now; // Record time button was pressed
  }

  // Button release detected
  if (buttonState == HIGH && lastButtonState == LOW) {
    unsigned long pressDuration = now - buttonPressedTime;
    
    if (currentState == MainMenu) {
      if (pressDuration < longPressTime) {
        // Short press: Cycle through challenges
        currentChallenge = (currentChallenge + 1) % 4; // Assuming 4 challenges
        displayMainMenu();
      } else {
        // Long press: Select the current challenge
        currentState = InChallenge;
        runChallenge(currentChallenge);
      }
    } else if (currentState == InChallenge) {
      // Long press while in a challenge returns to the main menu
      if (pressDuration >= longPressTime) {
        currentState = MainMenu;
        displayMainMenu();
      }
    }
  }

  lastButtonState = buttonState;
}

void displayMainMenu() {
  lcd.clear();
  lcd.print("Select Challenge");
  lcd.setCursor(0, 1);

  // Display the custom symbols for each challenge status
  for (int i = 0; i < 4; i++) {
    if (i == currentChallenge) {
      lcd.write(byte(2)); // Display selected symbol
    } else if (challengeCompleted[i]) {
      lcd.write(byte(0)); // Display completed symbol
    } else {
      lcd.write(byte(1)); // Display incomplete symbol
    }
  }
}

// Global challenge instances
PotentiometerLock potentiometerLock;
RedLightGreenLight redLightGreenLight;
MorseCodeChallenge morseCodeChallenge;
MelodyMe melodyMe;

void runChallenge(int challengeNumber) {
  lcd.clear();
  
  switch (challengeNumber) {
    case 0:
      lcd.print("Potentiometer Lock");
      potentiometerLock.run();
      break;
    case 1:
      lcd.print("Red Light Green Light");
      redLightGreenLight.run();
      break;
    case 2:
      lcd.print("Morse Code Challenge");
      morseCodeChallenge.run();
      break;
    case 3:
      lcd.print("Melody Me");
      melodyMe.run();
      break;
    default:
      lcd.print("Unknown Challenge");
      break;
  }

  // After initiating the challenge, switch the game state
  currentState = InChallenge;
}

void checkChallengeCompletion() {
  if (currentState != InChallenge) return;

  bool completed = false;

  switch (currentChallenge) {
    case 0:
      completed = potentiometerLock.isCompleted();
      break;
    case 1:
      completed = redLightGreenLight.isCompleted();
      break;
    case 2:
      completed = morseCodeChallenge.isCompleted();
      break;
    case 3:
      completed = melodyMe.isCompleted();
      break;
  }

  if (completed) {
    lcd.clear();
    lcd.print("Challenge Completed");
    delay(2000); // Give some time to read the message
    completeChallenge(currentChallenge);
  }
}

void completeChallenge(int challengeNumber) {
  // Code to mark a challenge as complete
  challengeCompleted[challengeNumber] = true;
  displayMainMenu();
}