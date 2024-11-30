#include "lcdsim.h"

// Constructor de la clase SimulatedLCD
SimulatedLCD::SimulatedLCD() {
    clear();
}

// Inicializa la pantalla LCD simulada
void SimulatedLCD::init(){
    // No se requiere inicialización
    clear();
}

// Enciende la luz de fondo de la pantalla LCD simulada
// Esta función no hace nada en la simulación
void SimulatedLCD::backlight() {}

// Limpia la pantalla y reinicia la posición del cursor
void SimulatedLCD::clear() {
    for (uint8_t i = 0; i < LCD_ROWS; i++) {
        for (uint8_t j = 0; j < LCD_COLS; j++) {
            _display[i][j] = ' ';
        }
        _display[i][LCD_COLS] = '\0'; // Carácter nulo al final de cada línea
    }
    _curr_col = 0;
    _curr_row = 0;
    render();
}

// Establece el cursor en la posición especificada
// @param col La posición de la columna (basada en 0)
// @param row La posición de la fila (basada en 0)
void SimulatedLCD::setCursor(uint8_t col, uint8_t row) {
    if (col < LCD_COLS && row < LCD_ROWS) {
        _curr_col = col;
        _curr_row = row;
    }
}

// Imprime una cadena en la pantalla LCD simulada
// @param str La cadena a imprimir
// @return El número de caracteres impresos
size_t SimulatedLCD::print(const char* str) {
    while (*str) {
        _display[_curr_row][_curr_col] = *str++;
        _curr_col++;
        if (_curr_col >= LCD_COLS) {
            _curr_col = 0;
            _curr_row++;
            if (_curr_row >= LCD_ROWS) {
            _curr_row = 0;
            }
        }
    }
    render();
    return 1;
}

// Renderiza el estado actual de la pantalla LCD simulada en la salida serial
void SimulatedLCD::render() {
    Serial.println("--------- Simulación LCD 2004A ---------");
    for (uint8_t i = 0; i < LCD_ROWS; i++) {
        Serial.print("|");
        Serial.print(_display[i]);
        Serial.println("|");
    }
    Serial.println("---------------------------------------");
}