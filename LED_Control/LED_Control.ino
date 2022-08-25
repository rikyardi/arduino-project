int LEDpin = 4;

void setup() {
  // put your setup code here, to run once:
  pinMode(LEDpin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LEDpin, LOW);
  Serial.println("LED Off");
  delay(1000);
  digitalWrite(LEDpin, HIGH);
  Serial.println("LED On");
  delay(5000);
  
} 
