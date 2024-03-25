#include <stdio.h> 
#include <stdlib.h> 
#include <stdbool.h>
#include <time.h>

#define JP1_BASE			0xFF200060
#define JP2_BASE			0xFF200070 //GPIO_1 memory mapped address
#define LED_BASE			0xFF200000 //red LEDs
#define SW_BASE				0xFF200040 //switch base 
#define TIMER_BASE			0xFF202000 //timer base 
#define TIMER_COUNT         100000000/(2*25000) //clock speed mapped to the PWM frequency of the motor 

void PWMcontrol(int temperature); 

volatile int *LEDs = (int*)LED_BASE;
volatile int *SW = (int*)SW_BASE;
volatile int *GPIO_0 = (int*)JP1_BASE;
volatile int *GPIO_1 = (int*)JP2_BASE;

int temperature;
int RPM;
int PWM_OUT; //reading from GPIO_1[0]
int PWM_IN; //reading from GPIO_1[1]

void PWMcontrol(int temperature) {
    // map temperature to RPM linearly
    int RPM = (temperature * 6100) / 1023;

    // calculate duty cycle as a percentage
    int dutyCycle = RPM / 6100;

    int high = dutyCycle * TIMER_COUNT; //count for every edge of the clock we are HIGH
    int low = TIMER_COUNT - high; //count for every edge of the clock we are LOW 

    // generate the PWM signal
    for (int cycle = 0; cycle < 25000; ++cycle) {

        *GPIO_1 |= 0x1;
        for (int i = 0; i < high; ++i); // on-time delay

        *GPIO_1 &= ~0x1;
        for (int i = 0; i < low; ++i); // off-time delay
    }

    // output fan speed in RPM 
    // display the current fan speed in RPM on the 10 LEDs (in binary)
    // max RPM is 6100 RPM, so for every 6 RPM, increase in 1 in binary on the LEDs
}





int PWMread(){
    // read in the tach pin 

}

int main(void){
    //set the direction register of the JP2 BASE to output 
    volatile int *gpio_direction_reg = (volatile int *)(JP2_BASE + 4);
    *gpio_direction_reg = 0x00000001;
    
    while(1){
        temperature = *SW;
        PWMcontrol(temperature);
    }

    //map switches to PWM signal (10 bit PWM signal)
    //generate PWM signal and OUTPUT from GPIO_1[0]
    //read the current PWM signal from the blue/yellow line (tach) on GPIO_1[1] and display it on the LEDs
}
