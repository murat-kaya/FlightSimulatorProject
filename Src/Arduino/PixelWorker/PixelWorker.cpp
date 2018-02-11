#include "PCF8814.h"
#include "charset.cpp"


float lineX, lineX1;
float lineY, lineY1;
int XX, YY;

static unsigned char memory[96][8]; //LCD memory data


// Purpose:    Write a byte of data to lcd
// Inputs:     c - the byte of data to write
void PCF8814::Lcd_Write(unsigned char cd, unsigned char c) {
  int i;
  digitalWrite(this->pin_sce, LOW);
  digitalWrite(this->pin_sclk, LOW);
  digitalWrite(this->pin_sdin, cd);
  digitalWrite(this->pin_sclk, HIGH);

  if (cd == PCF8814_DATA)
  {
    memory[XX][YY] = c; //write data to memory
    XX++;
  }

  for (i = 0; i < 8; i++) {
    digitalWrite(this->pin_sclk, LOW);
    if ((c & 0x80))
      digitalWrite(this->pin_sdin, HIGH);
    else
      digitalWrite(this->pin_sdin, LOW);
    digitalWrite(this->pin_sclk, HIGH);
    c <<= 1;
    //delay(10);
  }
  digitalWrite(this->pin_sce, HIGH);


}

void PCF8814::Gotoxy(char x, char y) {
  Lcd_Write(PCF8814_CMD, (0x00 | (x & 0x0F)));  // X axis initialisation: 0000 xxxx ( x3 x2 x1 x0)
  Lcd_Write(PCF8814_CMD, (0x10 | ((x >> 4) & 0x07))); // X axis initialisation: 0010 0xxx  ( x6 x5 x4)
  Lcd_Write(PCF8814_CMD, (0xB0 | (y & 0x0F)));  // Y axis initialisation: 0100 yyyy
 
}




// Purpose:    Turn a pixel on a graphic LCD on or off
// Inputs:     1) x - the x coordinate of the pixel
//             2) y - the y coordinate of the pixel
//             3) color - ON or OFF
void PCF8814::lcd_pixel(unsigned char x, unsigned char y, unsigned char color) {
  char aux[8];
  char i, o;
  char c = 1;
  char index;
  char temp;

  XX = x;
  YY = y / 8;

  this->Gotoxy(x, YY);
  
  temp = memory[x][YY];

  if (color == ON)
    bitSet(temp, y % 8);      // Turn the pixel on
  else                          // or
    bitClear(temp, y % 8);    // turn the pixel off

  this->Lcd_Write(PCF8814_DATA, temp);
}

/////////////////////////////////////////////////////////////////////////
// Purpose:       Draw a line on a nokia 1100 LCD
//                line drawing algorithm
// Inputs:        (x1, y1) - the start coordinate
//                (x2, y2) - the end coordinate
//                color - ON or OFF
// Dependencies:  glcd_pixel()
/////////////////////////////////////////////////////////////////////////
void PCF8814::lcd_line(int x1, int y1, int x2, int y2, int color)
{
  int dy, dx;
  int addx = 1, addy = 1;
  int P, diff;

  int i = 0;
  dx = abs((int)(x2 - x1));
  dy = abs((int)(y2 - y1));


  if (x1 > x2)
    addx = -1;
  if (y1 > y2)
    addy = -1;

  if (dx >= dy)
  {
    dy *= 2;
    P = dy - dx;
    diff = P - dx;

    for (; i <= dx; ++i)
    {
      this->lcd_pixel(x1, y1, color);

      if (P < 0)
      {
        P  += dy;
        x1 += addx;
      }
      else
      {
        P  += diff;
        x1 += addx;
        y1 += addy;
      }
    }
  }
  else
  {
    dx *= 2;
    P = dx - dy;
    diff = P - dy;

    for (; i <= dy; ++i)
    {
      this->lcd_pixel(x1, y1, color);

      if (P < 0)
      {
        P  += dx;
        y1 += addy;
      }
      else
      {
        P  += diff;
        x1 += addx;
        y1 += addy;
      }
    }
  }
}


/////////////////////////////////////////////////////////////////////////
// Purpose:       Draw a circle on a nokia 1100 LCD
// Inputs:        (x,y) - the center of the circle
//                radius - the radius of the circle
//                fill - YES or NO
//                color - ON or OFF
/////////////////////////////////////////////////////////////////////////
void PCF8814::lcd_circle(int x, int y, int radius, int fill, int color)
{
  signed int  a, b, P;

  a = 0;
  b = radius;
  P = 1 - radius;

  do
  {
    if (fill)
    {
      this->lcd_line(x - a, y + b, x + a, y + b, color);
      this->lcd_line(x - a, y - b, x + a, y - b, color);
      this->lcd_line(x - b, y + a, x + b, y + a, color);
      this->lcd_line(x - b, y - a, x + b, y - a, color);
    }
    else
    {
      this->lcd_pixel(a + x, b + y, color);
      this->lcd_pixel(b + x, a + y, color);
      this->lcd_pixel(x - a, b + y, color);
      this->lcd_pixel(x - b, a + y, color);
      this->lcd_pixel(b + x, y - a, color);
      this->lcd_pixel(a + x, y - b, color);
      this->lcd_pixel(x - a, y - b, color);
      this->lcd_pixel(x - b, y - a, color);
    }

    if (P < 0)
      P += 3 + 2 * a++;
    else
      P += 5 + 2 * (a++ - b--);
  } while (a <= b);
}

void PCF8814::drawBitmap(const unsigned char *data, unsigned char columns, unsigned char lines)
{
  unsigned char scolumn = this->column;
  unsigned char sline = this->line;

  // The bitmap will be clipped at the right/bottom edge of the display...
  unsigned char mx = (scolumn + columns > this->width) ? (this->width - scolumn) : columns;
  unsigned char my = (sline + lines > this->height / 8) ? (this->height / 8 - sline) : lines;

  for (unsigned char y = 0; y < my; y++) {
    this->setCursor(scolumn, sline + y);

    for (unsigned char x = 0; x < mx; x++) {
      this->send(PCF8814_DATA, data[y * columns + x]);
    }
  }

  // Leave the cursor in a consistent position...
  this->setCursor(scolumn + columns, sline);
}

void PCF8814::drawColumn(unsigned char lines, unsigned char value)
{
  unsigned char scolumn = this->column;
  unsigned char sline = this->line;

  // Keep "value" within range...
  if (value > lines * 8) {
    value = lines * 8;
  }

  // Find the line where "value" resides...
  unsigned char mark = (lines * 8 - 1 - value) / 8;

  // Clear the lines above the mark...
  for (unsigned char line = 0; line < mark; line++) {
    this->setCursor(scolumn, sline + line);
    this->send(PCF8814_DATA, 0x00);
  }

  // Compute the byte to draw at the "mark" line...
  unsigned char b = 0xff;
  for (unsigned char i = 0; i < lines * 8 - mark * 8 - value; i++) {
    b <<= 1;
  }

  this->setCursor(scolumn, sline + mark);
  this->send(PCF8814_DATA, b);

  // Fill the lines below the mark...
  for (unsigned char line = mark + 1; line < lines; line++) {
    this->setCursor(scolumn, sline + line);
    this->send(PCF8814_DATA, 0xff);
  }

  // Leave the cursor in a consistent position...
  this->setCursor(scolumn + 1, sline);
}

void PCF8814::drawAxisLines(int angle, int clearBit) {

  
  lineX = round(SCREEN_WIDTH / 2 + (RADIUS * cos(-angle * PI / 180) ));
  lineY = round(SCREEN_HEIGHT / 2 + (RADIUS * sin(-angle * PI / 180)));
  lineX1 = round(SCREEN_WIDTH / 2 + (RADIUS * cos(-(180 + angle) * PI / 180)) );
  lineY1 = round(SCREEN_HEIGHT / 2 + (RADIUS * sin(-(180 + angle) * PI / 180)));


  this->lcd_line(48, 32, lineX, lineY, clearBit);
  this->lcd_line(48, 32, lineX1, lineY1, clearBit);

}

///////////////////////////////////////////////////////////////////////// 
// Purpose:       Write text on a nokia 1100 LCD 
// Inputs:        (x,y) - The upper left coordinate of the first letter 
//                textptr - A pointer to an array of text to display 
//                size - The size of the text: 1 = 5x7, 2 = 10x14, ... 
//                color - ON or OFF 
///////////////////////////////////////////////////////////////////////// 
void PCF8814::lcd_text(int x, int y, char* textptr, int size, int color) 
{ 
   int j, k, l, m;                       // Loop counters 
   int pixelData[5];                     // Stores character data 

   for(; *textptr != '\0'; ++textptr, ++x)// Loop through the passed string 
   { 
      if(*textptr < 'S') // Checks if the letter is in the first font array 
         memcpy(pixelData, FONT[*textptr - ' '], 5); 
      else if(*textptr <= '~') // Check if the letter is in the second font array 
         memcpy(pixelData, FONT2[*textptr - 'S'], 5); 
      else 
         memcpy(pixelData, FONT[0], 5);   // Default to space 

      // Handles newline and carriage returns 
      switch(*textptr) 
      { 
         case '\n': 
            y += 7*size + 1; 
            continue; 
         case '\r': 
            x = 0; 
            continue; 
      } 

      if(x+5*size >= SCREEN_WIDTH)          // Performs character wrapping 
      { 
         x = 0;                           // Set x at far left position 
         y += 7*size + 1;                 // Set y at next position down 
      } 
      for(j=0; j<5; ++j, x+=size)         // Loop through character byte data 
      { 
         for(k=0; k < 7; ++k)             // Loop through the vertical pixels 
         { 
            if(bit_test(pixelData[j], k)) // Check if the pixel should be set 
            { 
               for(l=0; l < size; ++l)    // These two loops change the 
               {                          // character's size 
                  for(m=0; m < size; ++m) 
                  { 
                     lcd_pixel(x+m, y+k*size+l, color); // Draws the pixel 
                  } 
               } 
            } 
         } 
      } 
   } 
} 

int  PCF8814::bit_test (unsigned int k, int  n)  
{  
    if ( n < 0  || n > 7 )  
       return  0;  

    if ( (k >> (7 - n)) & 0x1 )  
       return 1;  
    else  
       return 0;  
}
