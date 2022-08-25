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

const int sampleWindow = 50;                        // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;

#define SENSOR_PIN A0

RTC_DS1307 rtc;
char daysOfTheWeek[7][12] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"};

void setup()
{
  Serial.begin(57600);
  pinMode(SENSOR_PIN, INPUT);
  
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  tmrpcm.speakerPin = 9;
  Serial.println("Datalogger Sensor");

  // Cek RTC ready 
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  Serial.print("Membaca SDcard...");
  if (!SD.begin(CS))
  {
    Serial.println("GAGAL/SDcard rusak!");
    while (1);
  }
  Serial.println("Berhasil");
}

void loop()
{
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

   rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
   DateTime now = rtc.now();
   
   char format[] = "DD/MM/YYYY";
   String tgl= String(now.toString(format));

   
   char format1[] = "hh:mm:ss";
   String waktu = String(now.toString(format1));
   Serial.println(waktu);
  
//    Serial.println(waktu+","+dataSensor+" | Data Tersimpan");
//    
//  dataku = SD.open(waktu+".txt", FILE_WRITE);
//  if (dataku)
//  {
//    dataku.println(waktu+","+dataSensor);
//    dataku.close();
//
//    Serial.println(dateTime+","+dataSensor+" | Data Tersimpan");
//    lcd.setCursor(0, 0);
//    lcd.print("Kebisingan:"+dataSensor);
//    
//  }
//  else
//  {
//    Serial.println(" | Gagal Tersimpan!");
//   
//  }
  delay(3000); 
tmrpcm.play("tenang.wav");
}
