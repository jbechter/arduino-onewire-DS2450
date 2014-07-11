/*
 *   DS2450Voltages
 *
 *   This example demonstrates the use of the DS2450 library and the Arduino
 *   OneWire library to read voltages from a Dallas Semiconductor
 *   DS2450 quad a/d.
 *
 *   by Joe Bechter
 *
 *   (C) 2012, bechter.com
 *
 *   All files, software, schematics and designs are provided as-is with no warranty.
 *   All files, software, schematics and designs are for experimental/hobby use.
 *   Under no circumstances should any part be used for critical systems where safety,
 *   life or property depends upon it. You are responsible for all use.
 *   You are free to use, modify, derive or otherwise extend for your own non-commercial purposes provided
 *       1. No part of this software or design may be used to cause injury or death to humans or animals.
 *       2. Use is non-commercial.
 *       3. Credit is given to the author (i.e. portions © bechter.com), and provide a link to the original source.
 *
 */

#include <Arduino.h>
#include <OneWire.h>
#include <DS2450.h>

// define the Arduino digital I/O pin to be used for the 1-Wire network here
const uint8_t ONE_WIRE_PIN = 2;

// define the 1-Wire address of the DS2450 quad a/d here (lsb first)
uint8_t DS2450_address[] = { 0x20, 0x1C, 0x86, 0x00, 0x00, 0x00, 0x00, 0x9E };

OneWire ow(ONE_WIRE_PIN);
DS2450 ds2450(&ow, DS2450_address);

void setup() {
    Serial.begin(9600);
    ds2450.begin();
}

void loop() {
    ds2450.update();
    if (ds2450.isError()) {
        Serial.println("Error reading from DS2450 device");
    } else {
        for (int channel = 0; channel < 4; channel++) {
            Serial.print("Channel ");
            Serial.print(char('A' + channel));
            Serial.print(" = ");
            Serial.print(ds2450.getVoltage(channel), 1);
            if (channel < 3) {
                Serial.print("v, ");
            } else {
                Serial.println("v.");
            }
        }
    }
    delay(500);
}
