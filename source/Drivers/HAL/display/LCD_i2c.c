#include "LCD_i2c.h"
#include "../../MCAL/I2C/i2c.h"
#include "../../MCAL/PIT/pit.h"

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

// Buffer LCD
#define LCD_BUFFER_SIZE 80

#define En 0b00000100  // Enable bit
#define Rw 0b00000010  // Read/Write bit
#define Rs 0b00000001  // Register select bit

//Formato del mensaje: 0b D7 D6 D5 D4 Backlight Enable RW RS

static uint8_t _displayfunction;
static uint8_t _displaycontrol;
static uint8_t _displaymode;
static uint8_t _numlines;
static uint8_t _backlightval;
static bool t_flag = true;
static uint8_t _cols;
static uint8_t _rows;
static uint8_t _address;

static void send(uint8_t value, uint8_t mode);
static void write4bits(uint8_t value);
static void expanderWrite(uint8_t data);
static void pulseEnable(uint8_t data);
static void i2cWrite(uint8_t data);
static void delayMicroseconds(uint32_t delay);
static void timer(void);
static i2c_cfg_t cfg;



void LCD_I2C_Init(uint8_t address, uint8_t rows) {
	pitInit();
    i2cInit(I2C_0);
    _address = address;
    _rows = rows;
    _cols = LCD_BUFFER_SIZE/rows;

}



void LCD_I2C_Begin(void) {
    _displayfunction = LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS;
    _backlightval = LCD_BACKLIGHT;

    // Esperar a que el display se inicie
    delayMicroseconds(50000);

    // Inicializar el display
    expanderWrite(_backlightval);
    delayMicroseconds(1000);

    write4bits(0x03 << 4);
    delayMicroseconds(4500);

    write4bits(0x03 << 4);
    delayMicroseconds(4500);

    write4bits(0x03 << 4);
    delayMicroseconds(1500);

    write4bits(0x02 << 4);

    // Configurar el display
    send(LCD_FUNCTIONSET | _displayfunction, 0);

    _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    send(LCD_DISPLAYCONTROL | _displaycontrol, 0);

    LCD_I2C_Clear();

    _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    send(LCD_ENTRYMODESET | _displaymode, 0);
}

void LCD_I2C_Backlight(void) {
    _backlightval = LCD_BACKLIGHT;
    expanderWrite(0);
}

void LCD_I2C_BacklightOff(void) {
    _backlightval = LCD_NOBACKLIGHT;
    expanderWrite(0);
}

void LCD_I2C_Clear(void) {
    send(LCD_CLEARDISPLAY, 0);
    delayMicroseconds(2000);
}

void LCD_I2C_Home(void) {
    send(LCD_RETURNHOME, 0);
    delayMicroseconds(2000);
}

void LCD_I2C_SetCursor(uint8_t column, uint8_t row) {
    if(row < _rows && column < _cols){
        uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
        send(LCD_SETDDRAMADDR | (column + row_offsets[row]), 0);
        delayMicroseconds(2000);
    }
}

void LCD_I2C_WriteChar(uint8_t character) {
    send(character, Rs);
}

void LCD_I2C_WriteString(const char* str) {
    while (*str) {
        LCD_I2C_WriteChar(*str++);
    }
}

void LCD_I2C_NoDisplay(void) {
    _displaycontrol &= ~LCD_DISPLAYON;
    send(LCD_DISPLAYCONTROL | _displaycontrol, 0);
}

void LCD_I2C_Display(void) {
    _displaycontrol |= LCD_DISPLAYON;
    send(LCD_DISPLAYCONTROL | _displaycontrol, 0);
}

void LCD_I2C_NoBlink(void) {
    _displaycontrol &= ~LCD_BLINKON;
    send(LCD_DISPLAYCONTROL | _displaycontrol, 0);
}

void LCD_I2C_Blink(void) {
    _displaycontrol |= LCD_BLINKON;
    send(LCD_DISPLAYCONTROL | _displaycontrol, 0);
}

void LCD_I2C_NoCursor(void) {
    _displaycontrol &= ~LCD_CURSORON;
    send(LCD_DISPLAYCONTROL | _displaycontrol, 0);
}

void LCD_I2C_Cursor(void) {
    _displaycontrol |= LCD_CURSORON;
    send(LCD_DISPLAYCONTROL | _displaycontrol, 0);
}

void LCD_I2C_ScrollDisplayLeft(void) {
    send(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT, 0);
}

void LCD_I2C_ScrollDisplayRight(void) {
    send(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT, 0);
}

void LCD_I2C_LeftToRight(void) {
    _displaymode |= LCD_ENTRYLEFT;
    send(LCD_ENTRYMODESET | _displaymode, 0);
}

void LCD_I2C_RightToLeft(void) {
    _displaymode &= ~LCD_ENTRYLEFT;
    send(LCD_ENTRYMODESET | _displaymode, 0);
}

void LCD_I2C_Autoscroll(void) {
    _displaymode |= LCD_ENTRYSHIFTINCREMENT;
    send(LCD_ENTRYMODESET | _displaymode, 0);
}

void LCD_I2C_NoAutoscroll(void) {
    _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
    send(LCD_ENTRYMODESET | _displaymode, 0);
}

void LCD_I2C_CreateChar(uint8_t location, uint8_t charmap[]) {
    location &= 0x7; // we only have 8 locations 0-7
    send(LCD_SETCGRAMADDR | (location << 3), 0);
    for (int i = 0; i < 8; i++) {
        LCD_I2C_WriteChar(charmap[i]);
    }
}

static void send(uint8_t value, uint8_t mode) {
    uint8_t highnib = value & 0xf0;
    uint8_t lownib = (value << 4) & 0xf0;
    write4bits((highnib) | mode);
    write4bits((lownib) | mode);
}

static void write4bits(uint8_t value) {
    expanderWrite(value);
    pulseEnable(value);
}

static void expanderWrite(uint8_t data) {
    i2cWrite(data | _backlightval);
	delayMicroseconds(10);//era 500

}

static void pulseEnable(uint8_t data) {
    expanderWrite(data | En);
    delayMicroseconds(10);//era 1000
    expanderWrite(data & ~En);
    delayMicroseconds(50); // era 5000
}

static void i2cWrite(uint8_t data) {
    cfg.mode = TX;
    cfg.cant_bytes_tx = 1;
    cfg.data[0] = data;
    cfg.address = _address;
    i2cInit_master(I2C_0, &cfg, 0, 0x10);
    while (I2CisBusy(I2C_0));
}

static void delayMicroseconds(uint32_t delay) {
	pitSetAndBegin(PIT_3, delay);
	pitSetIRQFunc(PIT_3, timer);
	while(true){
		if(t_flag==false)
			break;
	}
	t_flag = true;
	pitDisableIRQFunc(PIT_3);
	pitStopTimer(PIT_3);
}

static void timer(void) {
	t_flag = false;
}

