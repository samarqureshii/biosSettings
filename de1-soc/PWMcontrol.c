#include <stdio.h> 
#include <stdlib.h> 
#include <stdbool.h>
#include <time.h>

#define JP2_BASE			0xFF200070 //GPIO_1 memory mapped address
#define LED_BASE			0xFF200000 //red LEDs
#define SW_BASE				0xFF200040 //switch base 

void PWMcontrol(int temperature); 

volatile int *LEDs = LED_BASE;
volatile int *SW = SW_BASE;
volatile int *GPIO_1 = JP2_BASE;

int temperature;
int RPM;
int PWM_OUT; //reading from GPIO_1[0]
int PWM_IN; //reading from GPIO_1[1]

void PWMcontrol(int temperature){
    // map temperature to RPM linearly
    // output fan speed in RPM 
    // display the current fan speed in RPM on the 10 LEDs (in binary)
    // max RPM is 6100 RPM, so for every 6 RPM, increase in 1 in binary on the LEDs

}

int PWMread(){
    // read in the tach pin 

}

int main(void){
    //map switches to PWM signal (10 bit PWM signal)
    //generate PWM signal and OUTPUT from GPIO_1[0]
    //read the current PWM signal from the blue/yellow line (tach) on GPIO_1[1] and display it on the LEDs
}

//for 100MHz clock, we want to control the fan at 25kHz, so we need a counter that goes up to 4000
// figure out how to do this on the hardware timer later 