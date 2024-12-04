/***************************************************************************//**
  @file     display.h
  @brief    Driver display
  @author   Grupo 2
 ******************************************************************************/

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define HD44780_CLEAR_DISPLAY 0b00000001
#define HD44780_CURSOR_HOME 0b00000010
#define HD44780_ENTRY_MODE_SET 0b00000100

#define HD44780_DISPLAY_CONTROL 0b00001000

#define HD44780_CURSOR_OR_DISPLAY_SHIFT 0b00010000
#define HD44780_FUNCTION_SET 0b00100000
#define HD44780_SET_CGRAM_ADDR 0b01000000
#define HD44780_SET_DDRRAM_ADDR 0b10000000

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef struct {

    uint8_t rs;
    uint8_t rw;
    uint8_t E;
    uint8_t Led;
    uint8_t data;

} outputState_t;

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

void LCD_Init(uint8_t _address, uint8_t _columns, uint8_t _rows);

void LCD_begin();
void LCD_backlight();
void LCD_backlightOff();

void LCD_clear();
void LCD_home();
void LCD_leftToRight();
void LCD_rightToLeft();
void LCD_autoscroll();
void LCD_autoscrollOff();
void LCD_display();
void LCD_displayOff();
void LCD_cursor();
void LCD_cursorOff();
void LCD_blink();
void LCD_blinkOff();
void LCD_scrollDisplayLeft();
void LCD_scrollDisplayRight();
void LCD_createChar(uint8_t memory_location, uint8_t* charmap);
void LCD_setCursor(uint8_t column, uint8_t row);
uint8_t write(uint8_t character);

uint8_t GetLowData(outputState_t output);
uint8_t GetHighData(outputState_t output);

/*******************************************************************************
 ******************************************************************************/

#endif // _DISPLAY_H_
