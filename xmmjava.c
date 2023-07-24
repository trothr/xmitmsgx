/*
 *
 *        Name: xmmjava.c (C program source)
 *              Language binding layer for XMITMSGX for Java (JNI)
 *        Date: 2023-06-28 (Wed)
 *    See also: MessageService.java
 *
 */

#include <string.h>
#include <stdlib.h>

#include "xmitmsgx.h"

static struct MSGSTRUCT *ms = NULL;

#include <jni.h>

/* ------------------------------------------------------------- version
 * Class:     com_casita_xmitmsgx_MessageService
 * Method:    version
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
        Java_com_casita_xmitmsgx_MessageService_version
        (JNIEnv *env, jobject thisObj)
  {
    char  versionstring[256];
    int vermaj, vermin, verrev, verinc;
    int xmitmsgx_version = XMITMSGX_VERSION;

    vermaj = (xmitmsgx_version & 0xff000000) >> 24;          /* major */
    vermin = (xmitmsgx_version & 0x00ff0000) >> 16;          /* minor */
    verrev = (xmitmsgx_version & 0x0000ff00) >> 8;        /* revision */
    verinc =  xmitmsgx_version & 0x000000ff;             /* increment */

    sprintf(versionstring,"XMITMSGX version %d.%d.%d",vermaj,vermin,verrev,verinc);

    /* convert the C-string (char*) into JNI String (jstring) and return */
    return (*env)->NewStringUTF(env, versionstring);
  }

/* ---------------------------------------------------------------- init
 * Class:     com_casita_xmitmsgx_MessageService
 * Method:    init
 * Signature: (Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 * Calls:     xminit()
 */
JNIEXPORT jstring JNICALL Java_com_casita_xmitmsgx_MessageService_init
        (JNIEnv *env, jobject thisObj, jstring lib, jstring opt)
  {
    int rc;
    const char *fn;
    struct MSGSTRUCT ms0;
    jboolean iscopy;
    char initstring[256];

    /* convert the supplied Java string to a standard C string for fn */
    fn = (*env)->GetStringUTFChars(env, lib, &iscopy);

    /* allocate memory for a messages struct and point to it          */
    ms = malloc(sizeof(ms0));
    /* if the static pointer is non-null then we have a problem       */
    if (ms == NULL)
      { jclass Exception = (*env)->FindClass(env,"java/lang/Exception");
        (*env)->ThrowNew(env,Exception,"malloc() failed"); return NULL; }

    /* open the messages file and populate the messages struct */
    rc = xmopen((char*)fn,0,ms);
    if (rc != 0)
      { jclass Exception = (*env)->FindClass(env,"java/lang/Exception");
        free(ms);
        (*env)->ThrowNew(env,Exception,"xmopen() failed"); return NULL; }

    /* using pfxmaj and pfxmin is definitely outside the XMITMSGX API */
    strncpy(ms->pfxmaj,"XMM",4);
    strncpy(ms->pfxmin,"JNI",4);
    /* this really needs to be changed to allow matching the caller */

    sprintf(initstring,"%x",ms);

    /* convert the C-string (char*) into JNI String (jstring) and return */
    return (*env)->NewStringUTF(env, initstring);
  }


/* --------------------------------------------------------------- print
 * Class:     com_casita_xmitmsgx_MessageService
 * Method:    print
 * Signature: (I[Ljava/lang/String;)Ljava/lang/String;
 * Calls:     xmprint()
 */
JNIEXPORT jstring JNICALL Java_com_casita_xmitmsgx_MessageService_print
        (JNIEnv *env, jobject thisObj, jint mn, jobjectArray arg)
  {
    int n, i, j, rc;
    char failstring[256], *msgv[256];
    jobject jojo;
    jboolean iscopy;

//  sprintf(failstring,"not implemented");

    /* number of replacement tokens is indicated by size of the array */
    n = (*env)->GetArrayLength(env,arg);

    msgv[0] = "java";           /* arbitrary - token zero is reserved */
    j = 1;
    for (i = 0; i < n; i++)
      { jojo = (*env)->GetObjectArrayElement(env,arg,i);
        msgv[j] = (*env)->GetStringUTFChars(env, jojo, &iscopy);
        j++; }

    n++;          /* increment token count to handle our quirky logic */
    rc = xmprint(mn,n,msgv,0,ms);        /* print the selectd message */


    /* if we got an error then try to throw it the expected Java way  */
    if (rc < 0)
      { jclass Exception = (*env)->FindClass(env,"java/lang/Exception");
        (*env)->ThrowNew(env,Exception,"xmprint() failed"); return NULL; }

    /* convert the C-string (char*) into JNI String (jstring) and return */
    failstring[0] = 0x00;
    return (*env)->NewStringUTF(env, failstring);
  }

/* -------------------------------------------------------------- string
 * Class:     com_casita_xmitmsgx_MessageService
 * Method:    string
 * Signature: (I[Ljava/lang/String;)Ljava/lang/String;
 * Calls:     xmstring()
 */
JNIEXPORT jstring JNICALL Java_com_casita_xmitmsgx_MessageService_string
        (JNIEnv *env, jobject thisObj, jint mn, jobjectArray arg)
  {
    int n, i, j, rc;
    char msgstring[256], *msgv[256];
    jobject jojo;
    jboolean iscopy;


    /* number of replacement tokens is indicated by size of the array */
    n = (*env)->GetArrayLength(env,arg);

    msgv[0] = "java";           /* arbitrary - token zero is reserved */
    j = 1;
    for (i = 0; i < n; i++)
      { jojo = (*env)->GetObjectArrayElement(env,arg,i);
        msgv[j] = (*env)->GetStringUTFChars(env, jojo, &iscopy);
        j++; }

    n++;          /* increment token count to handle our quirky logic */
    rc = xmstring(msgstring,sizeof(msgstring),mn,n,msgv,ms);

    /* if we got an error then try to throw it the expected Java way  */
    if (rc < 0)
      { jclass Exception = (*env)->FindClass(env,"java/lang/Exception");
        (*env)->ThrowNew(env,Exception,"xmstring() failed");   }

    /* convert the C-string (char*) into JNI String (jstring) and return */
    return (*env)->NewStringUTF(env, msgstring);
  }

/* ---------------------------------------------------------------- quit
 * Class:     com_casita_xmitmsgx_MessageService
 * Method:    quit
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 * Calls:     xmclose()
 */
JNIEXPORT jstring JNICALL Java_com_casita_xmitmsgx_MessageService_quit
        (JNIEnv *env, jobject thisObj, jstring obj)
  {
    int rc;
    const char *xo;
    char failstring[256];
    jboolean iscopy;

//  sprintf(failstring,"not implemented");

    /* if the static pointer is non-null then we have a problem       */
    if (ms == NULL)
      { jclass Exception = (*env)->FindClass(env,"java/lang/Exception");
        (*env)->ThrowNew(env,Exception,"quit method failed"); return NULL; }

    /* convert the supplied Java string to a standard C string for fn */
    xo = (*env)->GetStringUTFChars(env, obj, &iscopy);

    /* do an orderly shutdown of the library */
    rc = xmclose(ms);

    /* convert the C-string (char*) into JNI String (jstring) and return */
    failstring[0] = 0x00;
    return (*env)->NewStringUTF(env, failstring);
  }


