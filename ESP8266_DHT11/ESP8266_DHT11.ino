
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "39yAoiyS0STfybC3XLS9054wWe2B5Yag";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Amran";
char pass[] = "rumahamran";

#define DHTPIN 2 
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;
 
void sendSensor()
{
  float kelembaban = dht.readHumidity();
  float suhu = dht.readTemperature();
  Serial.println(kelembaban);
    Serial.println(suhu);
  if (isnan(suhu) || isnan(kelembaban)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  Blynk.virtualWrite(V5, kelembaban);
  Blynk.virtualWrite(V6, suhu);
  
}
void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
  dht.begin();
  timer.setInterval(1000L, sendSensor);

}

void loop()
{
  Blynk.run();
  timer.run();
}
