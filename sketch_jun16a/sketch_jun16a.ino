#include "SD.h"
#define SD_ChipSelectPin 4
#include "SPI.h"
#include <Wire.h>
#include "RTClib.h"

const int sampleWindow = 50;                        // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;

#define SENSOR_PIN A0

RTC_DS1307 rtc;
char daysOfTheWeek[7][12] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"};

void getSuara(){
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
   return db;
   Serial.println(db);
}

void getWaktu(){
  DateTime now = rtc.now();
  String hari = daysOfTheWeek[now.dayOfTheWeek()];
  String tgl = "("+hari+")"+now.year()+"/"+now.month()+"/"+now.day();

  char buffer [25] = "";
    
  sprintf(buffer, "%04d/%02d/%02d, %02d:%02d:%02d", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());
  String dateTime = buffer;
  
  Serial.println(dateTime);
  return dateTime;
}

void setup() {
  pinMode (SENSOR_PIN, INPUT);
  Serial.begin(115200);

  // Cek RTC ready 
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
  }else{
    Serial.println("RTC Ready..");
  }

  // Cek SD Card ready
  if (!SD.begin(SD_ChipSelectPin)) {
    Serial.println("Card failed, or not present");
  }else{
    Serial.println("SD Card Ready..");
  }
}

void loop() {
  getSuara();
  getWaktu();
//
//  String sensor = String(db);
//  String dataString = sensor+","+tgl+","+dateTime;
//  File dataFile = SD.open(hari+".txt", FILE_WRITE);
//
//  if (dataFile) {
//    dataFile.println(dataString);
//    dataFile.close();
//    Serial.println(dataString);
//  }
//  else {
//    Serial.println("error opening datalog.txt");
//  }
}
