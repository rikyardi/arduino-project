#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x3F, 16, 2);//atau 0x3F

void setup()
{
  Serial.begin(9600);
  Serial.println("cek");
    // initialize the LCD
    lcd.init();
    
    // Turn on the blacklight and print a message.
    lcd.backlight();
    lcd.print("Hello, world!");
}

void loop()
{
    // Do nothing here...
}
