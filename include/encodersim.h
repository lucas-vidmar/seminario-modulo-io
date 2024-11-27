#ifndef SIMULATED_ENCODER_H
#define SIMULATED_ENCODER_H

#include <Arduino.h>

// Definiciones de pines y comandos
#define SIMENCODER_CLK GPIO_NUM_12
#define SIMENCODER_DT GPIO_NUM_13
#define SIMENCODER_SW GPIO_NUM_14
#define SIMENCODER_DEBOUNCE 15  // Tiempo de debounce en milisegundos

#define SIMENCODER_UP 'u'
#define SIMENCODER_DOWN 'd'
#define SIMENCODER_PRESS 'p'
#define SIMENCODER_RELEASE 'r'

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

private:
    int position;
    bool buttonPressed;
    int encoderMaxPosition;
    int encoderMinPosition;

    // Variables para el estado del botón y debounce
    bool buttonState;
    unsigned long lastUpdateTime;
    unsigned long lastButtonTime;
};

#endif // SIMULATED_ENCODER_H
