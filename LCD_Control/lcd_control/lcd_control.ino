#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // Kalau gagal ganti 0x3F ke 0x27 for a 16 chars and 2 line display

byte customChar[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000,

};

void setup()
{
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
}
void loop()
{
  for(int i=0; i<16; i++){
    lcd.clear();   
    if(i%2 == 0){
      lcd.setCursor(i, 0);
      lcd.write((byte)0);
    }else{
      lcd.setCursor(i, 1);
      lcd.write((byte)0);
    }
    delay(1000);
  }
}
