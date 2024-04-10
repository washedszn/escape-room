#include "RedLightGreenLight.h"
#include "HardwareConfig.h"

RedLightGreenLight::RedLightGreenLight() : completed(false) {
}

void RedLightGreenLight::run() {
    for (int level = 0; level < 3; ++level) {
        // we'll update the timer here before each level
        updateTimerDisplay();
        bool levelSuccess = false;
        Serial.begin(9600);
        lcd.clear();
        lcd.print("Starting level ");
        lcd.print(level + 1);
        delay(1000);

        while (!levelSuccess) {
            displayCountdown(3);
            levelSuccess = processLevel(level);
            
            displayLevelOutcome(levelSuccess, level);
        }

        if (level == 2) {
            lcd.clear();
            lcd.print("All Done!");
        }
    }

    completed = true;
}

bool RedLightGreenLight::isCompleted() {
    return completed;
}

void RedLightGreenLight::displayCountdown(int countdownSeconds) {
    for (int countdown = countdownSeconds; countdown > 0; --countdown) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Ready in: ");
        lcd.print(countdown);
        delay(1000);
    }
}

void RedLightGreenLight::displayLevelOutcome(bool success, int level) {
    lcd.clear();
    lcd.setCursor(0, 0);
    if (success) {
        lcd.print("Level ");
        lcd.print(level + 1);
        lcd.print(" Success");
    } else {
        lcd.print("Level ");
        lcd.print(level + 1);
        lcd.print(" Failed");
    }
    delay(2000); // Allow time to read the message
}

bool RedLightGreenLight::processLevel(int level) {
    int baseLightDurations[3] = {2000, 1500, 1000}; // Base duration for red light for each level in ms
    int windowDurations[3] = {500, 400, 300}; // Green light window for each level in ms
    int lightChangesPerLevel[3] = {5, 10, 15}; // Number of green light phases per level

    for (int currentChange = 0; currentChange < lightChangesPerLevel[level]; ++currentChange) {
        
        // Adjust red light duration randomly within a range of +-500ms
        int redLightDuration = baseLightDurations[level] + random(-500, 501);
        
        // Green light phase
        digitalWrite(GREEN_LED_PIN, HIGH);
        digitalWrite(RED_LED_PIN, LOW);
        Serial.println("Green light on");
        unsigned long greenLightStartTime = millis();
        bool buttonPressedDuringGreen = false;

        while (millis() - greenLightStartTime < windowDurations[level]) {
            if (tm1638.readButtons() & 0x80) {
                buttonPressedDuringGreen = true;
                Serial.println("Button pressed during green light");
                while (tm1638.readButtons() & 0x80) {
                    // wait for player to release button 
                }
                break;
            }
        }

        digitalWrite(GREEN_LED_PIN, LOW); // Turn off green light after the phase ends

        if (!buttonPressedDuringGreen) {
            Serial.println("Failed to press button during green light");
            return false; // Level failed due to no press during green
        }

        // Red light phase
        digitalWrite(RED_LED_PIN, HIGH);
        Serial.println("Red light on");
        unsigned long redLightStartTime = millis();

        while (millis() - redLightStartTime < redLightDuration) {
            if (tm1638.readButtons() & 0x80) {
                Serial.println("Button pressed during red light");
                return false; // Level failed due to button press during red
            }
        }
    }

    return true; // Successfully completed the level
}
