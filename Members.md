# MSGSTRUCT Members

The central function in the library is `msgmake()`
and the anchor for its operation is MSTSTRUCT.

## Required Supplied by Caller

* msgnum

`msgnum` is an integer indicating which message from the repository to call out.

* msgc

`msgc` is an integer indicating the number of replacement tokens, if any.

* msgv

`msgv` is a pointer to an array of replacement tokens, if any.

* msgbuf

`msgbuf` is a poiter to a buffer which will hold the resulting message string.

* msglen

`msglen` is the size of the buffer pointed to by `msgbuf`.

Upon return, `msgmake()` will have put the length of the resulting message into this field.

## Optional Supplied by Caller

* msglevel

`msglevel` indicates a SYSLOG level for supplemental functions in the library.
(e.g. `msgprint()`)

A default of zero instructs `msgmake()` to use the message severity letter.
Zero collides with SYSLOG "panic". This is intentional.


