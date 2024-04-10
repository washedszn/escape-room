#include "MorseCodeChallenge.h"
#include "HardwareConfig.h" // Ensure this includes TM1638 and buzzer

// Constructor, not much needed here for this example
MorseCodeChallenge::MorseCodeChallenge() {
}

void MorseCodeChallenge::run() {
    for(currentLevel = 0; currentLevel < 3; currentLevel++) {
        if (!levelCompleted[currentLevel]) {
            // Inform the player about the current level
            lcd.clear();
            lcd.print("Level ");
            lcd.print(currentLevel + 1);
            delay(1000); // Give the player a moment to read the message
            Serial.begin(9600);
            Serial.print("starting");
            // outputMorseCode(currentLevel); // Play Morse code for the current level
            
            bool buttonsCorrect, potCorrect;
            do {
                updateTimerDisplay();
                buttonsCorrect = checkButtonSequence(currentLevel);
                potCorrect = checkPotentiometerPosition(currentLevel);
                
                if (!buttonsCorrect || !potCorrect) {
                    // Provide feedback for incorrect input
                    lcd.clear();
                    lcd.print("Try again!");
                    delay(2000); // Give the player time to read the message
                    lcd.clear();
                    lcd.print("Level ");
                    lcd.print(currentLevel + 1);
                }
            } while (!buttonsCorrect || !potCorrect);
            
            // Provide success feedback
            lcd.clear();
            lcd.print("Success!");
            delay(2000); // Give the player time to read the success message
            
            levelCompleted[currentLevel] = true; // Mark level as completed
        }
    }
    // After all levels are completed, indicate game completion
    lcd.clear();
    lcd.print("All done!");
}

bool MorseCodeChallenge::isCompleted() {
    for (bool completed : levelCompleted) {
        if (!completed) return false;
    }
    return true; // All levels completed
}

void MorseCodeChallenge::outputMorseCode(int level) {
    // Define Morse code for numbers 1-9
    const char* morseNumbers[9] = {".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----."};

    // Morse code sequences for each level based on the button sequence
    const char* sequences[3] = {
        ".---- ..--- ...-- ....-",   // Level 1: 1234
        ".---- ...-- ..--- ....- -....", // Level 2: 13246
        "---.. ..--- ...-- .---- ....- -.... ..... --..." // Level 3: 82314657
    };

    // Select the sequence for the current level
    const char* sequence = sequences[level];

    // Loop through the Morse code sequence
    for (int i = 0; sequence[i] != '\0'; i++) {
        if (sequence[i] == '.') {
            // Dot: short beep
            tone(BUZZER_PIN, 1000, 250); // Play tone at 1000Hz for 250ms
            delay(250); // Duration of a dot
        } else if (sequence[i] == '-') {
            // Dash: long beep
            tone(BUZZER_PIN, 1000, 750); // Play tone at 1000Hz for 750ms
            delay(750); // Duration of a dash
        }
        
        delay(250); // Short pause between elements

        // Long pause between characters, but skip if at the end of the sequence
        if (sequence[i + 1] != '\0' && sequence[i + 1] != ' ') {
            delay(500); // Longer pause between characters
        }
    }

    // Pause before repeating the sequence
    delay(1500); // Wait a bit before repeating the Morse code
}

bool MorseCodeChallenge::checkButtonSequence(int level) {
    // Arrays defining the required button sequences for all levels
    const uint8_t levelSequences[3][8] = {
        {0b0001, 0b0011, 0b0111, 0b1111, 0x00}, // Level 1
        {0b0001, 0b0101, 0b0111, 0b1111, 0b111101, 0x00}, // Level 2
        {0b10000000, 0b10000010, 0b10000110, 0b10000111, 0b10100111, 0b11100111, 0b11101111, 0b11111111} // Level 3
    };
    const int sequenceLengths[3] = {4, 5, 8}; // Steps in each level's sequence

    uint8_t mistakeMask = 0; // Track any button presses outside the sequence

    for (int step = 0; step < sequenceLengths[level]; ++step) {
        bool stepCompleted = false;
        while (!stepCompleted) {
            uint8_t currentButtonState = tm1638.readButtons();
            uint8_t expectedState = levelSequences[level][step];

            // Check for correct button press
            if ((currentButtonState & expectedState) == expectedState) {
                stepCompleted = true;
                delay(100); // Brief pause
            } else if (currentButtonState & ~expectedState) { // Check for any button press outside the expected state
                // If any button outside the sequence is pressed, consider it a mistake
                mistakeMask |= (currentButtonState & ~expectedState); // Update mistake mask
                // Provide feedback for the error
                Serial.println("Mistake made, retry sequence.");
                return checkButtonSequence(level); // Exit and indicate sequence was not successfully completed
            }
            delay(50); // Debounce delay
        }
    }

    // Optional: Provide feedback for successfully completing the sequence
    Serial.println("Sequence completed successfully.");
    return true; // If all steps are matched, the sequence is successfully completed
}

bool MorseCodeChallenge::checkPotentiometerPosition(int level) {
    bool correctPosition = false;
    Serial.println("checking pot");

    // Level-specific potentiometer target positions and tolerance
    const float targetPositions[3] = {0, 75, 50}; // Target positions for levels 1, 2, and 3
    const float tolerance = 5; // Allowable tolerance in percentage

    // Loop until the potentiometer is in the correct position
    while (!correctPosition) {
        int potValue = analogRead(POTENTIOMETER_PIN);
        float position = (float)potValue / 1023.0 * 100.0; // Convert to percentage

        // Check if the position is within tolerance of the target
        if (level == 0) { // First level doesn't require potentiometer check
            correctPosition = true; // Automatically pass the check
        } else if (position >= targetPositions[level] - tolerance && position <= targetPositions[level] + tolerance) {
            correctPosition = true; // Position is correct
        } else {
            // Optionally, include a feedback mechanism here to inform the user
            // they are outside the target range, such as an LCD message or LED indication
        }
        
        // Small delay to prevent excessive analog reads
        delay(100);
    }

    return true; // Once the loop exits, the position is correct
}
