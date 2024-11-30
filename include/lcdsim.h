/**
 * @file lcdsim.h
 * @brief Archivo de encabezado para la clase SimulatedLCD.
 *
 * Este archivo contiene la definición de la clase SimulatedLCD, que se utiliza para simular una pantalla LCD.
 * La clase proporciona métodos para inicializar la pantalla, controlar la retroiluminación, borrar la pantalla,
 * configurar la posición del cursor, imprimir texto y representar el contenido de la pantalla.
 *
 */
#pragma once
#include <Arduino.h>

#define LCD_COLS 20
#define LCD_ROWS 4

/**
 * @class SimulatedLCD
 * @brief Una clase para simular una pantalla LCD con fines de prueba.
 * 
 * Esta clase proporciona métodos para simular el comportamiento de una pantalla LCD,
 * incluyendo la inicialización de la pantalla, el control de la retroiluminación, 
 * el borrado de la pantalla, la configuración de la posición del cursor, la impresión 
 * de texto y la representación del contenido de la pantalla.
 */
class SimulatedLCD {
  private:
    uint8_t _curr_col;
    uint8_t _curr_row;
    char _display[LCD_ROWS][LCD_COLS + 1]; // +1 para el carácter nulo al final de cada línea

  public:

    /**
    * @brief Construye un nuevo objeto SimulatedLCD.
    */
    SimulatedLCD();

    /**
    * @brief Inicializa la pantalla LCD simulada.
    */
    void init();

    /**
    * @brief Enciende la retroiluminación de la pantalla LCD simulada.
    */
    void backlight();

    /**
    * @brief Borra la pantalla LCD simulada.
    */
    void clear();

    /**
   * @brief Establece la posición del cursor en la pantalla LCD simulada.
    * 
    * @param col La posición de la columna (basada en 0).
    * @param row La posición de la fila (basada en 0).
    */
    void setCursor(uint8_t col, uint8_t row);

    /**
    * @brief Imprime una cadena en la pantalla LCD simulada en la posición actual del cursor.
    * 
    * @param str La cadena a imprimir.
    * @return El número de caracteres impresos.
    */
    size_t print(const char* str);

    /**
    * @brief Representa el contenido actual de la pantalla LCD simulada.
    */
    void render();
};