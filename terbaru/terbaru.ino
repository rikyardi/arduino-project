#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <TMRpcm.h>

#include "RTClib.h"

LiquidCrystal_I2C lcd(0x27,16,2);
RTC_DS1307 rtc;

TMRpcm tmrpcm;

const int sampleWindow = 50;                        // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;

#define SENSOR_PIN A0

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup () {
  Serial.begin(9600);
  pinMode(SENSOR_PIN, INPUT);
  
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  tmrpcm.speakerPin = 9;
  
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
       
    lcd.setCursor(0,0);
    lcd.print("RTC Error");
    while (1) delay(10);
  }
 if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));


  Serial.print("Membaca SDcard...");
  if (!SD.begin(4))
  {
    Serial.println("GAGAL/SDcard rusak!");
    lcd.setCursor(0,0);
    lcd.print("SD Card Error");
    while (1);
  }
  Serial.println("Berhasil");
}

void loop () {
  unsigned long startMillis= millis();                   // Start of sample window
  float peakToPeak = 0;                                  // peak-to-peak level
 
  unsigned int signalMax = 0;                            //minimum value
  unsigned int signalMin = 1024;                         //maximum value

  while (millis() - startMillis < sampleWindow){
    sample = analogRead(SENSOR_PIN);                    //get reading from microphone
    if (sample < 1024){                                  // toss out spurious readings
         if (sample > signalMax){
          signalMax = sample;                           // save just the max levels
         }
         else if (sample < signalMin){
          signalMin = sample;                           // save just the min levels
         }
      }
   }
 
   peakToPeak = signalMax - signalMin;                    // max - min = peak-peak amplitude
   int db = map(peakToPeak,20,900,40,90);             //calibrate for deciBels
   String dataSensor = String(db);

    DateTime now = rtc.now();

    char format[] = "DDMMYYYY";
    String tgl= String(now.toString(format));

    char format1[] = "hh:mm:ss";
    String waktu = String(now.toString(format1));
    Serial.println(waktu);
    
    Serial.println(waktu+","+dataSensor+" | Data Tersimpan");
    String filename = tgl+".txt";
    Serial.println(filename);
    
    File dataFile = SD.open(filename, FILE_WRITE);
    
    // if the file is available, write to it:
    if (dataFile) {
      dataFile.println(waktu+","+dataSensor);
      dataFile.close();
      // print to the serial port too:
      Serial.println(waktu+","+dataSensor);
      lcd.setCursor(0,0);
      lcd.print("Kebisingan: " + dataSensor + "dB");
      lcd.setCursor(0,1);
      lcd.print("Sukses Disimpan");
    }
    // if the file isn't open, pop up an error:
    else {
      Serial.println("error datalog file");
      lcd.setCursor(0,1);
      lcd.print("Gagal Disimpan");
    }
    
//    dataku = SD.open(tgl+".txt", FILE_WRITE);
//    if (dataku){
//      dataku.println(waktu+","+dataSensor);
//      dataku.close();
//
//      Serial.println(waktu+","+dataSensor+" | Data Tersimpan");
//      lcd.setCursor(0, 0);
//      lcd.print("Kebisingan:"+dataSensor);
//    }else{
//      Serial.println(" | Gagal Tersimpan!");
//    }
 
        tmrpcm.play("tenang.wav");    
    
  delay(1000);
}
