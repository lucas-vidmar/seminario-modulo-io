#pragma once

#include <Arduino.h>

#include "encoder.h"
#include "led.h"
#include "i2c.h"
#include "dac.h"
#include "adc.h"
#include "gpio.h"
#include "webserver.h"
#include "LiquidCrystal_I2C.h"
#include "lcdsim.h"
#include "encodersim.h"

void HTMLHandlers();
void print_menu(int selected);
void print_line(int line, String text);
void fsm();
void print_measurments();

// Define GPIOs
#define DI1 GPIO_NUM_26
#define DI2 GPIO_NUM_27
#define DI3 GPIO_NUM_36
#define DO1 GPIO_NUM_15
#define DO2 GPIO_NUM_32
#define DO3 GPIO_NUM_33
#define AI1 GPIO_NUM_34
#define AI2 GPIO_NUM_35
#define AO1 GPIO_NUM_25