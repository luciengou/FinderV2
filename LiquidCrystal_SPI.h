//YWROBOT
#ifndef LiquidCrystal_SPI_h
#define LiquidCrystal_SPI_h

#include "LED.h"
#include "SPI.h"

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

#define En 0x04		//B00000100  Enable bit
#define Rw 0x02		//B00000010  Read/Write bit
#define Rs 0x01		//B00000001  Register select bit

  void LiquidCrystal_SPI_write(uint8_t value);
  void LiquidCrystal_SPI_Initial(uint8_t lcd_cols,uint8_t lcd_rows);
  void LiquidCrystal_SPI_clear(void);
  void LiquidCrystal_SPI_home(void);
  void LiquidCrystal_SPI_noDisplay(void);
  void LiquidCrystal_SPI_display(void);
  void LiquidCrystal_SPI_noBlink(void);
  void LiquidCrystal_SPI_blink(void);
  void LiquidCrystal_SPI_noCursor(void);
  void LiquidCrystal_SPI_cursor(void);
  void LiquidCrystal_SPI_scrollDisplayLeft(void);
  void LiquidCrystal_SPI_scrollDisplayRight(void);
  void LiquidCrystal_SPI_printLeft(void);
  void LiquidCrystal_SPI_printRight(void);
  void LiquidCrystal_SPI_leftToRight(void);
  void LiquidCrystal_SPI_rightToLeft(void);
  void LiquidCrystal_SPI_shiftIncrement(void);
  void LiquidCrystal_SPI_shiftDecrement(void);
  void LiquidCrystal_SPI_nobacklight(void);
  void LiquidCrystal_SPI_backlight(void);
  void LiquidCrystal_SPI_autoscroll(void);
  void LiquidCrystal_SPI_noAutoscroll(void); 
  void LiquidCrystal_SPI_createChar(uint8_t, uint8_t[]);
  void LiquidCrystal_SPI_setCursor(uint8_t, uint8_t); 
  
////compatibility API function aliases
void LiquidCrystal_SPI_blink_on(void);						// alias for blink()
void LiquidCrystal_SPI_blink_off(void);       					// alias for noBlink()
void LiquidCrystal_SPI_cursor_on(void);      	 					// alias for cursor()
void LiquidCrystal_SPI_cursor_off(void);      					// alias for noCursor()
void LiquidCrystal_SPI_setBacklight(uint8_t new_val);				// alias for backlight() and nobacklight()
void LiquidCrystal_SPI_load_custom_character(uint8_t char_num, uint8_t *rows);	// alias for createChar()
void LiquidCrystal_SPI_printstr(uint8_t c[]);
void LiquidCrystal_SPI_printnum(uint16_t num);

/*
////Unsupported API functions (not implemented in this library)
uint8_t status();
void setContrast(uint8_t new_val);
uint8_t keypad();
void setDelay(int,int);
void on();
void off();
uint8_t init_bargraph(uint8_t graphtype);
void draw_horizontal_graph(uint8_t row, uint8_t column, uint8_t len,  uint8_t pixel_col_end);
void draw_vertical_graph(uint8_t row, uint8_t column, uint8_t len,  uint8_t pixel_col_end);
*/ 

  static void begin(uint8_t cols, uint8_t rows );
  static void write(uint8_t);
  static void command(uint8_t);
  static void send(uint8_t, uint8_t);
  static void write4bits(uint8_t);
  static void expanderWrite(uint8_t);
  static void pulseEnable(uint8_t);

  /*
  static uint8_t _displayfunction;
  static uint8_t _displaycontrol;
  static uint8_t _displaymode;
  static uint8_t _numlines;
  static uint8_t _cols;
  static uint8_t _rows;
  static uint8_t _backlightval;
  */


#endif
