//DEBUG CLASS
//include in all other needed sections
//write to DEBUG_Buffer using
var DEBUG_Providers[50];
var DEBUG_Provider_Number := 0;
var DEBUG_Text[250], uniptr[500];
var demodebug[50];
var demodebug2[50];
var bob:= 0;
var l,*b;




func DEBUG_Message()
    var n, ptr;
	DEBUG_Text[0]:=0;
    ptr := str_Ptr(DEBUG_Text);
    for(n := 0; n < DEBUG_Provider_Number; n++)
        str_Cat(ptr, DEBUG_Providers[n]());
    next
    ptr := str_Ptr(DEBUG_Text);
	ANSI2Uni(str_Ptr(DEBUG_Text), uniptr);
	return uniptr;
endfunc
func DEBUG_Register(var debug_function)
    DEBUG_Providers[DEBUG_Provider_Number] := debug_function;
    DEBUG_Provider_Number++;
endfunc

func testcase()
    DEBUG_Register(demoprovider);
    DEBUG_Register(demoprovider2);
    DEBUG_Message();
endfunc

func demoprovider()
    to (demodebug); print("Does this work as intended \n");
    var pg;
    pg := str_Ptr(demodebug);
    return pg;
endfunc

func demoprovider2()
    to (demodebug2); print("Does this work as intended as well ", bob, "\n");
	bob++;
    var pg;
    pg := str_Ptr(demodebug2);
    return pg;
endfunc

func ANSI2Uni(var *ansi, var *uni)
    var i, ch ;
    repeat
        ch := str_GetByte(ansi++) ;
        uni[i++] := ch ;
    until (!ch) ;

endfunc
