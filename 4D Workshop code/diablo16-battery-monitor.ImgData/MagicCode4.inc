//
// Added 26/06/2017 16:15:27
//

//SETTINGS PAGE RUN CLOCK
var debug_count := 0;
if(CurrentForm == 3 && timeru == 1)
    updatetime();
    timeru := 0;
endif

//DEBUG PAGE UPDATE EVERY 10 SECONDS
if(CurrentForm == 5 && timeru == 1)
    timeru := 0;
    debug_count++;
    if (debug_count==10)
        debug_count := 0;
        PrintStrings(2,DEBUG_Message(),1);
    endif
endif

