#ifndef HARDWARECONFIG_H
#define HARDWARECONFIG_H

#include <LiquidCrystal_I2C.h>
#include <TM1638plus.h>

// Pin assignments for the TM1638 module
#define TM1638_DIO_PIN 11
#define TM1638_CLK_PIN 12
#define TM1638_STB_PIN 13

// Pin assignments for the RGB LED
#define RED_LED_PIN 10
#define GREEN_LED_PIN 9
#define BLUE_LED_PIN 8

// Pin assignments for other components
#define BUZZER_PIN 7
#define POTENTIOMETER_PIN A0

// Initialize the TM1638plus module with pin assignments
extern TM1638plus tm1638;

// Initialize the LCD (16x2) with the I2C address
extern LiquidCrystal_I2C lcd;

void initializeHardware();

#endif // HARDWARECONFIG_H
