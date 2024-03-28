#include <stdio.h> 
#include <stdlib.h> 
#include <stdbool.h>
#include <time.h>

#define JP1_BASE			0xFF200060
#define JP2_BASE			0xFF200070 //GPIO_1 memory mapped address
#define LED_BASE			0xFF200000 //red LEDs
#define SW_BASE				0xFF200040 //switch base 
#define TIMER_BASE			0xFF202000 //timer base 
#define timerStatus ((volatile short*) (TIMER_BASE))
#define timerControl ((volatile short*) (TIMER_BASE+4))
#define timerTimeoutL ((volatile short*) (TIMER_BASE+8))
#define timerTimeoutH ((volatile short*) (TIMER_BASE+12))
#define timerSnapshotL ((volatile short*) (TIMER_BASE+16))
#define timerSnapshotH ((volatile short*) (TIMER_BASE+20))
#define PWM_freq           25000
#define SYSTEM_CLOCK        100000000

volatile int *GPIO_1 = (volatile int *)JP2_BASE;
volatile int *SW = (volatile int *)SW_BASE;
volatile int *gpio_direction_reg = (volatile int *)(JP2_BASE + 4);

void PWM(unsigned int dutyCycle);
void waitTO(); 
void timerConfig(unsigned int duration);

void PWMcontrol(unsigned int dutyCycle) { //keep the frequency static at 25Khz, just toggle the duty cycle based on the switches 
    unsigned int high = SYSTEM_CLOCK / PWM_freq * dutyCycle / 100;
    unsigned int low = SYSTEM_CLOCK / PWM_freq - high;

    // set the GPIO pin high
    *GPIO_1 |= 0x1;
    timerConfig(high);
    waitTO();
    //for (volatile unsigned int i = 0; i < high; ++i); //change to use hardware timer 

    // set the GPIO pin low
    *GPIO_1 &= ~0x1;
    timerConfig(low);
    waitTO();

    //for (volatile unsigned int i = 0; i < low; ++i); //change to use hardware timer 
}

void waitTO() {
    while ((*timerStatus & 0x1) == 0); // wait until the TO  bit is set
    *timerStatus = 0x0; // clear the TO bit by writing 0 to the status register
}

void timerConfig(unsigned int duration){
    *timerTimeoutL = duration & 0xFFFF;
    *timerTimeoutH = (duration >> 16) & 0xFFFF;
    *timerControl = 0x7;
}

int main(void) {
    *gpio_direction_reg = 0x00000001; //set GPIO_1[0] as an output pin

    while (1) {
        int SW_state = *SW; 
        int dutyCycle = (SW_state*100)/1023; //switches as a % of the duty cycle
        PWMcontrol(dutyCycle);
    }

    return 0; 
}
