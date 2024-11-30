#include "gpio.h"

// Constructor de la clase DigitalGPIO
DigitalGPIO::DigitalGPIO(uint8_t pin, uint8_t mode) : pin(pin), mode(mode) {}

// Configurar el modo del pin digital
void DigitalGPIO::setup() {
    pinMode(pin, mode);
}

// Leer el valor digital del pin
int DigitalGPIO::read() {
    return digitalRead(pin);
}

// Escribir un valor digital en el pin
void DigitalGPIO::write(int value) {
    digitalWrite(pin, value);
}

// Alternar el valor digital en el pin
void DigitalGPIO::toggle() {
    digitalWrite(pin, !digitalRead(pin));
}

// Constructor de la clase AnalogGPIO
AnalogGPIO::AnalogGPIO(uint8_t pin) : pin(pin) {}

// No se requiere configuración para la lectura analógica en Arduino
void AnalogGPIO::setup() {
    // No se requiere configuración para la lectura analógica en Arduino
}

// Leer el valor analógico del pin
int AnalogGPIO::read() {
    return analogRead(pin);
}

// Escribir un valor analógico en el pin
void AnalogGPIO::write(int value) {
    analogWrite(pin, value);
}