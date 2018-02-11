#include "PCF8814.h"
#include "charset.cpp"
#include <avr/pgmspace.h>

#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif


/*----------------------------------------------------------------------------------------------*/

const unsigned char *glyph;
unsigned char pgm_buffer[5];

#define lcd_gotoyx(ROW,COL)\
  lcd_row(ROW);\
  lcd_col(COL);


PCF8814::PCF8814(unsigned char sclk, unsigned char sdin,
                 unsigned char reset, unsigned char sce):
  pin_sclk(sclk),
  pin_sdin(sdin),
  pin_reset(reset),
  pin_sce(sce)
{}


void PCF8814::begin()
{
  this->width = width;
  this->height = height;

  this->column = 0;
  this->line = 0;

  pinMode(this->pin_sclk, OUTPUT);
  pinMode(this->pin_sdin, OUTPUT);
  pinMode(this->pin_reset, OUTPUT);
  pinMode(this->pin_sce, OUTPUT);

  digitalWrite(this->pin_reset, LOW);
  digitalWrite(this->pin_sce, HIGH);
  digitalWrite(this->pin_sclk, LOW);
  this->refresh();
  delay(200);
  this->clear();
}

void PCF8814::stop()
{
  this->clear();
  this->send(PCF8814_CMD, LCD_MODE | DISPLAY | OFF); //
  this->send(PCF8814_CMD, LCD_MODE | ALL | ON);    //
}

void PCF8814::refresh()
{
  digitalWrite(this->pin_reset, LOW);
  digitalWrite(this->pin_reset, HIGH);
  this->send(PCF8814_CMD, LCD_CHARGE_PUMP_ON);   //LCD_CHARGE_PUMP_ON == 0x2F
  this->lcd_contrast(LCD_CONTRAST);   // LCD_CONTRAST == 0x05
  this->lcd_on();
}

void PCF8814::lcd_on()
{
  this->send(PCF8814_CMD, LCD_MODE | DISPLAY | ON);
  this->send(PCF8814_CMD, LCD_MODE | ALL | OFF);
  this->send(PCF8814_CMD, LCD_MODE | INVERT | OFF);

}

void PCF8814::lcd_contrast(byte value)
{
  this->send(PCF8814_CMD, LCD_VOB_MSB | 0x04);
  this->send(PCF8814_CMD, LCD_VOB_LSB | (value & 0x1F));
}

void PCF8814::rotate(bool value)
{
  if (value == true) {
    this->send(PCF8814_CMD, LCD_MIRROR_Y | 0x08);
    this->send(PCF8814_CMD, LCD_MIRROR_X | 1);
  }
  else if (value == false) {
    this->send(PCF8814_CMD, LCD_MIRROR_Y | OFF);
    this->send(PCF8814_CMD, LCD_MIRROR_X | OFF);
  }
}

void PCF8814::clear()
{
  this->setCursor(0, 0);
  int index;
  for (index = 0; index < 864; index++)
  {
    this->send(PCF8814_DATA, 0x00);
  }

}


void PCF8814::clearLine()
{
  this->setCursor(0, this->line);

  for (unsigned char i = 0; i < this->width; i++) {
    this->send(PCF8814_DATA, 0x00);
  }

  this->setCursor(0, this->line);
}


void PCF8814::setPower(bool on)
{
  if (on == false) {
    this->send(PCF8814_CMD, LCD_MODE | DISPLAY | OFF ); //
    this->send(PCF8814_CMD, LCD_MODE | ALL | ON);
  }
  else {
    this->send(PCF8814_CMD, LCD_MODE | DISPLAY | ON ); //
    this->send(PCF8814_CMD, LCD_MODE | ALL | OFF);
    this->send(PCF8814_CMD, LCD_MODE | INVERT | ON);

  }
}


inline void PCF8814::display()
{
  this->setPower(true);
}


inline void PCF8814::noDisplay()
{
  this->setPower(false);
}


void PCF8814::setInverse(bool inverse)
{
  this->send(PCF8814_CMD, LCD_MODE | DISPLAY | ON);
  this->send(PCF8814_CMD, LCD_MODE | ALL | OFF);
  this->send(PCF8814_CMD, LCD_MODE | INVERT | ON);
}


void PCF8814::home()
{
  this->setCursor(0, this->line);
}

void PCF8814::setCursor(unsigned char column, unsigned char line)
{
  this->column = (column % this->width);
  this->line = (line % (this->height / 9 + 1));

  this->send(PCF8814_CMD, LCD_CHANGE_ROW_LSB | ( this->column & 0x0F));
  this->send(PCF8814_CMD, LCD_CHANGE_ROW_MSB | ( (this->column >> 4) & 0x07 ));
  this->send(PCF8814_CMD, LCD_CHANGE_COL | ( this->line & 0x0F ));
}


void PCF8814::createChar(unsigned char chr, const unsigned char *glyph)
{
  // ASCII 0-31 only...
  if (chr >= ' ') {
    return;
  }
  this->custom[chr] = glyph;
}


#if ARDUINO < 100
void PCF8814::write(uint8_t chr)
#else
size_t PCF8814::write(uint8_t chr)
#endif
{
  // ASCII 7-bit only...
  if (chr >= 0x80) {
#if ARDUINO < 100
    return;
#else
    return 0;
#endif
  }
  if (chr >= ' ') {
    // Regular ASCII characters are kept in flash to save RAM...
    memcpy_P(pgm_buffer, &charset[chr - ' '], sizeof(pgm_buffer));
    glyph = pgm_buffer;
  } else {
    // Custom glyphs, on the other hand, are stored in RAM...
    if (this->custom[chr]) {
      glyph = this->custom[chr];
    } else {
      // Default to a space character if unset...
      memcpy_P(pgm_buffer, &charset[0], sizeof(pgm_buffer));
      glyph = pgm_buffer;
    }
  }


  // Output one column at a time...
  for (unsigned char i = 0; i < 5; i++) {
    this->send(PCF8814_DATA, glyph[i]);
  }

  // One column between characters...
  this->send(PCF8814_DATA, 0x00);

  // Update the cursor position...
  this->column = (this->column + 6) % this->width;

  if (this->column == 0) {
    this->line = (this->line + 1) % (this->height / 9 + 1);
  }

#if ARDUINO >= 100
  return 1;
#endif
}

void PCF8814::send(unsigned char type, unsigned char data)
{
  digitalWrite(this->pin_sdin, type);
  digitalWrite(this->pin_sce, LOW);
  digitalWrite(this->pin_sclk, HIGH);
  digitalWrite(this->pin_sclk, LOW);
  shiftOut(this->pin_sdin, this->pin_sclk, MSBFIRST, data);
  digitalWrite(this->pin_sce, HIGH);
}


/* vim: set expandtab ts=4 sw=4: */
