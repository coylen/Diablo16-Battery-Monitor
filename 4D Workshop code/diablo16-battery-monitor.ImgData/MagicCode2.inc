//SETUP I2C and UART IN THIS SECTION READY FOR USE

//I2C STARTUP
//TODO: CHANGE PINS
I2C1_Open(I2C_SLOW,PA10,PA11);

//TODO: SETUP ADC CHANNELS

//TODO: Setup UART for BLUETOOTH

//set time for RTC
Corrected_RTC_to_sys();
var buf[10];
to(buf);putstr("DEBUG TEST");
PrintStrings(2, buf, 1);


//TODO: LOAD DATA FROM MEMORY

//TODO: SETUP TIMER FOR DATA CAPTURE



