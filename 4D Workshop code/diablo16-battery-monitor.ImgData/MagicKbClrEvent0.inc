//
// Added 26/06/2017 14:54:21
//
func rMagicKbClrEvent0(var reportID, var objType, var objHash, var value)
    if(tmp_num == -1)
        if(edt <12)
            //TODO: Does this work or do i need to stop timer for all edits
            sys_SetTimerEvent(TIMER7, 0);
        endif
        switch(edt)
            case 6:
                tmp_num:= hrs;
                break;
            case 7:
                tmp_num := minutes;
                break;
            case 8:
                tmp_num := seconds;
                break;
            case 9:
                tmp_num:= day;
                break;
            case 10:
                tmp_num := month;
                break;
            case 11:
                tmp_num := year;
                break;
            case 12:
                tmp_num := B1AHcapacity;
                break;
            case 13:
                tmp_num := B2AHcapacity;
                break;
        endswitch
    endif
    if(value>=48 && value<=57)
        switch
            case(edt < 11)
                tmp_num := (tmp_num * 10 +(value - 48)) % 100;
                break;
            case(edt == 11)
                if(tmp_num > 3000)
                    tmp_num %=1000;
                endif
                tmp_num := (tmp_num * 10 +(value - 48)) % 10000;
                break;
            case(edt >= 12)
                tmp_num := (tmp_num * 10 +(value - 48)) % 1000;
        endswitch
        WriteObject(tCustomdigits, edt, tmp_num);
    endif
    if(value == 58)
        updatetime();
        cleareditlines();
        sys_SetTimerEvent(TIMER7, timerupdate);
        sys_SetTimer(TIMER7,1000);
        tmp_num := -1;
    endif
    if(value == 59)
        var hr,mn,sec,ms, yr,mth,dy;
        sys_GetTimeVar(&hr,&mn,&sec,&ms);
        sys_GetDateVar(&yr,&mth,&dy);
        switch(edt)
            case 6:
                if(tmp_num >= 0 && tmp_num < 24)
                    hr := tmp_num;
                endif
                break;
            case 7:
                if(tmp_num >= 0 && tmp_num < 60)
                    mn := tmp_num;
                endif
                break;
            case 8:
                if(tmp_num >= 0 && tmp_num < 60)
                    sec := tmp_num;
                endif
                break;
            case 9:
                if(tmp_num >= 1 && tmp_num < 31)
                    dy := tmp_num;
                endif
                break;
            case 10:
                if(tmp_num >= 1 && tmp_num <= 12)
                    mth := tmp_num;
                endif
                break;
            case 11:
                if(tmp_num >= 2000 && tmp_num < 2100)
                    yr := tmp_num;
                endif
                break;
            case 12:
                 B1AHcapacity := tmp_num;
                 break;
            case 13:
                 B2AHcapacity := tmp_num;
                 break;
        endswitch
        if(edt <= 11)
            sys_SetTime(hr, mn, sec);
            sys_SetDate(yr, mth, dy);
            sys_to_RTC();
            sys_SetTimerEvent(TIMER7, timerupdate);
            sys_SetTimer(TIMER7,1000);
        endif
        cleareditlines();
        edt := 0;
        tmp_num := -1;
    endif
endfunc
