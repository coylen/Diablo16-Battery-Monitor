// K_LRS - dictates how often the scope is updated and
//         is the equivalent of RefreshIncrement
//#constant _hours:=0, _minutes, _seconds, _date, _month, _year;
#inherit "..\\libs\\DS1307.lib"
#inherit "include\\debug.inc"
//#inherit "i2c.lib"
//#constant K_LRS 1   // try this
//#constant K_LRS 5   // try this
#constant K_LRS 10  // try this
//#constant K_LRS 15  // try this
var hrs, minutes, seconds, msec, year, month, day;
var new_y1[96], old_y1[96], temp1[96], cache1[K_LRS];
var new_y2[96], old_y2[96], temp2[96], cache2[K_LRS];
var i_LRS, k_LRS, m_LRS;
var j_LRS := 0, counter_LRS := 0;

i_LRS := K_LRS - 1;
var timeru :=0;
var dummy :=0;
//SCOPE FUNCTIONS TO BE EDITED
func UpdateLRScope(var value)

    if (i_LRS >= 0)
        // sine wave
        j_LRS--;
        m_LRS := ABS(j_LRS%360);
        m_LRS := 360 - m_LRS;
        cache1[i_LRS] := SIN(m_LRS);

        // waveform from serial port
        cache2[i_LRS] := value;
        i_LRS--;

    else
        // update the buffer
        // shift contents k units to the right, using the temporary buffer "temp"
        // sine wave
        mem_Copy(new_y1, temp1, 2*(sizeof(new_y1)-K_LRS));
        mem_Copy(temp1, &new_y1[K_LRS], 2*(sizeof(new_y1)-K_LRS));
        mem_Copy(cache1, new_y1, 2*K_LRS);

        // waveform from serial port
        mem_Copy(new_y2, temp2, 2*(sizeof(new_y2)-K_LRS));
        mem_Copy(temp2, &new_y2[K_LRS], 2*(sizeof(new_y2)-K_LRS));
        mem_Copy(cache2, new_y2, 2*K_LRS);

        if(CurrentForm == 1)
            // draw and update scope
            Graticule(16, 16, 208, 166, 8,  25, 0x0280) ;
            gfx_Scope(16, 192, 166, 96, 2, 100, BLACK,
            old_y1, new_y1, LIME, old_y2, new_y2, ORANGE, 0,0,0, 0,0,0 ) ;
            //gfx_Hline(52 + 75, 40, 289, YELLOW) ;
        endif

        i_LRS := K_LRS - 1;
    endif

endfunc

func Graticule(var x1, var y1, var x2, var y2, var xinc, var yinc, var color)
    var y ;
    x1 += xinc ;
    while (x1 < x2)
        y := y1 + yinc ;
        while (y < y2)
            gfx_PutPixel(x1, y, color) ;
            y += yinc ;
        wend
        x1 += xinc ;
    wend
endfunc

//SETTINGS FORM INTERUPT TIMER
func timerupdate()
    timeru := 1;
    sys_SetTimer(TIMER7,1000);
endfunc


//
// Added 07/03/2017 16:26:14
//
// MagicCode2
//
// Battery values
var B1V[2], B1A[2], B1AH[2], B1Aoffset[2], B1charge, B1AHcapacity;
var B2V[2], B2A[2], B2AH[2], B2Aoffset[2], B2charge, B2AHcapacity;
// graph values
// day
var B1Vd[96],B1Ad[96], B1Capd[96],B2Vd[96],B2Ad[96], B2Capd[96];
var old_Vd[96], old_Ad[96], old_Capd[96], spared[96];
// week
var B1Vw[168],B1Aw[168], B1Capw[168],B2Vw[168],B2Aw[168], B2Capw[168];
var old_Vw[168], old_Aw[168], old_Capw[168], sparew[168];
// month
var B1Vm[186],B1Am[186], B1Capm[186],B2Vm[186],B2Am[186], B2Capm[186];
var old_Vm[186], old_Am[186], old_Capm[186], sparem[186];


// SETTINGS VARIABLES

var edt, tmp_num := -1;

//SETTINGS FORM TIMER DISPLAY UPDATE
func updatetime()
    sys_GetTimeVar(&hrs,&minutes,&seconds,&msec);
    sys_GetDateVar(&year, &month, &day);
    WriteObject(tCustomdigits, 6, hrs);
    WriteObject(tCustomdigits, 7, minutes);
    WriteObject(tCustomdigits, 8, seconds);
    WriteObject(tCustomdigits, 9, day);
    WriteObject(tCustomdigits, 10, month);
    WriteObject(tCustomdigits, 11, year);
endfunc

//SETTINGS FORM EDIT LINE RESET
func cleareditlines()
    gfx_Hline(43, 10, 38, BLACK);
    gfx_Hline(43, 41, 69, BLACK);
    gfx_Hline(43, 80, 108, BLACK);
    gfx_Hline(43, 168, 198, BLACK);
    gfx_Hline(43, 212, 242, BLACK);
    gfx_Hline(43, 252, 312, BLACK);
    gfx_Hline(127, 32, 78, BLACK);
    gfx_Hline(187, 32, 78, BLACK);
endfunc
