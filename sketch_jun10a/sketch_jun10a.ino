#define pinSpeakerA     9
#define pinSpeakerB     10
 
#define pinCS           8
#define faktorKali      2
 
#include <SD.h>
#include <SPI.h>
 
 
bool suaraDimainkan;
uint32_t sampleCounter;
byte ulangPerSampel;
byte ulang;
 
struct  HeaderWAV
{
  char                RIFF[4];
  unsigned long       ChunkSize;
  char                WAVE[4];
  char                fmt[4];
  unsigned long       Subchunk1Size;
  unsigned short      AudioFormat;
  unsigned short      NumOfChan;
  unsigned long       SamplesPerSec;
  unsigned long       bytesPerSec;
  unsigned short      blockAlign;
  unsigned short      bitsPerSample;
  char                Subchunk2ID[4];
  unsigned long       Subchunk2Size;
 
};
 
HeaderWAV headerWAV;
File fileSuara;
 
 
void setup(void)
{
  pinMode(pinSpeakerA, OUTPUT);
  pinMode(pinSpeakerB, OUTPUT);
 
  Serial.begin(9600);
  Serial.println("Memutar file suara .wav pada kartu memory SDCard dengan arduino");
  Serial.println("https://www.semesin.com/project/");
 
  if (!SD.begin(pinCS))
  {
    Serial.println("SD fail");
    return;
  }
}
 
void loop(void)
{
  if (!suaraDimainkan)
  {
    mainkanSuara("pulang.wav");
  }
  else
  {
    lanjutkanSuara();
  }
}
 
void mainkanSuara(char *namaFile)
{
  fileSuara = SD.open(namaFile);
  if ( !fileSuara )
  {
    Serial.println("File suara tidak ditemukan");
    return 0;
  }
 
  byte *alamat = (byte*)&headerWAV;
  for (byte i = 0; i < sizeof(headerWAV); i++)
  {
    byte data = fileSuara.read();
    *alamat++ = data;
  }
  Serial.print("namaFile=");
  Serial.println(namaFile);
  Serial.print("headerWAV.SamplesPerSec=");
  Serial.println(headerWAV.SamplesPerSec);
  Serial.print("headerWAV.NumOfChan=");
  Serial.println(headerWAV.NumOfChan);
  Serial.print("headerWAV.bitsPerSample=");
  Serial.println(headerWAV.bitsPerSample);
  Serial.print("headerWAV.Subchunk2Size=");
  Serial.println(headerWAV.Subchunk2Size);
 
  ulangPerSampel = 32000L / headerWAV.SamplesPerSec;
  ICR1 = 256;
  TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM11);
  TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS10);
 
  sampleCounter = 0;
  ulang = 0;
  suaraDimainkan = true;
}
 
void lanjutkanSuara()
{
  if (TIFR1 & _BV(TOV1))
  {
    TIFR1 |= _BV(TOV1);
    if (!(ulang++ % ulangPerSampel))
    {
      if (sampleCounter++ >= headerWAV.Subchunk2Size)
      {
        Serial.println("Selesai");
        stopPlayback();
      }
      else
      {
        byte data = fileSuara.read();
 
        uint16_t sample = data;
        OCR1B = 256 - sample;
        OCR1A = sample;
      }
    }
  }
}
 
 
void stopPlayback()
{
  TIMSK1 &= ~_BV(OCIE1A);
  TCCR1B &= ~_BV(CS10);
  OCR1A = 128;
  OCR1B = 128;
 
  fileSuara.close();
  digitalWrite(pinSpeakerA, LOW);
  digitalWrite(pinSpeakerB, LOW);
 
  suaraDimainkan = false;
}
