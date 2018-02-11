#include "Nokia2.h"
#include <math.h>

int incomingByte = 0; //Serial read
static int currentAngle = 0;
static int oldAngle = 0; //Position markers

static LCD lcd; //Nokia 1100 Lib
void setup() {
  lcd.Setup();
  lcd.Clear();
lcd.print("SKY");
//void LCD::drawCircleHelper( int16_t x0, int16_t y0,int16_t r, uint8_t cornername, uint16_t color)
lcd.drawCircle(48,32,30,1);
lcd.drawLine(18, 33, 78, 33, 1);
lcd.drawTriangle(44, 33,48, 33,44, 28, 1); 
lcd.setCursor(0,58);
lcd.print("GND");
lcd.Update();
Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();
    if(true==true){
    if (oldAngle != incomingByte) {
      lcd.drawCircle(48,32,30,1);
      lcd.drawLine(18, 33, 78, 33, 1);

      currentAngle = incomingByte;
      lcd.drawAxisLines(oldAngle, 0);
      lcd.drawAxisLines(currentAngle, 1);
      oldAngle = currentAngle;
      lcd.Update();
    }
    }
  }
}
