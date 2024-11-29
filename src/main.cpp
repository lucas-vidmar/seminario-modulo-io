#include "main.h"

int encoder_last_position = -1;

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
//AnalogGPIO ao1 = AnalogGPIO(AO1);

/* ------- Menu ------- */
enum MenuState {
  MAIN_MENU,
  MEASUREMENTS,
  CONTROL_MENU,
  CONTROL,
  CONFIG
};

enum ControlState {
  CONTROL_DO1,
  CONTROL_DO2,
  CONTROL_DO3,
  CONTROL_AO1,
  CONTROL_EXIT
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
  dac.init(&i2c);
  dac.digitalWrite(4095);
  adc.init(&i2c);
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
  //ao1.setup();

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
  print_line(0, "--Conectar al AP--");
  print_line(1, "IP: " + WiFi.softAPIP().toString());
  print_line(2, String("SSID: ") + ssid);
  print_line(3, String("Password: ") + password);

  //delay(5000); // Esperar 5 segundos para que el usuario pueda ver la información

  encoder.setPosition(0);
}

void loop(){
  #ifndef USE_ENCODER
  encoder.update();
  #endif

  //fsm();
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

  webServer.on("/readAI1*", HTTP_GET, [](AsyncWebServerRequest *request){
    float value = adc.read_voltage(0);
    request->send(200, "text/plain", String(value,2));
  });

  webServer.on("/readAI2*", HTTP_GET, [](AsyncWebServerRequest *request){
    float value = adc.read_voltage(1);
    request->send(200, "text/plain", String(value,2));
  });

  webServer.on("/readAI3*", HTTP_GET, [](AsyncWebServerRequest *request){
    float value = adc.read_voltage(2);
    request->send(200, "text/plain", String(value,2));
  });

  webServer.on("/readAI4*", HTTP_GET, [](AsyncWebServerRequest *request){
    float value = adc.read_voltage(3);
    request->send(200, "text/plain", String(value,2));
  });

  // Ajustar salida analógica
  webServer.on("/setAO1*", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("value")) {
      String valueStr = request->getParam("value")->value();
      int value = valueStr.toInt();
      dac.digitalWrite(value);
      request->send(200, "text/plain", "AO1* ajustado a " + valueStr);
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

void fsm() {
  static MenuState state = MAIN_MENU;
  static ControlState control_state = CONTROL_DO1;
  switch (state) {
    case MAIN_MENU:
      // Manejar el menú principal
      if (encoder_last_position != encoder.getPosition()) {
        String menuOptions[] = {"Mediciones", "Control", "Configuración"};
        print_menu(menuOptions, 3, "Menú principal");
        encoder_last_position = encoder.getPosition();
      }
      if (encoder.isButtonPressed()) {
        state = MenuState(encoder.getPosition() + 1);
        encoder_last_position = -1;
        encoder.setPosition(0);
      }
      break;
    case MEASUREMENTS:
      // Manejar las mediciones
      print_measurments();
      if (encoder.isButtonPressed()) {
        if (encoder.getPosition() == 9) {
          state = MAIN_MENU;
          encoder_last_position = -1;
          encoder.setPosition(0);
        }
      }
      delay(1000); // Actualizar cada segundo
      break;
    case CONTROL_MENU:
      // Manejar el menu de control
      static bool control_menu = true;
      if (encoder_last_position != encoder.getPosition()) {
        print_control(encoder.getPosition());
        encoder_last_position = encoder.getPosition();
      }
      if (encoder.isButtonPressed()) {
        state = (encoder.getPosition() == CONTROL_EXIT) ? MAIN_MENU : CONTROL; // Salir del menu de control o ir a control
        encoder_last_position = -1;
        control_state = ControlState(encoder.getPosition());
        encoder.setPosition(0);
      }
      break;
    case CONTROL:
      // Manejar el control
      switch (control_state) {
        case CONTROL_DO1:
          if (encoder_last_position != encoder.getPosition()) {
            control_digital(do1, "DO1");
            encoder_last_position = encoder.getPosition();
          }
          if (encoder.isButtonPressed()) {
            if (encoder.getPosition() == 0) {
              do1.toggle();
              control_digital(do1, "DO1");
            }
            else {
              state = CONTROL_MENU;
              encoder_last_position = -1;
              encoder.setPosition(0);
            }
          }
          break;
        case CONTROL_DO2:
          if (encoder_last_position != encoder.getPosition()) {
            control_digital(do2, "DO2");
            encoder_last_position = encoder.getPosition();
          }
          if (encoder.isButtonPressed()) {
            if (encoder.getPosition() == 0) {
              do2.toggle();
              control_digital(do2, "DO2");
            }
            else {
              state = CONTROL_MENU;
              encoder_last_position = -1;
              encoder.setPosition(0);
            }
          }
          break;
        case CONTROL_DO3:
          if (encoder_last_position != encoder.getPosition()) {
            control_digital(do3, "DO3");
            encoder_last_position = encoder.getPosition();
          }
          if (encoder.isButtonPressed()) {
            if (encoder.getPosition() == 0) {
              do3.toggle();
              control_digital(do3, "DO3");
            }
            else {
              state = CONTROL_MENU;
              encoder_last_position = -1;
              encoder.setPosition(0);
            }
          }
          break;
        case CONTROL_AO1:
          break;
        case CONTROL_EXIT:
          state = CONTROL_MENU;
          encoder_last_position = -1;
          encoder.setPosition(0);
          break;
      }
      break;
    case CONFIG:
      // Manejar la configuración
      if (encoder.isButtonPressed()) {
        state = MAIN_MENU;
        encoder_last_position = -1;
      }
      break;
  }
}

void print_menu(String options[],int options_size, String title) {
  lcd.clear();
  encoder.setMinPosition(0);
  encoder.setMaxPosition(options_size - 1);
  int selected = encoder.getPosition();
  print_line(0, "--" + title + "--");
  int options_to_print = min(LCD_ROWS - 1, options_size);

  String menuItems[options_to_print];
  int group = selected / (LCD_ROWS - 1);
  int start = group * (LCD_ROWS - 1);
  for (int i = 0; i < options_to_print; i++) {
    if (start + i >= options_size) {
      break;
    }
    if (start + i == selected) {
      menuItems[i] = ">" + options[start + i]; // Agregar un indicador de selección
    } else {
      menuItems[i] = options[start + i];
    }
  }
  for (int i = 0; i < options_to_print; i++) {
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

void print_measurments() {
  lcd.clear();
  String options[]= {
    "DI1: " + String(digitalRead(DI1) ? "OFF" : "ON"),
    "DI2: " + String(digitalRead(DI2) ? "OFF" : "ON"),
    "DI3: " + String(digitalRead(DI3) ? "OFF" : "ON"),
    "AI1: " + String(analogRead(AI1)),
    "AI2: " + String(analogRead(AI2)),
    "AI1*: " + String(adc.read_voltage(0)),
    "AI2*: " + String(adc.read_voltage(1)),
    "AI3*: " + String(adc.read_voltage(2)),
    "AI4*: " + String(adc.read_voltage(3)),
    "Volver"
  };
  print_menu(options, 10, "Mediciones");
}

void print_control(int selected){
  String controlItems[] = {"DO1", "DO2", "DO3", "AO1", "Volver"};
  print_menu(controlItems, 5, "Control");
}

void control_digital(DigitalGPIO d, String pinName){
  String status = d.read() == HIGH ? "OFF" : "ON";
  String options[] = {status, "Volver"};
  print_menu(options, 2, pinName);
}

void control_analog(AnalogGPIO a, String pinName){
  String current_value = "Valor: " + String(a.read());
  String options[] = {current_value, "Volver"};
  print_menu(options, 2, pinName);
}