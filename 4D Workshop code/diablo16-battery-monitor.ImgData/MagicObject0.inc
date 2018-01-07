//
// Added 7/24/2015 11:08:43 AM
//
func rMagicObject0(var action, var object, var newVal, var *ptr)
    var cnt, i, j, actobj ;
    if (action == WRITE_MAGIC_DBYTES)
        actobj   := ptr[0] ;
        if(actobj != 's') return; // exit if the message is not for
                                  // the special left-to-right scope
        cnt      := newVal - 1 ;
        j := 1 ;
        for (i := 0; i < cnt; i++)
            //WriteObject(tScope, actobj, ptr[j++]) ;
            UpdateLRScope(ptr[j++]);
        next
    endif
    seroutX(ACK) ;
endfunc
