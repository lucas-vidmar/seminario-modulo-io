#include "main.h"

static int encoder_last_position = -1;

/* ------- Perisfericos ------- */
BuiltInLed led = BuiltInLed();
I2C i2c = I2C();
DAC dac = DAC();
ADC adc = ADC();
#ifdef USE_LCD
LiquidCrystal_I2C lcd(0x3F, 20, 4);  // Crear el objeto lcd con dirección 0x3F y 20 columnas x 4 filas
#else
SimulatedLCD lcd = SimulatedLCD();
#endif
#ifdef USE_ENCODER
Encoder encoder = Encoder();
#else
SimulatedEncoder encoder = SimulatedEncoder();
#endif

/* ------- Web Server ------- */
const String ssid = "Modulo IO";
const String password = "iomodule";
WebServerESP32 webServer(ssid.c_str(), password.c_str());

/* ------- GPIOs ------- */
// DI: 26 27 36 | DO: 15 32 33
DigitalGPIO di1 = DigitalGPIO(DI1, INPUT);
DigitalGPIO di2 = DigitalGPIO(DI2, INPUT);
DigitalGPIO di3 = DigitalGPIO(DI3, INPUT);
DigitalGPIO do1 = DigitalGPIO(DO1, OUTPUT);
DigitalGPIO do2 = DigitalGPIO(DO2, OUTPUT);
DigitalGPIO do3 = DigitalGPIO(DO3, OUTPUT);
// AI: 34 35 | AO: 25
AnalogGPIO ai1 = AnalogGPIO(AI1);
AnalogGPIO ai2 = AnalogGPIO(AI2);
AnalogGPIO ao1 = AnalogGPIO(AO1);

/* ------- Menu ------- */
enum MenuState {
  MAIN_MENU,
  MEASUREMENTS,
  CONTROL,
  CONFIG
};


void setup() {

  Serial.begin(115200);

  // Inicializaciones
  led.init();
  led.blink(500);

  // Inicializar SPIFFS
  Serial.println("Inicializando SPIFFS...");
  if (!SPIFFS.begin(true)) {
    Serial.println("Error al montar SPIFFS");
    return;
  }
  
  // Inicializacion de perifericos
  Serial.println("Inicializando periféricos...");
  i2c.init();
  //dac.init(&i2c);
  //adc.init(&i2c);
  encoder.init();

  // Setup pines
  Serial.println("Configurando pines...");
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
  Serial.println("Configurando manejadores de rutas...");
  HTMLHandlers();
  
  // Iniciar el servidor web
  Serial.println("Iniciando servidor web...");
  webServer.setDefaultFile("index.html");
  webServer.begin();

  // Mostrar información en la pantalla
  lcd.init();
  lcd.backlight();
  lcd.clear();
  print_line(0, "Conectar al AP:");
  print_line(1, "IP: " + WiFi.softAPIP().toString());
  print_line(2, String("SSID: ") + ssid);
  print_line(3, String("Password: ") + password);

  delay(5000); // Esperar 5 segundos para que el usuario pueda ver la información

  // Imprimir menu en la pantalla
  encoder.setMaxPosition(2);
  encoder.setMinPosition(0);
  encoder.setPosition(0);
  print_menu(0);

}

void loop(){
  #ifndef USE_ENCODER
  encoder.update();
  #endif

  
  if (encoder_last_position != encoder.getPosition()) {
    print_menu(encoder.getPosition());
    encoder_last_position = encoder.getPosition();
  }
}

void HTMLHandlers() {
  webServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/index.html", "text/html");
  });

  // Leer entradas digitales
  webServer.on("/readDI1", HTTP_GET, [](AsyncWebServerRequest *request){
    int state = digitalRead(DI1);
    request->send(200, "text/plain", String(state));
  });

  webServer.on("/readDI2", HTTP_GET, [](AsyncWebServerRequest *request){
    int state = digitalRead(DI2);
    request->send(200, "text/plain", String(state));
  });

  webServer.on("/readDI3", HTTP_GET, [](AsyncWebServerRequest *request){
    int state = digitalRead(DI3);
    request->send(200, "text/plain", String(state));
  });
  // Leer entradas analógicas
  webServer.on("/readAI1", HTTP_GET, [](AsyncWebServerRequest *request){
    int value = analogRead(AI1);
    request->send(200, "text/plain", String(value));
  });

  webServer.on("/readAI2", HTTP_GET, [](AsyncWebServerRequest *request){
    int value = analogRead(AI2);
    request->send(200, "text/plain", String(value));
  });

  // Ajustar salida analógica
  webServer.on("/setAO1", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("value")) {
      String valueStr = request->getParam("value")->value();
      int value = valueStr.toInt();
      // Mapear el valor de 1-100 a 0-255 para el DAC
      int dacValue = map(value, 1, 100, 0, 255);
      dacWrite(AO1, dacValue);
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
      digitalWrite(DO1, state);
      request->send(200, "text/plain", "DO1 ajustado a " + stateStr);
    } else {
      request->send(400, "text/plain", "Falta el parámetro 'state'");
    }
  });

  webServer.on("/setDO2", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("state")) {
      String stateStr = request->getParam("state")->value();
      int state = stateStr.toInt();
      digitalWrite(DO2, state);
      request->send(200, "text/plain", "DO2 ajustado a " + stateStr);
    } else {
      request->send(400, "text/plain", "Falta el parámetro 'state'");
    }
  });

  webServer.on("/setDO3", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("state")) {
      String stateStr = request->getParam("state")->value();
      int state = stateStr.toInt();
      digitalWrite(DO3, state);
      request->send(200, "text/plain", "DO3 ajustado a " + stateStr);
    } else {
      request->send(400, "text/plain", "Falta el parámetro 'state'");
    }
  });

}

void print_menu(int selected) {
  lcd.clear();
  print_line(0, "Menu principal");
  String menuItems[] = {"Mediciones", "Control", "Configuracion"};
  for (int i = 0; i < 3; i++) {
    if (i == selected) {
      menuItems[i] = ">" + menuItems[i]; // Agregar un indicador de selección
    }
  }
  for (int i = 0; i < 3; i++) {
    print_line(i+1, menuItems[i]); // Imprimir cada elemento del menú
  }
  
}

void print_line(int line, String text) {
  lcd.setCursor(0, line);
  lcd.print(text.c_str());
  #ifdef USE_LCD
  Serial.print(text);
  #endif
}