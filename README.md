# taskManager
> HW/SW Emulation of Windows Task Manager in Nios II Architecture

## TODO
- VGA Driver (Annie)
    - Assume given input of fan speed and temperature, continually update as they increase/decrease on the VGA
    - UI similar to Task Manager
    - If time, real-time line graph that shows temperature and RPM changing overtime
- Analog sensor read to MCU (Samar)
- I2C communication to DE1 via GPIO (Samar)
- PWM script 3.3V output (Samar) 
    - Can be tested without the temperature sensor 
- Get 12V switching reg working 
- Test level shifter with 12V
- Power to cooling fan
- Batteries in multimeter

## Parts to buy
- Temperature sensor
    - [LilyPad one](https://www.creatroninc.com/product/lilypad-temperature-sensor/)
- 12V supply
    - [12V buckboost](https://www.creatroninc.com/product/1-25-26v-1a-adjustable-step-up-down-regulator/)
- Cooling fan
    - [12V DC brushless fan](https://www.creatroninc.com/product/12v-dc-brushless-fan-70x70x15mm/)