# XMITMSGX Functions

The central function in the library is `xmmake()`.
Other functions are derivatives for convenience and standarization.

## Routines in the Native Library

* int xmopen(,,)

Open the messages file, read it, get ready for service.
Returns: zero upon successful operation

`xmopen` takes the following arguments

filename: a pointer to unsigned character string
options: a bitmask as an integer
msgstruct: a pointer to MSGSTRUCT or NULL (NULL is not thread safe)

You should supply a MSGSTRUCT to represent your message repository.
If you supply NULL then xmopen() will attempt to use a global static
MSGSTRUCT, of which there is only one. (This is not thread safe.)

* int xmmake()

This is the central function: make a message.
All other print, string, and write functions are derivatives.
Returns: zero upon successful operation

`xmmake` takes the following argument

msgstruct: a pointer to MSGSTRUCT

While some functions will accept NULL for the MSGSTRUCT pointer,
xmmake() does not.

You must provide xmmake() with a buffer to hold the message, member
`msgbuf`, and its length, `msglen`. The latter will be replaced with
the size of the resulting message.

* int xmprint(,,,,)

Print a message, stdout or stderr depending on level/letter.
Newline automatically appended. Optionally SYSLOG the message.
Returns: number of characters printed, negative indicates error
Return value does not reflect SYSLOG effects or errors.

`xmprint` takes the following arguments

message number
count of replacement tokens (plus one for the reserved token)
array of replacement tokens (token zero is reserved)
options: a bitmask as an integer
msgstruct: a pointer to MSGSTRUCT or NULL (NULL is not thread safe)

* int xmwrite(,,,,,)

Write a message to the indicated file descriptor.
Newline automatically appended. Optionally SYSLOG the message.
Returns: number of bytes written, negative indicates error
Return value does not reflect SYSLOG effects or errors.

`xmwrite` takes the following arguments

file descriptor
message number
count of replacement tokens (plus one for the reserved token)
array of replacement tokens (token zero is reserved)
options: a bitmask as an integer
msgstruct: a pointer to MSGSTRUCT or NULL (NULL is not thread safe)

* int xmstring(,,,,,)

Build the message and put it into a string buffer. No newline.
Returns: number of bytes in string, negative indicates error

`xmstring` takes the following arguments

pointer to buffer to hold resulting message string
size of output buffer
message number
count of replacement tokens (plus one for the reserved token)
array of replacement tokens (token zero is reserved)
msgstruct: a pointer to MSGSTRUCT or NULL (NULL is not thread safe)

* int xmclose()

Close (figuratively): free common storage and reset static variables.

Returns: zero upon successful operation

`xmclose` takes the following argument

msgstruct: a pointer to MSGSTRUCT or NULL (NULL is not thread safe)







## Routines in the Rexx Interface

All Rexx calls return a condition code and an optional result string.
This is similar to the style of Rexx/Sockets.

RxXmitmsgX("VERSION")
RxXmitmsgX("INIT",library,options)
RxXmitmsgX("PRINT",messageid,arg1,arg2,arg3,...)
RxXmitmsgX("STRING",messageid,arg1,arg2,arg3,...)
RxXmitmsgX("QUIT",libobj)

Logically, there is only one Rexx function defined in the interface.
All Message Handler operations are driven as sub-functions of the one.


## Methods in the Java Interface

* method `version()`
* method `init(library,options)`
* method `print(messageid,argarray)`
* method `string(messageid,argarray)`
* method `quit(libobj)`


