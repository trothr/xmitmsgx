/*
 *
 *        Name: xmitmsgx.c (C program source)
 *              library of functions for XMITMSGX package
 *      Author: Rick Troth, rogue programmer
 *        Date: 2017-Nov-25 (Sat) Thanksgiving 2017
 *
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <syslog.h>
#include <errno.h>

#include "xmitmsgx.h"

struct MSGSTRUCT *msglobal = NULL, msstatic;

/* ---------------------------------------------------------------- OPEN
 * Open the messages file, read it, get ready for service.
 * Returns: zero upon successful operation
 * The VM/CMS counterpart does 'SET LANG' to load the messages file.
 */
int msgopen(const char*file,int opts,struct MSGSTRUCT*ms)
  {
    int rc, fd;
    struct stat statbuf;
    char filename[256]; int filesize;
    int memsize, i;
    char *p, *q, *escape, *locale;

    /* NULL struct pointer means to use global static storage
     * unless it was already used. */
/*  if (ms == NULL && msglobal != NULL) return EINVAL;  */
    if (ms == NULL && msglobal != NULL) return EBUSY;
    if (ms == NULL) ms = msglobal = &msstatic;

    ms->msgdata = NULL;
    ms->msgtable = NULL;

    /* look for the message repository */
    ms->msgfile = NULL;
    (void) memset(ms->locale,0x00,sizeof(ms->locale));
    (void) memset(ms->applid,0x00,sizeof(ms->applid));
    (void) strncpy(filename,file,sizeof(filename)-1);
    filename[sizeof(filename)-1] = 0x00;
    rc = stat(filename,&statbuf);

    if (rc != 0) {
      (void) snprintf(filename,sizeof(filename)-1,
        "%s.msgs",file);
      rc = stat(filename,&statbuf); }

    /* if that didn't work then try LANG environment variable */
    if (rc != 0 && (locale = getenv("LANG")) != NULL && *locale != 0x00) {
      (void) strncpy(ms->locale,locale,sizeof(ms->locale)-1);
      (void) snprintf(filename,sizeof(filename)-1,
        "/usr/share/locale/%s/%s.msgs",ms->locale,file);
        rc = stat(filename,&statbuf); }
    /* if that didn't work then try removing dotted encoding */
    if (rc != 0) {
      for (p = ms->locale; *p != 0x00 && *p != '.'; p++);
      if (*p != 0x00) { *p = 0x00;
      (void) snprintf(filename,sizeof(filename)-1,
        "/usr/share/locale/%s/%s.msgs",ms->locale,file);
        rc = stat(filename,&statbuf); } }


    /* if that didn't work then try LC_CTYPE environment variable */
    if (rc != 0 && (locale = getenv("LC_CTYPE")) != NULL && *locale != 0x00) {
      (void) strncpy(ms->locale,locale,sizeof(ms->locale)-1);
      (void) snprintf(filename,sizeof(filename)-1,
        "/usr/share/locale/%s/%s.msgs",ms->locale,file);
        rc = stat(filename,&statbuf); }
    /* if that didn't work then try removing dotted encoding */
    if (rc != 0) {
      for (p = ms->locale; *p != 0x00 && *p != '.'; p++);
      if (*p != 0x00) { *p = 0x00;
      (void) snprintf(filename,sizeof(filename)-1,
        "/usr/share/locale/%s/%s.msgs",ms->locale,file);
        rc = stat(filename,&statbuf); } }

    /* if that didn't work then try LOCALE environment variable */
    if (rc != 0 && (locale = getenv("LOCALE")) != NULL && *locale != 0x00) {
      (void) strncpy(ms->locale,locale,sizeof(ms->locale)-1);
      (void) snprintf(filename,sizeof(filename)-1,
        "/usr/share/locale/%s/%s.msgs",ms->locale,file);
        rc = stat(filename,&statbuf); }
    /* if that didn't work then try removing dotted encoding */
    if (rc != 0) {
      for (p = ms->locale; *p != 0x00 && *p != '.'; p++);
      if (*p != 0x00) { *p = 0x00;
      (void) snprintf(filename,sizeof(filename)-1,
        "/usr/share/locale/%s/%s.msgs",ms->locale,file);
        rc = stat(filename,&statbuf); } }

    /* if we can't find the file then return the best error we know */
    if (rc != 0) { if (errno != 0) return errno; else return rc; }


    /* allocate memory for the messages file */
    filesize = statbuf.st_size;    /* total size, in bytes */
    memsize = filesize + sizeof(filename) + 16;   /* add and pad */
    ms->msgdata = malloc(memsize);
    if (ms->msgdata == NULL) { if (errno != 0) return errno; else return ENOMEM; }

    /* open the message repository */
    rc = fd = open(filename,O_RDONLY);
    if (rc < 0) {
      (void) free(ms->msgdata);
      if (errno != 0) return errno; else return EBADF; }

    /* read the file into the buffer */
    rc = read(fd,ms->msgdata,filesize);
    if (rc < 0) {
      (void) free(ms->msgdata);
      if (errno != 0) return errno; else return EBADF; }
    (void) close(fd);

    /* put filename at end of buffer */
    p = &ms->msgdata[rc]; *p++ = 0x00;
    (void) strncpy(p,filename,sizeof(filename)-1);
    ms->msgfile = p;

    /* allocate the message array - sizing needs work */
    ms->msgtable = malloc(163840);
    if (ms->msgtable == NULL) {
      (void) free(ms->msgdata);
      if (errno != 0) return errno; else return ENOMEM; }
    (void) memset(ms->msgtable,0x00,163840);

    /* parse the file */
    p = ms->msgdata;
    ms->msgmax = 0;
    while (*p != 0x00) {

      /* mark off and measure this line */
      q = p; i = 0;
      while (*p != 0x00 && *p != '\n') { p++; i++; }
      if (*p == '\n') *p++ = 0x00;

      /* skip comments */
      if (*q == '*' || *q == '#') continue;

      /* look for escape character */
      if (*q != ' ' && (*q < '0' || *q > '9')) { ms->escape = q; continue; }

      /* ignore short lines */
      if (i < 10) continue;

      /* parse this line */
      q[4] = 0x00;
      i = atoi(q);
      if (i > ms->msgmax) ms->msgmax = i;
      ms->msgtable[i] = &q[8];

      }


    /* FIXME: write xmsgbase() and use it instead */
    p = (char*) basename(ms->msgfile);
    (void) strncpy(ms->applid,p,sizeof(ms->applid)-1);
    p = ms->applid;
    while (*p != 0x00 && *p != '.') p++; *p = 0x00;


    /* FIXME */
    (void) sprintf(ms->pfxmaj,"TST");
    (void) sprintf(ms->pfxmin,"MSG");
    p = ms->applid;
    for (i = 0; i < 3 && *p != 0x00; i++) ms->pfxmaj[i] = toupper(*p++);
    ms->pfxmaj[i] = 0x00;
    for (i = 0; i < 3 && *p != 0x00; i++) ms->pfxmin[i] = toupper(*p++);
    ms->pfxmin[i] = 0x00;


    /* handle SYSLOG and record other options */
    ms->msgopts = opts;
    if (ms->msgopts & MSGFLAG_SYSLOG) {
      /* figure out syslog identity */
      openlog(ms->applid,LOG_PID,LOG_USER);
      }

/*
    char *caller;
    char *prefix;
 */

    /* force clear other elements of the struct */
    ms->msgnum = 0;
    ms->msgc = 0;   ms->msgv = NULL;
    ms->msgbuf = NULL;   ms->msglen = 0;   ms->msgtext = NULL;

    ms->msglevel = 0;
    ms->msgfmt = 0;   ms->msgline = 0;   /* neither implemented */
    ms->letter = NULL;

    /* return success */
    return 0;
  }

/* ---------------------------------------------------------------- MAKE
 * Central function: make a message.
 * All other print, string, and write functions are derivatives.
 * Returns: zero upon successful operation
 */
int msgmake(struct MSGSTRUCT*ms)
  {
    int  rc, i, j;
    char *p, *q;

    if (ms == NULL) return EINVAL; /* invalid argument */
    if (ms->msgnum <= 0) return EINVAL; /* invalid argument */
    if (ms->msgnum > ms->msgmax) return EINVAL; /* invalid argument */

/* (void) printf("msgmake() msgnum %d\n",ms->msgnum); */
    /* NULL pointer indicates an undefined message */
    if (ms->msgtable[ms->msgnum] == NULL) return ENOENT;   /* no entry */


    p = ms->letter = ms->msgtable[ms->msgnum];

    i = rc = snprintf(ms->msgbuf,ms->msglen,"%s%s%03d%c ",
      ms->pfxmaj,ms->pfxmin,ms->msgnum,*p);

    p++; if (*p == ' ') p++;
    ms->msgtext = p;

    while (i < ms->msglen)
      {

        if (*p == *ms->escape)
          {
            p++;
            j = 0;
            while ('0' <= *p && *p <= '9')
              {
                j = j * 10;
                j = j + (*p & 0x0f);
                p++;
              }
            if (j < ms->msgc) q = ms->msgv[j];
                         else q = "";
            while (*q != 0x00 && i < ms->msglen)
              {
                ms->msgbuf[i] = *q;
                i++; q++;
              }
            ms->msgbuf[i] = *p;
            if (*p == 0x00) break;
          } else {
            ms->msgbuf[i] = *p;
            if (*p == 0x00) break;
            i++; p++;
          }
      }
    ms->msglen = i;


    /* optional syslogging */
    if (ms->msgopts & MSGFLAG_SYSLOG) {
      if (ms->msglevel == 0) {
        switch (*ms->letter) {
/*           MSGLEVEL_DEBUG:                    LOG_DEBUG         7 */
        case MSGLEVEL_INFO:      ms->msglevel = LOG_INFO;     /* I6 */ break;
        case MSGLEVEL_REPLY:     ms->msglevel = LOG_NOTICE;   /* R5 */ break;
        case MSGLEVEL_WARNING:   ms->msglevel = LOG_WARNING;  /* W4 */ break;
        case MSGLEVEL_ERROR:     ms->msglevel = LOG_ERR;      /* E3 */ break;
        case MSGLEVEL_SEVERE:    ms->msglevel = LOG_CRIT;     /* S2 */ break;
        case MSGLEVEL_TERMINAL:  ms->msglevel = LOG_ALERT;    /* T1 */ break;
/*           MSGLEVEL_EMERG:                    LOG_EMERG         0 */
/*                                              INTERNAL_NOPRI      */
        default:                 ms->msglevel = LOG_INFO;              break;
                           } }

                                      }

    /* reset transient elements of the struct */
/*
    ms->msgnum = 0;
    ms->msglevel = 0;
    ms->letter = NULL;
 */
    /* why reset them? caller might want them */

    return 0;
  }

/* --------------------------------------------------------------- PRINT
 * Print a message, stdout or stderr depending on level/letter.
 * Newline automatically appended. Optionally SYSLOG the message.
 * Returns: number of characters printed, negative indicates error
 * Return value does not reflect SYSLOG effects or errors.
 */
int msgprint(int msgnum,int msgc,char*msgv[],int msgopts,struct MSGSTRUCT*ms)
  {
    int  rc;
    struct MSGSTRUCT ts;
    char buffer[256];

    /* NULL message struct means use the static common struct */
    if (ms == NULL) ms = msglobal;
    if (ms == NULL) return EINVAL;
    (void) memcpy(&ts,ms,sizeof(ts));
    ms = &ts;

    ms->msgbuf = buffer;          /* output buffer supplied by caller */
    ms->msglen = sizeof(buffer) - 1;         /* size of output buffer */

    ms->msgnum = msgnum;        /* message number specified by caller */
    ms->msgc = msgc;                   /* count of tokens from caller */
    ms->msgv = msgv;                       /* token array from caller */
    ms->msglevel = 0;             /* zero means set level from letter */

    rc = msgmake(ms);                             /* make the message */
    if (rc < 0) return rc;
    if (rc > 0) return 0 - rc;

/*  syslog(ms->msglevel,"%s",ms->msgbuf); */

    if (ms->msglevel > 5)
    rc = fprintf(stdout,"%s\n",ms->msgbuf);
    else
    rc = fprintf(stderr,"%s\n",ms->msgbuf);

    return rc;
  }

/* --------------------------------------------------------------- WRITE
 * Write a message to the indicated file descriptor.
 * Newline automatically appended. Optionally SYSLOG the message.
 * Returns: number of bytes written, negative indicates error
 * Return value does not reflect SYSLOG effects or errors.
 */
int msgwrite(int fd,int msgnum,int msgc,char*msgv[],int msgopts,struct MSGSTRUCT*ms)
  {
    int  rc;
    struct MSGSTRUCT ts;
    char buffer[256];

    /* NULL message struct means use the static common struct */
    if (ms == NULL) ms = msglobal;
    if (ms == NULL) return EINVAL;
    (void) memcpy(&ts,ms,sizeof(ts));
    ms = &ts;

    ms->msgbuf = buffer;          /* output buffer supplied by caller */
    ms->msglen = sizeof(buffer) - 1;         /* size of output buffer */

    ms->msgnum = msgnum;        /* message number specified by caller */
    ms->msgc = msgc;                   /* count of tokens from caller */
    ms->msgv = msgv;                       /* token array from caller */
    ms->msglevel = 0;             /* zero means set level from letter */

    rc = msgmake(ms);                             /* make the message */
    if (rc < 0) return rc;
    if (rc > 0) return 0 - rc;

/*  syslog(ms->msglevel,"%s",ms->msgbuf); */

    ms->msgbuf[ms->msglen++] = '\n';
    rc = write(fd,ms->msgbuf,ms->msglen);

    return rc;
  }

/* -------------------------------------------------------------- STRING
 * Build the message and put it into a string buffer. No newline.
 * Returns: number of bytes in string, negative indicates error
 */
int msgstring(char*output,int outlen,int msgnum,int msgc,char*msgv[],struct MSGSTRUCT*ms)
  {
    int  rc;
    struct MSGSTRUCT ts;

    /* NULL message struct means use the static common struct */
    if (ms == NULL) ms = msglobal;
    if (ms == NULL) return EINVAL;
    (void) memcpy(&ts,ms,sizeof(ts));
    ms = &ts;

    ms->msgbuf = output;          /* output buffer supplied by caller */
    ms->msglen = outlen;                     /* size of output buffer */
    ms->msgnum = msgnum;        /* message number specified by caller */
    ms->msgc = msgc;                   /* count of tokens from caller */
    ms->msgv = msgv;                       /* token array from caller */
    ms->msglevel = 0;             /* zero means set level from letter */

    rc = msgmake(ms);                             /* make the message */
    if (rc < 0) return rc;
    if (rc > 0) return 0 - rc;

    return ms->msglen;   /* normal return is length of message string */
  }

/* --------------------------------------------------------------- CLOSE
 * Close (figuratively): free common storage and reset static variables.
 * Returns: zero upon successful operation
 */
int msgclose(struct MSGSTRUCT*ms)
  {
    /* NULL struct pointer means to use global static storage */
    if (ms == NULL && msglobal == NULL) return EINVAL;
    if (ms == NULL) { ms = msglobal; msglobal = NULL; }

    if (ms->msgdata != NULL) (void) free(ms->msgdata);
    if (ms->msgtable != NULL) (void) free(ms->msgtable);
    if (ms->msgopts & MSGFLAG_SYSLOG) closelog();

    (void) memset(ms->pfxmaj,0x00,sizeof(ms->pfxmaj));
    (void) memset(ms->pfxmin,0x00,sizeof(ms->pfxmin));
    (void) memset(ms->locale,0x00,sizeof(ms->locale));

    return 0;
  }


