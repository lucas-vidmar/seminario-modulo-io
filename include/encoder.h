/**
 * @file encoder.h
 * @brief Archivo de encabezado para la clase Encoder.
 * 
 * Este archivo contiene la definición y documentación para la clase Encoder,
 * que proporciona funcionalidad para interactuar con un codificador rotatorio.
 * La clase Encoder incluye métodos para inicializar el codificador, verificar
 * pulsaciones de botones, recuperar y establecer la posición del codificador,
 * y manejar interrupciones.
 * 
 * @note Esta clase asume que el hardware subyacente soporta codificadores rotatorios.
 * @note Esta clase asume que solo se usa un codificador.
 * 
 */
#pragma once

#include <Arduino.h>

#define ENCODER_CLK GPIO_NUM_12
#define ENCODER_DT GPIO_NUM_13
#define ENCODER_SW GPIO_NUM_14
#define ENCODER_ROTATION_DEBOUNCE 15
#define ENCODER_BUTTON_DEBOUNCE 200
#define ENCODER_BUTTON_LONG_PRESS 1000

/**
 * @class Encoder
 * @brief Clase para manejar un codificador rotativo.
 * 
 * La clase Encoder proporciona una interfaz para interactuar con un codificador rotativo,
 * permitiendo inicializarlo, leer su posición, verificar el estado del botón y manejar
 * interrupciones asociadas.
 * 
 * @note Esta clase está diseñada para ser utilizada en sistemas embebidos donde el manejo
 * de interrupciones y la eficiencia son críticos.
 */
class Encoder {
public:

    /**
     * @brief Construir un nuevo objeto Encoder.
     * 
     * Este constructor inicializa una nueva instancia de la clase Encoder.
     */
    Encoder();

    /**
     * @brief Inicializa el codificador.
     * 
     * Esta función configura las configuraciones necesarias e inicializa el codificador
     * para su uso. Debe ser llamada antes de cualquier otra función relacionada con el codificador.
     */
    void init();

    /**
     * @brief Verifica si el botón está presionado.
     * 
     * Esta función devuelve un valor booleano que indica si el botón
     * asociado con el codificador está actualmente presionado.
     * 
     * @return true si el botón está presionado, false en caso contrario.
     */
    bool isButtonPressed();

    /**
     * @brief Recupera la posición actual del codificador.
     * 
     * @return int El valor de la posición actual del codificador.
     */
    int getPosition();

    /**
     * @brief Establece la posición del codificador.
     * 
     * Esta función actualiza la posición actual del codificador al valor especificado.
     * 
     * @param pos La nueva posición para establecer en el codificador.
     */
    void setPosition(int pos);

    /**
     * @brief Restablece el estado del botón.
     * 
     * Esta función se utiliza para restablecer cualquier estado o contador asociado con el botón.
     * Debe ser llamada cuando el botón necesita ser reinicializado o limpiado.
     */
    void resetButton();

    /**
     * @brief Establece la posición máxima para el codificador.
     * 
     * Esta función establece el límite superior para la posición del codificador. El codificador
     * no reportará posiciones superiores a este valor.
     * 
     * @param maxPos El valor de la posición máxima a establecer.
     */
    void setMaxPosition(int maxPos);

    /**
     * @brief Establece la posición mínima para el codificador.
     * 
     * Esta función establece el valor mínimo de posición que puede tener el codificador.
     * Se utiliza para definir el límite inferior del rango del codificador.
     * 
     * @param minPos El valor de la posición mínima a establecer.
     */
    void setMinPosition(int minPos);

    /**
     * @brief Verifica si el codificador se ha movido.
     * 
     * Esta función devuelve un valor booleano que indica si el codificador
     * ha detectado algún movimiento desde la última verificación.
     * 
     * @return true si el codificador se ha movido, false en caso contrario.
     */
    bool moved();

    /**
     * @brief Actualiza el estado del codificador (no hace nada en la versión real).
     * 
     */
    void update();

private:

    /**
     * @brief Manejador de interrupciones para el codificador.
     * 
     * Esta función está marcada con IRAM_ATTR para asegurar que se coloque en la
     * RAM interna (IRAM) para una ejecución más rápida. Esto es particularmente importante
     * para las rutinas de servicio de interrupciones (ISR) para minimizar la latencia.
     */
    static void IRAM_ATTR handleInterrupt();

    /**
     * @brief Rutina de Servicio de Interrupción (ISR) para manejar interrupciones de botones.
     * 
     * Esta función está marcada con IRAM_ATTR para asegurar que se coloque en la
     * IRAM (RAM de Instrucción) para una ejecución más rápida. Está destinada a ser
     * llamada cuando ocurre una interrupción de botón, permitiendo un manejo rápido y
     * eficiente del evento de pulsación del botón.
     */
    static void IRAM_ATTR handleButtonInterrupt();

    static Encoder* instance;       // Puntero de instancia global
    volatile int lastState;         // Estado del pin CLK
    volatile int position;          // Posición del codificador (marcado como volatile)
    volatile bool buttonPressed;    // Estado de pulsación del botón (marcado como volatile)
    int encoderMaxPosition;         // Valor máximo de posición para el codificador
    int encoderMinPosition;         // Valor mínimo de posición para el codificador
};
