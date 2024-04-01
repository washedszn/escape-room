# Arduino Hardware Testing

This utility is designed to verify that all the components in your Arduino-based setup are connected and functioning properly. It is used as a preparatory step before launching the main escape room game to ensure that all hardware interactions work as expected.

## Description

The test script performs the following actions:

- Initializes the TM1638 module and displays a greeting message.
- Initializes the LCD and prints initialization messages.
- Cycles through potentiometer values and displays them on the LCD and TM1638.
- Generates a tone through the buzzer based on the potentiometer's position.
- Changes the RGB LED color based on the potentiometer value.

## Hardware Requirements

- Arduino microcontroller
- TM1638 module
- 16x2 I2C LCD display
- Buzzer
- Potentiometer
- RGB LED
- Appropriate resistors for LED and buzzer

Ensure the components are connected as defined in the pin assignments within the code.