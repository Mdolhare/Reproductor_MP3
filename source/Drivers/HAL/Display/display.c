

/*******************************************************************************
  @file     display.c
  @brief    Driver para display lcd
  @author   GRUPO 2
 ******************************************************************************/
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "display.h"
#include "../i2c.h"
#include "../pit.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

static void Inicialize_LCD(void);
static void I2C_Write(uint8_t output);
static void LCD_Write(uint8_t output, bool inicialization);

static void delayMicroseconds(uint32_t time);

static void timer(void);

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/


/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static uint8_t address;
static uint8_t columnMax;
static uint8_t rowMax;
static outputState_t output;
static uint8_t displayState;
static uint8_t entryState;

static i2c_cfg_t cfg;

static bool t_flag;

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void LCD_Init(uint8_t _address, uint8_t _columns, uint8_t _rows) {

	address = _address;
	columnMax = _columns;
	rowMax = _rows;
	displayState = 0;
	entryState = 0;

	cfg.address = _address;
	cfg.mode = TX;
	cfg.cant_bytes_tx = 2;

	pitInit();

}


void LCD_begin(void) {
    i2cInit(I2C_0);

    // Clear i2c adapter
    I2C_Write(0b00000000);

    // Wait more than 40 ms after powerOn

    delayMicroseconds(50000);
    Inicialize_LCD();

}


void LCD_backlight(void) {

    output.Led = 1;
    // Led pin is independent from LCD data and control lines.
    I2C_Write(0b00000000 | output.Led << 3);

}


void LCD_backlightOff(void) {

    output.Led = 0;
    // Led pin is independent from LCD data and control lines.
    I2C_Write(0b00000000 | output.Led << 3);

}


void LCD_clear(void) {

    output.rs = 0;
    output.rw = 0;

    LCD_Write(HD44780_CLEAR_DISPLAY, true);
    delayMicroseconds(1550);

}


void LCD_home(void) {

    output.rs = 0;
    output.rw = 0;

    LCD_Write(HD44780_CURSOR_HOME, true);
    delayMicroseconds(1550);

}


void LCD_leftToRight(void) {

    output.rs = 0;
    output.rw = 0;

    entryState |= (1 << 1);

    LCD_Write(HD44780_ENTRY_MODE_SET | entryState, true);
    delayMicroseconds(37);

}


void LCD_rightToLeft(void) {

    output.rs = 0;
    output.rw = 0;

    entryState &= ~(1 << 1);

    LCD_Write(HD44780_ENTRY_MODE_SET | entryState, true);
    delayMicroseconds(37);

}


void LCD_autoscroll(void) {

    output.rs = 0;
    output.rw = 0;

    entryState |= 1;

    LCD_Write(HD44780_ENTRY_MODE_SET | entryState, true);
    delayMicroseconds(37);

}


void LCD_autoscrollOff(void) {

    output.rs = 0;
    output.rw = 0;

    entryState &= ~1;

    LCD_Write(HD44780_ENTRY_MODE_SET | entryState, true);
    delayMicroseconds(37);

}


void LCD_display(void) {

    output.rs = 0;
    output.rw = 0;

    displayState |= (1 << 2);

    LCD_Write(HD44780_DISPLAY_CONTROL | displayState, true);
    delayMicroseconds(37);

}


void LCD_displayOff(void) {

    output.rs = 0;
    output.rw = 0;

    displayState &= ~(1 << 2);

    LCD_Write(HD44780_DISPLAY_CONTROL | displayState, true);
    delayMicroseconds(37);

}


void LCD_cursor(void) {

    output.rs = 0;
    output.rw = 0;

    displayState |= (1 << 1);

    LCD_Write(HD44780_DISPLAY_CONTROL | displayState, true);
    delayMicroseconds(37);

}


void LCD_cursorOff(void) {

    output.rs = 0;
    output.rw = 0;

    displayState &= ~(1 << 1);

    LCD_Write(HD44780_DISPLAY_CONTROL | displayState, true);
    delayMicroseconds(37);

}


void LCD_blink(void) {

    output.rs = 0;
    output.rw = 0;

    displayState |= 1;

    LCD_Write(HD44780_DISPLAY_CONTROL | displayState, true);
    delayMicroseconds(37);

}


void LCD_blinkOff(void) {

    output.rs = 0;
    output.rw = 0;

    displayState &= ~1;

    LCD_Write(HD44780_DISPLAY_CONTROL | displayState, true);
    delayMicroseconds(37);

}


void LCD_scrollDisplayLeft(void) {

    output.rs = 0;
    output.rw = 0;

    LCD_Write(0b00011000, true);
    delayMicroseconds(37);

}


void LCD_scrollDisplayRight(void) {

    output.rs = 0;
    output.rw = 0;

    LCD_Write(0b00011100, true);
    delayMicroseconds(37);

}


void LCD_createChar(uint8_t memory_location, uint8_t* charmap) {

    output.rs = 0;
    output.rw = 0;

    memory_location %= 8;

    LCD_Write(HD44780_SET_CGRAM_ADDR | (memory_location << 3), true);
    delayMicroseconds(37);

    for (int i = 0; i < 8; i++)
        write(charmap[i]);

    LCD_setCursor(0, 0); // Set the address pointer back to the DDRAM

}


void LCD_setCursor(uint8_t column, uint8_t row) {

    static const uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
    output.rs = 0;
    output.rw = 0;
     // sanity limits
    if (column > columnMax) { column = columnMax; }
    // sanity limits
    if (row > rowMax) { row = rowMax; }

    uint8_t newAddress = row_offsets[row] + column;

    LCD_Write(HD44780_SET_DDRRAM_ADDR | newAddress, true);
    delayMicroseconds(37);

}


uint8_t write(uint8_t character) {

    output.rs = 1;
    output.rw = 0;

    LCD_Write(character, true);
    delayMicroseconds(41);

    return 1;

}


uint8_t GetLowData(outputState_t output) {

	uint8_t buffer = output.rs;
	buffer |= output.rw << 1;
	buffer |= output.E << 2;
	buffer |= output.Led << 3;
	buffer |= (output.data & 0x0F) << 4;

	return buffer;

}

uint8_t GetHighData(outputState_t output) {

	uint8_t buffer = output.rs;
	buffer |= output.rw << 1;
	buffer |= output.E << 2;
	buffer |= output.Led << 3;
	buffer |= (output.data & 0xF0);

	return buffer;

}



/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static void Inicialize_LCD(void) {

    // See HD44780U datasheet "Initializing by Instruction" Figure 24 (4-Bit Interface)
    // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
	// according to datasheet, we need at least 40 ms after power rises above 2.7 V
	// before sending commands. Arduino can turn on way before 4.5 V so we'll wait 50
    output.rs = 0;
    output.rw = 0;

    // wait more than 40 ms after Vcc = 2.7 V
    delayMicroseconds(50000);

    // first
    LCD_Write(0b00110000, true);
    delayMicroseconds(4200);
    // second
    LCD_Write(0b00110000, true);
    delayMicroseconds(150);
    // third
    LCD_Write(0b00110000, true);
    delayMicroseconds(37);

    // Function Set - 4 bits mode
    LCD_Write(0b00100000, true);
    delayMicroseconds(37);
    // Function Set  - 4 bit Interface, 1 = 2 lines, 0 = 5x8 font
    LCD_Write(0b00101000, true);
    delayMicroseconds(37);

    LCD_displayOff();
    LCD_clear();
    LCD_leftToRight();

}


static void I2C_Write(uint8_t output) {

	cfg.data[0] = output;
	cfg.data[1] = 0xAA;

	i2cInit_master(I2C_0, &cfg, 0, 0x30);

}


static void LCD_Write(uint8_t _output, bool initialization) {

    output.data = _output;

    output.E = true;
    I2C_Write(GetHighData(output));
    // High part of enable should be > 450 ns
    delayMicroseconds(1);

    output.E = false;
    I2C_Write(GetHighData(output));

    // During initialization we only send half a byte
    if (!initialization) {
        // I think we need a delay between half byte writes, but no sure how long it needs to be.
        delayMicroseconds(37);

        output.E = true;
        I2C_Write(GetLowData(output));
        // High part of enable should be > 450 ns
        delayMicroseconds(1);

        output.E = false;
        I2C_Write(GetLowData(output));
    }

}

void delayMicroseconds(uint32_t time) {
	pitSetAndBegin(PIT_3, time);
	pitSetIRQFunc(PIT_3, timer);
	while(!timer);
	t_flag = false;
	pitDisableIRQFunc(PIT_3);
	pitDisable(PIT_3);
}

static void timer(void) {
	t_flag = true;
}


