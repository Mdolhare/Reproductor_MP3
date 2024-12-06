#include "displayLCD.h"
#include <string.h>
#include "LCD_i2c.h"

typedef struct{
    uint8_t column;
    uint8_t row;
}curs_t;

static curs_t cursor = {0, 0};
static char bufferLine1[41] = {0};
static char bufferLine2[41] = {0};
static curs_t bufferCursor = {0, 0};

void displayLCD_Init(void){
    LCD_I2C_Init();
}



void displayLCD_Begin(void){
    LCD_I2C_Begin();
    LCD_I2C_LeftToRight();
    LCD_I2C_Clear();
    LCD_I2C_Backlight();
    LCD_I2C_Home();
    LCD_I2C_NoCursor();
    LCD_I2C_NoBlink();

}

void displayLCD_BufferStringLine1(const char* str) {
    if (strlen(str) < sizeof(bufferLine1)) {
        strncpy(bufferLine1, str, sizeof(bufferLine1) - 1);
        bufferLine1[sizeof(bufferLine1) - 1] = '\0';
    } else {
        // Handle the error case where the string is too long
        // For example, you can truncate the string or log an error
        strncpy(bufferLine1, str, sizeof(bufferLine1) - 1);
        bufferLine1[sizeof(bufferLine1) - 1] = '\0';
    }
}

void displayLCD_BufferStringLine2(const char* str) {
    if (strlen(str) < sizeof(bufferLine2)) {
        strncpy(bufferLine2, str, sizeof(bufferLine2) - 1);
        bufferLine2[sizeof(bufferLine2) - 1] = '\0';
    } else {
        // Handle the error case where the string is too long
        // For example, you can truncate the string or log an error
        strncpy(bufferLine2, str, sizeof(bufferLine2) - 1);
        bufferLine2[sizeof(bufferLine2) - 1] = '\0';
    }
}

void displayLCD_BufferCursorPosition(uint8_t column, uint8_t row) {
    bufferCursor.column = column;
    bufferCursor.row = row;
}

void displayLCD_Update(void) {
    LCD_I2C_SetCursor(0, 0);
    LCD_I2C_WriteString(bufferLine1);
    LCD_I2C_SetCursor(0, 1);
    LCD_I2C_WriteString(bufferLine2);
    displayLCD_ShowCursorAt(bufferCursor.column, bufferCursor.row);
}

void displayLCD_ShowStringLine1(const char* str) {
    LCD_I2C_SetCursor(0, 0);
    LCD_I2C_WriteString(str);
}

void displayLCD_ShowStringLine2(const char* str) {
    LCD_I2C_SetCursor(0, 1);
    LCD_I2C_WriteString(str);
}

void displayLCD_ShowStringAt(const char* str, uint8_t column, uint8_t row) {
    LCD_I2C_SetCursor(column, row);
    LCD_I2C_WriteString(str);
}

void displayLCD_ShowCharAt(const char c, uint8_t column, uint8_t row){
	LCD_I2C_SetCursor(column, row);
	LCD_I2C_WriteChar(c);
}


void displayLCD_ShowCursorAt(uint8_t column, uint8_t row){
    LCD_I2C_SetCursor( cursor.column, cursor.row);
    LCD_I2C_WriteChar(' ');
    cursor.column = column;
    cursor.row = row;  
    LCD_I2C_SetCursor(cursor.column, cursor.row);
    LCD_I2C_WriteChar(CURSOR);
}

void displayLCD_ShowCursor(void) {
    LCD_I2C_Cursor();
}

void displayLCD_HideCursor(void) {
    LCD_I2C_NoCursor();
}

void displayLCD_Clear(void) {
    LCD_I2C_Clear();
}

void displayLCD_Backlight(void) {
    LCD_I2C_Backlight();
}

void displayLCD_BacklightOff(void) {
    LCD_I2C_BacklightOff();
}
