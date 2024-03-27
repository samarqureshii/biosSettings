
#define ADC_BASE			0xFF204000
#define HEX3_HEX0_BASE			0xFF200020
#define HEX5_HEX4_BASE			0xFF200030

volatile int *HEX3_0 = (volatile int *)HEX3_HEX0_BASE;
volatile int *ADC = (volatile int *)ADC_BASE;

void adcRead();
int temperature;

int main(void){
    adcRead(); // read adc and display the temperature value on the HEX
}

void adcRead(){ //read from the internal 12-bit ADC
    //scale / convert raw 12 bit ADC reading into integer temperature value 
    temperature = *ADC;

    // display the current temperature on the hex
    //extract the ones bit and display on HEX0
    if(temperature > 9){    //then extract the tens bit and display on HEX1
        if(temperature > 99){ //then extract the hundreds bit and display on HEX2

        }
    }

    //temperature should not go above 3 digits, only use HEX2, HEX1, HEX0
    // bits 6 - 0 are HEX0 (ones place)
    // bits 14 - 8 are HEX1 (tens place)
    // bits 22 - 16 are HEX2 (hundreds place)

    //extract each digit and display on the corresponding hex
}