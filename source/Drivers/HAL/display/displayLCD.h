/***************************************************************************//**
  @file     displayLCD.h
  @brief    Módulo para facilitar el uso del display LCD por medio de I2C
 ******************************************************************************/

#ifndef _DISPLAYLCD_H_
#define _DISPLAYLCD_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdint.h>
#include <stdbool.h>


//definiciones generales
#define CURSOR      '>'
#define BLANK       "                "
#define ROWS        4 //cantidad de filas del display 1, 2 o 4

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Inicializa el display LCD por medio de I2C y configura todo el display
 */
void displayLCD_Init(uint8_t address);

void displayLCD_Begin(void);

void displayLCD_createChar(uint8_t location, uint8_t charmap[]);

/**
 * @brief Muestra un string en la primera línea del display
 * @param str Cadena de caracteres a mostrar
 */
void displayLCD_ShowStringLine(const char* str, uint8_t row);

/**
 * @brief Muestra un string en una posición específica del display
 * @param str Cadena de caracteres a mostrar
 * @param column Columna donde se mostrará el string
 * @param row Fila donde se mostrará el string
 */
void displayLCD_ShowStringAt(const char* str, uint8_t column, uint8_t row);

void displayLCD_ShowCharAt(const char c, uint8_t column, uint8_t row);

/**
 * @brief Muestra el cursor en una posición específica del display
 * @param column Columna donde se mostrará el cursor
 * @param row Fila donde se mostrará el cursor
 */
void displayLCD_ShowCursorAt(uint8_t column, uint8_t row);

/**
 * @brief Oculta el cursor
 */
void displayLCD_HideCursor(void);

/**
 * @brief Muestra el cursor en la posición actual
 */
void displayLCD_ShowPosition(void);

/**
 * @brief Oculta el cursor
 */
void displayLCD_HidePosition(void);

/**
 * @brief Limpia el contenido del display
 */
void displayLCD_Clear(void);

/**
 * @brief Enciende la luz de fondo del display
 */
void displayLCD_Backlight(void);

/**
 * @brief Apaga la luz de fondo del display
 */
void displayLCD_BacklightOff(void);

void displayLCD_Update(void);

void displayLCD_BufferLine(const char* str, uint8_t row);

void displayLCD_BufferCursorPosition(uint8_t column, uint8_t row);

void displayLCD_ShowNumberAt(int8_t number, uint8_t column, uint8_t row);

/*******************************************************************************
 ******************************************************************************/

#endif // _DISPLAYLCD_H_
