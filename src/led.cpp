#include "led.h"

// El constructor inicializa el temporizador a nullptr
BuiltInLed::BuiltInLed() : timer(nullptr) { }

void BuiltInLed::init() {
    // Configurar el pin del LED incorporado como salida
    pinMode(BUILT_IN_LED_PIN, OUTPUT);
    // Asegurarse de que el LED esté inicialmente apagado
    digitalWrite(BUILT_IN_LED_PIN, LOW);
    // Inicializar el temporizador con un prescaler de 80
    timer = timerBegin(0, 80, true);
}

void BuiltInLed::blink(int interval) {
    // Adjuntar la interrupción del temporizador a la función onTimer
    timerAttachInterrupt(timer, &onTimer, true);
    // Configurar la alarma del temporizador para que se active en el intervalo especificado (en microsegundos)
    timerAlarmWrite(timer, interval * 1000, true);
    // Habilitar la alarma del temporizador
    timerAlarmEnable(timer);
}

// Manejador de interrupciones del temporizador para alternar el estado del LED
void BuiltInLed::onTimer() {
    digitalWrite(BUILT_IN_LED_PIN, !digitalRead(BUILT_IN_LED_PIN));
}

// Alternar manualmente el estado del LED
void BuiltInLed::toggle() {
    digitalWrite(BUILT_IN_LED_PIN, !digitalRead(BUILT_IN_LED_PIN));
}