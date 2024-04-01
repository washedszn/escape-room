#include "HardwareConfig.h"

// Initialize TM1638plus module with defined pins
TM1638plus tm1638(TM1638_STB_PIN, TM1638_CLK_PIN, TM1638_DIO_PIN);

// Initialize LiquidCrystal_I2C with I2C address 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

void initializeHardware() {
    // Start the TM1638 module
    tm1638.displayBegin();

    // Initialize the LCD
    lcd.init();
    lcd.backlight();

    // Setup the RGB LED pins as output
    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(GREEN_LED_PIN, OUTPUT);
    pinMode(BLUE_LED_PIN, OUTPUT);

    // Setup the buzzer pin as output
    pinMode(BUZZER_PIN, OUTPUT);

    // No need to setup the potentiometer pin as it is an analog input
}
