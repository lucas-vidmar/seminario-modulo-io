#include "main.h"

BuiltInLed led = BuiltInLed();
I2C i2c = I2C();
DAC dac = DAC();
ADC adc = ADC();
Encoder encoder = Encoder();
WebServerESP32 webServer("ESP32_Hotspot", "12345678");

/* ------- GPIOs ------- */
// DI: 26 27 36 | DO: 15 32 33
DigitalGPIO di1 = DigitalGPIO(26, INPUT);
DigitalGPIO di2 = DigitalGPIO(27, INPUT);
DigitalGPIO di3 = DigitalGPIO(36, INPUT);
DigitalGPIO do1 = DigitalGPIO(15, OUTPUT);
DigitalGPIO do2 = DigitalGPIO(32, OUTPUT);
DigitalGPIO do3 = DigitalGPIO(33, OUTPUT);
// AI: 34 35 | AO: 25
AnalogGPIO ai1 = AnalogGPIO(34);
AnalogGPIO ai2 = AnalogGPIO(35);
AnalogGPIO ao1 = AnalogGPIO(25);


void setup() {

  // Inicializaciones
  led.init();
  //led.blink(500);
  //i2c.init();
  //dac.init(&i2c);
  //adc.init(&i2c);
  encoder.init();

  // Setup pines
  di1.setup();
  di2.setup();
  di3.setup();
  do1.setup();
  do2.setup();
  do3.setup();
  ai1.setup();
  ai2.setup();
  ao1.setup();

  // Configurar manejadores para rutas
  webServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/index.html", "text/html");
  });
  
  webServer.on("/toggleLED", HTTP_GET, [](AsyncWebServerRequest *request){
      led.toggle();
      request->send(200, "text/plain", "LED toggled");
  });
  
  // Configurar archivo por defecto
  webServer.setDefaultFile("index.html");
  
  // Iniciar el servidor web
  webServer.begin();
}

void loop() {

}