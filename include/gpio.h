/**
 * @file gpio.h
 * @brief Archivo de encabezado para la clase GPIO.
 *
 * Este archivo contiene la definición de la clase GPIO, que proporciona una interfaz
 * para operaciones de Entrada/Salida de Propósito General (GPIO).
 * Incluye métodos para configurar, leer y escribir en pines GPIO.
 *
 */
#pragma once

#include <Arduino.h>

/**
 * @class GPIO
 * @brief Clase base abstracta para operaciones de Entrada/Salida de Propósito General (GPIO).
 * 
 * Esta clase proporciona una interfaz para configurar, leer y escribir en pines GPIO.
 */
class GPIO {
public:
    virtual void setup() = 0;
    virtual int read() = 0;
    virtual void write(int value) = 0;
};

/**
 * @class DigitalGPIO
 * @brief Una clase para representar un pin GPIO digital.
 * 
 * Esta clase proporciona métodos para configurar, leer, escribir y alternar un pin GPIO digital.
 */
class DigitalGPIO : public GPIO {
private:
    uint8_t pin;
    uint8_t mode;

public:

    /**
    * @brief Constructor para DigitalGPIO.
    * 
    * @param pin El número del pin GPIO.
    * @param mode El modo del pin GPIO (por ejemplo, INPUT, OUTPUT).
    */
    DigitalGPIO(uint8_t pin, uint8_t mode);

    /**
    * @brief Configura el pin GPIO con el modo especificado.
    * 
    * Este método sobrescribe el método setup en la clase base GPIO.
    */
    void setup() override;

    /**
    * @brief Lee el valor actual del pin GPIO.
    * 
    * @return El valor actual del pin GPIO (por ejemplo, HIGH o LOW).
    * 
    * Este método sobrescribe el método read en la clase base GPIO.
    */
    int read() override;

    /**
    * @brief Escribe un valor en el pin GPIO.
    * 
    * @param value El valor a escribir en el pin GPIO (por ejemplo, HIGH o LOW).
    * 
    * Este método sobrescribe el método write en la clase base GPIO.
    */
    void write(int value) override;

    /**
    * @brief Alterna el valor actual del pin GPIO.
    * 
    * Este método cambia el estado del pin GPIO de HIGH a LOW o de LOW a HIGH.
    */
    void toggle();
};

/**
 * @class AnalogGPIO
 * @brief Una clase para representar un pin GPIO analógico.
 * 
 * Esta clase hereda de la clase base GPIO y proporciona
 * funcionalidad para configurar, leer y escribir en un pin GPIO analógico.
 */
class AnalogGPIO : public GPIO {
private:
    uint8_t pin;

public:
    /**
     * @brief Construye un objeto AnalogGPIO con el pin especificado.
     * 
     * @param pin El número del pin a ser usado para el GPIO analógico.
     */
    AnalogGPIO(uint8_t pin);

    /**
     * @brief Configura la configuración del GPIO.
     *
     * Esta función sobrescribe la función setup de la clase base para inicializar
     * los pines GPIO y configurarlos según sea necesario para la aplicación.
     */
    void setup() override;

    /**
     * @brief Lee el valor actual del pin GPIO.
     * 
     * Esta función sobrescribe la implementación de la clase base para leer el
     * estado actual del pin GPIO y devolver su valor.
     * 
     * @return int El valor actual del pin GPIO (típicamente 0 o 1).
     */
    int read() override;
    
    /**
     * @brief Escribe un valor en el pin GPIO.
     * 
     * Esta función sobrescribe la implementación de la clase base para escribir un valor
     * entero especificado en el pin GPIO. El comportamiento exacto y el efecto de esta operación
     * de escritura dependen del hardware específico y la configuración del GPIO.
     * 
     * @param value El valor entero a ser escrito en el pin GPIO.
     */
    void write(int value) override;
};
