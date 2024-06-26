
#define ADC_BASE			0xFF204000
#define HEX3_HEX0_BASE			0xFF200020
#define LED_BASE			0xFF200000

volatile int *HEX3_0 = (volatile int*) HEX3_HEX0_BASE;
volatile int *ADC_ptr = (int *) ADC_BASE;
volatile int *LEDs = (volatile int*) LED_BASE;

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

void adcRead();
int temperature;

int main(void){
    *(ADC_ptr+1) = 0xFFFFFFFF;	// sets the ADC up to automatically perform conversions
    while(1){
        adcRead();
    }
}

void adcRead(){ //read from the internal 12-bit ADC
    //scale / convert raw 12 bit ADC reading into integer temperature value (MAKE SURE COMPILE FLAGS ARE ON)
    int rawADC = *(ADC_ptr) & 0xFFF;
    float voltage = rawADC * (5.0 / 4095.0); //12 bit resolution
    float tempC = (voltage - 0.75) / (10.0 / 1000.0) + 25; //calibration
    int temperature = (int)(tempC + (tempC > 0 ? 0.5 : -0.5)) - 25; // maybe remove, no arduino

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
    *LEDs = temperature; //temporary debug 

}