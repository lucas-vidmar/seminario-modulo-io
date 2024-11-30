/**
 * @file encodersim.h
 * @brief Archivo de encabezado para la clase SimulatedEncoder.
 * 
 * Este archivo contiene la definición de la clase SimulatedEncoder, que se utiliza para simular un codificador.
 * La clase proporciona métodos para inicializar el codificador, actualizar su estado basado en entradas del usuario, y verificar
 * el estado del codificador (posición, pulsación de botón, movimiento).
 * 
 */
#pragma once

#include <Arduino.h>

// Definiciones de comandos
#define SIMENCODER_DEBOUNCE 15  // Tiempo de debounce en milisegundos
#define SIMENCODER_UP '6'
#define SIMENCODER_DOWN '4'
#define SIMENCODER_PRESS '5'

/**
 * @class SimulatedEncoder
 * @brief Una clase para simular un codificador con seguimiento de posición y detección de pulsación de botón.
 * 
 * Esta clase proporciona métodos para inicializar, actualizar e interactuar con un codificador simulado.
 * Permite establecer y obtener la posición del codificador, así como verificar el movimiento
 * y los estados de pulsación de botón.
 * 
 * @note La posición del codificador está limitada por un valor mínimo y máximo.
 * 
 */
class SimulatedEncoder {
public:
    /**
     * @brief Construir un nuevo objeto Simulated Encoder.
     */
    SimulatedEncoder();

    /**
     * @brief Inicializar el codificador simulado.
     */
    void init();

    /**
     * @brief Simular el proceso de actualización (procesar entradas del usuario).
     */
    void update();

    /**
     * @brief Verificar si el botón está presionado.
     * 
     * @return true si el botón está presionado, false en caso contrario.
     */
    bool isButtonPressed();

    /**
     * @brief Obtener la posición actual del codificador.
     * 
     * @return int Posición actual.
     */
    int getPosition();

    /**
     * @brief Establecer la posición del codificador.
     * 
     * @param pos Nueva posición.
     */
    void setPosition(int pos);

    /**
     * @brief Establecer la posición máxima del codificador.
     * 
     * @param maxPos Posición máxima.
     */
    void setMaxPosition(int maxPos);

    /**
     * @brief Establecer la posición mínima del codificador.
     * 
     * @param minPos Posición mínima.
     */
    void setMinPosition(int minPos);

    /**
     * @brief Verificar si el codificador se ha movido.
     * 
     * @return true si el codificador se ha movido, false en caso contrario.
     */
    bool moved();

    /**
     * @brief Verificar si el codificador se ha movido a la izquierda.
     * 
     * @return true si el codificador se ha movido a la izquierda, false en caso contrario.
     */
    bool movedLeft();

    /**
     * @brief Verificar si el codificador se ha movido a la derecha.
     * 
     * @return true si el codificador se ha movido a la derecha, false en caso contrario.
     */
    bool movedRight();

private:
    int position; ///< Posición actual del codificador.
    bool buttonPressed; ///< Estado del botón.
    int encoderMaxPosition; ///< Posición máxima del codificador.
    int encoderMinPosition; ///< Posición mínima del codificador.
};