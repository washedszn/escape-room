#include "MelodyMe.h"
#include "HardwareConfig.h"

const int MelodyMe::melodyLengths[3] = {5, 10, 15};
const int MelodyMe::melodies[3][15] = {
    {1, 2, 3, 4, 5},
    {1, 3, 5, 2, 4, 6, 7, 8, 1, 2},
    {8, 7, 6, 5, 4, 3, 2, 1, 8, 7, 6, 5, 4, 3, 2}
};

MelodyMe::MelodyMe() : completed(false), currentLevel(0) {}

void MelodyMe::run() {
    for (currentLevel = 0; currentLevel < 3; currentLevel++) {
        lcd.clear();
        lcd.print("Level ");
        lcd.print(currentLevel + 1);

        playMelody(currentLevel);

        while (!captureInput(currentLevel)) {
            lcd.clear();
            lcd.print("Wrong, press USER_BTN to retry");
            while (digitalRead(USER_BTN) == HIGH); // Wait for button press
            while (digitalRead(USER_BTN) == LOW);  // Wait for button release
            playMelody(currentLevel);
        }
    }
    completed = true;
    lcd.clear();
    lcd.print("Nice Melodies!");
    delay(2000);
}

void MelodyMe::playMelody(int level) {
    for (int i = 0; i < melodyLengths[level]; i++) {
        int note = melodies[level][i];
        tone(BUZZER_PIN, note * 100, 400);
        tm1638.setLED(note - 1, true);
        delay(500);
        tm1638.setLED(note - 1, false);
    }
}

bool MelodyMe::captureInput(int level) {
    int noteIndex = 0;

    while (noteIndex < melodyLengths[level]) {
        uint8_t buttons = tm1638.readButtons();

        if (digitalRead(USER_BTN) == LOW) {
            lcd.clear();
            lcd.print("Replaying melody...");
            playMelody(level);
            while (digitalRead(USER_BTN) == LOW);
            lcd.clear();
            lcd.print("Level ");
            lcd.print(currentLevel + 1);
            continue;
        }

        for (int j = 0; j < 8; j++) {
            if (buttons & (1 << j)) {
                tm1638.setLED(j, true);
                tone(BUZZER_PIN, (j + 1) * 100, 200);

                if ((j + 1) == melodies[level][noteIndex]) {
                    noteIndex++;
                    while (tm1638.readButtons() & (1 << j));
                    tm1638.setLED(j, false);
                    if (noteIndex == melodyLengths[level]) {
                        lcd.clear();
                        lcd.print("Success!");
                        delay(1000);
                        return true;
                    }
                } else {
                    clearAllLEDs();
                    noteIndex = 0;
                    break;
                }
                delay(200);
            }
        }
        delay(50);
    }
    return false;
}

void MelodyMe::clearAllLEDs() {
    for (int k = 0; k < 8; k++) {
        tm1638.setLED(k, false);
    }
}

void MelodyMe::reset() {
    completed = false;
    currentLevel = 0;
}

bool MelodyMe::isCompleted() {
    return completed;
}