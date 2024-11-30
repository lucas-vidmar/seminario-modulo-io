#pragma once

#include <Arduino.h>
#include "I2CScanner.h"

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
#include "pwm.h"

void HTMLHandlers();
void print_menu(String options[],int options_size, String title);
void print_line(int line, String text);
void fsm();
void print_measurments();
void print_control(int selected);
void control_digital(DigitalGPIO d, String pinName);
void control_analog(AnalogGPIO a, String pinName);
void control_dac(DAC d, String pinName);
void control_pwm(PWM p, String pinName);
bool printOneTime();

// Define GPIOs
#define DI1 GPIO_NUM_26
#define DI2 GPIO_NUM_27
#define DI3 GPIO_NUM_36
#define DO1 GPIO_NUM_15
#define DO2 GPIO_NUM_33
#define DO3 GPIO_NUM_32
#define AI1 GPIO_NUM_34
#define AI2 GPIO_NUM_35
//#define AO1 GPIO_NUM_25
#define PPWM1 GPIO_NUM_4
#define PPWM2 GPIO_NUM_5
#define PWMFREQ 50

#define USE_LCD