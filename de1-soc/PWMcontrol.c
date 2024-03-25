#include <stdio.h> 
#include <stdlib.h> 
#include <stdbool.h>
#include <time.h>

#define JP2_BASE            0xFF200070 //GPIO_1 memory mapped address
#define TIMER_COUNT         100000000 / (2 * 25000) // 100MHz clock / (2 * 25kHz PWM frequency)

volatile int *GPIO_1 = (volatile int *)JP2_BASE;

void PWMcontrol() {
    int half_period = TIMER_COUNT;

    // generate the square wave signal
    for (int cycle = 0; cycle < 25000; ++cycle) {
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
        PWMcontrol(); 
    }

    return 0; 
}
