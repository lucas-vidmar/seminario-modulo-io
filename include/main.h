/**
 * @file main.h
 * @brief Archivo de encabezado principal para el proyecto.
 *
 * Este archivo contiene las declaraciones de funciones y variables utilizadas en el proyecto.
 * Incluye la inicialización de módulos, la configuración de HTML, la lógica de la máquina de estados finitos (FSM),
 * y la impresión de mediciones y menús.
 */
#pragma once

#include <Arduino.h>
#include "I2CScanner.h"

/*-- Módulos --*/
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

// Definir GPIOs
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

/**
 * @brief Inicializa y configura los manejadores de HTML.
 *
 * Esta función es responsable de configurar los manejadores necesarios
 * para procesar contenido HTML. Debe ser llamada durante la fase de 
 * inicialización de la aplicación para asegurar que todas las funcionalidades 
 * relacionadas con HTML estén correctamente configuradas.
 */
void HTMLHandlers();

/**
 * @brief Imprime un menú con las opciones y el título dados.
 * 
 * @param options Un array de cadenas que representan las opciones del menú.
 * @param options_size El número de opciones en el menú.
 * @param title El título del menú.
 */
void print_menu(String options[], int options_size, String title);

/**
 * @brief Imprime una línea de texto.
 * 
 * Esta función imprime el texto especificado en el número de línea dado.
 * 
 * @param line El número de línea donde se debe imprimir el texto.
 * @param text El texto a imprimir.
 */
void print_line(int line, String text);

/**
 * @brief Función para manejar la lógica de la máquina de estados finitos (FSM).
 *
 * Esta función es responsable de gestionar los estados y transiciones
 * de la máquina de estados finitos. Debe ser llamada periódicamente para
 * asegurar que la FSM opere correctamente.
 */
void fsm();

/**
 * @brief Imprime las mediciones en la salida estándar.
 *
 * Esta función recupera e imprime las mediciones de manera formateada.
 * Los detalles específicos de las mediciones y su formato dependen de la 
 * implementación.
 */
void print_measurments();

/**
 * @brief Imprime un mensaje una sola vez.
 * 
 * Esta función está destinada a imprimir un mensaje específico o realizar una acción específica solo una vez.
 * 
 * @return true si el mensaje se imprimió correctamente, false en caso contrario.
 */
bool printOneTime();

/**
 * @brief Imprime la información de control basada en la opción seleccionada.
 * 
 * @param selected Un entero que representa la opción de control seleccionada.
 */
void print_control(int selected);

/**
 * @brief Controla un pin GPIO digital.
 * 
 * Esta función permite controlar un pin GPIO digital especificando el pin
 * y su nombre.
 * 
 * @param d El objeto DigitalGPIO que representa el pin digital a controlar.
 * @param pinName El nombre del pin como una cadena.
 */
void control_digital(DigitalGPIO d, String pinName);

/**
 * @brief Controla un pin GPIO analógico.
 * 
 * @param a Una instancia de AnalogGPIO que representa el GPIO analógico a controlar.
 * @param pinName Una cadena que representa el nombre del pin a controlar.
 */
void control_analog(AnalogGPIO a, String pinName);

/**
 * @brief Controla el DAC (Convertidor Digital a Analógico) basado en el pin especificado.
 * 
 * @param d El objeto DAC a controlar.
 * @param pinName El nombre del pin asociado con el DAC.
 */
void control_dac(DAC d, String pinName);

/**
 * @brief Controla la señal PWM en un pin especificado.
 * 
 * @param p El objeto PWM que contiene la configuración para la señal PWM.
 * @param pinName El nombre del pin donde se aplicará la señal PWM.
 */
void control_pwm(PWM p, String pinName);