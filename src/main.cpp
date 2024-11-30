#include "main.h"

bool print = true;

/* ------- Perifericos ------- */
BuiltInLed led = BuiltInLed();
I2C i2c = I2C();
DAC dac = DAC();
ADC adc = ADC();
#ifdef USE_LCD
LiquidCrystal_I2C lcd(0x27, 20, 4);  // Crear el objeto lcd con dirección 0x3F y 20 columnas x 4 filas
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
PWM pwm1 = PWM(PPWM1, PWMFREQ, 0);
PWM pwm2 = PWM(PPWM2, PWMFREQ, 0);

I2CScanner scanner;

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
  CONTROL_PWM1,
  CONTROL_PWM2,
  CONTROL_EXIT
};

void setup() {

  Serial.begin(115200);

  // I2C Scanner
  scanner.Init();
  scanner.Scan();

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

  // Settear PWM a 0
  pwm1.setDutyCycle(0);
  pwm2.setDutyCycle(0);
  
  // Iniciar el servidor web
  Serial.println("Iniciando servidor web...");
  webServer.setDefaultFile("index.html");
  webServer.begin();

  // Mostrar información en la pantalla
  lcd.init();
  lcd.backlight();
  lcd.clear();
  String ops[] = {"IP: " + WiFi.softAPIP().toString(), "SSID: " + ssid, "Password: " + password, "Continuar"};
  while (!(encoder.isButtonPressed() && encoder.getPosition() == 3)) { // Esperar a que el usuario presione el botón y seleccione "Continuar"
    if (encoder.moved() || printOneTime()) {
      print_menu(ops, 4, "Informacion AP");
    }
    encoder.update();
  }
  encoder.setPosition(0);
}

void loop(){
  encoder.update();
  fsm();
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
    float value = analogRead(AI1)*3.3/4095;
    request->send(200, "text/plain", String(value,2));
  });

  webServer.on("/readAI2", HTTP_GET, [](AsyncWebServerRequest *request){
    float value = analogRead(AI2)*3.3/4095;
    request->send(200, "text/plain", String(value,2));
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

  // Ajustar PWM1
  webServer.on("/setPWM1", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("duty")) {
      String dutyStr = request->getParam("duty")->value();
      int duty = dutyStr.toInt();
      pwm1.setDutyCycle(100-duty);
      request->send(200, "text/plain", "PWM1 ajustado a " + dutyStr);
    } else {
      request->send(400, "text/plain", "Faltan parámetro 'duty'");
    }
  });
  // Ajustar PWM2
  webServer.on("/setPWM2", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("duty")) {
      String dutyStr = request->getParam("duty")->value();
      int duty = dutyStr.toInt();
      pwm2.setDutyCycle(100-duty);
      request->send(200, "text/plain", "PWM2 ajustado a " + dutyStr);
    } else {
      request->send(400, "text/plain", "Faltan parámetro 'duty'");
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
      if (encoder.moved() || printOneTime()) {
        String menuOptions[] = {"Mediciones", "Control", "Configuracion"};
        print_menu(menuOptions, 3, "Menu principal");
      }
      if (encoder.isButtonPressed()) {
        state = MenuState(encoder.getPosition() + 1);
        encoder.setPosition(0);
        print = true;
      }
      break;
    case MEASUREMENTS:
      // Manejar las mediciones
      static int time = millis();
      if (encoder.moved() || millis() - time > 1000 || printOneTime()) { // Imprimir medidas si el encoder se mueve o ha pasado 1s
        time = millis();
        print_measurments();
      }
      if (encoder.isButtonPressed() && encoder.getPosition() == 9) {
        state = MAIN_MENU;
        encoder.setPosition(0);
        print = true;
      }
      break;
    case CONTROL_MENU:
      // Manejar el menu de control
      if (encoder.moved() || printOneTime()) print_control(encoder.getPosition());
      if (encoder.isButtonPressed()) {
        state = (encoder.getPosition() == CONTROL_EXIT) ? MAIN_MENU : CONTROL; // Salir del menu de control o ir a control
        control_state = ControlState(encoder.getPosition());
        encoder.setPosition(0);
        print = true;
      }
      break;
    case CONTROL:
      // Manejar el control
      switch (control_state) {
        case CONTROL_DO1:
          if (encoder.moved() || printOneTime()) control_digital(do1, "DO1");
          if (encoder.isButtonPressed()) {
            if (encoder.getPosition() == 0) {
              do1.toggle();
              control_digital(do1, "DO1");
            }
            else {
              state = CONTROL_MENU;
              encoder.setPosition(0);
              print = true;
            }
          }
          break;
        case CONTROL_DO2:
          if (encoder.moved() || printOneTime()) control_digital(do2, "DO2");
          if (encoder.isButtonPressed()) {
            if (encoder.getPosition() == 0) {
              do2.toggle();
              control_digital(do2, "DO2");
            }
            else {
              state = CONTROL_MENU;
              encoder.setPosition(0);
              print = true;
            }
          }
          break;
        case CONTROL_DO3:
          if (encoder.moved() || printOneTime()) control_digital(do3, "DO3");
          if (encoder.isButtonPressed()) {
            if (encoder.getPosition() == 0) {
              do3.toggle();
              control_digital(do3, "DO3");
            }
            else {
              state = CONTROL_MENU;
              encoder.setPosition(0);
              print = true;
            }
          }
          break;
        case CONTROL_AO1:
          encoder.setMaxPosition(5000/100);
          encoder.setMinPosition(0);
          while (!encoder.isButtonPressed()) {
            if (encoder.moved() || printOneTime()) {
              int val = encoder.getPosition() * 100;
              int digitalVal = (int)(val * DAC_MAX_DIGITAL_VALUE / 5000);
              dac.digitalWrite(digitalVal);
              control_dac(dac, "AO1");
            }
            encoder.update();
          }
          state = CONTROL_MENU;
          encoder.setPosition(0);
          print = true;
          break;
        case CONTROL_PWM1:
          encoder.setMaxPosition(10);
          encoder.setMinPosition(0);
          while (!encoder.isButtonPressed()) {
            if (encoder.moved() || printOneTime()) {
              pwm1.setDutyCycle(encoder.getPosition() * 10); // Ajustar el ciclo de trabajo de 10 en 10
              control_pwm(pwm1, "PWM1");
            }
            encoder.update();
          }
          state = CONTROL_MENU;
          encoder.setPosition(0);
          print = true;
          break;
        case CONTROL_PWM2:
          encoder.setMaxPosition(10);
          encoder.setMinPosition(0);
          while (!encoder.isButtonPressed()) {
            if (encoder.moved() || printOneTime()) {
              pwm2.setDutyCycle(encoder.getPosition() * 10); // Ajustar el ciclo de trabajo de 10 en 10
              control_pwm(pwm2, "PWM2");
            }
            encoder.update();
          }
          state = CONTROL_MENU;
          encoder.setPosition(0);
          print = true;
          break;
        case CONTROL_EXIT:
          state = CONTROL_MENU;
          encoder.setPosition(0);
          print = true;
          break;
      }
      break;
    case CONFIG:
      // Manejar la configuración
      if (encoder.isButtonPressed()) {
        state = MAIN_MENU;
        encoder.setPosition(0);
        print = true;
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
  Serial.println(text);
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
  String controlItems[] = {"DO1", "DO2", "DO3", "AO1", "PWM1", "PWM2", "Volver"};
  print_menu(controlItems, 7, "Control");
}

void control_digital(DigitalGPIO d, String pinName){
  String status = d.read() == HIGH ? "OFF" : "ON";
  String options[] = {status, "Volver"};
  print_menu(options, 2, pinName);
}

void control_dac(DAC d, String pinName){
  lcd.clear();
  print_line(0, "--Control de " + pinName + "--");
  float vinV = d.readVoltage() / 1000.0;
  print_line(1, "Valor: " + String(vinV, 2) + "V");
}

void control_pwm(PWM p, String pinName){
  lcd.clear();
  print_line(0, "--Control de " + pinName + "--");
  print_line(1, "Duty: " + String(p.getDutyCycle()*100/255) + "%");
}

bool printOneTime() { // Función para imprimir solo una vez
  if (print) {
    print = false;
    return true;
  }
  return false;
}