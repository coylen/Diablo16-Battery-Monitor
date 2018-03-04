//SETUP I2C and UART IN THIS SECTION READY FOR USE

//SETUP DEBUG PROVIDERS
//DEBUG_Register(DS1307DEBUGProvider);

//TODO: CAN BE REMOVED AT SOME POINT
//DEBUG_Register(demoprovider2);


//I2C STARTUP
//TODO: CHANGE PINS
I2C1_Open(I2C_SLOW,PA10,PA11);
//TODO: SETUP ADC CHANNELS

//TODO: Setup UART for BLUETOOTH

//Time debug set

//set time for RTC
RTC_to_sys();
//AT24test();

//TODO: LOAD DATA FROM MEMORY


//SETUP TIMER FOR DATA CAPTURE
sys_SetTimerEvent(TIMER5, batterytimerupdate);
sys_SetTimer(TIMER5,10000);

