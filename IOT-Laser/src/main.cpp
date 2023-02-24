#include <HardwareSerial.h>

int laser = 17;
int bouton = 17;

void setup()
{
  Serial.begin(9600);
  pinMode(bouton, INPUT);
  pinMode(laser, OUTPUT);
  Serial.println("Starting");
}

void  start(){

}

void loop()
{
  int data = 0b11010;
  int data2 = 0b11001;

  //Serial.println("Debut");

  //la trame doit durée 200ms
  for (int i = 4; i >= 0; i--) {
    int bit = (data2 >> i) & 1;
    //Serial.println(bit);

    if (bit == 0) {
      digitalWrite(laser, LOW);
      delay(20);
    } else {
      digitalWrite(laser, HIGH);
      delay(20);
    }
    int buttonState = digitalRead(bouton);

   if (buttonState == 0) {
    digitalWrite(laser, HIGH);
    Serial.println(bit);
  } else {
   digitalWrite(laser, LOW);
   }
  }
  //Serial.println("Fin");
    //digitalWrite(laser, LOW);
  //  delay(5000);

}

void sendDataUsingLaser()
{
}