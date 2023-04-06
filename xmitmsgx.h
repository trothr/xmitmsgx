/*
 *
 *        Name: xmitmsgx.h, "transmit message" for POSIX
 *              read and emit enumerated messages from the repository
 *      Author: Rick Troth, rogue programmer
 *        Date: 2014-May-01 (Thu)
 *              2017-Nov-23 (Thu) Thanksgiving 2017
 *              2023-03-16 (Thu)
 */

#ifndef _XMITMSGX_H
#define _XMITMSGX_H

/* xmitmsgx-2.1.1 */
#define  XMITMSGX_VERSION  (((2) << 24) + ((1) << 16) + ((1) << 8) + (0))

/* priorities (these are ordered)                                     */
/*      MSGLEVEL_DEBUG           LOG_DEBUG   7 debug-level messages, not used here */
#define MSGLEVEL_INFO     'I' /* LOG_INFO    6 informational */
#define MSGLEVEL_REPLY    'R' /* LOG_NOTICE  5 normal but significant condition, ATTENTION */
#define MSGLEVEL_WARNING  'W' /* LOG_WARNING 4 warning conditions */
#define MSGLEVEL_ERROR    'E' /* LOG_ERR     3 error conditions */
#define MSGLEVEL_SEVERE   'S' /* LOG_CRIT    2 critical conditions */
#define MSGLEVEL_TERMINAL 'T' /* LOG_ALERT   1 action must be taken immediately */
/*      MSGLEVEL_EMERG           LOG_EMERG   0 reserved */

/* the following are used by derivative functions, not by xmmake() itself */
#define  MSGFLAG_SYSLOG   0x01   /* used by xmopen() to set-up logging */
#define  MSGFLAG_NOLOG    0x02   /* used by xmprint() and xmwrite() to skip logging */
#define  MSGFLAG_NOCODE   0x04   /* means message text only, good for decorations */
#define  MSGFLAG_NOPRINT  0x08   /* implies log only */
/* what about time stamp? logging automtically has time stamping */

#define  MSGERR_NOLIB     813
#define  MSGERR_NOMSG     814

typedef struct MSGSTRUCT
  {
    int  msgnum;                /* message number */
    int  msgc;                  /* count of replacement tokens */
    unsigned char **msgv;       /* vector of replacement tokens */
    unsigned char *msgbuf;      /* buffer supplied by caller */
    int  msglen;                /* buffer size on input, msg size on return */
    unsigned char *msgtext;     /* offset past msg code/header */

    int  msgfmt;        /* message format number (for future use) */
    int  msgline;       /* message line number (for future use, zero means all lines) */
    int  msglevel;      /* message level/serverity (zero means use the letter in the file) */
    int  msgopts;       /* set by xmopen(), sometimes overridden for xmmake() */

    /* the following are probably not for external use */
    unsigned char *caller;       /* default is getenv("LOGNAME") roughly, msgu */
    unsigned char *prefix;       /* default is applid[0..2]||caller[0..2] */
    unsigned char *letter;       /* default taken from message file */

    /* the following are filled in by xmopen() not for external use */
    int  msgmax;                /* highest message number in table */
    unsigned char **msgtable;   /* array of messages (allocated memory) */
    unsigned char *msgdata;     /* messages file content (allocated memory) */
    unsigned char *msgfile;     /* name of message file found (for reference and debugging) */
    unsigned char *escape;      /* the escape character (for reference and debugging) */

    unsigned char  pfxmaj[4];   /* truncated up-cased applid/major */
    unsigned char  pfxmin[4];   /* truncated up-cased caller/minor */
    unsigned char  locale[32];  /* possibly truncated to match the nearest file found */
    unsigned char  applid[32];  /* default is basename of messages file, used as SYSLOG identity */

  } MSGSTRUCT;

/* Open the messages file, read it, get ready for service. */
extern int xmopen(unsigned char*,int,struct MSGSTRUCT*);
/* filename, opts, MSGSTRUCT */
/* Specify a syslog ident via applid in MSGSTRUCT. */
/* specify a syslog facility via optional MSGSTRUCT */
// calls xminit()
// may call openlog(char*ident,int option,int facility)

/* This is the heart of the utility. */
extern int xmmake(struct MSGSTRUCT*);

/* Print to stdout or stderr depending on level, optionally syslog. */
extern int xmprint(int,int,unsigned char*[],int,struct MSGSTRUCT*);
/* msgnum, msgc, msgv, opts */

/* Write to file descriptor, optionally syslog. */
extern int xmwrite(int,int,int,unsigned char*[],int,struct MSGSTRUCT*);
/* fd, msgnum, msgc, msgv, opts */

/* Generate a message and store it as a string. */
extern int xmstring(unsigned char*,int,int,int,unsigned char*[],struct MSGSTRUCT*);
/* output, outlen, msgnum, msgc, msgv */

/* Clear the message repository struct. */
extern int xmclose(struct MSGSTRUCT*);
// calls xmquit()
// may call closelog()

/* internal functions */
extern int xm_lev2pri(unsigned char*);
extern int xm_negative(int);

#endif

/* some example formats (for future use)
00 standard message
01 explanation
02 user action
03 mnemonics or symbolics
 */


