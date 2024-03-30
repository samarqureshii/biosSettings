#include <stdio.h> 
#include <stdlib.h> 
#include <stdbool.h>
#include <time.h>

#define ADC_BASE			0xFF204000
#define HEX3_HEX0_BASE			0xFF200020
#define LED_BASE			0xFF200000

volatile int *HEX3_0 = (volatile int*) HEX3_HEX0_BASE;
volatile int *ADC_ptr = (int *) ADC_BASE;
volatile int *LEDs = (volatile int*) LED_BASE;

int temperature = 0;
int RPM = 0; //fan speed in revolutions per min
int usage = 0;

int main (void){

    while(1){
        //read digital temperature via ADC
        *(ADC_ptr+1) = 0xFFFFFFFF;	// sets the ADC up to automatically perform conversions
        adcRead();

        //generate PWM command and updated fan speed 
        int usage = ((temperature - 25)*100) / 105.0; //duty cycle as a percentage of possible temperature
        pwmControl(usage);

        //draw and update VGA screen
        vgaDriver(temperature, RPM, usage);
    }

 return 0;
}