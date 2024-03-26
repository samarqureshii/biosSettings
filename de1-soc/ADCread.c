
#define ADC_BASE			0xFF204000
#define HEX3_HEX0_BASE			0xFF200020
#define HEX5_HEX4_BASE			0xFF200030

volatile int* HEX3_0 = (volatile int *)HEX3_HEX0_BASE;

void adcRead();
int temperature;

int main(void){
    adcRead(); // read adc and display the temperature value on the HEX
}

void adcRead(){ //read from the internal 12-bit ADC
    // display the current temperature on the hex

    //temperature should not go above 3 digits, only use HEX2, HEX1, HEX0
    // bits 6 - 0 are HEX0 (ones place)
    // bits 14 - 8 are HEX1 (tens place)
    // bits 22 - 16 are HEX2 (hundreds place)

    //extract each digit and display on the corresponding hex
}