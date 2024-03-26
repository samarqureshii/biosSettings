#include <Arduino.h>

const int pwmPin = 3; 
const int tachPin = 5;
const int tempSensor = A0;
const float scaleFactor = 20; //20 mV
// const int led1 = 2;
// const int led2 = 4;
// const int led3 = 6;
// const int led4 = 7;
// const int led5 = 8;
// const int led6 = 9;
// const int led7 = 10;
// const int led8 = 11;
// const int led9 = 12;
// const int led10 = 12;
//const int ledPins[] = {2, 4, 6, 7, 8, 9, 10, 11, 12, 13};

int PWMcontrol(int temperature);
int dutyCycle = 0;

void setup() {
  //RPM OUTPUT (10 bit LEDs)
  // for(int i = 0; i< 10; i++){
  //   pinMode(ledPins[i], OUTPUT);
  // }
  // //MOTOR CONTROL
  // pinMode(pwmPin, OUTPUT);
  // pinMode(tachPin, INPUT);
  Serial.begin(9600);
  //analogReference(EXTERNAL);
  
}

void loop() {
  int sensorVal = analogRead(A0);
  float voltage = sensorVal*(3.3/1023);
  float tempC = (voltage - 0.5)/(20.0/1000.0);
  Serial.print("Temperature: ");
  Serial.print(sensorVal);
  Serial.println(" C");
  delay(1000);

    // analogWrite(pwmPin, dutyCycle);
    // // Serial.print("Temperature: ");
    // // Serial.print(temp);
    // Serial.print("PWM Duty Cycle: ");
    // Serial.println(dutyCycle);
    // Serial.println("\n\n");

    // delay(100);
    // dutyCycle++;
    // if(dutyCycle>=255){ // reset the duty cycle
    //   dutyCycle = 0;
    // }

    // int durationHigh = pulseIn(tachPin, HIGH);
    // int durationCycle = durationHigh + pulseIn(tachPin, LOW);
    // int speed = (durationHigh / (float)durationCycle) * 100;
    // Serial.println("Raw Speed (tach-in): ");
    // Serial.println(speed);


}

int PWMcontrol(int temperature) {
  // analogWrite expects a value from 0 to 255 for 8-bit PWM resolution
  int dutyCycle = map(temperature, 22, 100, 0, 255);

  return dutyCycle;
}
