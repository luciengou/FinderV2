#include "LiquidCrystal_SPI.h"
#include "Timer.h"
#include "SPI.h"

#define printSPI(args)	SPI_Write_Byte(args)

  static uint8_t _displayfunction;
  static uint8_t _displaycontrol;
  static uint8_t _displaymode;
  static uint8_t _numlines;
  static uint8_t _cols;
  static uint8_t _rows;
  static uint8_t _backlightval;

inline void LiquidCrystal_SPI_write(uint8_t value) {
	send(value, Rs);
}

// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set:
//    DL = 1; 8-bit interface data
//    N = 0; 1-line display
//    F = 0; 5x8 dot character font
// 3. Display on/off control:
//    D = 0; Display off
//    C = 0; Cursor off
//    B = 0; Blinking off
// 4. Entry mode set:
//    I/D = 1; Increment by 1
//    S = 0; No shift
//
// Note, however, that resetting the Arduino doesn't reset the LCD, so we
// can't assume that its in that state when a sketch starts (and the
// LiquidCrystal constructor is called).

void LiquidCrystal_SPI_Initial(uint8_t lcd_cols,uint8_t lcd_rows){
	_cols = lcd_cols;
 	_rows = lcd_rows;
	_backlightval = LCD_NOBACKLIGHT;

	_displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
	begin(_cols, _rows);
}

static void begin(uint8_t cols, uint8_t lines) {
	_displayfunction |= LCD_2LINE;

	_numlines = lines;

	// SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
	// according to datasheet, we need at least 40ms after power rises above 2.7V
	// before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
	//delay(50);
	delay_ms(50);

	// Now we pull both RS and R/W low to begin commands
	expanderWrite(_backlightval);	// reset expanderand turn backlight off (Bit 8 =1)
	//delay(1000);
	delay_ms(1000);

  	//put the LCD into 4 bit mode
	// this is according to the hitachi HD44780 datasheet
	// figure 24, pg 46

	  // we start in 8bit mode, try to set 4 bit mode
	write4bits(0x03 << 4);
	//delayMicroseconds(4500); // wait min 4.1ms
	delay_us(2000);
	delay_us(2100); // wait min 4.1ms

   	// second try
	write4bits(0x03 << 4);
	//delayMicroseconds(4500); // wait min 4.1ms
	delay_us(2000);
	delay_us(2100); // wait min 4.1ms

	// third go!
	write4bits(0x03 << 4);
	//delayMicroseconds(150);
	delay_us(150);

	// finally, set to 4-bit interface
	write4bits(0x02 << 4);

	// set # lines, font size, etc.
	command(LCD_FUNCTIONSET | _displayfunction);

	// turn the display on with no cursor or blinking default
	_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
	LiquidCrystal_SPI_display();

	// clear it off
	LiquidCrystal_SPI_clear();

	// Initialize to default text direction (for roman languages)
	_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

	// set the entry mode
	command(LCD_ENTRYMODESET | _displaymode);

	LiquidCrystal_SPI_home();

}

/********** high level commands, for the user! */
void LiquidCrystal_SPI_clear(){
	command(LCD_CLEARDISPLAY);// clear display, set cursor position to zero
	//delayMicroseconds(2000);  // this command takes a long time!
	delay_us(2000);
}

void LiquidCrystal_SPI_home(){
	command(LCD_RETURNHOME);  // set cursor position to zero
	//delayMicroseconds(2000);  // this command takes a long time!
	delay_us(2000);
}

void LiquidCrystal_SPI_setCursor(uint8_t col, uint8_t row){
	int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
	if ( row > _numlines ) {
		row = _numlines-1;    // we count rows starting w/0
	}
	command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

// Turn the display on/off (quickly)
void LiquidCrystal_SPI_noDisplay() {
	_displaycontrol &= ~LCD_DISPLAYON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystal_SPI_display() {
	_displaycontrol |= LCD_DISPLAYON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void LiquidCrystal_SPI_noCursor() {
	_displaycontrol &= ~LCD_CURSORON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystal_SPI_cursor() {
	_displaycontrol |= LCD_CURSORON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void LiquidCrystal_SPI_noBlink() {
	_displaycontrol &= ~LCD_BLINKON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void LiquidCrystal_SPI_blink() {
	_displaycontrol |= LCD_BLINKON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void LiquidCrystal_SPI_scrollDisplayLeft(void) {
	command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void LiquidCrystal_SPI_scrollDisplayRight(void) {
	command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void LiquidCrystal_SPI_leftToRight(void) {
	_displaymode |= LCD_ENTRYLEFT;
	command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void LiquidCrystal_SPI_rightToLeft(void) {
	_displaymode &= ~LCD_ENTRYLEFT;
	command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void LiquidCrystal_SPI_autoscroll(void) {
	_displaymode |= LCD_ENTRYSHIFTINCREMENT;
	command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void LiquidCrystal_SPI_noAutoscroll(void) {
	_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
	command(LCD_ENTRYMODESET | _displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void LiquidCrystal_SPI_createChar(uint8_t location, uint8_t charmap[]) {
	location &= 0x7; // we only have 8 locations 0-7
	command(LCD_SETCGRAMADDR | (location << 3));
	for (int i=0; i<8; i++) {
		LiquidCrystal_SPI_write(charmap[i]);
	}
}

// Turn the (optional) backlight off/on
void LiquidCrystal_SPI_nobacklight(void) {
	_backlightval=LCD_NOBACKLIGHT;
	expanderWrite(0);
}

void LiquidCrystal_SPI_backlight(void) {
	_backlightval=LCD_BACKLIGHT;
	expanderWrite(0);
}



/*********** mid level commands, for sending data/cmds */

static inline void command(uint8_t value) {
	send(value, 0);
}


/************ low level data pushing commands **********/

// write either command or data
static void send(uint8_t value, uint8_t mode) {
	uint8_t highnib=value&0xf0;
	uint8_t lownib=(value<<4)&0xf0;

	write4bits((highnib)|mode);
	write4bits((lownib)|mode);
}

static void write4bits(uint8_t value) {
	expanderWrite(value);
	pulseEnable(value);
}

static void expanderWrite(uint8_t _data){

	printSPI((int)(_data) | _backlightval);
	SPI_nCS=1;
	SPI_nCS=0;

}

static void pulseEnable(uint8_t _data){
	expanderWrite(_data | En);	// En high
	delay_us(1);

	expanderWrite(_data & ~En);	// En low
	delay_us(50);
}


// Alias functions

void LiquidCrystal_SPI_cursor_on(){
	LiquidCrystal_SPI_cursor();
}

void LiquidCrystal_SPI_cursor_off(){
	LiquidCrystal_SPI_noCursor();
}

void LiquidCrystal_SPI_blink_on(){
	LiquidCrystal_SPI_blink();
}

void LiquidCrystal_SPI_blink_off(){
	LiquidCrystal_SPI_noBlink();
}

void LiquidCrystal_SPI_load_custom_character(uint8_t char_num, uint8_t *rows){
		LiquidCrystal_SPI_createChar(char_num, rows);
}

void LiquidCrystal_SPI_setBacklight(uint8_t new_val){
	if(new_val){
		LiquidCrystal_SPI_backlight();		// turn backlight on
	}else{
		LiquidCrystal_SPI_nobacklight();		// turn backlight off
	}
}

void LiquidCrystal_SPI_printstr(uint8_t c[]){
	//This function is not identical to the function used for "real" SPI displays
	//it's here so the user sketch doesn't have to be changed
	//print(c);

	uint8_t *ch = c;

	while (*ch)
	{
		LiquidCrystal_SPI_write((uint8_t) *ch);
		ch++;                               //  Grab the next character.
	}
}

void LiquidCrystal_SPI_printnum(uint16_t num)
{
	uint8_t x=0,j=0,temp_char[5];

	while(num>0){
		temp_char[x]=num % 10+0x30;
		num /=10;
		x++;
	}

	temp_char[x]=0;

	uint8_t	temp;

	x--;

	while(j<x)
	{
		temp=temp_char[j];
		temp_char[j]=temp_char[x];
		temp_char[x]=temp;
		j++;
		x--;
	}

	LiquidCrystal_SPI_printstr(temp_char);
}

/*
// unsupported API functions
void LiquidCrystal_SPI_off(){}
void LiquidCrystal_SPI_on(){}
void LiquidCrystal_SPI_setDelay (int cmdDelay,int charDelay) {}
uint8_t LiquidCrystal_SPI_status(){return 0;}
uint8_t LiquidCrystal_SPI_keypad (){return 0;}
uint8_t LiquidCrystal_SPI_init_bargraph(uint8_t graphtype){return 0;}
void LiquidCrystal_SPI_draw_horizontal_graph(uint8_t row, uint8_t column, uint8_t len,  uint8_t pixel_col_end){}
void LiquidCrystal_SPI_draw_vertical_graph(uint8_t row, uint8_t column, uint8_t len,  uint8_t pixel_row_end){}
void LiquidCrystal_SPI_setContrast(uint8_t new_val){}
*/
