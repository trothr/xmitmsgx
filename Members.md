# MSGSTRUCT Members

The central function in the library is `xmmake()`
and the anchor for its operation is MSGSTRUCT.

The MSGSTRUCT must be initialized by calling `xmopen()`
before calling `xmmake()`.

## Required Supplied by Caller

* msgnum

`msgnum` is an integer indicating which message from the repository to call out.

* msgc

`msgc` is an integer indicating the number of replacement tokens, zero if none.

* msgv

`msgv` is a pointer to an array of replacement tokens, if any.

* msgbuf

`msgbuf` is a pointer to a buffer which will hold the resulting message string.
The message will be null-terminated if the buffer is large enough.

* msglen

`msglen` is the size of the buffer pointed to by `msgbuf`.

Upon return, `xmmake()` will have put the length of the resulting message into this field.

## Optional Supplied by Caller

* msglevel

`msglevel` indicates a SYSLOG level for supplemental functions in the library 
(e.g. `xmprint()`). A default of zero instructs `xmmake()` to use the message
severity letter in the message source file and not alter the severity.
A `msglevel` value of zero collides with SYSLOG "panic"; this is intentional.


