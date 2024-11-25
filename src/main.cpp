#include "main.h"

BuiltInLed led = BuiltInLed();
I2C i2c = I2C();
DAC dac = DAC();
ADC adc = ADC();

void setup() {

  // Inicializaciones
  led.init();
  led.blink(500);
  i2c.init();
  dac.init(&i2c);
  adc.init(&i2c);

}

void loop() {

}