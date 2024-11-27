#include "gpio.h"

DigitalGPIO::DigitalGPIO(uint8_t pin, uint8_t mode) : pin(pin), mode(mode) {}

void DigitalGPIO::setup() {
    pinMode(pin, mode);
}

int DigitalGPIO::read() {
    return digitalRead(pin);
}

void DigitalGPIO::write(int value) {
    digitalWrite(pin, value);
}

void DigitalGPIO::toggle() {
    digitalWrite(pin, !digitalRead(pin));
}

AnalogGPIO::AnalogGPIO(uint8_t pin) : pin(pin) {}

void AnalogGPIO::setup() {
    // No setup required for analog read in Arduino
}

int AnalogGPIO::read() {
    return analogRead(pin);
}

void AnalogGPIO::write(int value) {
    analogWrite(pin, value);
}