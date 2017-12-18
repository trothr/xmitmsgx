
#include <stdio.h>

#include "xmitmsgx.h"

int main()
  {
    int rc, msgc, msgn, msgo;
    char buffer[256], *msgv[16];
    struct MSGSTRUCT teststruct;

    msgc = 4;
    msgv[0] = "ZZZ"; msgv[1] = "AAA"; msgv[2] = "BBB"; msgv[3] = "CCC";
    msgn = 2;
    msgo = 0;

    /** report version ************************************************/
    (void) printf("test: xmitmsgx version %d.%d.%d\n",
      (XMITMSG_VERSION>>24),(XMITMSG_VERSION>>16)&0xFF,(XMITMSG_VERSION>>8)&0xFF);

    /** test msgopen() ************************************************/
    (void) printf("test: ***** msgopen() *****\n");
    rc = msgopen("xmitmsgx.msgs",0,&teststruct);
    (void) printf("test: msgopen() returned %d\n",rc);
    if (rc != 0) return rc;

    (void) printf(" max message number %d\n",teststruct.msgmax);
/*  (void) printf(" raw message 2 '%s'\n",teststruct.msgtable[2]);  */
/*  (void) printf(" msglevel %d\n",teststruct.msglevel);  */

    (void) printf(" esape '%c'\n",*teststruct.escape);
    (void) printf(" major '%s' minor '%s'\n",teststruct.pfxmaj,teststruct.pfxmin);
    (void) printf(" messages file '%s'\n",teststruct.msgfile);
    (void) printf(" locale '%s'\n",teststruct.locale);

    (void) printf(" applid '%s'\n",teststruct.applid);

/*
    int  msgopts;       
    char *applid;       
    char *caller;       
    char *prefix;       
    char *letter;       
 */



    /** test msgmake() ************************************************/
    (void) printf("test: ***** msgmake() *****\n");
    teststruct.msgnum = 599;
    rc = msgmake(&teststruct);
    (void) printf("test: msgmake() returned %d\n",rc);

    /** test msgstring() **********************************************/
    (void) printf("test: ***** msgstring() *****\n");
    rc = msgstring(buffer,sizeof(buffer)-1,msgn,msgc,msgv,&teststruct);
    (void) printf("test: msgstring() returned %d\n",rc);
    if (rc < 0) return rc;
    (void) printf(" '%s'\n",buffer);

    /** test msgprint() ***********************************************/
    (void) printf("test: ***** msgprint() *****\n");
    rc = msgprint(msgn,msgc,msgv,msgo,&teststruct);
    (void) printf("test: msgprint() returned %d\n",rc);
    if (rc < 0) return rc;

    /** W, E, S, T ****************************************************/
    (void) printf("test: W/E/S/T\n");
    rc = msgprint(4,msgc,msgv,msgo,&teststruct);
    if (rc < 0) return rc;
    rc = msgprint(8,msgc,msgv,msgo,&teststruct);
    if (rc < 0) return rc;
    rc = msgprint(12,msgc,msgv,msgo,&teststruct);
    if (rc < 0) return rc;
    rc = msgprint(16,msgc,msgv,msgo,&teststruct);
    if (rc < 0) return rc;


    /** test msgwrite() ***********************************************/
    (void) printf("test: ***** msgwrite() *****\n");
    rc = msgwrite(1,msgn,msgc,msgv,msgo,&teststruct);
    (void) printf("test: msgwrite() returned %d\n",rc);
    if (rc < 0) return rc;

    /** test msgclose() ***********************************************/
    (void) printf("test: ***** msgclose() *****\n");
    rc = msgclose(&teststruct);
    (void) printf("test: msgclose() returned %d\n",rc);
    if (rc != 0) return rc;

    (void) printf("test: ***** all tests pass *****\n");

    return 0;
  }


