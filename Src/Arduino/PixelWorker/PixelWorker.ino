#include "Nokia2.h"
#include <math.h>

int incomingByte = 0; //Serial read
static int currentAngle = 0;
static int oldAngle = 0; //Position markers
int CodeIn;
String altit = "", altitold = "";

static LCD lcd; //Nokia 1100 Lib
void setup() {
  lcd.Setup();
  lcd.Rotate(true);
  lcd.Clear();
lcd.print("SKY");
//void LCD::drawCircleHelper( int16_t x0, int16_t y0,int16_t r, uint8_t cornername, uint16_t color)
lcd.drawCircle(48,32,25,1);//Gyro Circle
lcd.drawLine(18, 33, 78, 33, 1);//Horizon Line



lcd.setCursor(0,58);
lcd.print("GND");
lcd.Update();
Serial.begin(9600);
}

/**/
char getChar()
{
  while (Serial.available() == 0); 
  return ((char)Serial.read());
}

void setAltitude() {
  CodeIn = getChar(); // Get the second identifier
  switch (CodeIn) { // Now lets find what to do with it
    case 'a': // found AP altitude setting
      altit = "";
      altit += getChar();
      altit += getChar();
      altit += getChar();
     if (altit != altitold) {
          lcd.drawAxisLines(-10, altitold.toInt(), 0);
          lcd.drawAxisLines(-10, altit.toInt(), 1);

        altitold = altit;
     }
     break;
  }
}
/**/
int startx = 46;
void loop() {
  if (Serial.available()> 0) {
    CodeIn = getChar();
    if (CodeIn == '=') {
      setAltitude(); // The first identifier is "="
      lcd.drawLine(18, 33, 78, 33, 1);//Horizon Line
      //lcd.Update();
    } 
  }

lcd.drawLine(startx, 28, startx+2, 28, 1);//Altitude level 5
lcd.drawLine(startx, 23, startx+2, 23, 1);//Altitude level 10
lcd.drawLine(startx, 18, startx+2, 18, 1);//Altitude level 15
lcd.drawLine(startx, 13, startx+2, 13, 1);//Altitude level 20
lcd.Update();
}
