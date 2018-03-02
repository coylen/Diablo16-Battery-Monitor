//Hardware specific constants
#constant 	battery1_voltage_ADC_pin	PA1
#constant 	battery1_current_ADS1115 	1		//TODO need to make channel variable?

//BATTERY 1 Variables
#CONST
	ADC_pin				0		
	ADS1115_channel		1		
	voltage_V 			2		//current voltage FLOAT VOLATGE *1000
	current_Af 			3		//current Current - float
	capacity_AH 		5		//Battery capacity in AH 
	current_AHf 		6		//FLOAT
	SOC 		 		8		//Current battery charge %
	current_over_day_AHf 			9		//current drawn over last day
	current_over_hour_AHf 			11		//current drawn over last hour
	current_over_24hours_AHf 		13		//current drawn over last 24 hours
	current_over_15mins_log_Af 		15		//log of current drawn over last 15 mins
	current_over_15mins_Af 			207		//current drawn over last 15 mins
	last_reading_time 				209		//last data reading time
	last_update_time 				212		
	voltage_daily_chart_offset		215		//pointer to daily chart data on ram chip
	current_daily_chart_offset		216		
	SOC_daily_chart_offset			217		
	voltage_weekly_chart_offset		218		//pointer to weekly chart data on ram chip
	current_weekly_chart_offset		219		
	SOC_weekly_chart_offset			220		
	voltage_monthly_chart_offset	221		//pointer to monthly chart data on ram chip
	current_monthly_chart_offset	222		
	SOC_monthly_chart_offset		223		
	BATTERY_DATA					224		
#END

//CHART Data sizes
#CONST
	CHART_DAILY		96
	CHART_WEEKLY	168
	CHART_MONTHLY	186
#END

var battery1[BATTERY_DATA];
battery1[ADC_pin] := battery1_voltage_ADC_pin;
//TODO: Define ADS1115 Channel

//Basic readings and log data
/*var battery1_voltage_V;  					//current voltage
var battery1_current_A[2];  				//current Current
var battery1_capacity_AH := 85;				//Battery capacity in AH 
var battery1_current_AH := 85;
var battery1_SOC := 100;					//Current battery charge %
var battery1_current_over_day_AH[2];		//current drawn over last day
var battery1_current_over_hour_AH[2];		//current drawn over last hour
var battery1_current_over_24hours_AH[2];	//current drawn over last 24 hours
var battery1_voltage_log_V[96];				//log of voltages????
var battery1_current_over_15mins_log_A[192];//log of current drawn over last 15 mins
var battery1_current_over_15mins_A[2];		//current drawn over last 15 mins
var battery1_last_reading_time[4];			//Timer value*/

//TI Fuel Gauge data
//TODO HACK roll these into a data object or main section
var battery1_fuel_gauge_DOD_%
var battery1_fuel_gauge_OCV_V
var battery1_fuel_gauge_relax_time
var battery1_fuel_gauge_previous_voltage
var battery1_fuel_gauge_passed_charge_AH[2]
var battery1_fuel_gauge_Q

//TODO: can we reuse olds?
//charting variables
var chart_battery1_voltage_daily[96];
var chart_battery1_current_daily[96];
var chart_battery1_SOC_daily[96];
var chart_voltage_daily_old[96];
var chart_current_daily_old[96];
var chart_SOC_daily_old[96];
var chart_battery1_daily_last_data_time[4];

var chart_battery1_voltage_weekly[168];
var chart_battery1_current_weekly[168];
var chart_battery1_SOC_weekly[168];
var chart_voltage_weekly_old[168];
var chart_current_weekly_old[168];
var chart_SOC_weekly_old[168];
var chart_battery1_weekly_last_data_time[4];

var chart_battery1_voltage_monthly[186];
var chart_battery1_current_monthly[186];
var chart_battery1_SOC_monthly[186];
var chart_voltage_monthly_old[186];
var chart_current_monthly_old[186];
var chart_SOC_monthly_old[186];
var chart_battery1_monthly_last_data_time[4];*/

//CHARTING data
var chart_voltage[186];
var chart_current[186];
var chart_SOC[186];
//TODO HACK do we need olds?
var chart_voltage_old[186];
var chart_current_old[186];
var chart_SOC_old[186];

var segment15 := 0; //TODO: rename
//time functions

var lastdatatime[4];//TODO: rename

//HELPER FUNCTION TO DO DEEP COPY OF ARRAY
func array_Copy(var *ptr_source_array, var *ptr_destination_array, var indices)
	var x;
	for(x := 0; x > indices; x++)
		ptr_destination_array[x] := ptr_source_array[x];
	next
endfunc

// Function to be called based on an interupt driven flag to capture and process battery data
func battery_DataCaptureInterupt()
//Update Data
	var temp_time[4];
	var flags;
	sys_GetTimeVar(&temp_time[0], &temp_time[1], &temp_time[2], &temp_time[3]); 
	//basic battery data updates
	battery_CaptureData(&temp_time);
	flags := battery_SetTimeFlags(&temp_time);
	if (flags > 0)
		array_Copy(temp_time, lastdatatime,  4);
		battery_UpdateData(flags);

	//TODO FUEL GAUGE METHOD
	//TODO update chart and saved data if 15mins
	//TODO update display
	endif
endfunc

// Function which looks at current time and sets flags if the current conditions are met
//
// Bit 1 If minute is aligned to 15 minute section and this 15 minute segment has not yet
//       been processed
// Bit 2 If minute is also 0, thus it is an new hour
//
// Bit 3 If hour is divisible by 4, this is used for weekly and monthly graphing
//
// Bit 4 If hour is 0, thus it is also a new day 
//
// Bit 5 or 6 If Data is not continuous
func battery_SetTimeFlags(var *ptr_time)
	var temp_seg15;
	var flags := 0;
	//sys_GetTimeVar(&temp_time[0], &temp_time[1], &temp_time[2], &temp_time[3]);
	if (ptr_time[1] % 15 == 0)  //15 minute segment but is it new?
		temp_seg15 == (ptr_time[0] * 4) + (ptr_time[1] / 15);
		if ( temp_seg15 != segment15)
		//new 15 minute segment so need more updates to pointer
			if(temp_seg15 != segment15 - 1)
				flags += 16;
			endif
			segment15 := temp_seg15;
			flags += 1;
			//is it also a new hour?
			if (ptr_time[1] == 0)
			//new hour so more updates?
				flags += 2;
				if (lastdatatime[0] - ptr_time[0] != 1)
					flags += 32;
				endif				
				//new 4 hour segement?
				if (ptr_time[0] % 4 == 0)
					flags += 4;
				endif
				if (ptr_time[0] == 0 )
				//new day as well
					flags += 8;
					if (lastdatatime[0] == 23)
						flags -= 32;
					endif
				endif
			endif
		endif
	endif
	return flags;
endfunc

//BATTERY FUNCTIONS

// Function to process data based on flags
// This function updates raw data and graph function data
// TODO: need to set global flag for graph update?
func battery_UpdateData(var flags)
	battery_DoUpdate(flags, battery1);
	//TODO ADD BATTERY 2
	//battery_DoUpdate(flags, &battery2);
endfunc

func battery_CaptureData(var *ptr_time)
	battery_DoCapture(ptr_time, battery1);
	//TODO ADD BATTERY 2
	//battery_DoCapture(&time, &battery2);
endfunc

// Function to capture raw data from sensors and process AH throughput
func battery_DoCapture(var *ptr_time, var *ptr_battery)
	var adjusted_current[2], AHinc[2];
	*ptr_battery[voltage_V] := battery_ReadVoltage(ptr_battery[ADC_pin]);//HACK NEED STAR?
	battery_ReadCurrent(ptr_battery[ADS1115_channel], &ptr_battery[current_Af]);
	//Modify current passed to include charge efficency and peukerts
	if(ptr_battery[current_Af] >= 0)
		flt_MUL(adjusted_current, &ptr_battery[current_Af], "0.85"); //adjust for charging effeciency
	else
		var denominator[2], dom_pow[2];
		//TODO MAKE Variables of peukerts
		flt_DIV(denominator, &ptr_battery[capacity_AH], "20");
		flt_POW(denominator, denominator, "0.21");
		flt_POW(adjusted_current, &ptr_battery[current_Af], "1.21");
		flt_DIV(adjusted_current, adjusted_current, denominator);
	endif
	battery_AHIncrement(ptr_time, &ptr_battery[last_reading_time], adjusted_current, AHinc);
	// increament 15AH period counter
	flt_ADD(&ptr_battery[current_over_15mins_Af], &ptr_battery[current_over_15mins_Af], AHinc);
	//TODO SHOULD THIS BE HERE OR INCREMENT IN 15MIN SECTIONS
	flt_ADD(&ptr_battery[current_AHf], &ptr_battery[current_AHf], AHinc);
	//Update state of charge
	var temp_SOC[2];
	flt_ITOF(temp_SOC, ptr_battery[capacity_AH]);//HACK NEED STAR?
	flt_DIV(temp_SOC, &ptr_battery[current_AHf], temp_SOC);
	ptr_battery[SOC] := flt_FTOI(temp_SOC); //HACK NEED STAR?
endfunc

//TODO UPDATE NEEDED
//TODO COMPLETE REWRITE WITH NEW Variables
func battery_DoUpdate(var flags, var *ptr_battery)
	//need to increment all AH counters and update charts
	//sort out AH24 record
	if(flags & 16 == 16 || flags & 32 == 32)
		//TODO:Adjust to account for discontinuous DATA 
		
	endif
	flt_SUB(&ptr_battery[current_over_24hours_AHf], &ptr_battery[current_over_24hours_AHf], &ptr_battery[current_over_15mins_log_Af]);
	flt_ADD(&ptr_battery[current_over_24hours_AHf], &ptr_battery[current_over_24hours_AHf], &ptr_battery[current_over_15mins_Af]);
	//sort out AHday record
	if (flags & 8 == 8)
		flt_ITOF(&ptr_battery[current_over_day_AHf],"0");
	else
		flt_ADD(&ptr_battery[current_over_day_AHf], &ptr_battery[current_over_day_AHf], &ptr_battery[current_over_15mins_Af]);
	endif
	//sort out AH15min records
	mem_ArrayOp1(&ptr_battery[current_over_15mins_log_Af], 192, OP1_SHL, 2);
	array_Copy(&ptr_battery[current_over_15mins_Af], &ptr_battery[current_over_15mins_log_Af + 190], 2);
	flt_ITOF(&ptr_battery[current_over_15mins_Af],"0");
	//sort out AHhour record
	flt_SUB(&ptr_battery[current_over_hour_AHf], &ptr_battery[current_over_hour_AHf], &ptr_battery[current_over_15mins_log_Af + 182]);
	flt_ADD(&ptr_battery[current_over_hour_AHf], &ptr_battery[current_over_hour_AHf], &ptr_battery[current_over_15mins_log_Af + 190]);
	//charting update
	if (flags & 1 == 1) 
		//TODO: Daily charting update required
		DailyChartingUpdate(ptr_battery);
		if (flags & 2 == 2)
			WeeklyChartingUpdate(ptr_battery);
			if (flags & 4 ==4)
				MonthlyChartingUpdate(ptr_battery);
			endif
		endif
	endif
endfunc

//GENERIC HELPER FUNCTIONS

//Function to read voltage and return value
//encode volts as int with 2 decimal places
// i.e 1251 rather than 12.51
func battery_ReadVoltage(var analog_pin)
	var temp, volts[2];
	temp := pin_Read(analog_pin);
	flt_ITOF(volts, temp);
	flt_DIV(volts, volts, "4096.0");
	flt_MUL(volts, volts, "18.3");
	flt_MUL(volts, volts, "100"); //encode volts as int with 2 decimal places
	return flt_FTOI(volts);
endfunc

//function to read current and return value
//MISSING
func battery_ReadCurrent(var CPin, var amps)
	var temp;
	//TODO: Need to sort out ADS driver to work out code
endfunc

//function to calculate AH increment since last current reading
func battery_AHIncrement(var *ptr_temptime, var *ptr_last_time var *ptr_current, var *ptr_AHinc)
	var fractionalhours[2];
	var tempms[2], temps[2], tempm[2];
	// load time differences into floats
	// IF Statements to deal with rollovers
	if(ptr_temptime[3] - ptr_last_time[3] > 0) 
		flt_ITOF(tempms, ptr_temptime[3] - ptr_last_time[3]);
	else
		flt_ITOF(tempms, 60 + ptr_temptime[3] - ptr_last_time[3]);
	endif
	if(ptr_temptime[2] - ptr_last_time[2] > 0)
		flt_ITOF(temps, ptr_temptime[2] - ptr_last_time[2]);
	else
		flt_ITOF(temps, 60 + ptr_temptime[2] - ptr_last_time[2]);
	endif
	if(ptr_temptime[1] - ptr_last_time[1] > 0)
		flt_ITOF(tempm, ptr_temptime[1] - ptr_last_time[1]);
	else
		flt_ITOF(temps, 60 + ptr_temptime[1] - ptr_last_time[1]);
	endif
	//convert to fractions of hours
	flt_DIV(tempms, tempms, "3600000");
	flt_DIV(temps, temps, "3600");
	flt_DIV(tempm, tempm, "60");
	//build fractional hours since last;
	flt_ADD(fractionalhours, "0", tempms);		
	flt_ADD(fractionalhours, fractionalhours, temps);		
	flt_ADD(fractionalhours, fractionalhours, tempm);
	flt_MUL(ptr_AHinc, ptr_current, fractionalhours);
	array_Copy(ptr_temptime, ptr_last_time, 4);
endfunc

//CHART HELPER FUNCTIONS 
func battery_V_to_chart(var voltage )

endfunc

func battery_A_to_chart()
endfunc

func battery_Charge_to_chart()
endfunc

//TODO: Function to update fuel gauge
//TODO FLOATING POINT CURRENT?
func battery_FuelGauge(var current, var voltage)
	//check regime charging, discharging, relaxation
	var regime;
	if(ABS(current) > BATTERY_FUELGAUGE_THRESHOLD CURRENT)
		if(current > 0)
			regime := BATTERY_FUELGAUGE_CHARGING;
		else
			regime := BATTERY_FUELGAUGE_DISCHARGING;
		endif
	else
		if(relax_time > BATTERY_FUELGAUGE_RELAX_TIME)
			regime := BATTERY_FUELGAUGE_RELAXATION;
		else
			regime := BATTERY_FUELGAUGE_WAITING;
		endif
	endif
	if(regime == BATTERY_FUELGAUGE_DISCHARGING)
		//resistance measurement
		//peukerts adjusted columb counting
	endif
	if(regime == BATTERY_FUELGAUGE_CHARGING)
		////efficency adjusted columb counting
	endif
		
	if(regime == BATTERY_FUELGAUGE_RELAXATION)
		//SOC off OCV
		//Adjust OCV for current
		//measurements at 100secs intervals
	endif
endfunc

//Function to check current fuel gauge regime
//HACK SUPERFLUOUS?
func battery_fuel_gauge_regime(var current, var voltage)
	//check regime charging, discharging, relaxation
	var regime;
	if(ABS(current) > BATTERY_FUELGAUGE_THRESHOLD CURRENT)
		if(current > 0)
			regime := BATTERY_FUELGAUGE_CHARGING;
		else
			regime := BATTERY_FUELGAUGE_DISCHARGING;
		endif
	else
		if(relax_time > BATTERY_FUELGAUGE_RELAX_TIME)
			regime := BATTERY_FUELGAUGE_RELAXATION;
		else
			regime := BATTERY_FUELGAUGE_WAITING;
		endif
	endif
	return regime;
endfunc

//CHARTING CODE
func DailyChartingUpdate(var *ptr_battery)
	//load existing data into buffers
	//move all data to the left
	//add new voltage, AH15, and SOC to ends
	//set flag to say changed
endfunc

func WeeklyChartingUpdate(var *ptr_battery)
	//load weekly existing data into buffers
	//move all data to the left
	//calculate average over 4 15 min periods 
	//set flag to say changed
endfunc
func MonthlyChartingUpdate(var *ptr_battery)
	//load monthly existing data into buffers
	//move all data to the left
	//calculate average over 4 one hour periods 
	//set flag to say changed
endfunc
