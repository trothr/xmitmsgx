/*
 *        Name: xmitmsg.c (C program source)
 *              "transmit message" for POSIX
 *              read and present enumerated messages from the repository
 *      Author: Rick Troth, Rice University, Information Systems
 *              Rick Troth, rogue programmer, La Casita en la Esquina
 *        Date: 1993-May-24
 *              2003-Sep-03
 *     Rewrite: 2023-03-15 (Wednesday)
 */

#include <stdlib.h>

#define MSGMAX 16

#include "xmitmsgx.h"

/* development */
#include <stdio.h>

/* ------------------------------------------------------------------ */
int main(int argc,char*argv[])
  {
    int rc, msgn, msgc, i, j, n;
    unsigned char *applid, *caller, *letter, buffer[256], *msgv[MSGMAX];

    applid = "xmitmsgx";
    letter = caller = "";

    for (n = 1; n < argc && argv[n][0] == '-'; n++)
      {
        switch (argv[n][2])
          {
            case 'a':   if (++n >= argc) break;
                        applid = argv[n];
                        break;
            case 'c':   if (++n >= argc) break;
                        caller = argv[n];
                        break;
            case 'l':   if (++n >= argc) break;
                        letter = argv[n];
                        break;
            default:
                        break;
          }
      }

//  msgn = atoi(argv[n++]);
    msgc = argc - n;

    /* Ensure that we have enough arguments.                          */
    if (msgc < 1)
      { xmopen("xmitmsgx",0,NULL);
/*      (void) xmprint(386,0,NULL,0,NULL);   ** "Missing operand(s)." */
        (void) xmprint(405,0,NULL,0,NULL);
        (void) xmclose(NULL);
        return 1; }     /* return code was originally 24 to match CMS */

    /* Open the messages file, read it, get ready for service.        */
    rc = xmopen(applid,0,NULL);
    /* But if that failed try to report *why* it failed.              */
    if (rc != 0)
      { xmopen("xmitmsgx",0,NULL);
        msgv[1] = applid; msgc = 2;
        (void) xmprint(rc,msgc,msgv,0,NULL);
        (void) xmclose(NULL);
        return 1; }

    msgn = atoi(argv[n++]);
    msgc = argc - n;

    /* Get message number and queue-up any replacement tokens.        */
    msgv[0] = applid;
    i = 1; j = n;
    while (i < MSGMAX && j < argc) msgv[i++] = argv[j++];
    msgc = i;

    /* Print stdout or stderr, depending on level, optionally syslog. */
    rc = xmprint(msgn,msgc,msgv,0,NULL);
    if (rc < 0)
      { (void) xmclose(NULL);
        return rc; }

    /* Clear the message repository struct. */
    rc = xmclose(NULL);
    if (rc != 0) return rc;

    return 0;
  }


/*

XMITMSG --applid applid [options] msgnumber [substitution [substitution [...]]]

--FORmat msgfmt
--LINE number

--LETter letter
--CALLER caller

--HEADer # This is the default.
--NOHEADer

--SYSLANG lang

--APPLID applid

         The message header consists of:

         xxxmmmnnns
         xxxmmmnnnns

         Where:

         xxx
                  specifies the application ID, DMS for CMS
         mmm
                  specifies the CALLER name
         nnn or nnnn
                  specifies the message number
         s
                  specifies the severity code

3. You can use XMITMSG from CMS to display a repository message on your
   screen; this is useful when you want to verify the content of a repository.

4. For more information on how to issue messages from assembler programs, see
   the APPLMSG macro in z/VM: CMS Macros and Functions Reference.

08750101E Attempt to divide by &1 is invalid
08750201E Attempt to &2 by &1 is invalid
08760101E Error &1.  rc = &3.
   | | ||
   | | ||________severity code
   | | |______line of message
   | |_____format of message
   |___number of message

Messages DMS813E and DMS814E can be displayed (depending on what you have
specified for the ERRMSG, DISPLAY/NODISPLAY, and VAR options) if XMITMSG
encounters an error when it attempts to retrieve the requested message.

*  DMS065E option option specified twice [RC=24]
*  DMS066E option1 and option2 are conflicting options [RC=24]
*  DMS080E Invalid numtype number [RC=24]
*  DMS109S Virtual storage capacity exceeded [RC=104]
*  DMS405E Invalid or missing message number [RC=24]
*  DMS408E Number of substitutions exceeds 20 [RC=24]
*  DMSnnnE Number of substitutions exceeds 9 [RC=24]
*  DMS631E XMITMSG must be invoked from an EXEC 2 or REXX exec or as a CMS command [RC=24]
*  DMS813E repos repository not found, message nnnn cannot be retrieved [RC=16]
*  DMS814E Message number nnnn, format nn, line nn, was not found; it was called from routine in application applid [RC=12]
*  DMS2045E Invalid substitution value - blank or parenthesis [RC=24]

 */

