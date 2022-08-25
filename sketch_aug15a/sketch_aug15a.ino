#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>
#include <TMRpcm.h>

LiquidCrystal_I2C lcd(0x27,16,2);
const int CS = 4;
File dataku;
TMRpcm tmrpcm;
RTC_DS1307 rtc;

#define SENSOR_PIN A0
  
void setup()
{
  Serial.begin(9600);
  pinMode(SENSOR_PIN, INPUT);
  
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  tmrpcm.speakerPin = 9;
  Serial.println(F("Datalogger Sensor"));

  // Cek RTC ready 
  if (! rtc.begin()) {
    Serial.println(F("Couldn't find RTC"));
    lcd.setCursor(0, 0);
    lcd.print("RTC Error!");
  }

  Serial.print(F("Membaca SDcard..."));
  if (!SD.begin(CS))
  {
    Serial.println(F("GAGAL/SDcard rusak!"));

    lcd.setCursor(0, 0);
    lcd.print("SD Card Error!");
  }
  Serial.println(F("Berhasil"));
}

void loop()
{
  char daysOfTheWeek[7][12] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"};
  DateTime now = rtc.now();
  String today = daysOfTheWeek[now.dayOfTheWeek()]+now.day()+now.month()+now.year();

  const int sampleWindow = 50;                        // Sample window width in mS (50 mS = 20Hz)
  unsigned int sample;

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
   int db = map(peakToPeak,20,1000,40,90);             //calibrate for deciBels
   String dataSensor = String(db);

//  dataku = SD.open("sensor.txt", FILE_WRITE);
//  if (dataku)
//  {
//    dataku.println(dataRTC()+","+dataSensor());
//    dataku.close();

    Serial.println(today+","+dataSensor+" | Data Tersimpan");
    lcd.setCursor(0, 0);
    lcd.print("Kebisingan:"+dataSensor);
        tmrpcm.play("pulang.wav");
    
//  }
//  else
//  {
//    Serial.println(F("Gagal Tersimpan!"));
    lcd.setCursor(0, 1);
    lcd.print("Gagal Disimpan");
//  }
  delay(1000);
}
