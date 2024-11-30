#include "dac.h"

DAC::DAC() : i2c(nullptr) {}

void DAC::init(I2C* i2c_pointer){
    i2c = i2c_pointer;
    set_voltage(0); // Set DAC to default value (0V)
}

void DAC::set_voltage(int voltageInMmV) {
    // Calculate DAC value
    float value = ((voltageInMmV / 1000.0) / DAC_V_MAX) * DAC_RESOLUTION; // Convert mV to V and scale to DAC resolution
    // Round to nearest integer
    digitalWrite((uint16_t)(value + 0.5));
}

void DAC::digitalWrite(uint16_t value) {
    if (value > DAC_MAX_DIGITAL_VALUE) { // Check if value is out of range
        Serial.println("DAC value out of range");
        return;
    }

    // Write value to DAC using I2C library
    uint8_t data[2];
    data[0] = (value >> 8) & 0x0F;
    data[1] = value & 0xFF;
    i2c->write(MCP4725_ADDR, data, 2);
}

int DAC::readVoltage() {
    if (!i2c) { // Verifica si el puntero I2C es válido
        Serial.println("I2C not initialized");
        return -1; // Error
    }

    uint8_t data[3];
    i2c->read(MCP4725_ADDR, data, 3);    

    // Decodificación de los datos
    uint16_t dac_value = ((data[0] & 0x0F) << 8) | data[1]; // Bits 11-0
    dac_value = (dac_value << 4) | ((data[2] & 0xF0) >> 4); // Bits 3-0 de data[2]

    // Convierte a voltaje
    float voltage = (float)dac_value / (DAC_RESOLUTION - 1) * DAC_V_MAX;

    return (int)(voltage * 1000.0 + 0.5); // Devuelve el voltaje en mV
}