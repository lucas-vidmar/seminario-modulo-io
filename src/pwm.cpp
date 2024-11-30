#include "pwm.h"

PWM::PWM(uint8_t pin, uint32_t frequency, uint8_t dutyCycle) 
    : _pin(pin), _frequency(frequency), _dutyCycle(dutyCycle) {
    pinMode(_pin, OUTPUT);
    analogWriteFrequency(_frequency);
    analogWrite(_pin, _dutyCycle);
}

void PWM::setFrequency(uint32_t frequency) {
    _frequency = frequency;
    analogWriteFrequency(_frequency);
}

void PWM::setDutyCycle(uint8_t dutyCycle) {
    // Adjust input 0-100 to 0-255
    _dutyCycle = map(dutyCycle, 0, 100, 0, 255);
    analogWrite(_pin, _dutyCycle);
}

uint32_t PWM::getFrequency() const {
    return _frequency;
}

uint8_t PWM::getDutyCycle() const {
    return _dutyCycle;
}