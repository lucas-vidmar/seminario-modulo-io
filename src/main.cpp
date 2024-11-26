#include "main.h"

BuiltInLed led = BuiltInLed();
I2C i2c = I2C();
DAC dac = DAC();
ADC adc = ADC();
Encoder encoder = Encoder();
WebServerESP32 webServer("ESP32_Hotspot", "12345678");

/* ------- GPIOs ------- */
// DI: 26 27 36 | DO: 15 32 33
DigitalGPIO di1 = DigitalGPIO(GPIO_NUM_26, INPUT);
DigitalGPIO di2 = DigitalGPIO(GPIO_NUM_27, INPUT);
DigitalGPIO di3 = DigitalGPIO(GPIO_NUM_36, INPUT);
DigitalGPIO do1 = DigitalGPIO(GPIO_NUM_15, OUTPUT);
DigitalGPIO do2 = DigitalGPIO(GPIO_NUM_32, OUTPUT);
DigitalGPIO do3 = DigitalGPIO(GPIO_NUM_33, OUTPUT);
// AI: 34 35 | AO: 25
AnalogGPIO ai1 = AnalogGPIO(GPIO_NUM_34);
AnalogGPIO ai2 = AnalogGPIO(GPIO_NUM_35);
AnalogGPIO ao1 = AnalogGPIO(GPIO_NUM_25);


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
  HTMLHandlers();
  
  // Configurar archivo por defecto
  webServer.setDefaultFile("index.html");
  
  // Iniciar el servidor web
  webServer.begin();
}

void loop(){
  delay(1000);

}

void HTMLHandlers() {
  webServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/index.html", "text/html");
  });

  // Leer entradas digitales
  webServer.on("/readDI1", HTTP_GET, [](AsyncWebServerRequest *request){
    int state = digitalRead(GPIO_NUM_26);
    request->send(200, "text/plain", String(state));
  });

  webServer.on("/readDI2", HTTP_GET, [](AsyncWebServerRequest *request){
    int state = digitalRead(GPIO_NUM_27);
    request->send(200, "text/plain", String(state));
  });

  webServer.on("/readDI3", HTTP_GET, [](AsyncWebServerRequest *request){
    int state = digitalRead(GPIO_NUM_36);
    request->send(200, "text/plain", String(state));
  });
  // Leer entradas analógicas
  webServer.on("/readAI1", HTTP_GET, [](AsyncWebServerRequest *request){
    int value = analogRead(GPIO_NUM_34);
    request->send(200, "text/plain", String(value));
  });

  webServer.on("/readAI2", HTTP_GET, [](AsyncWebServerRequest *request){
    int value = analogRead(GPIO_NUM_35);
    request->send(200, "text/plain", String(value));
  });

  // Ajustar salida analógica
  webServer.on("/setAO1", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("value")) {
      String valueStr = request->getParam("value")->value();
      int value = valueStr.toInt();
      // Mapear el valor de 1-100 a 0-255 para el DAC
      int dacValue = map(value, 1, 100, 0, 255);
      dacWrite(GPIO_NUM_25, dacValue);
      request->send(200, "text/plain", "AO1 ajustado a " + valueStr);
    } else {
      request->send(400, "text/plain", "Falta el parámetro 'value'");
    }
  });

  // Ajustar salidas digitales
  webServer.on("/setDO1", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("state")) {
      String stateStr = request->getParam("state")->value();
      int state = stateStr.toInt();
      digitalWrite(GPIO_NUM_15, state);
      request->send(200, "text/plain", "DO1 ajustado a " + stateStr);
    } else {
      request->send(400, "text/plain", "Falta el parámetro 'state'");
    }
  });

  webServer.on("/setDO2", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("state")) {
      String stateStr = request->getParam("state")->value();
      int state = stateStr.toInt();
      digitalWrite(GPIO_NUM_32, state);
      request->send(200, "text/plain", "DO2 ajustado a " + stateStr);
    } else {
      request->send(400, "text/plain", "Falta el parámetro 'state'");
    }
  });

  webServer.on("/setDO3", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("state")) {
      String stateStr = request->getParam("state")->value();
      int state = stateStr.toInt();
      digitalWrite(GPIO_NUM_33, state);
      request->send(200, "text/plain", "DO3 ajustado a " + stateStr);
    } else {
      request->send(400, "text/plain", "Falta el parámetro 'state'");
    }
  });




}