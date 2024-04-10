#include "PotentiometerLock.h"
#include "HardwareConfig.h"

// Combination numbers for each lock
const int lockCombinations[3] = {250, 500, 750};
int currentLock = 0; // Currently selected lock

// Custom character for a locked lock
byte lockedLock[8] = {
  0b01110, //   ***
  0b10001, //  *   *
  0b10001, //  *   *
  0b11111, //  *****
  0b11011, //  ** **
  0b11011, //  ** **
  0b11111, //  *****
  0b00000  //       
};

// Custom character for an unlocked (solved) lock
byte unlockedLock[8] = {
  0b01110, //   ***
  0b10001, //  *   *
  0b10000, //  *   
  0b11110, //  ****
  0b11011, //  ** **
  0b11011, //  ** **
  0b11111, //  *****
  0b00000  //  
};

PotentiometerLock::PotentiometerLock() : completed(false) {
    lockState[0] = lockState[1] = lockState[2] = false; // Initially, all locks are locked
}

void PotentiometerLock::run() {
    lcd.init(); // Initialize the LCD (if not already done)
    lcd.backlight(); // Ensure the backlight is on
    
    createLockCharacters(); // Create the custom characters for the lock
    
    updateDisplay(); // Initial display update
    
    while(!isCompleted()) {
        checkTM1638ButtonPress();
        checkCombination();
        updateTimerDisplay();
        delay(100); // Short delay to debounce button presses and slow down loop iteration
    }
}

void PotentiometerLock::createLockCharacters() {
    lcd.createChar(0, lockedLock);
    lcd.createChar(1, unlockedLock);
}

void PotentiometerLock::checkTM1638ButtonPress() {
    uint8_t buttons = tm1638.readButtons(); // Read the state of the buttons
    if (buttons & 0x80) { // Assuming the right-most button corresponds to the 0x80 bit
        cycleLocks(); // Cycle to the next lock
        delay(300); // Debounce delay
    }
}

void PotentiometerLock::updateDisplay() {
    lcd.clear();
    lcd.setCursor(0, 0);
    
    int potValue = analogRead(POTENTIOMETER_PIN); // Read potentiometer value
    lcd.print("Code: ");
    lcd.print(potValue);

    lcd.setCursor(0, 1);
    for(int i = 0; i < 3; i++) {
        if(lockState[i]) {
            lcd.write(byte(1)); // Unlocked lock character
        } else {
            lcd.write(byte(0)); // Locked lock character
        }
    }
}

void PotentiometerLock::checkCombination() {
    int potValue = analogRead(POTENTIOMETER_PIN);
    if(abs(potValue - lockCombinations[currentLock]) < 10) { // Assuming a threshold for correct value
        lockState[currentLock] = true; // Unlock the lock
        tone(BUZZER_PIN, 1000, 200); // Play a tone (1000 Hz for 200 ms)
    }
    updateDisplay(); // Update the display to reflect any changes
}

void PotentiometerLock::cycleLocks() {
    currentLock = (currentLock + 1) % 3; // Cycle through the locks
    updateDisplay(); // Refresh the display to show current lock selection
}

bool PotentiometerLock::isCompleted() {
    for(int i = 0; i < 3; i++) {
        if(!lockState[i]) return false;
    }
    return true; // All locks are unlocked
}
