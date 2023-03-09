#include <Arduino.h>

int pin_Laser = 17;
int buttonPin = 17;

void setup(){
    Serial.begin(9600);
    pinMode(buttonPin, INPUT);
    pinMode(pin_Laser, OUTPUT);
}

void loop(){


    digitalWrite(pin_Laser, HIGH);
    delay(20);
    digitalWrite(pin_Laser, HIGH);
    delay(20);
    digitalWrite(pin_Laser, LOW);
    delay(20);
    digitalWrite(pin_Laser, LOW);
    delay(20);
    digitalWrite(pin_Laser, HIGH);
    Serial.println("Fin");



}
