#include "encoder.h"

// Inicializar puntero de instancia estática
Encoder* Encoder::instance = nullptr;

// Constructor para inicializar el estado del encoder
Encoder::Encoder() : lastState(LOW), position(0), buttonPressed(false), encoderMaxPosition(10), encoderMinPosition(1) { }

// Inicializar pines y interrupciones del encoder
void Encoder::init() {
    pinMode(ENCODER_CLK, INPUT);
    pinMode(ENCODER_DT, INPUT);
    pinMode(ENCODER_SW, INPUT_PULLUP);
    lastState = digitalRead(ENCODER_CLK);

    instance = this;

    // Adjuntar interrupciones sin llamar a gpio_install_isr_service
    attachInterrupt(digitalPinToInterrupt(ENCODER_CLK), handleInterrupt, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCODER_SW), handleButtonInterrupt, FALLING);
}

// Manejador de interrupciones para la rotación del encoder
void IRAM_ATTR Encoder::handleInterrupt() {
    static unsigned long lastInterruptTime = 0;
    unsigned long interruptTime = millis();
    
    // Antirrebote: ignorar interrupciones que ocurren dentro de los 15ms de la última interrupción
    if (interruptTime - lastInterruptTime > ENCODER_ROTATION_DEBOUNCE) {
        if (instance) {
            int state = digitalRead(ENCODER_CLK);
            if (state != instance->lastState) { // Si el estado ha cambiado, ocurrió un pulso
                if (digitalRead(ENCODER_DT) != state) { // Si el estado de DT es diferente del estado de CLK
                    instance->position++;
                } else {
                    instance->position--;
                }
                instance->lastState = state;
            }
        }
    }
    lastInterruptTime = interruptTime;
}

// Manejador de interrupciones para la pulsación del botón
void IRAM_ATTR Encoder::handleButtonInterrupt() {
    static unsigned long lastButtonInterruptTime = 0;
    unsigned long interruptTime = millis();
    
    // Antirrebote: ignorar interrupciones que ocurren dentro de los 50ms de la última interrupción
    if (interruptTime - lastButtonInterruptTime > ENCODER_BUTTON_DEBOUNCE) {
        if (instance) {
            instance->buttonPressed = true;
        }
    }
    lastButtonInterruptTime = interruptTime;
}

// Verificar si el botón fue presionado
bool Encoder::isButtonPressed() {
    bool wasPressed = buttonPressed;
    buttonPressed = false;  // Restablecer después de leer
    return wasPressed;
}

// Obtener la posición actual del encoder
int Encoder::getPosition() {
    if (position > encoderMaxPosition) {
        setPosition(encoderMaxPosition);
    } else if (position < encoderMinPosition) {
        setPosition(encoderMinPosition);
    }
    return position;
}

// Establecer la posición del encoder
void Encoder::setPosition(int pos) {
    position = pos;
}

// Establecer la posición máxima del encoder
void Encoder::setMaxPosition(int maxPos) {
    encoderMaxPosition = maxPos;
    if (position > encoderMaxPosition) setPosition(encoderMaxPosition);
}

// Establecer la posición mínima del encoder
void Encoder::setMinPosition(int minPos) {
    encoderMinPosition = minPos;
    if (position < encoderMinPosition) setPosition(encoderMinPosition);
}

// Verificar si el encoder se movió
bool Encoder::moved(){
    static int last_position = 0;
    if (last_position != position) {
        last_position = position;
        return true;
    }
    return false;
}

// Función de actualización (actualmente no hace nada)
void Encoder::update() {
    // Nada que hacer aquí
}