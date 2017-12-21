
#include <stdio.h>

#include "xmitmsgx.h"

int main()
  {
    int rc, msgc, msgn, msgo;
    unsigned char buffer[256], *msgv[16];
    struct MSGSTRUCT teststruct;

    msgc = 4;
    msgv[0] = "ZZZ"; msgv[1] = "AAA"; msgv[2] = "BBB"; msgv[3] = "CCC";
    msgn = 2;
    msgo = 0;

    /** report version ************************************************/
    (void) printf("test: xmitmsgx version %d.%d.%d\n",
      (XMITMSG_VERSION>>24),(XMITMSG_VERSION>>16)&0xFF,(XMITMSG_VERSION>>8)&0xFF);

    /** test xmopen() ************************************************/
    (void) printf("test: ***** xmopen() *****\n");
    rc = xmopen("xmitmsgx.msgs",0,&teststruct);
    (void) printf("test: xmopen() returned %d\n",rc);
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



    /** test xmmake() ************************************************/
    (void) printf("test: ***** xmmake() *****\n");
    teststruct.msgnum = 599;
    rc = xmmake(&teststruct);
    (void) printf("test: xmmake() returned %d\n",rc);

    /** test xmstring() **********************************************/
    (void) printf("test: ***** xmstring() *****\n");
    rc = xmstring(buffer,sizeof(buffer)-1,msgn,msgc,msgv,&teststruct);
    (void) printf("test: xmstring() returned %d\n",rc);
    if (rc < 0) return rc;
    (void) printf(" '%s'\n",buffer);

    /** test xmprint() ***********************************************/
    (void) printf("test: ***** xmprint() *****\n");
    rc = xmprint(msgn,msgc,msgv,msgo,&teststruct);
    (void) printf("test: xmprint() returned %d\n",rc);
    if (rc < 0) return rc;

    /** W, E, S, T ****************************************************/
    (void) printf("test: W/E/S/T\n");
    rc = xmprint(4,msgc,msgv,msgo,&teststruct);
    if (rc < 0) return rc;
    rc = xmprint(8,msgc,msgv,msgo,&teststruct);
    if (rc < 0) return rc;
    rc = xmprint(12,msgc,msgv,msgo,&teststruct);
    if (rc < 0) return rc;
    rc = xmprint(16,msgc,msgv,msgo,&teststruct);
    if (rc < 0) return rc;


    /** test xmwrite() ***********************************************/
    (void) printf("test: ***** xmwrite() *****\n");
    rc = xmwrite(1,msgn,msgc,msgv,msgo,&teststruct);
    (void) printf("test: xmwrite() returned %d\n",rc);
    if (rc < 0) return rc;

    /** test xmclose() ***********************************************/
    (void) printf("test: ***** xmclose() *****\n");
    rc = xmclose(&teststruct);
    (void) printf("test: xmclose() returned %d\n",rc);
    if (rc != 0) return rc;

    (void) printf("test: ***** all tests pass *****\n");

    return 0;
  }


