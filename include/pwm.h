#include <Arduino.h>

class PWM {
public:

    PWM(uint8_t pin, uint32_t frequency, uint8_t dutyCycle);
    void setFrequency(uint32_t frequency);
    void setDutyCycle(uint8_t dutyCycle);
    uint32_t getFrequency() const;
    uint8_t getDutyCycle() const;
private:
    uint8_t _pin;
    uint32_t _frequency;
    uint8_t _dutyCycle;
};