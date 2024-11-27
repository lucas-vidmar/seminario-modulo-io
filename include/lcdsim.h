#pragma once
#include <Arduino.h>

#define LCD_COLS 20
#define LCD_ROWS 4

class SimulatedLCD {
  private:
    uint8_t _curr_col;
    uint8_t _curr_row;
    char _display[LCD_ROWS][LCD_COLS + 1]; // +1 para el carácter nulo al final de cada línea

  public:
    SimulatedLCD();
    void init();
    void backlight();
    void clear();
    void setCursor(uint8_t col, uint8_t row);
    size_t print(const char* str);
    void render();
};