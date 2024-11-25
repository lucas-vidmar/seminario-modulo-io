#pragma once

#include <Arduino.h>

class GPIO {
public:
    virtual void setup() = 0;
    virtual int read() = 0;
    virtual void write(int value) = 0;
};

class DigitalGPIO : public GPIO {
private:
    uint8_t pin;
    uint8_t mode;

public:
    DigitalGPIO(uint8_t pin, uint8_t mode);

    void setup() override;
    int read() override;
    void write(int value) override;
};

class AnalogGPIO : public GPIO {
private:
    uint8_t pin;

public:
    AnalogGPIO(uint8_t pin);

    void setup() override;
    int read() override;
    void write(int value) override;
};
