# XMITMSGX Functions

The central function in the library is `xmmake()`. 
Other functions are derivatives for convenience and standarization. 

## Routines in the Library

* int xmopen(,,)

Open the messages file, read it, get ready for service.

Returns: zero upon successful operation

`xmopen` takes the following arguments

filename: pointer to unsigned character string
options: bitmask as an integer
pointer to MSGSTRUCT or NULL

You should supply a MSGSTRUCT to represent your message repository.
If you supply NULL then xmopen() will attempt to use a global static
MSGSTRUCT, of which there is only one. (This is not thread safe.)

* int xmmake()

This is the central function: make a message.

All other print, string, and write functions are derivatives.

Returns: zero upon successful operation

`xmmake` takes the following argument

pointer to MSGSTRUCT or NULL

If you supply NULL then xmmake() will attempt to use a global static
MSGSTRUCT, if it was provisioned by xmopen(). This is not thread safe.

* int xmprint(,,,,)

Print a message, stdout or stderr depending on level/letter.

Newline automatically appended. Optionally SYSLOG the message.

Returns: number of characters printed, negative indicates error

Return value does not reflect SYSLOG effects or errors.

`xmprint` takes the following arguments

message number
count of replacement tokens
array of replacement tokens
options
pointer to MSGSTRUCT or NULL

* int xmwrite(,,,,,)

Write a message to the indicated file descriptor.

Newline automatically appended. Optionally SYSLOG the message.

Returns: number of bytes written, negative indicates error

Return value does not reflect SYSLOG effects or errors.

`xmwrite` takes the following arguments

file descriptor
message number
count of replacement tokens
array of replacement tokens
options
pointer to MSGSTRUCT or NULL

* int xmstring(,,,,,)

 * Build the message and put it into a string buffer. No newline.
 * Returns: number of bytes in string, negative indicates error

`xmstring` takes the following arguments

pointer to buffer to hold resulting message string
size of output buffer
message number
count of rep
array of rep
pointer to MSGSTRUCT or NULL

* int xmclose()

Close (figuratively): free common storage and reset static variables.

Returns: zero upon successful operation

`xmclose` takes the following argument

pointer to MSGSTRUCT or NULL


