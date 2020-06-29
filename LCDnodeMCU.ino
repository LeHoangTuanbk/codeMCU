#include <LiquidCrystal_I2C.h>

#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {

  Serial.begin(115200);
  
  //Use predefined PINS consts
  
  Wire.begin(D2,D1); // chi có 2 chân D2, D1 là hỗ trợ SPI
  lcd.backlight();
  lcd.init();
  
  lcd.home();
  
  lcd.print("Hello, NodeMCU");

}
void loop()
{ // do nothing here 
  }
 // this is a test for git