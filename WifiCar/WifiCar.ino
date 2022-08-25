/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on NodeMCU.

  Note: This requires ESP8266 support package:
    https://github.com/esp8266/Arduino

  Please be sure to select the right NodeMCU module
  in the Tools -> Board menu!

  For advanced settings please follow ESP examples :
   - ESP8266_Standalone_Manual_IP.ino
   - ESP8266_Standalone_SmartConfig.ino
   - ESP8266_Standalone_SSL.ino

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

/* Fill-in your Template ID (only if using Blynk.Cloud) */
//#define BLYNK_TEMPLATE_ID   "YourTemplateID"


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YiKEq6aQT3A0rt-xsJCXaMazeXqpgOnz";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Amran";
char pass[] = "rumahamran";

#define ENA D0
#define IN1 D1
#define IN2 D2
#define IN3 D3
#define IN4 D4
#define ENB D5

bool maju = 0;
bool mundur = 0;
bool kiri = 0;
bool kanan = 0;

int speed;

void setup()
{
  // Debug console
  Serial.begin(9600);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  Blynk.begin(auth, ssid, pass);
}

BLYNK_WRITE(V0){
  maju = param.asInt();
}

BLYNK_WRITE(V1){
  mundur = param.asInt();
}

BLYNK_WRITE(V2){
  kiri = param.asInt();
}

BLYNK_WRITE(V3){
  kanan = param.asInt();
}

BLYNK_WRITE(V4){
  speed = param.asInt();
}

void smartcar(){
  if(maju == 1){
    mbl_maju();
    Serial.println("Mobil Maju");
  }else if(mundur == 1){
    mbl_mundur();
    Serial.println("Mobil Mundur");
  }else if(kiri == 1){
    belok_kiri();
    Serial.println("Mobil Belok Kiri");
  }else if(kanan == 1){
    belok_kanan();
    Serial.println("Mobil Belok Kanan");
  }else if(maju == 0 && mundur == 0 && kiri == 0 && kanan == 0){
    mbl_berhenti();
    Serial.println("Mobil Berhenti");  
  }
}

void mbl_maju(){
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void mbl_mundur(){
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void belok_kiri(){
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void belok_kanan(){
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void mbl_berhenti(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void loop()
{
  Blynk.run();
  smartcar();
}
