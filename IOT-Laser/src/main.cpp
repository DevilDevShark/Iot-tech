#include <HardwareSerial.h>

int laserPin = 17;
int buttonPin = 17;

void setup()
{
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  pinMode(laserPin, OUTPUT);
  Serial.println("Starting");
}

void start()
{
}

void loop()
{
  int data = 0b11010;
  int data2 = 0b11001;

  // la trame doit durée 200ms
  for (int i = 4; i >= 0; i--)
  {
    int bit = (data2 >> i) & 1;

    if (bit == 0)
    {
      digitalWrite(laserPin, LOW);
      delay(20);
    }
    else
    {
      digitalWrite(laserPin, HIGH);
      delay(20);
    }
    int buttonState = digitalRead(buttonPin);

    if (buttonState == 0)
    {
      digitalWrite(laserPin, HIGH);
      Serial.println(bit);
    }
    else
    {
      digitalWrite(laserPin, LOW);
    }
  }
}