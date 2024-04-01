#include <LiquidCrystal_I2C.h>
#include "PotentiometerLock.h"
#include "RedLightGreenLight.h"
#include "MorseCodeChallenge.h"
#include "MelodyMe.h"
#include "Utilities.h"
#include "HardwareConfig.h"

// Instantiate challenge objects
PotentiometerLock potLock;
//...other challenge objects

void setup() {
    // Initialize all hardware and challenges
}

void loop() {
    // Implement the main menu system and call the appropriate challenge based on user input
    // For example:
    // if (button1 pressed) potLock.begin();
    // if (button2 pressed) rlgl.begin();
    // ...etc
}
