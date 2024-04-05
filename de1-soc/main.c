#include <stdio.h> 
#include <stdlib.h> 
#include <stdbool.h>
#include <time.h>

/*******************************************************************************
 * This file provides address values that exist in the DE1-SoC Computer
 ******************************************************************************/

#ifndef __SYSTEM_INFO__
#define __SYSTEM_INFO__

#define BOARD				"DE1-SoC"

/* Memory */
#define DDR_BASE			0x40000000
#define DDR_END				0x7FFFFFFF
#define A9_ONCHIP_BASE			0xFFFF0000
#define A9_ONCHIP_END			0xFFFFFFFF
#define SDRAM_BASE			0x00000000
#define SDRAM_END			0x03FFFFFF
#define FPGA_PIXEL_BUF_BASE		0x08000000
#define FPGA_PIXEL_BUF_END		0x0803FFFF
#define FPGA_CHAR_BASE			0x09000000
#define FPGA_CHAR_END			0x09001FFF

/* Cyclone V FPGA devices */
#define LED_BASE			0xFF200000
#define LEDR_BASE			0xFF200000
#define HEX3_HEX0_BASE			0xFF200020
#define HEX5_HEX4_BASE			0xFF200030
#define SW_BASE				0xFF200040
#define KEY_BASE			0xFF200050
#define JP1_BASE			0xFF200060
#define JP2_BASE			0xFF200070
#define PS2_BASE			0xFF200100
#define PS2_DUAL_BASE			0xFF200108
#define JTAG_UART_BASE			0xFF201000
#define IrDA_BASE			0xFF201020
#define TIMER_BASE			0xFF202000
#define TIMER_2_BASE			0xFF202020
#define AV_CONFIG_BASE			0xFF203000
#define RGB_RESAMPLER_BASE    		0xFF203010
#define PIXEL_BUF_CTRL_BASE		0xFF203020
#define CHAR_BUF_CTRL_BASE		0xFF203030
#define AUDIO_BASE			0xFF203040
#define VIDEO_IN_BASE			0xFF203060
#define EDGE_DETECT_CTRL_BASE		0xFF203070
#define ADC_BASE			0xFF204000

/* Cyclone V HPS devices */
#define HPS_GPIO1_BASE			0xFF709000
#define I2C0_BASE			0xFFC04000
#define I2C1_BASE			0xFFC05000
#define I2C2_BASE			0xFFC06000
#define I2C3_BASE			0xFFC07000
#define HPS_TIMER0_BASE			0xFFC08000
#define HPS_TIMER1_BASE			0xFFC09000
#define HPS_TIMER2_BASE			0xFFD00000
#define HPS_TIMER3_BASE			0xFFD01000
#define FPGA_BRIDGE			0xFFD0501C

#endif

#ifndef __NIOS2_CTRL_REG_MACROS__
#define __NIOS2_CTRL_REG_MACROS__ /*****************************************************************************/
/* Macros for accessing the control registers.                               */
/*****************************************************************************/
#define NIOS2_READ_STATUS(dest) \
do { dest = __builtin_rdctl(0); } while (0)
#define NIOS2_WRITE_STATUS(src) \
do { __builtin_wrctl(0, src); } while (0)
#define NIOS2_READ_ESTATUS(dest) \
do { dest = __builtin_rdctl(1); } while (0)
#define NIOS2_READ_BSTATUS(dest) \
do { dest = __builtin_rdctl(2); } while (0)
#define NIOS2_READ_IENABLE(dest) \
do { dest = __builtin_rdctl(3); } while (0)
#define NIOS2_WRITE_IENABLE(src) \
do { __builtin_wrctl(3, src); } while (0)
#define NIOS2_READ_IPENDING(dest) \
do { dest = __builtin_rdctl(4); } while (0)
#define NIOS2_READ_CPUID(dest) \
do { dest = __builtin_rdctl(5); } while (0)
#endif

#define timerStatus ((volatile short*) (TIMER_BASE))
#define timerControl ((volatile short*) (TIMER_BASE+4))
#define timerTimeoutL ((volatile short*) (TIMER_BASE+8))
#define timerTimeoutH ((volatile short*) (TIMER_BASE+12))
#define timerSnapshotL ((volatile short*) (TIMER_BASE+16))
#define timerSnapshotH ((volatile short*) (TIMER_BASE+20))
#define PWM_freq           25000
#define SYSTEM_CLOCK        100000000

/*************************************************************************************************************************************/
/********************************************************FUNCTION DECLARATIONS********************************************************/
/************************************************************************************************************************************/

void timerISR();
void timerConfig(unsigned int duration);
void interrupt_handler(void); 
void adcRead();

/********************************************************************************************************************************/
/********************************************************GLOBAL VARIABLES********************************************************/
/********************************************************************************************************************************/

volatile int *HEX3_0 = (volatile int*) HEX3_HEX0_BASE;
volatile int *ADC_ptr = (int *) ADC_BASE;
volatile int *LEDs = (volatile int*) LED_BASE;
volatile int *GPIO_1 = (volatile int *)JP2_BASE;
volatile int *SW = (volatile int *)SW_BASE;
volatile int *gpio_direction_reg = (volatile int *)(JP2_BASE + 4);

int temperature = 0;
int RPM = 0; //fan speed in revolutions per min
int usage = 0; //CPU usage
int pwmHigh = 0;
int pwmLow = 0;
int pwmState = 0; // ON or OFF
int lookupTable[10] = {
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111  // 9
};

/******************************************************************************************************************/
/********************************************************MAIN******************************************************/
/******************************************************************************************************************/

int main (void){
    *gpio_direction_reg = 0x00000001; // set GPIO_1[0] as an output pin
    // *(ADC_ptr+1) = 0xFFFFFFFF;	// sets the ADC up to automatically perform conversions
    NIOS2_WRITE_IENABLE(0x1); //level 0 (interval timer)
    NIOS2_WRITE_STATUS(0x1); //enable Nios II interrupts
    timerConfig(1000); //initially configure the timer to enable interrupts and such 

    while(1){
        //read digital temperature via ADC
        *(ADC_ptr+1) = 0xFFFFFFFF;	// sets the ADC up to automatically perform conversions
        adcRead(); //update the current temperature 
        *LEDs = usage; //change this to display the current RPM 

        //draw and update VGA screen
        // vgaDriver(temperature, RPM, usage);
    }

    return 0; 

 return 0;

}

/*******************************************************************************************************************************/
/********************************************************HELPER FUNCTIONS*******************************************************/
/*******************************************************************************************************************************/

void adcRead(){ //read from the internal 12-bit ADC
    //scale / convert raw 12 bit ADC reading into integer temperature value (MAKE SURE COMPILE FLAGS ARE ON)
    int rawADC = *(ADC_ptr) & 0xFFF;
    float voltage = rawADC * (5.0 / 4095.0); //12 bit resolution
    float tempC = (voltage - 0.75) / (10.0 / 1000.0) + 25; //calibration
    temperature = (int)(tempC + (tempC > 0 ? 0.5 : -0.5)) - 25; // maybe remove, no arduino

    //temperature should not go above 3 digits, only use HEX2, HEX1, HEX0
    // bits 6 - 0 are HEX0 (ones place)
    // bits 14 - 8 are HEX1 (tens place)
    // bits 22 - 16 are HEX2 (hundreds place)

    //extract each digit and display on the corresponding hex
    int hundreds = (int)temperature / 100;
    int tens = (temperature % 100) / 10;
    int ones = (int)temperature % 10;

    //*HEX3_0 = 0;

    if (temperature < 100) {
        hundreds = 0;
    }


    *HEX3_0 = (lookupTable[hundreds] << 16) | (lookupTable[tens] << 8) | lookupTable[ones];


}

void timerISR() { //ensures at least one full cycle runs before checking the SW_state again
    *timerStatus = 0x0; // clear the TO bit to acknowledge the interrupt
    
    //map temperature to CPU usage 
    float temp_percentage = ((float)(temperature - 23) / 103) * 100.0;
    usage = (int)(temp_percentage + 0.5); 
    if (usage < 0) usage = 0;
    if (usage > 100) usage = 100;

    int dutyCycle = usage;

    pwmHigh = (SYSTEM_CLOCK / PWM_freq) * (dutyCycle / 100.0);
    pwmLow = (SYSTEM_CLOCK / PWM_freq) - pwmHigh;
    
    // toggle GPIO 
    if (pwmState == 1) {
        *GPIO_1 |= 0x1; //high
        timerConfig(pwmHigh); 
    } 
    
    else {
        *GPIO_1 &= ~0x1; // low
        timerConfig(pwmLow); 
    }
    pwmState = !pwmState; // toggle state for next cycle
}

// void waitTO() { //polling timer (CPU block)
//     while ((*timerStatus & 0x1) == 0); // wait until the TO  bit is set
//     *timerStatus = 0x0; // clear the TO bit by writing 0 to the status register
// }

void timerConfig(unsigned int duration){ //start the timer 
    *timerStatus = 0x0; // clear the TO bit to acknowledge the interrupt
    *timerTimeoutL = duration & 0xFFFF; // base + 8
    *timerTimeoutH = (duration >> 16) & 0xFFFF; // base + 12
    *timerControl = 0x7; //start and CONT bits  (base + 4)
}

/* The assembly language code below handles CPU exception processing. This
 * code should not be modified; instead, the C language code in the function
 * interrupt_handler() can be modified as needed for a given application.
*/
void the_exception(void) __attribute__((section(".exceptions")));
void the_exception(void)
/*******************************************************************************
 * Exceptions code. By giving the code a section attribute with the name
 * ".exceptions" we allow the linker program to locate this code at the proper
 * exceptions vector address.
 * This code calls the interrupt handler and later returns from the exception.
 ******************************************************************************/
{
asm("subi sp, sp, 128");
asm("stw et, 96(sp)");
asm("rdctl et, ctl4");
asm("beq et, r0, SKIP_EA_DEC"); // Interrupt is not external
asm("subi ea, ea, 4"); /* Must decrement ea by one instruction
                                        * for external interupts, so that the
                                        * interrupted instruction will be run */
asm("SKIP_EA_DEC:");
asm("stw r1, 4(sp)"); // Save all registers 
asm("stw r2, 8(sp)");
asm("stw r3, 12(sp)");
asm("stw r4, 16(sp)");
asm("stw r5, 20(sp)");
asm("stw r6, 24(sp)");
asm("stw r7, 28(sp)");
asm("stw r8, 32(sp)");
asm("stw r9, 36(sp)");
asm("stw r10, 40(sp)");
asm("stw r11, 44(sp)");
asm("stw r12, 48(sp)");
asm("stw r13, 52(sp)");
asm("stw r14, 56(sp)");
asm("stw r15, 60(sp)");
asm("stw r16, 64(sp)");
asm("stw r17, 68(sp)");
asm("stw r18, 72(sp)");
asm("stw r19, 76(sp)");
asm("stw r20, 80(sp)");
asm("stw r21, 84(sp)");
asm("stw r22, 88(sp)");
asm("stw r23, 92(sp)");
asm("stw r25, 100(sp)");
asm("stw r26, 104(sp)");
// skip r27 because it is sp, and there is no point in saving this
asm("stw r28, 112(sp)");
asm("stw r29, 116(sp)");
asm("stw r30, 120(sp)");
asm("stw r31, 124(sp)");
asm("addi fp, sp, 128");
asm("call  interrupt_handler");
asm("ldw r1, 4(sp)"); // Restore all registers 
asm("ldw r2, 8(sp)");
asm("ldw r3, 12(sp)");
asm("ldw r4, 16(sp)");
asm("ldw r5, 20(sp)");
asm("ldw r6, 24(sp)");
asm("ldw r7, 28(sp)");
asm("ldw r8, 32(sp)");
asm("ldw r9, 36(sp)");
asm("ldw r10, 40(sp)");
asm("ldw r11, 44(sp)");
asm("ldw r12, 48(sp)");
asm("ldw r13, 52(sp)");
asm("ldw r14, 56(sp)");
asm("ldw r15, 60(sp)");
asm("ldw r16, 64(sp)");
asm("ldw r17, 68(sp)");
asm("ldw r18, 72(sp)");
asm("ldw r19, 76(sp)");
asm("ldw r20, 80(sp)");
asm("ldw r21, 84(sp)");
asm("ldw r22, 88(sp)");
asm("ldw r23, 92(sp)");
asm("ldw r24, 96(sp)");
asm("ldw r25, 100(sp)");
asm("ldw r26, 104(sp)");
asm("ldw r28, 112(sp)");
asm("ldw r29, 116(sp)");
asm("ldw r30, 120(sp)");
asm("ldw r31, 124(sp)");
asm("addi sp, sp, 128");
asm("eret");
}


void interrupt_handler(void) {  //when the TO bit times out 
    int ipending;
    NIOS2_READ_IPENDING(ipending);
    if (ipending & 0x1){ // timer caused the interrupt
        timerISR();
    }
    return;
}