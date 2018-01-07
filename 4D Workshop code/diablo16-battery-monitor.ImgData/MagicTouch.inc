//
// Added 26/06/2017 14:30:41
//
// Use 'ImageTouched' to detect 'current' object compare to values such as iWinbutton0
// to determine the object for which a touch has just been detected.
//


var x, y;
x:= touch_Get(TOUCH_GETX);
y:= touch_Get(TOUCH_GETY);

if(CurrentForm == 3 && tmp_num == -1)
//HOURS
    if(x >8 && x<38 && y>20 && y<40)
        cleareditlines();
        gfx_Hline(43, 10, 38, RED);
        edt := 6;
        ImageTouched := -1;
    endif
//MINS
    if(x >39 && x<76 && y>20 && y<40)
        cleareditlines();
        gfx_Hline(43, 41, 69, RED);
        edt := 7;
        ImageTouched := -1;
    endif
//SECS
    if(x >76 && x<110 && y>20 && y<40)
        cleareditlines();
        gfx_Hline(43, 80, 108, RED);
         edt := 8;
        ImageTouched := -1;
    endif
//days
    if(x >168 && x<198 && y>20 && y<40)
        cleareditlines();
        gfx_Hline(43, 168, 198, RED);
         edt := 9;
        ImageTouched := -1;
    endif
//month
    if(x >212 && x<242 && y>20 && y<40)
        cleareditlines();
        gfx_Hline(43, 212, 242, RED);
         edt := 10;
        ImageTouched := -1;
    endif
//year
    if(x >252 && x<312 && y>20 && y<40)
        cleareditlines();
        gfx_Hline(43, 252, 312, RED);
         edt := 11;
        ImageTouched := -1;
    endif
//bat1 cap
    if(x >32 && x<78 && y>104 && y<124)
        cleareditlines();
        gfx_Hline(127, 32, 78, RED);
         edt := 12;
        ImageTouched := -1;
    endif

//bat 2 cap
    if(x >32 && x<78 && y>164 && y<184)
        cleareditlines();
        gfx_Hline(187, 32, 78, RED);
         edt := 13;
        ImageTouched := -1;
    endif
endif

if(CurrentForm == 5)
    ActivateForm(0);
endif
