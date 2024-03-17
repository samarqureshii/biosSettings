#include <Arduino.h>

const int pwmPin = 3; 
const int tachPin = 5;

int PWMcontrol(int temperature);
int dutyCycle = 0;

void setup() {
  pinMode(pwmPin, OUTPUT);
  pinMode(tachPin, INPUT);
  Serial.begin(9600);
  
}

void loop() {

    analogWrite(pwmPin, dutyCycle);

    // Serial.print("Temperature: ");
    // Serial.print(temp);
    Serial.print("PWM Duty Cycle: ");
    Serial.println(dutyCycle);
    Serial.println("\n\n");

    delay(100);
    dutyCycle++;
    if(dutyCycle>=255){ // reset the duty cycle
      dutyCycle = 0;
    }

    int durationHigh = pulseIn(tachPin, HIGH);
    int durationCycle = durationHigh + pulseIn(tachPin, LOW);
    int speed = (durationHigh / (float)durationCycle) * 100;
    Serial.println("Speed (RPM): ");
    Serial.println(speed*62.88);
}

int PWMcontrol(int temperature) {
  // analogWrite expects a value from 0 to 255 for 8-bit PWM resolution
  int dutyCycle = map(temperature, 22, 100, 0, 255);

  return dutyCycle;
}
