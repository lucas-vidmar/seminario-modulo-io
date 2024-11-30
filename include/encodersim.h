#pragma once

#include <Arduino.h>

// Definiciones de comandos
#define SIMENCODER_DEBOUNCE 15  // Tiempo de debounce en milisegundos
#define SIMENCODER_UP '6'
#define SIMENCODER_DOWN '4'
#define SIMENCODER_PRESS '5'

class SimulatedEncoder {
public:
    SimulatedEncoder();

    void init();

    // Simular la actualización (procesar entradas del usuario)
    void update();

    bool isButtonPressed();

    int getPosition();
    void setPosition(int pos);

    void setMaxPosition(int maxPos);
    void setMinPosition(int minPos);

    bool moved();

private:
    int position;
    bool buttonPressed;
    int encoderMaxPosition;
    int encoderMinPosition;
};