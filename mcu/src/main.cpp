#include <Arduino.h>
int led = 10;

void setup() {
  pinMode(led, OUTPUT);
}

void loop() {
  digitalWrite(led, HIGH);
  delay (1500);
  digitalWrite(led, LOW);
  delay(5000);
  // analogRead in from the temperature 

  // send over I2C to the GPIO pins 

  // LATER: TODO
  // connect the yellow wire (tachometer) to the input of the MCU so we can debug and read RPM in real time 
}



