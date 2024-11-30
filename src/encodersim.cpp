#include "encodersim.h"

SimulatedEncoder::SimulatedEncoder()
    : position(0), buttonPressed(false), encoderMaxPosition(10), encoderMinPosition(1) { }

// Inicializa el encoder simulado e imprime los comandos disponibles
void SimulatedEncoder::init() {
    Serial.println("SimulatedEncoder inicializado.");
    Serial.println("Comandos:");
    Serial.print("  '"); Serial.print(SIMENCODER_UP); Serial.println("' - Girar hacia arriba (incrementar posición)");
    Serial.print("  '"); Serial.print(SIMENCODER_DOWN); Serial.println("' - Girar hacia abajo (decrementar posición)");
    Serial.print("  '"); Serial.print(SIMENCODER_PRESS); Serial.println("' - Pulsar el botón");
}

// Actualiza el estado del encoder basado en la entrada serial
void SimulatedEncoder::update() {
    static unsigned long last_time = 0;
    // Procesar la entrada serial sin bloquear
    while (Serial.available() > 0) {
        // Antirrebote
        if (millis() - last_time < SIMENCODER_DEBOUNCE) {
            // Ignorar la entrada actual debido al antirrebote
            Serial.read(); // Descartar el carácter actual
            continue; // Continuar con el siguiente carácter
        }

        last_time = millis(); // Actualizar el tiempo de la última entrada
        char cmd = Serial.read();
        switch (cmd) {
            case SIMENCODER_UP:
                position++;
                if (position > encoderMaxPosition) {
                    position = encoderMaxPosition;
                }
                break;
            case SIMENCODER_DOWN:
                position--;
                if (position < encoderMinPosition) {
                    position = encoderMinPosition;
                }
                break;
            case SIMENCODER_PRESS:
                buttonPressed = true;
                break;
            default:
                // Ignorar otros caracteres
                break;
        }
    }
}

// Verifica si el botón fue presionado
bool SimulatedEncoder::isButtonPressed() {
    bool wasPressed = buttonPressed;
    buttonPressed = false;  // Reiniciar después de leer
    return wasPressed;
}

// Obtiene la posición actual del encoder
int SimulatedEncoder::getPosition() {
    // Asegurarse de que la posición esté dentro de los límites
    if (position > encoderMaxPosition) {
        position = encoderMaxPosition;
    } else if (position < encoderMinPosition) {
        position = encoderMinPosition;
    }
    return position;
}

// Establece la posición del encoder
void SimulatedEncoder::setPosition(int pos) {
    position = pos;
    // Asegurarse de que la posición esté dentro de los límites
    if (position > encoderMaxPosition) {
        position = encoderMaxPosition;
    } else if (position < encoderMinPosition) {
        position = encoderMinPosition;
    }
}

// Establece la posición máxima del encoder
void SimulatedEncoder::setMaxPosition(int maxPos) {
    encoderMaxPosition = maxPos;
    if (position > encoderMaxPosition) {
        position = encoderMaxPosition;
    }
}

// Establece la posición mínima del encoder
void SimulatedEncoder::setMinPosition(int minPos) {
    encoderMinPosition = minPos;
    if (position < encoderMinPosition) {
        position = encoderMinPosition;
    }
}

// Verifica si la posición del encoder ha cambiado
bool SimulatedEncoder::moved(){
    static int last_position = 0;
    if (last_position != position) {
        last_position = position;
        return true;
    }
    return false;
}