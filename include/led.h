/**
 * @file led.h
 * @brief Archivo de encabezado para la clase BuiltInLed.
 * 
 * Este archivo contiene la declaración de la clase BuiltInLed, que proporciona métodos
 * para controlar el LED incorporado en un microcontrolador. La clase incluye funcionalidad
 * para inicializar el hardware del LED y hacer que el LED parpadee a un intervalo especificado.
 * 
 */
#pragma once

#include <Arduino.h>

#define BUILT_IN_LED_PIN GPIO_NUM_2

/**
 * @class BuiltInLed
 * @brief Una clase para controlar el LED incorporado en un microcontrolador.
 * 
 * La clase BuiltInLed proporciona métodos para inicializar y controlar el LED incorporado
 * en un microcontrolador. Incluye funcionalidad para inicializar el hardware del LED
 * y hacer que el LED parpadee a un intervalo especificado.
 */
class BuiltInLed {
    
public:
    /**
     * @brief Constructor de la clase BuiltInLed.
     * 
     * Este constructor inicializa el LED incorporado en el microcontrolador.
     */
    BuiltInLed();

    /**
     * @brief Inicializa el hardware del LED.
     * 
     * Esta función configura las configuraciones necesarias para inicializar el hardware del LED.
     * Debe ser llamada antes de usar cualquier otra función relacionada con el LED.
     */
    void init();

    /**
     * @brief Hace parpadear un LED a un intervalo especificado.
     * 
     * Esta función hará que el LED parpadee encendiéndose y apagándose en el intervalo especificado
     * por el parámetro. El intervalo se da en milisegundos.
     * 
     * @param interval El tiempo en milisegundos para que el LED permanezca encendido o apagado.
     */
    void blink(int interval);

    /**
     * @brief Alterna el estado del LED.
     * 
     * Esta función cambia el estado del LED de encendido a apagado o de apagado a encendido.
     */
    void toggle();

private:
    hw_timer_t *timer;

    /**
     * @brief Método estático para manejar eventos de temporizador.
     * 
     * Este método se llama cuando ocurre un evento de temporizador. Es responsable de
     * ejecutar las acciones necesarias asociadas con el temporizador.
     */
    static void onTimer();
};