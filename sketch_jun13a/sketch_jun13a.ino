#include <Wire.h>

const int sampleWindow = 50;                        // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;

#define SENSOR_PIN A0


void setup() {
  // put your setup code here, to run once:
  pinMode (SENSOR_PIN, INPUT); // Set the signal pin as input  
  Serial.begin(115200);
}

void loop() {
   unsigned long startMillis= millis();                   // Start of sample window
   float peakToPeak = 0;                                  // peak-to-peak level
 
   unsigned int signalMax = 0;                            //minimum value
   unsigned int signalMin = 1024;                         //maximum value

    while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(SENSOR_PIN);                    //get reading from microphone
      if (sample < 1024)                                  // toss out spurious readings
      {
         if (sample > signalMax)
         {
            signalMax = sample;                           // save just the max levels
         }
         else if (sample < signalMin)
         {
            signalMin = sample;                           // save just the min levels
         }
      }
   }
 
   peakToPeak = signalMax - signalMin;                    // max - min = peak-peak amplitude
   int db = map(peakToPeak,20,900,40,90);             //calibrate for deciBels
   Serial.println(db);
   
}
