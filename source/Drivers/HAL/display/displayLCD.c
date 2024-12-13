#include "displayLCD.h"
#include <string.h>
#include "LCD_i2c.h"

#define BLANK	"                    "

typedef struct{
    uint8_t column;
    uint8_t row;
}curs_t;

static curs_t cursor = {0, 0};
static char bufferLines[ROWS][LCD_BUFFER_SIZE/ROWS] = {0};

static curs_t bufferCursor = {0, 0};

void displayLCD_Init(uint8_t address){
    LCD_I2C_Init(address, ROWS);
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

void displayLCD_createChar(uint8_t location, uint8_t charmap[]){
    LCD_I2C_CreateChar(location, charmap);
}

void displayLCD_BufferStringLine(const char* str, uint8_t row) {

    if (row < ROWS) {
        if (strlen(str) < sizeof(bufferLines[row])) {
            strncpy(bufferLines[row], str, sizeof(bufferLines[row]) - 1);
            bufferLines[row][sizeof(bufferLines[row]) - 1] = '\0';
        } else {
            // Handle the error case where the string is too long
            // For example, you can truncate the string or log an error
            strncpy(bufferLines[row], str, sizeof(bufferLines[row]) - 1);
            bufferLines[row][sizeof(bufferLines[row]) - 1] = '\0';
        }
    }
}

void displayLCD_BufferCursorPosition(uint8_t column, uint8_t row) {
    bufferCursor.column = column;
    bufferCursor.row = row;
}

void displayLCD_Update(void) {
    uint8_t i;
    for(i = 0; i < ROWS; i++){
        LCD_I2C_SetCursor(0, i);
        LCD_I2C_WriteString(bufferLines[i]);
    }
    displayLCD_ShowCursorAt(bufferCursor.column, bufferCursor.row);
}

void displayLCD_ShowStringLine(const char* str, uint8_t row){
    if(row < ROWS){
    	LCD_I2C_SetCursor(0, row);
    	//LCD_I2C_WriteString(BLANK);
    	LCD_I2C_WriteString(str);
	}
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

void displayLCD_HideCursor(void){
    LCD_I2C_SetCursor(cursor.column, cursor.row);
    LCD_I2C_WriteChar(' ');
}


void displayLCD_ShowPosition(void) {
    LCD_I2C_Cursor();
}

void displayLCD_HidePosition(void) {
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


void displayLCD_ShowNumberAt(int8_t number, uint8_t column, uint8_t row){
    char str[4] = {0}; // 3 characters + null terminator
    str[0] = (number >= 0) ? '+' : '-';
    number = (number >= 0) ? number : -number;
    str[1] = (number / 10) + '0';
    str[2] = (number % 10) + '0';
    //str[3] = '\0';
    displayLCD_ShowStringAt(str, column, row);

}
