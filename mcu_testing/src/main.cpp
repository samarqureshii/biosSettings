#include <Arduino.h>

const int pwmPin = 3; 
const int tachPin = 5;

int PWMcontrol(int temperature);

void setup() {
  pinMode(pwmPin, OUTPUT);
  pinMode(tachPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  for (int temp = 22; temp < 100; temp++) { //22 deg C to 100deg C

    analogWrite(pwmPin, PWMcontrol(temp));

    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.print("C, PWM Duty Cycle: ");
    Serial.println(PWMcontrol(temp));
    Serial.println("/n/n");

    delay(2000);
  }
}

int PWMcontrol(int temperature) {
  // analogWrite expects a value from 0 to 255 for 8-bit PWM resolution
  int dutyCycle = map(temperature, 22, 100, 0, 255);

  return dutyCycle;
}
