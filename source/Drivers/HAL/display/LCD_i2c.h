/***************************************************************************//**
  @file     disp.h
  @brief    Control de display LCD por medio de interfaz I2C
 ******************************************************************************/

#ifndef _LCD_I2CH_
#define _LCD_I2CH_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define LCD_BUFFER_SIZE 80


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Inicializa el display LCD por medio de I2C
 */
void LCD_I2C_Init(uint8_t address, uint8_t rows);


/**
 * @brief Inicializa el display LCD por medio de I2C
 */
void LCD_I2C_Begin(void);

/**
 * @brief Enciende la luz de fondo del display
 */
void LCD_I2C_Backlight(void);

/**
 * @brief Apaga la luz de fondo del display
 */
void LCD_I2C_BacklightOff(void);

/**
 * @brief Limpia el contenido del display
 */
void LCD_I2C_Clear(void);

/**
 * @brief Mueve el cursor a la posición inicial
 */
void LCD_I2C_Home(void);

/**
 * @brief Establece la posición del cursor
 * @param column Columna del cursor
 * @param row Fila del cursor
 */
void LCD_I2C_SetCursor(uint8_t column, uint8_t row);

/**
 * @brief Escribe un carácter en la posición actual del cursor
 * @param character Carácter a escribir
 */
void LCD_I2C_WriteChar(uint8_t character);

/**
 * @brief Escribe una cadena de caracteres en la posición actual del cursor
 * @param str Cadena de caracteres a escribir
 */
void LCD_I2C_WriteString(const char* str);

/**
 * @brief Apaga la visualización del display
 */
void LCD_I2C_NoDisplay(void);

/**
 * @brief Enciende la visualización del display
 */
void LCD_I2C_Display(void);

/**
 * @brief Apaga el parpadeo del cursor
 */
void LCD_I2C_NoBlink(void);

/**
 * @brief Enciende el parpadeo del cursor
 */
void LCD_I2C_Blink(void);

/**
 * @brief Apaga el cursor
 */
void LCD_I2C_NoCursor(void);

/**
 * @brief Enciende el cursor
 */
void LCD_I2C_Cursor(void);

/**
 * @brief Desplaza el contenido del display a la izquierda
 */
void LCD_I2C_ScrollDisplayLeft(void);

/**
 * @brief Desplaza el contenido del display a la derecha
 */
void LCD_I2C_ScrollDisplayRight(void);

/**
 * @brief Establece el modo de escritura de izquierda a derecha
 */
void LCD_I2C_LeftToRight(void);

/**
 * @brief Establece el modo de escritura de derecha a izquierda
 */
void LCD_I2C_RightToLeft(void);

/**
 * @brief Activa el desplazamiento automático
 */
void LCD_I2C_Autoscroll(void);

/**
 * @brief Desactiva el desplazamiento automático
 */
void LCD_I2C_NoAutoscroll(void);

void LCD_I2C_CreateChar(uint8_t location, uint8_t charmap[]);

/*******************************************************************************
 ******************************************************************************/

#endif // _LCD_I2CH_