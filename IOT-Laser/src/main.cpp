#include <HardwareSerial.h>

int laser = 17;

void setup()
{
  Serial.begin(9600);
  pinMode(laser, OUTPUT);
  Serial.println("Starting");
}

void loop()
{
  int data = 0b11010;
  int data2 = 0b11001;

  for (int i = 0; i < 10; i++) {
      digitalWrite(laser, HIGH);
      delay(100);
      digitalWrite(laser, LOW);      
      delay(100);
    }

  Serial.println("Debut");

  //la trame doit durée 200ms
  for (int i = 4; i >= 0; i--) {
    int bit = (data2 >> i) & 1;
    Serial.println(bit);

    if (bit == 0) {
      digitalWrite(laser, LOW);
      delay(40);
    } else {
      digitalWrite(laser, HIGH);
      delay(40);
    }
  }
  Serial.println("Fin");
  digitalWrite(laser, LOW);
  delay(5000);
}

void sendDataUsingLaser()
{
}