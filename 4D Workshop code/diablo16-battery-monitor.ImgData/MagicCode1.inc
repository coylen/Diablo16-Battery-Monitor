//
// Added 7/24/2015 9:52:19 AM
//
// MagicCode1
//
var b;
sys_SetTimerEvent(TIMER7, 0);
if(CurrentForm == 0)
    WriteObject(tUserButton, 0, 1);
endif

if(CurrentForm == 1)
    WriteObject(tUserButton, 11, 1);
    SendReport(REPORT_OBJ, tUserButton, 5, 99);

    if (img_GetWord(hndl, iUserbutton5, IMAGE_INDEX) == 2)
            SendReport(REPORT_OBJ, tUserButton, 5,img_GetWord(hndl, iUserbutton5, IMAGE_TAG2));
            SendReport(REPORT_OBJ, tUserButton, 5, 55) ;
            // Create empty initial scope
            gfx_RectangleFilled(16, 16, 208, 166, BLACK) ;
            //gfx_Hline(52 + 75, 40, 289, YELLOW) ;
            Graticule(16, 16, 208, 166, 8,  25, 0x0280) ;

            // draw and update scope
            Graticule(16, 16, 208, 166, 8,  25, 0x0280) ;
            //TODO: need to update object names after demo code
            gfx_Scope(16, 192, 166, 96, 2, 100, BLACK,
            old_y1, new_y1, LIME, old_y2, new_y2, ORANGE, 0,0,0, 0,0,0 ) ;
            //gfx_Hline(52 + 75, 40, 289, YELLOW) ;
    endif
    if (img_GetWord(hndl, iUserbutton6, IMAGE_INDEX) == 2)
            // Create empty initial scope
            gfx_RectangleFilled(16, 16, 208, 166, YELLOW) ;
            SendReport(REPORT_OBJ, tUserButton, 5, 66);
    endif
endif

//SETTINGS PAGE
if(CurrentForm == 3)
    WriteObject(tUserButton, 18, 1);
    updatetime();
    sys_SetTimerEvent(TIMER7, timerupdate);
    sys_SetTimer(TIMER7,1000);
endif

//SLEEP PAGE
if(CurrentForm == 4)
    gfx_Contrast(0);
endif

//DEGBUG PAGE
if(CurrentForm == 5 && dummy ==0)
    PrintStrings(2,DEBUG_Message(),1);
    sys_SetTimerEvent(TIMER7, timerupdate);
    sys_SetTimer(TIMER7,1000);
endif



