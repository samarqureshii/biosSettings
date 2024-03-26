#include <stdio.h> 
#include <stdlib.h> 
#include <stdbool.h>
#include <time.h>

#define TIMER_BASE          0xFF202000
#define timerStatus ((volatile short*) (TIMER_BASE))
#define timerControl ((volatile short*) (timer+4))
#define timerTimeoutL ((volatile short*) (timer+8))
#define timerTimeoutH ((volatile short*) (timer+12))
#define timerSnapshotL ((volatile short*) (timer+16))
#define timerSnapshotH ((volatile short*) (timer+20))

#define JP1_BASE			0xFF200060
#define JP2_BASE			0xFF200070 //GPIO_1 memory mapped address
#define LED_BASE			0xFF200000 //red LEDs
#define SW_BASE				0xFF200040 //switch base 
#define TIMER_BASE			0xFF202000 //timer base 
#define TIMER_COUNT         100000000 / (2 * 25000) // 100MHz clock / (2 * 25kHz PWM frequency)
#define SYSTEM_CLOCK        100000000

volatile int *GPIO_1 = (volatile int *)JP2_BASE;
volatile int *SW = (volatile int *)SW_BASE;

void PWMcontrol(int frequency) {
    int half_period =  SYSTEM_CLOCK / (2 * frequency);;

    // generate the square wave signal
    for (int cycle = 0; cycle < frequency; ++cycle) { //one cycle 
        // set the GPIO pin high
        *GPIO_1 |= 0x1;

        // on-time delay
        for (volatile int i = 0; i < half_period; ++i);

        *GPIO_1 &= ~0x1;

        // off-time delay
        for (volatile int i = 0; i < half_period; ++i);
    }
}

int main(void) {
    // set the direction register of JP2 to output for GPIO_1[0]
    volatile int *gpio_direction_reg = (volatile int *)(JP2_BASE + 4);
    *gpio_direction_reg = 0x00000001;

    while (1) {
        int SW_state = *SW; 
        int frequency = ((SW_state & 0x3FF) * 20) + 100; // map switch state to frequency, increment by 20 Hz
        PWMcontrol(frequency);
    }

    return 0; 
}
