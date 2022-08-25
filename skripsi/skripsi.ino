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

void setup() {
  pinMode(SENSOR_PIN, INPUT);
  Serial.begin(115200);
  Serial.print("Initializing SD card...");
  
  // Cek RTC ready 
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }
  
  // Cek SD Card ready
  if (!SD.begin(SD_ChipSelectPin)) {
    Serial.println("Card failed, or not present");
    while (1);
  }
  Serial.println("card initialized.");
}

void loop() {
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
   Serial.println(db);
  
//    DateTime now = rtc.now();
//    String hari = daysOfTheWeek[now.dayOfTheWeek()];
//    String tgl = "("+hari+")"+now.year()+"/"+now.month()+"/"+now.day();
//
//    char buffer [25] = "";
//    
//    sprintf(buffer, "%04d/%02d/%02d, %02d:%02d:%02d", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());
//    String dateTime = buffer;
//   
//    // make a string for assembling the data to log:
//    String sensor = String(db);
//    String dataString = sensor+","+tgl+","+dateTime;
//
//  // read three sensors and append to the string:
////  for (int i = 0; i < 3; i++) {
////    int sensor = analogRead(analogPin);
////    dataString += String(sensor);
////    if (i < 2) {
////      dataString += ",";
////    }
////  }
//  
//  // open the file. note that only one file can be open at a time,
//  // so you have to close this one before opening another.
//  File dataFile = SD.open(hari+".txt", FILE_WRITE);
//  
//  // if the file is available, write to it:
//  if (dataFile) {
//    dataFile.println(dataString);
//    dataFile.close();
//    // print to the serial port too:
//    Serial.println(dataString);
//  }
//  // if the file isn't open, pop up an error:
//  else {
//    Serial.println("error opening datalog.txt");
//  }
}
