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
  int data = 0b01010101;

  for (int i = 0; i < 8; i++)
  {
    int bit = (data >> i) & 1;

    if (bit == 1)
    {
      digitalWrite(laser, HIGH);
      delay(10);
      digitalWrite(laser, LOW);
      delay(10);
    }
    else
    {
      digitalWrite(laser, LOW);
      delay(1000);
    }
  }
}

void sendDataUsingLaser()
{
}