#include <stdio.h> 
#include <stdlib.h> 
#include <stdbool.h>
#include <time.h>


int main (void){
    int temperature = 0;
    int RPM = 0; //fan speed in revolutions per 

    while(1){
        //read digital temperature via I2C from GPIO (or from on board ADC)

        //generate PWM command and updated fan speed 

        //draw and update VGA screen

    }

 return 0;
}