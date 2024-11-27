#include "encodersim.h"

SimulatedEncoder::SimulatedEncoder()
    : position(0), buttonPressed(false), encoderMaxPosition(10), encoderMinPosition(1), buttonState(false) { }

void SimulatedEncoder::init() {
    Serial.println("SimulatedEncoder inicializado.");
    Serial.println("Comandos:");
    Serial.print("  '"); Serial.print(SIMENCODER_UP); Serial.println("' - Girar hacia arriba (incrementar posición)");
    Serial.print("  '"); Serial.print(SIMENCODER_DOWN); Serial.println("' - Girar hacia abajo (decrementar posición)");
    Serial.print("  '"); Serial.print(SIMENCODER_PRESS); Serial.println("' - Pulsar el botón");
    Serial.print("  '"); Serial.print(SIMENCODER_RELEASE); Serial.println("' - Soltar el botón");
}

void SimulatedEncoder::update() {
    static unsigned long last_time = 0;
    // Procesar la entrada serial sin bloquear
    while (Serial.available() > 0) {
        // Debounce
        if (millis() - last_time < SIMENCODER_DEBOUNCE) {
            // Ignorar entrada actual por debounce
            Serial.read(); // Descartar el carácter actual
            continue; // Seguir con el siguiente carácter
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
                if (!buttonState) {
                    buttonPressed = true;
                    buttonState = true;
                }
                break;
            case SIMENCODER_RELEASE:
                buttonState = false;
                break;
            default:
                // Ignorar otros caracteres
                break;
        }
    }
}

bool SimulatedEncoder::isButtonPressed() {
    bool wasPressed = buttonPressed;
    buttonPressed = false;  // Reset después de leer
    return wasPressed;
}

int SimulatedEncoder::getPosition() {
    // Asegurarse de que la posición esté dentro de los límites
    if (position > encoderMaxPosition) {
        position = encoderMaxPosition;
    } else if (position < encoderMinPosition) {
        position = encoderMinPosition;
    }
    return position;
}

void SimulatedEncoder::setPosition(int pos) {
    position = pos;
    // Asegurarse de que la posición esté dentro de los límites
    if (position > encoderMaxPosition) {
        position = encoderMaxPosition;
    } else if (position < encoderMinPosition) {
        position = encoderMinPosition;
    }
}

void SimulatedEncoder::setMaxPosition(int maxPos) {
    encoderMaxPosition = maxPos;
    if (position > encoderMaxPosition) {
        position = encoderMaxPosition;
    }
}

void SimulatedEncoder::setMinPosition(int minPos) {
    encoderMinPosition = minPos;
    if (position < encoderMinPosition) {
        position = encoderMinPosition;
    }
}
