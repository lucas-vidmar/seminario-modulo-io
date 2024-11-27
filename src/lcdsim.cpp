#include "lcdsim.h"


SimulatedLCD::SimulatedLCD() {
    clear();
}

void SimulatedLCD::init(){
    // No se requiere inicialización
    clear();
}

void SimulatedLCD::backlight() {}

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

void SimulatedLCD::setCursor(uint8_t col, uint8_t row) {
    if (col < LCD_COLS && row < LCD_ROWS) {
    _curr_col = col;
    _curr_row = row;
    }
}

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

void SimulatedLCD::render() {
    Serial.println("--------- Simulación LCD 2004A ---------");
    for (uint8_t i = 0; i < LCD_ROWS; i++) {
    Serial.print("|");
    Serial.print(_display[i]);
    Serial.println("|");
    }
    Serial.println("---------------------------------------");
}