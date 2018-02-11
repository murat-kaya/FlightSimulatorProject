/*
   PCF8814 - Interface with Philips PCF8814 (or compatible) LCDs.

   Copyright (c) 2014 Catalin Vasiliu <vasiliu.catalin.mihai@gmail.com>

   Originaly made by Carlos Rodrigues <cefrodrigues@gmail.com> for PCD8544(Nokia 3310 LCD)

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.
*/


#ifndef PCF8814_H
#define PCF8814_H


#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif


// Chip variants supported...
#define CHIP_PCF8814 0


#define CHAR_WIDTH 1
#define CHAR_HEIGHT 1

#define PCF8814_CMD  LOW
#define PCF8814_DATA HIGH

#define PI 3.141592653589793
#define RADIUS 30



#define LCD_CONTRAST 0xF3
#define SCREEN_HEIGHT 64
#define SCREEN_WIDTH 96
#define ON 0x01
#define OFF 0x00
#define ALL 0x04
#define INVERT 0x06
#define DISPLAY 0x0E
#define LCD_NOP 0xE3
#define LCD_MODE 0xA0
#define LCD_VOB_MSB 0x20
#define LCD_VOB_LSB 0x80//80
#define LCD_CHARGE_PUMP_ON 0x2F
#define LCD_RAM_ADDR_MODE 0xAA
#define LCD_CHANGE_ROW_LSB 0x00
#define LCD_CHANGE_ROW_MSB 0x10
#define LCD_CHANGE_COL 0xB0
#define LCD_MIRROR_Y 0xC0
#define LCD_MIRROR_X 0xA0
#define LCD_EXT_OSC 0x3A
#define LCD_SOFT_RESET 0xE2

class PCF8814: public Print {
  public:
    // All the pins can be changed from the default values...
    PCF8814(unsigned char sclk  = 13,   /* clock       (display pin 2) */
            unsigned char sdin  = 11,   /* data-in     (display pin 3) */
            unsigned char reset = 12,   /* reset       (display pin 8) */
            unsigned char sce   = 10);  /* enable      (display pin 5) */

    // Display initialization (dimensions in pixels)...
    void begin();

    void stop();
    void refresh();
    void lcd_on();

    //set contrast values 0x00 to 0x1F....default, set by lib 0x05
    void lcd_contrast(byte value);

    //rotate display 180 ... in fact mirror x and mirror y
    void rotate(bool value);

    // Erase everything on the display...
    void clear();
    void clearLine();  // ...or just the current line

    // Control the display's power state...
    void setPower(bool on);

    // For compatibility with the LiquidCrystal library...
    void display();
    void noDisplay();

    // Activate white-on-black mode (whole display)...
    void setInverse(bool inverse);

    // Place the cursor at the start of the current line...
    void home();

    // Place the cursor at position (column, line)...
    void setCursor(unsigned char column, unsigned char line);

    // Assign a user-defined glyph (5x8) to an ASCII character (0-31)...
    void createChar(unsigned char chr, const unsigned char *glyph);

    // Write an ASCII character at the current cursor position (7-bit)...
#if ARDUINO < 100
    virtual void write(uint8_t chr);
#else
    virtual size_t write(uint8_t chr);
#endif

    // Draw a bitmap at the current cursor position...
    void drawBitmap(const unsigned char *data, unsigned char columns, unsigned char lines);

    // Draw a chart element at the current cursor position...
    void drawColumn(unsigned char lines, unsigned char value);
    void Lcd_Write(unsigned char cd, unsigned char c);
    void Gotoxy(char x, char y);
    void lcd_pixel(unsigned char x, unsigned char y, unsigned char color);
    void lcd_line(int x1, int y1, int x2, int y2, int color) ;
    void lcd_circle(int x, int y, int radius, int fill, int color);
    void drawAxisLines(int angle, int clearBit);
    void lcd_text(int x, int y, char* textptr, int size, int color);
    int bit_test (unsigned int  word, int  n);


  private:
    unsigned char pin_sclk;
    unsigned char pin_sdin;
    unsigned char pin_reset;
    unsigned char pin_sce;

    // The size of the display, in pixels...
    unsigned char width;
    unsigned char height;

    // Current cursor position...
    unsigned char column;
    unsigned char line;

    // User-defined glyphs (below the ASCII space character)...
    const unsigned char *custom[' '];

    // Send a command or data to the display...
    void send(unsigned char type, unsigned char data);

};


#endif  /* PCF8814_H */


/* vim: set expandtab ts=4 sw=4: */
