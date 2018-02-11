#include "PCF8814.h"
#include <math.h>

static PCF8814 lcd; //Nokia 1100 Lib
int incomingByte = 0; //Serial read
int currentAngle, oldAngle; //Position markers

void setup() {
  lcd.begin();
  //lcd.rotate(true);
  Serial.begin(9600);

  //Initial drawings
  //Circle - x,y,radius,fill,black/white
  lcd.lcd_circle(48, 32, 30, 0, 1);

  //Line - x1,y1,x2,y2,black/white
  lcd.lcd_line(18, 33, 78, 33, 1);

  //Cursor position - x,7
  lcd.setCursor(0,0);
  //Print some strings
  lcd.print("UP");
    //Cursor position - x,7
  lcd.setCursor(0,8);
  lcd.print("DOWN");
  
}

void loop() {

  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();
    if (currentAngle != incomingByte) {
      lcd.lcd_circle(48, 32, 30, 0, 1);
      lcd.lcd_line(18, 33, 78, 33, 1);

      currentAngle = incomingByte;
      lcd.drawAxisLines(oldAngle, 0);
      lcd.drawAxisLines(currentAngle, 1);
      oldAngle = currentAngle;
    }
  }
}

