/*******************************************************************************
  @file     display.c
  @brief    Driver para display lcd
  @author   GRUPO 2
 ******************************************************************************/
/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "display_v0.h"
#include "../../../MCAL/I2C/i2c.h"
#include "../../../MCAL/PIT/pit.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
/*
#define HD44780_INIT_SEQ        0x30
#define HD44780_DISP_CLEAR      0x01
#define HD44780_DISP_OFF        0x08
#define HD44780_DISP_ON         0x0C
#define HD44780_CURSOR_ON       0x0E
#define HD44780_CURSOR_OFF      0x0C
#define HD44780_CURSOR_BLINK    0x0F
#define HD44780_RETURN_HOME     0x02
#define HD44780_ENTRY_MODE      0x06
#define HD44780_4BIT_MODE       0x20

*/



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

#define En 0b00000100  // Enable bit
#define Rw 0b00000010  // Read/Write bit
#define Rs 0b00000001  // Register select bit

//Formato del mensaje: 0b D7 D6 D5 D4 Backlight Enable RW RS


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
static void I2C_Write_Pulse_En(uint8_t output) ;
//void delayMicroseconds(uint32_t time);

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
static uint8_t displayFunction;
static uint8_t backlightval;

static i2c_cfg_t cfg;

static bool t_flag = true;

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
	cfg.cant_bytes_tx = 1;

	output.Led = 0;

	pitInit();

}


void LCD_begin(void) {
    i2cInit(I2C_0);

    delayMicroseconds(1000*15);//15ms

    // Clear i2c adapter
    I2C_Write(0b00000000);

    // Wait more than 40 ms after powerOn
    delayMicroseconds(100000);

    backlightval = LCD_NOBACKLIGHT;

    displayFunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
    if(rowMax > 1)
    	displayFunction |= LCD_2LINE;

    Inicialize_LCD();

}


void LCD_backlight(void) {

    output.Led = 1;
    // Led pin is independent from LCD data and control lines.
    I2C_Write(0b00000000 | output.Led << 3);
    delayMicroseconds(370);

}


void LCD_backlightOff(void) {

    output.Led = 0;
    // Led pin is independent from LCD data and control lines.
    I2C_Write(0b00000000 | output.Led << 3);

}


void LCD_clear(void) {

    //output.rs = 0;
    //output.rw = 0;

    LCD_Write(LCD_CLEARDISPLAY, true);
    delayMicroseconds(15500);

}


void LCD_home(void) {

    output.rs = 0;
    output.rw = 0;

    LCD_Write(HD44780_CURSOR_HOME, true);
    delayMicroseconds(15500*2);  //era 1550

}


void LCD_leftToRight(void) {

   // output.rs = 0;
   // output.rw = 0;

    entryState |= (1 << 1);

    LCD_Write(LCD_ENTRYMODESET | LCD_ENTRYLEFT, true);
    delayMicroseconds(370);

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

//    displayState |= (1 << 2);
    displayState |= LCD_DISPLAYON;

    LCD_Write(LCD_DISPLAYCONTROL | displayState, true);
    delayMicroseconds(370); //este era 37

}


void LCD_displayOff(void) {

    output.rs = 0;
    output.rw = 0;

    displayState &= ~(1 << 2);

    LCD_Write(HD44780_DISPLAY_CONTROL | displayState, true);
    delayMicroseconds(370);

}


void LCD_cursor(void) {

    output.rs = 0;
    output.rw = 0;

    displayState |= (1 << 1);

    LCD_Write(HD44780_DISPLAY_CONTROL | displayState, true);
    delayMicroseconds(370); //era 37

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
    delayMicroseconds(370); //Era 37

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
    delayMicroseconds(1000); //este era 37

}


uint8_t write(uint8_t character) {

    output.rs = 1;
    output.rw = 0;

    LCD_Write(character, true);
    delayMicroseconds(10000);//este era 41

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
//0bdddd led E rw rs
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

    // RS y R/W low
    I2C_Write(backlightval);
    delayMicroseconds(5000);


    // first
    //LCD_Write(0b00110000, false);
    //I2C_Write(0b00110000);
    I2C_Write(0b00110000);
    I2C_Write_Pulse_En(0b00110000);
    delayMicroseconds(4500);

    // second
    I2C_Write(0b00110000);
    I2C_Write_Pulse_En(0b00110000);
    delayMicroseconds(4500);

    // third
    I2C_Write(0b00110000);
    I2C_Write_Pulse_En(0b00110000);
    delayMicroseconds(150);

    // Set 4-bit interface
    //0x20
    LCD_Write(0b00100000, false);
    //I2C_Write(0b00100000);

    delayMicroseconds(370);

    // Function Set  - 4 bit Interface, 1 = 2 lines, 0 = 5x8 font
    LCD_Write(0b00101000, true);

    delayMicroseconds(370);

    displayState = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    LCD_display();
    //LCD_displayOff();

    LCD_clear();
    LCD_leftToRight();


}


static void LCD_Write(uint8_t _output, bool initialization) {

    output.data = _output;

    // During initialization we only send half a byte
    if (!initialization) {
        // I think we need a delay between half byte writes, but no sure how long it needs to be.

    	I2C_Write(output.data);
    	delayMicroseconds(1000);

        //output.E = 1;
        I2C_Write(output.data | En);
        // High part of enable should be > 450 ns
        delayMicroseconds(1000);

        //output.E = 0;
        I2C_Write(output.data & ~En);
        delayMicroseconds(1000);
    }
    else{
    	output.E = 1; //era true
		I2C_Write(GetHighData(output));
		//I2C_Write(output.data);

		// High part of enable should be > 450 ns
		delayMicroseconds(1000);

		output.E = 0; //era false
		I2C_Write(GetLowData(output));
		delayMicroseconds(1000);
    }

}

static void I2C_Write_Pulse_En(uint8_t output) {

    I2C_Write(output | En);
    delayMicroseconds(10);

    I2C_Write(output & ~En);
    delayMicroseconds(50);

}

static void I2C_Write(uint8_t output) {

	cfg.data[0] = output | backlightval;
	cfg.cant_bytes_tx = 1;
	i2cInit_master(I2C_0, &cfg, 0, 0x30);

	while(I2CisBusy(I2C_0));

}



void delayMicroseconds(uint32_t time) {
	pitInit();
	pitSetAndBegin(PIT_3, time);
	pitSetIRQFunc(PIT_3, timer);
	//volatile int a = 0;
//	while(a++ < 100000){


	while(true){
		//pitSetIRQFunc(PIT_0, timer);

		if(t_flag==false)
			break;
	}

	t_flag = true;
	pitDisableIRQFunc(PIT_3);
	pitStopTimer(PIT_3);
/*

	uint32_t i = time;
	while(i--);

*/

}

static void timer(void) {
	t_flag = false;
}


