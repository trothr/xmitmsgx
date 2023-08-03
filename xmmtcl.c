/*
 *
 *        Name: xmmtcl.c (C sourced interface library)
 *              Language binding layer for the Message Handler from Tcl
 *        Date: 2023-07-27 (Thu)
 */


/* requisite system headers */
#include <tcl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "xmitmsgx.h"

static struct MSGSTRUCT *tclms = NULL, *errms = NULL;

/* ------------------------------------------------------------------ */
int TclHello(ClientData clientData,Tcl_Interp*interp,int argc,char*argv[])
  {
    int i;
    printf("running %d\n",argc);
    for (i = 1; i < argc; i++) printf("'%s'\n",argv[i]);

/*  Tcl_SetResult(interp,"from-c",TCL_STATIC);                        */
/*  Tcl_SetResult(interp,"from-c",TCL_DYNAMIC);                       */
    Tcl_SetResult(interp,"from-c",TCL_VOLATILE);

    Tcl_AppendElement(interp,"second");

    printf("ran!\n");
/*  return TCL_ERROR;                                                 */
    return TCL_OK;
  }

/* ------------------------------------------------------------------ */
int TclGoodbye(ClientData clientData,Tcl_Interp*interp,int argc,char*argv[])
  {
    return TCL_OK;
  }

/* ---------------------------------------------------------- TclVersion
 */
int TclVersion(ClientData clientData,Tcl_Interp*interp,int argc,char*argv[])
  {
    int xmitmsgx_version = XMITMSGX_VERSION;
    char buffer[256];

    /* split the version integer at bytes and reformat for display    */
    sprintf(buffer,"XMITMSGX version %d.%d.%d",
       (xmitmsgx_version>>24),
      ((xmitmsgx_version>>16)&0xFF),
      ((xmitmsgx_version>>8)&0xFF));

    Tcl_SetResult(interp,buffer,TCL_VOLATILE);
    return TCL_OK;
  }

/* ------------------------------------------------------------- TclInit
 */
int TclInit(ClientData clientData,Tcl_Interp*interp,int argc,char*argv[])
  {
    int rc;
    char buffer[256];
    struct MSGSTRUCT ms0, *ms1;

    /* error out if we were called more than once (after prior init)  */
    if (tclms != NULL)
      { if (errms == NULL)
          { ms1 = malloc(sizeof(ms0));
            if (ms1 == NULL)
              { perror("malloc():"); return TCL_ERROR; }
            rc = xmopen("xmitmsgx",0,ms1);
            if (rc != 0)
              { perror("xmopen()"); free(ms1); return TCL_ERROR; }
            errms = ms1; }
        xmprint(302,0,NULL,0,errms);           /* already initialized */
        xmstring(buffer,sizeof(buffer),302,0,NULL,errms);
        Tcl_SetResult(interp,buffer,TCL_VOLATILE);
        return TCL_ERROR; }

    ms1 = malloc(sizeof(ms0));
    if (ms1 == NULL)
      { perror("malloc():");
        buffer[0] = 0x00;                                               // fixme: need to stuff the buffer
        Tcl_SetResult(interp,buffer,TCL_VOLATILE);                      // fixme: need to stuff the buffer
        return TCL_ERROR; }

    /* call xmopen() to initialize the library */
    rc = xmopen(argv[1],0,ms1);
    if (rc != 0)
      { free(ms1);
        buffer[0] = 0x00;                                               // fixme: need to stuff the buffer
        Tcl_SetResult(interp,buffer,TCL_VOLATILE);                      // fixme: need to stuff the buffer
        return TCL_ERROR; }

    /* using pfxmaj and pfxmin is definitely outside the XMITMSGX API */
    strncpy(ms1->pfxmaj,"XMM",4);
    strncpy(ms1->pfxmin,"TCL",4);
    /* this really needs to be changed to allow matching the caller */

    /* supply a return string */
    sprintf(buffer,"%x",ms1);

    /* mark this message struct active */
    tclms = ms1;

    Tcl_SetResult(interp,buffer,TCL_VOLATILE);
    return TCL_OK;
  }

/* ------------------------------------------------------------ TclPrint
 */
int TclPrint(ClientData clientData,Tcl_Interp*interp,int argc,char*argv[])
  {
    int rc, msgn;
    char buffer[256];
    struct MSGSTRUCT ms0, *ms1;


    /* error out if we were called without having been initialized    */
    if (tclms == NULL)
      { if (errms == NULL)
          { ms1 = malloc(sizeof(ms0));
            if (ms1 == NULL)
              { perror("malloc():"); return TCL_ERROR; }
            rc = xmopen("xmitmsgx",0,ms1);
            if (rc != 0)
              { perror("xmopen()"); free(ms1); return TCL_ERROR; }
            errms = ms1; }
        xmprint(301,0,NULL,0,errms);           /* not yet initialized */
        xmstring(buffer,sizeof(buffer),301,0,NULL,errms);
        Tcl_SetResult(interp,buffer,TCL_VOLATILE);
        return TCL_ERROR; }

//  for (i = 1; i < argc; i++) printf("'%s'\n",argv[i]);

    msgn = atoi(argv[1]);
    argc--; argv++;
    rc = xmprint(msgn,argc,argv,0,tclms);

//  if (rc != 0)
//      Tcl_SetResult(interp,buffer,TCL_VOLATILE);                      // fixme: need to stuff the buffer
//      return TCL_ERROR; }

    Tcl_SetResult(interp,"",TCL_VOLATILE);
    return TCL_OK;
  }

/* ----------------------------------------------------------- TclString
 */
int TclString(ClientData clientData,Tcl_Interp*interp,int argc,char*argv[])
  {
    int rc, msgn;
    char buffer[256];
    struct MSGSTRUCT ms0, *ms1;

    /* error out if we were called without having been initialized    */
    if (tclms == NULL)
      { if (errms == NULL)
          { ms1 = malloc(sizeof(ms0));
            if (ms1 == NULL)
              { perror("malloc():"); return TCL_ERROR; }
            rc = xmopen("xmitmsgx",0,ms1);
            if (rc != 0)
              { perror("xmopen()"); free(ms1); return TCL_ERROR; }
            errms = ms1; }
        xmprint(301,0,NULL,0,errms);           /* not yet initialized */
        xmstring(buffer,sizeof(buffer),301,0,NULL,errms);
        Tcl_SetResult(interp,buffer,TCL_VOLATILE);
        return TCL_ERROR; }

    msgn = atoi(argv[1]);
    argc--; argv++;
//  rc = xmprint(msgn,argc,argv,0,tclms);
    rc = xmstring(buffer,sizeof(buffer),msgn,argc,argv,tclms);

//  if (rc != 0)
//      Tcl_SetResult(interp,buffer,TCL_VOLATILE);                      // fixme: need to stuff the buffer
//      return TCL_ERROR; }

    Tcl_SetResult(interp,buffer,TCL_VOLATILE);
    return TCL_OK;
  }

/* ------------------------------------------------------------- TclQuit
 */
int TclQuit(ClientData clientData,Tcl_Interp*interp,int argc,char*argv[])
  {
    int rc;
    char buffer[256];
    struct MSGSTRUCT ms0, *ms1;

    /* error out if we were called without having been initialized    */
    if (tclms == NULL)
      { if (errms == NULL)
          { ms1 = malloc(sizeof(ms0));
            if (ms1 == NULL)
              { perror("malloc():"); return TCL_ERROR; }
            rc = xmopen("xmitmsgx",0,ms1);
            if (rc != 0)
              { perror("xmopen()"); free(ms1); return TCL_ERROR; }
            errms = ms1; }
        xmprint(301,0,NULL,0,errms);           /* not yet initialized */
        xmstring(buffer,sizeof(buffer),301,0,NULL,errms);
        Tcl_SetResult(interp,buffer,TCL_VOLATILE);
        return TCL_ERROR; }

    ms1 = tclms;
    rc = xmclose(ms1);
    if (rc != 0)
      { Tcl_SetResult(interp,buffer,TCL_VOLATILE);                      // fixme: need to stuff the buffer
        return TCL_ERROR; }

    free(ms1);
    /* mark this message struct no longer active */
    tclms = NULL;

    Tcl_SetResult(interp,"",TCL_VOLATILE);
    return TCL_OK;
  }

/* ---------------------------------------------------------------------
 *  The following routine is required by Tcl when loading this module.
 *  This must come after the various command routines have been defined.
 */
int Xmmtcl_Init(Tcl_Interp *interp)
  {
/*  printf("loading\n");                                              */
    if (Tcl_Init(interp) == TCL_ERROR) return TCL_ERROR;
//  Tcl_CreateCommand(interp,"hello",(void*)TclHello,(ClientData)NULL,(Tcl_CmdDeleteProc*)NULL);
//  Tcl_CreateCommand(interp,"goodbye",(void*)TclGoodbye,(ClientData)NULL,(Tcl_CmdDeleteProc*)NULL);
    Tcl_CreateCommand(interp,"xmversion",(void*)TclVersion,(ClientData)NULL,(Tcl_CmdDeleteProc*)NULL);
    Tcl_CreateCommand(interp,"xminit",(void*)TclInit,(ClientData)NULL,(Tcl_CmdDeleteProc*)NULL);
    Tcl_CreateCommand(interp,"xmprint",(void*)TclPrint,(ClientData)NULL,(Tcl_CmdDeleteProc*)NULL);
    Tcl_CreateCommand(interp,"xmstring",(void*)TclString,(ClientData)NULL,(Tcl_CmdDeleteProc*)NULL);
    Tcl_CreateCommand(interp,"xmquit",(void*)TclQuit,(ClientData)NULL,(Tcl_CmdDeleteProc*)NULL);
/*  printf("loaded\n");                                               */
    return TCL_OK;
  }

/*

void
    Tcl_SetByteArrayObj(Tcl_Obj *objPtr,CONST unsigned char *bytes, int length);

unsigned char *
Tcl_GetByteArrayFromObj(Tcl_Obj *objPtr,int *lengthPtr);

 */


