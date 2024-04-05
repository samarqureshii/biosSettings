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

volatile int *GPIO_1 = (volatile int *)JP2_BASE;
volatile int *SW = (volatile int *)SW_BASE;
volatile int *gpio_direction_reg = (volatile int *)(JP2_BASE + 4);
volatile int *LEDs = (volatile int*) LED_BASE;

int pwmHigh = 0;
int pwmLow = 0;
int pwmState = 0; // ON or OFF
int dutyCycle = 0;

void PWMcontrol(unsigned int dutyCycle);
void timerISR();
void timerConfig(unsigned int duration);
void interrupt_handler(void); 

/* The assembly language code below handles CPU reset processing */
void the_reset(void) __attribute__((section(".reset")));
void the_reset(void)
/*******************************************************************************
 * Reset code. By giving the code a section attribute with the name ".reset" we
 * allow the linker program to locate this code at the proper reset vector
 * address. This code just calls the main program.
 ******************************************************************************/
{
    asm(".set noat");
    asm(".set nobreak");
    asm("movia r2, main" );
    asm("jmp r2");
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
}

int main(void) {
    *gpio_direction_reg = 0x00000001; // set GPIO_1[0] as an output pin
    NIOS2_WRITE_STATUS(0x1); //enable Nios II interrupts
    NIOS2_WRITE_IENABLE(0x1); //level 0 (interval timer)
    timerConfig(1000); //initially configure the timer to enable interrupts and such 

    while (1) {
        *LEDs = dutyCycle;
    }

    return 0; 
}

void timerISR() { //ensures at least one full cycle runs before checking the SW_state again
    *timerStatus = 0x0; // clear the TO bit to acknowledge the interrupt
    
    int SW_state = *SW;
    dutyCycle = (SW_state * 100) / 1023;

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