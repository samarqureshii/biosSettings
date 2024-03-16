#include <stdio.h> 
#include <stdlib.h> 
#include <stdbool.h>
#include <time.h>

int PWMcontrol(float temperature){
    // map temperature to RPM linearly
    // output fan speed in RPM 
    // display the current fan speed in RPM on the 10 LEDs (in binary)
    // max RPM is 6100 RPM, so for every 6 RPM, increase in 1 in binary on the LEDs
}

//for 100MHz clock, we want to control the fan at 25kHz, so we need a counter that goes up to 4000
// figure out how to do this on the hardware timer later 